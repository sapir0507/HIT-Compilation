#include "parser.h"
#include "semantic.h"
//#include "main.c"
struct ST* ST;
Token* curr_token;
Token* curr_token_id_of_func = NULL;


void Parse() 
{
	parse_PROG();
	match(TOKEN_EOF);
}

void parse_PROG()
{
	Token* help;
	// semantic 
	// global table - contain scope of global, main and function
	ST = (struct ST*)malloc(sizeof(struct ST));
	ST->symbolListHead = NULL;
	ST->parentSymbolTable = NULL;
	ST->Chead = NULL;

//	Token* curr_token = next_token();
	curr_token = next_token();
	// first program
	switch (curr_token->tokenType)
	{
	case TOKEN_FLOAT:
	case TOKEN_INT:
	case TOKEN_VOID:
	{
		//semantic 
		// main scope insert 
		ST = enter_scope(ST);

		fprintf(yy_parser, "Rule (PROG -> GLOBAL_VARS; FUNCTION_PRE_DEFS end FUNC_FULL_DEFS) \n");

		curr_token = next_token();
		parse_GLOBAL_VAR_DEFS();
		parse_FUNC_PRE_DEFS();
		help = curr_token;
		while (help->tokenType != TOKEN_EOF)
		{
			parse_FUNC_FULL_DEFINITION();
			help = next_token();
			curr_token = back_token();
		}
		fprintf(yy_semantic, "The semantic check has ended.\n");

		ST = exit_scope(ST);

	}break;

	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_INT, TOKEN_FLOAT OR VOID' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);
		while (curr_token->tokenType != TOKEN_EOF)
		{
			curr_token = next_token();
		}
		back_token();
	}

	}

}

int parse_GLOBAL_VAR_DEFS()
{
	curr_token = back_token();

	switch (curr_token->tokenType)
	{

	case TOKEN_FLOAT:
	case TOKEN_INT:
	{
		// semantic 
		int num_of_VAR_DEFINITION, num_of_VAR_DEFINITIONS_t;

		fprintf(yy_parser, "Rule (VAR_DEFINITIONS -> VAR_DEFINITION VAR_DEFINITIONS_t) \n");
		//	curr_token  = back_token();
		num_of_VAR_DEFINITION = parse_VAR_DEFINITION(TO_DEFINE);  //the number of variable we used (global)
		num_of_VAR_DEFINITIONS_t = parse_VAR_DEFINITIONS_t();
//		curr_token = next_token();
		//semantic
		return num_of_VAR_DEFINITION + num_of_VAR_DEFINITIONS_t;

	}break;

	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_FLOAT' , 'TOKEN_INT' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);
		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_SEMICOLON && curr_token->tokenType != TOKEN_R_PARENTHESIS)
		{
			curr_token = next_token();
		}
		curr_token = back_token();

		return -1;
	}
	}
}

int parse_VAR_DEFINITIONS(enum action act)
{
	curr_token = back_token();

	switch (curr_token->tokenType)
	{

	case TOKEN_FLOAT:
	case TOKEN_INT:
	{
		// semantic 
		int num_of_VAR_DEFINITION, num_of_VAR_DEFINITIONS_t;

		fprintf(yy_parser, "Rule (VAR_DEFINITIONS -> VAR_DEFINITION VAR_DEFINITIONS_t) \n");
	//	curr_token  = back_token();
		num_of_VAR_DEFINITION = parse_VAR_DEFINITION(act);  //the number of variable we used (global)
		num_of_VAR_DEFINITIONS_t = parse_VAR_DEFINITIONS_t();
		
		//semantic
		return num_of_VAR_DEFINITION + num_of_VAR_DEFINITIONS_t;

	}break;

	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_FLOAT' , 'TOKEN_INT' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);
		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_SEMICOLON && curr_token->tokenType != TOKEN_R_PARENTHESIS)
		{
			curr_token = next_token();
		}
		curr_token = back_token();

		return -1;
	}
	}
}

int parse_VAR_DEFINITIONS_t()
{
	Token* curr_token = next_token();

	switch (curr_token->tokenType)
	{

	case TOKEN_SEMICOLON:
	{
		curr_token = next_token();
		//back_token();
		if (curr_token->tokenType == TOKEN_FLOAT || curr_token->tokenType == TOKEN_INT)
		{
			curr_token = next_token(); //id
			curr_token = next_token(); //(|[|;|,
			if (curr_token->tokenType != TOKEN_L_PARENTHESIS)
			{
				curr_token = back_token();
				curr_token = back_token();
				fprintf(yy_parser, "Rule (VAR_DEFINITIONS_t -> ; VAR_DEFINITIONS) \n");
				//curr_token = back_token(); ////
				curr_token = next_token();
				return parse_VAR_DEFINITIONS(TO_DEFINE);
			} 
			else
			{
				curr_token = back_token();
				curr_token = back_token();
				curr_token = back_token(); 
				return 0;  ///27.7.20
			}
		}
		else if(curr_token->tokenType == TOKEN_VOID)
		{
			fprintf(yy_parser, "Rule (VAR_DEFINITIONS_t -> EPSILON) \n");
			curr_token = back_token();
			//semantic 
			return 0;
		}
		else
		{
			fprintf(yy_parser, "Rule (VAR_DEFINITIONS_t -> EPSILON) \n");
			curr_token = back_token();
			curr_token = back_token(); //added 27.7.20
			//semantic 
			return 0;
		}
	}break;

	case TOKEN_R_PARENTHESIS:
	{
		fprintf(yy_parser, "Rule (VAR_DEFINITIONS_t -> EPSILON) \n");
		back_token();

		//semantic 
		return 0;
	}break;

	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_SEMICOLON' , 'TOKEN_R_PARENTHESIS' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);
		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_SEMICOLON && curr_token->tokenType != TOKEN_R_PARENTHESIS)
		{
			curr_token = next_token();
		}
		back_token();

		//semantic todo
		return -1;
	}
	}
}

