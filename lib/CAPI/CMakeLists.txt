add_mlir_public_c_api_library(CustomCAPI
  Dialects.cpp
  LINK_LIBS PUBLIC
  MLIRCustom
)
