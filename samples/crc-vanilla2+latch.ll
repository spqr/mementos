; ModuleID = 'samples/crc-vanilla2+latch.bc'
target datalayout = "e-p:16:16:16-i8:8:8-i16:16:16-i32:16:32-n8:16"
target triple = "msp430-elf-"

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
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

define i16 @main() nounwind {
entry:
  %retval = alloca i16, align 2
  store i16 0, i16* %retval
  call void @do_crc()
  ret i16 27
}
