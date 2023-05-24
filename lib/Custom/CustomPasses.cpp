//===- CustomPasses.cpp - Custom passes -----------------*- C++ -*-===//
//
// This file is licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#include <iostream>
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"

#include "Custom/CustomPasses.h"

namespace mlir::custom {
#define GEN_PASS_DEF_CUSTOMSWITCHBARFOO
#include "Custom/CustomPasses.h.inc"
#define GEN_PASS_DEF_CUSTOMFOOTOCONST
#include "Custom/CustomPasses.h.inc"

namespace {
class CustomSwitchBarFooRewriter : public OpRewritePattern<func::FuncOp> {
public:
  using OpRewritePattern<func::FuncOp>::OpRewritePattern;
  LogicalResult matchAndRewrite(func::FuncOp op,
                                PatternRewriter &rewriter) const final {
    if (op.getSymName() == "bar") {
      rewriter.updateRootInPlace(op, [&op]() { op.setSymName("foo"); });
      return success();
    }
    return failure();
  }
};

class CustomSwitchBarFoo
    : public impl::CustomSwitchBarFooBase<CustomSwitchBarFoo> {
public:
  using impl::CustomSwitchBarFooBase<
      CustomSwitchBarFoo>::CustomSwitchBarFooBase;
  void runOnOperation() final {
    RewritePatternSet patterns(&getContext());
    patterns.add<CustomSwitchBarFooRewriter>(&getContext());
    FrozenRewritePatternSet patternSet(std::move(patterns));
    if (failed(applyPatternsAndFoldGreedily(getOperation(), patternSet)))
      signalPassFailure();
  }
};
} // namespace
namespace {
  
 // namespace
 class CustomFooToConstRewriter : public OpRewritePattern<func::FuncOp> {
public:
 using OpRewritePattern<func::FuncOp>::OpRewritePattern;
  LogicalResult matchAndRewrite(func::FuncOp op,
                                PatternRewriter &rewriter) const final {
    if (op.getSymName() == "bar") {
      rewriter.updateRootInPlace(op, [&op]() { op.setSymName("foo"); });
      return success();
    }
    return failure();
  
  }
};

class CustomFooToConst
    : public impl::CustomFooToConstBase<CustomFooToConst> {
public:
  using impl::CustomFooToConstBase<
      CustomFooToConst>::CustomFooToConstBase;
  void runOnOperation() final {
    RewritePatternSet patterns(&getContext());
    patterns.add<CustomFooToConstRewriter>(&getContext());
    FrozenRewritePatternSet patternSet(std::move(patterns));
    if (failed(applyPatternsAndFoldGreedily(getOperation(), patternSet)))
      signalPassFailure();
  }
};
} // namespace
} // namespace mlir::custom