int parse_VAR_DEFINITION(enum action act)
{
	//Token* curr_token = next_token();
	switch (curr_token->tokenType)
	{
	case TOKEN_FLOAT:
	case TOKEN_INT:
	{
		fprintf(yy_parser, "Rule (VAR_DEFINITION -> TYPE VARIABLES_LIST) \n");

		curr_token = back_token();
		enum type type = parse_TYPE();
		return parse_VARIABLES_LIST(act, type);
	}break;

	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_FLOAT' , 'TOKEN_INT' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_SEMICOLON && curr_token->tokenType != TOKEN_R_PARENTHESIS)
		{
			curr_token = next_token();
		}
		back_token();

		return -1;
	}
	}
}

enum type parse_TYPE()
{
	static int first = 0;
	if (first)
		curr_token = next_token();
	else first = 1;
	//curr_token = next_token();
	switch (curr_token->tokenType)
	{
	case TOKEN_FLOAT:
	{
		fprintf(yy_parser, "Rule (TYPE -> float) \n");

		//semantic
		return FLOAT;
	}break;
	case TOKEN_INT:
	{
		fprintf(yy_parser, "Rule (TYPE -> int) \n");

		//semantic
		return INTEGER;
	}break;
	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_FLOAT' , 'TOKEN_INT' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_ID)
		{
			curr_token = next_token();
		}
		curr_token = back_token();

		//semantic 
		return ERROR;
	}
	}
}

int parse_VARIABLES_LIST(enum action action, enum type type)
{
	curr_token = next_token(); //int id; |int id,id,id,...,id;| int id[int_number]; | int id, id[...],...| int id[..], id;
	switch (curr_token->tokenType)
	{
	case TOKEN_ID:
	{
		//semantic 
		int num_of_VARIABLES_LIST_t, size_of_var;

		fprintf(yy_parser, "Rule (VARIABLES_LIST -> VARIABLE VARIABLES_LIST_t) \n");
		curr_token = back_token();
		size_of_var = parse_VARIABLE(action, type);
		num_of_VARIABLES_LIST_t = parse_VARIABLES_LIST_t(action, type);

		//semantic
		return size_of_var + num_of_VARIABLES_LIST_t + 1;
	}break;
	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_ID' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_SEMICOLON && curr_token->tokenType != TOKEN_R_PARENTHESIS)
		{
			curr_token = next_token();
		}
		curr_token = back_token();

		//semantic todo
		return -1;
	}
	}
}

int parse_VARIABLES_LIST_t(enum action action, enum type type)
{
	curr_token = next_token();
	switch (curr_token->tokenType)
	{
	case TOKEN_COMMA:
	{
		//semantic
		int num_of_VARIABLES_LIST_t;

		fprintf(yy_parser, "Rule (VARIABLES_LIST_t -> ,VARIABLE VARIABLES_LIST_t) \n");
		parse_VARIABLE(action, type);
		num_of_VARIABLES_LIST_t = parse_VARIABLES_LIST_t(action, type);

		//semantic
		return num_of_VARIABLES_LIST_t + 1;
	}break;

	//follow
	case TOKEN_SEMICOLON:
	{
		fprintf(yy_parser, "Rule (VARIABLES_LIST_t -> EPSILON ) \n");
		curr_token = back_token();

		return 0; //changed from 0 to 1 on 27.7.20
	}break;
	case TOKEN_R_PARENTHESIS:
	{
		fprintf(yy_parser, "Rule (VARIABLES_LIST_t -> EPSILON ) \n");
		curr_token = back_token();

		return 1; //changed from 0 to 1 on 27.7.20
	}break;

	case TOKEN_L_BRACKET:
	{
		curr_token = next_token();
		if (curr_token->tokenType == TOKEN_INT_NUMBER)
		{
			curr_token = next_token();
			if (curr_token->tokenType == TOKEN_R_BRACKET)
			{
				fprintf(yy_parser, "Rule (VARIABLES_LIST_t [12] -> EPSILON ) \n");
			}
			else
			{
				fprintf(yy_parser, "Expected: one of tokens 'TOKEN_R_BRACKET' at line %d ", curr_token->lineNumber);
				fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);
			}
		}
		else
		{
			fprintf(yy_parser, "Expected: one of tokens 'TOKEN_INT_NUMBER' at line %d ", curr_token->lineNumber);
			fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);
		}

	}

	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_COMMA' , 'TOKEN_SEMICOLON' , 'TOKEN_R_PARENTHESIS' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_SEMICOLON && curr_token->tokenType != TOKEN_R_PARENTHESIS)
		{
			curr_token = next_token();
		}
		curr_token = back_token();

		//semantic todo
		return -1;
	}
	}
}

