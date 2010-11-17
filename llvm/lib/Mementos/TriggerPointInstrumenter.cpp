#include "mementos/Mementos.h"
#include "mementos/Passes.h"
using namespace llvm;

void MementosTriggerPointInstrumenter::setModule (Module *M) {
  containingModule = M;
  triggerPointFunction =
    containingModule->getFunction(triggerPointFunctionName);
  if (!triggerPointFunction) {
    DEBUG(errs() << "Trigger point function ("
        << triggerPointFunctionName << ") not found in module "
        << containingModule->getModuleIdentifier()
        << "; did you forget to link in the Mementos library?\n");
  }
}

const std::string MementosTriggerPointInstrumenter::triggerPointFunctionName =
    MEMENTOS_FUNCTION_PREFIX "checkpoint";

// vim:ts=2 sw=2
