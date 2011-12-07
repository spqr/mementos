; ModuleID = 'crc-vanilla2+latch+gsize+mementos+o.bc'
target datalayout = "e-p:16:16:16-i8:8:8-i16:16:16-i32:16:32-n8:16"
target triple = "msp430-elf-"

@GlobalAllocSize = constant i16 0
@"\010x01C0" = external global i16
@j = common global i16 0, align 2
@baseaddr = common global i16 0, align 2
@"\010x012C" = external global i16
@"\010x0128" = external global i16
@i = common global i16 0, align 2
@tmpsize = common global i16 0, align 2
@k = common global i16 0, align 2

define zeroext i16 @crc16_ccitt_mnotp(i8* %data, i16 zeroext %n) nounwind {
entry:
  %data.addr = alloca i8*, align 2
  %n.addr = alloca i16, align 2
  %i = alloca i16, align 2
  %j = alloca i16, align 2
  %crc_16 = alloca i16, align 2
  store i8* %data, i8** %data.addr, align 2
  store i16 %n, i16* %n.addr, align 2
  store i16 -1, i16* %crc_16, align 2
  store i16 0, i16* %i, align 2
  br label %for.cond

for.cond:                                         ; preds = %for.inc19, %entry
  %tmp = load i16* %i, align 2
  %tmp1 = load i16* %n.addr, align 2
  %cmp = icmp ult i16 %tmp, %tmp1
  br i1 %cmp, label %for.body, label %for.end22

for.body:                                         ; preds = %for.cond
  %tmp2 = load i16* %i, align 2
  %tmp3 = load i8** %data.addr, align 2
  %arrayidx = getelementptr inbounds i8* %tmp3, i16 %tmp2
  %tmp4 = volatile load i8* %arrayidx
  %conv = zext i8 %tmp4 to i16
  %shl = shl i16 %conv, 8
  %tmp5 = load i16* %crc_16, align 2
  %xor = xor i16 %tmp5, %shl
  store i16 %xor, i16* %crc_16, align 2
  store i16 0, i16* %j, align 2
  br label %for.cond6

for.cond6:                                        ; preds = %for.inc, %for.body
  %tmp7 = load i16* %j, align 2
  %cmp8 = icmp ult i16 %tmp7, 8
  br i1 %cmp8, label %for.body10, label %for.end

for.body10:                                       ; preds = %for.cond6
  %tmp11 = load i16* %crc_16, align 2
  %and = and i16 %tmp11, -32768
  %tobool = icmp ne i16 %and, 0
  br i1 %tobool, label %if.then, label %if.else

if.then:                                          ; preds = %for.body10
  %tmp12 = load i16* %crc_16, align 2
  %shl13 = shl i16 %tmp12, 1
  store i16 %shl13, i16* %crc_16, align 2
  %tmp14 = load i16* %crc_16, align 2
  %xor15 = xor i16 %tmp14, 4129
  store i16 %xor15, i16* %crc_16, align 2
  br label %if.end

if.else:                                          ; preds = %for.body10
  %tmp16 = load i16* %crc_16, align 2
  %shl17 = shl i16 %tmp16, 1
  store i16 %shl17, i16* %crc_16, align 2
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %tmp18 = load i16* %j, align 2
  %inc = add i16 %tmp18, 1
  store i16 %inc, i16* %j, align 2
  br label %for.cond6

for.end:                                          ; preds = %for.cond6
  br label %for.inc19

for.inc19:                                        ; preds = %for.end
  %tmp20 = load i16* %i, align 2
  %inc21 = add i16 %tmp20, 1
  store i16 %inc21, i16* %i, align 2
  br label %for.cond

for.end22:                                        ; preds = %for.cond
  %tmp23 = load i16* %crc_16, align 2
  %xor24 = xor i16 %tmp23, -1
  ret i16 %xor24
}

