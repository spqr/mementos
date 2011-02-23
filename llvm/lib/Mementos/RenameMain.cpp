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
