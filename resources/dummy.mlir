  func.func @main() {
  %1 = arith.constant 2 : i64
  %2 = arith.constant 3 : i64
  %0 = casair.addi %1, %2 : i64 
  vector.print %0 : i64
  return
 }

