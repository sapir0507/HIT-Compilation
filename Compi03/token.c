#include "token.h"

/*
There are four functions of token handling:
- function create_and_store_tokens ; it is called by the lexical analyzer when it identifies a legal token in the input text.
- functions next_token and back_token; they are called by parser during the syntax analysis (the second stage of compilation)
- function for free_memory
*/

int currentIndex = 0;
int out_of_bounds = 0;
Node* currentNode = NULL;

#define TOKEN_ARRAY_SIZE 100

/*
* This function creates a token and stores it in the storage.
*/
void create_and_store_token(eTOKENS token_t, char* lex, int line)
{
	int length = strlen(lex) + 1;

	// case 1: there is still no tokens in the storage.
	if (currentNode == NULL)
	{
		currentNode = (Node*)malloc(sizeof(Node));

		if (currentNode == NULL)
		{
			fprintf(yyout, "\nUnable to allocate memory! \n");
			exit(0);
		}
		currentNode->tokenArray = (Token*)calloc(sizeof(Token), TOKEN_ARRAY_SIZE);
		if (currentNode->tokenArray == NULL)
		{
			fprintf(yyout, "\nUnable to allocate memory! \n");
			exit(0);
		}
		currentNode->prev = NULL;
		currentNode->next = NULL;
	}

	// case 2: at least one token exsits in the storage.
	else
	{
		// the array (the current node) is full, need to allocate a new node
		if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			currentIndex = 0;
			currentNode->next = (Node*)malloc(sizeof(Node));

			if (currentNode == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory! \n");
				exit(0);
			}
			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
			currentNode->tokenArray = (Token*)calloc(sizeof(Token), TOKEN_ARRAY_SIZE);

			if (currentNode->tokenArray == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory! \n");
				exit(0);
			}
			currentNode->next = NULL;
		}

		// the array (the current node) is not full
		else
		{
			currentIndex++;
		}
	}

	currentNode->tokenArray[currentIndex].tokenType = token_t;
	currentNode->tokenArray[currentIndex].lineNumber = line;

	currentNode->tokenArray[currentIndex].lexeme = (char*)malloc(sizeof(char) * length);
#ifdef _WIN32
	strcpy_s(currentNode->tokenArray[currentIndex].lexeme, length, lex);
#else
	strcpy(currentNode->tokenArray[currentIndex].lexeme, lexeme);
#endif
}


/*
* This function returns the token in the storage that is stored immediately before the current token (if exsits).
*/
Token* back_token()
{
	if (currentIndex == 0)
	{
		if (currentNode->prev != NULL)
		{
			currentNode = currentNode->prev;
			currentIndex = TOKEN_ARRAY_SIZE - 1;
		}
		else
		{
			if (out_of_bounds)
			{
				fprintf(yy_parser, "Back token was expected but first token returned");
			}
			else
			{
				out_of_bounds = 1;
			}
		}
	}
	else
	{
		currentIndex--;
	}
	return &currentNode->tokenArray[currentIndex];
}

/*
* If the next token already exists in the storage (this happens when back_token was called before this call to next_token):
*  this function returns the next stored token.
* Else: continues to read the input file in order to identify, create and store a new token (using yylex function);
*  returns the token that was created.
*/
Token* next_token()
{
	if (currentNode != NULL)
	{
		if (currentNode->tokenArray[currentIndex + 1].lexeme != NULL && currentIndex < TOKEN_ARRAY_SIZE - 1)//
		{
			currentIndex++;
		}
		else
		{
			if (currentNode->next != NULL)
			{
				currentNode = currentNode->next;
				currentIndex = 0;
			}
			else
			{
				if (currentNode->tokenArray[currentIndex].tokenType != TOKEN_EOF)
				{
					yylex();
				}
			}

		}
	}

	else
	{
		yylex();
	}

	out_of_bounds = 0;
	return &currentNode->tokenArray[currentIndex];
}


