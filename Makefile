SRCS := src/main.c \
		src/lexer_debug.c \
		src/parser.c \
		src/ast.c \
		src/token.c \
		src/char_bank.c \
		src/string_view.c \
		src/string_builder.c \
		src/args.c \
		src/printer.c \
		src/frontend.c \
		src/paths.c

TARGET := ctnc

TEST_FILE := test/test.ctnc

all:
	gcc $(SRCS) -o $(TARGET)

.PHONY: run
run:
	make && ./$(TARGET) $(TEST_FILE)

.PHONY: clean
clean:
	rm $(TARGET)