int parse_VARIABLE(enum action action, enum type type)
{
	curr_token = next_token();

	//semantic
	int size_of_var = 0;
	int inserted;

	switch (curr_token->tokenType)
	{
	case TOKEN_ID:
	{
		Token* holder = curr_token;
		fprintf(yy_parser, "Rule (VARIABLE -> id VARIABLE_t) \n");
		size_of_var = parse_VARIABLE_t(); //should return or not?

		if (action == TO_USE)
		{
			symbol* entry_of_id = lookup(ST, holder->lexeme);
			if (entry_of_id == NULL)
			{
				fprintf(yy_semantic, "ERROR at line: %d the variable with lexme: %s is not define \n", holder->lineNumber, holder->lexeme);
			}
		}

		else // want to define var with name curr_token->lexeme
		{
			// semantic    
			symbol* entry_of_id = lookup(ST, holder->lexeme);
			if (entry_of_id == NULL)
			{
				inserted = insert(ST, holder->lexeme, type, size_of_var, variable);
				if (!inserted)
				{
					fprintf(yy_semantic, "ERROR at line: %d the variable with lexme: %s is already define\n", holder->lineNumber, holder->lexeme);
				}
			}
			else
			{
				if (entry_of_id->kind == function)
					fprintf(yy_semantic, "ERROR at line: %d the function with lexme: %s is already define\n", holder->lineNumber, holder->lexeme);
				else if (entry_of_id->kind == variable)
					fprintf(yy_semantic, "ERROR at line: %d the variable with lexme: %s is already define\n", holder->lineNumber, holder->lexeme);
				else if (entry_of_id->kind == parameter)
					fprintf(yy_semantic, "ERROR at line: %d the parameter with lexme: %s is already define\n", holder->lineNumber, holder->lexeme); 
			}

		}
		//printf("lexme:%s type:%d  size:%d \n", curr_token->lexeme, type,size_of_var);
		return size_of_var;
	}break;
	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_ID' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_SEMICOLON && curr_token->tokenType != TOKEN_R_PARENTHESIS && curr_token->tokenType != TOKEN_COMMA)
		{
			curr_token = next_token();
		}
		curr_token = back_token();
		return 0;
	}
	}
}

int parse_VARIABLE_t()
{
	 curr_token = next_token();
	Token* token_for_size = NULL;

	switch (curr_token->tokenType)
	{
	case TOKEN_R_BRACKET:
	{
		fprintf(yy_parser, "Rule (VARIABLE_t -> [int_number] ) \n");

		//semantic
		curr_token = next_token();
		token_for_size = curr_token;

		//needs to add here
		symbol* entry_of_function = lookup(ST, token_for_size->lexeme);
		if (curr_token->tokenType != TOKEN_INT_NUMBER)
		{
			if (entry_of_function != NULL && entry_of_function->type != TOKEN_INT_NUMBER)
				fprintf(yy_semantic, "ERROR at line: %d the variable with lexme: %s isn't a decimal number\n", curr_token->lineNumber, curr_token->lexeme);
		}
		///////end semantic////////
		//curr_token = next_token();
		match(TOKEN_L_BRACKET);

		// semantic
		return atoi(token_for_size->lexeme);
	}break;

	//follow
	case TOKEN_COMMA:
	case TOKEN_SEMICOLON:
	case TOKEN_ASSIGNMENT_OP:

	case TOKEN_L_PARENTHESIS:
	{
		fprintf(yy_parser, "Rule (VARIABLE_t -> EPSILON ) \n");
		curr_token = back_token();

		//semantic
		return -1;
	}break;	
	case TOKEN_R_PARENTHESIS:
	{
		fprintf(yy_parser, "Rule (VARIABLE_t -> EPSILON ) \n");
		curr_token = back_token();

		//semantic
		return -1;
	}break;
	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_L_BRACKET' , 'TOKEN_COMMA' , 'TOKEN_SEMICOLON' , 'TOKEN_ASSIGNMENT_OP' , 'TOKEN_R_PARENTHESIS' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_SEMICOLON && curr_token->tokenType != TOKEN_R_PARENTHESIS && curr_token->tokenType != TOKEN_COMMA && curr_token->tokenType != TOKEN_ASSIGNMENT_OP)
		{
			curr_token = next_token();
		}
		curr_token = back_token();

		//semantic todo
		return -2;
	}
	}
}

