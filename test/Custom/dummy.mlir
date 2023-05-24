// RUN: casair-opt %s | casair-opt | FileCheck %s

module {
    // CHECK-LABEL: func @bar()
    func.func @bar() {
        %0 = arith.constant 1 : i32
        // CHECK: %{{.*}} = casair.foo %{{.*}} : i32
        %res = casair.foo %0 : i32
        return
    }

    // CHECK-LABEL: func @casair_types(%arg0: !casair.custom<"10">)
    func.func @casair_types(%arg0: !casair.custom<"10">) {
        return
    }
}
