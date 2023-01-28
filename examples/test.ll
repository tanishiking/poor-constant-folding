define i32 @foo() {
  %a = add i32 2, 3
  ret i32 %a
}

define i32 @bar() {
  %a = add i32 2, 3
  %b = add i32 1, 2
  %c = add i32 %a, %b
  ret i32 %c
}
