#include <Custom/CustomDialect.h>
#include <Custom/CustomOps.h>
#include <Custom/CustomPasses.h>
#include <Custom/CustomTypes.h>

#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorOr.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

// #include "mlir/Dialect/Affine/Passes.h"
#include "mlir/Dialect/LLVMIR/Transforms/Passes.h"
#include "mlir/ExecutionEngine/ExecutionEngine.h"
#include "mlir/ExecutionEngine/OptUtils.h"
#include "mlir/IR/AsmState.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/Verifier.h"
#include "mlir/InitAllDialects.h"
#include "mlir/Parser/Parser.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Pass/PassManager.h"
#include "mlir/Target/LLVMIR/Dialect/Builtin/BuiltinToLLVMIRTranslation.h"
#include "mlir/Target/LLVMIR/Dialect/LLVMIR/LLVMToLLVMIRTranslation.h"
#include "mlir/Target/LLVMIR/Export.h"
#include "mlir/Transforms/Passes.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "llvm/Support/FileUtilities.h"

#include <fstream>
#include <string>
#include <iostream>

using namespace mlir::custom;
namespace cl = llvm::cl;
using namespace mlir;

static cl::opt<std::string> inputFilename(cl::Positional,cl::desc("input mlir file"),cl::init("-"),cl::value_desc("filename"));

enum Action {
    EmitMLIR,
    EmitLLVMIR,
    EmitMLIRCPPAPI,
    EmitQ6
};

static cl::opt<enum Action> emitAction(
    "emit", cl::desc("Choose IR Type:"),
    cl::values(clEnumValN(EmitMLIR, "mlir", "Emit MLIR"),
               clEnumValN(EmitLLVMIR, "llvmir", "Emit LLVM IR"),
               clEnumValN(EmitMLIRCPPAPI, "cppapi", "Emit MLIR using c++ apis"),
               clEnumValN(EmitQ6, "q6", "Emit MLIR for tpch q6")
               ));

static cl::opt<std::string> outputFilename("o", cl::desc("Output Filename"), cl::value_desc("filename"), cl::init("-"));


 

int loadAndProcessMLIR(mlir::MLIRContext &context, mlir::OwningOpRef<mlir::ModuleOp> &module){
    llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> fileOrErr = llvm::MemoryBuffer::getFileOrSTDIN(inputFilename);
    if (std::error_code ec = fileOrErr.getError()) {
        llvm::errs() << "Could not open input file: " << ec.message() << "\n";
        return -1;
    }
    llvm::SourceMgr sourceMgr;
    sourceMgr.AddNewSourceBuffer(std::move(*fileOrErr), llvm::SMLoc());
    module = mlir::parseSourceFile<mlir::ModuleOp>(sourceMgr, &context);
    return 0;
}

int dumpLLVMIR(mlir::MLIRContext &context, mlir::OwningOpRef<mlir::ModuleOp> &module) {
     mlir::PassManager pm(module.get()->getName());
     if (mlir::failed(mlir::applyPassManagerCLOptions(pm)))
        return 4;
     pm.addPass(mlir::custom::createLowerToStdPass());
     pm.addNestedPass<mlir::LLVM::LLVMFuncOp>(mlir::LLVM::createDIScopeForLLVMFuncOpPass());
     if (mlir::failed(pm.run(*module)))
        return 4;
     return 0;
}

int generateMLIR(mlir::MLIRContext &context, mlir::OwningOpRef<mlir::ModuleOp> &module, Location loc){
    OpBuilder builder(&context);

    auto funcType = builder.getFunctionType(std::nullopt, std::nullopt);
    auto funcn = builder.create<func::FuncOp>(loc, "main", funcType);

    auto factFuncType = builder.getFunctionType(builder.getI64Type(), builder.getI64Type());
    auto factFunc = builder.create<func::FuncOp>(loc, "calcFact", factFuncType);
    factFunc.setPrivate();

    Block *entryBlock = funcn.addEntryBlock();
    builder.setInsertionPointToEnd(entryBlock);
    auto var = builder.create<arith::ConstantOp>(loc, IntegerType::get(&context, 64), builder.getI64IntegerAttr(12));
    auto ans = builder.create<func::CallOp>(loc,factFunc, ValueRange{var});
    builder.create<vector::PrintOp>(loc, ans.getResult(0));
    builder.create<func::ReturnOp>(loc);

    Block *entryBlock1 = factFunc.addEntryBlock();
    builder.setInsertionPointToEnd(entryBlock1);
    auto num0 = builder.create<arith::ConstantOp>(loc, IntegerType::get(&context, 64), builder.getI64IntegerAttr(1));
    auto num1 = builder.create<arith::ConstantOp>(loc, IntegerType::get(&context, 64),var.getResult());
    auto fact1 = builder.create<arith::ConstantOp>(loc, IntegerType::get(&context, 64), builder.getI64IntegerAttr(1));
    auto ub = builder.create<arith::ConstantOp>(loc, IntegerType::get(&context, 64), var.getResult());
    auto lb = builder.create<arith::ConstantOp>(loc, IntegerType::get(&context, 64), builder.getI64IntegerAttr(1));
    auto step = builder.create<arith::ConstantOp>(loc, IntegerType::get(&context, 64), builder.getI64IntegerAttr(1));
    auto loop = builder.create<scf::ForOp>(loc, lb, ub, step, ValueRange{fact1, num1}, 
                    [&](OpBuilder &b, Location loc, Value iv, ValueRange args){
                        Value afact1 = args[0];
                        Value num1_next = args[1];
                        Value next = b.create<custom::MulIOp>(loc, afact1, num1_next);
                        Value dec = b.create<custom::SubIOp>(loc, num1_next, num0);
                        b.create<scf::YieldOp>(loc, ValueRange{next, dec});

                    });
    builder.create<func::ReturnOp>(loc, ValueRange{loop.getResult(0)});

    module->push_back(funcn);
    module->push_back(factFunc);
    return 0;
}  