void parse_FUNC_PRE_DEFS() 
{
	int isFuncDecaration = 0;
	curr_token = next_token();
	switch (curr_token->tokenType)
	{
	case TOKEN_VOID:
	case TOKEN_FLOAT:
	case TOKEN_INT:
	{
		fprintf(yy_parser, "Rule (FUNC_PRE_DEFS -> FUNC_DEFINITION FUNC_DEFINITIONS_t) \n");
		curr_token = back_token();
		//todo
		isFuncDecaration = IsDeclration();
		if (isFuncDecaration)
		{
			parse_FUNC_DEFINITION();
			parse_FUNC_DEFINITIONS_t();
		}
	}break;

	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_VOID' , 'TOKEN_FLOAT' , 'TOKEN_INT' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}

void parse_FUNC_DEFINITIONS_t()
{
	 int isFuncDecaration = 0;
	 curr_token = next_token();
	switch (curr_token->tokenType)
	{
		// var is nullable
		// first cases:
	case TOKEN_VOID:
	case TOKEN_FLOAT:
	case TOKEN_INT:
	{
		fprintf(yy_parser, "Rule (FUNC_DEFINITIONS_t -> FUNC_DEFINITION FUNC_DEFINITIONS_t) \n");
		curr_token = back_token();
		//todo DONE. FUNCTION PRE-DEF AND FUNC FULL DEF HANDLED SEPERATELY.
		isFuncDecaration = IsDeclration();
		if (isFuncDecaration)
		{
			parse_FUNC_DEFINITION();
			parse_FUNC_DEFINITIONS_t();
		}
	}break;

	//follow cases:
	case TOKEN_EOF:
	{
		fprintf(yy_parser, "Rule (FUNC_DEFINITIONS_t -> EPSILON ) \n");
		curr_token = back_token();
	}break;

	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_VOID' , 'TOKEN_FLOAT' , 'TOKEN_INT' , 'TOKEN_EOF' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF)
		{
			curr_token = next_token();
		}
		curr_token = back_token();
	}
	}
}

int  IsDeclration()
{
	int forwhile = 1, isdec = 1, counter = 0, i;
	curr_token = next_token();
	
	while (forwhile)
	{
		if (curr_token->tokenType == TOKEN_SEMICOLON)
			forwhile = 0;
		else if (curr_token->tokenType == TOKEN_L_CURLY)
			forwhile = 0;
		curr_token = next_token();
		counter++; //counts all the times we nexted the token
	}

	curr_token = back_token();
	//counter--;

	if (curr_token->tokenType == TOKEN_SEMICOLON)
		isdec = 1;
	else isdec = 0;

	for (i = 0; i < counter; i++)
		curr_token = back_token();

	return isdec;
}

void parse_FUNC_DEFINITION()
{
	 curr_token = next_token();
	switch (curr_token->tokenType)
	{
	case TOKEN_VOID:
	case TOKEN_FLOAT:
	case TOKEN_INT:
	{
		//semantic 
		Type type_of_RETURNED_TYPE;
		int num_of_PARAM_DEFINITIONS;
		int inserted;


		fprintf(yy_parser, "Rule (FUNC_DEFINITION -> RETURNED_TYPE id (PARAM_DEFINITIONS)) \n");

		curr_token = back_token();
		type_of_RETURNED_TYPE = parse_RETURNED_TYPE();

		// semantic
		curr_token = next_token();
		Token* token_of_id = curr_token;
		curr_token = back_token();

		match(TOKEN_ID);
		match(TOKEN_L_PARENTHESIS);

		//semantic
		inserted = insert(ST, token_of_id->lexeme, type_of_RETURNED_TYPE, -1, function);
		if (!inserted)
		{
			fprintf(yy_semantic, "ERROR at line: %d the function with lexme: %s already define\n", token_of_id->lineNumber, token_of_id->lexeme);
		}

		// semantic - do this here becous i want the names of parametrs will be in list symbol of this function
		ST = enter_scope(ST);
		curr_token = next_token();
		//curr_token = back_token();
		num_of_PARAM_DEFINITIONS = parse_PARAM_DEFINITIONS();

		match(TOKEN_R_PARENTHESIS);
		match(TOKEN_SEMICOLON);

		//semantic
		//ST = exit_scope(ST); //27.7.20

		//semantic - update the real num of parameters of function
		if (inserted)
		{
			symbol* entry_of_function = lookup(ST, token_of_id->lexeme);
			entry_of_function->size = num_of_PARAM_DEFINITIONS;
		}

	}break;

	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_VOID' , 'TOKEN_FLOAT' , 'TOKEN_INT' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_VOID && curr_token->tokenType != TOKEN_FLOAT && curr_token->tokenType != TOKEN_INT)
		{
			curr_token = next_token();
		}
		curr_token = back_token();
	}
	}
}

Type parse_RETURNED_TYPE()
{
	 curr_token = next_token();
	switch (curr_token->tokenType)
	{
	case TOKEN_VOID:
	{
		fprintf(yy_parser, "Rule (RETURNED_TYPE -> void ) \n");

		//semantic
		return VOID;
	}break;
	case TOKEN_FLOAT:
	case TOKEN_INT:
	{
		fprintf(yy_parser, "Rule (RETURNED_TYPE -> TYPE ) \n");

		curr_token = back_token();
		Type type_of_TYPE = parse_TYPE();

		//semantic
		return type_of_TYPE;
	}break;

	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_VOID' , 'TOKEN_FLOAT' , 'TOKEN_INT' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_ID)
		{
			curr_token = next_token();
		}
		curr_token = back_token();

		//semantic todo
		return ERROR;
	}
	}
}

