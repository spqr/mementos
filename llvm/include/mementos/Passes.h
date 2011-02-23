/*
Copyright (c) 2011, University of Massachusetts Amherst
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
 3. Neither the name of the University of Massachusetts Amherst nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written
    permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
#include <map>
#include <string>

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
    std::map<StringRef, unsigned> ProblematicFunctions;

    MementosWarnings ();
    virtual bool runOnBasicBlock (BasicBlock &);
    virtual void getAnalysisUsage (AnalysisUsage &) const;
    void print (raw_ostream &, const Module *) const;
  };

}

#endif // MEMENTOS_PASSES_H

// vim:ts=2 sw=2
