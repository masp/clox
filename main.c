#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "token_parser.h"

#define BUFLINE_SIZE   4096


static void report_error(int line, char *err_msg) {
  printf("\n\nError occurred on line %d: %s\n", line, err_msg);
  exit(64);
}

static void run(char *source, size_t source_len) {
  printf("Running source:\n%s\n", source);

  TOKEN_PARSER_HANDLE tk_parser = new_token_parser(source, source_len);
  while (getchar() != EOF) {
    TOKEN token = tk_parser->next_token(tk_parser);
    printf("Parsed token of type %s\n", get_token_name(token));
  }
  tk_parser->destroy(tk_parser);
}

static void run_file(char *file_name) {
  printf("Loading file %s...\n", file_name);
  char *source = NULL;
  FILE *f = fopen(file_name, "r");
  if (f != NULL) {
    if (fseek(f, 0, SEEK_END) == 0) {
      long bufsize = ftell(f);
      if (bufsize == -1) {
        printf("Error: unable to find filesize\n");
        fclose(f);
        exit(63);
      }
      if (bufsize >= SIZE_MAX) {
        printf("Error: file too large\n");
        fclose(f);
        exit(62);
      }

      source = (char*) malloc((size_t) bufsize + 1);
      if (fseek(f, 0, SEEK_SET) != 0) {
        printf("Error: unable to return to beginning of file for reading\n");
        free(source);
        fclose(f);
        exit(61);
      }

      size_t len = fread(source, sizeof(char), (size_t) bufsize, f);
      if (len > 0) {
        source[len] = '\0';

        run(source, len+1);
      } else {
        printf("Unable to read from file\n");
      }
      fclose(f);
      free(source);
    }
  }
}

static void exit_interpreter() {
  printf("\nExiting...\n");
  exit(0);
}

static void run_interpreter() {
  char *line = (char*) malloc(BUFLINE_SIZE);
  for (;;) {
    printf("\n> ");

    int i = 0;
    int c;
    while (i < BUFLINE_SIZE && (c = getchar()) != '\n') {
      if (c == EOF) {
        exit_interpreter();
      }
      line[i] = (char) c;
      i++;
    }
    line[i] = 0;

    if (strncmp(line, "exit", 4) == 0) {
      exit_interpreter();
      break;
    }

    run(line, (size_t) i + 1);
  }
}

int main(int argc, char **argv) {
  if (argc == 2) {
    run_file(argv[1]);
  } else if (argc == 1) {
    run_interpreter();
  } else {
    printf("Usage: clox [script]\n");
    return 64;
  }
  return 0;
}