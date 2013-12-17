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
#include "llvm/IR/Constants.h"
using namespace llvm;

char MementosAddGlobalSizeGlobal::ID = 0;
static RegisterPass<MementosAddGlobalSizeGlobal>
A("mementos-add-global-size-global",
    "Add a global variable indicating the total size of a module's globals"
    "(including itself) in bytes",
    true, /* preserves the CFG */
    false /* is not an analysis pass */);

MementosAddGlobalSizeGlobal::MementosAddGlobalSizeGlobal () : ModulePass(ID) {
}

bool MementosAddGlobalSizeGlobal::runOnModule (Module &M) {
    MementosSizeGlobals &SG = getAnalysis<MementosSizeGlobals>();
    unsigned tsbytes = SG.TotalSizeInBytes;
    DEBUG(outs() << "Size of globals (bytes): " << tsbytes << "\n");

    // TargetData *TD = getAnalysisIfAvailable<TargetData>();
    Type *Int16 = Type::getInt16Ty(M.getContext());
    Constant *TotalSizeInBytes =
        ConstantInt::get(Int16, tsbytes);
        /* no longer add TD->getTypeAllocSize(Int16)) to account for the global
         * we're creating, since it should go in .rodata (i.e., in NVRAM) */
    GlobalVariable *GlobalAllocSize =
        new GlobalVariable(M, Int16, true,
                GlobalValue::ExternalLinkage,
                TotalSizeInBytes,
                "GlobalAllocSize");

    return true;
}

void MementosAddGlobalSizeGlobal::getAnalysisUsage (AnalysisUsage &AU) const {
    AU.addRequired<MementosSizeGlobals>();
}

// vim:ts=2 sw=2
