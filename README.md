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
