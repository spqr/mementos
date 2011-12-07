; ModuleID = 'rsa+plainclang.bc'
target datalayout = "e-p:16:16:16-i8:8:8-i16:16:16-i32:16:32-n8:16"
target triple = "msp430-elf-"

@test_rsa_encrypt.n = internal constant [32 x i16] [i16 -15592, i16 21590, i16 -20199, i16 24028, i16 -22073, i16 -25506, i16 2206, i16 -32115, i16 -10594, i16 -13555, i16 -4096, i16 -227, i16 11165, i16 11245, i16 -5686, i16 26504, i16 19521, i16 -32690, i16 -19762, i16 10294, i16 29158, i16 15343, i16 -17497, i16 -25631, i16 -29558, i16 -9788, i16 -26578, i16 -2440, i16 -14463, i16 12347, i16 18764, i16 -13451], align 2
@test_rsa_encrypt.e = internal constant [2 x i16] [i16 1, i16 1], align 2
@test_rsa_encrypt.plaintext = internal constant [32 x i16] [i16 1870, i16 -25599, i16 -26703, i16 26644, i16 -22585, i16 -19118, i16 -27739, i16 -14767, i16 -7599, i16 1328, i16 4558, i16 15781, i16 14320, i16 25269, i16 18885, i16 13000, i16 9848, i16 20785, i16 26688, i16 -18407, i16 28595, i16 10024, i16 -23949, i16 27405, i16 -27848, i16 21784, i16 -20581, i16 -25491, i16 -4372, i16 -5413, i16 -19676, i16 29934], align 2

define i16 @add_word(i16* %c_i, i16 %a_i, i16 %b_i, i16 %epsilon_prime) nounwind {
entry:
  %c_i.addr = alloca i16*, align 2
  %a_i.addr = alloca i16, align 2
  %b_i.addr = alloca i16, align 2
  %epsilon_prime.addr = alloca i16, align 2
  %epsilon = alloca i16, align 2
  store i16* %c_i, i16** %c_i.addr, align 2
  store i16 %a_i, i16* %a_i.addr, align 2
  store i16 %b_i, i16* %b_i.addr, align 2
  store i16 %epsilon_prime, i16* %epsilon_prime.addr, align 2
  %tmp = load i16* %a_i.addr, align 2
  %tmp1 = load i16* %b_i.addr, align 2
  %add = add i16 %tmp, %tmp1
  %tmp2 = load i16* %epsilon_prime.addr, align 2
  %add3 = add i16 %add, %tmp2
  %tmp4 = load i16** %c_i.addr, align 2
  store i16 %add3, i16* %tmp4
  %tmp5 = load i16** %c_i.addr, align 2
  %tmp6 = load i16* %tmp5
  %tmp7 = load i16* %a_i.addr, align 2
  %tmp8 = load i16* %epsilon_prime.addr, align 2
  %add9 = add i16 %tmp7, %tmp8
  %cmp = icmp uge i16 %tmp6, %add9
  br i1 %cmp, label %land.lhs.true, label %if.else

land.lhs.true:                                    ; preds = %entry
  %tmp10 = load i16** %c_i.addr, align 2
  %tmp11 = load i16* %tmp10
  %tmp12 = load i16* %b_i.addr, align 2
  %cmp13 = icmp uge i16 %tmp11, %tmp12
  br i1 %cmp13, label %if.then, label %if.else

if.then:                                          ; preds = %land.lhs.true
  store i16 0, i16* %epsilon, align 2
  br label %if.end

if.else:                                          ; preds = %land.lhs.true, %entry
  store i16 1, i16* %epsilon, align 2
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %tmp14 = load i16* %epsilon, align 2
  ret i16 %tmp14
}

define zeroext i8 @subtract_word(i16* %c_i, i16 %a_i, i16 %b_i, i8 zeroext %epsilon_prime) nounwind {
entry:
  %c_i.addr = alloca i16*, align 2
  %a_i.addr = alloca i16, align 2
  %b_i.addr = alloca i16, align 2
  %epsilon_prime.addr = alloca i8, align 1
  %epsilon = alloca i8, align 1
  store i16* %c_i, i16** %c_i.addr, align 2
  store i16 %a_i, i16* %a_i.addr, align 2
  store i16 %b_i, i16* %b_i.addr, align 2
  store i8 %epsilon_prime, i8* %epsilon_prime.addr, align 1
  %tmp = load i16* %a_i.addr, align 2
  %tmp1 = load i16* %b_i.addr, align 2
  %sub = sub i16 %tmp, %tmp1
  %tmp2 = load i8* %epsilon_prime.addr, align 1
  %conv = zext i8 %tmp2 to i16
  %sub3 = sub i16 %sub, %conv
  %tmp4 = load i16** %c_i.addr, align 2
  store i16 %sub3, i16* %tmp4
  %tmp5 = load i16* %b_i.addr, align 2
  %cmp = icmp eq i16 -1, %tmp5
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  store i8 1, i8* %epsilon, align 1
  br label %if.end13

if.else:                                          ; preds = %entry
  %tmp7 = load i16* %a_i.addr, align 2
  %tmp8 = load i16* %b_i.addr, align 2
  %cmp9 = icmp ult i16 %tmp7, %tmp8
  br i1 %cmp9, label %if.then11, label %if.else12

if.then11:                                        ; preds = %if.else
  store i8 1, i8* %epsilon, align 1
  br label %if.end

if.else12:                                        ; preds = %if.else
  store i8 0, i8* %epsilon, align 1
  br label %if.end

if.end:                                           ; preds = %if.else12, %if.then11
  br label %if.end13

if.end13:                                         ; preds = %if.end, %if.then
  %tmp14 = load i8* %epsilon, align 1
  ret i8 %tmp14
}

define i16 @add_mp_elements(i16* %pfe_c, i16* %pfe_a, i16* %pfe_b, i8 zeroext %wordlength) nounwind {
entry:
  %pfe_c.addr = alloca i16*, align 2
  %pfe_a.addr = alloca i16*, align 2
  %pfe_b.addr = alloca i16*, align 2
  %wordlength.addr = alloca i8, align 1
  %epsilon = alloca i16, align 2
  %i = alloca i16, align 2
  store i16* %pfe_c, i16** %pfe_c.addr, align 2
  store i16* %pfe_a, i16** %pfe_a.addr, align 2
  store i16* %pfe_b, i16** %pfe_b.addr, align 2
  store i8 %wordlength, i8* %wordlength.addr, align 1
  %tmp = load i16** %pfe_c.addr, align 2
  %arrayidx = getelementptr inbounds i16* %tmp, i16 0
  %tmp1 = load i16** %pfe_a.addr, align 2
  %arrayidx2 = getelementptr inbounds i16* %tmp1, i16 0
  %tmp3 = load i16* %arrayidx2
  %tmp4 = load i16** %pfe_b.addr, align 2
  %arrayidx5 = getelementptr inbounds i16* %tmp4, i16 0
  %tmp6 = load i16* %arrayidx5
  %call = call i16 @add_word(i16* %arrayidx, i16 %tmp3, i16 %tmp6, i16 0)
  store i16 %call, i16* %epsilon, align 2
  store i16 1, i16* %i, align 2
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %tmp7 = load i16* %i, align 2
  %tmp8 = load i8* %wordlength.addr, align 1
  %conv = zext i8 %tmp8 to i16
  %cmp = icmp slt i16 %tmp7, %conv
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %tmp10 = load i16* %i, align 2
  %tmp11 = load i16** %pfe_c.addr, align 2
  %arrayidx12 = getelementptr inbounds i16* %tmp11, i16 %tmp10
  %tmp13 = load i16* %i, align 2
  %tmp14 = load i16** %pfe_a.addr, align 2
  %arrayidx15 = getelementptr inbounds i16* %tmp14, i16 %tmp13
  %tmp16 = load i16* %arrayidx15
  %tmp17 = load i16* %i, align 2
  %tmp18 = load i16** %pfe_b.addr, align 2
  %arrayidx19 = getelementptr inbounds i16* %tmp18, i16 %tmp17
  %tmp20 = load i16* %arrayidx19
  %tmp21 = load i16* %epsilon, align 2
  %call22 = call i16 @add_word(i16* %arrayidx12, i16 %tmp16, i16 %tmp20, i16 %tmp21)
  store i16 %call22, i16* %epsilon, align 2
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %tmp23 = load i16* %i, align 2
  %inc = add nsw i16 %tmp23, 1
  store i16 %inc, i16* %i, align 2
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %tmp24 = load i16* %epsilon, align 2
  ret i16 %tmp24
}

define i16 @subtract_mp_elements(i16* %pfe_c, i16* %pfe_a, i16* %pfe_b, i8 zeroext %wordlength) nounwind {
entry:
  %pfe_c.addr = alloca i16*, align 2
  %pfe_a.addr = alloca i16*, align 2
  %pfe_b.addr = alloca i16*, align 2
  %wordlength.addr = alloca i8, align 1
  %epsilon = alloca i16, align 2
  %i = alloca i16, align 2
  store i16* %pfe_c, i16** %pfe_c.addr, align 2
  store i16* %pfe_a, i16** %pfe_a.addr, align 2
  store i16* %pfe_b, i16** %pfe_b.addr, align 2
  store i8 %wordlength, i8* %wordlength.addr, align 1
  %tmp = load i16** %pfe_c.addr, align 2
  %arrayidx = getelementptr inbounds i16* %tmp, i16 0
  %tmp1 = load i16** %pfe_a.addr, align 2
  %arrayidx2 = getelementptr inbounds i16* %tmp1, i16 0
  %tmp3 = load i16* %arrayidx2
  %tmp4 = load i16** %pfe_b.addr, align 2
  %arrayidx5 = getelementptr inbounds i16* %tmp4, i16 0
  %tmp6 = load i16* %arrayidx5
  %call = call zeroext i8 @subtract_word(i16* %arrayidx, i16 %tmp3, i16 %tmp6, i8 zeroext 0)
  %conv = zext i8 %call to i16
  store i16 %conv, i16* %epsilon, align 2
  store i16 1, i16* %i, align 2
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %tmp7 = load i16* %i, align 2
  %tmp8 = load i8* %wordlength.addr, align 1
  %conv9 = zext i8 %tmp8 to i16
  %cmp = icmp slt i16 %tmp7, %conv9
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %tmp11 = load i16* %i, align 2
  %tmp12 = load i16** %pfe_c.addr, align 2
  %arrayidx13 = getelementptr inbounds i16* %tmp12, i16 %tmp11
  %tmp14 = load i16* %i, align 2
  %tmp15 = load i16** %pfe_a.addr, align 2
  %arrayidx16 = getelementptr inbounds i16* %tmp15, i16 %tmp14
  %tmp17 = load i16* %arrayidx16
  %tmp18 = load i16* %i, align 2
  %tmp19 = load i16** %pfe_b.addr, align 2
  %arrayidx20 = getelementptr inbounds i16* %tmp19, i16 %tmp18
  %tmp21 = load i16* %arrayidx20
  %tmp22 = load i16* %epsilon, align 2
  %conv23 = trunc i16 %tmp22 to i8
  %call24 = call zeroext i8 @subtract_word(i16* %arrayidx13, i16 %tmp17, i16 %tmp21, i8 zeroext %conv23)
  %conv25 = zext i8 %call24 to i16
  store i16 %conv25, i16* %epsilon, align 2
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %tmp26 = load i16* %i, align 2
  %inc = add nsw i16 %tmp26, 1
  store i16 %inc, i16* %i, align 2
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %tmp27 = load i16* %epsilon, align 2
  ret i16 %tmp27
}

