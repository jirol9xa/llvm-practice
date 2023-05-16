#include "llvm/ADT/Hashing.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Utils/CallGraph.h"
#include <string>
#include <sys/stat.h>
#include <system_error>
#include <unordered_map>
#include <unordered_set>

using namespace llvm;

#define DEBUG_TYPE "callGraph"

namespace {
  StringRef LoggerFuncName = "_Z6Loggerv";

  FunctionType *getLoggerFuncType(Module &M);
  FunctionCallee getCallLogFunc(Module &M);
  void createDummyLogger(Module &M, IRBuilder<> &Builder);
}

PreservedAnalyses CallGraphPass::run(Module &M, ModuleAnalysisManager &AM) {
    // We don't instrument Logger file
    if (M.getName().count("Logger.ll"))
        return PreservedAnalyses::all();

    // Prepare builder for IR modification
    LLVMContext &Ctx = M.getContext();
    IRBuilder<> Builder(Ctx);

    createDummyLogger(M, Builder);
    FunctionCallee CallLogFunc = getCallLogFunc(M);
 
    // Traverse all Functions
    for (Function &F : M.functions()) {
      if (F.empty())
        continue;
      StringRef FuncName = F.getName();
      if (FuncName == LoggerFuncName || FuncName == "main" ||
          FuncName.count("_GLOBAL_") || FuncName.count("global_var"))
        continue;

      Builder.SetInsertPointPastAllocas(&F);
      Builder.CreateCall(CallLogFunc);
    }

    // Our pass should not stop optimisation at all
    return PreservedAnalyses::all();
}

namespace {
FunctionType *getLoggerFuncType(Module &M) {
  Type *RetType = Type::getVoidTy(M.getContext());
  return FunctionType::get(RetType, false);
}

FunctionCallee getCallLogFunc(Module &M) {
  return (M.getOrInsertFunction(LoggerFuncName, getLoggerFuncType(M)));
}

void createDummyLogger(Module &M, IRBuilder<> &Builder) {
  // Use LinkOnceAnyLinkage linkage typem because we need to merge dummyLogger
  // with the actual one, when profiling. LinkOnce is better than weak in case
  // of profiing .cxx file with only global variables, because optimizer
  // can just cut off our dummy definition.
  Function *DummyLogger = Function::Create(
      getLoggerFuncType(M), Function::LinkOnceAnyLinkage, LoggerFuncName, M);

  BasicBlock *BB = BasicBlock::Create(M.getContext(), "entry", DummyLogger);
  Builder.SetInsertPoint(BB);
  Builder.CreateRetVoid();
}
}
