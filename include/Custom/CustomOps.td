//===- CasaIROps.td - CasaIR dialect ops -----------*- tablegen -*-===//
//
// This file is licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CASAIR_OPS
#define CASAIR_OPS

include "CasaIR/CasaIRTypes.td"
include "mlir/Interfaces/InferTypeOpInterface.td"
include "mlir/Interfaces/SideEffectInterfaces.td"

def CasaIR_FooOp : CasaIR_Op<"foo", [Pure, Commutative]> {
    let summary = "Illustrates how to define an operation.";
    let description = [{
        The `casair.foo` operation illustrates how to define a new
        operation in a dialect. It uses an operation trait to declare that it
        has no side effects.

        This operation takes an integer argument and returns an integer.

        Example:

        ```mlir
        %0 = custom.constant 2 : i32
        // Apply the foo operation to %0
        %1 = casair.foo %0 : i32
        ```
    }];

    let arguments = (ins I64:$input);
    let results = (outs I64:$res);

    let assemblyFormat = [{
        $input attr-dict `:` type($input)
    }];
}

def Custom_AddIOp : Custom_Op<"addi", [Commutative]> {
  let summary = "integer addition operation";
  let description = [{
    The `addi` operation takes two operands and returns one result, each of
    these is required to be the same type. This type may be an integer scalar
    type, a vector whose element type is integer, or a tensor of integers. It
    has no standard attributes.

    Example:

    ```mlir
    // Scalar addition.
    %a = custom .addi %b, %c : i32
    ```
  }];

  let arguments = (ins I64:$lhs, I64:$rhs);
  let results = (outs I64:$res);

   let assemblyFormat = "$lhs `,` $rhs attr-dict `:` type($res)";
}

def Custom_SubIOp : Custom_Op<"subi", [Commutative]> {
  let summary = "integer subtraction operation";
  let description = [{
    The `subi` operation takes two operands and returns one result, each of
    these is required to be the same type.

    Example:

    ```mlir
    // Scalar addition.
    %a = custom.subi %b, %c : i64
    ```
  }];

  let arguments = (ins I64:$lhs, I64:$rhs);
  let results = (outs I64:$res);

   let assemblyFormat = "$lhs `,` $rhs attr-dict `:` type($res)";
}

//===----------------------------------------------------------------------===//
// MulIOp
//===----------------------------------------------------------------------===//

def Custom_MulIOp : Custom_Op<"muli", [Commutative]> {
  let summary = "integer multiplication operation";
  let description = [{
    The `muli` operation takes two operands and returns one result, each of
    these is required to be the same type.

    Example:

    ```mlir
    // Scalar multiplication.
    %a = custom.muli %b, %c : i64
    ```
  }];

  let arguments = (ins I64:$lhs, I64:$rhs);
  let results = (outs I64:$res);

   let assemblyFormat = "$lhs `,` $rhs attr-dict `:` type($res)";
}

//----Square Root ----//
def CasaIR_SqrtIOp : CasaIR_Op<"sqrt", [Commutative]> {
  let summary = "integer multiplication operation";
  let description = [{
    The `muli` operation takes two operands and returns one result, each of
    these is required to be the same type.

    Example:

    ```mlir
    // Scalar multiplication.
    %a = casair.sqrt %b : i64
    ```
  }];

  let arguments = (ins I64:$lhs);
  let results = (outs I64:$res);

   let assemblyFormat = "$lhs attr-dict `:` type($res)";
}

//===----------------------------------------------------------------------===//
// DivIOp
//===----------------------------------------------------------------------===//

def CasaIR_DivIOp : CasaIR_Op<"divi", [Commutative]> {
  let summary = "integer division operation";
  let description = [{
    The `divi` operation takes two operands and returns one result, each of
    these is required to be the same type.

    Example:

    ```mlir
    // Scalar division.
    %a = casair.divi %b, %c : i64
    ```
  }];

  let arguments = (ins I64:$lhs, I64:$rhs);
  let results = (outs I64:$res);

   let assemblyFormat = "$lhs `,` $rhs attr-dict `:` type($res)";
}

//===----------------------------------------------------------------------===//
// ModIOp
//===----------------------------------------------------------------------===//

def CasaIR_ModIOp : CasaIR_Op<"modi", [Commutative]> {
  let summary = "integer modulo operation";
  let description = [{
    The `modi` operation takes two operands and returns one result, each of
    these is required to be the same type.

    Example:

    ```mlir
    // Scalar division.
    %a = casair.modi %b, %c : i64
    ```
  }];

  let arguments = (ins I64:$lhs, I64:$rhs);
  let results = (outs I64:$res);

   let assemblyFormat = "$lhs `,` $rhs attr-dict `:` type($res)";
}

//===----------------------------------------------------------------------===//
// MaxIOp
//===----------------------------------------------------------------------===//

def CasaIR_MaxIOp : CasaIR_Op<"maxi", [Commutative]> {
  let summary = "integer max operation";
  let description = [{
    The `maxi` operation takes two operands and returns one result, each of
    these is required to be the same type. 

    Example:

    ```mlir
    // Scalar division.
    %a = casair.maxi %b, %c : i64
    ```
  }];

  let arguments = (ins I64:$lhs, I64:$rhs);
  let results = (outs I64:$res);

   let assemblyFormat = "$lhs `,` $rhs attr-dict `:` type($res)";
}

//===----------------------------------------------------------------------===//
// MinIOp
//===----------------------------------------------------------------------===//

def CasaIR_MinIOp : CasaIR_Op<"mini", [Commutative]> {
  let summary = "integer min operation";
  let description = [{
    The `addi` operation takes two operands and returns one result, each of
    these is required to be the same type.

    Example:

    ```mlir
    // Scalar division.
    %a = casair.mini %b, %c : i64
    ```
  }];

  let arguments = (ins I64:$lhs, I64:$rhs);
  let results = (outs I64:$res);

   let assemblyFormat = "$lhs `,` $rhs attr-dict `:` type($res)";
}

def CasaIR_PrintOp : CasaIR_Op<"print"> {
     let summary = "print operation";
  let description = [{
    The "print" builtin operation prints a given input tensor, and produces
    no results.
  }];

  // The print operation takes an input tensor to print.
  // We also allow a F64MemRef to enable interop during partial lowering.
  let arguments = (ins I64:$input);

  let assemblyFormat = "$input attr-dict `:` type($input)";
}
#endif // CASAIR_OPS
