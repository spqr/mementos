#include "mementos/Mementos.h"
#include "mementos/Passes.h"
#include "llvm/Constants.h"
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
    const Type *Int16 = Type::getInt16Ty(M.getContext());
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
