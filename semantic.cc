#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "syntax.h"
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <ios>
#include <fstream>

using namespace std;

#define TRUE 1
#define FALSE 0

//----------------------------- token types ------------------------------
#define KEYWORDS 14
#define RESERVED 38
#define VAR 1
#define BEGIN 2
#define END 3
#define ASSIGN 4 
#define IF 5
#define WHILE 6 
#define DO 7
#define THEN 8
#define PRINT 9
#define INT 10
#define REAL 11
#define STRING 12
#define BOOLEAN 13
#define TYPE 14
#define PLUS 15
#define MINUS 16
#define DIV 17
#define MULT 18
#define EQUAL 19
#define COLON 20 
#define COMMA 21
#define SEMICOLON 22
#define LBRAC 23
#define RBRAC 24
#define LPAREN 25
#define RPAREN 26
#define NOTEQUAL 27
#define GREATER 28
#define LESS 29
#define LTEQ 30
#define GTEQ 31
#define DOT 32
#define ID 33
#define NUM 34
#define REALNUM 35
#define ERROR 36
#define LBRACE 37
#define RBRACE 38
#define NOOP 39


//-------------------- Data structures -----------------------------------------

//string
string temp_type_string;
string temp_typename;
string recent_id;
string leftop_id;
string rightop_id;
string selected_id;

//Integer
int temp_typeid;
int start_val;
int new_val;
int leftop_type;
int rightop_type;
int selected_type;


//List
list<string> typename_order_list;
list<string> typesec_typename_order_list;
list<string> varsec_typename_order_list;
list<string>  temp_id_list;
list<string>  typesec_typelist;
list<string>  typesec_idlist;
list<string>  varsec_typelist;
list<string> varsec_idlist;
list<string> :: iterator  sl_it; //sl_it = string_list_it
list<string> :: iterator sl_it_2;
list<string> :: iterator sl_it_3;
list<int> typesec_type_id_list;
list<int> varsec_type_id_list;
list<int> :: iterator il_it; //il_it = int_list_it
list<string>  old_id_list;

//Set

set<string> seen_typesec_typename_set;
set<string> seen_typesec_ids_set;
set<string> seen_varsec_typename_set;
set<string> seen_varsec_ids_set;
set<string> typesec_idset;
set<string> varsec_idset;
set<int> error_code_set;
set<int> :: iterator is_it; //is_it=int_set_it
set<string> repeating_typename_set; 

//Map
map<string, int>  typevalue_to_typeid_map;
map<string, list<string> >  typesec_typename_to_idlist_map;
map<string, list<string> > varsec_typename_to_idlist_map;
map<string, exprNode*> stmtlhs_to_stmtrhsnode_map;

//Map Iterator
map<string, list<string> > :: iterator slm_it; //slm_it = string_to_list_map_it
map<string, exprNode*> :: iterator sem_it; //sem_it = string_to_exprNode_map_it
map<string, int> :: iterator nid_it; //nid_it = typevalue_to_typeid_map


//------------------- reserved words and token strings -----------------------


char *reserved[] = 
	{	"",
		"VAR", 
		"BEGIN", 
		"END", 
		"ASSIGN", 
		"IF", 
		"WHILE", 
		"DO", 
		"THEN", 
		"print", 
		"INT",
		"REAL",
		"STRING",
		"BOOLEAN",
		"TYPE",
		"+",
		"-", 
		"/", 
		"*", 
		"=", 
		":", 
		",", 
		";", 
		"[", 
		"]", 
		"(", 
		")", 
		"<>", 
		">", 
		"<",
		"<=",
		">=",
		".",
		"ID",
		"NUM",
		"REALNUM",		
		"ERROR",
		"{",
		"}"
		};




int printToken(int ttype)
{
   if (ttype <= RESERVED)
   {   printf("%s\n",reserved[ttype]);
       return 1;
   } else
       return 0; 
}
//---------------------------------------------------------

//---------------------------------------------------------
// Global Variables associated with the next input token
#define MAX_TOKEN_LENGTH 100

char token[MAX_TOKEN_LENGTH];      // token string
int  ttype;                        // token type
int  activeToken = FALSE;                  
int  tokenLength;


int line_no = 1;

//----------------------------------------------------------
int skipSpace()
{
   char c;

   c = getchar(); 
   line_no += (c == '\n');
   while (!feof(stdin) && isspace(c))
   {    c = getchar(); 
        line_no += (c == '\n');
   }

   // return character to input buffer if eof is not reached
   if (!feof(stdin)) 
        ungetc(c,stdin);
}

int isKeyword(char *s)
{
     int i;

     for (i = 1; i <= KEYWORDS; i++)
	if (strcmp(reserved[i],s) == 0)
	   return i;
     return FALSE;
}

// ungetToken() simply sets a flag so that when getToken() is called
// the old ttype is returned and the old token is not overwritten 
// NOTE: BETWEEN ANY TWO SEPARATE CALLS TO ungetToken() THERE MUST BE
// AT LEAST ONE CALL TO getToken()
// CALLING TWO ungetToken() WILL NOT UNGET TWO TOKENS  
void ungetToken()
{
    activeToken = TRUE;
}


