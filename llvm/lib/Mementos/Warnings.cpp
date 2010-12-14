#include "mementos/Mementos.h"
#include "mementos/Passes.h"
#include "llvm/Instructions.h"
using namespace llvm;

STATISTIC(NumWarningsIssued, "Number of warnings issued");

char MementosWarnings::ID = 0;
static RegisterPass<MementosWarnings>
C("mementos-warnings",
    "Warns about checkpointing hazards",
    true, /* preserves the CFG */
    true /* is an analysis pass */);

MementosWarnings::MementosWarnings ()
  : BasicBlockPass(ID) {
}

bool MementosWarnings::runOnBasicBlock (BasicBlock &BB) {
  return false;
}

// vim:ts=2 sw=2
