//===- CustomDialect.cpp - Custom dialect ---------------*- C++ -*-===//
//
// This file is licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Custom/CustomDialect.h"
#include "Custom/CustomOps.h"
#include "Custom/CustomTypes.h"

using namespace mlir;
using namespace mlir::custom;

#include "Custom/CustomOpsDialect.cpp.inc"

//===----------------------------------------------------------------------===//
// Custom dialect.
//===----------------------------------------------------------------------===//

void CustomDialect::initialize() {
  addOperations<
#define GET_OP_LIST
#include "Custom/CustomOps.cpp.inc"
      >();
  registerTypes();
}