int scan_number()
{
	char c;
	
	c = getchar();
	if (isdigit(c))
	{	// First collect leading digits before dot
		// 0 is a nNUM by itself
		if (c == '0')	      
		{	token[tokenLength] = c;
			tokenLength++;
			token[tokenLength] = '\0';
		} else
		{	while (isdigit(c))
			{	token[tokenLength] = c;
				tokenLength++;;
				c = getchar();
			}
			ungetc(c,stdin);
			token[tokenLength] = '\0';
		}

		// Check if leading digits are integer part of a REALNUM
		c = getchar();
		if (c == '.')
		{	c = getchar();
			if (isdigit(c))
			{	token[tokenLength] = '.';
				tokenLength++;
				while (isdigit(c))
				{	token[tokenLength] = c;
					tokenLength++;
					c = getchar();
				}
				token[tokenLength] = '\0';
				if (!feof(stdin)) 
					ungetc(c,stdin);
				return REALNUM;
			} else
			{	ungetc(c, stdin);    // note that ungetc returns characters on a stack, so we first
				c = '.';             // return the second character and set c to '.' and return c again
				ungetc(c,stdin);				                                 
				return  NUM;         
                        }
		} else
		{	ungetc(c, stdin);
			return NUM;
		}
	} else
		return ERROR;   
}


int scan_id_or_keyword()
{
	int ttype;
	char c;

	c = getchar();
	if (isalpha(c))
	{	while (isalnum(c))
		{	token[tokenLength] = c;
			tokenLength++;
			c = getchar();
		}
		if (!feof(stdin)) 
			ungetc(c,stdin); 
             
		token[tokenLength] = '\0';		                
		ttype = isKeyword(token); 
		if (ttype == 0) 
			ttype = ID;
		return ttype;
	} else
		return ERROR;
}                            
   

int getToken()
{	char c;
 
       if (activeToken)
       { activeToken = FALSE;
         return ttype;
       }   // we do not need an else because the function returns in the body 
           // of the if
   
       skipSpace();   
       tokenLength = 0;
       c = getchar();
       switch (c)
       {   case '.': return DOT;
           case '+': return PLUS;
           case '-': return MINUS;
           case '/': return DIV;
           case '*': return MULT;
           case '=': return EQUAL;
           case ':': return COLON;
           case ',': return COMMA;
           case ';': return SEMICOLON;
           case '[': return LBRAC;
           case ']': return RBRAC;
           case '(': return LPAREN;
           case ')': return RPAREN;
           case '{': return LBRACE;
           case '}': return RBRACE;
           case '<':
               c = getchar();
               if (c == '=')
                   return LTEQ;
               else if (c == '>')
                   return NOTEQUAL;
               else
               {
                   ungetc(c,stdin);
                   return LESS;
               }
           case '>': 
               c = getchar();
               if (c == '=')
                   return GTEQ;
               else
               {
                   ungetc(c, stdin);
                   return GREATER;
               }
           default :
               if (isdigit(c))
               {	ungetc(c,stdin);
               		return scan_number();
               }
               else if (isalpha(c))
               {	ungetc(c,stdin);
               		return scan_id_or_keyword();
               }
               else if (c == EOF)
               		return EOF;
               else
               		return ERROR;
	}
}



/*----------------------------------------------------------------------------
SYNTAX ANALYSIS SECTION
----------------------------------------------------------------------------*/
#define PRIMARY 0
#define EXPR 1

/*-------------------------------UTILITIE---------------------------*/
void syntax_error(char* NT, int line_no)
{
	printf("Syntax error while parsing %s line %d\n", NT, line_no);
}

/*--------------------------------------------------------------------
  PRINTING PARSE TREE
---------------------------------------------------------------------*/
void print_parse_tree(struct programNode* program)
{
	//cout<< "Inside Print parse tree";	
	print_decl(program->decl); 
	print_body(program->body);
}

void print_decl(struct declNode* dec)
{
	if (dec->type_decl_section != NULL)
	{	
		print_type_decl_section(dec->type_decl_section);
	}
	if (dec->var_decl_section != NULL)
	{	
		print_var_decl_section(dec->var_decl_section);
	}
}

void print_body(struct bodyNode* body)
{
	printf("{\n");
	print_stmt_list(body->stmt_list); 
	printf("}\n");
	
}

void print_var_decl_section(struct var_decl_sectionNode* varDeclSection)
{
	printf("VAR\n");
	if (varDeclSection->var_decl_list != NULL)
		print_var_decl_list(varDeclSection->var_decl_list);
}

void print_var_decl_list(struct var_decl_listNode* varDeclList)
{
	print_var_decl(varDeclList->var_decl);
	if (varDeclList->var_decl_list != NULL)
		print_var_decl_list(varDeclList->var_decl_list);	
}

void print_var_decl(struct var_declNode* varDecl)
{	
	print_id_list(varDecl->id_list);
	printf(": ");
	print_type_name(varDecl->type_name);
	printf(";\n");
}

void print_type_decl_section(struct type_decl_sectionNode* typeDeclSection)
{
	printf("TYPE\n");
	if (typeDeclSection->type_decl_list != NULL)
		print_type_decl_list(typeDeclSection->type_decl_list);
}

