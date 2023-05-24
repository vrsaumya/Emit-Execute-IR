; ModuleID = 'LLVMDialectModule'
source_filename = "LLVMDialectModule"

declare ptr @malloc(i64)

declare void @free(ptr)

declare void @printNewline()

declare void @printI64(i64)

define void @main() {
  %1 = call i64 @calcFact(i64 12)
  call void @printI64(i64 %1)
  call void @printNewline()
  ret void
}

define i64 @calcFact(i64 %0) {
  br label %2

2:                                                ; preds = %7, %1
  %3 = phi i64 [ %10, %7 ], [ 1, %1 ]
  %4 = phi i64 [ %8, %7 ], [ 1, %1 ]
  %5 = phi i64 [ %9, %7 ], [ 12, %1 ]
  %6 = icmp slt i64 %3, 12
  br i1 %6, label %7, label %11

7:                                                ; preds = %2
  %8 = mul i64 %4, %5
  %9 = sub i64 %5, 1
  %10 = add i64 %3, 1
  br label %2

11:                                               ; preds = %2
  ret i64 %4
}

!llvm.module.flags = !{!0}

!0 = !{i32 2, !"Debug Info Version", i32 3}
