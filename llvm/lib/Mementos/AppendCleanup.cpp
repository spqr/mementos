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
