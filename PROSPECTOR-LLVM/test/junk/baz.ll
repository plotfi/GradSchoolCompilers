; ModuleID = 'baz.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128"
target triple = "x86_64-linux-gnu"

define i32 @main() nounwind {
entry:
  %retval = alloca i32                            ; <i32*> [#uses=2]
  %i = alloca i32                                 ; <i32*> [#uses=4]
  %0 = alloca i32                                 ; <i32*> [#uses=2]
  %"alloca point" = bitcast i32 0 to i32          ; <i32> [#uses=0]
  store i32 1, i32* %i, align 4
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
  store i32 0, i32* %0, align 4
  %5 = load i32* %0, align 4                      ; <i32> [#uses=1]
  store i32 %5, i32* %retval, align 4
  br label %return

return:                                           ; preds = %bb2
  %retval3 = load i32* %retval                    ; <i32> [#uses=1]
  ret i32 %retval3
}
