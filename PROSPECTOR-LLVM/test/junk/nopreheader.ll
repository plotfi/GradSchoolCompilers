; ModuleID = 'nopreheader.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128"
target triple = "x86_64-linux-gnu"

define i32 @main() nounwind {
entry:
  %retval = alloca i32                            ; <i32*> [#uses=2]
  %i = alloca i32                                 ; <i32*> [#uses=7]
  %0 = alloca i32                                 ; <i32*> [#uses=2]
  %"alloca point" = bitcast i32 0 to i32          ; <i32> [#uses=0]
  store i32 1, i32* %i, align 4
  %1 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %2 = icmp eq i32 %1, 0                          ; <i1> [#uses=1]
  br i1 %2, label %bb, label %bb1

bb:                                               ; preds = %entry
  store i32 1, i32* %i, align 4
  br label %bb1

bb1:                                              ; preds = %bb, %entry
  br label %bb3

bb2:                                              ; preds = %bb3
  %3 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %4 = sub i32 %3, 1                              ; <i32> [#uses=1]
  store i32 %4, i32* %i, align 4
  br label %bb3

bb3:                                              ; preds = %bb2, %bb1
  %5 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %6 = icmp ne i32 %5, 0                          ; <i1> [#uses=1]
  br i1 %6, label %bb2, label %bb4

bb4:                                              ; preds = %bb3
  %7 = load i32* %i, align 4                      ; <i32> [#uses=1]
  store i32 %7, i32* %0, align 4
  %8 = load i32* %0, align 4                      ; <i32> [#uses=1]
  store i32 %8, i32* %retval, align 4
  br label %return

return:                                           ; preds = %bb4
  %retval5 = load i32* %retval                    ; <i32> [#uses=1]
  ret i32 %retval5
}
