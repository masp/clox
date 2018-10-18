//
// Created by Mark Asp on 10/17/18.
//

#ifndef CLOX_TOKEN_PARSER_H
#define CLOX_TOKEN_PARSER_H

#include <stddef.h>

typedef enum Token_Type {
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  EOF_TOKEN, UNKNOWN_TOKEN
} Token_Type;

typedef struct slice_t {
  char *val;
  size_t len;
} SLICE;

typedef union token_data_t {
  SLICE str_data;
  double d_value;
} TOKEN_DATA;

typedef struct token_t {
  Token_Type type;
  TOKEN_DATA value;
} TOKEN;

const char* get_token_name(TOKEN token);

typedef struct token_parser_t {
  char *source;
  size_t source_len;
  size_t iter_idx;

  TOKEN (*next_token) (struct token_parser_t *h);
  int   (*peek_char)  (struct token_parser_t *h);
  int   (*next_char)  (struct token_parser_t *h);
  void  (*destroy)    (struct token_parser_t *h);
} *TOKEN_PARSER_HANDLE;

TOKEN_PARSER_HANDLE new_token_parser(char *source, size_t source_len);

#endif //CLOX_TOKEN_PARSER_H
