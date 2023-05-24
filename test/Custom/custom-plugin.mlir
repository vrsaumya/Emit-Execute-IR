// RUN: mlir-opt %s --load-dialect-plugin=%custom_libs/CustomPlugin%shlibext --pass-pipeline="builtin.module(custom-switch-bar-foo)" | FileCheck %s

module {
  // CHECK-LABEL: func @foo()
  func.func @bar() {
    return
  }

  // CHECK-LABEL: func @custom_types(%arg0: !custom.custom<"10">)
  func.func @custom_types(%arg0: !custom.custom<"10">) {
    return
  }
}
