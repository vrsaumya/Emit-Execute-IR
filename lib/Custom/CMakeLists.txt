add_mlir_dialect_library(MLIRCustom
        CustomTypes.cpp
        CustomDialect.cpp
        CustomOps.cpp
        CustomPasses.cpp
        LowerToStd.cpp
        loadDataForQ6.cpp

        ADDITIONAL_HEADER_DIRS
        ${PROJECT_SOURCE_DIR}/include/Custom

        DEPENDS
        MLIRCustomOpsIncGen
        MLIRCustomPassesIncGen

        LINK_LIBS PUBLIC
        MLIRIR
        MLIRInferTypeOpInterface
        MLIRFuncDialect
        )