void print_type_decl_list(struct type_decl_listNode* typeDeclList)
{
	print_type_decl(typeDeclList->type_decl);
	if (typeDeclList->type_decl_list != NULL)
		print_type_decl_list(typeDeclList->type_decl_list);	
}

void print_type_decl(struct type_declNode* typeDecl)
{	
	print_id_list(typeDecl->id_list);
	printf(": ");
	print_type_name(typeDecl->type_name);
	printf(";\n");
}

void print_type_name(struct type_nameNode* typeName)
{
	if (typeName->type != ID)
		printf("%s ", reserved[typeName->type]);
	else
		printf("%s ", typeName->id);
}

void print_id_list(struct id_listNode* idList)
{
	printf("%s ",idList->id);
	if (idList->id_list != NULL)
	{	printf(", ");
		print_id_list(idList->id_list);
	}
}

void print_stmt_list(struct stmt_listNode* stmt_list)
{
	print_stmt(stmt_list->stmt);
	if (stmt_list->stmt_list != NULL)
		print_stmt_list(stmt_list->stmt_list);

}

void print_assign_stmt(struct assign_stmtNode* assign_stmt)
{
	printf("%s ", assign_stmt->id);
	printf("= ");

	rightop_type = print_expression_prefix(assign_stmt->expr);
	rightop_id = selected_id;

	leftop_type = typevalue_to_typeid_map[assign_stmt->id];
	
	if ((leftop_type == ID) && (rightop_type == ID))
	{
		selected_id = rightop_id;
		selected_type = leftop_type;

		update_builtin_id_type();	
	}		
				
	else if (leftop_type == ID)
	{

		/**** Here *****User defined = Builtin type ******* please fix it *****/


		selected_id = leftop_id;
		selected_type = rightop_type;

		update_builtin_id_type();
	}

	else if (rightop_type == ID)
	{
		selected_id = rightop_id;
		selected_type = leftop_type;

		update_builtin_id_type();
	}

	else if (leftop_type != rightop_type)
	{
		error_code_set.insert(3);
		//exit(0);
	}

	printf("; \n");
}

void print_while_stmt(struct while_stmtNode* while_stmt)
{

//******************************************
//ADD THE LOGIC OF HANDLING WHILE PART
//******************************************

}

void print_stmt(struct stmtNode* stmt)
{
	if (stmt->stmtType == ASSIGN)
		print_assign_stmt(stmt->assign_stmt);
}

int print_expression_prefix(struct exprNode* expr)
{
	if (expr->tag == EXPR)
	{
		leftop_type = print_expression_prefix(expr->leftOperand);
		leftop_id = recent_id;
		recent_id = "";

		printf("%s ", reserved[expr->oper]);

		rightop_type = print_expression_prefix(expr->rightOperand);
		rightop_id = recent_id;
		recent_id = "";
		

		//case 1 one builtin and one user defined
		if ((leftop_type == ID) && (rightop_type == ID))
		{
			//Update rightop_id with leftop_id
						
			selected_id = leftop_id;
			selected_type = rightop_type;	

			update_builtin_id_type();

			rightop_id = leftop_id = "";
		}

		else if ((leftop_type == ID) && (rightop_type != ID) || ((rightop_type == ID && leftop_type != ID)))
		{
			if (leftop_type == ID)
			{
				selected_id = leftop_id;
				selected_type = rightop_type;

				update_builtin_id_type();
			}
			else 
			{

				selected_id = rightop_id;
				selected_type = leftop_type;
	
				update_builtin_id_type();
   			}
		}
		else if (leftop_type != rightop_type)
		{
			error_code_set.insert(2);
			//exit(0);	
		}
		
	} else
	if (expr->tag == PRIMARY)
	{
		if (expr->primary->tag == ID)
		{
			printf("%s ", expr->primary->id);
			recent_id = expr->primary->id;
			return ID;
		}
		else if (expr->primary->tag == NUM)
		{
			printf("%d ", expr->primary->ival);
			return NUM;
		}
		else if (expr->primary->tag == REALNUM)
		{
			printf("%.4f ", expr->primary->fval);
			return REALNUM;
		}
	}
	return 0;
}

void update_builtin_id_type()
{
	temp_id_list = typesec_typename_to_idlist_map[selected_id];
	for(sl_it = temp_id_list.begin(); sl_it != temp_id_list.end(); sl_it++)
	{
		typevalue_to_typeid_map[(*sl_it)] = selected_type;
	}	
	typevalue_to_typeid_map[selected_id] = selected_type;

	temp_id_list = varsec_typename_to_idlist_map[selected_id];
	for(sl_it_2 = temp_id_list.begin(); sl_it_2 != temp_id_list.end(); sl_it_2++)
	{
		typevalue_to_typeid_map[(*sl_it_2)] = selected_type;
	}	
} 


/*--------------------------------------------------------------------
  CREATING PARSE TREE NODE
---------------------------------------------------------------------*/
struct programNode* make_programNode()
{	
	return (struct programNode*) malloc(sizeof(struct programNode));
}

