//===- CUSTOMDialect.td - CUSTOM dialect -----------*- tablegen -*-===//
//
// This file is licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CUSTOM_DIALECT
#define CUSTOM_DIALECT

include "mlir/IR/OpBase.td"

//===----------------------------------------------------------------------===//
// Custom dialect definition.
//===----------------------------------------------------------------------===//

def Custom_Dialect : Dialect {
    let name = "custom";
    let summary = "A custom out-of-tree MLIR dialect.";
    let description = [{
        This dialect is an example of an out-of-tree MLIR dialect designed to
        illustrate the basic setup required to develop MLIR-based tools without
        working inside of the LLVM source tree.
    }];
    let cppNamespace = "::mlir::custom";

    let useDefaultTypePrinterParser = 1;
    let extraClassDeclaration = [{
        void registerTypes();
    }];
}

//===----------------------------------------------------------------------===//
// Base custom operation definition.
//===----------------------------------------------------------------------===//

class Custom_Op<string mnemonic, list<Trait> traits = []> :
        Op<Custom_Dialect, mnemonic, traits>;

#endif // CUSTOM_DIALECT
