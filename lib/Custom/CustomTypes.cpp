//===- CustomTypes.cpp - Custom dialect types -----------*- C++ -*-===//
//
// This file is licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Custom/CustomTypes.h"

#include "Custom/CustomDialect.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/DialectImplementation.h"
#include "llvm/ADT/TypeSwitch.h"

using namespace mlir::custom;

#define GET_TYPEDEF_CLASSES
#include "Custom/CustomOpsTypes.cpp.inc"

void CustomDialect::registerTypes() {
  addTypes<
#define GET_TYPEDEF_LIST
#include "Custom/CustomOpsTypes.cpp.inc"
      >();
}
