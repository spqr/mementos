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

#include "mementos/Mementos.h"
#include "mementos/Passes.h"
#include "llvm/Instructions.h"
#include <string>
using namespace llvm;

char MementosAppendCleanup::ID = 0;
static RegisterPass<MementosAppendCleanup> B("mementos-append-cleanup",
    "adds post-main() Mementos cleanup",
    false, /* does not preserve the CFG */
    false /* is not an analysis pass */);

const std::string MementosAppendCleanup::cleanupFunctionName =
    MEMENTOS_FUNCTION_PREFIX "cleanup";

MementosAppendCleanup::MementosAppendCleanup () : FunctionPass(ID) {
}

bool MementosAppendCleanup::doInitialization (Module &M) {
    cleanupFunction = M.getFunction(cleanupFunctionName);
    if (!cleanupFunction)
      return false;

    return true;
}

bool MementosAppendCleanup::runOnFunction (Function &F) {
  DEBUG(outs() << "Parent module of function: " <<
      F.getParent()->getModuleIdentifier() << "\n");
  if (F.getName() != MEMENTOS_OLDMAINNAME)
    return false;

  // replace F's terminator with a call to __mementos_cleanup(), which does
  // stuff related to checkpoint maintenance
  BasicBlock &bb = F.getBasicBlockList().back();

  // Append a call to the cleanup function in place of the old terminator
  if (cleanupFunction) {
    TerminatorInst* ti = bb.getTerminator();
    DEBUG(outs() << "Will append before this terminator: " << *ti << "\n");
    CallInst::Create(cleanupFunction, "", ti);
  } else {
    DEBUG(errs() << "cleanupFunction is null!  No function '" <<
        cleanupFunctionName << "'?\n");
    return false;
  }

  return true;
}

// vim:ts=2 sw=2