struct declNode* make_declNode()
{
	return (struct declNode*) malloc(sizeof(struct declNode));
}

struct type_decl_sectionNode* make_type_decl_sectionNode()
{
	return (struct type_decl_sectionNode*) malloc(sizeof(struct type_decl_sectionNode));
}

struct var_decl_sectionNode* make_var_decl_sectionNode()
{
	return (struct var_decl_sectionNode*) malloc(sizeof(struct var_decl_sectionNode));
}

struct var_declNode* make_var_declNode()
{
	return (struct var_declNode*) malloc(sizeof(struct var_declNode));
}

struct type_declNode* make_type_declNode()
{
	return (struct type_declNode*) malloc(sizeof(struct type_declNode));
}

struct type_decl_listNode* make_type_decl_listNode()
{
	return (struct type_decl_listNode*) malloc(sizeof(struct type_decl_listNode));
}

struct var_decl_listNode* make_var_decl_listNode()
{
	return (struct var_decl_listNode*) malloc(sizeof(struct var_decl_listNode));
}

struct id_listNode* make_id_listNode()
{
	return (struct id_listNode*) malloc(sizeof(struct id_listNode));
}

struct type_nameNode* make_type_nameNode()
{
	return (struct type_nameNode*) malloc(sizeof(struct type_nameNode));
}

struct bodyNode* make_bodyNode()
{
	return (struct bodyNode*) malloc(sizeof(struct bodyNode));
}

struct stmt_listNode* make_stmt_listNode()
{
	return (struct stmt_listNode*) malloc(sizeof(struct stmt_listNode));
}

struct stmtNode* make_stmtNode()
{
	return (struct stmtNode*) malloc(sizeof(struct stmtNode));
}

struct while_stmtNode* make_while_stmtNode()
{
	return (struct while_stmtNode*) malloc(sizeof(struct while_stmtNode));
}

struct assign_stmtNode* make_assign_stmtNode()
{
	return (struct assign_stmtNode*) malloc(sizeof(struct assign_stmtNode));
}

struct exprNode* make_exprNode()
{
	return (struct exprNode*) malloc(sizeof(struct exprNode));
}

struct primaryNode* make_primaryNode()
{
	return (struct primaryNode*) malloc(sizeof(struct primaryNode));
}

/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
  PARSING AND BUILDING PARSE TREE
---------------------------------------------------------------------*/
struct primaryNode* primary()
{
	struct primaryNode* primar;
}

struct conditionNode* condition()
{
	return NULL;
}

struct exprNode* factor()
{
	struct exprNode* facto;
	
	ttype = getToken();
	
	if (ttype == LPAREN)
	{	facto = expr();
		ttype = getToken();
		if (ttype == RPAREN)
			return facto;
		else 
		{	syntax_error("factor. RPAREN expected", line_no);
			exit(0);
		}
	} else
	if (ttype == NUM)
	{	facto = make_exprNode();
		facto->primary = make_primaryNode();
		facto->tag = PRIMARY;
		facto->oper = NOOP;
		facto->leftOperand = NULL;
		facto->rightOperand = NULL;
		facto->primary->tag = NUM;
		facto->primary->ival = atoi(token);
		return facto;
	} else
	if (ttype == REALNUM)
	{	facto = make_exprNode();
		facto->primary = make_primaryNode();
		facto->tag = PRIMARY;
		facto->oper = NOOP;
		facto->leftOperand = NULL;
		facto->rightOperand = NULL;
		facto->primary->tag = REALNUM;
		facto->primary->fval = atof(token);
		return facto;
	} else
	if (ttype == ID)
	{	facto = make_exprNode();
		facto->primary = make_primaryNode();
		facto->tag = PRIMARY;
		facto->oper = NOOP;
		facto->leftOperand = NULL;
		facto->rightOperand = NULL;
		facto->primary->tag = ID;
		facto->primary->id = (char *) malloc((tokenLength+1)*sizeof(char));
		strcpy(facto->primary->id,token);
		return facto;
	} else
	{	syntax_error("factor. NUM, REALNUM, or ID, expected", line_no);
		exit(0);
	}
}

struct exprNode*term()
{
	struct exprNode* ter;
	struct exprNode* f;

	
	ttype = getToken();
	if ((ttype == ID)|(ttype == LPAREN)|(ttype == NUM)|(ttype == REALNUM))
	{	ungetToken();
		f = factor();
		ttype = getToken();
		if ((ttype == MULT) | (ttype == DIV))
		{	ter = make_exprNode();
			ter->oper = ttype; 
			ter->leftOperand = f;
			ter->rightOperand = term();
			ter->tag = EXPR;
			return ter;
		} else	
		if ((ttype == SEMICOLON)|(ttype == PLUS)|(ttype == MINUS)|(ttype == RPAREN))
		{	ungetToken();
			return f;
		} else
		{	syntax_error("term. MULT or DIV expected", line_no);
			exit(0);
		}
	} else
	{	syntax_error("term. ID, LPAREN, NUM, or REALNUM expected", line_no);
		exit(0);
	}
}


struct exprNode* expr()
{
	struct exprNode* exp;
	struct exprNode* t;
	
