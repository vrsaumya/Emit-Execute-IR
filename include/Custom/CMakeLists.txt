add_mlir_dialect(CustomOps custom)
add_mlir_doc(CustomDialect CustomDialect Custom/ -gen-dialect-doc)
add_mlir_doc(CustomOps CustomOps Custom/ -gen-op-doc)

set(LLVM_TARGET_DEFINITIONS CustomPasses.td)
mlir_tablegen(CustomPasses.h.inc --gen-pass-decls)
add_public_tablegen_target(MLIRCustomPassesIncGen)
