get_property(dialect_libs GLOBAL PROPERTY MLIR_DIALECT_LIBS)
get_property(conversion_libs GLOBAL PROPERTY MLIR_CONVERSION_LIBS)
set(LIBS
        ${dialect_libs}
        ${conversion_libs}
        MLIRArithDialect
        MLIROptLib
        MLIRCustom
        )
add_llvm_executable(custom-tool custom-tool.cpp)

llvm_update_compile_flags(custom-tool)
target_link_libraries(custom-tool PRIVATE ${LIBS})

mlir_check_all_link_libraries(custom-tool)
