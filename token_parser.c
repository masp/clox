//
// Created by Mark Asp on 10/17/18.
//
#include "token_parser.h"

#include <stdlib.h>
#include <memory.h>
#include <ctype.h>
#include <stdio.h>

static const char *token_type_names[] = {
  "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE",
  "COMMA", "DOT", "MINUS", "PLUS", "SEMICOLON", "SLASH", "STAR",

  // One or two character tokens.
  "BANG", "BANG_EQUAL",
  "EQUAL", "EQUAL_EQUAL",
  "GREATER", "GREATER_EQUAL",
  "LESS", "LESS_EQUAL",

  // Literals.
  "IDENTIFIER", "STRING", "NUMBER",

  // Keywords.
  "AND", "CLASS", "ELSE", "FALSE", "FUN", "FOR", "IF", "NIL", "OR",
  "PRINT", "RETURN", "SUPER", "THIS", "TRUE", "VAR", "WHILE",

  "EOF", "UNKNOWN"
};

const char* get_token_name(TOKEN token) {
  return token_type_names[token.type];
}

static const struct token_parser_t Empty_Token_Parser;

char token_parser_next_char(TOKEN_PARSER_HANDLE h) {
  if (h->iter_idx >= h->source_len) {
    return 0;
  }
  return h->source[h->iter_idx++];
}

char token_parser_peek_char(TOKEN_PARSER_HANDLE h) {
  return h->source[h->iter_idx];
}

/* Start parsing logic */

static int isend(int c) {
  return isspace(c) || c == ';' || c == EOF;
}

static SLICE read_identifier(TOKEN_PARSER_HANDLE h) {
  SLICE slice = { 0 };
  slice.val = &h->source[h->iter_idx];
  int c;
  while (!isend(c = h->peek_char(h))) {
    slice.len++;
    h->next_char(h);
  }
  return slice;
}

TOKEN token_parser_next_token(TOKEN_PARSER_HANDLE h) {
  int start_char = h->peek_char(h);
  switch (start_char) {
    case '(': return (TOKEN) { LEFT_PAREN };
    case ')': return (TOKEN) { RIGHT_PAREN };
    case '{': return (TOKEN) { LEFT_BRACE };
    case '}': return (TOKEN) { RIGHT_BRACE };
    case ',': return (TOKEN) { COMMA };
    case '.': return (TOKEN) { DOT };
    case '-': return (TOKEN) { MINUS };
    case '+': return (TOKEN) { PLUS };
    case ';': return (TOKEN) { SEMICOLON };
    case '/': return (TOKEN) { SLASH };
    case '*': return (TOKEN) { STAR };
    case EOF: return (TOKEN) { EOF_TOKEN };


      IDENTIFIER, STRING, NUMBER,
    default:
      if (isalpha(start_char)) {
        TOKEN identifier = read_identifier(h);
      } else if (isnumber(start_char)) {

      }
      return (TOKEN) { UNKNOWN_TOKEN };
  }
}

/* End parsing logic */

void destroy_token_parser(TOKEN_PARSER_HANDLE h) {
  if (h != NULL) free(h);
}

TOKEN_PARSER_HANDLE new_token_parser(char *source, size_t len) {
  TOKEN_PARSER_HANDLE token_parser = (TOKEN_PARSER_HANDLE)malloc(sizeof(struct token_parser_t));
  memcpy(token_parser, &Empty_Token_Parser, sizeof(struct token_parser_t));
  token_parser->peek_char = token_parser_peek_char;
  token_parser->next_char = token_parser_next_char;
  token_parser->destroy = destroy_token_parser;

  return token_parser;
}
