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

  Attributes alp = containingFunction->getAttributes().getFnAttributes();
  DEBUG(outs() << "Containing function " << containingFunction->getName()
      << " attributes: " << Attribute::getAsString(alp) << "\n");

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
