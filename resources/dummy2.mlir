module {
  func.func @main() {
    %c2_i64 = arith.constant 2 : i64
    %c3_i64 = arith.constant 3 : i64
    %0 = arith.addi %c2_i64, %c3_i64 : i64
    return
  }
}