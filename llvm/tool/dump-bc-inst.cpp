#include <iostream>
#include <string>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/ErrorOr.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/Analysis/TargetTransformInfo.h>
//#include <llvm/Analysis/CostModelDummy.h>

using namespace llvm;

int main(int argc, char *argv[]) {
    //StringRef filename = "FILE_NAME";
    StringRef filename = argv[1];
    LLVMContext context;

    ErrorOr<std::unique_ptr<MemoryBuffer>> fileOrErr =
        MemoryBuffer::getFileOrSTDIN(filename);
    if (std::error_code ec = fileOrErr.getError()) {
        std::cerr << " Error opening input file: " + ec.message() << std::endl;
        return 2;
    }
    Expected<std::unique_ptr<Module>> moduleOrErr =
        parseBitcodeFile(fileOrErr.get()->getMemBufferRef(), context);
    if (std::error_code ec = fileOrErr.getError()) {
        std::cerr << "Error reading Moduule: " + ec.message() << std::endl;
        return 3;
    }

    Module *m = moduleOrErr.get().get();
    std::cout << "Successfully read Module:" << std::endl;
    std::cout << " Name: " << m->getName().str() << std::endl;
    std::cout << " Target triple: " << m->getTargetTriple() << std::endl;

    for (auto iter1 = m->getFunctionList().begin(); iter1 != m->getFunctionList().end(); iter1++) {
        Function &f = *iter1;

        //CostModelAnalysisDummy obj;
        std::cout << " STEP: 1 Function: " << f.getName().str() << std::endl;
        //obj.runOnFunction(f);

        std::cout << " STEP: 2 Function: " << f.getName().str() << std::endl;
        for (auto iter2 = f.getBasicBlockList().begin(); iter2 != f.getBasicBlockList().end(); iter2++) {
            BasicBlock &bb = *iter2;
            std::cout << "  BasicBlock: " << bb.getName().str() << std::endl;
            for (auto iter3 = bb.begin(); iter3 != bb.end(); iter3++) {
                Instruction &inst = *iter3;

                //std::cout << std::endl << " Number of Cycles" << obj.getInstructionCost(&inst) << std::endl;

                std::cout << "   Instruction " << &inst << " : " << inst.getOpcodeName();

                unsigned int  i = 0;
                unsigned int opnt_cnt = inst.getNumOperands();
                for (; i < opnt_cnt; ++i)
                {
                    Value *opnd = inst.getOperand(i);
                    std::string o;
                    //          raw_string_ostream os(o);
                    //         opnd->print(os);
                    //opnd->printAsOperand(os, true, m);
                    if (opnd->hasName()) {
                        o = opnd->getName();
                        std::cout << " " << o << ",";
                    }
                    else {
                        std::cout << " ptr" << opnd << ",";
                    }
                }
                std::cout << std::endl;
            }
        }
    }
    return 0;
}
