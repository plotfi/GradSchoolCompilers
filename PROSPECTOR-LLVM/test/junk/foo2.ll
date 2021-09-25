; ModuleID = 'foo2.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128"
target triple = "x86_64-linux-gnu"

@.str = private constant [11 x i8] c"Inner loop\00", align 1 ; <[11 x i8]*> [#uses=1]
@.gstr_funcname_foo = internal constant [4 x i8] c"foo\00" ; <[4 x i8]*> [#uses=1]
@.gstr_loopname_foo_bb6 = internal constant [8 x i8] c"foo_bb6\00" ; <[8 x i8]*> [#uses=1]
@.gstr_parentloopname_foo_bb8 = internal constant [8 x i8] c"foo_bb8\00" ; <[8 x i8]*> [#uses=1]
@.gstr_loopname_foo_bb8 = internal constant [8 x i8] c"foo_bb8\00" ; <[8 x i8]*> [#uses=1]
@.gstr_parentloopname_foo_bb10 = internal constant [9 x i8] c"foo_bb10\00" ; <[9 x i8]*> [#uses=1]
@.gstr_loopname_foo_bb10 = internal constant [9 x i8] c"foo_bb10\00" ; <[9 x i8]*> [#uses=1]
@.gstr_parentloopname_foo_bb12 = internal constant [9 x i8] c"foo_bb12\00" ; <[9 x i8]*> [#uses=1]
@.gstr_loopname_foo_bb12 = internal constant [9 x i8] c"foo_bb12\00" ; <[9 x i8]*> [#uses=1]
@.gstr_parentloopname_foo_bb14 = internal constant [9 x i8] c"foo_bb14\00" ; <[9 x i8]*> [#uses=1]
@.gstr_loopname_foo_bb14 = internal constant [9 x i8] c"foo_bb14\00" ; <[9 x i8]*> [#uses=1]
@.gstr_parentloopname_foo_bb16 = internal constant [9 x i8] c"foo_bb16\00" ; <[9 x i8]*> [#uses=1]
@.gstr_loopname_foo_bb16 = internal constant [9 x i8] c"foo_bb16\00" ; <[9 x i8]*> [#uses=1]
@.gstr_parentloopname_ = internal constant [1 x i8] zeroinitializer ; <[1 x i8]*> [#uses=1]
@.gstr_funcname_main = internal constant [5 x i8] c"main\00" ; <[5 x i8]*> [#uses=1]
@.gstr_loopname_main_label1 = internal constant [12 x i8] c"main_label1\00" ; <[12 x i8]*> [#uses=1]
@.gstr_loopname_main_bb9 = internal constant [9 x i8] c"main_bb9\00" ; <[9 x i8]*> [#uses=1]
@.gstr_loopname_main_bb7 = internal constant [9 x i8] c"main_bb7\00" ; <[9 x i8]*> [#uses=1]
@.gstr_loopname_main_bb4 = internal constant [9 x i8] c"main_bb4\00" ; <[9 x i8]*> [#uses=1]
@.gstr_loopname_main_bb1 = internal constant [9 x i8] c"main_bb1\00" ; <[9 x i8]*> [#uses=1]

define void @foo() nounwind {
entry:
  %n = alloca i32                                 ; <i32*> [#uses=4]
  %m = alloca i32                                 ; <i32*> [#uses=4]
  %l = alloca i32                                 ; <i32*> [#uses=4]
  %k = alloca i32                                 ; <i32*> [#uses=4]
  %j = alloca i32                                 ; <i32*> [#uses=4]
  %i = alloca i32                                 ; <i32*> [#uses=4]
  %"alloca point" = bitcast i32 0 to i32          ; <i32> [#uses=0]
  store i32 0, i32* %i, align 4
  call void @ProvideFunctionStats(i8* noalias getelementptr ([4 x i8]* @.gstr_funcname_foo, i32 0, i32 0)) nounwind
  call void @OnLoopStart(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb16, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  call void @PushLoopStack(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb16, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  call void @PrintLoopStack()
  br label %bb16

bb:                                               ; preds = %bb16
  store i32 0, i32* %j, align 4
  call void @OnLoopStart(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb14, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb16, i32 0, i32 0)) nounwind
  call void @PushLoopStack(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb14, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb16, i32 0, i32 0)) nounwind
  call void @PrintLoopStack()
  br label %bb14

bb1:                                              ; preds = %bb14
  store i32 0, i32* %k, align 4
  call void @OnLoopStart(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb12, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb14, i32 0, i32 0)) nounwind
  call void @PushLoopStack(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb12, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb14, i32 0, i32 0)) nounwind
  call void @PrintLoopStack()
  br label %bb12

bb2:                                              ; preds = %bb12
  store i32 0, i32* %l, align 4
  call void @OnLoopStart(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb10, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb12, i32 0, i32 0)) nounwind
  call void @PushLoopStack(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb10, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb12, i32 0, i32 0)) nounwind
  call void @PrintLoopStack()
  br label %bb10

bb3:                                              ; preds = %bb10
  store i32 0, i32* %m, align 4
  call void @OnLoopStart(i8* noalias getelementptr ([8 x i8]* @.gstr_loopname_foo_bb8, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb10, i32 0, i32 0)) nounwind
  call void @PushLoopStack(i8* noalias getelementptr ([8 x i8]* @.gstr_loopname_foo_bb8, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb10, i32 0, i32 0)) nounwind
  call void @PrintLoopStack()
  br label %bb8

bb4:                                              ; preds = %bb8
  store i32 0, i32* %n, align 4
  call void @OnLoopStart(i8* noalias getelementptr ([8 x i8]* @.gstr_loopname_foo_bb6, i32 0, i32 0), i8* getelementptr ([8 x i8]* @.gstr_parentloopname_foo_bb8, i32 0, i32 0)) nounwind
  call void @PushLoopStack(i8* noalias getelementptr ([8 x i8]* @.gstr_loopname_foo_bb6, i32 0, i32 0), i8* getelementptr ([8 x i8]* @.gstr_parentloopname_foo_bb8, i32 0, i32 0)) nounwind
  call void @PrintLoopStack()
  br label %bb6

bb5:                                              ; preds = %bb6
  %0 = call i32 @puts(i8* getelementptr inbounds ([11 x i8]* @.str, i64 0, i64 0)) nounwind ; <i32> [#uses=0]
  %1 = load i32* %n, align 4                      ; <i32> [#uses=1]
  %2 = add nsw i32 %1, 1                          ; <i32> [#uses=1]
  store i32 %2, i32* %n, align 4
  br label %bb6

bb6:                                              ; preds = %bb5, %bb4
  %3 = load i32* %n, align 4                      ; <i32> [#uses=1]
  %4 = icmp sle i32 %3, 1                         ; <i1> [#uses=1]
  call void @OnLoopIterate(i8* noalias getelementptr ([8 x i8]* @.gstr_loopname_foo_bb6, i32 0, i32 0), i8* getelementptr ([8 x i8]* @.gstr_parentloopname_foo_bb8, i32 0, i32 0)) nounwind
  br i1 %4, label %bb5, label %bb7

bb7:                                              ; preds = %bb6
  call void @PopLoopStack()
  call void @OnLoopExit(i8* noalias getelementptr ([8 x i8]* @.gstr_loopname_foo_bb6, i32 0, i32 0), i8* getelementptr ([8 x i8]* @.gstr_parentloopname_foo_bb8, i32 0, i32 0)) nounwind
  %5 = load i32* %m, align 4                      ; <i32> [#uses=1]
  %6 = add nsw i32 %5, 1                          ; <i32> [#uses=1]
  store i32 %6, i32* %m, align 4
  br label %bb8

bb8:                                              ; preds = %bb7, %bb3
  %7 = load i32* %m, align 4                      ; <i32> [#uses=1]
  %8 = icmp sle i32 %7, 1                         ; <i1> [#uses=1]
  call void @OnLoopIterate(i8* noalias getelementptr ([8 x i8]* @.gstr_loopname_foo_bb8, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb10, i32 0, i32 0)) nounwind
  br i1 %8, label %bb4, label %bb9

bb9:                                              ; preds = %bb8
  call void @PopLoopStack()
  call void @OnLoopExit(i8* noalias getelementptr ([8 x i8]* @.gstr_loopname_foo_bb8, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb10, i32 0, i32 0)) nounwind
  %9 = load i32* %l, align 4                      ; <i32> [#uses=1]
  %10 = add nsw i32 %9, 1                         ; <i32> [#uses=1]
  store i32 %10, i32* %l, align 4
  br label %bb10

bb10:                                             ; preds = %bb9, %bb2
  %11 = load i32* %l, align 4                     ; <i32> [#uses=1]
  %12 = icmp sle i32 %11, 1                       ; <i1> [#uses=1]
  call void @OnLoopIterate(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb10, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb12, i32 0, i32 0)) nounwind
  br i1 %12, label %bb3, label %bb11

bb11:                                             ; preds = %bb10
  call void @PopLoopStack()
  call void @OnLoopExit(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb10, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb12, i32 0, i32 0)) nounwind
  %13 = load i32* %k, align 4                     ; <i32> [#uses=1]
  %14 = add nsw i32 %13, 1                        ; <i32> [#uses=1]
  store i32 %14, i32* %k, align 4
  br label %bb12

bb12:                                             ; preds = %bb11, %bb1
  %15 = load i32* %k, align 4                     ; <i32> [#uses=1]
  %16 = icmp sle i32 %15, 1                       ; <i1> [#uses=1]
  call void @OnLoopIterate(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb12, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb14, i32 0, i32 0)) nounwind
  br i1 %16, label %bb2, label %bb13

bb13:                                             ; preds = %bb12
  call void @PopLoopStack()
  call void @OnLoopExit(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb12, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb14, i32 0, i32 0)) nounwind
  %17 = load i32* %j, align 4                     ; <i32> [#uses=1]
  %18 = add nsw i32 %17, 1                        ; <i32> [#uses=1]
  store i32 %18, i32* %j, align 4
  br label %bb14

bb14:                                             ; preds = %bb13, %bb
  %19 = load i32* %j, align 4                     ; <i32> [#uses=1]
  %20 = icmp sle i32 %19, 1                       ; <i1> [#uses=1]
  call void @OnLoopIterate(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb14, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb16, i32 0, i32 0)) nounwind
  br i1 %20, label %bb1, label %bb15

bb15:                                             ; preds = %bb14
  call void @PopLoopStack()
  call void @OnLoopExit(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb14, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb16, i32 0, i32 0)) nounwind
  %21 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %22 = add nsw i32 %21, 1                        ; <i32> [#uses=1]
  store i32 %22, i32* %i, align 4
  br label %bb16

bb16:                                             ; preds = %bb15, %entry
  %23 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %24 = icmp sle i32 %23, 4                       ; <i1> [#uses=1]
  call void @OnLoopIterate(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb16, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  br i1 %24, label %bb, label %bb17

bb17:                                             ; preds = %bb16
  call void @PopLoopStack()
  call void @OnLoopExit(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb16, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  br label %return

return:                                           ; preds = %bb17
  ret void
}

declare i32 @puts(i8*)

define i32 @main() nounwind {
entry:
  %retval = alloca i32                            ; <i32*> [#uses=2]
  %i = alloca i32                                 ; <i32*> [#uses=18]
  %0 = alloca i32                                 ; <i32*> [#uses=2]
  %"alloca point" = bitcast i32 0 to i32          ; <i32> [#uses=0]
  store i32 10, i32* %i, align 4
  call void @foo() nounwind
  call void @ProvideFunctionStats(i8* noalias getelementptr ([5 x i8]* @.gstr_funcname_main, i32 0, i32 0)) nounwind
  br label %whileloop

gwb:                                              ; preds = %bb2
  br label %forloop

gfl:                                              ; preds = %bb5
  br label %whileloop2

gw2:                                              ; preds = %bb8
  br label %dooloop

whileloop:                                        ; preds = %entry
  call void @OnLoopStart(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_main_bb1, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  call void @PushLoopStack(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_main_bb1, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  call void @PrintLoopStack()
  br label %bb1

bb:                                               ; preds = %bb1
  %1 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %2 = sub i32 %1, 1                              ; <i32> [#uses=1]
  store i32 %2, i32* %i, align 4
  br label %bb1

bb1:                                              ; preds = %bb, %whileloop
  %3 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %4 = icmp ne i32 %3, 0                          ; <i1> [#uses=1]
  call void @OnLoopIterate(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_main_bb1, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  br i1 %4, label %bb, label %bb2

bb2:                                              ; preds = %bb1
  call void @PopLoopStack()
  call void @OnLoopExit(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_main_bb1, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  br label %gwb

forloop:                                          ; preds = %gwb
  store i32 0, i32* %i, align 4
  call void @OnLoopStart(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_main_bb4, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  call void @PushLoopStack(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_main_bb4, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  call void @PrintLoopStack()
  br label %bb4

bb3:                                              ; preds = %bb4
  %5 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %6 = add nsw i32 %5, 1                          ; <i32> [#uses=1]
  store i32 %6, i32* %i, align 4
  br label %bb4

bb4:                                              ; preds = %bb3, %forloop
  %7 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %8 = icmp sle i32 %7, 4                         ; <i1> [#uses=1]
  call void @OnLoopIterate(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_main_bb4, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  br i1 %8, label %bb3, label %bb5

bb5:                                              ; preds = %bb4
  call void @PopLoopStack()
  call void @OnLoopExit(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_main_bb4, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  br label %gfl

whileloop2:                                       ; preds = %gfl
  call void @OnLoopStart(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_main_bb7, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  call void @PushLoopStack(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_main_bb7, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  call void @PrintLoopStack()
  br label %bb7

bb6:                                              ; preds = %bb7
  %9 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %10 = sub i32 %9, 1                             ; <i32> [#uses=1]
  store i32 %10, i32* %i, align 4
  br label %bb7

bb7:                                              ; preds = %bb6, %whileloop2
  %11 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %12 = icmp ne i32 %11, 0                        ; <i1> [#uses=1]
  call void @OnLoopIterate(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_main_bb7, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  br i1 %12, label %bb6, label %bb8

bb8:                                              ; preds = %bb7
  call void @PopLoopStack()
  call void @OnLoopExit(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_main_bb7, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  br label %gw2

dooloop:                                          ; preds = %gw2
  store i32 2, i32* %i, align 4
  call void @OnLoopStart(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_main_bb9, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  call void @PushLoopStack(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_main_bb9, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  call void @PrintLoopStack()
  br label %bb9

bb9:                                              ; preds = %bb9, %dooloop
  %13 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %14 = sub i32 %13, 1                            ; <i32> [#uses=1]
  store i32 %14, i32* %i, align 4
  %15 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %16 = icmp ne i32 %15, 0                        ; <i1> [#uses=1]
  call void @OnLoopIterate(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_main_bb9, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  br i1 %16, label %bb9, label %bb10

bb10:                                             ; preds = %bb9
  call void @PopLoopStack()
  call void @OnLoopExit(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_main_bb9, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  %17 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %18 = icmp ne i32 %17, 0                        ; <i1> [#uses=1]
  br i1 %18, label %end, label %label1fph

label:                                            ; preds = %label1
  store i32 1, i32* %i, align 4
  %19 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %20 = icmp ne i32 %19, 0                        ; <i1> [#uses=1]
  br i1 %20, label %end, label %label1

label1:                                           ; preds = %label1fph, %label
  call void @OnLoopIterate(i8* noalias getelementptr ([12 x i8]* @.gstr_loopname_main_label1, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  br label %label

end:                                              ; preds = %label, %bb10
  call void @PopLoopStack()
  call void @OnLoopExit(i8* noalias getelementptr ([12 x i8]* @.gstr_loopname_main_label1, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  store i32 0, i32* %0, align 4
  %21 = load i32* %0, align 4                     ; <i32> [#uses=1]
  store i32 %21, i32* %retval, align 4
  br label %return

return:                                           ; preds = %end
  %retval11 = load i32* %retval                   ; <i32> [#uses=1]
  ret i32 %retval11

label1fph:                                        ; preds = %bb10
  call void @OnLoopStart(i8* noalias getelementptr ([12 x i8]* @.gstr_loopname_main_label1, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  call void @PushLoopStack(i8* noalias getelementptr ([12 x i8]* @.gstr_loopname_main_label1, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  call void @PrintLoopStack()
  br label %label1
}

declare void @ProvideFunctionStats(i8*)

declare void @OnLoopStart(i8*, i8*)

declare void @PushLoopStack(i8*, i8*)

declare void @PrintLoopStack()

declare void @OnLoopIterate(i8*, i8*)

declare void @OnLoopExit(i8*, i8*)

declare void @PopLoopStack()
