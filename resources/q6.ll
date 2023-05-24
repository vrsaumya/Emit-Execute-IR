; ModuleID = 'LLVMDialectModule'
source_filename = "LLVMDialectModule"

declare ptr @malloc(i64)

declare void @free(ptr)

declare void @printNewline()

declare void @printF64(double)

define void @main() {
  %1 = call ptr @malloc(i64 ptrtoint (ptr getelementptr (double, ptr null, i32 300) to i64))
  %2 = call ptr @malloc(i64 ptrtoint (ptr getelementptr (i64, ptr null, i32 100) to i64))
  %3 = call i64 @loadData_q6(ptr %1, ptr %1, i64 0, i64 100, i64 3, i64 3, i64 1, ptr %2, ptr %2, i64 0, i64 100, i64 1)
  br label %4

4:                                                ; preds = %29, %0
  %5 = phi i64 [ %30, %29 ], [ 0, %0 ]
  %6 = icmp slt i64 %5, 99
  br i1 %6, label %7, label %31

7:                                                ; preds = %4
  %8 = mul i64 %5, 3
  %9 = add i64 %8, 0
  %10 = getelementptr double, ptr %1, i64 %9
  %11 = load double, ptr %10, align 8
  %12 = mul i64 %5, 3
  %13 = add i64 %12, 1
  %14 = getelementptr double, ptr %1, i64 %13
  %15 = load double, ptr %14, align 8
  %16 = mul i64 %5, 3
  %17 = add i64 %16, 2
  %18 = getelementptr double, ptr %1, i64 %17
  %19 = load double, ptr %18, align 8
  %20 = fcmp olt double %11, 2.000000e+01
  %21 = fcmp ult double %15, 1.000000e-01
  %22 = fcmp ugt double %15, 5.000000e-02
  br i1 %20, label %23, label %28

23:                                               ; preds = %7
  br i1 %21, label %24, label %27

24:                                               ; preds = %23
  br i1 %22, label %25, label %26

25:                                               ; preds = %24
  call void @printF64(double %11)
  call void @printNewline()
  call void @printF64(double %15)
  call void @printNewline()
  call void @printF64(double %19)
  call void @printNewline()
  br label %27

26:                                               ; preds = %24
  br label %27

27:                                               ; preds = %25, %26, %23
  br label %28

28:                                               ; preds = %27, %7
  br label %29

29:                                               ; preds = %28
  %30 = add i64 %5, 1
  br label %4

31:                                               ; preds = %4
  ret void
}

declare i64 @loadData_q6(ptr, ptr, i64, i64, i64, i64, i64, ptr, ptr, i64, i64, i64)

declare void @printMemrefF64(i64, ptr)

!llvm.module.flags = !{!0}

!0 = !{i32 2, !"Debug Info Version", i32 3}
