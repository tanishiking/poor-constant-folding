# PoorConstantFolding
## Build

```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
$ cd ..
```

## Run

```sh
opt -enable-new-pm=0 \
    -load build/poor-constant-folding/libPoorConstantFoldingPass.so \
    -poor-constant-folding \
    examples/test.ll -S
```

## Example

```
$ cat examples/add.ll
define i32 @main() {
  %x = add i32 1, 2
  ret i32 %x
}

$ opt -enable-new-pm=0 -load build/poor-constant-folding/libPoorConstantFoldingPass.so -poor-constant-folding examples/add.ll -S
; ModuleID = 'examples/add.ll'
source_filename = "examples/add.ll"

define i32 @main() {
  ret i32 3
}
```


```
$ cat examples/test.ll
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


$ opt -enable-new-pm=0 -load build/poor-constant-folding/libPoorConstantFoldingPass.so -poor-constant-folding examples/test.ll -S
; ModuleID = 'examples/test.ll'
source_filename = "examples/test.ll"

define i32 @foo() {
  ret i32 5
}

define i32 @bar() {
  ret i32 8
}
```