int generateMLIRForQ6(mlir::MLIRContext &context, mlir::OwningOpRef<mlir::ModuleOp> &module, Location loc){
    OpBuilder builder(&context);

    auto funcType = builder.getFunctionType(std::nullopt, std::nullopt);
    auto funcn = builder.create<func::FuncOp>(loc, "main", funcType);

    Type elementType = builder.getF64Type();
    auto memTp = MemRefType::get({100, 3}, elementType);
    auto memTp2 = MemRefType::get({100}, builder.getI64Type());
    auto loadFuncnType = builder.getFunctionType({memTp, memTp2}, builder.getI64Type());
    auto loadFunc = builder.create<func::FuncOp>(loc, "loadData_q6", loadFuncnType);
    loadFunc.setPrivate();

    // auto memTp2 = MemRefType::get({10, 3}, elementType);
    UnrankedMemRefType castMemrefType = UnrankedMemRefType::get(memTp.getElementType(), /*memorySpace=*/0);
    auto printMemRefFuncnType = builder.getFunctionType(castMemrefType, std::nullopt);
    auto printMemRefFunc = builder.create<func::FuncOp>(loc, "printMemrefF64", printMemRefFuncnType);
    printMemRefFunc.setPrivate();

    Block *entryBlock = funcn.addEntryBlock();
    // Region *funcBody = entryBlock->getParent();
    builder.setInsertionPointToEnd(entryBlock);

    Value mem = builder.create<memref::AllocOp>(loc, memTp);
    Value mem2 = builder.create<memref::AllocOp>(loc, memTp2);
    // Value mem2 = builder.create<memref::AllocOp>(loc, memTp2);

    auto x = builder.create<func::CallOp>(loc,loadFunc, ValueRange{mem, mem2});

    auto qnty = builder.create<arith::ConstantOp>(loc, builder.getIndexAttr(0));
    auto disc = builder.create<arith::ConstantOp>(loc,  builder.getIndexAttr(1));
    auto extp = builder.create<arith::ConstantOp>(loc,  builder.getIndexAttr(2));

    Value qnty_filter_cond = builder.create<arith::ConstantOp>(loc, FloatType::getF64(&context), builder.getF64FloatAttr(20.0));
    Value disc_lt_cond = builder.create<arith::ConstantOp>(loc, FloatType::getF64(&context), builder.getF64FloatAttr(0.10));
    Value disc_gt_cond = builder.create<arith::ConstantOp>(loc, FloatType::getF64(&context), builder.getF64FloatAttr(0.05));
    
    auto fact1 = builder.create<arith::ConstantOp>(loc, IntegerType::get(&context, 64), builder.getI64IntegerAttr(1));
    auto lower_bound_itr = builder.create<arith::ConstantOp>(loc,  builder.getIndexAttr(0));
    auto step = builder.create<arith::ConstantOp>(loc, builder.getIndexAttr(1));
    auto upper_bound_itr = builder.create<arith::ConstantOp>(loc,  builder.getIndexAttr(99));

    // auto l_shpd = builder.create<memref::LoadOp>(loc, mem2, ValueRange{step});
    // auto b = builder.create<memref::LoadOp>(loc, mem2, ValueRange{lower_bound_itr});
    //  builder.create<memref::LoadOp>(loc, mem2, ValueRange{fact1});

    auto loop = builder.create<scf::ForOp>(loc, lower_bound_itr, upper_bound_itr, step, ValueRange{}, 
                    [&](OpBuilder &b, Location loc, Value iv, ValueRange args){
            
                        Value l_qty = b.create<memref::LoadOp>(loc, mem, ValueRange{iv, qnty});
                        auto l_disc = b.create<memref::LoadOp>(loc, mem, ValueRange{iv, disc});
                        auto l_ext_price = b.create<memref::LoadOp>(loc, mem, ValueRange{iv, extp});
                        // auto l_shpd = b.create<memref::LoadOp>(loc, mem2, ValueRange{iv});

                        Value l_qty_filter = b.create<arith::CmpFOp>(loc, arith::CmpFPredicate::OLT, l_qty, qnty_filter_cond);
                        Value l_disc_lt_filter = b.create<arith::CmpFOp>(loc, arith::CmpFPredicate::ULT, l_disc, disc_lt_cond);
                        Value l_disc_gt_filter = b.create<arith::CmpFOp>(loc, arith::CmpFPredicate::UGT, l_disc, disc_gt_cond);

                        scf::IfOp ifOp = b.create<scf::IfOp>(loc, l_qty_filter,
                                [&](OpBuilder &b, Location loc){
                                     scf::IfOp ifOp = b.create<scf::IfOp>(loc, l_disc_lt_filter,
                                        [&](OpBuilder &b, Location loc){
                                            scf::IfOp ifOp = b.create<scf::IfOp>(loc, l_disc_gt_filter,
                                                [&](OpBuilder &b, Location loc){
                                                    b.create<vector::PrintOp>(loc, l_qty);
                                                    b.create<vector::PrintOp>(loc, l_disc);
                                                    b.create<vector::PrintOp>(loc, l_ext_price);
                                                    // b.create<vector::PrintOp>(loc, l_shpd);
                                                    b.create<scf::YieldOp>(loc);
                                                },
                                                [&](OpBuilder &b, Location loc) {
                                                    b.create<scf::YieldOp>(loc);
                                                }
                                            );
                                            b.create<scf::YieldOp>(loc);
                                        },
                                        [&](OpBuilder &b, Location loc) {
                                            b.create<scf::YieldOp>(loc);
                                        }
                                    );
                                    b.create<scf::YieldOp>(loc);
                                },
                                [&](OpBuilder &b, Location loc) {
                                    b.create<scf::YieldOp>(loc);
                                }
                        );
                        b.create<scf::YieldOp>(loc, ValueRange{});
                    });
    //  Value castedMem = builder.create<memref::CastOp>(loc, castMemrefType, mem2);

    // auto printFuncCall = builder.create<func::CallOp>(loc, printMemRefFunc, castedMem);

    // builder.create<vector::PrintOp>(loc, l_shpd);
    // builder.create<vector::PrintOp>(loc, b);
    builder.create<func::ReturnOp>(loc);

    module->push_back(funcn);
    module->push_back(loadFunc);
    module->push_back(printMemRefFunc);
    return 0;
}  

