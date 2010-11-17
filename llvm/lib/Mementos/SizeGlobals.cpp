//===-- SizeGlobals.cpp - Counts number of bytes in globals ---------------===//
//===----------------------------------------------------------------------===//
//
// Counts the number of bytes required for globals (and therefore in need of
// checkpointing).
//
//===----------------------------------------------------------------------===//

#include "mementos/Mementos.h"
#include "mementos/Passes.h"
using namespace llvm;

STATISTIC(NumBytesRequired, "Number of bytes required for globals");

char MementosSizeGlobals::ID = 0;
static RegisterPass<MementosSizeGlobals> F("mementos-size-globals",
    "Counts the number of bytes required for globals",
    true, /* preserves the CFG */
    true /* is an analysis pass */);

MementosSizeGlobals::MementosSizeGlobals() : ModulePass(ID) {
  NumBytesRequired = TotalSizeInBytes = 0;
}

bool MementosSizeGlobals::runOnModule (Module &M) {
  TD = getAnalysisIfAvailable<TargetData>();
  for (Module::const_global_iterator i = M.global_begin();
      i != M.global_end(); ++i) {
    const Type *T = i->getType()->getElementType();
    const unsigned int bytes = TD->getTypeAllocSize(T);
    const StringRef s = i->getName();
    DEBUG(outs() << "Found a global of size " << bytes << " (" << s << ")\n");
    if (s.find("0x") != StringRef::npos) {
      DEBUG(outs() << "Skipping fake global with name \"" << s << "\"\n");
    } else {
      TotalSizeInBytes += bytes;
    }
  }
  NumBytesRequired += TotalSizeInBytes;
  return false; // have not modified the module
}

void MementosSizeGlobals::getAnalysisUsage (AnalysisUsage &AU) const {
  AU.setPreservesAll();
}

void MementosSizeGlobals::print (raw_ostream &OS, const Module *M) const {
  OS << ">>>>------------>\n";
  OS << "TargetData: " << TD->getStringRepresentation() << "\n";
  for (Module::const_global_iterator i = M->global_begin();
      i != M->global_end(); ++i) {
    OS << "Found a global: " << *i;
  }
  OS << ">>>>------------>\n";
}

// vim:ts=2 sw=2
