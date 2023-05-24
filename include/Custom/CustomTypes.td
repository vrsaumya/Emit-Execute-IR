//===- CustomTypes.td - Custom dialect types -------*- tablegen -*-===//
//
// This file is licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CUSTOM_TYPES
#define CUSTOM_TYPES

include "mlir/IR/AttrTypeBase.td"
include "Custom/CustomDialect.td"

//===----------------------------------------------------------------------===//
// Custom type definitions
//===----------------------------------------------------------------------===//

class Custom_Type<string name, string typeMnemonic, list<Trait> traits = []>
    : TypeDef<Custom_Dialect, name, traits> {
  let mnemonic = typeMnemonic;
}

def Custom_CustomType : Custom_Type<"Custom", "custom"> {
    let summary = "Custom custom type";
    let description = "Custom type in custom dialect";
    let parameters = (ins StringRefParameter<"the custom value">:$value);
    let assemblyFormat = "`<` $value `>`";
}

#endif // CUSTOM_TYPES
