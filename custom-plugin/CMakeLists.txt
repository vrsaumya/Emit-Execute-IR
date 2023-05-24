add_llvm_library(CasaIRPlugin
        # BUILDTREE_ONLY is only for testing purposes
        MODULE BUILDTREE_ONLY
        casair-plugin.cpp

        DEPENDS
        MLIRCasaIR
        PLUGIN_TOOL
        mlir-opt

        LINK_LIBS
        MLIRCasaIR
        )
target_include_directories(
        CasaIRPlugin
        PRIVATE
        "${CASAIR_BINARY_DIR}/include"
)
