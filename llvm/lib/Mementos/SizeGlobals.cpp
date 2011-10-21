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
    Type *T = i->getType()->getElementType();
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
