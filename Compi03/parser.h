#include "token.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Parse();
void parse_PROG();

int parse_GLOBAL_VAR_DEFS();
int parse_VAR_DEFINITIONS(enum action);
int parse_VAR_DEFINITIONS_t(); // 'a'
int parse_VAR_DEFINITION(enum action);

enum type parse_TYPE();

int parse_VARIABLES_LIST(enum action, enum type);
int parse_VARIABLES_LIST_t(enum action, enum type);

int parse_VARIABLE(enum action, enum type);
int parse_VARIABLE_t(); // 'b'

void parse_FUNC_PRE_DEFS();
void parse_FUNC_DEFINITIONS_t();
void parse_FUNC_DEFINITION();
int  IsDeclration();

void parse_FUNC_FULL_DEFINITION();
void parse_FUNC_FULL_DEFINITIONS_t();
void parse_FUNC_FULL_DEFINITION();
int parse_PARAM_FULL_DEFINITIONS(enum action);

enum Type parse_RETURNED_TYPE();
int parse_PARAM_DEFINITIONS();

void parse_STATEMENTS();
void parse_STATEMENTS_t(); // 'c'
void parse_STATEMENT();
void parse_STATEMENT_t(); // 'd'
void parse_STATEMENT_t2(struct symbol*); // 'E'
void parse_BLOCK();
int parse_PARAMETERS_LIST();
enum Type parse_EXPRESSION();
enum Type parse_EXPRESSION_t(struct symbol*); // 'F'