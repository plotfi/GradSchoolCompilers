; ModuleID = 'bar.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128"
target triple = "x86_64-linux-gnu"

@.str = private constant [19 x i8] c"normal loop exit: \00", align 1 ; <[19 x i8]*> [#uses=1]
@.str1 = private constant [6 x i8] c"break\00", align 1 ; <[6 x i8]*> [#uses=1]
@.str2 = private constant [5 x i8] c"goto\00", align 1 ; <[5 x i8]*> [#uses=1]
@.str3 = private constant [8 x i8] c"return:\00", align 1 ; <[8 x i8]*> [#uses=1]

define void @bar() nounwind {
entry:
  %l = alloca i32                                 ; <i32*> [#uses=0]
  %j = alloca i32                                 ; <i32*> [#uses=1]
  %h = alloca i32                                 ; <i32*> [#uses=1]
  %k = alloca i32                                 ; <i32*> [#uses=1]
  %i = alloca i32                                 ; <i32*> [#uses=5]
  %"alloca point" = bitcast i32 0 to i32          ; <i32> [#uses=0]
  store i32 2, i32* %i, align 4
  %0 = call i32 @puts(i8* getelementptr inbounds ([19 x i8]* @.str, i64 0, i64 0)) nounwind ; <i32> [#uses=0]
  br label %bb1

bb:                                               ; preds = %bb1
  %1 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %2 = sub i32 %1, 1                              ; <i32> [#uses=1]
  store i32 %2, i32* %i, align 4
  br label %bb1

bb1:                                              ; preds = %bb, %entry
  %3 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %4 = icmp ne i32 %3, 0                          ; <i1> [#uses=1]
  br i1 %4, label %bb, label %bb2

bb2:                                              ; preds = %bb1
  store i32 100, i32* %i, align 4
  %5 = load i32* %k, align 4                      ; <i32> [#uses=1]
  %6 = icmp ne i32 %5, 0                          ; <i1> [#uses=1]
  br i1 %6, label %bb3, label %bb4

bb3:                                              ; preds = %bb2
  %7 = call i32 @puts(i8* getelementptr inbounds ([6 x i8]* @.str1, i64 0, i64 0)) nounwind ; <i32> [#uses=0]
  br label %bb4

bb4:                                              ; preds = %bb3, %bb2
  %8 = load i32* %h, align 4                      ; <i32> [#uses=1]
  %9 = icmp ne i32 %8, 0                          ; <i1> [#uses=1]
  br i1 %9, label %bb5, label %label1

bb5:                                              ; preds = %bb4
  %10 = call i32 @puts(i8* getelementptr inbounds ([5 x i8]* @.str2, i64 0, i64 0)) nounwind ; <i32> [#uses=0]
  br label %label1

label1:                                           ; preds = %bb5, %bb4
  %11 = load i32* %j, align 4                     ; <i32> [#uses=1]
  %12 = icmp ne i32 %11, 0                        ; <i1> [#uses=1]
  br i1 %12, label %bb6, label %bb7

bb6:                                              ; preds = %label1
  %13 = call i32 @puts(i8* getelementptr inbounds ([8 x i8]* @.str3, i64 0, i64 0)) nounwind ; <i32> [#uses=0]
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
