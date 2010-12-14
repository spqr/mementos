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
  /* silently skip BBs inside Mementos-internal functions or functions the
   * programmer has directed Mementos to ignore */
  StringRef fname = BB.getParent()->getName();
  if ((fname.find(MEMENTOS_FUNCTION_PREFIX) != StringRef::npos) ||
      (fname.find(MEMENTOS_MNOTP) != StringRef::npos))
    return false;
  
  BasicBlock::iterator i, end;
  for (i = BB.begin(), end = BB.end(); i != end; ++i) {
    if (isa<StoreInst>(*i) && cast<StoreInst>(*i).isVolatile()) {
      DEBUG(outs() << "Found a volatile store instruction: " << *i << "\n");
      ProblematicFunctions[fname]++;
      ++NumWarningsIssued;
    }
  }

  return false;
}

void MementosWarnings::getAnalysisUsage (AnalysisUsage &AU) const {
  AU.setPreservesAll();
}

void MementosWarnings::print (raw_ostream &O, const Module *M) const {
  if (! ProblematicFunctions.size())
    return;

  O << "Functions that triggered warnings:\n";
  // iterate over keys of ProblematicFunctions
  for (std::map<StringRef, unsigned>::const_iterator
      I = ProblematicFunctions.begin(), E = ProblematicFunctions.end();
      I != E; ++I) {
    O << " - " << I->first << "\n";
  }
  O << "Consider adding a " << MEMENTOS_MNOTP << " suffix to make Mementos"
    << " not instrument these functions.\n";
}

// vim:ts=2 sw=2