define void @add_mod_p(i16* %c, i16* %a, i16* %b, i16* %p, i8 zeroext %wordlength) nounwind {
entry:
  %c.addr = alloca i16*, align 2
  %a.addr = alloca i16*, align 2
  %b.addr = alloca i16*, align 2
  %p.addr = alloca i16*, align 2
  %wordlength.addr = alloca i8, align 1
  %epsilon = alloca i8, align 1
  store i16* %c, i16** %c.addr, align 2
  store i16* %a, i16** %a.addr, align 2
  store i16* %b, i16** %b.addr, align 2
  store i16* %p, i16** %p.addr, align 2
  store i8 %wordlength, i8* %wordlength.addr, align 1
  %tmp = load i16** %c.addr, align 2
  %tmp1 = load i16** %a.addr, align 2
  %tmp2 = load i16** %b.addr, align 2
  %tmp3 = load i8* %wordlength.addr, align 1
  %call = call i16 @add_mp_elements(i16* %tmp, i16* %tmp1, i16* %tmp2, i8 zeroext %tmp3)
  %conv = trunc i16 %call to i8
  store i8 %conv, i8* %epsilon, align 1
  %tmp4 = load i8* %epsilon, align 1
  %conv5 = zext i8 %tmp4 to i16
  %cmp = icmp eq i16 1, %conv5
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %tmp7 = load i16** %c.addr, align 2
  %tmp8 = load i16** %c.addr, align 2
  %tmp9 = load i16** %p.addr, align 2
  %tmp10 = load i8* %wordlength.addr, align 1
  %call11 = call i16 @subtract_mp_elements(i16* %tmp7, i16* %tmp8, i16* %tmp9, i8 zeroext %tmp10)
  br label %if.end25

if.else:                                          ; preds = %entry
  %tmp12 = load i16** %c.addr, align 2
  %tmp13 = load i16** %p.addr, align 2
  %tmp14 = load i8* %wordlength.addr, align 1
  %call15 = call zeroext i8 @compare_mp_elements(i16* %tmp12, i16* %tmp13, i8 zeroext %tmp14)
  %conv16 = zext i8 %call15 to i16
  %cmp17 = icmp eq i16 1, %conv16
  br i1 %cmp17, label %if.then19, label %if.end

if.then19:                                        ; preds = %if.else
  %tmp20 = load i16** %c.addr, align 2
  %tmp21 = load i16** %c.addr, align 2
  %tmp22 = load i16** %p.addr, align 2
  %tmp23 = load i8* %wordlength.addr, align 1
  %call24 = call i16 @subtract_mp_elements(i16* %tmp20, i16* %tmp21, i16* %tmp22, i8 zeroext %tmp23)
  br label %if.end

if.end:                                           ; preds = %if.then19, %if.else
  br label %if.end25

if.end25:                                         ; preds = %if.end, %if.then
  ret void
}

define zeroext i8 @compare_mp_elements(i16* %a, i16* %b, i8 zeroext %wordlength) nounwind {
entry:
  %retval = alloca i8, align 1
  %a.addr = alloca i16*, align 2
  %b.addr = alloca i16*, align 2
  %wordlength.addr = alloca i8, align 1
  %i = alloca i16, align 2
  store i16* %a, i16** %a.addr, align 2
  store i16* %b, i16** %b.addr, align 2
  store i8 %wordlength, i8* %wordlength.addr, align 1
  %tmp = load i8* %wordlength.addr, align 1
  %conv = zext i8 %tmp to i16
  %sub = sub nsw i16 %conv, 1
  store i16 %sub, i16* %i, align 2
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %tmp1 = load i16* %i, align 2
  %cmp = icmp sgt i16 %tmp1, -1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %tmp3 = load i16* %i, align 2
  %tmp4 = load i16** %a.addr, align 2
  %arrayidx = getelementptr inbounds i16* %tmp4, i16 %tmp3
  %tmp5 = load i16* %arrayidx
  %tmp6 = load i16* %i, align 2
  %tmp7 = load i16** %b.addr, align 2
  %arrayidx8 = getelementptr inbounds i16* %tmp7, i16 %tmp6
  %tmp9 = load i16* %arrayidx8
  %cmp10 = icmp ult i16 %tmp5, %tmp9
  br i1 %cmp10, label %if.then, label %if.end

if.then:                                          ; preds = %for.body
  store i8 0, i8* %retval
  br label %return

if.end:                                           ; preds = %for.body
  %tmp12 = load i16* %i, align 2
  %tmp13 = load i16** %a.addr, align 2
  %arrayidx14 = getelementptr inbounds i16* %tmp13, i16 %tmp12
  %tmp15 = load i16* %arrayidx14
  %tmp16 = load i16* %i, align 2
  %tmp17 = load i16** %b.addr, align 2
  %arrayidx18 = getelementptr inbounds i16* %tmp17, i16 %tmp16
  %tmp19 = load i16* %arrayidx18
  %cmp20 = icmp ugt i16 %tmp15, %tmp19
  br i1 %cmp20, label %if.then22, label %if.end23

if.then22:                                        ; preds = %if.end
  store i8 1, i8* %retval
  br label %return

if.end23:                                         ; preds = %if.end
  br label %for.inc

for.inc:                                          ; preds = %if.end23
  %tmp24 = load i16* %i, align 2
  %dec = add nsw i16 %tmp24, -1
  store i16 %dec, i16* %i, align 2
  br label %for.cond

for.end:                                          ; preds = %for.cond
  store i8 1, i8* %retval
  br label %return

return:                                           ; preds = %for.end, %if.then22, %if.then
  %0 = load i8* %retval
  ret i8 %0
}

define void @subtract_mod_p(i16* %c, i16* %a, i16* %b, i16* %p, i8 zeroext %wordlength) nounwind {
entry:
  %c.addr = alloca i16*, align 2
  %a.addr = alloca i16*, align 2
  %b.addr = alloca i16*, align 2
  %p.addr = alloca i16*, align 2
  %wordlength.addr = alloca i8, align 1
  %epsilon = alloca i8, align 1
  store i16* %c, i16** %c.addr, align 2
  store i16* %a, i16** %a.addr, align 2
  store i16* %b, i16** %b.addr, align 2
  store i16* %p, i16** %p.addr, align 2
  store i8 %wordlength, i8* %wordlength.addr, align 1
  %tmp = load i16** %c.addr, align 2
  %tmp1 = load i16** %a.addr, align 2
  %tmp2 = load i16** %b.addr, align 2
  %tmp3 = load i8* %wordlength.addr, align 1
  %call = call i16 @subtract_mp_elements(i16* %tmp, i16* %tmp1, i16* %tmp2, i8 zeroext %tmp3)
  %conv = trunc i16 %call to i8
  store i8 %conv, i8* %epsilon, align 1
  %tmp4 = load i8* %epsilon, align 1
  %conv5 = zext i8 %tmp4 to i16
  %cmp = icmp eq i16 1, %conv5
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %tmp7 = load i16** %c.addr, align 2
  %tmp8 = load i16** %c.addr, align 2
  %tmp9 = load i16** %p.addr, align 2
  %tmp10 = load i8* %wordlength.addr, align 1
  %call11 = call i16 @add_mp_elements(i16* %tmp7, i16* %tmp8, i16* %tmp9, i8 zeroext %tmp10)
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  ret void
}

