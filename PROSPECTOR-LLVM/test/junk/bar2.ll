; ModuleID = 'bar2.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128"
target triple = "x86_64-linux-gnu"

@.str = private constant [19 x i8] c"normal loop exit: \00", align 1 ; <[19 x i8]*> [#uses=1]
@.str1 = private constant [6 x i8] c"break\00", align 1 ; <[6 x i8]*> [#uses=1]
@.str2 = private constant [5 x i8] c"goto\00", align 1 ; <[5 x i8]*> [#uses=1]
@.str3 = private constant [8 x i8] c"return:\00", align 1 ; <[8 x i8]*> [#uses=1]
@.gstr1 = internal constant [26 x i8] c"header of loop 0x2e8ddc0\0A\00" ; <[26 x i8]*> [#uses=1]
@.gstr2 = internal constant [24 x i8] c"exit of loop 0x2e8ddc0\0A\00" ; <[24 x i8]*> [#uses=1]

define void @bar() nounwind {
entry:
  %i = alloca i32                                 ; <i32*> [#uses=14]
  %"alloca point" = bitcast i32 0 to i32          ; <i32> [#uses=0]
  store i32 2, i32* %i, align 4
  %0 = call i32 @puts(i8* getelementptr inbounds ([19 x i8]* @.str, i64 0, i64 0)) nounwind ; <i32> [#uses=0]
  %1 = call i32 (i8*, ...)* @printf(i8* noalias getelementptr ([26 x i8]* @.gstr1, i32 0, i32 0)) nounwind ; <i32> [#uses=0]
  br label %bb1

bb:                                               ; preds = %bb1
  %2 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %3 = sub i32 %2, 1                              ; <i32> [#uses=1]
  store i32 %3, i32* %i, align 4
  br label %bb1

bb1:                                              ; preds = %bb, %entry
  %4 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %5 = icmp ne i32 %4, 0                          ; <i1> [#uses=1]
  %6 = call i32 (i8*, ...)* @printf(i8* noalias getelementptr ([24 x i8]* @.gstr2, i32 0, i32 0)) nounwind ; <i32> [#uses=0]
  br i1 %5, label %bb, label %bb2

bb2:                                              ; preds = %bb1
  store i32 100, i32* %i, align 4
  %7 = call i32 @puts(i8* getelementptr inbounds ([6 x i8]* @.str1, i64 0, i64 0)) nounwind ; <i32> [#uses=0]
  %8 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %9 = icmp ne i32 %8, 0                          ; <i1> [#uses=1]
  br i1 %9, label %bb3, label %bb4

bb3:                                              ; preds = %bb2
  %10 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %11 = sub i32 %10, 1                            ; <i32> [#uses=1]
  store i32 %11, i32* %i, align 4
  br label %bb4

bb4:                                              ; preds = %bb3, %bb2
  %12 = call i32 @puts(i8* getelementptr inbounds ([5 x i8]* @.str2, i64 0, i64 0)) nounwind ; <i32> [#uses=0]
  %13 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %14 = icmp ne i32 %13, 0                        ; <i1> [#uses=1]
  br i1 %14, label %bb5, label %label1

bb5:                                              ; preds = %bb4
  %15 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %16 = sub i32 %15, 1                            ; <i32> [#uses=1]
  store i32 %16, i32* %i, align 4
  br label %label1

label1:                                           ; preds = %bb5, %bb4
  %17 = call i32 @puts(i8* getelementptr inbounds ([8 x i8]* @.str3, i64 0, i64 0)) nounwind ; <i32> [#uses=0]
  %18 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %19 = icmp ne i32 %18, 0                        ; <i1> [#uses=1]
  br i1 %19, label %bb6, label %bb7

bb6:                                              ; preds = %label1
  %20 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %21 = sub i32 %20, 1                            ; <i32> [#uses=1]
  store i32 %21, i32* %i, align 4
  br label %bb7

bb7:                                              ; preds = %bb6, %label1
  br label %return

return:                                           ; preds = %bb7
  ret void
}

declare i32 @puts(i8*)

define i32 @main() nounwind {
entry:
  %retval = alloca i32                            ; <i32*> [#uses=2]
  %0 = alloca i32                                 ; <i32*> [#uses=2]
  %"alloca point" = bitcast i32 0 to i32          ; <i32> [#uses=0]
  call void @bar() nounwind
  store i32 0, i32* %0, align 4
  %1 = load i32* %0, align 4                      ; <i32> [#uses=1]
  store i32 %1, i32* %retval, align 4
  br label %return

return:                                           ; preds = %entry
  %retval1 = load i32* %retval                    ; <i32> [#uses=1]
  ret i32 %retval1
}

declare i32 @printf(i8* noalias, ...) nounwind
