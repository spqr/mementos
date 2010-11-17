#include "mementos/Mementos.h"
#include "mementos/Passes.h"
#include "llvm/Support/CommandLine.h"
using namespace llvm;

char MementosRenameMain::ID = 0;
static RegisterPass<MementosRenameMain> E("mementos-rename-main",
    "Renames main() to " MEMENTOS_OLDMAINNAME,
    false, /* does not preserve the CFG -- XXX or does it? */
    false /* is not an analysis pass */);

static cl::opt<bool> nameAfterModule("mementos-name-main-after-module",
    cl::desc("Rename main() to _main_<module>() instead of "
      MEMENTOS_OLDMAINNAME "()"),
    cl::init(false), cl::Hidden);

MementosRenameMain::MementosRenameMain () : FunctionPass(ID) {
}

bool MementosRenameMain::runOnFunction (Function &F) {
  const std::string& moduleId = F.getParent()->getModuleIdentifier();
  DEBUG(outs() << "Parent module of function: " << moduleId << "\n");
  if (F.getName() != "main")
    return false;

  // change main()'s name because Mementos provides its own main() function
  if (nameAfterModule) { // should we add a module-specific suffix?
    const std::string& newName = "_main_" +
      moduleId.substr(0, moduleId.find('.'));
    F.setName(newName);
  } else {
    F.setName(oldMainName);
  }

  return true;
}

const std::string MementosRenameMain::oldMainName = MEMENTOS_OLDMAINNAME;