define void @set_to_zero(i16* %c, i8 zeroext %wordlength) nounwind {
entry:
  %c.addr = alloca i16*, align 2
  %wordlength.addr = alloca i8, align 1
  %i = alloca i16, align 2
  store i16* %c, i16** %c.addr, align 2
  store i8 %wordlength, i8* %wordlength.addr, align 1
  store i16 0, i16* %i, align 2
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %tmp = load i16* %i, align 2
  %tmp1 = load i8* %wordlength.addr, align 1
  %conv = zext i8 %tmp1 to i16
  %cmp = icmp slt i16 %tmp, %conv
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %tmp3 = load i16* %i, align 2
  %tmp4 = load i16** %c.addr, align 2
  %arrayidx = getelementptr inbounds i16* %tmp4, i16 %tmp3
  store i16 0, i16* %arrayidx
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %tmp5 = load i16* %i, align 2
  %inc = add nsw i16 %tmp5, 1
  store i16 %inc, i16* %i, align 2
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

define void @multiply_words(i16 %a, i16 %b, i16* %uv) nounwind {
entry:
  %a.addr = alloca i16, align 2
  %b.addr = alloca i16, align 2
  %uv.addr = alloca i16*, align 2
  %uv_32 = alloca i32, align 2
  %u = alloca i16, align 2
  %v = alloca i16, align 2
  store i16 %a, i16* %a.addr, align 2
  store i16 %b, i16* %b.addr, align 2
  store i16* %uv, i16** %uv.addr, align 2
  %tmp = load i16* %a.addr, align 2
  %conv = zext i16 %tmp to i32
  %tmp1 = load i16* %b.addr, align 2
  %conv2 = zext i16 %tmp1 to i32
  %mul = mul i32 %conv, %conv2
  store i32 %mul, i32* %uv_32, align 2
  %tmp3 = load i32* %uv_32, align 2
  %shr = lshr i32 %tmp3, 16
  %conv4 = trunc i32 %shr to i16
  store i16 %conv4, i16* %u, align 2
  %tmp5 = load i32* %uv_32, align 2
  %conv6 = trunc i32 %tmp5 to i16
  store i16 %conv6, i16* %v, align 2
  %tmp7 = load i16* %u, align 2
  %tmp8 = load i16** %uv.addr, align 2
  %arrayidx = getelementptr inbounds i16* %tmp8, i16 1
  store i16 %tmp7, i16* %arrayidx
  %tmp9 = load i16* %v, align 2
  %tmp10 = load i16** %uv.addr, align 2
  %arrayidx11 = getelementptr inbounds i16* %tmp10, i16 0
  store i16 %tmp9, i16* %arrayidx11
  ret void
}

define void @multiply_words_2(i16 %a, i16 %b, i16* %uv) nounwind {
entry:
  %a.addr = alloca i16, align 2
  %b.addr = alloca i16, align 2
  %uv.addr = alloca i16*, align 2
  %a0 = alloca i16, align 2
  %a1 = alloca i16, align 2
  %b0 = alloca i16, align 2
  %b1 = alloca i16, align 2
  %t = alloca [2 x i16], align 2
  %s = alloca [2 x i16], align 2
  %m = alloca i16, align 2
  %borrow = alloca i16, align 2
  store i16 %a, i16* %a.addr, align 2
  store i16 %b, i16* %b.addr, align 2
  store i16* %uv, i16** %uv.addr, align 2
  %tmp = load i16* %a.addr, align 2
  %and = and i16 %tmp, -256
  %shr = lshr i16 %and, 8
  store i16 %shr, i16* %a0, align 2
  %tmp1 = load i16* %a.addr, align 2
  %and2 = and i16 %tmp1, 255
  store i16 %and2, i16* %a1, align 2
  %tmp3 = load i16* %b.addr, align 2
  %and4 = and i16 %tmp3, -256
  %shr5 = lshr i16 %and4, 8
  store i16 %shr5, i16* %b0, align 2
  %tmp6 = load i16* %b.addr, align 2
  %and7 = and i16 %tmp6, 255
  store i16 %and7, i16* %b1, align 2
  %tmp8 = load i16* %a1, align 2
  %tmp9 = load i16* %b1, align 2
  %mul = mul i16 %tmp8, %tmp9
  store i16 %mul, i16* %m, align 2
  %tmp10 = load i16* %m, align 2
  %and11 = and i16 %tmp10, 255
  %arrayidx = getelementptr inbounds [2 x i16]* %t, i32 0, i16 0
  store i16 %and11, i16* %arrayidx
  %tmp12 = load i16* %m, align 2
  %and13 = and i16 %tmp12, -256
  %shr14 = lshr i16 %and13, 8
  store i16 %shr14, i16* %borrow, align 2
  %tmp15 = load i16* %a0, align 2
  %tmp16 = load i16* %b1, align 2
  %mul17 = mul i16 %tmp15, %tmp16
  %tmp18 = load i16* %borrow, align 2
  %add = add i16 %mul17, %tmp18
  store i16 %add, i16* %m, align 2
  %tmp19 = load i16* %m, align 2
  %and20 = and i16 %tmp19, 255
  %shl = shl i16 %and20, 8
  %arrayidx21 = getelementptr inbounds [2 x i16]* %t, i32 0, i16 0
  %tmp22 = load i16* %arrayidx21
  %xor = xor i16 %tmp22, %shl
  store i16 %xor, i16* %arrayidx21
  %tmp23 = load i16* %m, align 2
  %and24 = and i16 %tmp23, -256
  %shr25 = lshr i16 %and24, 8
  %arrayidx26 = getelementptr inbounds [2 x i16]* %t, i32 0, i16 1
  store i16 %shr25, i16* %arrayidx26
  %tmp27 = load i16* %a1, align 2
  %tmp28 = load i16* %b0, align 2
  %mul29 = mul i16 %tmp27, %tmp28
  store i16 %mul29, i16* %m, align 2
  %tmp30 = load i16* %m, align 2
  %and31 = and i16 %tmp30, 255
  %shl32 = shl i16 %and31, 8
  %arrayidx33 = getelementptr inbounds [2 x i16]* %s, i32 0, i16 0
  store i16 %shl32, i16* %arrayidx33
  %tmp34 = load i16* %m, align 2
  %and35 = and i16 %tmp34, -256
  %shr36 = lshr i16 %and35, 8
  store i16 %shr36, i16* %borrow, align 2
  %tmp37 = load i16* %a0, align 2
  %tmp38 = load i16* %b0, align 2
  %mul39 = mul i16 %tmp37, %tmp38
  %tmp40 = load i16* %borrow, align 2
  %add41 = add i16 %mul39, %tmp40
  store i16 %add41, i16* %m, align 2
  %tmp42 = load i16* %m, align 2
  %arrayidx43 = getelementptr inbounds [2 x i16]* %s, i32 0, i16 1
  store i16 %tmp42, i16* %arrayidx43
  %tmp44 = load i16** %uv.addr, align 2
  %arraydecay = getelementptr inbounds [2 x i16]* %s, i32 0, i32 0
  %arraydecay45 = getelementptr inbounds [2 x i16]* %t, i32 0, i32 0
  %call = call i16 @add_mp_elements(i16* %tmp44, i16* %arraydecay, i16* %arraydecay45, i8 zeroext 2)
  ret void
}

define void @multiply_mp_elements(i16* %c, i16* %a, i16* %b, i8 zeroext %wordlength) nounwind {
entry:
  %c.addr = alloca i16*, align 2
  %a.addr = alloca i16*, align 2
  %b.addr = alloca i16*, align 2
  %wordlength.addr = alloca i8, align 1
  %UV = alloca [2 x i16], align 2
  %temp1 = alloca [2 x i16], align 2
  %temp2 = alloca [2 x i16], align 2
  %i = alloca i16, align 2
  %j = alloca i16, align 2
  store i16* %c, i16** %c.addr, align 2
  store i16* %a, i16** %a.addr, align 2
  store i16* %b, i16** %b.addr, align 2
  store i8 %wordlength, i8* %wordlength.addr, align 1
  %tmp = load i16** %c.addr, align 2
  %tmp1 = load i8* %wordlength.addr, align 1
  %conv = zext i8 %tmp1 to i16
  %mul = mul nsw i16 2, %conv
  %conv2 = trunc i16 %mul to i8
  call void @set_to_zero(i16* %tmp, i8 zeroext %conv2)
  %arraydecay = getelementptr inbounds [2 x i16]* %UV, i32 0, i32 0
  call void @set_to_zero(i16* %arraydecay, i8 zeroext 2)
  store i16 0, i16* %i, align 2
  br label %for.cond

for.cond:                                         ; preds = %for.inc57, %entry
  %tmp3 = load i16* %i, align 2
  %tmp4 = load i8* %wordlength.addr, align 1
  %conv5 = zext i8 %tmp4 to i16
  %cmp = icmp slt i16 %tmp3, %conv5
  br i1 %cmp, label %for.body, label %for.end60

for.body:                                         ; preds = %for.cond
  %arrayidx = getelementptr inbounds [2 x i16]* %UV, i32 0, i16 1
  store i16 0, i16* %arrayidx
  store i16 0, i16* %j, align 2
  br label %for.cond7

for.cond7:                                        ; preds = %for.inc, %for.body
  %tmp8 = load i16* %j, align 2
  %tmp9 = load i8* %wordlength.addr, align 1
  %conv10 = zext i8 %tmp9 to i16
  %cmp11 = icmp slt i16 %tmp8, %conv10
  br i1 %cmp11, label %for.body13, label %for.end

for.body13:                                       ; preds = %for.cond7
  %arrayidx14 = getelementptr inbounds [2 x i16]* %UV, i32 0, i16 1
  %tmp15 = load i16* %arrayidx14
  %arrayidx16 = getelementptr inbounds [2 x i16]* %temp2, i32 0, i16 0
  store i16 %tmp15, i16* %arrayidx16
  %arrayidx17 = getelementptr inbounds [2 x i16]* %temp2, i32 0, i16 1
  store i16 0, i16* %arrayidx17
  %tmp18 = load i16* %i, align 2
  %tmp19 = load i16** %a.addr, align 2
  %arrayidx20 = getelementptr inbounds i16* %tmp19, i16 %tmp18
  %tmp21 = load i16* %arrayidx20
  %tmp22 = load i16* %j, align 2
  %tmp23 = load i16** %b.addr, align 2
  %arrayidx24 = getelementptr inbounds i16* %tmp23, i16 %tmp22
  %tmp25 = load i16* %arrayidx24
  %arraydecay26 = getelementptr inbounds [2 x i16]* %UV, i32 0, i32 0
  call void @multiply_words(i16 %tmp21, i16 %tmp25, i16* %arraydecay26)
  %tmp27 = load i16* %i, align 2
  %tmp28 = load i16* %j, align 2
  %add = add nsw i16 %tmp27, %tmp28
  %tmp29 = load i16** %c.addr, align 2
  %arrayidx30 = getelementptr inbounds i16* %tmp29, i16 %add
  %tmp31 = load i16* %arrayidx30
  %arrayidx32 = getelementptr inbounds [2 x i16]* %temp1, i32 0, i16 0
  store i16 %tmp31, i16* %arrayidx32
  %arrayidx33 = getelementptr inbounds [2 x i16]* %temp1, i32 0, i16 1
  store i16 0, i16* %arrayidx33
  %arraydecay34 = getelementptr inbounds [2 x i16]* %UV, i32 0, i32 0
  %arraydecay35 = getelementptr inbounds [2 x i16]* %UV, i32 0, i32 0
  %arraydecay36 = getelementptr inbounds [2 x i16]* %temp1, i32 0, i32 0
  %call = call i16 @add_mp_elements(i16* %arraydecay34, i16* %arraydecay35, i16* %arraydecay36, i8 zeroext 2)
  %arraydecay37 = getelementptr inbounds [2 x i16]* %UV, i32 0, i32 0
  %arraydecay38 = getelementptr inbounds [2 x i16]* %UV, i32 0, i32 0
  %arraydecay39 = getelementptr inbounds [2 x i16]* %temp2, i32 0, i32 0
  %call40 = call i16 @add_mp_elements(i16* %arraydecay37, i16* %arraydecay38, i16* %arraydecay39, i8 zeroext 2)
  %arrayidx41 = getelementptr inbounds [2 x i16]* %UV, i32 0, i16 0
  %tmp42 = load i16* %arrayidx41
  %tmp43 = load i16* %i, align 2
  %tmp44 = load i16* %j, align 2
  %add45 = add nsw i16 %tmp43, %tmp44
  %tmp46 = load i16** %c.addr, align 2
  %arrayidx47 = getelementptr inbounds i16* %tmp46, i16 %add45
  store i16 %tmp42, i16* %arrayidx47
  br label %for.inc

for.inc:                                          ; preds = %for.body13
  %tmp48 = load i16* %j, align 2
  %inc = add nsw i16 %tmp48, 1
  store i16 %inc, i16* %j, align 2
  br label %for.cond7

for.end:                                          ; preds = %for.cond7
  %arrayidx49 = getelementptr inbounds [2 x i16]* %UV, i32 0, i16 1
  %tmp50 = load i16* %arrayidx49
  %tmp51 = load i16* %i, align 2
  %tmp52 = load i8* %wordlength.addr, align 1
  %conv53 = zext i8 %tmp52 to i16
  %add54 = add nsw i16 %tmp51, %conv53
  %tmp55 = load i16** %c.addr, align 2
  %arrayidx56 = getelementptr inbounds i16* %tmp55, i16 %add54
  store i16 %tmp50, i16* %arrayidx56
  br label %for.inc57

for.inc57:                                        ; preds = %for.end
  %tmp58 = load i16* %i, align 2
  %inc59 = add nsw i16 %tmp58, 1
  store i16 %inc59, i16* %i, align 2
  br label %for.cond

for.end60:                                        ; preds = %for.cond
  ret void
}

define void @multiply_mp_elements2(i16* %c, i16* %a, i8 zeroext %wordlength_a, i16* %b, i8 zeroext %wordlength_b) nounwind {
entry:
  %c.addr = alloca i16*, align 2
  %a.addr = alloca i16*, align 2
  %wordlength_a.addr = alloca i8, align 1
  %b.addr = alloca i16*, align 2
  %wordlength_b.addr = alloca i8, align 1
  %UV = alloca [2 x i16], align 2
  %temp1 = alloca [2 x i16], align 2
  %temp2 = alloca [2 x i16], align 2
  %i = alloca i16, align 2
  %j = alloca i16, align 2
  store i16* %c, i16** %c.addr, align 2
  store i16* %a, i16** %a.addr, align 2
  store i8 %wordlength_a, i8* %wordlength_a.addr, align 1
  store i16* %b, i16** %b.addr, align 2
  store i8 %wordlength_b, i8* %wordlength_b.addr, align 1
  %tmp = load i16** %c.addr, align 2
  %tmp1 = load i8* %wordlength_a.addr, align 1
  %conv = zext i8 %tmp1 to i16
  %tmp2 = load i8* %wordlength_b.addr, align 1
  %conv3 = zext i8 %tmp2 to i16
  %add = add nsw i16 %conv, %conv3
  %conv4 = trunc i16 %add to i8
  call void @set_to_zero(i16* %tmp, i8 zeroext %conv4)
  %arraydecay = getelementptr inbounds [2 x i16]* %UV, i32 0, i32 0
  call void @set_to_zero(i16* %arraydecay, i8 zeroext 2)
  store i16 0, i16* %i, align 2
  br label %for.cond

for.cond:                                         ; preds = %for.inc60, %entry
  %tmp5 = load i16* %i, align 2
  %tmp6 = load i8* %wordlength_b.addr, align 1
  %conv7 = zext i8 %tmp6 to i16
  %cmp = icmp slt i16 %tmp5, %conv7
  br i1 %cmp, label %for.body, label %for.end63

for.body:                                         ; preds = %for.cond
  %arrayidx = getelementptr inbounds [2 x i16]* %UV, i32 0, i16 1
  store i16 0, i16* %arrayidx
  store i16 0, i16* %j, align 2
  br label %for.cond9

for.cond9:                                        ; preds = %for.inc, %for.body
  %tmp10 = load i16* %j, align 2
  %tmp11 = load i8* %wordlength_a.addr, align 1
  %conv12 = zext i8 %tmp11 to i16
  %cmp13 = icmp slt i16 %tmp10, %conv12
  br i1 %cmp13, label %for.body15, label %for.end

for.body15:                                       ; preds = %for.cond9
  %arrayidx16 = getelementptr inbounds [2 x i16]* %UV, i32 0, i16 1
  %tmp17 = load i16* %arrayidx16
  %arrayidx18 = getelementptr inbounds [2 x i16]* %temp2, i32 0, i16 0
  store i16 %tmp17, i16* %arrayidx18
  %arrayidx19 = getelementptr inbounds [2 x i16]* %temp2, i32 0, i16 1
  store i16 0, i16* %arrayidx19
  %tmp20 = load i16* %i, align 2
  %tmp21 = load i16** %a.addr, align 2
  %arrayidx22 = getelementptr inbounds i16* %tmp21, i16 %tmp20
  %tmp23 = load i16* %arrayidx22
  %tmp24 = load i16* %j, align 2
  %tmp25 = load i16** %b.addr, align 2
  %arrayidx26 = getelementptr inbounds i16* %tmp25, i16 %tmp24
  %tmp27 = load i16* %arrayidx26
  %arraydecay28 = getelementptr inbounds [2 x i16]* %UV, i32 0, i32 0
  call void @multiply_words(i16 %tmp23, i16 %tmp27, i16* %arraydecay28)
  %tmp29 = load i16* %i, align 2
  %tmp30 = load i16* %j, align 2
  %add31 = add nsw i16 %tmp29, %tmp30
  %tmp32 = load i16** %c.addr, align 2
  %arrayidx33 = getelementptr inbounds i16* %tmp32, i16 %add31
  %tmp34 = load i16* %arrayidx33
  %arrayidx35 = getelementptr inbounds [2 x i16]* %temp1, i32 0, i16 0
  store i16 %tmp34, i16* %arrayidx35
  %arrayidx36 = getelementptr inbounds [2 x i16]* %temp1, i32 0, i16 1
  store i16 0, i16* %arrayidx36
  %arraydecay37 = getelementptr inbounds [2 x i16]* %UV, i32 0, i32 0
  %arraydecay38 = getelementptr inbounds [2 x i16]* %UV, i32 0, i32 0
  %arraydecay39 = getelementptr inbounds [2 x i16]* %temp1, i32 0, i32 0
  %call = call i16 @add_mp_elements(i16* %arraydecay37, i16* %arraydecay38, i16* %arraydecay39, i8 zeroext 2)
  %arraydecay40 = getelementptr inbounds [2 x i16]* %UV, i32 0, i32 0
  %arraydecay41 = getelementptr inbounds [2 x i16]* %UV, i32 0, i32 0
  %arraydecay42 = getelementptr inbounds [2 x i16]* %temp2, i32 0, i32 0
  %call43 = call i16 @add_mp_elements(i16* %arraydecay40, i16* %arraydecay41, i16* %arraydecay42, i8 zeroext 2)
  %arrayidx44 = getelementptr inbounds [2 x i16]* %UV, i32 0, i16 0
  %tmp45 = load i16* %arrayidx44
  %tmp46 = load i16* %i, align 2
  %tmp47 = load i16* %j, align 2
  %add48 = add nsw i16 %tmp46, %tmp47
  %tmp49 = load i16** %c.addr, align 2
  %arrayidx50 = getelementptr inbounds i16* %tmp49, i16 %add48
  store i16 %tmp45, i16* %arrayidx50
  br label %for.inc

for.inc:                                          ; preds = %for.body15
  %tmp51 = load i16* %j, align 2
  %inc = add nsw i16 %tmp51, 1
  store i16 %inc, i16* %j, align 2
  br label %for.cond9

for.end:                                          ; preds = %for.cond9
  %arrayidx52 = getelementptr inbounds [2 x i16]* %UV, i32 0, i16 1
  %tmp53 = load i16* %arrayidx52
  %tmp54 = load i16* %i, align 2
  %tmp55 = load i8* %wordlength_b.addr, align 1
  %conv56 = zext i8 %tmp55 to i16
  %add57 = add nsw i16 %tmp54, %conv56
  %tmp58 = load i16** %c.addr, align 2
  %arrayidx59 = getelementptr inbounds i16* %tmp58, i16 %add57
  store i16 %tmp53, i16* %arrayidx59
  br label %for.inc60

for.inc60:                                        ; preds = %for.end
  %tmp61 = load i16* %i, align 2
  %inc62 = add nsw i16 %tmp61, 1
  store i16 %inc62, i16* %i, align 2
  br label %for.cond

for.end63:                                        ; preds = %for.cond
  ret void
}

define void @mult_by_power_of_b(i16* %out, i16 %wordlength_out, i16* %a, i16 %wordlength_a, i16 %k) nounwind {
entry:
  %out.addr = alloca i16*, align 2
  %wordlength_out.addr = alloca i16, align 2
  %a.addr = alloca i16*, align 2
  %wordlength_a.addr = alloca i16, align 2
  %k.addr = alloca i16, align 2
  %i = alloca i16, align 2
  store i16* %out, i16** %out.addr, align 2
  store i16 %wordlength_out, i16* %wordlength_out.addr, align 2
  store i16* %a, i16** %a.addr, align 2
  store i16 %wordlength_a, i16* %wordlength_a.addr, align 2
  store i16 %k, i16* %k.addr, align 2
  store i16 0, i16* %i, align 2
  %tmp = load i16** %out.addr, align 2
  %tmp1 = load i16* %wordlength_out.addr, align 2
  %conv = trunc i16 %tmp1 to i8
  call void @set_to_zero(i16* %tmp, i8 zeroext %conv)
  br label %while.cond

while.cond:                                       ; preds = %if.end, %entry
  %tmp2 = load i16* %i, align 2
  %tmp3 = load i16* %k.addr, align 2
  %add = add i16 %tmp2, %tmp3
  %tmp4 = load i16* %wordlength_out.addr, align 2
  %cmp = icmp ult i16 %add, %tmp4
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %tmp6 = load i16* %i, align 2
  %tmp7 = load i16* %wordlength_a.addr, align 2
  %cmp8 = icmp ult i16 %tmp6, %tmp7
  br i1 %cmp8, label %if.then, label %if.end

if.then:                                          ; preds = %while.body
  %tmp10 = load i16* %i, align 2
  %tmp11 = load i16** %a.addr, align 2
  %arrayidx = getelementptr inbounds i16* %tmp11, i16 %tmp10
  %tmp12 = load i16* %arrayidx
  %tmp13 = load i16* %i, align 2
  %tmp14 = load i16* %k.addr, align 2
  %add15 = add i16 %tmp13, %tmp14
  %tmp16 = load i16** %out.addr, align 2
  %arrayidx17 = getelementptr inbounds i16* %tmp16, i16 %add15
  store i16 %tmp12, i16* %arrayidx17
  br label %if.end

if.end:                                           ; preds = %if.then, %while.body
  %tmp18 = load i16* %i, align 2
  %inc = add nsw i16 %tmp18, 1
  store i16 %inc, i16* %i, align 2
  br label %while.cond

while.end:                                        ; preds = %while.cond
  ret void
}

define void @mod_pow_of_b(i16* %out, i16 %wordlength_out, i16* %a, i16 %wordlength_a, i16 %k) nounwind {
entry:
  %out.addr = alloca i16*, align 2
  %wordlength_out.addr = alloca i16, align 2
  %a.addr = alloca i16*, align 2
  %wordlength_a.addr = alloca i16, align 2
  %k.addr = alloca i16, align 2
  %i = alloca i16, align 2
  store i16* %out, i16** %out.addr, align 2
  store i16 %wordlength_out, i16* %wordlength_out.addr, align 2
  store i16* %a, i16** %a.addr, align 2
  store i16 %wordlength_a, i16* %wordlength_a.addr, align 2
  store i16 %k, i16* %k.addr, align 2
  br label %while.cond

while.cond:                                       ; preds = %if.end, %entry
  %tmp = load i16* %i, align 2
  %tmp1 = load i16* %wordlength_out.addr, align 2
  %cmp = icmp ult i16 %tmp, %tmp1
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %tmp2 = load i16* %i, align 2
  %tmp3 = load i16* %wordlength_a.addr, align 2
  %cmp4 = icmp ult i16 %tmp2, %tmp3
  br i1 %cmp4, label %if.then, label %if.else

if.then:                                          ; preds = %while.body
  %tmp5 = load i16* %i, align 2
  %tmp6 = load i16** %a.addr, align 2
  %arrayidx = getelementptr inbounds i16* %tmp6, i16 %tmp5
  %tmp7 = load i16* %arrayidx
  %tmp8 = load i16* %i, align 2
  %tmp9 = load i16** %out.addr, align 2
  %arrayidx10 = getelementptr inbounds i16* %tmp9, i16 %tmp8
  store i16 %tmp7, i16* %arrayidx10
  br label %if.end

if.else:                                          ; preds = %while.body
  %tmp11 = load i16* %i, align 2
  %tmp12 = load i16** %out.addr, align 2
  %arrayidx13 = getelementptr inbounds i16* %tmp12, i16 %tmp11
  store i16 0, i16* %arrayidx13
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %tmp14 = load i16* %i, align 2
  %inc = add nsw i16 %tmp14, 1
  store i16 %inc, i16* %i, align 2
  br label %while.cond

while.end:                                        ; preds = %while.cond
  ret void
}

define void @div_by_power_of_b(i16* %out, i16 %out_len, i16* %a, i16 %a_len, i16 %k) nounwind {
entry:
  %out.addr = alloca i16*, align 2
  %out_len.addr = alloca i16, align 2
  %a.addr = alloca i16*, align 2
  %a_len.addr = alloca i16, align 2
  %k.addr = alloca i16, align 2
  %i = alloca i16, align 2
  store i16* %out, i16** %out.addr, align 2
  store i16 %out_len, i16* %out_len.addr, align 2
  store i16* %a, i16** %a.addr, align 2
  store i16 %a_len, i16* %a_len.addr, align 2
  store i16 %k, i16* %k.addr, align 2
  %tmp = load i16** %out.addr, align 2
  %tmp1 = load i16* %out_len.addr, align 2
  %conv = trunc i16 %tmp1 to i8
  call void @set_to_zero(i16* %tmp, i8 zeroext %conv)
  %tmp2 = load i16* %out_len.addr, align 2
  %add = add i16 %tmp2, 1
  %tmp3 = load i16* %a_len.addr, align 2
  %tmp4 = load i16* %k.addr, align 2
  %sub = sub i16 %tmp3, %tmp4
  %cmp = icmp ugt i16 %add, %sub
  br i1 %cmp, label %if.then, label %if.end26

if.then:                                          ; preds = %entry
  store i16 0, i16* %i, align 2
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %if.then
  %tmp6 = load i16* %i, align 2
  %tmp7 = load i16* %out_len.addr, align 2
  %cmp8 = icmp ult i16 %tmp6, %tmp7
  br i1 %cmp8, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %tmp10 = load i16* %k.addr, align 2
  %tmp11 = load i16* %i, align 2
  %add12 = add i16 %tmp10, %tmp11
  %tmp13 = load i16* %a_len.addr, align 2
  %cmp14 = icmp ult i16 %add12, %tmp13
  br i1 %cmp14, label %if.then16, label %if.end

if.then16:                                        ; preds = %for.body
  %tmp17 = load i16* %k.addr, align 2
  %tmp18 = load i16* %i, align 2
  %add19 = add i16 %tmp17, %tmp18
  %tmp20 = load i16** %a.addr, align 2
  %arrayidx = getelementptr inbounds i16* %tmp20, i16 %add19
  %tmp21 = load i16* %arrayidx
  %tmp22 = load i16* %i, align 2
  %tmp23 = load i16** %out.addr, align 2
  %arrayidx24 = getelementptr inbounds i16* %tmp23, i16 %tmp22
  store i16 %tmp21, i16* %arrayidx24
  br label %if.end

if.end:                                           ; preds = %if.then16, %for.body
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %tmp25 = load i16* %i, align 2
  %inc = add nsw i16 %tmp25, 1
  store i16 %inc, i16* %i, align 2
  br label %for.cond

for.end:                                          ; preds = %for.cond
  br label %if.end26

if.end26:                                         ; preds = %for.end, %entry
  ret void
}

define void @multiply_sp_by_mp_element(i16* %c, i16 %a, i16* %b, i16 %wordlength_b) nounwind {
entry:
  %c.addr = alloca i16*, align 2
  %a.addr = alloca i16, align 2
  %b.addr = alloca i16*, align 2
  %wordlength_b.addr = alloca i16, align 2
  %uv = alloca i32, align 2
  %u = alloca i16, align 2
  %v = alloca i16, align 2
  %carry = alloca i16, align 2
  %j = alloca i16, align 2
  store i16* %c, i16** %c.addr, align 2
  store i16 %a, i16* %a.addr, align 2
  store i16* %b, i16** %b.addr, align 2
  store i16 %wordlength_b, i16* %wordlength_b.addr, align 2
  %tmp = load i16** %c.addr, align 2
  %tmp1 = load i16* %wordlength_b.addr, align 2
  %add = add i16 %tmp1, 1
  %conv = trunc i16 %add to i8
  call void @set_to_zero(i16* %tmp, i8 zeroext %conv)
  store i32 0, i32* %uv, align 2
  store i16 0, i16* %carry, align 2
  store i16 0, i16* %j, align 2
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %tmp2 = load i16* %j, align 2
  %tmp3 = load i16* %wordlength_b.addr, align 2
  %cmp = icmp ult i16 %tmp2, %tmp3
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %tmp5 = load i16* %a.addr, align 2
  %conv6 = zext i16 %tmp5 to i32
  %tmp7 = load i16* %j, align 2
  %tmp8 = load i16** %b.addr, align 2
  %arrayidx = getelementptr inbounds i16* %tmp8, i16 %tmp7
  %tmp9 = load i16* %arrayidx
  %conv10 = zext i16 %tmp9 to i32
  %mul = mul i32 %conv6, %conv10
  %tmp11 = load i16* %carry, align 2
  %conv12 = zext i16 %tmp11 to i32
  %add13 = add i32 %mul, %conv12
  store i32 %add13, i32* %uv, align 2
  %tmp14 = load i32* %uv, align 2
  %shr = lshr i32 %tmp14, 16
  %conv15 = trunc i32 %shr to i16
  store i16 %conv15, i16* %u, align 2
  %tmp16 = load i32* %uv, align 2
  %conv17 = trunc i32 %tmp16 to i16
  store i16 %conv17, i16* %v, align 2
  %tmp18 = load i16* %v, align 2
  %tmp19 = load i16* %j, align 2
  %tmp20 = load i16** %c.addr, align 2
  %arrayidx21 = getelementptr inbounds i16* %tmp20, i16 %tmp19
  store i16 %tmp18, i16* %arrayidx21
  %tmp22 = load i16* %u, align 2
  store i16 %tmp22, i16* %carry, align 2
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %tmp23 = load i16* %j, align 2
  %inc = add nsw i16 %tmp23, 1
  store i16 %inc, i16* %j, align 2
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %tmp24 = load i16* %carry, align 2
  %tmp25 = load i16* %wordlength_b.addr, align 2
  %tmp26 = load i16** %c.addr, align 2
  %arrayidx27 = getelementptr inbounds i16* %tmp26, i16 %tmp25
  store i16 %tmp24, i16* %arrayidx27
  ret void
}

define i16 @are_mp_equal(i16* %a, i16* %b, i8 zeroext %wordlength) nounwind {
entry:
  %a.addr = alloca i16*, align 2
  %b.addr = alloca i16*, align 2
  %wordlength.addr = alloca i8, align 1
  %i = alloca i16, align 2
  %answ = alloca i16, align 2
  store i16* %a, i16** %a.addr, align 2
  store i16* %b, i16** %b.addr, align 2
  store i8 %wordlength, i8* %wordlength.addr, align 1
  store i16 0, i16* %i, align 2
  store i16 1, i16* %answ, align 2
  br label %while.cond

while.cond:                                       ; preds = %if.end, %entry
  %tmp = load i16* %answ, align 2
  %cmp = icmp eq i16 1, %tmp
  br i1 %cmp, label %land.rhs, label %land.end

land.rhs:                                         ; preds = %while.cond
  %tmp1 = load i16* %i, align 2
  %tmp2 = load i8* %wordlength.addr, align 1
  %conv = zext i8 %tmp2 to i16
  %cmp3 = icmp slt i16 %tmp1, %conv
  br label %land.end

land.end:                                         ; preds = %land.rhs, %while.cond
  %0 = phi i1 [ false, %while.cond ], [ %cmp3, %land.rhs ]
  br i1 %0, label %while.body, label %while.end

while.body:                                       ; preds = %land.end
  %tmp5 = load i16* %i, align 2
  %tmp6 = load i16** %a.addr, align 2
  %arrayidx = getelementptr inbounds i16* %tmp6, i16 %tmp5
  %tmp7 = load i16* %arrayidx
  %tmp8 = load i16* %i, align 2
  %tmp9 = load i16** %b.addr, align 2
  %arrayidx10 = getelementptr inbounds i16* %tmp9, i16 %tmp8
  %tmp11 = load i16* %arrayidx10
  %cmp12 = icmp ne i16 %tmp7, %tmp11
  br i1 %cmp12, label %if.then, label %if.end

if.then:                                          ; preds = %while.body
  store i16 0, i16* %answ, align 2
  br label %if.end

if.end:                                           ; preds = %if.then, %while.body
  %tmp14 = load i16* %i, align 2
  %inc = add nsw i16 %tmp14, 1
  store i16 %inc, i16* %i, align 2
  br label %while.cond

while.end:                                        ; preds = %land.end
  %tmp15 = load i16* %answ, align 2
  ret i16 %tmp15
}

define void @copy_mp(i16* %out, i16* %in, i16 %wordlength) nounwind {
entry:
  %out.addr = alloca i16*, align 2
  %in.addr = alloca i16*, align 2
  %wordlength.addr = alloca i16, align 2
  %i = alloca i16, align 2
  store i16* %out, i16** %out.addr, align 2
  store i16* %in, i16** %in.addr, align 2
  store i16 %wordlength, i16* %wordlength.addr, align 2
  store i16 0, i16* %i, align 2
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %tmp = load i16* %i, align 2
  %tmp1 = load i16* %wordlength.addr, align 2
  %cmp = icmp slt i16 %tmp, %tmp1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %tmp2 = load i16* %i, align 2
  %tmp3 = load i16** %in.addr, align 2
  %arrayidx = getelementptr inbounds i16* %tmp3, i16 %tmp2
  %tmp4 = load i16* %arrayidx
  %tmp5 = load i16* %i, align 2
  %tmp6 = load i16** %out.addr, align 2
  %arrayidx7 = getelementptr inbounds i16* %tmp6, i16 %tmp5
  store i16 %tmp4, i16* %arrayidx7
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %tmp8 = load i16* %i, align 2
  %inc = add nsw i16 %tmp8, 1
  store i16 %inc, i16* %i, align 2
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret void
}

define i16 @ith_bit(i16 %e, i16 %i) nounwind {
entry:
  %retval = alloca i16, align 2
  %e.addr = alloca i16, align 2
  %i.addr = alloca i16, align 2
  %mask = alloca i16, align 2
  store i16 %e, i16* %e.addr, align 2
  store i16 %i, i16* %i.addr, align 2
  %tmp = load i16* %i.addr, align 2
  %shl = shl i16 1, %tmp
  store i16 %shl, i16* %mask, align 2
  %tmp1 = load i16* %e.addr, align 2
  %tmp2 = load i16* %mask, align 2
  %and = and i16 %tmp1, %tmp2
  store i16 %and, i16* %mask, align 2
  %tmp3 = load i16* %mask, align 2
  %cmp = icmp eq i16 0, %tmp3
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  store i16 0, i16* %retval
  br label %return

if.else:                                          ; preds = %entry
  store i16 1, i16* %retval
  br label %return

return:                                           ; preds = %if.else, %if.then
  %0 = load i16* %retval
  ret i16 %0
}

define i16 @bit_length(i16 %e) nounwind {
entry:
  %e.addr = alloca i16, align 2
  %i = alloca i16, align 2
  %found_one = alloca i16, align 2
  store i16 %e, i16* %e.addr, align 2
  store i16 15, i16* %i, align 2
  store i16 0, i16* %found_one, align 2
  br label %while.cond

while.cond:                                       ; preds = %if.end, %entry
  %tmp = load i16* %found_one, align 2
  %cmp = icmp eq i16 0, %tmp
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %tmp1 = load i16* %e.addr, align 2
  %tmp2 = load i16* %i, align 2
  %call = call i16 @ith_bit(i16 %tmp1, i16 %tmp2)
  %cmp3 = icmp eq i16 1, %call
  br i1 %cmp3, label %if.then, label %if.else

if.then:                                          ; preds = %while.body
  store i16 1, i16* %found_one, align 2
  br label %if.end

if.else:                                          ; preds = %while.body
  %tmp4 = load i16* %i, align 2
  %dec = add nsw i16 %tmp4, -1
  store i16 %dec, i16* %i, align 2
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %tmp5 = load i16* %i, align 2
  ret i16 %tmp5
}

define i16 @mp_bit_length(i16* %e, i16 %wordlength) nounwind {
entry:
  %e.addr = alloca i16*, align 2
  %wordlength.addr = alloca i16, align 2
  %i = alloca i16, align 2
  %length = alloca i16, align 2
  %last_non_zero_word = alloca i16, align 2
  store i16* %e, i16** %e.addr, align 2
  store i16 %wordlength, i16* %wordlength.addr, align 2
  %tmp = load i16* %wordlength.addr, align 2
  %sub = sub i16 %tmp, 1
  store i16 %sub, i16* %i, align 2
  store i16 -1, i16* %last_non_zero_word, align 2
  br label %while.cond

while.cond:                                       ; preds = %if.end, %entry
  %tmp3 = load i16* %i, align 2
  %cmp = icmp sgt i16 %tmp3, -1
  br i1 %cmp, label %land.rhs, label %land.end

land.rhs:                                         ; preds = %while.cond
  %tmp4 = load i16* %last_non_zero_word, align 2
  %cmp5 = icmp slt i16 %tmp4, 0
  br label %land.end

land.end:                                         ; preds = %land.rhs, %while.cond
  %0 = phi i1 [ false, %while.cond ], [ %cmp5, %land.rhs ]
  br i1 %0, label %while.body, label %while.end

while.body:                                       ; preds = %land.end
  %tmp6 = load i16* %i, align 2
  %tmp7 = load i16** %e.addr, align 2
  %arrayidx = getelementptr inbounds i16* %tmp7, i16 %tmp6
  %tmp8 = load i16* %arrayidx
  %cmp9 = icmp ne i16 0, %tmp8
  br i1 %cmp9, label %if.then, label %if.end

if.then:                                          ; preds = %while.body
  %tmp10 = load i16* %i, align 2
  store i16 %tmp10, i16* %last_non_zero_word, align 2
  br label %if.end

if.end:                                           ; preds = %if.then, %while.body
  %tmp11 = load i16* %i, align 2
  %dec = add nsw i16 %tmp11, -1
  store i16 %dec, i16* %i, align 2
  br label %while.cond

while.end:                                        ; preds = %land.end
  %tmp12 = load i16* %last_non_zero_word, align 2
  %mul = mul nsw i16 16, %tmp12
  %tmp13 = load i16* %last_non_zero_word, align 2
  %tmp14 = load i16** %e.addr, align 2
  %arrayidx15 = getelementptr inbounds i16* %tmp14, i16 %tmp13
  %tmp16 = load i16* %arrayidx15
  %call = call i16 @bit_length(i16 %tmp16)
  %add = add nsw i16 %mul, %call
  store i16 %add, i16* %length, align 2
  %tmp17 = load i16* %length, align 2
  ret i16 %tmp17
}

define i16 @mp_ith_bit(i16* %e, i16 %i) nounwind {
entry:
  %e.addr = alloca i16*, align 2
  %i.addr = alloca i16, align 2
  %word = alloca i16, align 2
  %word_bit = alloca i16, align 2
  store i16* %e, i16** %e.addr, align 2
  store i16 %i, i16* %i.addr, align 2
  %tmp = load i16* %i.addr, align 2
  %div = sdiv i16 %tmp, 16
  store i16 %div, i16* %word, align 2
  %tmp1 = load i16* %word, align 2
  %tmp2 = load i16** %e.addr, align 2
  %arrayidx = getelementptr inbounds i16* %tmp2, i16 %tmp1
  %tmp3 = load i16* %arrayidx
  %tmp4 = load i16* %i.addr, align 2
  %tmp5 = load i16* %word, align 2
  %mul = mul i16 %tmp5, 16
  %sub = sub i16 %tmp4, %mul
  %call = call i16 @ith_bit(i16 %tmp3, i16 %sub)
  store i16 %call, i16* %word_bit, align 2
  %tmp6 = load i16* %word_bit, align 2
  ret i16 %tmp6
}

define i16 @mp_non_zero_words(i16* %e, i16 %wordlength) nounwind {
entry:
  %e.addr = alloca i16*, align 2
  %wordlength.addr = alloca i16, align 2
  %i = alloca i16, align 2
  %last_non_zero_word = alloca i16, align 2
  store i16* %e, i16** %e.addr, align 2
  store i16 %wordlength, i16* %wordlength.addr, align 2
  %tmp = load i16* %wordlength.addr, align 2
  %sub = sub i16 %tmp, 1
  store i16 %sub, i16* %i, align 2
  store i16 -1, i16* %last_non_zero_word, align 2
  br label %while.cond

while.cond:                                       ; preds = %if.end, %entry
  %tmp2 = load i16* %i, align 2
  %cmp = icmp sgt i16 %tmp2, -1
  br i1 %cmp, label %land.rhs, label %land.end

land.rhs:                                         ; preds = %while.cond
  %tmp3 = load i16* %last_non_zero_word, align 2
  %cmp4 = icmp slt i16 %tmp3, 0
  br label %land.end

land.end:                                         ; preds = %land.rhs, %while.cond
  %0 = phi i1 [ false, %while.cond ], [ %cmp4, %land.rhs ]
  br i1 %0, label %while.body, label %while.end

while.body:                                       ; preds = %land.end
  %tmp5 = load i16* %i, align 2
  %tmp6 = load i16** %e.addr, align 2
  %arrayidx = getelementptr inbounds i16* %tmp6, i16 %tmp5
  %tmp7 = load i16* %arrayidx
  %cmp8 = icmp ne i16 0, %tmp7
  br i1 %cmp8, label %if.then, label %if.end

if.then:                                          ; preds = %while.body
  %tmp9 = load i16* %i, align 2
  store i16 %tmp9, i16* %last_non_zero_word, align 2
  br label %if.end

if.end:                                           ; preds = %if.then, %while.body
  %tmp10 = load i16* %i, align 2
  %dec = add nsw i16 %tmp10, -1
  store i16 %dec, i16* %i, align 2
  br label %while.cond

while.end:                                        ; preds = %land.end
  %tmp11 = load i16* %last_non_zero_word, align 2
  ret i16 %tmp11
}

define void @multiply_mod_p(i16* %c, i16* %a, i16* %b, i16* %p, i8 zeroext %wordlength) nounwind {
entry:
  %c.addr = alloca i16*, align 2
  %a.addr = alloca i16*, align 2
  %b.addr = alloca i16*, align 2
  %p.addr = alloca i16*, align 2
  %wordlength.addr = alloca i8, align 1
  %saved_stack = alloca i8*
  store i16* %c, i16** %c.addr, align 2
  store i16* %a, i16** %a.addr, align 2
  store i16* %b, i16** %b.addr, align 2
  store i16* %p, i16** %p.addr, align 2
  store i8 %wordlength, i8* %wordlength.addr, align 1
  %0 = call i8* @llvm.stacksave()
  store i8* %0, i8** %saved_stack
  %tmp = load i8* %wordlength.addr, align 1
  %conv = zext i8 %tmp to i16
  %mul = mul nsw i16 2, %conv
  %1 = mul i16 2, %mul
  %vla = alloca i8, i16 %1, align 2
  %tmp1 = bitcast i8* %vla to i16*
  %tmp2 = load i8* %wordlength.addr, align 1
  %tmp3 = zext i8 %tmp2 to i16
  %2 = mul i16 2, %tmp3
  %vla4 = alloca i8, i16 %2, align 2
  %tmp5 = bitcast i8* %vla4 to i16*
  %tmp6 = load i16** %a.addr, align 2
  %tmp7 = load i16** %b.addr, align 2
  %tmp8 = load i8* %wordlength.addr, align 1
  call void @multiply_mp_elements(i16* %tmp1, i16* %tmp6, i16* %tmp7, i8 zeroext %tmp8)
  %tmp9 = load i16** %c.addr, align 2
  %tmp10 = load i8* %wordlength.addr, align 1
  %conv11 = zext i8 %tmp10 to i16
  %mul12 = mul nsw i16 2, %conv11
  %tmp13 = load i16** %p.addr, align 2
  %tmp14 = load i8* %wordlength.addr, align 1
  %conv15 = zext i8 %tmp14 to i16
  call void @divide_mp_elements(i16* %tmp5, i16* %tmp9, i16* %tmp1, i16 %mul12, i16* %tmp13, i16 %conv15)
  %tmp16 = load i8** %saved_stack
  call void @llvm.stackrestore(i8* %tmp16)
  ret void
}

declare i8* @llvm.stacksave() nounwind

define void @divide_mp_elements(i16* %q, i16* %r, i16* %x_in, i16 %n, i16* %y, i16 %t) nounwind {
entry:
  %q.addr = alloca i16*, align 2
  %r.addr = alloca i16*, align 2
  %x_in.addr = alloca i16*, align 2
  %n.addr = alloca i16, align 2
  %y.addr = alloca i16*, align 2
  %t.addr = alloca i16, align 2
  %i = alloca i16, align 2
  %j = alloca i16, align 2
  %k = alloca i16, align 2
  %saved_stack = alloca i8*
  %temp_ls = alloca [2 x i16], align 2
  %ls = alloca [3 x i16], align 2
  %rs = alloca [3 x i16], align 2
  %temp = alloca i16, align 2
  store i16* %q, i16** %q.addr, align 2
  store i16* %r, i16** %r.addr, align 2
  store i16* %x_in, i16** %x_in.addr, align 2
  store i16 %n, i16* %n.addr, align 2
  store i16* %y, i16** %y.addr, align 2
  store i16 %t, i16* %t.addr, align 2
  %0 = call i8* @llvm.stacksave()
  store i8* %0, i8** %saved_stack
  %tmp = load i16* %n.addr, align 2
  %1 = mul i16 2, %tmp
  %vla = alloca i8, i16 %1, align 2
  %tmp1 = bitcast i8* %vla to i16*
  %tmp2 = load i16* %n.addr, align 2
  %2 = mul i16 2, %tmp2
  %vla3 = alloca i8, i16 %2, align 2
  %tmp4 = bitcast i8* %vla3 to i16*
  %tmp5 = load i16* %n.addr, align 2
  %3 = mul i16 2, %tmp5
  %vla6 = alloca i8, i16 %3, align 2
  %tmp7 = bitcast i8* %vla6 to i16*
  %tmp8 = load i16* %n.addr, align 2
  %4 = mul i16 2, %tmp8
  %vla9 = alloca i8, i16 %4, align 2
  %tmp10 = bitcast i8* %vla9 to i16*
  store i16 0, i16* %i, align 2
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %tmp15 = load i16* %i, align 2
  %tmp16 = load i16* %n.addr, align 2
  %cmp = icmp slt i16 %tmp15, %tmp16
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %tmp17 = load i16* %i, align 2
  %tmp18 = load i16** %x_in.addr, align 2
  %arrayidx = getelementptr inbounds i16* %tmp18, i16 %tmp17
  %tmp19 = load i16* %arrayidx
  %tmp20 = load i16* %i, align 2
  %arrayidx21 = getelementptr inbounds i16* %tmp1, i16 %tmp20
  store i16 %tmp19, i16* %arrayidx21
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %tmp22 = load i16* %i, align 2
  %inc = add nsw i16 %tmp22, 1
  store i16 %inc, i16* %i, align 2
  br label %for.cond

for.end:                                          ; preds = %for.cond
  store i16 0, i16* %j, align 2
  br label %for.cond23

for.cond23:                                       ; preds = %for.inc32, %for.end
  %tmp24 = load i16* %j, align 2
  %tmp25 = load i16* %n.addr, align 2
  %tmp26 = load i16* %t.addr, align 2
  %sub = sub nsw i16 %tmp25, %tmp26
  %cmp27 = icmp sle i16 %tmp24, %sub
  br i1 %cmp27, label %for.body28, label %for.end35

for.body28:                                       ; preds = %for.cond23
  %tmp29 = load i16* %j, align 2
  %tmp30 = load i16** %q.addr, align 2
  %arrayidx31 = getelementptr inbounds i16* %tmp30, i16 %tmp29
  store i16 0, i16* %arrayidx31
  br label %for.inc32

for.inc32:                                        ; preds = %for.body28
  %tmp33 = load i16* %j, align 2
  %inc34 = add nsw i16 %tmp33, 1
  store i16 %inc34, i16* %j, align 2
  br label %for.cond23

for.end35:                                        ; preds = %for.cond23
  %tmp36 = load i16* %n.addr, align 2
  %tmp37 = load i16** %y.addr, align 2
  %tmp38 = load i16* %t.addr, align 2
  %tmp39 = load i16* %n.addr, align 2
  %tmp40 = load i16* %t.addr, align 2
  %sub41 = sub nsw i16 %tmp39, %tmp40
  call void @mult_by_power_of_b(i16* %tmp4, i16 %tmp36, i16* %tmp37, i16 %tmp38, i16 %sub41)
  br label %while.cond

while.cond:                                       ; preds = %while.body, %for.end35
  %tmp42 = load i16* %n.addr, align 2
  %conv = trunc i16 %tmp42 to i8
  %call = call zeroext i8 @compare_mp_elements(i16* %tmp1, i16* %tmp4, i8 zeroext %conv)
  %conv43 = zext i8 %call to i16
  %cmp44 = icmp eq i16 1, %conv43
  br i1 %cmp44, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %tmp46 = load i16* %n.addr, align 2
  %tmp47 = load i16* %t.addr, align 2
  %sub48 = sub nsw i16 %tmp46, %tmp47
  %tmp49 = load i16** %q.addr, align 2
  %arrayidx50 = getelementptr inbounds i16* %tmp49, i16 %sub48
  %tmp51 = load i16* %arrayidx50
  %add = add i16 %tmp51, 1
  %tmp52 = load i16* %n.addr, align 2
  %tmp53 = load i16* %t.addr, align 2
  %sub54 = sub nsw i16 %tmp52, %tmp53
  %tmp55 = load i16** %q.addr, align 2
  %arrayidx56 = getelementptr inbounds i16* %tmp55, i16 %sub54
  store i16 %add, i16* %arrayidx56
  %tmp57 = load i16* %n.addr, align 2
  %conv58 = trunc i16 %tmp57 to i8
  %call59 = call i16 @subtract_mp_elements(i16* %tmp1, i16* %tmp1, i16* %tmp4, i8 zeroext %conv58)
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %tmp60 = load i16* %n.addr, align 2
  %sub61 = sub nsw i16 %tmp60, 1
  store i16 %sub61, i16* %i, align 2
  br label %for.cond62

for.cond62:                                       ; preds = %for.inc225, %while.end
  %tmp63 = load i16* %i, align 2
  %tmp64 = load i16* %t.addr, align 2
  %sub65 = sub nsw i16 %tmp64, 1
  %cmp66 = icmp sgt i16 %tmp63, %sub65
  br i1 %cmp66, label %for.body68, label %for.end227

for.body68:                                       ; preds = %for.cond62
  %tmp69 = load i16* %i, align 2
  %arrayidx70 = getelementptr inbounds i16* %tmp1, i16 %tmp69
  %tmp71 = load i16* %arrayidx70
  %tmp72 = load i16* %t.addr, align 2
  %sub73 = sub nsw i16 %tmp72, 1
  %tmp74 = load i16** %y.addr, align 2
  %arrayidx75 = getelementptr inbounds i16* %tmp74, i16 %sub73
  %tmp76 = load i16* %arrayidx75
  %sub77 = sub i16 %tmp71, %tmp76
  store i16 %sub77, i16* %temp, align 2
  %tmp78 = load i16* %temp, align 2
  %cmp79 = icmp eq i16 0, %tmp78
  br i1 %cmp79, label %if.then, label %if.else

if.then:                                          ; preds = %for.body68
  %tmp81 = load i16* %i, align 2
  %tmp82 = load i16* %t.addr, align 2
  %sub83 = sub nsw i16 %tmp81, %tmp82
  %tmp84 = load i16** %q.addr, align 2
  %arrayidx85 = getelementptr inbounds i16* %tmp84, i16 %sub83
  store i16 -1, i16* %arrayidx85
  br label %if.end

if.else:                                          ; preds = %for.body68
  %tmp86 = load i16* %i, align 2
  %arrayidx87 = getelementptr inbounds i16* %tmp1, i16 %tmp86
  %tmp88 = load i16* %arrayidx87
  %tmp89 = load i16* %i, align 2
  %sub90 = sub nsw i16 %tmp89, 1
  %arrayidx91 = getelementptr inbounds i16* %tmp1, i16 %sub90
  %tmp92 = load i16* %arrayidx91
  %tmp93 = load i16* %t.addr, align 2
  %sub94 = sub nsw i16 %tmp93, 1
  %tmp95 = load i16** %y.addr, align 2
  %arrayidx96 = getelementptr inbounds i16* %tmp95, i16 %sub94
  %tmp97 = load i16* %arrayidx96
  %call98 = call i16 @divide_2_word_by_1_word(i16 %tmp88, i16 %tmp92, i16 %tmp97)
  %tmp99 = load i16* %i, align 2
  %tmp100 = load i16* %t.addr, align 2
  %sub101 = sub nsw i16 %tmp99, %tmp100
  %tmp102 = load i16** %q.addr, align 2
  %arrayidx103 = getelementptr inbounds i16* %tmp102, i16 %sub101
  store i16 %call98, i16* %arrayidx103
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %tmp104 = load i16* %t.addr, align 2
  %sub105 = sub nsw i16 %tmp104, 2
  %tmp106 = load i16** %y.addr, align 2
  %arrayidx107 = getelementptr inbounds i16* %tmp106, i16 %sub105
  %tmp108 = load i16* %arrayidx107
  %arrayidx109 = getelementptr inbounds [2 x i16]* %temp_ls, i32 0, i16 0
  store i16 %tmp108, i16* %arrayidx109
  %tmp110 = load i16* %t.addr, align 2
  %sub111 = sub nsw i16 %tmp110, 1
  %tmp112 = load i16** %y.addr, align 2
  %arrayidx113 = getelementptr inbounds i16* %tmp112, i16 %sub111
  %tmp114 = load i16* %arrayidx113
  %arrayidx115 = getelementptr inbounds [2 x i16]* %temp_ls, i32 0, i16 1
  store i16 %tmp114, i16* %arrayidx115
  %arraydecay = getelementptr inbounds [3 x i16]* %ls, i32 0, i32 0
  %tmp116 = load i16* %i, align 2
  %tmp117 = load i16* %t.addr, align 2
  %sub118 = sub nsw i16 %tmp116, %tmp117
  %tmp119 = load i16** %q.addr, align 2
  %arrayidx120 = getelementptr inbounds i16* %tmp119, i16 %sub118
  %tmp121 = load i16* %arrayidx120
  %arraydecay122 = getelementptr inbounds [2 x i16]* %temp_ls, i32 0, i32 0
  call void @multiply_sp_by_mp_element(i16* %arraydecay, i16 %tmp121, i16* %arraydecay122, i16 2)
  %tmp123 = load i16* %i, align 2
  %sub124 = sub nsw i16 %tmp123, 2
  %arrayidx125 = getelementptr inbounds i16* %tmp1, i16 %sub124
  %tmp126 = load i16* %arrayidx125
  %arrayidx127 = getelementptr inbounds [3 x i16]* %rs, i32 0, i16 0
  store i16 %tmp126, i16* %arrayidx127
  %tmp128 = load i16* %i, align 2
  %sub129 = sub nsw i16 %tmp128, 1
  %arrayidx130 = getelementptr inbounds i16* %tmp1, i16 %sub129
  %tmp131 = load i16* %arrayidx130
  %arrayidx132 = getelementptr inbounds [3 x i16]* %rs, i32 0, i16 1
  store i16 %tmp131, i16* %arrayidx132
  %tmp133 = load i16* %i, align 2
  %arrayidx134 = getelementptr inbounds i16* %tmp1, i16 %tmp133
  %tmp135 = load i16* %arrayidx134
  %arrayidx136 = getelementptr inbounds [3 x i16]* %rs, i32 0, i16 2
  store i16 %tmp135, i16* %arrayidx136
  br label %while.cond137

while.cond137:                                    ; preds = %while.body144, %if.end
  %arraydecay138 = getelementptr inbounds [3 x i16]* %rs, i32 0, i32 0
  %arraydecay139 = getelementptr inbounds [3 x i16]* %ls, i32 0, i32 0
  %call140 = call zeroext i8 @compare_mp_elements(i16* %arraydecay138, i16* %arraydecay139, i8 zeroext 3)
  %conv141 = zext i8 %call140 to i16
  %cmp142 = icmp eq i16 0, %conv141
  br i1 %cmp142, label %while.body144, label %while.end165

while.body144:                                    ; preds = %while.cond137
  %tmp145 = load i16* %i, align 2
  %tmp146 = load i16* %t.addr, align 2
  %sub147 = sub nsw i16 %tmp145, %tmp146
  %tmp148 = load i16** %q.addr, align 2
  %arrayidx149 = getelementptr inbounds i16* %tmp148, i16 %sub147
  %tmp150 = load i16* %arrayidx149
  %sub151 = sub i16 %tmp150, 1
  %tmp152 = load i16* %i, align 2
  %tmp153 = load i16* %t.addr, align 2
  %sub154 = sub nsw i16 %tmp152, %tmp153
  %tmp155 = load i16** %q.addr, align 2
  %arrayidx156 = getelementptr inbounds i16* %tmp155, i16 %sub154
  store i16 %sub151, i16* %arrayidx156
  %arraydecay157 = getelementptr inbounds [3 x i16]* %ls, i32 0, i32 0
  %tmp158 = load i16* %i, align 2
  %tmp159 = load i16* %t.addr, align 2
  %sub160 = sub nsw i16 %tmp158, %tmp159
  %tmp161 = load i16** %q.addr, align 2
  %arrayidx162 = getelementptr inbounds i16* %tmp161, i16 %sub160
  %tmp163 = load i16* %arrayidx162
  %arraydecay164 = getelementptr inbounds [2 x i16]* %temp_ls, i32 0, i32 0
  call void @multiply_sp_by_mp_element(i16* %arraydecay157, i16 %tmp163, i16* %arraydecay164, i16 2)
  br label %while.cond137

while.end165:                                     ; preds = %while.cond137
  %tmp166 = load i16* %n.addr, align 2
  %tmp167 = load i16** %y.addr, align 2
  %tmp168 = load i16* %t.addr, align 2
  %tmp169 = load i16* %i, align 2
  %tmp170 = load i16* %t.addr, align 2
  %sub171 = sub nsw i16 %tmp169, %tmp170
  call void @mult_by_power_of_b(i16* %tmp7, i16 %tmp166, i16* %tmp167, i16 %tmp168, i16 %sub171)
  %tmp172 = load i16* %i, align 2
  %tmp173 = load i16* %t.addr, align 2
  %sub174 = sub nsw i16 %tmp172, %tmp173
  %tmp175 = load i16** %q.addr, align 2
  %arrayidx176 = getelementptr inbounds i16* %tmp175, i16 %sub174
  %tmp177 = load i16* %arrayidx176
  %tmp178 = load i16* %n.addr, align 2
  call void @multiply_sp_by_mp_element(i16* %tmp10, i16 %tmp177, i16* %tmp7, i16 %tmp178)
  %tmp179 = load i16* %n.addr, align 2
  %conv180 = trunc i16 %tmp179 to i8
  %call181 = call zeroext i8 @compare_mp_elements(i16* %tmp1, i16* %tmp10, i8 zeroext %conv180)
  %conv182 = zext i8 %call181 to i16
  %cmp183 = icmp eq i16 0, %conv182
  br i1 %cmp183, label %if.then185, label %if.else204

if.then185:                                       ; preds = %while.end165
  %tmp186 = load i16* %n.addr, align 2
  %conv187 = trunc i16 %tmp186 to i8
  %call188 = call i16 @add_mp_elements(i16* %tmp1, i16* %tmp1, i16* %tmp7, i8 zeroext %conv187)
  %tmp189 = load i16* %n.addr, align 2
  %conv190 = trunc i16 %tmp189 to i8
  %call191 = call i16 @subtract_mp_elements(i16* %tmp1, i16* %tmp1, i16* %tmp10, i8 zeroext %conv190)
  %tmp192 = load i16* %i, align 2
  %tmp193 = load i16* %t.addr, align 2
  %sub194 = sub nsw i16 %tmp192, %tmp193
  %tmp195 = load i16** %q.addr, align 2
  %arrayidx196 = getelementptr inbounds i16* %tmp195, i16 %sub194
  %tmp197 = load i16* %arrayidx196
  %sub198 = sub i16 %tmp197, 1
  %tmp199 = load i16* %i, align 2
  %tmp200 = load i16* %t.addr, align 2
  %sub201 = sub nsw i16 %tmp199, %tmp200
  %tmp202 = load i16** %q.addr, align 2
  %arrayidx203 = getelementptr inbounds i16* %tmp202, i16 %sub201
  store i16 %sub198, i16* %arrayidx203
  br label %if.end208

if.else204:                                       ; preds = %while.end165
  %tmp205 = load i16* %n.addr, align 2
  %conv206 = trunc i16 %tmp205 to i8
  %call207 = call i16 @subtract_mp_elements(i16* %tmp1, i16* %tmp1, i16* %tmp10, i8 zeroext %conv206)
  br label %if.end208

if.end208:                                        ; preds = %if.else204, %if.then185
  store i16 0, i16* %k, align 2
  br label %for.cond209

for.cond209:                                      ; preds = %for.inc221, %if.end208
  %tmp210 = load i16* %k, align 2
  %tmp211 = load i16* %t.addr, align 2
  %cmp212 = icmp slt i16 %tmp210, %tmp211
  br i1 %cmp212, label %for.body214, label %for.end224

for.body214:                                      ; preds = %for.cond209
  %tmp215 = load i16* %k, align 2
  %arrayidx216 = getelementptr inbounds i16* %tmp1, i16 %tmp215
  %tmp217 = load i16* %arrayidx216
  %tmp218 = load i16* %k, align 2
  %tmp219 = load i16** %r.addr, align 2
  %arrayidx220 = getelementptr inbounds i16* %tmp219, i16 %tmp218
  store i16 %tmp217, i16* %arrayidx220
  br label %for.inc221

for.inc221:                                       ; preds = %for.body214
  %tmp222 = load i16* %k, align 2
  %inc223 = add nsw i16 %tmp222, 1
  store i16 %inc223, i16* %k, align 2
  br label %for.cond209

for.end224:                                       ; preds = %for.cond209
  br label %for.inc225

for.inc225:                                       ; preds = %for.end224
  %tmp226 = load i16* %i, align 2
  %dec = add nsw i16 %tmp226, -1
  store i16 %dec, i16* %i, align 2
  br label %for.cond62

for.end227:                                       ; preds = %for.cond62
  %tmp228 = load i8** %saved_stack
  call void @llvm.stackrestore(i8* %tmp228)
  ret void
}

declare void @llvm.stackrestore(i8*) nounwind

define i16 @divide_2_word_by_1_word(i16 %x1, i16 %x0, i16 %y) nounwind {
entry:
  %retval = alloca i16, align 2
  %x1.addr = alloca i16, align 2
  %x0.addr = alloca i16, align 2
  %y.addr = alloca i16, align 2
  %result = alloca i32, align 2
  store i16 %x1, i16* %x1.addr, align 2
  store i16 %x0, i16* %x0.addr, align 2
  store i16 %y, i16* %y.addr, align 2
  %tmp = load i16* %y.addr, align 2
  %cmp = icmp ne i16 0, %tmp
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %tmp1 = load i16* %x1.addr, align 2
  %conv = zext i16 %tmp1 to i32
  %shl = shl i32 %conv, 16
  store i32 %shl, i32* %result, align 2
  %tmp2 = load i32* %result, align 2
  %tmp3 = load i16* %x0.addr, align 2
  %conv4 = zext i16 %tmp3 to i32
  %add = add i32 %tmp2, %conv4
  store i32 %add, i32* %result, align 2
  %tmp5 = load i32* %result, align 2
  %tmp6 = load i16* %y.addr, align 2
  %conv7 = zext i16 %tmp6 to i32
  %div = udiv i32 %tmp5, %conv7
  store i32 %div, i32* %result, align 2
  %tmp8 = load i32* %result, align 2
  %conv9 = trunc i32 %tmp8 to i16
  store i16 %conv9, i16* %retval
  br label %return

if.else:                                          ; preds = %entry
  store i16 0, i16* %retval
  br label %return

return:                                           ; preds = %if.else, %if.then
  %0 = load i16* %retval
  ret i16 %0
}

define void @mod_exp(i16* %A, i16* %g, i16* %e, i16 %e_length, i16* %p, i16 %p_length) nounwind {
entry:
  %A.addr = alloca i16*, align 2
  %g.addr = alloca i16*, align 2
  %e.addr = alloca i16*, align 2
  %e_length.addr = alloca i16, align 2
  %p.addr = alloca i16*, align 2
  %p_length.addr = alloca i16, align 2
  %saved_stack = alloca i8*
  %i = alloca i16, align 2
  %t = alloca i16, align 2
  store i16* %A, i16** %A.addr, align 2
  store i16* %g, i16** %g.addr, align 2
  store i16* %e, i16** %e.addr, align 2
  store i16 %e_length, i16* %e_length.addr, align 2
  store i16* %p, i16** %p.addr, align 2
  store i16 %p_length, i16* %p_length.addr, align 2
  %0 = call i8* @llvm.stacksave()
  store i8* %0, i8** %saved_stack
  %tmp = load i16* %p_length.addr, align 2
  %1 = mul i16 2, %tmp
  %vla = alloca i8, i16 %1, align 2
  %tmp1 = bitcast i8* %vla to i16*
  %tmp4 = load i16** %e.addr, align 2
  %tmp5 = load i16* %e_length.addr, align 2
  %call = call i16 @mp_bit_length(i16* %tmp4, i16 %tmp5)
  store i16 %call, i16* %t, align 2
  %tmp6 = load i16** %A.addr, align 2
  %tmp7 = load i16* %p_length.addr, align 2
  %conv = trunc i16 %tmp7 to i8
  call void @set_to_zero(i16* %tmp6, i8 zeroext %conv)
  %tmp8 = load i16** %A.addr, align 2
  %arrayidx = getelementptr inbounds i16* %tmp8, i16 0
  store i16 1, i16* %arrayidx
  %tmp9 = load i16* %t, align 2
  store i16 %tmp9, i16* %i, align 2
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %tmp10 = load i16* %i, align 2
  %cmp = icmp sge i16 %tmp10, 0
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %tmp12 = load i16** %A.addr, align 2
  %tmp13 = load i16** %A.addr, align 2
  %tmp14 = load i16** %p.addr, align 2
  %tmp15 = load i16* %p_length.addr, align 2
  %conv16 = trunc i16 %tmp15 to i8
  call void @multiply_mod_p(i16* %tmp1, i16* %tmp12, i16* %tmp13, i16* %tmp14, i8 zeroext %conv16)
  %tmp17 = load i16** %A.addr, align 2
  %tmp18 = load i16* %p_length.addr, align 2
  call void @copy_mp(i16* %tmp17, i16* %tmp1, i16 %tmp18)
  %tmp19 = load i16** %e.addr, align 2
  %tmp20 = load i16* %i, align 2
  %call21 = call i16 @mp_ith_bit(i16* %tmp19, i16 %tmp20)
  %cmp22 = icmp eq i16 1, %call21
  br i1 %cmp22, label %if.then, label %if.end

if.then:                                          ; preds = %for.body
  %tmp24 = load i16** %A.addr, align 2
  %tmp25 = load i16** %g.addr, align 2
  %tmp26 = load i16** %p.addr, align 2
  %tmp27 = load i16* %p_length.addr, align 2
  %conv28 = trunc i16 %tmp27 to i8
  call void @multiply_mod_p(i16* %tmp1, i16* %tmp24, i16* %tmp25, i16* %tmp26, i8 zeroext %conv28)
  %tmp29 = load i16** %A.addr, align 2
  %tmp30 = load i16* %p_length.addr, align 2
  call void @copy_mp(i16* %tmp29, i16* %tmp1, i16 %tmp30)
  br label %if.end

if.end:                                           ; preds = %if.then, %for.body
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %tmp31 = load i16* %i, align 2
  %dec = add nsw i16 %tmp31, -1
  store i16 %dec, i16* %i, align 2
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %tmp32 = load i8** %saved_stack
  call void @llvm.stackrestore(i8* %tmp32)
  ret void
}

define void @test_rsa_encrypt() nounwind {
entry:
  %n = alloca [32 x i16], align 2
  %e = alloca [2 x i16], align 2
  %plaintext = alloca [32 x i16], align 2
  %ciphertext = alloca [32 x i16], align 2
  %tmp = bitcast [32 x i16]* %n to i8*
  call void @llvm.memcpy.p0i8.p0i8.i16(i8* %tmp, i8* bitcast ([32 x i16]* @test_rsa_encrypt.n to i8*), i16 64, i32 2, i1 false)
  %tmp2 = bitcast [2 x i16]* %e to i8*
  call void @llvm.memcpy.p0i8.p0i8.i16(i8* %tmp2, i8* bitcast ([2 x i16]* @test_rsa_encrypt.e to i8*), i16 4, i32 2, i1 false)
  %tmp4 = bitcast [32 x i16]* %plaintext to i8*
  call void @llvm.memcpy.p0i8.p0i8.i16(i8* %tmp4, i8* bitcast ([32 x i16]* @test_rsa_encrypt.plaintext to i8*), i16 64, i32 2, i1 false)
  %arraydecay = getelementptr inbounds [32 x i16]* %ciphertext, i32 0, i32 0
  %arraydecay6 = getelementptr inbounds [32 x i16]* %plaintext, i32 0, i32 0
  %arraydecay7 = getelementptr inbounds [2 x i16]* %e, i32 0, i32 0
  %arraydecay8 = getelementptr inbounds [32 x i16]* %n, i32 0, i32 0
  call void @mod_exp(i16* %arraydecay, i16* %arraydecay6, i16* %arraydecay7, i16 1, i16* %arraydecay8, i16 32)
  ret void
}

declare void @llvm.memcpy.p0i8.p0i8.i16(i8* nocapture, i8* nocapture, i16, i32, i1) nounwind

define i16 @main() nounwind {
entry:
  %retval = alloca i16, align 2
  store i16 0, i16* %retval
  call void @test_rsa_encrypt()
  call void asm sideeffect "NOP", ""() nounwind, !srcloc !0
  ret i16 11
}

!0 = metadata !{i32 19846}
