//===- CustomTypes.h - Custom dialect types -------------*- C++ -*-===//
//
// This file is licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CUSTOM_CUSTOMTYPES_H
#define CUSTOM_CUSTOMTYPES_H

#include "mlir/IR/BuiltinTypes.h"

#define GET_TYPEDEF_CLASSES
#include "Custom/CustomOpsTypes.h.inc"

#endif // CUSTOM_CUSTOMTYPES_H