int parse_PARAM_DEFINITIONS()
{
	// curr_token = next_token();
	switch (curr_token->tokenType)
	{
		// first
	case TOKEN_FLOAT:
	case TOKEN_INT:
	{
		fprintf(yy_parser, "Rule (PARAM_DEFINITIONS -> VAR_DEFINITIONS ) \n");

		curr_token = next_token(); //changed 27.7.20 
		return parse_VAR_DEFINITIONS(TO_DEFINE); //? SHOULD I ADD enum action TO PARAM_DEFINITIONS?
	}break;

	// follow
	case TOKEN_R_PARENTHESIS:
	{
		fprintf(yy_parser, "Rule (PARAM_DEFINITIONS -> EPSILON ) \n");

		curr_token = back_token();

		// semantic 
		return 0;
	}break;

	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_FLOAT' , 'TOKEN_INT' , 'TOKEN_R_PARENTHESIS' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_R_PARENTHESIS)
		{
			curr_token = next_token();
		}
		curr_token = back_token();

		// semantic todo
		return -1;
	}
	}
}

void parse_FUNC_FULL_DEFS()
{

	curr_token = next_token();
	switch (curr_token->tokenType)
	{
	case TOKEN_VOID:
	case TOKEN_FLOAT:
	case TOKEN_INT:
	{
		fprintf(yy_parser, "Rule (FUNC_FULL_DEFS -> FUNC_FULL_DEFINITION FUNC_FULL_DEFINITIONS_t) \n");
		curr_token = back_token();
		//todo
		parse_FUNC_FULL_DEFINITION();
		parse_FUNC_FULL_DEFINITIONS_t();
	}break;

	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_VOID' , 'TOKEN_FLOAT' , 'TOKEN_INT' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF)
		{
			curr_token = next_token();
		}
		back_token();
	}
	}
}

void parse_FUNC_FULL_DEFINITIONS_t()
{
	curr_token = next_token();
	switch (curr_token->tokenType)
	{
		// var is nullable
		// first cases:
	case TOKEN_VOID:
	case TOKEN_FLOAT:
	case TOKEN_INT:
	{
		fprintf(yy_parser, "Rule (FUNC_FULL_DEFINITIONS_t -> FUNC_FULL_DEFINITION FUNC_FULL_DEFINITIONS_t) \n");
		curr_token = back_token();
		
		parse_FUNC_FULL_DEFINITION();
		parse_FUNC_FULL_DEFINITIONS_t();
	}break;

	//follow cases:
	case TOKEN_EOF:
	{
		fprintf(yy_parser, "Rule (FUNC_FULL_DEFINITIONS_t -> EPSILON ) \n");
		curr_token = back_token();
	}break;

	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_VOID' , 'TOKEN_FLOAT' , 'TOKEN_INT' , 'TOKEN_EOF' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF)
		{
			curr_token = next_token();
		}
		curr_token = back_token();
	}
	}
}

void parse_FUNC_FULL_DEFINITION()
{
	curr_token = next_token();
	switch (curr_token->tokenType)
	{
	case TOKEN_VOID:
	case TOKEN_FLOAT:
	case TOKEN_INT:
	{
		//semantic 
		Type type_of_RETURNED_TYPE;
		int num_of_PARAM_DEFINITIONS;
		int inserted;


		fprintf(yy_parser, "Rule (FUNC_FULL_DEFINITION -> RETURNED_TYPE id (PARAM_FULL_DEFINITIONS) BLOCK ) \n");
		ST = enter_scope(ST);
		curr_token = back_token();
		type_of_RETURNED_TYPE = parse_RETURNED_TYPE();
		// semantic
		Token* token_of_id = next_token();
		curr_token_id_of_func = token_of_id;
		curr_token = back_token();

		match(TOKEN_ID);
		match(TOKEN_L_PARENTHESIS);

		//semantic
		symbol* is_function_declared = lookup(ST, token_of_id->lexeme);
	//	inserted = insert(ST, token_of_id->lexeme, type_of_RETURNED_TYPE, -1, function);
		if (is_function_declared==NULL)
		{
			fprintf(yy_semantic, "ERROR at line: %d the function with lexme: %s isn't defined\n", token_of_id->lineNumber, token_of_id->lexeme);
		}

		// semantic - do this here becous i want the names of parametrs will be in list symbol of this function
		ST = enter_scope(ST);

		num_of_PARAM_DEFINITIONS = parse_PARAM_FULL_DEFINITIONS(TO_USE);

		match(TOKEN_R_PARENTHESIS);
		parse_BLOCK();

		//semantic
		ST = exit_scope(ST);
		ST = exit_scope(ST);
		ST = exit_scope(ST);

	}break;

	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_VOID' , 'TOKEN_FLOAT' , 'TOKEN_INT' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_VOID && curr_token->tokenType != TOKEN_FLOAT && curr_token->tokenType != TOKEN_INT)
		{
			curr_token = next_token();
		}
		curr_token = back_token();
	}
	}
}

int parse_PARAM_FULL_DEFINITIONS(enum action act)
{
	curr_token = next_token();
	switch (curr_token->tokenType)
	{
		// first
	case TOKEN_FLOAT:
	case TOKEN_INT:
	{
		fprintf(yy_parser, "Rule (PARAM_DEFINITIONS -> VAR_DEFINITIONS ) \n");
		curr_token = next_token();
		return parse_VAR_DEFINITIONS(act);
	}break;

	// follow
	case TOKEN_R_PARENTHESIS:
	{
		fprintf(yy_parser, "Rule (PARAM_FULL_DEFINITIONS -> Epsilon ) \n");

		curr_token = back_token();

		// semantic 
		return 0;
	}break;

	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_FLOAT' , 'TOKEN_INT' , 'TOKEN_R_PARENTHESIS' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_R_PARENTHESIS)
		{
			curr_token = next_token();
		}
		curr_token = back_token();

		// semantic todo
		return -1;
	}
	}
}