define void @do_crc() nounwind {
entry:
  %accumulator = alloca i16, align 2
  %i = alloca i16, align 2
  volatile store i16 0, i16* %accumulator, align 2
  store i16 0, i16* %i, align 2
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %tmp = load i16* %i, align 2
  %cmp = icmp ult i16 %tmp, 64
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %tmp1 = load i16* %i, align 2
  %mul = mul i16 %tmp1, 64
  %add = add i16 -8192, %mul
  %0 = inttoptr i16 %add to i8*
  %call = call zeroext i16 @crc16_ccitt_mnotp(i8* %0, i16 zeroext 32)
  %tmp2 = volatile load i16* %accumulator, align 2
  %add3 = add i16 %tmp2, %call
  volatile store i16 %add3, i16* %accumulator, align 2
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %tmp4 = load i16* %i, align 2
  %inc = add i16 %tmp4, 1
  store i16 %inc, i16* %i, align 2
  call void @__mementos_checkpoint()
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

define i16 @_old_main() nounwind {
entry:
  %retval = alloca i16, align 2
  store i16 0, i16* %retval
  call void @do_crc()
  ret i16 27
}

define void @__mementos_checkpoint() nounwind {
entry:
  %tmp = volatile load i16* @"\010x01C0", align 2
  %cmp = icmp uge i16 %tmp, 21845
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  call void asm sideeffect "RET", ""() nounwind, !srcloc !0
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  call void asm sideeffect "PUSH 2(R1)\0A\09PUSH R1\0A\09ADD #6, 0(R1)\0A\09PUSH R2\0A\09PUSH R4\0A\09PUSH R5\0A\09PUSH R6\0A\09PUSH R7\0A\09PUSH R8\0A\09PUSH R9\0A\09PUSH R10\0A\09PUSH R11\0A\09PUSH R12\0A\09PUSH R13\0A\09PUSH R14\0A\09PUSH R15", ""() nounwind, !srcloc !1
  call void asm sideeffect "MOV 26(R1), &$0", "=*m"(i16* @j) nounwind, !srcloc !2
  %tmp1 = load i16* @j, align 2
  %call = call i16 @__mementos_locate_next_bundle(i16 %tmp1)
  store i16 %call, i16* @baseaddr, align 2
  %tmp2 = load i16* @baseaddr, align 2
  %cmp3 = icmp eq i16 %tmp2, -1
  br i1 %cmp3, label %if.then4, label %if.end13

if.then4:                                         ; preds = %if.end
  %call5 = call i16 @__mementos_segment_marked_erase(i16 -1536)
  %tobool = icmp ne i16 %call5, 0
  br i1 %tobool, label %if.then6, label %if.else

if.then6:                                         ; preds = %if.then4
  call void @__mementos_erase_segment(i16 -1536)
  store i16 -1536, i16* @baseaddr, align 2
  br label %if.end12

if.else:                                          ; preds = %if.then4
  %call7 = call i16 @__mementos_segment_marked_erase(i16 -1024)
  %tobool8 = icmp ne i16 %call7, 0
  br i1 %tobool8, label %if.then9, label %if.else10

if.then9:                                         ; preds = %if.else
  call void @__mementos_erase_segment(i16 -1024)
  store i16 -1024, i16* @baseaddr, align 2
  br label %if.end11

if.else10:                                        ; preds = %if.else
  call void asm sideeffect "ADD #30, R1\0A\09RET", ""() nounwind, !srcloc !3
  br label %if.end11

if.end11:                                         ; preds = %if.else10, %if.then9
  br label %if.end12

if.end12:                                         ; preds = %if.end11, %if.then6
  br label %if.end13

if.end13:                                         ; preds = %if.end12, %if.end
  call void asm sideeffect "DINT", ""() nounwind, !srcloc !4
  volatile store i16 -23296, i16* @"\010x012C", align 2
  volatile store i16 -23232, i16* @"\010x0128", align 2
  call void asm sideeffect "PUSH R12", ""() nounwind, !srcloc !5
  call void asm sideeffect "PUSH R13", ""() nounwind, !srcloc !6
  call void asm sideeffect "MOV #0x0300, R13", ""() nounwind, !srcloc !7
  call void asm sideeffect "SUB &$0, R13", "*m"(i16* @j) nounwind, !srcloc !8
  call void asm sideeffect "MOV &$0, R12", "*m"(i16* @baseaddr) nounwind, !srcloc !9
  call void asm sideeffect "MOV.B R13, 1(R12)", ""() nounwind, !srcloc !10
  call void asm sideeffect "MOV &$0, R13", "*m"(i16* @GlobalAllocSize) nounwind, !srcloc !11
  call void asm sideeffect "INC R13", ""() nounwind, !srcloc !12
  call void asm sideeffect "AND #0xFE, R13", ""() nounwind, !srcloc !13
  call void asm sideeffect "MOV.B R13, 0(R12)", ""() nounwind, !srcloc !14
  call void asm sideeffect "POP R13", ""() nounwind, !srcloc !15
  call void asm sideeffect "POP R12", ""() nounwind, !srcloc !16
  call void asm sideeffect "MOV &$0, R14", "*m"(i16* @baseaddr) nounwind, !srcloc !17
  call void asm sideeffect "POP 30(R14)\0A\09POP 28(R14)\0A\09POP 26(R14)\0A\09POP 24(R14)\0A\09POP 22(R14)\0A\09POP 20(R14)\0A\09POP 18(R14)\0A\09POP 16(R14)\0A\09POP 14(R14)\0A\09POP 12(R14)\0A\09POP 10(R14)\0A\09POP 8(R14)\0A\09POP 6(R14)\0A\09POP 4(R14)\0A\09POP 2(R14)", ""() nounwind, !srcloc !18
  %tmp14 = load i16* @j, align 2
  store i16 %tmp14, i16* @i, align 2
  %tmp15 = load i16* @i, align 2
  %sub = sub i16 768, %tmp15
  store i16 %sub, i16* @j, align 2
  br label %while.cond

while.cond:                                       ; preds = %while.body, %if.end13
  %tmp16 = load i16* @i, align 2
  %sub17 = sub i16 768, %tmp16
  %cmp18 = icmp ugt i16 %sub17, 0
  br i1 %cmp18, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %tmp19 = load i16* @i, align 2
  %0 = inttoptr i16 %tmp19 to i16*
  %tmp20 = load i16* %0
  %tmp21 = load i16* @baseaddr, align 2
  %add = add i16 %tmp21, 32
  %tmp22 = load i16* @j, align 2
  %add23 = add i16 %add, %tmp22
  %tmp24 = load i16* @i, align 2
  %sub25 = sub i16 768, %tmp24
  %sub26 = sub i16 %add23, %sub25
  %1 = inttoptr i16 %sub26 to i16*
  store i16 %tmp20, i16* %1
  %tmp27 = load i16* @i, align 2
  %add28 = add i16 %tmp27, 2
  store i16 %add28, i16* @i, align 2
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %tmp29 = load i16* @j, align 2
  %add30 = add i16 %tmp29, 32
  store i16 %add30, i16* @j, align 2
  store i16 512, i16* @i, align 2
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %while.end
  %tmp31 = load i16* @i, align 2
  %tmp32 = load i16* @GlobalAllocSize, align 2
  %add33 = add nsw i16 %tmp32, 1
  %and = and i16 %add33, -2
  %add34 = add i16 512, %and
  %cmp35 = icmp ult i16 %tmp31, %add34
  br i1 %cmp35, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %tmp36 = load i16* @i, align 2
  %2 = inttoptr i16 %tmp36 to i16*
  %tmp37 = load i16* %2
  %tmp38 = load i16* @baseaddr, align 2
  %tmp39 = load i16* @j, align 2
  %add40 = add i16 %tmp38, %tmp39
  %3 = inttoptr i16 %add40 to i16*
  store i16 %tmp37, i16* %3
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %tmp41 = load i16* @i, align 2
  %add42 = add i16 %tmp41, 2
  store i16 %add42, i16* @i, align 2
  %tmp43 = load i16* @j, align 2
  %add44 = add i16 %tmp43, 2
  store i16 %add44, i16* @j, align 2
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %tmp45 = load i16* @baseaddr, align 2
  %tmp46 = load i16* @j, align 2
  %add47 = add i16 %tmp45, %tmp46
  %4 = inttoptr i16 %add47 to i16*
  store i16 -16723, i16* %4
  %tmp48 = load i16* @baseaddr, align 2
  %cmp49 = icmp eq i16 %tmp48, -1536
  br i1 %cmp49, label %if.then52, label %lor.lhs.false

lor.lhs.false:                                    ; preds = %for.end
  %tmp50 = load i16* @baseaddr, align 2
  %cmp51 = icmp eq i16 %tmp50, -1024
  br i1 %cmp51, label %if.then52, label %if.end61

if.then52:                                        ; preds = %lor.lhs.false, %for.end
  %tmp53 = load i16* @baseaddr, align 2
  %cmp54 = icmp ult i16 %tmp53, -1024
  %cond = select i1 %cmp54, i16 -1024, i16 -1536
  store i16 %cond, i16* @j, align 2
  %tmp55 = load i16* @j, align 2
  %call56 = call i16 @__mementos_segment_is_empty(i16 %tmp55)
  %tobool57 = icmp ne i16 %call56, 0
  br i1 %tobool57, label %if.end60, label %if.then58

if.then58:                                        ; preds = %if.then52
  %tmp59 = load i16* @j, align 2
  call void @__mementos_mark_segment_erase(i16 %tmp59)
  br label %if.end60

if.end60:                                         ; preds = %if.then58, %if.then52
  br label %if.end61

if.end61:                                         ; preds = %if.end60, %lor.lhs.false
  volatile store i16 -23296, i16* @"\010x0128", align 2
  volatile store i16 -23280, i16* @"\010x012C", align 2
  call void asm sideeffect "EINT", ""() nounwind, !srcloc !19
  ret void
}

define i16 @__mementos_locate_next_bundle(i16 %sp) nounwind {
entry:
  %retval = alloca i16, align 2
  %sp.addr = alloca i16, align 2
  %size = alloca i16, align 2
  %b = alloca i16, align 2
  %bsize = alloca i16, align 2
  %next = alloca i16, align 2
  store i16 %sp, i16* %sp.addr, align 2
  %tmp = load i16* %sp.addr, align 2
  %sub = sub i16 768, %tmp
  %add = add i16 32, %sub
  %tmp1 = load i16* @GlobalAllocSize, align 2
  %add2 = add nsw i16 %tmp1, 1
  %and = and i16 %add2, -2
  %add3 = add i16 %add, %and
  %add4 = add i16 %add3, 2
  store i16 %add4, i16* %size, align 2
  %call = call i16 @__mementos_find_active_bundle()
  store i16 %call, i16* @baseaddr, align 2
  %tmp5 = load i16* @baseaddr, align 2
  %cmp = icmp eq i16 %tmp5, -1
  br i1 %cmp, label %if.then, label %if.end17

if.then:                                          ; preds = %entry
  %call6 = call i16 @__mementos_segment_is_empty(i16 -1536)
  %tobool = icmp ne i16 %call6, 0
  br i1 %tobool, label %land.lhs.true, label %if.end

land.lhs.true:                                    ; preds = %if.then
  %call7 = call i16 @__mementos_segment_is_empty(i16 -1024)
  %tobool8 = icmp ne i16 %call7, 0
  br i1 %tobool8, label %if.then9, label %if.end

if.then9:                                         ; preds = %land.lhs.true
  store i16 -1536, i16* %retval
  br label %return

if.end:                                           ; preds = %land.lhs.true, %if.then
  %call10 = call i16 @__mementos_segment_marked_erase(i16 -1536)
  %tobool11 = icmp ne i16 %call10, 0
  br i1 %tobool11, label %land.lhs.true12, label %if.end16

land.lhs.true12:                                  ; preds = %if.end
  %call13 = call i16 @__mementos_segment_marked_erase(i16 -1024)
  %tobool14 = icmp ne i16 %call13, 0
  br i1 %tobool14, label %if.then15, label %if.end16

if.then15:                                        ; preds = %land.lhs.true12
  call void @__mementos_erase_segment(i16 -1536)
  store i16 -1536, i16* %retval
  br label %return

if.end16:                                         ; preds = %land.lhs.true12, %if.end
  store i16 -1, i16* %retval
  br label %return

if.end17:                                         ; preds = %entry
  %tmp18 = load i16* @baseaddr, align 2
  %0 = inttoptr i16 %tmp18 to i16*
  %tmp19 = load i16* %0
  %shr = lshr i16 %tmp19, 8
  %add20 = add i16 32, %shr
  %tmp21 = load i16* @baseaddr, align 2
  %1 = inttoptr i16 %tmp21 to i16*
  %tmp22 = load i16* %1
  %and23 = and i16 %tmp22, 255
  %add24 = add i16 %add20, %and23
  %add25 = add i16 %add24, 2
  store i16 %add25, i16* %bsize, align 2
  %tmp26 = load i16* @baseaddr, align 2
  %tmp27 = load i16* %bsize, align 2
  %add28 = add i16 %tmp26, %tmp27
  store i16 %add28, i16* %next, align 2
  br label %while.cond

while.cond:                                       ; preds = %if.end51, %if.end17
  %tmp29 = load i16* %next, align 2
  %2 = inttoptr i16 %tmp29 to i16*
  %tmp30 = load i16* %2
  %cmp31 = icmp ne i16 %tmp30, -1
  br i1 %cmp31, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %tmp32 = load i16* %next, align 2
  %3 = inttoptr i16 %tmp32 to i16*
  %tmp33 = load i16* %3
  %shr34 = lshr i16 %tmp33, 8
  %add35 = add i16 32, %shr34
  %tmp36 = load i16* %next, align 2
  %4 = inttoptr i16 %tmp36 to i16*
  %tmp37 = load i16* %4
  %and38 = and i16 %tmp37, 255
  %add39 = add i16 %add35, %and38
  %add40 = add i16 %add39, 2
  store i16 %add40, i16* %bsize, align 2
  %tmp41 = load i16* %bsize, align 2
  %tmp42 = load i16* %next, align 2
  %add43 = add i16 %tmp42, %tmp41
  store i16 %add43, i16* %next, align 2
  %tmp44 = load i16* %next, align 2
  %cmp45 = icmp ugt i16 %tmp44, -512
  br i1 %cmp45, label %if.then46, label %if.end51

if.then46:                                        ; preds = %while.body
  %tmp47 = load i16* @baseaddr, align 2
  %cmp48 = icmp uge i16 %tmp47, -1024
  br i1 %cmp48, label %if.then49, label %if.else

if.then49:                                        ; preds = %if.then46
  store i16 -1536, i16* %next, align 2
  call void @__mementos_mark_segment_erase(i16 -1024)
  br label %if.end50

if.else:                                          ; preds = %if.then46
  store i16 -1024, i16* %next, align 2
  call void @__mementos_mark_segment_erase(i16 -1536)
  br label %if.end50

if.end50:                                         ; preds = %if.else, %if.then49
  br label %while.end

if.end51:                                         ; preds = %while.body
  br label %while.cond

while.end:                                        ; preds = %if.end50, %while.cond
  %tmp52 = load i16* %next, align 2
  %cmp53 = icmp ult i16 %tmp52, -1024
  br i1 %cmp53, label %if.then54, label %if.else66

if.then54:                                        ; preds = %while.end
  %tmp55 = load i16* %next, align 2
  %tmp56 = load i16* %size, align 2
  %add57 = add i16 %tmp55, %tmp56
  %cmp58 = icmp ugt i16 %add57, -1024
  br i1 %cmp58, label %if.then59, label %if.end65

if.then59:                                        ; preds = %if.then54
  %call60 = call i16 @__mementos_segment_is_empty(i16 -1024)
  %tobool61 = icmp ne i16 %call60, 0
  br i1 %tobool61, label %if.then62, label %if.else63

if.then62:                                        ; preds = %if.then59
  store i16 -1024, i16* %next, align 2
  call void @__mementos_mark_segment_erase(i16 -1536)
  br label %if.end64

if.else63:                                        ; preds = %if.then59
  call void @__mementos_mark_segment_erase(i16 -1024)
  store i16 -1, i16* %retval
  br label %return

if.end64:                                         ; preds = %if.then62
  br label %if.end65

if.end65:                                         ; preds = %if.end64, %if.then54
  br label %if.end78

if.else66:                                        ; preds = %while.end
  %tmp67 = load i16* %next, align 2
  %tmp68 = load i16* %size, align 2
  %add69 = add i16 %tmp67, %tmp68
  %cmp70 = icmp ugt i16 %add69, -512
  br i1 %cmp70, label %if.then71, label %if.end77

if.then71:                                        ; preds = %if.else66
  %call72 = call i16 @__mementos_segment_is_empty(i16 -1536)
  %tobool73 = icmp ne i16 %call72, 0
  br i1 %tobool73, label %if.then74, label %if.else75

if.then74:                                        ; preds = %if.then71
  store i16 -1536, i16* %next, align 2
  call void @__mementos_mark_segment_erase(i16 -1024)
  br label %if.end76

if.else75:                                        ; preds = %if.then71
  call void @__mementos_mark_segment_erase(i16 -1536)
  store i16 -1, i16* %retval
  br label %return

if.end76:                                         ; preds = %if.then74
  br label %if.end77

if.end77:                                         ; preds = %if.end76, %if.else66
  br label %if.end78

if.end78:                                         ; preds = %if.end77, %if.end65
  %tmp79 = load i16* %next, align 2
  store i16 %tmp79, i16* %retval
  br label %return

return:                                           ; preds = %if.end78, %if.else75, %if.else63, %if.end16, %if.then15, %if.then9
  %5 = load i16* %retval
  ret i16 %5
}

define i16 @__mementos_segment_marked_erase(i16 %addr) nounwind {
entry:
  %addr.addr = alloca i16, align 2
  store i16 %addr, i16* %addr.addr, align 2
  %tmp = load i16* %addr.addr, align 2
  %add = add i16 %tmp, 2
  %0 = inttoptr i16 %add to i16*
  %tmp1 = load i16* %0
  %cmp = icmp eq i16 %tmp1, 0
  br i1 %cmp, label %land.rhs, label %land.end

land.rhs:                                         ; preds = %entry
  %tmp2 = load i16* %addr.addr, align 2
  %add3 = add i16 %tmp2, 512
  %sub = sub i16 %add3, 2
  %1 = inttoptr i16 %sub to i16*
  %tmp4 = load i16* %1
  %cmp5 = icmp eq i16 %tmp4, 0
  br label %land.end

land.end:                                         ; preds = %land.rhs, %entry
  %2 = phi i1 [ false, %entry ], [ %cmp5, %land.rhs ]
  %land.ext = zext i1 %2 to i16
  ret i16 %land.ext
}

define void @__mementos_erase_segment(i16 %addr_in_segment) nounwind {
entry:
  %addr_in_segment.addr = alloca i16, align 2
  store i16 %addr_in_segment, i16* %addr_in_segment.addr, align 2
  call void asm sideeffect "DINT", ""() nounwind, !srcloc !20
  volatile store i16 -23296, i16* @"\010x012C", align 2
  volatile store i16 -23294, i16* @"\010x0128", align 2
  %tmp = load i16* %addr_in_segment.addr, align 2
  %0 = inttoptr i16 %tmp to i16*
  store i16 0, i16* %0
  volatile store i16 -23296, i16* @"\010x0128", align 2
  volatile store i16 -23280, i16* @"\010x012C", align 2
  call void asm sideeffect "EINT", ""() nounwind, !srcloc !21
  ret void
}

define i16 @__mementos_segment_is_empty(i16 %addr) nounwind {
entry:
  %retval = alloca i16, align 2
  %addr.addr = alloca i16, align 2
  %a = alloca i16, align 2
  store i16 %addr, i16* %addr.addr, align 2
  %tmp = load i16* %addr.addr, align 2
  store i16 %tmp, i16* %a, align 2
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %tmp1 = load i16* %a, align 2
  %tmp2 = load i16* %addr.addr, align 2
  %add = add i16 %tmp2, 512
  %cmp = icmp ult i16 %tmp1, %add
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %tmp3 = load i16* %a, align 2
  %0 = inttoptr i16 %tmp3 to i16*
  %tmp4 = load i16* %0
  %cmp5 = icmp ne i16 %tmp4, -1
  br i1 %cmp5, label %if.then, label %if.end

if.then:                                          ; preds = %for.body
  store i16 0, i16* %retval
  br label %return

if.end:                                           ; preds = %for.body
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %tmp6 = load i16* %a, align 2
  %add7 = add i16 %tmp6, 32
  store i16 %add7, i16* %a, align 2
  br label %for.cond

for.end:                                          ; preds = %for.cond
  store i16 1, i16* %retval
  br label %return

return:                                           ; preds = %for.end, %if.then
  %1 = load i16* %retval
  ret i16 %1
}

define void @__mementos_mark_segment_erase(i16 %addr) nounwind {
entry:
  %addr.addr = alloca i16, align 2
  store i16 %addr, i16* %addr.addr, align 2
  volatile store i16 -23296, i16* @"\010x012C", align 2
  volatile store i16 -23232, i16* @"\010x0128", align 2
  %tmp = load i16* %addr.addr, align 2
  %add = add i16 %tmp, 2
  %0 = inttoptr i16 %add to i16*
  store i16 0, i16* %0
  %tmp1 = load i16* %addr.addr, align 2
  %add2 = add i16 %tmp1, 512
  %sub = sub i16 %add2, 2
  %1 = inttoptr i16 %sub to i16*
  store i16 0, i16* %1
  volatile store i16 -23296, i16* @"\010x0128", align 2
  volatile store i16 -23280, i16* @"\010x012C", align 2
  ret void
}

define void @__mementos_restore(i16 %b) nounwind {
entry:
  %b.addr = alloca i16, align 2
  store i16 %b, i16* %b.addr, align 2
  %tmp = load i16* %b.addr, align 2
  store i16 %tmp, i16* @baseaddr, align 2
  %tmp1 = load i16* @baseaddr, align 2
  %0 = inttoptr i16 %tmp1 to i16*
  %tmp2 = load i16* %0
  %shr = lshr i16 %tmp2, 8
  store i16 %shr, i16* @tmpsize, align 2
  store i16 0, i16* @i, align 2
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %tmp3 = load i16* @i, align 2
  %tmp4 = load i16* @tmpsize, align 2
  %cmp = icmp ult i16 %tmp3, %tmp4
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  call void asm sideeffect "MOV #0x0300, &$0", "=*m"(i16* @j) nounwind, !srcloc !22
  call void asm sideeffect "SUB &$0, &$1", "=*m,*m"(i16* @i, i16* @j) nounwind, !srcloc !23
  call void asm sideeffect "DECD.W &$0", "*m"(i16* @j) nounwind, !srcloc !24
  call void asm sideeffect "MOV &$1, &$0", "=*m,*m"(i16* @k, i16* @baseaddr) nounwind, !srcloc !25
  call void asm sideeffect "ADD #30, &$0", "*m"(i16* @k) nounwind, !srcloc !26
  call void asm sideeffect "ADD &$1, &$0", "=*m,*m"(i16* @k, i16* @tmpsize) nounwind, !srcloc !27
  call void asm sideeffect "SUB &$1, &$0", "=*m,*m"(i16* @k, i16* @i) nounwind, !srcloc !28
  call void asm sideeffect "MOV &$0, R7", "*m"(i16* @k) nounwind, !srcloc !29
  call void asm sideeffect "MOV &$0, R8", "*m"(i16* @j) nounwind, !srcloc !30
  call void asm sideeffect "MOV @R7, 0(R8)", ""() nounwind, !srcloc !31
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %tmp5 = load i16* @i, align 2
  %add = add i16 %tmp5, 2
  store i16 %add, i16* @i, align 2
  br label %for.cond

for.end:                                          ; preds = %for.cond
  call void asm sideeffect "MOV &$0, R7", "*m"(i16* @tmpsize) nounwind, !srcloc !32
  call void asm sideeffect "MOV &$0, R6", "*m"(i16* @baseaddr) nounwind, !srcloc !33
  call void asm sideeffect "MOV @R6, R8\0A\09AND #255, R8", ""() nounwind, !srcloc !34
  call void asm sideeffect "CLR.W R9", ""() nounwind, !srcloc !35
  call void asm sideeffect "rdloop:", ""() nounwind, !srcloc !36
  call void asm sideeffect "CMP R7, R9\0A\09JC afterrd", ""() nounwind, !srcloc !37
  call void asm sideeffect "MOV R7, R10\0A\09ADD R6, R10\0A\09ADD #30, R10", ""() nounwind, !srcloc !38
  call void asm sideeffect "MOV @R10, 0x0200(R9)", ""() nounwind, !srcloc !39
  call void asm sideeffect "INCD R9", ""() nounwind, !srcloc !40
  call void asm sideeffect "JMP rdloop", ""() nounwind, !srcloc !41
  call void asm sideeffect "afterrd:", ""() nounwind, !srcloc !42
  call void asm sideeffect "MOV R6, &$0", "=*m"(i16* @baseaddr) nounwind, !srcloc !43
  call void asm sideeffect "MOV 2(R6), &$0", "=*m"(i16* @j) nounwind, !srcloc !44
  call void asm sideeffect "MOV 4(R6), R1", ""() nounwind, !srcloc !45
  call void asm sideeffect "PUSH 30(R6)\0A\09PUSH 28(R6)\0A\09PUSH 26(R6)\0A\09PUSH 24(R6)\0A\09PUSH 22(R6)\0A\09PUSH 20(R6)\0A\09PUSH 18(R6)\0A\09PUSH 16(R6)\0A\09PUSH 14(R6)\0A\09PUSH 12(R6)\0A\09PUSH 10(R6)\0A\09PUSH 8(R6)\0A\09PUSH 6(R6)\0A\09PUSH 4(R6)\0A\09PUSH 2(R6)", ""() nounwind, !srcloc !46
  call void asm sideeffect "MOV 28(R1), R15\0A\09MOV 26(R1), R14\0A\09MOV 24(R1), R13\0A\09MOV 22(R1), R12\0A\09MOV 20(R1), R11\0A\09MOV 18(R1), R10\0A\09MOV 16(R1), R9\0A\09MOV 14(R1), R8\0A\09MOV 12(R1), R7\0A\09MOV 10(R1), R6\0A\09MOV  8(R1), R5\0A\09MOV  6(R1), R4\0A\09MOV  4(R1), R2\0A\09MOV  2(R1), R1", ""() nounwind, !srcloc !47
  %tmp6 = load i16* @baseaddr, align 2
  %add7 = add i16 %tmp6, 2
  %1 = inttoptr i16 %add7 to i16*
  %tmp8 = load i16* %1
  store i16 %tmp8, i16* @j, align 2
  call void asm sideeffect "MOV &$0, R0", "*m"(i16* @j) nounwind, !srcloc !48
  ret void
}

define i16 @__mementos_find_active_bundle() nounwind {
entry:
  %retval = alloca i16, align 2
  %bun = alloca i16, align 2
  %magic = alloca i16, align 2
  %endloc = alloca i16, align 2
  %candidate = alloca i16, align 2
  store i16 -1536, i16* %bun, align 2
  store i16 -1, i16* %candidate, align 2
  %call = call i16 @__mementos_segment_is_empty(i16 -1536)
  %tobool = icmp ne i16 %call, 0
  br i1 %tobool, label %if.then, label %lor.lhs.false

lor.lhs.false:                                    ; preds = %entry
  %call1 = call i16 @__mementos_segment_marked_erase(i16 -1536)
  %tobool2 = icmp ne i16 %call1, 0
  br i1 %tobool2, label %if.then, label %if.end30

if.then:                                          ; preds = %lor.lhs.false, %entry
  store i16 -1024, i16* %bun, align 2
  br label %do.body

do.body:                                          ; preds = %do.cond, %if.then
  %tmp = load i16* %bun, align 2
  %0 = inttoptr i16 %tmp to i16*
  %tmp3 = load i16* %0
  %cmp = icmp eq i16 %tmp3, -1
  br i1 %cmp, label %if.then4, label %if.end

if.then4:                                         ; preds = %do.body
  %tmp5 = load i16* %candidate, align 2
  store i16 %tmp5, i16* %retval
  br label %return

if.end:                                           ; preds = %do.body
  %tmp6 = load i16* %bun, align 2
  %tmp7 = load i16* %bun, align 2
  %1 = inttoptr i16 %tmp7 to i16*
  %tmp8 = load i16* %1
  %and = and i16 %tmp8, 255
  %add = add i16 %tmp6, %and
  %tmp9 = load i16* %bun, align 2
  %2 = inttoptr i16 %tmp9 to i16*
  %tmp10 = load i16* %2
  %shr = lshr i16 %tmp10, 8
  %add11 = add i16 %add, %shr
  %add12 = add i16 %add11, 2
  %add13 = add i16 %add12, 30
  store i16 %add13, i16* %endloc, align 2
  %tmp14 = load i16* %endloc, align 2
  %3 = inttoptr i16 %tmp14 to i16*
  %tmp15 = load i16* %3
  store i16 %tmp15, i16* %magic, align 2
  %tmp16 = load i16* %magic, align 2
  %cmp17 = icmp eq i16 %tmp16, -16723
  br i1 %cmp17, label %if.then18, label %if.else

if.then18:                                        ; preds = %if.end
  %tmp19 = load i16* %bun, align 2
  store i16 %tmp19, i16* %candidate, align 2
  br label %if.end24

if.else:                                          ; preds = %if.end
  %tmp20 = load i16* %bun, align 2
  %cmp21 = icmp eq i16 %tmp20, -1024
  br i1 %cmp21, label %if.then22, label %if.end23

if.then22:                                        ; preds = %if.else
  call void @__mementos_mark_segment_erase(i16 -1024)
  br label %if.end23

if.end23:                                         ; preds = %if.then22, %if.else
  br label %do.end

if.end24:                                         ; preds = %if.then18
  %tmp25 = load i16* %endloc, align 2
  %add26 = add i16 %tmp25, 2
  store i16 %add26, i16* %bun, align 2
  br label %do.cond

do.cond:                                          ; preds = %if.end24
  %tmp27 = load i16* %bun, align 2
  %cmp28 = icmp ult i16 %tmp27, -512
  br i1 %cmp28, label %do.body, label %do.end

do.end:                                           ; preds = %do.cond, %if.end23
  %tmp29 = load i16* %candidate, align 2
  store i16 %tmp29, i16* %retval
  br label %return

if.end30:                                         ; preds = %lor.lhs.false
  br label %do.body31

do.body31:                                        ; preds = %do.cond63, %if.end30
  %tmp32 = load i16* %bun, align 2
  %4 = inttoptr i16 %tmp32 to i16*
  %tmp33 = load i16* %4
  %cmp34 = icmp eq i16 %tmp33, -1
  br i1 %cmp34, label %if.then35, label %if.end37

if.then35:                                        ; preds = %do.body31
  %tmp36 = load i16* %candidate, align 2
  store i16 %tmp36, i16* %retval
  br label %return

if.end37:                                         ; preds = %do.body31
  %tmp38 = load i16* %bun, align 2
  %tmp39 = load i16* %bun, align 2
  %5 = inttoptr i16 %tmp39 to i16*
  %tmp40 = load i16* %5
  %and41 = and i16 %tmp40, 255
  %add42 = add i16 %tmp38, %and41
  %tmp43 = load i16* %bun, align 2
  %6 = inttoptr i16 %tmp43 to i16*
  %tmp44 = load i16* %6
  %shr45 = lshr i16 %tmp44, 8
  %add46 = add i16 %add42, %shr45
  %add47 = add i16 %add46, 2
  %add48 = add i16 %add47, 30
  store i16 %add48, i16* %endloc, align 2
  %tmp49 = load i16* %endloc, align 2
  %7 = inttoptr i16 %tmp49 to i16*
  %tmp50 = load i16* %7
  store i16 %tmp50, i16* %magic, align 2
  %tmp51 = load i16* %magic, align 2
  %cmp52 = icmp eq i16 %tmp51, -16723
  br i1 %cmp52, label %if.then53, label %if.else55

if.then53:                                        ; preds = %if.end37
  %tmp54 = load i16* %bun, align 2
  store i16 %tmp54, i16* %candidate, align 2
  br label %if.end60

if.else55:                                        ; preds = %if.end37
  %tmp56 = load i16* %bun, align 2
  %cmp57 = icmp eq i16 %tmp56, -1536
  br i1 %cmp57, label %if.then58, label %if.end59

if.then58:                                        ; preds = %if.else55
  call void @__mementos_mark_segment_erase(i16 -1536)
  br label %if.end59

if.end59:                                         ; preds = %if.then58, %if.else55
  br label %do.end66

if.end60:                                         ; preds = %if.then53
  %tmp61 = load i16* %endloc, align 2
  %add62 = add i16 %tmp61, 2
  store i16 %add62, i16* %bun, align 2
  br label %do.cond63

do.cond63:                                        ; preds = %if.end60
  %tmp64 = load i16* %bun, align 2
  %cmp65 = icmp ult i16 %tmp64, -1024
  br i1 %cmp65, label %do.body31, label %do.end66

do.end66:                                         ; preds = %do.cond63, %if.end59
  %tmp67 = load i16* %candidate, align 2
  store i16 %tmp67, i16* %retval
  br label %return

return:                                           ; preds = %do.end66, %if.then35, %do.end, %if.then4
  %8 = load i16* %retval
  ret i16 %8
}

define i16 @main() nounwind {
entry:
  %retval = alloca i16, align 2
  store i16 0, i16* %retval
  %call = call i16 @__mementos_find_active_bundle()
  store i16 %call, i16* @i, align 2
  %tmp = load i16* @i, align 2
  %cmp = icmp uge i16 %tmp, -1536
  br i1 %cmp, label %land.lhs.true, label %if.end14

land.lhs.true:                                    ; preds = %entry
  %tmp1 = load i16* @i, align 2
  %cmp2 = icmp ult i16 %tmp1, -512
  br i1 %cmp2, label %if.then, label %if.end14

if.then:                                          ; preds = %land.lhs.true
  %tmp3 = load i16* @i, align 2
  %cmp4 = icmp ult i16 %tmp3, -1024
  br i1 %cmp4, label %if.then5, label %if.else

if.then5:                                         ; preds = %if.then
  %call6 = call i16 @__mementos_segment_marked_erase(i16 -1024)
  %tobool = icmp ne i16 %call6, 0
  br i1 %tobool, label %if.then7, label %if.end

if.then7:                                         ; preds = %if.then5
  call void @__mementos_erase_segment(i16 -1024)
  br label %if.end

if.end:                                           ; preds = %if.then7, %if.then5
  br label %if.end12

if.else:                                          ; preds = %if.then
  %call8 = call i16 @__mementos_segment_marked_erase(i16 -1536)
  %tobool9 = icmp ne i16 %call8, 0
  br i1 %tobool9, label %if.then10, label %if.end11

if.then10:                                        ; preds = %if.else
  call void @__mementos_erase_segment(i16 -1536)
  br label %if.end11

if.end11:                                         ; preds = %if.then10, %if.else
  br label %if.end12

if.end12:                                         ; preds = %if.end11, %if.end
  %tmp13 = load i16* @i, align 2
  call void @__mementos_restore(i16 %tmp13)
  store i16 13, i16* %retval
  br label %return

if.end14:                                         ; preds = %land.lhs.true, %entry
  %call15 = call i16 @_old_main()
  store i16 0, i16* %retval
  br label %return

return:                                           ; preds = %if.end14, %if.end12
  %0 = load i16* %retval
  ret i16 %0
}

!0 = metadata !{i32 1525}
!1 = metadata !{i32 1764, i32 1777, i32 1837, i32 1906, i32 1973, i32 2080, i32 2144, i32 2208, i32 2272, i32 2336, i32 2400, i32 2465, i32 2529, i32 2592, i32 2655, i32 2718}
!2 = metadata !{i32 2962}
!3 = metadata !{i32 3680, i32 3694}
!4 = metadata !{i32 3829}
!5 = metadata !{i32 3957}
!6 = metadata !{i32 3988}
!7 = metadata !{i32 4069}
!8 = metadata !{i32 4122}
!9 = metadata !{i32 4251}
!10 = metadata !{i32 4302}
!11 = metadata !{i32 4420}
!12 = metadata !{i32 4478}
!13 = metadata !{i32 4508}
!14 = metadata !{i32 4599}
!15 = metadata !{i32 4640}
!16 = metadata !{i32 4670}
!17 = metadata !{i32 4761}
!18 = metadata !{i32 4812, i32 4826, i32 4869, i32 4912, i32 4955, i32 4998, i32 5041, i32 5084, i32 5126, i32 5168, i32 5210, i32 5252, i32 5293, i32 5335, i32 5377}
!19 = metadata !{i32 6669}
!20 = metadata !{i32 21852}
!21 = metadata !{i32 22003}
!22 = metadata !{i32 8020}
!23 = metadata !{i32 8085}
!24 = metadata !{i32 8140}
!25 = metadata !{i32 8242}
!26 = metadata !{i32 8304}
!27 = metadata !{i32 8351}
!28 = metadata !{i32 8412}
!29 = metadata !{i32 8502}
!30 = metadata !{i32 8548}
!31 = metadata !{i32 8594}
!32 = metadata !{i32 9088}
!33 = metadata !{i32 9156}
!34 = metadata !{i32 9222, i32 9236}
!35 = metadata !{i32 9374}
!36 = metadata !{i32 9432}
!37 = metadata !{i32 9507, i32 9520}
!38 = metadata !{i32 9641, i32 9655, i32 9719}
!39 = metadata !{i32 9841}
!40 = metadata !{i32 9988}
!41 = metadata !{i32 10047}
!42 = metadata !{i32 10120}
!43 = metadata !{i32 10240}
!44 = metadata !{i32 10516}
!45 = metadata !{i32 10634}
!46 = metadata !{i32 10744, i32 10758, i32 10815, i32 10872, i32 10929, i32 10986, i32 11043, i32 11100, i32 11156, i32 11212, i32 11268, i32 11324, i32 11378, i32 11467, i32 11522}
!47 = metadata !{i32 11734, i32 11752, i32 11792, i32 11832, i32 11872, i32 11912, i32 11952, i32 11991, i32 12030, i32 12069, i32 12108, i32 12147, i32 12186, i32 12225}
!48 = metadata !{i32 12605}
