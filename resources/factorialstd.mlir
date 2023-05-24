module {
  func.func @main() {
    %c23_i64 = arith.constant 23 : i64
    %0 = call @calcFact(%c23_i64) : (i64) -> i64
    vector.print %0 : i64
    return
  }
  func.func private @calcFact(%arg0: i64) -> i64 {
    %c1_i64 = arith.constant 1 : i64
    %c23_i64 = arith.constant 12 : i64
    %c1_i64_0 = arith.constant 1 : i64
    %c23_i64_1 = arith.constant 12 : i64
    %c1_i64_2 = arith.constant 1 : i64
    %c1_i64_3 = arith.constant 1 : i64
    %0:2 = scf.for %arg1 = %c1_i64_2 to %c23_i64_1 step %c1_i64_3 
          iter_args(%arg2 = %c1_i64_0, %arg3 = %c23_i64) -> (i64, i64)  : i64 {
      %1 = arith.muli %arg2, %arg3 : i64
      %2 = arith.subi %arg3, %c1_i64 : i64
      scf.yield %1, %2 : i64, i64
    }
    return %0#0 : i64
  }
}
