; ModuleID = 'indirect.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128"
target triple = "x86_64-linux-gnu"

@codetable.1443 = internal global [1 x i8*] [i8* inttoptr (i64 1 to i8*)] ; <[1 x i8*]*> [#uses=1]

define i32 @main() nounwind {
entry:
  %retval = alloca i32                            ; <i32*> [#uses=1]
  %gotovar.1 = alloca i8*                         ; <i8**> [#uses=2]
  %i = alloca i32                                 ; <i32*> [#uses=6]
  %opcode = alloca i32                            ; <i32*> [#uses=2]
  %memtmp = alloca i64                            ; <i64*> [#uses=2]
  %"alloca point" = bitcast i32 0 to i32          ; <i32> [#uses=0]
  store i32 0, i32* %opcode, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %i, align 4
  br label %bb3

bb:                                               ; preds = %bb3
  %0 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %1 = icmp eq i32 %0, 4                          ; <i1> [#uses=1]
  br i1 %1, label %bb1, label %bb2

bb1:                                              ; preds = %bb
  %2 = load i32* %opcode, align 4                 ; <i32> [#uses=1]
  %3 = sext i32 %2 to i64                         ; <i64> [#uses=1]
  %4 = getelementptr inbounds [1 x i8*]* @codetable.1443, i64 0, i64 %3 ; <i8**> [#uses=1]
  %5 = load i8** %4, align 8                      ; <i8*> [#uses=1]
  store i8* %5, i8** %gotovar.1, align 8
  %6 = load i8** %gotovar.1, align 8              ; <i8*> [#uses=1]
  %7 = ptrtoint i8* %6 to i64                     ; <i64> [#uses=1]
  store i64 %7, i64* %memtmp
  br label %indirectgoto

bb2:                                              ; preds = %bb
  %8 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %9 = add nsw i32 %8, 1                          ; <i32> [#uses=1]
  store i32 %9, i32* %i, align 4
  br label %bb3

bb3:                                              ; preds = %bb2, %entry
  %10 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %11 = icmp sle i32 %10, 9                       ; <i1> [#uses=1]
  br i1 %11, label %bb, label %bb4

bb4:                                              ; preds = %bb3
  br label %LABEL1

LABEL1:                                           ; preds = %indirectgoto, %indirectgoto, %bb4
  br label %return

return:                                           ; preds = %LABEL1
  %retval5 = load i32* %retval                    ; <i32> [#uses=1]
  ret i32 %retval5

indirectgoto:                                     ; preds = %bb1
  %gotodest = load i64* %memtmp                   ; <i64> [#uses=1]
  switch i64 %gotodest, label %LABEL1 [
    i64 1, label %LABEL1
  ]
}