	ttype = getToken();
	if ((ttype == ID)|(ttype == LPAREN)|(ttype == NUM)|(ttype == REALNUM))
	{	ungetToken();
		t = term();
		ttype = getToken();
		if ((ttype == PLUS) | (ttype == MINUS))
		{	exp = make_exprNode();
			exp->oper = ttype; 
			exp->leftOperand = t;
			exp->rightOperand = expr();			
			exp->tag = EXPR;
			return exp;
		} else	
		if ((ttype == SEMICOLON)|(ttype == MULT)|(ttype==DIV)|(ttype == RPAREN))
		{	ungetToken();
			return t;
		} else
		{	syntax_error("expr. PLUS, MINUS, or SEMICOLON expected", line_no);
			exit(0);
		}
	} else
	{	syntax_error("expr. ID, LPAREN, NUM, or REALNUM expected", line_no);
		exit(0);
	}
}

struct assign_stmtNode* assign_stmt()
{	struct assign_stmtNode* assignStmt;

	ttype = getToken();
	if (ttype == ID) 
	{	assignStmt = make_assign_stmtNode();
		assignStmt->id = (char *) malloc((tokenLength+1)*sizeof(char));
		strcpy(assignStmt->id,token);
		ttype = getToken();
		if (ttype == EQUAL)
		{	assignStmt->expr = expr();
			stmtlhs_to_stmtrhsnode_map[assignStmt->id] = assignStmt->expr;
			return assignStmt;
		} else
		{	syntax_error("assign_stmt. EQUAL expected", line_no);
			exit(0);
		}
	} else
	{
		syntax_error("assign_stmt. ID expected", line_no);
		exit(0);
	}
}

struct while_stmtNode* while_stmt()
{
//***********************************************
// ADD LOGIC OF PARSING WHILE STMT HERE
//*********************************************

	return NULL;
}

struct stmtNode* stmt()
{
	struct stmtNode* stm;

	ttype = getToken();
	stm = make_stmtNode();

	if (ttype == ID) // assign_stmt
	{	ungetToken();
		stm->assign_stmt = assign_stmt();
		stm->stmtType = ASSIGN;
		ttype = getToken();
		if (ttype == SEMICOLON)
		{	return stm;
		}
		else
		{	syntax_error("stmt. SEMICOLON expected", line_no);
			exit(0);
		}
	} else
	if (ttype == WHILE) // while_stmt
	{	ungetToken();
		stm->while_stmt = while_stmt();
		stm->stmtType = WHILE;	
	} else // syntax error
	{
		syntax_error("stmt. ID or WHILE expected", line_no);
		exit(0);
	}	
}

struct stmt_listNode* stmt_list()
{
	struct stmt_listNode* stmtList;

	ttype = getToken();

	if ((ttype == ID)|(ttype == WHILE))
	{	ungetToken();
		stmtList = make_stmt_listNode();
		stmtList->stmt = stmt();
		ttype = getToken();
		if (ttype == ID)
		{	ungetToken();
			stmtList->stmt_list = stmt_list();
			return stmtList;
		} else	// If the next token is not in FOLLOW(stmt_list), 
			// let the caller handle it. 
		{	ungetToken();
			return stmtList;
		}
	} else
	{
		syntax_error("stmt_list. ID or WHILE expected", line_no);
		exit(0);
	}
}

struct bodyNode* body()
{	struct bodyNode* bod;

	ttype = getToken();
	if (ttype == LBRACE)
	{	bod = make_bodyNode();
		bod->stmt_list = stmt_list();
		ttype = getToken();
		if (ttype == RBRACE)
			return bod;
		else
		{	syntax_error("body. RBRACE expected", line_no);
			exit(0); 
		}
	} else
	{	syntax_error("body. LBRACE expected", line_no);
		exit(0); 
	}
}

struct type_nameNode* type_name()
{
	struct type_nameNode* tName;
	tName = make_type_nameNode();

	ttype = getToken();
	if ((ttype == ID)|(ttype == INT)|(ttype==REAL)
		|(ttype == STRING)|(ttype==BOOLEAN))
	{	tName->type = ttype;
		if (ttype == ID)
		{	tName->id = (char *) malloc(tokenLength+1);
			strcpy(tName->id,token);
		}
		if(typevalue_to_typeid_map.count(token)==0)
		{
			typevalue_to_typeid_map[token] = ttype;
			repeating_typename_set.insert(token);	
		}	
		typename_order_list.push_back(token);
		temp_type_string = token; 
		return tName;
	} else
	{	syntax_error("type_name. type name expected", line_no);
		exit(0);
	}
}

struct id_listNode* id_list()
{
	struct id_listNode* idList;
	idList = make_id_listNode();
	ttype = getToken();
	if (ttype == ID)
	{	
		idList->id = (char*) malloc(tokenLength+1);
		strcpy(idList->id, token);
		temp_id_list.push_back(token);
		ttype = getToken();
		if (ttype == COMMA)
		{
			idList->id_list = id_list();
			return idList;
			
		} else
		if (ttype == COLON)
		{	ungetToken();
			idList->id_list = NULL;
			return idList;
		} else
		{	syntax_error("id_list. COMMA or COLON expected", line_no);
			exit(0);
		}
	} else
	{	syntax_error("id_list. ID expected", line_no);
		exit(0);
	}
}

