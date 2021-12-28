#pragma once

#ifndef TOKEN_H
#define TOKEN_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

extern FILE *yyin, *yyout, *yy_parser, *yy_semantic;

typedef enum eTOKENS {

#pragma region numbers
	TOKEN_INT_NUMBER,		/* 25, -28 */
	TOKEN_FLOAT_NUMBER,			/* 25e4 = 25 * 10^4 */
#pragma endregion

#pragma region operators
	TOKEN_AR_OP,				/* + , - */
	TOKEN_REL_OP,				/* < , > , <= , >= , != , == */
	TOKEN_ASSIGNMENT_OP,		/* = */
	TOKEN_MULTIPLICATION_OP,	/* * */
	TOKEN_DIVISION_OP,			/* / */
#pragma endregion

#pragma region identifier
	TOKEN_ID,
#pragma endregion

#pragma region keywords
	TOKEN_KEYWORDS,
#pragma endregion

#pragma region seperation-signs
	TOKEN_COMMA,				/*,*/
	TOKEN_COLON,				/*:*/
	TOKEN_SEMICOLON,			/*;*/
	TOKEN_L_CURLY,				/*{*/
	TOKEN_R_CURLY,				/*}*/
	TOKEN_L_BRACKET,			/*[*/
	TOKEN_R_BRACKET,			/*]*/
	TOKEN_L_PARENTHESIS,		/*(*/
	TOKEN_R_PARENTHESIS,		/*)*/
#pragma endregion

#pragma region comments
	TOKEN_L_COMMENTS,
	TOKEN_R_COMMENTS,
#pragma endregion

#pragma region other-tokens
	//TOKEN_PROGRAM,
	TOKEN_INT,
	TOKEN_FLOAT,
	TOKEN_VOID,
	TOKEN_RETURN,
	TOKEN_IF,
	TOKEN_WHILE,
	TOKEN_OP_EQUAL,
	TOKEN_EOF
#pragma endregion

}eTOKENS;

//string replica of eTOKENS
static const char* eTokenString[] =
{
"TOKEN_INT_NUMBER",
"TOKEN_FLOAT_NUMBER",
"TOKEN_AR_OP",
"TOKEN_REL_OP",
"TOKEN_ASSIGNMENT_OP",
"TOKEN_MULTIPLICATION_OP",
"TOKEN_DIVISION_OP",
"TOKEN_ID",
"TOKEN_KEYWORDS",
"TOKEN_COMMA",
"TOKEN_COLON",
"TOKEN_SEMICOLON",
"TOKEN_L_CURLY",
"TOKEN_R_CURLY",
"TOKEN_L_BRACKET",
"TOKEN_R_BRACKET",
"TOKEN_L_PARENTHESIS",
"TOKEN_R_PARENTHESIS",
"TOKEN_L_COMMENTS",
"TOKEN_R_COMMENTS",
"TOKEN_PROGRAM",
"TOKEN_INT",
"TOKEN_FLOAT",
"TOKEN_VOID",
"TOKEN_RETURN",
"TOKEN_IF",
"TOKEN_WHILE",
"TOKEN_OP_EQUAL",
"TOKEN_EOF"
};

typedef	struct Token {
	eTOKENS tokenType;
	char* lexeme;
	int lineNumber;
}Token;

typedef	struct Node {
	Token* tokenArray;
	struct Node* prev;
	struct Node* next;
}Node;

void create_and_store_token(eTOKENS token_t, char* lex, int line);

Token* next_token();
Token* back_token();
void free_storage();
int match(eTOKENS token);
eTOKENS Handle_token(eTOKENS, char *, int);
char* toString(int);

//void init();
//eTOKENS Handle_token(eTOKENS token_t, char* lex, int line);

#endif // !TOKEN_H