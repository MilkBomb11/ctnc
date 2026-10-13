all:
	gcc src/main.c src/lexer_debug.c src/parser.c src/ast.c src/token.c src/char_bank.c src/string_view.c src/args.c -o main

.PHONY: run
run:
	make && ./main
