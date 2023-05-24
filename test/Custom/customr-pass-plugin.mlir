// RUN: mlir-opt %s --load-pass-plugin=%custom_libs/CustomPlugin%shlibext --pass-pipeline="builtin.module(standalone-switch-bar-foo)" | FileCheck %s

module {
  // CHECK-LABEL: func @foo()
  func.func @bar() {
    return
  }

  // CHECK-LABEL: func @abar()
  func.func @abar() {
    return
  }
}
