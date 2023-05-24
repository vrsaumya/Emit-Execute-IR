#include "mlir/IR/BuiltinDialect.h"
#include "Custom/CustomDialect.h"
#include "Custom/CustomPasses.h"
#include "Custom/CustomOps.h"

#include "mlir/Dialect/Affine/IR/AffineOps.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/MemRef/IR/MemRef.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/DialectConversion.h"
#include "llvm/ADT/Sequence.h"

#include "mlir/Conversion/LLVMCommon/Pattern.h"

#include <iostream>
#include "mlir/Conversion/VectorToLLVM/ConvertVectorToLLVM.h"

#include "mlir/Conversion/LLVMCommon/TypeConverter.h"
#include "mlir/Conversion/LLVMCommon/VectorPattern.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Arith/Utils/Utils.h"
#include "mlir/Dialect/LLVMIR/FunctionCallUtils.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/Dialect/MemRef/IR/MemRef.h"
#include "mlir/Dialect/Vector/Interfaces/MaskableOpInterface.h"
#include "mlir/Dialect/Vector/Transforms/LoweringPatterns.h"
#include "mlir/Dialect/Vector/Transforms/VectorTransforms.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/TypeUtilities.h"
#include "mlir/Target/LLVMIR/TypeToLLVM.h"
#include "mlir/Transforms/DialectConversion.h"
#include "llvm/Support/Casting.h"
#include <optional>
using namespace mlir;
using namespace mlir::custom;

namespace {
template <class OpClass, class StdOpClass>
struct BinaryOpLowering : public OpConversionPattern<OpClass> {
public:
  using OpConversionPattern<OpClass>::OpConversionPattern;
  LogicalResult
  matchAndRewrite(OpClass binOp,
                  typename OpConversionPattern<OpClass>::OpAdaptor adaptor,
                  ConversionPatternRewriter &rewriter) const override {
      //auto type = getBaseType(binOp.left().getType());
      rewriter.template replaceOpWithNewOp<StdOpClass>(
          binOp, adaptor.getLhs(), adaptor.getRhs());
      return success();

  }
};

using AddOpLowering = BinaryOpLowering<custom::AddIOp, arith::AddIOp>;
using SubOpLowering = BinaryOpLowering<custom::SubIOp, arith::SubIOp>;
using MulOpLowering = BinaryOpLowering<custom::MulIOp, arith::MulIOp>;
}
namespace {
struct CustomToStdLoweringPass
    : public PassWrapper<CustomToStdLoweringPass, OperationPass<ModuleOp>> {
  MLIR_DEFINE_EXPLICIT_INTERNAL_INLINE_TYPE_ID(CustomToStdLoweringPass)

  void getDependentDialects(DialectRegistry &registry) const override {
    registry.insert<mlir::arith::ArithDialect, func::FuncDialect, memref::MemRefDialect, vector::VectorDialect>();
  }
  void runOnOperation() final;
};
}

void CustomToStdLoweringPass::runOnOperation() {

  ConversionTarget target(getContext());

  target.addLegalDialect<affine::AffineDialect, BuiltinDialect, arith::ArithDialect,
                         func::FuncDialect, memref::MemRefDialect, vector::VectorDialect>();
  target.addIllegalDialect<custom::CustomDialect>();

  RewritePatternSet patterns(&getContext());
  patterns.add<AddOpLowering>(
      &getContext());

  if (failed(
          applyPartialConversion(getOperation(), target, std::move(patterns))))
    signalPassFailure();
}

std::unique_ptr<Pass> mlir::custom::createLowerToStdPass() {
  return std::make_unique<customToStdLoweringPass>();
}