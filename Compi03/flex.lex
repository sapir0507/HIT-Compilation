
%{
    #define _CRT_SECURE_NO_WARNINGS
    #pragma warning(disable : 4996)
    #include "Token.h"
    #include "parser.h"
    #include "semantic.h"
    #include "stdio.h"
    #include "stdlib.h"
    int currentLineNumber=1;
    FILE *yy_parser = NULL;
    FILE *yy_semantic = NULL;
    void error(char*,int);
    void printToken(Token* i_Token);
%}


KEYWORDS_INT             "int"
KEYWORDS_FLOAT        "float"
KEYWORDS_VOID          "void"
KEYWORDS_IF                "if"
KEYWORDS_RETURN     "return"
KEYWORDS_WHILE        "while"


DIGIT                                     [0-9]
NONZERO                             [1-9]
CHAR                                     [A-Za-z]
CHARNUM                            [a-zA-Z0-9]


ID	                        [a-z]([A-Za-z0-9]|"_"[A-Za-z0-9])*
WRONG_ID	      (_)?[A-Za-z]([A-Za-z0-9]|"_"+[A-Za-z0-9])*
FLOAT_NUMBER            [1-9][0-9]*"."(0|[0-9]*)("e"("-"|"+")?(0|[0-9]*))?
INT_NUMBER                 ({NONZERO}{DIGIT}*|0)

ASSIGNMENT_OP                  "="
REL_OP                                   "<="|">="|"=="|"!="|"<"|">"


AR_OP                                    "+"      
COMMA                                  ","
COLON                                    ":"
SEMICOLON                            ";"
TOKEN_R_CURLY                     "}"
TOKEN_L_CURLY                     "{"
BRACKETS_RIGHT                   "["
BRACKETS_LEFT                      "]"
PARENTHESIS_RIGHT              ")"
PARENTHESIS_LEFT                 "("
COMMENT_RIGHT                  "/*"
COMMENT_LEFT                     "*/"
SPACE                                       " "
TAB                                           [\t]
NEWLINE                                  [\n]


%%

{KEYWORDS_FLOAT}      {Handle_token(TOKEN_FLOAT,yytext,currentLineNumber); return;}

{KEYWORDS_INT}           {Handle_token(TOKEN_INT,yytext,currentLineNumber); return;}

{KEYWORDS_VOID}        {Handle_token(TOKEN_VOID,yytext,currentLineNumber); return;}

{KEYWORDS_RETURN}  {Handle_token(TOKEN_RETURN,yytext,currentLineNumber); return;}

{KEYWORDS_IF}              {Handle_token(TOKEN_IF,yytext,currentLineNumber); return;}

{KEYWORDS_WHILE}     {Handle_token(TOKEN_WHILE,yytext,currentLineNumber); return;}

{COMMA}                       {Handle_token(TOKEN_COMMA,yytext,currentLineNumber); return;}

{COLON}                        {Handle_token(TOKEN_COLON,yytext,currentLineNumber); return;}

{SEMICOLON}         {Handle_token(TOKEN_SEMICOLON,yytext,currentLineNumber); return;}

{PARENTHESIS_RIGHT}   {Handle_token(TOKEN_R_PARENTHESIS,yytext,currentLineNumber); return;}

{PARENTHESIS_LEFT}  {Handle_token(TOKEN_L_PARENTHESIS,yytext,currentLineNumber); return;}

{TOKEN_R_CURLY} {Handle_token(TOKEN_R_CURLY,yytext,currentLineNumber); return;}

{TOKEN_L_CURLY} {Handle_token(TOKEN_L_CURLY,yytext,currentLineNumber); return;}

{BRACKETS_LEFT} {Handle_token(TOKEN_L_BRACKET,yytext,currentLineNumber); return;}

{BRACKETS_RIGHT} {Handle_token(TOKEN_R_BRACKET,yytext,currentLineNumber); return;}



{ID}                {Handle_token(TOKEN_ID,yytext,currentLineNumber); return;}


{WRONG_ID}       fprintf(yyout,"The string %s at line: %d isn't a legal token in the language.\n", yytext, currentLineNumber);

