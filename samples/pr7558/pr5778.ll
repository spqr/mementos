target datalayout = "e-p:16:16:16-i8:8:8-i16:16:16-i32:16:32-n8:16"
target triple = "msp430-elf-"

define void @pr5778() nounwind {
entry:
  %0 = call i8* @llvm.stacksave()
  ret void
}

declare i8* @llvm.stacksave() nounwind
