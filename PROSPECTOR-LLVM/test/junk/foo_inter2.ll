; ModuleID = '../TestProrgrams/foo_inter2.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128"
target triple = "x86_64-linux-gnu"

%struct.LoopNode = type { %struct.LoopNode*, i8*, i8*, i8* }
%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i64, i16, i8, [1 x i8], i8*, i64, i8*, i8*, i8*, i8*, i64, i32, [20 x i8] }
%struct._IO_marker = type { %struct._IO_marker*, %struct._IO_FILE*, i32 }

@.str = private constant [41 x i8] c"----------------------------------------\00", align 1 ; <[41 x i8]*> [#uses=1]
@.str1 = private constant [16 x i8] c"Loop Start: %s\0A\00", align 1 ; <[16 x i8]*> [#uses=1]
@.str2 = private constant [18 x i8] c"Loop Iterate: %s\0A\00", align 1 ; <[18 x i8]*> [#uses=1]
@.str3 = private constant [15 x i8] c"Loop Exit: %s\0A\00", align 1 ; <[15 x i8]*> [#uses=1]
@.str4 = private constant [42 x i8] c"----------------------------------------\0A\00", align 1 ; <[42 x i8]*> [#uses=1]
@loopstack_size = global i32 0                    ; <i32*> [#uses=9]
@GlobalLoopStack = global %struct.LoopNode* null  ; <%struct.LoopNode**> [#uses=7]
@stderr = external global %struct._IO_FILE*       ; <%struct._IO_FILE**> [#uses=1]
@.str5 = private constant [38 x i8] c"newLoopNode() OUT OF MEMORY at %s:%d\0A\00", align 1 ; <[38 x i8]*> [#uses=1]
@.str16 = private constant [33 x i8] c"prospector_loopstack_functions.c\00", align 1 ; <[33 x i8]*> [#uses=1]
@.str27 = private constant [1 x i8] zeroinitializer, align 1 ; <[1 x i8]*> [#uses=1]
@.str38 = private constant [7 x i8] c"[%s]->\00", align 1 ; <[7 x i8]*> [#uses=1]
@.str49 = private constant [3 x i8] c"//\00", align 1 ; <[3 x i8]*> [#uses=1]
@.str10 = private constant [11 x i8] c"Inner loop\00", align 1 ; <[11 x i8]*> [#uses=1]
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
@.gstr_loopname_main_label1 = internal constant [12 x i8] c"main_label1\00" ; <[12 x i8]*> [#uses=1]
@.gstr_loopname_main_bb9 = internal constant [9 x i8] c"main_bb9\00" ; <[9 x i8]*> [#uses=1]
@.gstr_loopname_main_bb7 = internal constant [9 x i8] c"main_bb7\00" ; <[9 x i8]*> [#uses=1]
@.gstr_loopname_main_bb4 = internal constant [9 x i8] c"main_bb4\00" ; <[9 x i8]*> [#uses=1]
@.gstr_loopname_main_bb1 = internal constant [9 x i8] c"main_bb1\00" ; <[9 x i8]*> [#uses=1]

define void @OnLoopStart(i8* %loop, i8* %str) {
entry:
  %loop_addr = alloca i8*                         ; <i8**> [#uses=2]
  %str_addr = alloca i8*                          ; <i8**> [#uses=1]
  %currStackSize = alloca i32                     ; <i32*> [#uses=3]
  %i = alloca i32                                 ; <i32*> [#uses=9]
  %"alloca point" = bitcast i32 0 to i32          ; <i32> [#uses=0]
  store i8* %loop, i8** %loop_addr
  store i8* %str, i8** %str_addr
  store i32 0, i32* %i, align 4
  %0 = load i32* @loopstack_size, align 4         ; <i32> [#uses=1]
  store i32 %0, i32* %currStackSize, align 4
  %1 = call i32 @putchar(i32 10) nounwind         ; <i32> [#uses=0]
  store i32 0, i32* %i, align 4
  br label %bb1

bb:                                               ; preds = %bb1
  %2 = call i32 @putchar(i32 9) nounwind          ; <i32> [#uses=0]
  %3 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %4 = add nsw i32 %3, 1                          ; <i32> [#uses=1]
  store i32 %4, i32* %i, align 4
  br label %bb1

bb1:                                              ; preds = %bb, %entry
  %5 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %6 = load i32* %currStackSize, align 4          ; <i32> [#uses=1]
  %7 = icmp sle i32 %5, %6                        ; <i1> [#uses=1]
  br i1 %7, label %bb, label %bb2

bb2:                                              ; preds = %bb1
  %8 = call i32 @puts(i8* getelementptr inbounds ([41 x i8]* @.str, i64 0, i64 0)) nounwind ; <i32> [#uses=0]
  store i32 0, i32* %i, align 4
  br label %bb4

bb3:                                              ; preds = %bb4
  %9 = call i32 @putchar(i32 9) nounwind          ; <i32> [#uses=0]
  %10 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %11 = add nsw i32 %10, 1                        ; <i32> [#uses=1]
  store i32 %11, i32* %i, align 4
  br label %bb4

bb4:                                              ; preds = %bb3, %bb2
  %12 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %13 = load i32* %currStackSize, align 4         ; <i32> [#uses=1]
  %14 = icmp sle i32 %12, %13                     ; <i1> [#uses=1]
  br i1 %14, label %bb3, label %bb5

bb5:                                              ; preds = %bb4
  %15 = load i8** %loop_addr, align 8             ; <i8*> [#uses=1]
  %16 = call i32 (i8*, ...)* @printf(i8* noalias getelementptr inbounds ([16 x i8]* @.str1, i64 0, i64 0), i8* %15) nounwind ; <i32> [#uses=0]
  br label %return

return:                                           ; preds = %bb5
  ret void
}

declare i32 @putchar(i32)

declare i32 @puts(i8*)

declare i32 @printf(i8* noalias, ...) nounwind

define void @OnLoopIterate(i8* %loop, i8* %str) {
entry:
  %loop_addr = alloca i8*                         ; <i8**> [#uses=2]
  %str_addr = alloca i8*                          ; <i8**> [#uses=1]
  %currStackSize = alloca i32                     ; <i32*> [#uses=2]
  %i = alloca i32                                 ; <i32*> [#uses=5]
  %"alloca point" = bitcast i32 0 to i32          ; <i32> [#uses=0]
  store i8* %loop, i8** %loop_addr
  store i8* %str, i8** %str_addr
  store i32 0, i32* %i, align 4
  %0 = load i32* @loopstack_size, align 4         ; <i32> [#uses=1]
  store i32 %0, i32* %currStackSize, align 4
  store i32 0, i32* %i, align 4
  br label %bb1

bb:                                               ; preds = %bb1
  %1 = call i32 @putchar(i32 9) nounwind          ; <i32> [#uses=0]
  %2 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %3 = add nsw i32 %2, 1                          ; <i32> [#uses=1]
  store i32 %3, i32* %i, align 4
  br label %bb1

bb1:                                              ; preds = %bb, %entry
  %4 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %5 = load i32* %currStackSize, align 4          ; <i32> [#uses=1]
  %6 = icmp slt i32 %4, %5                        ; <i1> [#uses=1]
  br i1 %6, label %bb, label %bb2

bb2:                                              ; preds = %bb1
  %7 = load i8** %loop_addr, align 8              ; <i8*> [#uses=1]
  %8 = call i32 (i8*, ...)* @printf(i8* noalias getelementptr inbounds ([18 x i8]* @.str2, i64 0, i64 0), i8* %7) nounwind ; <i32> [#uses=0]
  br label %return

return:                                           ; preds = %bb2
  ret void
}

define void @OnLoopExit(i8* %loop, i8* %str) {
entry:
  %loop_addr = alloca i8*                         ; <i8**> [#uses=2]
  %str_addr = alloca i8*                          ; <i8**> [#uses=1]
  %currStackSize = alloca i32                     ; <i32*> [#uses=3]
  %i = alloca i32                                 ; <i32*> [#uses=9]
  %"alloca point" = bitcast i32 0 to i32          ; <i32> [#uses=0]
  store i8* %loop, i8** %loop_addr
  store i8* %str, i8** %str_addr
  store i32 0, i32* %i, align 4
  %0 = load i32* @loopstack_size, align 4         ; <i32> [#uses=1]
  store i32 %0, i32* %currStackSize, align 4
  store i32 0, i32* %i, align 4
  br label %bb1

bb:                                               ; preds = %bb1
  %1 = call i32 @putchar(i32 9) nounwind          ; <i32> [#uses=0]
  %2 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %3 = add nsw i32 %2, 1                          ; <i32> [#uses=1]
  store i32 %3, i32* %i, align 4
  br label %bb1

bb1:                                              ; preds = %bb, %entry
  %4 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %5 = load i32* %currStackSize, align 4          ; <i32> [#uses=1]
  %6 = icmp slt i32 %4, %5                        ; <i1> [#uses=1]
  br i1 %6, label %bb, label %bb2

bb2:                                              ; preds = %bb1
  %7 = load i8** %loop_addr, align 8              ; <i8*> [#uses=1]
  %8 = call i32 (i8*, ...)* @printf(i8* noalias getelementptr inbounds ([15 x i8]* @.str3, i64 0, i64 0), i8* %7) nounwind ; <i32> [#uses=0]
  store i32 0, i32* %i, align 4
  br label %bb4

bb3:                                              ; preds = %bb4
  %9 = call i32 @putchar(i32 9) nounwind          ; <i32> [#uses=0]
  %10 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %11 = add nsw i32 %10, 1                        ; <i32> [#uses=1]
  store i32 %11, i32* %i, align 4
  br label %bb4

bb4:                                              ; preds = %bb3, %bb2
  %12 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %13 = load i32* %currStackSize, align 4         ; <i32> [#uses=1]
  %14 = icmp slt i32 %12, %13                     ; <i1> [#uses=1]
  br i1 %14, label %bb3, label %bb5

bb5:                                              ; preds = %bb4
  %15 = call i32 @puts(i8* getelementptr inbounds ([42 x i8]* @.str4, i64 0, i64 0)) nounwind ; <i32> [#uses=0]
  br label %return

return:                                           ; preds = %bb5
  ret void
}

define %struct.LoopNode* @newLoopNode() nounwind {
entry:
  %retval = alloca %struct.LoopNode*              ; <%struct.LoopNode**> [#uses=2]
  %currLoop = alloca %struct.LoopNode*            ; <%struct.LoopNode**> [#uses=7]
  %0 = alloca %struct.LoopNode*                   ; <%struct.LoopNode**> [#uses=2]
  %"alloca point" = bitcast i32 0 to i32          ; <i32> [#uses=0]
  %1 = call noalias i8* @malloc(i64 32) nounwind  ; <i8*> [#uses=1]
  %2 = bitcast i8* %1 to %struct.LoopNode*        ; <%struct.LoopNode*> [#uses=1]
  store %struct.LoopNode* %2, %struct.LoopNode** %currLoop, align 8
  %3 = load %struct.LoopNode** %currLoop, align 8 ; <%struct.LoopNode*> [#uses=1]
  %4 = icmp eq %struct.LoopNode* %3, null         ; <i1> [#uses=1]
  br i1 %4, label %bb, label %bb1

bb:                                               ; preds = %entry
  %5 = load %struct._IO_FILE** @stderr, align 8   ; <%struct._IO_FILE*> [#uses=1]
  %6 = call i32 (%struct._IO_FILE*, i8*, ...)* @fprintf(%struct._IO_FILE* noalias %5, i8* noalias getelementptr ([38 x i8]* @.str5, i64 0, i64 0), i8* getelementptr ([33 x i8]* @.str16, i64 0, i64 0), i32 24) nounwind ; <i32> [#uses=0]
  call void @exit(i32 0) noreturn nounwind
  unreachable

bb1:                                              ; preds = %entry
  %7 = load %struct.LoopNode** %currLoop, align 8 ; <%struct.LoopNode*> [#uses=1]
  %8 = getelementptr inbounds %struct.LoopNode* %7, i32 0, i32 0 ; <%struct.LoopNode**> [#uses=1]
  store %struct.LoopNode* null, %struct.LoopNode** %8, align 8
  %9 = load %struct.LoopNode** %currLoop, align 8 ; <%struct.LoopNode*> [#uses=1]
  %10 = getelementptr inbounds %struct.LoopNode* %9, i32 0, i32 1 ; <i8**> [#uses=1]
  store i8* null, i8** %10, align 8
  %11 = load %struct.LoopNode** %currLoop, align 8 ; <%struct.LoopNode*> [#uses=1]
  %12 = getelementptr inbounds %struct.LoopNode* %11, i32 0, i32 2 ; <i8**> [#uses=1]
  store i8* null, i8** %12, align 8
  %13 = load %struct.LoopNode** %currLoop, align 8 ; <%struct.LoopNode*> [#uses=1]
  %14 = getelementptr inbounds %struct.LoopNode* %13, i32 0, i32 3 ; <i8**> [#uses=1]
  store i8* null, i8** %14, align 8
  %15 = load %struct.LoopNode** %currLoop, align 8 ; <%struct.LoopNode*> [#uses=1]
  store %struct.LoopNode* %15, %struct.LoopNode** %0, align 8
  %16 = load %struct.LoopNode** %0, align 8       ; <%struct.LoopNode*> [#uses=1]
  store %struct.LoopNode* %16, %struct.LoopNode** %retval, align 8
  br label %return

return:                                           ; preds = %bb1
  %retval2 = load %struct.LoopNode** %retval      ; <%struct.LoopNode*> [#uses=1]
  ret %struct.LoopNode* %retval2
}

declare noalias i8* @malloc(i64) nounwind

declare i32 @fprintf(%struct._IO_FILE* noalias, i8* noalias, ...) nounwind

declare void @exit(i32) noreturn nounwind

define void @initLoopNode(%struct.LoopNode** %loop, i8* %name, i8* %basicblock, i8* %parent) nounwind {
entry:
  %loop_addr = alloca %struct.LoopNode**          ; <%struct.LoopNode***> [#uses=4]
  %name_addr = alloca i8*                         ; <i8**> [#uses=2]
  %basicblock_addr = alloca i8*                   ; <i8**> [#uses=2]
  %parent_addr = alloca i8*                       ; <i8**> [#uses=2]
  %currLoop = alloca %struct.LoopNode*            ; <%struct.LoopNode**> [#uses=5]
  %"alloca point" = bitcast i32 0 to i32          ; <i32> [#uses=0]
  store %struct.LoopNode** %loop, %struct.LoopNode*** %loop_addr
  store i8* %name, i8** %name_addr
  store i8* %basicblock, i8** %basicblock_addr
  store i8* %parent, i8** %parent_addr
  store %struct.LoopNode* null, %struct.LoopNode** %currLoop, align 8
  %0 = load %struct.LoopNode*** %loop_addr, align 8 ; <%struct.LoopNode**> [#uses=1]
  %1 = icmp eq %struct.LoopNode** %0, null        ; <i1> [#uses=1]
  br i1 %1, label %bb1, label %bb

bb:                                               ; preds = %entry
  %2 = load %struct.LoopNode*** %loop_addr, align 8 ; <%struct.LoopNode**> [#uses=1]
  %3 = load %struct.LoopNode** %2, align 8        ; <%struct.LoopNode*> [#uses=1]
  %4 = icmp eq %struct.LoopNode* %3, null         ; <i1> [#uses=1]
  br i1 %4, label %bb1, label %bb2

bb1:                                              ; preds = %bb, %entry
  br label %bb3

bb2:                                              ; preds = %bb
  %5 = load %struct.LoopNode*** %loop_addr, align 8 ; <%struct.LoopNode**> [#uses=1]
  %6 = load %struct.LoopNode** %5, align 8        ; <%struct.LoopNode*> [#uses=1]
  store %struct.LoopNode* %6, %struct.LoopNode** %currLoop, align 8
  %7 = load %struct.LoopNode** %currLoop, align 8 ; <%struct.LoopNode*> [#uses=1]
  %8 = getelementptr inbounds %struct.LoopNode* %7, i32 0, i32 1 ; <i8**> [#uses=1]
  %9 = load i8** %name_addr, align 8              ; <i8*> [#uses=1]
  store i8* %9, i8** %8, align 8
  %10 = load %struct.LoopNode** %currLoop, align 8 ; <%struct.LoopNode*> [#uses=1]
  %11 = getelementptr inbounds %struct.LoopNode* %10, i32 0, i32 2 ; <i8**> [#uses=1]
  %12 = load i8** %basicblock_addr, align 8       ; <i8*> [#uses=1]
  store i8* %12, i8** %11, align 8
  %13 = load %struct.LoopNode** %currLoop, align 8 ; <%struct.LoopNode*> [#uses=1]
  %14 = getelementptr inbounds %struct.LoopNode* %13, i32 0, i32 3 ; <i8**> [#uses=1]
  %15 = load i8** %parent_addr, align 8           ; <i8*> [#uses=1]
  store i8* %15, i8** %14, align 8
  br label %bb3

bb3:                                              ; preds = %bb2, %bb1
  br label %return

return:                                           ; preds = %bb3
  ret void
}

define void @PushLoopStack(i8* %loopname, i8* %parent) {
entry:
  %loopname_addr = alloca i8*                     ; <i8**> [#uses=3]
  %parent_addr = alloca i8*                       ; <i8**> [#uses=2]
  %currLoop = alloca %struct.LoopNode*            ; <%struct.LoopNode**> [#uses=5]
  %"alloca point" = bitcast i32 0 to i32          ; <i32> [#uses=0]
  store i8* %loopname, i8** %loopname_addr
  store i8* %parent, i8** %parent_addr
  store %struct.LoopNode* null, %struct.LoopNode** %currLoop, align 8
  %0 = load i8** %loopname_addr, align 8          ; <i8*> [#uses=1]
  %1 = icmp eq i8* %0, null                       ; <i1> [#uses=1]
  br i1 %1, label %bb1, label %bb

bb:                                               ; preds = %entry
  %2 = call %struct.LoopNode* @newLoopNode() nounwind ; <%struct.LoopNode*> [#uses=1]
  store %struct.LoopNode* %2, %struct.LoopNode** %currLoop, align 8
  %3 = load i8** %loopname_addr, align 8          ; <i8*> [#uses=1]
  %4 = load i8** %parent_addr, align 8            ; <i8*> [#uses=1]
  call void @initLoopNode(%struct.LoopNode** %currLoop, i8* %3, i8* getelementptr ([1 x i8]* @.str27, i64 0, i64 0), i8* %4) nounwind
  %5 = load %struct.LoopNode** %currLoop, align 8 ; <%struct.LoopNode*> [#uses=1]
  %6 = load %struct.LoopNode** @GlobalLoopStack, align 8 ; <%struct.LoopNode*> [#uses=1]
  %7 = getelementptr inbounds %struct.LoopNode* %5, i32 0, i32 0 ; <%struct.LoopNode**> [#uses=1]
  store %struct.LoopNode* %6, %struct.LoopNode** %7, align 8
  %8 = load %struct.LoopNode** %currLoop, align 8 ; <%struct.LoopNode*> [#uses=1]
  store %struct.LoopNode* %8, %struct.LoopNode** @GlobalLoopStack, align 8
  %9 = load i32* @loopstack_size, align 4         ; <i32> [#uses=1]
  %10 = add nsw i32 %9, 1                         ; <i32> [#uses=1]
  store i32 %10, i32* @loopstack_size, align 4
  br label %bb1

bb1:                                              ; preds = %bb, %entry
  br label %return

return:                                           ; preds = %bb1
  ret void
}

define void @PopLoopStack() nounwind {
entry:
  %currLoop = alloca %struct.LoopNode*            ; <%struct.LoopNode**> [#uses=3]
  %"alloca point" = bitcast i32 0 to i32          ; <i32> [#uses=0]
  %0 = load %struct.LoopNode** @GlobalLoopStack, align 8 ; <%struct.LoopNode*> [#uses=1]
  store %struct.LoopNode* %0, %struct.LoopNode** %currLoop, align 8
  %1 = load %struct.LoopNode** %currLoop, align 8 ; <%struct.LoopNode*> [#uses=1]
  %2 = icmp ne %struct.LoopNode* %1, null         ; <i1> [#uses=1]
  br i1 %2, label %bb, label %bb1

bb:                                               ; preds = %entry
  %3 = load %struct.LoopNode** @GlobalLoopStack, align 8 ; <%struct.LoopNode*> [#uses=1]
  %4 = getelementptr inbounds %struct.LoopNode* %3, i32 0, i32 0 ; <%struct.LoopNode**> [#uses=1]
  %5 = load %struct.LoopNode** %4, align 8        ; <%struct.LoopNode*> [#uses=1]
  store %struct.LoopNode* %5, %struct.LoopNode** @GlobalLoopStack, align 8
  %6 = load %struct.LoopNode** %currLoop, align 8 ; <%struct.LoopNode*> [#uses=1]
  %7 = bitcast %struct.LoopNode* %6 to i8*        ; <i8*> [#uses=1]
  call void @free(i8* %7) nounwind
  %8 = load i32* @loopstack_size, align 4         ; <i32> [#uses=1]
  %9 = sub i32 %8, 1                              ; <i32> [#uses=1]
  store i32 %9, i32* @loopstack_size, align 4
  br label %bb1

bb1:                                              ; preds = %bb, %entry
  %10 = load %struct.LoopNode** @GlobalLoopStack, align 8 ; <%struct.LoopNode*> [#uses=1]
  %11 = icmp eq %struct.LoopNode* %10, null       ; <i1> [#uses=1]
  br i1 %11, label %bb2, label %bb3

bb2:                                              ; preds = %bb1
  store i32 0, i32* @loopstack_size, align 4
  br label %bb3

bb3:                                              ; preds = %bb2, %bb1
  br label %return

return:                                           ; preds = %bb3
  ret void
}

declare void @free(i8*) nounwind

define void @PrintLoopStack() nounwind {
entry:
  %currStackSize = alloca i32                     ; <i32*> [#uses=2]
  %i = alloca i32                                 ; <i32*> [#uses=5]
  %currLoop = alloca %struct.LoopNode*            ; <%struct.LoopNode**> [#uses=5]
  %"alloca point" = bitcast i32 0 to i32          ; <i32> [#uses=0]
  %0 = load %struct.LoopNode** @GlobalLoopStack, align 8 ; <%struct.LoopNode*> [#uses=1]
  store %struct.LoopNode* %0, %struct.LoopNode** %currLoop, align 8
  store i32 0, i32* %i, align 4
  %1 = load i32* @loopstack_size, align 4         ; <i32> [#uses=1]
  store i32 %1, i32* %currStackSize, align 4
  store i32 0, i32* %i, align 4
  br label %bb1

bb:                                               ; preds = %bb1
  %2 = call i32 @putchar(i32 9) nounwind          ; <i32> [#uses=0]
  %3 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %4 = add nsw i32 %3, 1                          ; <i32> [#uses=1]
  store i32 %4, i32* %i, align 4
  br label %bb1

bb1:                                              ; preds = %bb, %entry
  %5 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %6 = load i32* %currStackSize, align 4          ; <i32> [#uses=1]
  %7 = icmp slt i32 %5, %6                        ; <i1> [#uses=1]
  br i1 %7, label %bb, label %bb2

bb2:                                              ; preds = %bb1
  br label %bb4

bb3:                                              ; preds = %bb4
  %8 = load %struct.LoopNode** %currLoop, align 8 ; <%struct.LoopNode*> [#uses=1]
  %9 = getelementptr inbounds %struct.LoopNode* %8, i32 0, i32 1 ; <i8**> [#uses=1]
  %10 = load i8** %9, align 8                     ; <i8*> [#uses=1]
  %11 = call i32 (i8*, ...)* @printf(i8* noalias getelementptr ([7 x i8]* @.str38, i64 0, i64 0), i8* %10) nounwind ; <i32> [#uses=0]
  %12 = load %struct.LoopNode** %currLoop, align 8 ; <%struct.LoopNode*> [#uses=1]
  %13 = getelementptr inbounds %struct.LoopNode* %12, i32 0, i32 0 ; <%struct.LoopNode**> [#uses=1]
  %14 = load %struct.LoopNode** %13, align 8      ; <%struct.LoopNode*> [#uses=1]
  store %struct.LoopNode* %14, %struct.LoopNode** %currLoop, align 8
  br label %bb4

bb4:                                              ; preds = %bb3, %bb2
  %15 = load %struct.LoopNode** %currLoop, align 8 ; <%struct.LoopNode*> [#uses=1]
  %16 = icmp ne %struct.LoopNode* %15, null       ; <i1> [#uses=1]
  br i1 %16, label %bb3, label %bb5

bb5:                                              ; preds = %bb4
  %17 = call i32 @puts(i8* getelementptr ([3 x i8]* @.str49, i64 0, i64 0)) nounwind ; <i32> [#uses=0]
  br label %return

return:                                           ; preds = %bb5
  ret void
}

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
  %0 = call i32 @puts(i8* getelementptr ([11 x i8]* @.str10, i64 0, i64 0)) nounwind ; <i32> [#uses=0]
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
  %5 = load i32* %m, align 4                      ; <i32> [#uses=1]
  %6 = add nsw i32 %5, 1                          ; <i32> [#uses=1]
  store i32 %6, i32* %m, align 4
  call void @OnLoopExit(i8* noalias getelementptr ([8 x i8]* @.gstr_loopname_foo_bb6, i32 0, i32 0), i8* getelementptr ([8 x i8]* @.gstr_parentloopname_foo_bb8, i32 0, i32 0)) nounwind
  call void @PopLoopStack()
  br label %bb8

bb8:                                              ; preds = %bb7, %bb3
  %7 = load i32* %m, align 4                      ; <i32> [#uses=1]
  %8 = icmp sle i32 %7, 1                         ; <i1> [#uses=1]
  call void @OnLoopIterate(i8* noalias getelementptr ([8 x i8]* @.gstr_loopname_foo_bb8, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb10, i32 0, i32 0)) nounwind
  br i1 %8, label %bb4, label %bb9

bb9:                                              ; preds = %bb8
  %9 = load i32* %l, align 4                      ; <i32> [#uses=1]
  %10 = add nsw i32 %9, 1                         ; <i32> [#uses=1]
  store i32 %10, i32* %l, align 4
  call void @OnLoopExit(i8* noalias getelementptr ([8 x i8]* @.gstr_loopname_foo_bb8, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb10, i32 0, i32 0)) nounwind
  call void @PopLoopStack()
  br label %bb10

bb10:                                             ; preds = %bb9, %bb2
  %11 = load i32* %l, align 4                     ; <i32> [#uses=1]
  %12 = icmp sle i32 %11, 1                       ; <i1> [#uses=1]
  call void @OnLoopIterate(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb10, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb12, i32 0, i32 0)) nounwind
  br i1 %12, label %bb3, label %bb11

bb11:                                             ; preds = %bb10
  %13 = load i32* %k, align 4                     ; <i32> [#uses=1]
  %14 = add nsw i32 %13, 1                        ; <i32> [#uses=1]
  store i32 %14, i32* %k, align 4
  call void @OnLoopExit(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb10, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb12, i32 0, i32 0)) nounwind
  call void @PopLoopStack()
  br label %bb12

bb12:                                             ; preds = %bb11, %bb1
  %15 = load i32* %k, align 4                     ; <i32> [#uses=1]
  %16 = icmp sle i32 %15, 1                       ; <i1> [#uses=1]
  call void @OnLoopIterate(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb12, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb14, i32 0, i32 0)) nounwind
  br i1 %16, label %bb2, label %bb13

bb13:                                             ; preds = %bb12
  %17 = load i32* %j, align 4                     ; <i32> [#uses=1]
  %18 = add nsw i32 %17, 1                        ; <i32> [#uses=1]
  store i32 %18, i32* %j, align 4
  call void @OnLoopExit(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb12, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb14, i32 0, i32 0)) nounwind
  call void @PopLoopStack()
  br label %bb14

bb14:                                             ; preds = %bb13, %bb
  %19 = load i32* %j, align 4                     ; <i32> [#uses=1]
  %20 = icmp sle i32 %19, 1                       ; <i1> [#uses=1]
  call void @OnLoopIterate(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb14, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb16, i32 0, i32 0)) nounwind
  br i1 %20, label %bb1, label %bb15

bb15:                                             ; preds = %bb14
  %21 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %22 = add nsw i32 %21, 1                        ; <i32> [#uses=1]
  store i32 %22, i32* %i, align 4
  call void @OnLoopExit(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb14, i32 0, i32 0), i8* getelementptr ([9 x i8]* @.gstr_parentloopname_foo_bb16, i32 0, i32 0)) nounwind
  call void @PopLoopStack()
  br label %bb16

bb16:                                             ; preds = %bb15, %entry
  %23 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %24 = icmp sle i32 %23, 4                       ; <i1> [#uses=1]
  call void @OnLoopIterate(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb16, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  br i1 %24, label %bb, label %bb17

bb17:                                             ; preds = %bb16
  call void @OnLoopExit(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_foo_bb16, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  call void @PopLoopStack()
  br label %return

return:                                           ; preds = %bb17
  ret void
}

define i32 @main() nounwind {
entry:
  %retval = alloca i32                            ; <i32*> [#uses=2]
  %i = alloca i32                                 ; <i32*> [#uses=18]
  %0 = alloca i32                                 ; <i32*> [#uses=2]
  %"alloca point" = bitcast i32 0 to i32          ; <i32> [#uses=0]
  store i32 10, i32* %i, align 4
  call void @foo() nounwind
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
  call void @OnLoopExit(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_main_bb1, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  call void @PopLoopStack()
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
  call void @OnLoopExit(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_main_bb4, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  call void @PopLoopStack()
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
  call void @OnLoopExit(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_main_bb7, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  call void @PopLoopStack()
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
  %17 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %18 = icmp ne i32 %17, 0                        ; <i1> [#uses=1]
  call void @OnLoopExit(i8* noalias getelementptr ([9 x i8]* @.gstr_loopname_main_bb9, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  call void @PopLoopStack()
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
  store i32 0, i32* %0, align 4
  %21 = load i32* %0, align 4                     ; <i32> [#uses=1]
  store i32 %21, i32* %retval, align 4
  call void @OnLoopExit(i8* noalias getelementptr ([12 x i8]* @.gstr_loopname_main_label1, i32 0, i32 0), i8* getelementptr ([1 x i8]* @.gstr_parentloopname_, i32 0, i32 0)) nounwind
  call void @PopLoopStack()
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
