module attributes {llvm.data_layout = ""} {
  llvm.func @printNewline()
  llvm.func @printI64(i64)
  llvm.func @main() {
    %0 = llvm.mlir.constant(5 : i32) : i32
    %1 = arith.extsi %0 : i32 to i64
    llvm.call @printI64(%1) : (i64) -> ()
    llvm.call @printNewline() : () -> ()
    llvm.return
  }
}