struct type_declNode* type_decl()
{	
	struct type_declNode* typeDecl;
	typeDecl = make_type_declNode();
	ttype = getToken();
	if (ttype == ID)
	{	ungetToken();
		typeDecl->id_list = id_list();
		ttype = getToken();
		if (ttype == COLON)
		{	typeDecl->type_name = type_name();
			if(typesec_typename_to_idlist_map.count(temp_type_string)>0)
				old_id_list = typesec_typename_to_idlist_map[temp_type_string];
				for(sl_it=old_id_list.begin(); sl_it!=old_id_list.end(); sl_it++)
				{
					temp_id_list.push_back(*sl_it);
				}
				old_id_list.clear();
			typesec_typename_to_idlist_map[temp_type_string] = temp_id_list;
			temp_id_list.clear();
			ttype = getToken();
			if (ttype == SEMICOLON)
			{	return typeDecl;
			}
			else
			{	syntax_error("type_decl. SEMICOLON expected", line_no);
				exit(0);
			}
		} 
	} else
	{
		syntax_error("type_decl. ID expected", line_no);
	}
}

struct var_declNode* var_decl()
{
	struct var_declNode* varDecl;
	varDecl = make_var_declNode();
	ttype = getToken();
	if (ttype == ID)
	{	ungetToken();
		varDecl->id_list = id_list();
		ttype = getToken();
		if (ttype == COLON)
		{	varDecl->type_name = type_name();
			if (varsec_typename_to_idlist_map.count(temp_type_string)>0)
				old_id_list = varsec_typename_to_idlist_map[temp_type_string];
				for (sl_it = old_id_list.begin(); sl_it != old_id_list.end(); sl_it++)
				{
					temp_id_list.push_back(*sl_it);
				}
			varsec_typename_to_idlist_map[temp_type_string] = temp_id_list;
			temp_id_list.clear();	
			ttype = getToken();
			if (ttype == SEMICOLON)
			{	return varDecl;
			}
			else
			{	syntax_error("var_decl. SEMICOLON expected", line_no);
				exit(0);
			}
		} 
	}
}	

struct var_decl_listNode* var_decl_list()
{
	struct var_decl_listNode* varDeclList;
	varDeclList = make_var_decl_listNode();

	ttype = getToken();
	if (ttype == ID)
	{	ungetToken();
		varDeclList->var_decl = var_decl();
		ttype = getToken();
		if (ttype == ID)
		{	ungetToken();
			varDeclList->var_decl_list = var_decl_list();
			return varDeclList;
		}  else	
		{	ungetToken();
			return varDeclList;
		} 
	} else
	{	syntax_error("var_decl_list. ID expected", line_no);
		exit(0);
	} 
}

struct type_decl_listNode* type_decl_list()
{
	struct type_decl_listNode* typeDeclList;
	typeDeclList = make_type_decl_listNode();

	ttype = getToken();
	if (ttype == ID)
	{	ungetToken();
		typeDeclList->type_decl = type_decl();
		ttype = getToken();
		if (ttype == ID)
		{	ungetToken();
			typeDeclList->type_decl_list = type_decl_list();
			return typeDeclList;
		}  else	
		{	ungetToken();
			return typeDeclList;
		} 
	} else
	{	syntax_error("type_decl_list. ID expected", line_no);
		exit(0);
	} 
}

struct var_decl_sectionNode* var_decl_section()
{
	struct var_decl_sectionNode *varDeclSection;
	varDeclSection = make_var_decl_sectionNode();
	
	//Copy TYPE SEC seen type name order and empty it.
	copy_typsec_typename_order_list();

	ttype = getToken();
	if (ttype == VAR)
	{	// no need to ungetToken() 
		varDeclSection->var_decl_list = var_decl_list();  
		return varDeclSection;
	} else
	{	syntax_error("var_decl_section. VAR expected", line_no);
		exit(0);
	}
}

struct type_decl_sectionNode* type_decl_section()
{
	struct type_decl_sectionNode *typeDeclSection;
	typeDeclSection = make_type_decl_sectionNode();

	ttype = getToken();
	if (ttype == TYPE)
	{
		typeDeclSection->type_decl_list = type_decl_list();
		return typeDeclSection;            
	} else
	{	syntax_error("type_decl_section. TYPE expected", line_no);
		exit(0);
	}
}


struct declNode* decl()
{	
	struct declNode* dec;
	dec = make_declNode();

	ttype = getToken();
	if (ttype == TYPE)
	{	ungetToken();
		dec->type_decl_section = type_decl_section();
		ttype = getToken();
		if (ttype == VAR)
    		{	// type_decl_list is epsilon
			// or type_decl already parsed and the 
			// next token is checked
			ungetToken();
       			dec->var_decl_section = var_decl_section();
    		} else
		{	ungetToken();
			dec->var_decl_section = NULL;
		}
		return dec;
	} else
	{
		dec->type_decl_section = NULL; 
	    	if (ttype == VAR)
    		{	// type_decl_list is epsilon
			// or type_decl already parsed and the 
			// next token is checked
			ungetToken(); 
       			dec->var_decl_section = var_decl_section();
			return dec;
    		} else
		{	if (ttype == LBRACE)
			{	ungetToken();	
				dec->var_decl_section = NULL;
				return dec;
			} else
			{	syntax_error("decl. LBRACE expected", line_no);
				exit(0);		// stop after first syntax error
			}
		}
	}
}