int main(int argc, char **argv){
    mlir::registerAsmPrinterCLOptions();
    mlir::registerMLIRContextCLOptions();
    mlir::registerPassManagerCLOptions();
    
    cl::ParseCommandLineOptions(argc, argv, "Custom compiler\n");


    mlir::MLIRContext context;
    context.getOrLoadDialect<mlir::arith::ArithDialect>();
    context.getOrLoadDialect<mlir::func::FuncDialect>();
    context.getOrLoadDialect<mlir::custom::CustomDialect>();
    context.getOrLoadDialect<mlir::vector::VectorDialect>();
    context.getOrLoadDialect<mlir::scf::SCFDialect>();
    context.getOrLoadDialect<mlir::memref::MemRefDialect>();
    auto loc = UnknownLoc::get(&context);
    mlir::OwningOpRef<mlir::ModuleOp> module(mlir::ModuleOp::create(loc));
    
   
    if (emitAction == Action::EmitMLIRCPPAPI){

        generateMLIR(context, module, loc);
        module->dump();

        return 0;
    }

    // The following two Ifs are parsing an mlir file only. Keeping it as placeholder for future use of the code.
    // Parsing from sql or physical plan to mlir. 
    if(int error = loadAndProcessMLIR(context, module)){
        return error;
    }

    if (emitAction == Action::EmitMLIR){
        module->dump();
        return 0;
    }

    // Emit LLVM Dialect
    if (emitAction == Action::EmitLLVMIR){
        if(int error = dumpLLVMIR(context, module)){
            return error;
        } 
        module->dump();
        return 0;
    }

    llvm::errs() << "Please specify an action next time!";
    return -1;
}