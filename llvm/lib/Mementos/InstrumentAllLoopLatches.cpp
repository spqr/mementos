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
using namespace llvm;

STATISTIC(NumLoopLatchesInstrumented, "Number of loop latches instrumented");

char MementosInstrumentAllLoopLatches::ID = 0;
static RegisterPass<MementosInstrumentAllLoopLatches>
D("mementos-instrument-all-loop-latches",
    "Adds trigger point calls to loops",
    false, /* does not preserve the CFG */
    false /* is not an analysis pass */);

MementosInstrumentAllLoopLatches::MementosInstrumentAllLoopLatches ()
  : LoopPass(ID) {
}

bool MementosInstrumentAllLoopLatches::doInitialization (Loop *loop,
    LPPassManager &LPM) {
    //                  BasicBlock   Function     Module
  containingFunction = loop->getHeader()->getParent();
  setModule(containingFunction->getParent());

  AttributeSet alp = containingFunction->getAttributes().getFnAttributes();
  DEBUG(outs() << "Containing function " << containingFunction->getName()
      << " attributes: " << alp.getAsString(0) << "\n");

  return (triggerPointFunction != NULL);
}

bool MementosInstrumentAllLoopLatches::runOnLoop (Loop *loop,
    LPPassManager &LPM) {
  if (!triggerPointFunction) return false;

  BasicBlock *targetBlock = NULL;

  // The right place to insert a trigger point is the Loop's backedge a/k/a
  // latch, the basic block that transfers control back to the top of the loop.
  // If this loop has a latch (which it does if its header is in "normal" form,
  // according to LoopBase::getLoopLatch's comment), tack our trigger point
  // there.  Otherwise, tack it into the Loop passed in as loop.
  if (!(targetBlock = loop->getLoopLatch())) {
    targetBlock = loop->getHeader(); // equivalent to getBlocks().front()
  }

  StringRef parentFunctionName = containingFunction->getName();
  DEBUG(outs() << "Parent Function of loop: " << parentFunctionName << "\n");
  if (parentFunctionName.find("__") != StringRef::npos) {
    DEBUG(outs() << "Loop's parent function begins with '__'; " <<
        " leaving this Loop unmolested.\n");
    return false;
  }
  if (parentFunctionName.find(MEMENTOS_MNOTP) != StringRef::npos) {
    DEBUG(outs() << "Loop's parent function has '" << MEMENTOS_MNOTP
        << "' in its name; not adding trigger points to it.\n");
    return false;
  }

  // XXX LoopPasses process loops' basic blocks in "loop nest order", which just
  // means the innermost one is processed first and the outermost one is
  // processed last -- see the "Writing an LLVM Pass" document.  Should we walk
  // up to the outermost loop with getParentLoop()?  Disadvantage: might end up
  // with too-large granularity (trigger points too infrequently).  Advantage:
  // might not end up with too-small granularity.

  TerminatorInst* TI = targetBlock->getTerminator();
  if (!TI) { // Really shouldn't happen
    DEBUG(errs() << "Loop lacks terminator\n");
    return false; // We did not modify the program.
  }
  DEBUG(outs() << "Loop has terminator: " << *TI << "\n");

  // Append the trigger point function
  if (triggerPointFunction) {
    CallInst::Create(triggerPointFunction, "", TI);
  } else {
    DEBUG(errs() << "triggerPointFunction has become null!  No function '" <<
        triggerPointFunctionName << "'?\n");
  }

  // Yay!
  ++NumLoopLatchesInstrumented;

  // Return true because we changed the Loop's code
  return true;
}