void parse_STATEMENTS()
{
	 curr_token = next_token();
	switch (curr_token->tokenType)
	{
	case TOKEN_RETURN:
	case TOKEN_L_CURLY:
	case TOKEN_ID:
	{
		fprintf(yy_parser, "Rule (STATEMENTS -> STATEMENT;STATEMENTS_t ) \n");

		curr_token = back_token();
		parse_STATEMENT();
		curr_token = next_token();
		if (curr_token->tokenType != TOKEN_R_CURLY)
		{
			curr_token = back_token();
			if (curr_token->tokenType != TOKEN_R_CURLY)
				match(TOKEN_SEMICOLON);
		}
		else curr_token = back_token();
		parse_STATEMENTS_t();
	}break;


	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_RETURN' , 'TOKEN_L_CURLY' , 'TOKEN_ID' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_R_CURLY)
		{
			curr_token = next_token();
		}
		curr_token = back_token();
	}
	}
}

void parse_STATEMENTS_t()
{
	 curr_token = next_token();
	switch (curr_token->tokenType)
	{
	case TOKEN_RETURN:
	case TOKEN_L_CURLY:
	case TOKEN_ID:
	{
		fprintf(yy_parser, "Rule (STATEMENTS_t -> STATEMENTS ) \n");

		curr_token = back_token();
		parse_STATEMENTS();
	}break;

	case TOKEN_EOF:
	case TOKEN_R_CURLY:
	{
		fprintf(yy_parser, "Rule (STATEMENTS_t -> EPSILON ) \n");

		curr_token = back_token();
	}break;


	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_RETURN' , 'TOKEN_L_CURLY' , 'TOKEN_ID' , 'TOKEN_EOF' , 'TOKEN_R_CURLY' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_R_CURLY)
		{
			curr_token = next_token();
		}
		curr_token = back_token();
	}
	}
}

void parse_STATEMENT()
{
	 curr_token = next_token();
	switch (curr_token->tokenType)
	{
	case TOKEN_RETURN:
	{
		fprintf(yy_parser, "Rule (STATEMENT -> return STATEMENT_t ) \n");
		symbol* entry_of_id = lookup(ST, curr_token_id_of_func->lexeme); //problem looking for x
		parse_STATEMENT_t();
		if(curr_token->tokenType==TOKEN_INT_NUMBER && entry_of_id->type!= INTEGER)
		{
			fprintf(yy_semantic, "ERROR at line: %d the return value doesn't match with the function's return value \n", curr_token->lineNumber);
		}
		else if (curr_token->tokenType == TOKEN_FLOAT_NUMBER && entry_of_id->type != FLOAT)
		{
			fprintf(yy_semantic, "ERROR at line: %d the return value doesn't match with the function's return value \n", curr_token->lineNumber);
		}
		else if(curr_token->tokenType == TOKEN_ID)
		{
			symbol* type_of_id = lookup(ST, curr_token_id_of_func->lexeme); //problem looking for x
			if (type_of_id->type != entry_of_id->type)
			{
				fprintf(yy_semantic, "ERROR at line: %d the return value doesn't match with the function's return value \n", curr_token->lineNumber);
			}
		}

	}break;
	//todo methodsCall(id) -> DONE. IN STATEMENT T2, THERE IS A CHECK FOR METHOD CALL
	case TOKEN_ID:
	{
		fprintf(yy_parser, "Rule (STATEMENT -> id STATEMENT_t2 ) \n");

		// semantic
		symbol* entry_of_id = lookup(ST, curr_token->lexeme); //problem looking for x
		if (entry_of_id == NULL)
		{
			if (entry_of_id->kind == function)
				fprintf(yy_semantic, "ERROR at line: %d the function with lexme: %s not define \n", curr_token->lineNumber, curr_token->lexeme);
			else if (entry_of_id->kind == variable)
				fprintf(yy_semantic, "ERROR at line: %d the variable with lexme: %s not define \n", curr_token->lineNumber, curr_token->lexeme);
		}
		parse_STATEMENT_t2(entry_of_id);
	}break;

	case TOKEN_L_CURLY:
	{
		fprintf(yy_parser, "Rule (STATEMENT -> BLOCK ) \n");

		curr_token = back_token();

		// semantic 
		ST = enter_scope(ST);

		parse_BLOCK();

		// semantic 
		ST = exit_scope(ST);

	}break;

	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_RETURN' , 'TOKEN_ID' , 'TOKEN_L_CURLY' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_SEMICOLON)
		{
			curr_token = next_token();
		}
		curr_token = back_token();
	}
	}
}

void parse_STATEMENT_t()
{
	 curr_token = next_token();
	switch (curr_token->tokenType)
	{
	case TOKEN_ID:
	case TOKEN_INT_NUMBER:
	case TOKEN_FLOAT_NUMBER:
	{
		fprintf(yy_parser, "Rule (STATEMENT_t -> EXPRESSION ) \n");
		curr_token = back_token();
		parse_EXPRESSION();
	}break;

	case TOKEN_SEMICOLON:
	{
		fprintf(yy_parser, "Rule (STATEMENT_t -> EPSILON ) \n");

		curr_token = back_token();
	}break;

	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_ID' , 'TOKEN_INT_NUBMER' , 'TOKEN_REAL_NUMBER' , 'TOKEN_SEMICOLON' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_SEMICOLON)
		{
			curr_token = next_token();
		}
		curr_token = back_token();
	}
	}
}

