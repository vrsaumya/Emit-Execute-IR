module {
  func.func @main() {
    %alloc = memref.alloc() : memref<100x3xf64>
    %alloc_0 = memref.alloc() : memref<100xi64>
    %0 = call @loadData_q6(%alloc, %alloc_0) : (memref<100x3xf64>, memref<100xi64>) -> i64
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %c2 = arith.constant 2 : index
    %cst = arith.constant 2.000000e+01 : f64
    %cst_1 = arith.constant 1.000000e-01 : f64
    %cst_2 = arith.constant 5.000000e-02 : f64
    %c1_i64 = arith.constant 1 : i64
    %c0_3 = arith.constant 0 : index
    %c1_4 = arith.constant 1 : index
    %c99 = arith.constant 99 : index
    scf.for %arg0 = %c0_3 to %c99 step %c1_4 {
      %1 = memref.load %alloc[%arg0, %c0] : memref<100x3xf64>
      %2 = memref.load %alloc[%arg0, %c1] : memref<100x3xf64>
      %3 = memref.load %alloc[%arg0, %c2] : memref<100x3xf64>
      %4 = arith.cmpf olt, %1, %cst : f64
      %5 = arith.cmpf ult, %2, %cst_1 : f64
      %6 = arith.cmpf ugt, %2, %cst_2 : f64
      scf.if %4 {
        scf.if %5 {
          scf.if %6 {
            vector.print %1 : f64
            vector.print %2 : f64
            vector.print %3 : f64
          } else {
          }
        } else {
        }
      } else {
      }
    }
    return
  }
  func.func private @loadData_q6(memref<100x3xf64>, memref<100xi64>) -> i64
  func.func private @printMemrefF64(memref<*xf64>)
}