struct programNode* program()
{	struct programNode* prog;

	prog = make_programNode();
	ttype = getToken();
	if ((ttype == TYPE) | (ttype == VAR) | (ttype == LBRACE))
	{	ungetToken();  
		prog->decl = decl();
		prog->body = body();
		return prog;
	} else
	{	syntax_error("program. TYPE or VAR or LBRACE expected", line_no);
		exit(0);		// stop after first syntax error
	}
}


void play_with_ds()
{

	// Populate typsec_typelist and typesec_idlist 

	for(slm_it = typesec_typename_to_idlist_map.begin(); slm_it!=typesec_typename_to_idlist_map.end(); slm_it++)
	{
		typesec_typelist.push_back((*slm_it).first);
			
		for(sl_it = (*slm_it).second.begin(); sl_it != (*slm_it).second.end(); sl_it++)
		{
			typesec_idlist.push_back(*sl_it);
			typesec_idset.insert(*sl_it);			
		}	
	}

	// Populate varsec_typelist and varsec_idlist

	for(slm_it = varsec_typename_to_idlist_map.begin(); slm_it!=varsec_typename_to_idlist_map.end(); slm_it++)
	{
		varsec_typelist.push_back((*slm_it).first);
		
		for(sl_it = (*slm_it).second.begin(); sl_it != (*slm_it).second.end(); sl_it++)
		{
			varsec_idlist.push_back(*sl_it);
			varsec_idset.insert(*sl_it);
		}
	}
}


void print_ds()
{
	// Print typsec_typelist and typesec_idlist 

	//cout<< "After first for loop in print ds"<<"\n";
	for(slm_it = typesec_typename_to_idlist_map.begin(); slm_it!=typesec_typename_to_idlist_map.end(); slm_it++)
	{
		cout<<((*slm_it).first)<<" :";
			
		for(sl_it = (*slm_it).second.begin(); sl_it != (*slm_it).second.end(); sl_it++)
		{
			cout<<(*sl_it)<<" ,";			
		}
		cout<<"\n";	
	}
	cout<<"\n\n\n";

	// Print varsec_typelist and varsec_idlist
	//cout<< "After second for loop in print ds"<<"\n";
	for(slm_it = varsec_typename_to_idlist_map.begin(); slm_it!=varsec_typename_to_idlist_map.end(); slm_it++)
	{
		cout<<((*slm_it).first)<<" :";
		
		for(sl_it = (*slm_it).second.begin(); sl_it != (*slm_it).second.end(); sl_it++)
		{
			cout<<(*sl_it)<<" ,";
		}
		cout<<"\n";
	}
	cout<<"\n\n\n";


	//Print typevalue_to_typeid_map
	//cout<< "After third for loop in print ds"<<"\n";
	
	for(nid_it = typevalue_to_typeid_map.begin(); nid_it != typevalue_to_typeid_map.end(); nid_it++)
	{
		cout<<(*nid_it).first<<"   "<<(*nid_it).second<<" "<<"\n";
	}	
	cout<<"\n\n\n";
}



void check_for_error_typesec()
{
	//cout<< "\n"<<"Check for error type section"<<"\n";
	for(sl_it = typesec_typename_order_list.begin(); sl_it != typesec_typename_order_list.end(); sl_it++)
	{
		temp_typename = *sl_it;
		temp_typeid = typevalue_to_typeid_map[temp_typename];
		
		//check for error code 0
		if (seen_typesec_typename_set.count(temp_typename)!=0 and temp_typeid == 33)
		{
			error_code_set.insert(0);
			//exit(0);
		}

		//check for error code 1
		if ((seen_typesec_ids_set.count(temp_typename)==0) && (typesec_idset.count(temp_typename)>0))

		{
			error_code_set.insert(1);
			//exit(0);

		}

		//check for error code 2
		temp_id_list = typesec_typename_to_idlist_map[temp_typename];
		for(sl_it_2 = temp_id_list.begin(); sl_it_2 != temp_id_list.end(); sl_it_2++)
		{
			if (seen_typesec_ids_set.count(*sl_it_2)!=0)
			{
				error_code_set.insert(2);
				//exit(0);
			}
			seen_typesec_ids_set.insert(*sl_it_2);
		}
	
		//Populating seen data structures
	
		seen_typesec_typename_set.insert(temp_typename);

	} 
}


