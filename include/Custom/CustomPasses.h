//===- CustomPasses.h - Custom passes  ------------------*- C++ -*-===//
//
// This file is licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#ifndef CUSTOM_CUSTOMPASSES_H
#define CUSTOM_CUSTOMPASSES_H

#include "Custom/CustomDialect.h"
#include "Custom/CustomOps.h"
#include "mlir/Pass/Pass.h"
#include <memory>

namespace mlir {
    class Pass;

namespace custom {
#define GEN_PASS_DECL
#include "Custom/CustomPasses.h.inc"
std::unique_ptr<mlir::Pass> createLowerToStdPass();

#define GEN_PASS_REGISTRATION
#include "Custom/CustomPasses.h.inc"
} // namespace standalone
} // namespace mlir

#endif