{AR_OP}            fprintf(yyout,"Token of kind '%s' was found at line: %d, lexeme: '%s'. \n", "TOKEN_AR_OP", currentLineNumber, yytext); create_and_store_token(TOKEN_AR_OP, yytext, currentLineNumber); return;

{REL_OP}           fprintf(yyout,"Token of kind '%s' was found at line: %d, lexeme: '%s'. \n", "TOKEN_REL_OP", currentLineNumber, yytext); create_and_store_token(TOKEN_REL_OP, yytext, currentLineNumber); return;

{INT_NUMBER}	   fprintf(yyout,"Token of kind '%s' was found at line: %d, lexeme: '%s'. \n", "TOKEN_INT_NUMBER", currentLineNumber, yytext); create_and_store_token(TOKEN_INT_NUMBER, yytext, currentLineNumber); return;

{FLOAT_NUMBER}     fprintf(yyout,"Token of kind '%s' was found at line: %d, lexeme: '%s'. \n", "TOKEN_FLOAT_NUMBER", currentLineNumber, yytext); create_and_store_token(TOKEN_FLOAT_NUMBER, yytext, currentLineNumber); return;

<<EOF>>		       fprintf(yyout,"Token of kind '%s' was found at line: %d.\n", "TOKEN_EOF", currentLineNumber, yytext); create_and_store_token(TOKEN_EOF, yytext, currentLineNumber); return 0;

"/*"[^*]*"*/"     /*do nothing till ‘*’  */

{COMMENT_RIGHT}  {Handle_token(TOKEN_R_COMMENTS,yytext,currentLineNumber); return;}

{COMMENT_LEFT}   {Handle_token(TOKEN_L_COMMENTS,yytext,currentLineNumber); return;}

{ASSIGNMENT_OP}  {Handle_token(TOKEN_ASSIGNMENT_OP,yytext,currentLineNumber); return;}

"*"              {Handle_token(TOKEN_MULTIPLICATION_OP,yytext,currentLineNumber); return;}

"/"                {Handle_token(TOKEN_DIVISION_OP,yytext,currentLineNumber); return;}

{NEWLINE}   	 currentLineNumber++;

{TAB}                           {}

{SPACE}     	 {}	

"."		{error(yytext,currentLineNumber);}

"!"                               {error(yytext,currentLineNumber);}

%%

void printToken(Token* i_Token)
{
	fprintf(yyout,"Token kind: %d. Lexeme:'%s'. Line:%d. \n", i_Token->tokenType, i_Token->lexeme, i_Token->lineNumber);
}

void error(char *lexeme ,int lineNum)
{
	printf("error: unrecognized character in line %4d: %4s\n",lineNum,lexeme);
}

int main(int argc, char **argv )
{

	Token *currToken;
	
	//--------------------------Test1-------------------------------------
	yyin  = fopen("C:\\temp\\test1.txt","r");
	yyout = fopen("C:\\temp\\test1_205668064_302846621_lex.txt","w");
	yy_parser = fopen("C:\\temp\\test1_205668064_302846621_syntatic.txt","w"); /*output file*/
	yy_semantic = fopen("C:\\temp\\test1_205668064_302846621_semantic.txt","w"); /*output file*/
	
	if(yyin)
                 {
	Parse();
	free_storage();
	fclose(yyin);
	fclose(yyout);
	fclose(yy_parser);
	fclose(yy_semantic);
	}
                 else
                 {
		printf("Error: The file 'test1' not found.\n");
	}
	//--------------------------Test2-------------------------------------


                 init();
	currentLineNumber = 1;

	yyin  = fopen("C:\\temp\\test2.txt","r");
	yyout = fopen("C:\\temp\\test2_205668064_302846621_lex.txt","w");
	yy_parser = fopen("C:\\temp\\test2_205668064_302846621_syntatic.txt","w"); /*output file*/
	yy_semantic = fopen("C:\\temp\\test2_205668064_302846621_semantic.txt","w"); /*output file*/


                 if(yyin)
                 {
	Parse();
	free_storage();
	fclose(yyin);
	fclose(yyout);
	fclose(yy_parser);
	fclose(yy_semantic);
	}
                 else
                 {
		printf("Error: The file 'test2' not found.\n");
	}
                 return 0;
}

int yywrap()
{
return 1;
}