# CTNC

C's type notation is unintuitive so I made a translator which detects colon notated types (similar to Rust, Typescript) and translates it to C notations. This allows you to write code in a type notating style similar to Rust in C. It only works for variable, functions, and struct fields definitions/declarations for now.
- - -
## Build from Source
clone this repo and do
```
  make
```
this will create an executable that can be called.

## Usage
```
  <path/to/ctnc> [-h|--help] [-s|--stdout] <file/to/translate1> <file/to/translate2> ...
```
Omitting `-s` will save the translation result of each file to a new file. The new file will have the same name as the original file but with `.c` tacked on. When the original file ends with `.ctnc` it will be replaced with `.c`.

## Flags
- `-h`, `--help`: prints out usage info.
- `-s`, `--stdout`: outputs the translation result to stdout instead of a file.

## Eamples
```c  
x: static{const{ptr{int}}};
zd: static{const{ptr{struct x}}};
arrx: ptr{arr{View,3}} a

f: fn (x: int64, velocity: static{ptr{int}}, z: struct hello, he: int86, hello: size_t) -> const{float};
x: ptr { fn(x: fn () -> struct y ) -> ptr{ptr{void}} };
y: fn(x: fn () -> struct y ) -> void;

f: ptr {fn (val: const{ptr{char}}) -> ptr{const{char}} };

add: fn (x: struct hello, y: Int64, update: ptr{fn (hello: ptr{int}) -> const {static{ptr{ size_t }}} } ) -> ptr{struct T}
{
  ...
}
```
becomes
```c  
static const int *x;
static const struct x *zd;
View (*arrx)[3] a

const float f(int64 x, static int *velocity, struct hello z, int86 he, size_t hello);
void **(*x)(struct y x());
void y(struct y x());

const char *(*f)(const char *val);

struct T *add(struct hello x, Int64 y, const static size_t *(*update)(int *hello))
{

}
```
