//===- CallGraph.h ----------------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_UTILS_CALLGRAPH_H
#define LLVM_TRANSFORMS_UTILS_CALLGRAPH_H

#include "llvm/IR/PassManager.h"

namespace llvm {
class Module;

class CallGraphPass : public PassInfoMixin<CallGraphPass> {
public:
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM);
};

}

#endif
