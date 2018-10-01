#include "llvm/ADT/STLExtras.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/Analysis/VectorUtils.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Value.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include <iostream>
#include "llvm/IR/LegacyPassManagers.h"

using namespace llvm;


#define CM_NAME "cost-model-sanji"
#define DEBUG_TYPE CM_NAME

    class CostModelAnalysisDummy : public PMDataManager, public FunctionPass, public PMTopLevelManager {

    public:
        static char ID; // Class identification, replacement for typeinfo
        CostModelAnalysisDummy() : FunctionPass(ID), PMDataManager(), PMTopLevelManager(new FPPassManager()), F(nullptr), TTI(nullptr) {
            llvm::initializeCostModelAnalysisDummyPass(
                *PassRegistry::getPassRegistry());
        }

        /// Returns the expected cost of the instruction.
        /// Returns -1 if the cost is unknown.
        /// Note, this method does not cache the cost calculation and it
        /// can be expensive in some cases.
        unsigned getInstructionCost(const Instruction *I) const;
        bool runOnFunction(Function &F) override;

        PMDataManager *getAsPMDataManager() override { return this; }
        Pass *getAsPass() override { return this; }

        PassManagerType getTopLevelPassManagerType() override {
            return PMT_BasicBlockPassManager;
        }

        FPPassManager *getContainedManager(unsigned N) {
            assert(N < PassManagers.size() && "Pass number out of range!");
            FPPassManager *FP = static_cast<FPPassManager *>(PassManagers[N]);
            return FP;
        }
    private:
        void getAnalysisUsage(AnalysisUsage &AU) const override;

        void print(raw_ostream &OS, const Module*) const override;

        /// The function that we analyze.
        Function *F;
        /// Target information.
        const TargetTransformInfo *TTI;
    };

    FunctionPass *createCostModelAnalysisDummyPass();