void parse_STATEMENT_t2(symbol* entry_of_id)
{
	 curr_token = next_token();
	switch (curr_token->tokenType)
	{
	//case TOKEN_L_BRACKET:
	case TOKEN_R_BRACKET: //changed from token_l_bracket to token_r_bracket on 27.7.20
	case TOKEN_ASSIGNMENT_OP:
	{
		fprintf(yy_parser, "Rule (STATEMENT_t2 -> VARIABLE_t = EXPRESSION ) \n");

		// semantic
		int size_of_VARIABLE_t;

		curr_token = back_token();
		size_of_VARIABLE_t = parse_VARIABLE_t();

		//semantic 
		if (entry_of_id != NULL)
		{
			int size_of_id = entry_of_id->size;
			symbol* look = lookup(ST, curr_token->lexeme);
			//semantic 
			if (look !=NULL && look->kind == function)
			{
				fprintf(yy_semantic, "ERROR at line: %d - you're using a function name like a variable \n", curr_token->lineNumber);
			}
			else
			{
				if (size_of_id > -1 && size_of_VARIABLE_t == -1)
				{
					// role-7 and 5
					fprintf(yy_semantic, "ERROR at line: %d - you're using an array like a variable \n", curr_token->lineNumber);
					//return ERROR;
				}
				else if (size_of_id == -1 && size_of_VARIABLE_t != -1)
				{
					// role-8
					fprintf(yy_semantic, "ERROR at line: %d -you're using a variable like an array \n", curr_token->lineNumber);
				}
				else if (size_of_id > -1 && size_of_VARIABLE_t > size_of_id)
				{
					// role-9
					fprintf(yy_semantic, "ERROR at line: %d - out of bound array \n", curr_token->lineNumber);
				}
			}
		}


		match(TOKEN_ASSIGNMENT_OP);
		Type type_of_EXPRESSION = parse_EXPRESSION();

		//semantic
		if (entry_of_id != NULL && type_of_EXPRESSION != ERROR)
		{
			if (entry_of_id->type != type_of_EXPRESSION)
			{
				fprintf(yy_semantic, "ERROR at line: %d - missmatch of types \n", curr_token->lineNumber);
			}
		}
	}break;

	case TOKEN_L_PARENTHESIS:
	{
		// semantic
		int num_of_PARAMETERS_LIST;

		fprintf(yy_parser, "Rule (STATEMENT_t2 -> ( PARAMETERS_LIST ) ) \n");

		num_of_PARAMETERS_LIST = parse_PARAMETERS_LIST();

		//semantic
		if (entry_of_id != NULL)
		{
			if (entry_of_id->size != num_of_PARAMETERS_LIST)
			{
				fprintf(yy_semantic, "ERROR at line: %d - missmatch number of parametrs \n", curr_token->lineNumber);
			}
		}

		match(TOKEN_R_PARENTHESIS);
	}break;

	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_L_BRACKET' , 'TOKEN_ASSIGNMENT_OP' , 'TOKEN_L_PARENTHESIS' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_SEMICOLON)
		{
			curr_token = next_token();
		}
		curr_token = back_token();
	}
	}
}


void parse_BLOCK()
{
	 curr_token = next_token();
	switch (curr_token->tokenType)
	{
	case TOKEN_L_CURLY:
	{
		fprintf(yy_parser, "Rule (BLOCK -> { VAR_DEFINITIONS;STATEMENTS } ) \n");
		ST = enter_scope(ST);
		curr_token = next_token();
		if (curr_token->tokenType != TOKEN_R_CURLY)
		{
			curr_token = next_token();
			parse_VAR_DEFINITIONS(TO_DEFINE); //??? 
			match(TOKEN_SEMICOLON);
			parse_STATEMENTS();
		}
		else curr_token = back_token();
		match(TOKEN_R_CURLY);
		ST = exit_scope(ST);

	}break;
	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_L_CURLY' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_VOID && curr_token->tokenType != TOKEN_FLOAT && curr_token->tokenType != TOKEN_INT && curr_token->tokenType != TOKEN_SEMICOLON)
		{
			curr_token = next_token();
		}
		curr_token = back_token();
	}
	}
}


int parse_PARAMETERS_LIST()
{
	 curr_token = next_token();
	switch (curr_token->tokenType)
	{
	case TOKEN_ID:
	{
		fprintf(yy_parser, "Rule (PARAMETERS_LIST -> VARIABLES_LIST ) \n");

		curr_token = back_token();
		return parse_VARIABLES_LIST(TO_USE, VOID); // semantic need to change this !-!-!
	}break;

	case TOKEN_R_PARENTHESIS:
	{
		fprintf(yy_parser, "Rule (PARAMETERS_LIST -> EPSILON ) \n");

		curr_token = back_token();

		//semantic 
		return 0;
	}break;

	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_ID' , 'TOKEN_R_PARENTHESIS' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_R_PARENTHESIS)
		{
			curr_token = next_token();
		}
		curr_token = back_token();
		//semantic todo
		return -1;
	}
	}
}

