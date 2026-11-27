CC := gcc
LEX := flex
YACC := bison
CFLAGS := -Wall -Wextra -Iinclude -O2

PARSER_C := src/parser.c
PARSER_H := include/parser.h
LEXER_C := src/lexer.c
TARGET := minisql.exe

.PHONY: all clean test test_err

all: $(TARGET)

$(PARSER_C) $(PARSER_H): src/parser.y
	$(YACC) -d --defines=$(PARSER_H) -o $(PARSER_C) src/parser.y

$(LEXER_C): src/lexer.l $(PARSER_H)
	$(LEX) -o $(LEXER_C) src/lexer.l

$(TARGET): $(PARSER_C) $(LEXER_C) src/ast.c src/symtab.c src/codegen.c src/main.c
	$(CC) $(CFLAGS) -o $@ $(PARSER_C) $(LEXER_C) src/ast.c src/symtab.c src/codegen.c src/main.c

test: $(TARGET)
	./$(TARGET) tests/test_valid.sql

test_err: $(TARGET)

	-./$(TARGET) tests/test_error.sql
	./$(TARGET) tests/test_error.sql

clean:
	rm -f $(TARGET) $(PARSER_C) $(PARSER_H) $(LEXER_C)