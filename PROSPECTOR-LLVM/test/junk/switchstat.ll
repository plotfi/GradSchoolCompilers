; ModuleID = 'switchstat.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128"
target triple = "x86_64-linux-gnu"

define i32 @main() nounwind {
entry:
  %retval = alloca i32                            ; <i32*> [#uses=2]
  %a = alloca i32                                 ; <i32*> [#uses=6]
  %0 = alloca i32                                 ; <i32*> [#uses=2]
  %"alloca point" = bitcast i32 0 to i32          ; <i32> [#uses=0]
  store i32 8, i32* %a, align 4
  br label %bb4

bb:                                               ; preds = %bb4
  %1 = load i32* %a, align 4                      ; <i32> [#uses=1]
  switch i32 %1, label %bb3 [
    i32 1, label %bb1
    i32 2, label %bb2
  ]

bb1:                                              ; preds = %bb
  store i32 0, i32* %a, align 4
  br label %bb4

bb2:                                              ; preds = %bb
  store i32 1, i32* %a, align 4
  br label %bb4

bb3:                                              ; preds = %bb
  store i32 2, i32* %a, align 4
  br label %bb4

bb4:                                              ; preds = %bb3, %bb2, %bb1, %entry
  %2 = load i32* %a, align 4                      ; <i32> [#uses=1]
  %3 = icmp sgt i32 %2, 0                         ; <i1> [#uses=1]
  br i1 %3, label %bb, label %bb5

bb5:                                              ; preds = %bb4
  store i32 0, i32* %0, align 4
  %4 = load i32* %0, align 4                      ; <i32> [#uses=1]
  store i32 %4, i32* %retval, align 4
  br label %return

return:                                           ; preds = %bb5
  %retval6 = load i32* %retval                    ; <i32> [#uses=1]
  ret i32 %retval6
}