Type parse_EXPRESSION()
{
	 curr_token = next_token();
	switch (curr_token->tokenType)
	{
	case TOKEN_ID:
	{
		fprintf(yy_parser, "Rule (EXPRESSION -> id EXPRESSION_t ) \n");

		//semantic
		symbol* entry_of_id = lookup(ST, curr_token->lexeme);
		Type type_to_return;
		if (entry_of_id == NULL)
		{
			fprintf(yy_semantic, "ERROR at line: %d the variable with lexme: %s not define \n", curr_token->lineNumber, curr_token->lexeme);
			type_to_return = ERROR;
		}

		Type type_of_EXPRESSION_t = parse_EXPRESSION_t(entry_of_id);

		//semantic
		if (entry_of_id != NULL && type_of_EXPRESSION_t != ERROR)
		{
			if (entry_of_id->type == INTEGER && type_of_EXPRESSION_t == INTEGER)
			{
				type_to_return = INTEGER;
			}
			else
			{
				type_to_return = FLOAT;
			}
		}
		else
		{
			type_to_return = ERROR;
		}
		return type_to_return;
	}break;

	case TOKEN_FLOAT_NUMBER:
	{
		fprintf(yy_parser, "Rule (EXPRESSION -> float_number ) \n");
		parse_EXPRESSION_t(FLOAT); //added 28.7.20
		//semantic
		return FLOAT;
	}break;
	case TOKEN_INT_NUMBER:
	{
		fprintf(yy_parser, "Rule (EXPRESSION -> int_number ) \n");
		parse_EXPRESSION_t(INTEGER);//added 28.7.20
		//semantic
		return INTEGER;
	}break;

	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_ID' , 'TOKEN_FLOAT_NUMBER' , 'TOKEN_INT_NUBMER' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_SEMICOLON)
		{
			curr_token = next_token();
		}
		curr_token = back_token();
		return ERROR;
	}
	}
}

Type parse_EXPRESSION_t(symbol* entry_of_id)
{
	 curr_token = next_token();
	switch (curr_token->tokenType)
	{
	case TOKEN_L_BRACKET:
	{
		fprintf(yy_parser, "Rule (EXPRESSION_t -> VARIABLE_t ) \n");

		//semantic
		int size_of_VARIABLE_t;

		curr_token = back_token();
		size_of_VARIABLE_t = parse_VARIABLE_t();

		//semantic 
		if (entry_of_id != NULL)
		{
			int size_of_id = entry_of_id->size;

			//semantic 
			if (size_of_id > -1 && size_of_VARIABLE_t == -1)
			{
				// role-7 and 5
				fprintf(yy_semantic, "ERROR at line: %d - you're trying to use an array like val \n", curr_token->lineNumber);
				return ERROR;
			}
			else if (size_of_id == -1 && size_of_VARIABLE_t != -1)
			{
				// role-8
				fprintf(yy_semantic, "ERROR at line: %d - you're trying to use a val like an array \n", curr_token->lineNumber);
				return ERROR;
			}
			else if (size_of_id > -1 && size_of_VARIABLE_t > size_of_id)
			{
				// role-9
				fprintf(yy_semantic, "ERROR at line: %d - out of bound array \n", curr_token->lineNumber);
				return ERROR;
			}
			else
			{
				return entry_of_id->type;
			}
		}
		else
		{
			return ERROR;
		}
	}break;


	case TOKEN_MULTIPLICATION_OP:
	{
		fprintf(yy_parser, "Rule (EXPRESSION_t -> *EXPRESSION ) \n");

		return parse_EXPRESSION();
	}break;
	case TOKEN_AR_OP: //CHANGED now
	{
		fprintf(yy_parser, "Rule (EXPRESSION_t -> +EXPRESSION ) \n"); //CHANGED now

		return parse_EXPRESSION();
	}break;

	case TOKEN_SEMICOLON:
	{
		fprintf(yy_parser, "Rule (EXPRESSION_t -> EPSILON ) \n");

		curr_token = back_token();
		//semantic todo - need to check this id is not function
		if (entry_of_id != NULL)
		{
			if (entry_of_id->size == -1)
			{
				return entry_of_id->type;
			}
			else
			{
				// role-7 and 5
				fprintf(yy_semantic, "ERROR at line: %d -you're trying to use an array like val \n", curr_token->lineNumber);
				return ERROR;
			}
		}
		return ERROR;


	}break;

	default:
	{
		fprintf(yy_parser, "Expected: one of tokens 'TOKEN_L_BRACKET' , 'TOKEN_OP_MUL' , 'TOKEN_OP_DIV' , 'TOKEN_SEMICOLON' at line %d ", curr_token->lineNumber);
		fprintf(yy_parser, "Actual token: '%s' , lexeme “ %s ” \n", eTokenString[curr_token->tokenType], curr_token->lexeme);

		while (curr_token->tokenType != TOKEN_EOF && curr_token->tokenType != TOKEN_SEMICOLON)
		{
			curr_token = next_token();
		}
		curr_token = back_token();

		//semantic todo
		return ERROR;
	}
	}
}