void check_for_error_varsec()
{
	//cout<<"\n"<<"check for error var sec"<<"\n";

	for(sl_it = varsec_typename_order_list.begin(); sl_it!=varsec_typename_order_list.end(); sl_it++)
	{
		cout<<"\n"<<"Inside for loop of check error for var sec"<<*sl_it<<"\n";
		temp_typename = *sl_it;
		temp_typeid = typevalue_to_typeid_map[temp_typename];
	
		//check for error code 0	
		if (seen_varsec_typename_set.count(temp_typename)!=0 and temp_typeid == 33)
		{
			error_code_set.insert(0);
			//exit(0);
		}

		//check for error code 1
		if ((seen_varsec_ids_set.count(temp_typename)==0) && (varsec_idset.count(temp_typename)>0))

		{
			error_code_set.insert(1);

		}

		//check for error code 2
		temp_id_list = varsec_typename_to_idlist_map[temp_typename];
		for(sl_it_2 = temp_id_list.begin(); sl_it_2 != temp_id_list.end(); sl_it_2++)
		{
			if (seen_varsec_ids_set.count(*sl_it_2)!=0)
			{
				error_code_set.insert(2);
			}
			if (seen_typesec_typename_set.count(*sl_it_2)>0)
			{
				error_code_set.insert(1);
			}
			seen_varsec_ids_set.insert(*sl_it_2);
		}
	
		//Populating seen data structures
	
		seen_varsec_typename_set.insert(temp_typename);

	}
	//cout << "\n"<<"ENd of error chekcing for var sec"<<"\n"; 
}


void type_typeconversion()
{
	cout << "typeconversion part";
	start_val = 13;

	//TYPE results table manipulation

	
	for(sl_it = typesec_typename_order_list.begin(); sl_it != typesec_typename_order_list.end(); sl_it++)
	{
		temp_typename = *sl_it;
		temp_typeid = typevalue_to_typeid_map[temp_typename];

		if (temp_typeid==ID)
		{
			new_val += start_val + 1;
			start_val = new_val;
			typevalue_to_typeid_map[temp_typename] = new_val;
		
			//update this change to rest of the vaues having this type
			temp_id_list = typesec_typename_to_idlist_map[temp_typename];
			for(sl_it_2 = temp_id_list.begin(); sl_it_2 != temp_id_list.end(); sl_it_2++)
			{
				typevalue_to_typeid_map[(*sl_it_2)] = new_val;
			}
		}
		else if(repeating_typename_set.count(temp_typename) > 0)
		{
			//In this case, update the temp id value to all the ids
			temp_id_list = typesec_typename_to_idlist_map[temp_typename];
			for(sl_it_3 = temp_id_list.begin(); sl_it_3 !=temp_id_list.end(); sl_it_3++)
			{
				typevalue_to_typeid_map[(*sl_it_3)] = temp_typeid;
			}
			typevalue_to_typeid_map[temp_typename] = temp_typeid;
		}		
	}
}


void var_typeconversions()

{
	//VAR results table manipulation

	for(sl_it = varsec_typename_order_list.begin(); sl_it != varsec_typename_order_list.end(); sl_it++)
	{
		temp_typename = *sl_it;
		temp_typeid = typevalue_to_typeid_map[temp_typename];

		if (temp_typeid==ID)
		{
			new_val += start_val + 1;
			start_val += 1;
			typevalue_to_typeid_map[temp_typename] = new_val;
		
			//update this change to rest of the vaues having this type
			temp_id_list = varsec_typename_to_idlist_map[temp_typename];
			for(sl_it_2 = temp_id_list.begin(); sl_it_2 != temp_id_list.end(); sl_it_2++)
			{
				typevalue_to_typeid_map[*sl_it_2] = new_val;
			}
		}
		else 
		{
			temp_id_list = varsec_typename_to_idlist_map[temp_typename];
			for(sl_it_3 = temp_id_list.begin(); sl_it_3 != temp_id_list.end(); sl_it_3++)
			{
				typevalue_to_typeid_map[*sl_it_3] = temp_typeid;
			}
		}

	}
}


void copy_typsec_typename_order_list()
{
	for(sl_it=typename_order_list.begin(); sl_it!=typename_order_list.end(); sl_it++)
	{
		typesec_typename_order_list.push_back(*sl_it);
	}
	typename_order_list.clear();
}	

void copy_varsec_typename_order_list()
{
	//cout << "\n"<<"Inside copy varsec typename order list"<<"\n";

	for(sl_it=typename_order_list.begin(); sl_it!=typename_order_list.end(); sl_it++)
	{
		varsec_typename_order_list.push_back(*sl_it);
	}
	//cout << "\n size of varsec order list"<<varsec_typename_order_list.size()<<"\n";
	typename_order_list.clear();
}

void print_error()
{
	for(is_it=error_code_set.begin(); is_it!=error_code_set.end(); is_it++)
	{
		cout<<"Error "<<*is_it<<"\n";
	}
}

void check_for_error()
{
	copy_varsec_typename_order_list();
	check_for_error_typesec();
	check_for_error_varsec();
	print_error();
}



// COMMON mistakes:
//    *     = instead of ==
//    *     not allocating space before strcpy
int main()
{
	struct programNode* parseTree;
	parseTree = program();
	play_with_ds();	
	check_for_error();
	//print_ds();
	type_typeconversion();
	var_typeconversions();
	print_ds();
	print_parse_tree(parseTree);
	check_for_error();
	printf("\nSUCCESSFULLY PARSED INPUT!\n");
	return 0;
}

