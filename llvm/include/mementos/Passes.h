#ifndef MEMENTOS_PASSES_H
#define MEMENTOS_PASSES_H

#include "llvm/Pass.h"
#include "llvm/Module.h"
#include "llvm/DerivedTypes.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Support/Debug.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Support/raw_ostream.h"

namespace llvm {

  struct MementosSizeGlobals : public ModulePass {
    static char ID;
    TargetData *TD;
    unsigned TotalSizeInBytes;

    MementosSizeGlobals ();
    virtual bool runOnModule (Module &);
    virtual void getAnalysisUsage (AnalysisUsage &) const;
    void print (raw_ostream &, const Module *) const;
  };

  struct MementosAddGlobalSizeGlobal : public ModulePass {
    static char ID;

    MementosAddGlobalSizeGlobal ();
    virtual bool runOnModule (Module &);
    virtual void getAnalysisUsage (AnalysisUsage &) const;
  };

  struct MementosAppendCleanup : public FunctionPass {
    static char ID;
    Function *cleanupFunction;
    static const std::string cleanupFunctionName;

    MementosAppendCleanup ();
    virtual bool doInitialization (Module &);
    virtual bool runOnFunction (Function &);
  };

  struct MementosRenameMain : public FunctionPass {
    static char ID;
    static const std::string oldMainName;

    MementosRenameMain ();
    virtual bool runOnFunction (Function &);
  };

  struct MementosTriggerPointInstrumenter {
    static const std::string triggerPointFunctionName;
    Function *triggerPointFunction;
    Module *containingModule;
    void setModule (Module *);
  };

  struct MementosInstrumentAllLoopLatches :
    public LoopPass, public MementosTriggerPointInstrumenter
  {
    static char ID;
    Function *containingFunction;

    MementosInstrumentAllLoopLatches ();
    virtual bool doInitialization (Loop *, LPPassManager&);
    virtual bool runOnLoop (Loop *, LPPassManager&);
  };

  struct MementosInstrumentAllFunctionReturns :
    public BasicBlockPass, public MementosTriggerPointInstrumenter
  {
    static char ID;

    MementosInstrumentAllFunctionReturns ();
    virtual bool doInitialization (Module &);
    virtual bool doInitialization (Function &);
    virtual bool runOnBasicBlock (BasicBlock &);
  };

  struct MementosWarnings : public BasicBlockPass
  {
    static char ID;

    MementosWarnings ();
    virtual bool runOnBasicBlock (BasicBlock &);
    virtual void getAnalysisUsage (AnalysisUsage &) const;
  };
}

#endif // MEMENTOS_PASSES_H

// vim:ts=2 sw=2
