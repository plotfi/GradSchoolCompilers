; ModuleID = 'foo.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128"
target triple = "x86_64-linux-gnu"

@.str = private constant [11 x i8] c"Inner loop\00", align 1 ; <[11 x i8]*> [#uses=1]

define void @foo() nounwind {
entry:
  %j = alloca i32                                 ; <i32*> [#uses=4]
  %i = alloca i32                                 ; <i32*> [#uses=4]
  %"alloca point" = bitcast i32 0 to i32          ; <i32> [#uses=0]
  store i32 0, i32* %i, align 4
  br label %bb4

bb:                                               ; preds = %bb4
  store i32 0, i32* %j, align 4
  br label %bb2

bb1:                                              ; preds = %bb2
  %0 = call i32 @puts(i8* getelementptr inbounds ([11 x i8]* @.str, i64 0, i64 0)) nounwind ; <i32> [#uses=0]
  %1 = load i32* %j, align 4                      ; <i32> [#uses=1]
  %2 = add nsw i32 %1, 1                          ; <i32> [#uses=1]
  store i32 %2, i32* %j, align 4
  br label %bb2

bb2:                                              ; preds = %bb1, %bb
  %3 = load i32* %j, align 4                      ; <i32> [#uses=1]
  %4 = icmp sle i32 %3, 1                         ; <i1> [#uses=1]
  br i1 %4, label %bb1, label %bb3

bb3:                                              ; preds = %bb2
  %5 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %6 = add nsw i32 %5, 1                          ; <i32> [#uses=1]
  store i32 %6, i32* %i, align 4
  br label %bb4

bb4:                                              ; preds = %bb3, %entry
  %7 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %8 = icmp sle i32 %7, 4                         ; <i1> [#uses=1]
  br i1 %8, label %bb, label %bb5

bb5:                                              ; preds = %bb4
  br label %return

return:                                           ; preds = %bb5
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
  br label %whileloop

gwb:                                              ; preds = %bb2
  br label %forloop

gfl:                                              ; preds = %bb5
  br label %whileloop2

gw2:                                              ; preds = %bb8
  br label %dooloop

whileloop:                                        ; preds = %entry
  br label %bb1

bb:                                               ; preds = %bb1
  %1 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %2 = sub i32 %1, 1                              ; <i32> [#uses=1]
  store i32 %2, i32* %i, align 4
  br label %bb1

bb1:                                              ; preds = %bb, %whileloop
  %3 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %4 = icmp ne i32 %3, 0                          ; <i1> [#uses=1]
  br i1 %4, label %bb, label %bb2

bb2:                                              ; preds = %bb1
  br label %gwb

forloop:                                          ; preds = %gwb
  store i32 0, i32* %i, align 4
  br label %bb4

bb3:                                              ; preds = %bb4
  %5 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %6 = add nsw i32 %5, 1                          ; <i32> [#uses=1]
  store i32 %6, i32* %i, align 4
  br label %bb4

bb4:                                              ; preds = %bb3, %forloop
  %7 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %8 = icmp sle i32 %7, 4                         ; <i1> [#uses=1]
  br i1 %8, label %bb3, label %bb5

bb5:                                              ; preds = %bb4
  br label %gfl

whileloop2:                                       ; preds = %gfl
  br label %bb7

bb6:                                              ; preds = %bb7
  %9 = load i32* %i, align 4                      ; <i32> [#uses=1]
  %10 = sub i32 %9, 1                             ; <i32> [#uses=1]
  store i32 %10, i32* %i, align 4
  br label %bb7

bb7:                                              ; preds = %bb6, %whileloop2
  %11 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %12 = icmp ne i32 %11, 0                        ; <i1> [#uses=1]
  br i1 %12, label %bb6, label %bb8

bb8:                                              ; preds = %bb7
  br label %gw2

dooloop:                                          ; preds = %gw2
  store i32 2, i32* %i, align 4
  br label %bb9

bb9:                                              ; preds = %bb9, %dooloop
  %13 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %14 = sub i32 %13, 1                            ; <i32> [#uses=1]
  store i32 %14, i32* %i, align 4
  %15 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %16 = icmp ne i32 %15, 0                        ; <i1> [#uses=1]
  br i1 %16, label %bb9, label %bb10

bb10:                                             ; preds = %bb9
  %17 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %18 = icmp ne i32 %17, 0                        ; <i1> [#uses=1]
  br i1 %18, label %end, label %label1

label:                                            ; preds = %label1
  store i32 1, i32* %i, align 4
  %19 = load i32* %i, align 4                     ; <i32> [#uses=1]
  %20 = icmp ne i32 %19, 0                        ; <i1> [#uses=1]
  br i1 %20, label %end, label %label1

label1:                                           ; preds = %label, %bb10
  br label %label

end:                                              ; preds = %label, %bb10
  store i32 0, i32* %0, align 4
  %21 = load i32* %0, align 4                     ; <i32> [#uses=1]
  store i32 %21, i32* %retval, align 4
  br label %return

return:                                           ; preds = %end
  %retval11 = load i32* %retval                   ; <i32> [#uses=1]
  ret i32 %retval11
}
