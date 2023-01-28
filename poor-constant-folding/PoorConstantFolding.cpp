
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "vector"

using namespace llvm;

namespace
{
  struct PoorConstantFolding : public FunctionPass
  {
    static char ID;
    PoorConstantFolding() : FunctionPass(ID) {}
    bool runOnFunction(Function &F) override
    {
      std::vector<Instruction *> eraselist;

      for (BasicBlock &BB : F)
      {
        for (Instruction &Instr : BB)
        {
          // > The first template argument specifies a class to use for creating constants.
          // > This defaults to creating minimally folded constants.
          // https://llvm.org/doxygen/classllvm_1_1IRBuilder.html
          // Not sure BasicBlock is the way to go...
          // IRBuilder<> builder(&BB);
          if (IntegerType *IT = dyn_cast<IntegerType>(Instr.getType()))
          {
            if (Instr.getOpcode() == Instruction::Add && IT->getBitWidth() == 32)
            {
              Value *lhs = Instr.getOperand(0);
              Value *rhs = Instr.getOperand(1);
              if (isa<ConstantInt>(lhs) && isa<ConstantInt>(rhs))
              {
                auto ilhs = cast<ConstantInt>(lhs);
                auto irhs = cast<ConstantInt>(rhs);
                auto sum = ConstantInt::get(IT, ilhs->getValue() + irhs->getValue());

                eraselist.push_back(&Instr);

                // for (Use &U : Instr.uses())
                // {
                //   User *user = U.getUser();
                //   user->setOperand(U.getOperandNo(), sum);
                //   // errs() << "(debug) " << user->getName() << "\n";
                // }
                // There's a convinient API
                Instr.replaceAllUsesWith(sum);
              }
            }
          }
        }
      }
      // Can't call eraseFromParent in for loop
      // https://groups.google.com/g/llvm-dev/c/g82nCn-0ZeU
      for (auto Instr : eraselist)
      {
        Instr->eraseFromParent();
      }
      return !eraselist.empty();
    }
  };
}

char PoorConstantFolding::ID = 0;

static RegisterPass<PoorConstantFolding>
    X("poor-constant-folding", "Poor Constant Folding Pass",
      false,
      true);