int match(eTOKENS token)
{
	Token* curr_token = next_token();

	if (curr_token->tokenType != token)
	{
		fprintf(yy_parser, "Expected: token '%s' at line %d ", eTokenString[token], curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s', lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);
		return 0;
	}

	return 1;
}

void free_storage()
{
	Node* temp = NULL;

	// take the currentNode to end of list
	while (currentNode->next != NULL)
	{
		currentNode = currentNode->next;
	}

	// remove and free all memory in list storege
	while (currentNode->prev != NULL)
	{
		temp = currentNode;
		for (int i = 0; i < TOKEN_ARRAY_SIZE; i++)
		{
			if (temp->tokenArray[i].lexeme != NULL)
			{
				free(temp->tokenArray[i].lexeme);
			}
		}
		free(temp->tokenArray);
		currentNode = currentNode->prev;
		free(temp);

	}
	for (int i = 0; i < TOKEN_ARRAY_SIZE; i++)
	{
		if (currentNode->tokenArray[i].lexeme != NULL)
		{
			free(currentNode->tokenArray[i].lexeme);
		}
	}

	free(currentNode->tokenArray);
	free(currentNode);

	currentNode = NULL;
	currentIndex = 0;
}

eTOKENS Handle_token(eTOKENS kind, char *lexeme, int lineNum) {
	create_and_store_token(kind, lexeme, lineNum);
	fprintf(yyout, "Token of kind: %-20s \t found at line: %3d. \t lexeme: %s\n", toString(kind), lineNum, lexeme);
	return kind;
}

char* toString(int tokenType) { //return the token's string

	switch (tokenType) {
	case TOKEN_INT_NUMBER:	return "'{INT_NUMBER}'";
	case TOKEN_FLOAT_NUMBER: return "'{FLOAT_NUMBER}'";
	case TOKEN_AR_OP:return "'{AR_OP}'";
	case TOKEN_REL_OP:return "'{REL_OP}'";
	case TOKEN_ASSIGNMENT_OP:return "'{ASSIGNMENT_OP}'";
	case TOKEN_MULTIPLICATION_OP:return "'{MULTIPLICATION_OP}'";
	case TOKEN_DIVISION_OP:return "'{DIVISION_OP}'";
	case TOKEN_ID:return "'{ID}'";
	case TOKEN_KEYWORDS:return "'{KEYWORDS}'";
	case TOKEN_COMMA:return "'{COMMA}'";
	case TOKEN_COLON:return "'{COLON}'";
	case TOKEN_SEMICOLON:return "'{SEMICOLON}'";
	case TOKEN_L_CURLY:return "'{BRACES_LEFT}'";
	case TOKEN_R_CURLY:return "'{BRACES_RIGHT}'";
	case TOKEN_L_BRACKET:return "'{BRACKETS_LEFT}'";
	case TOKEN_R_BRACKET:return "'{BRACKETS_RIGHT}'";
	case TOKEN_L_PARENTHESIS:return "'{PARENTHESIS_LEFT}'";
	case TOKEN_R_PARENTHESIS:return "'{PARENTHESIS_RIGHT}'";
	case TOKEN_L_COMMENTS:return "'{COMMENT_LEFT}'";
	case TOKEN_R_COMMENTS:return "'{COMMENT_RIGHT}'";
	case TOKEN_INT:return "'{KEYWORDS_INT}'";
	case TOKEN_FLOAT:return "'{KEYWORDS_FLOAT}'";
	case TOKEN_VOID:return "'{KEYWORDS_VOID}'";
	case TOKEN_RETURN:return "'{KEYWORDS_RETURN}'";
	case TOKEN_IF:return "'{KEYWORDS_IF}'";
	case TOKEN_WHILE:return "'{KEYWORDS_WHILE}'";
	case TOKEN_OP_EQUAL:return "'{KEYWORDS_FLOAT}'";
	case TOKEN_EOF:return "'{KEYWORDS_FLOAT}'";

	default:
		return "ERROR!";
	}
}

void init() 
{
	currentIndex = 0;
	currentNode = NULL;
}