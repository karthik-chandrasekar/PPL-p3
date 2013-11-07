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
#include <vector>

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

list<string> order_id_list(set<string> temp_id_set);
void print_set(set<string> temp_id_set);
void print_list(list<string> temp_id_list);

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
int start_val = 13;
int new_val;
int leftop_type;
int rightop_type;
int selected_type;
int UD = 13;
int up_limit = 33;

//List
list<string> typename_order_list;
list<string> typesec_typename_order_list;
list<string> varsec_typename_order_list;
list<string>  temp_id_list;
list<string> temp_id_list_2;
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
list<int> error_code_list;

list<string> type_explicit_list;
list<string> type_implicit_list;
list<string> var_explicit_list;
list<string> var_implicit_list;
list<string> built_in_types_list;

//Set

set<string> seen_typesec_typename_set;
set<string> seen_typesec_ids_set;
set<string> seen_varsec_typename_set;
set<string> seen_varsec_ids_set;
set<string> typesec_idset;
set<string> varsec_idset;
set<int> :: iterator is_it; 				//is_it=int_set_it
set<string> old_id_set;
set<string> :: iterator ss_it; 				// string set iterator
set<string>  temp_id_set;
set<string> temp_id_set_1;
set<string> built_in_types_set;
set<string> published_op_set;
set<int> seen_id_set;

//Map
map<string, int>  typevalue_to_typeid_map;
map<string, list<string> >  typesec_typename_to_idlist_map;
map<string, list<string> > varsec_typename_to_idlist_map;
map<string, int> var_implicit_pos_map;

map<int, list<string> > typesec_typeid_to_ids_list_map;
map<int, list<string> > varsec_typeid_to_ids_list_map;
map<int, set<string> > typeid_to_ids_set_map;
map<int, list<string> > typeid_to_ids_list_map;
map<string, set<string> > output_map;
map<string, int> type_decl_map;
map<string, int> var_decl_map;


//Map Iterator
map<string, list<string> > :: iterator slm_it; 						//slm_it = string_to_list_map_it
map<string, exprNode*> :: iterator sem_it; 						//sem_it = string_to_exprNode_map_it
map<string, int> :: iterator nid_it; 							//nid_it = typevalue_to_typeid_map
map<int, list<string> > :: iterator ilm_it; 						//ilm_it = int_to_list_map_it
map<int, set<string> > :: iterator ism_it;						//ism_it = int_to_set_map_it
map<string, set<string> > :: iterator ssm_it;                   //ssm_it = string_to_set_map_it



//Vector

vector<pair<string, list<string> > > output_vector;

vector<pair<string, list<string> > > :: iterator vec_it;

vector<pair<string, list<string> > > uo_typesec_map;
vector<pair<string, list<string> > > uo_varsec_map;

vector<pair<string, list<string> > > :: iterator usl_it;
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
   {   //printf("%s\n",reserved[ttype]);
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
	//printf("Syntax error while parsing %s line %d\n", NT, line_no);
}

/*--------------------------------------------------------------------
  PRINTING PARSE TREE
---------------------------------------------------------------------*/
void print_parse_tree(struct programNode* program)
{
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
    //cout<<"Inside print body";
	//printf("{\n");
	print_stmt_list(body->stmt_list); 
    //printf("}\n");
	
}

void print_var_decl_section(struct var_decl_sectionNode* varDeclSection)
{
	//printf("VAR\n");
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
	//printf(": ");
	print_type_name(varDecl->type_name);
	//printf(";\n");
}

void print_type_decl_section(struct type_decl_sectionNode* typeDeclSection)
{
	//printf("TYPE\n");
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
	//printf(": ");
	//print_type_name(typeDecl->type_name);
	//printf(";\n");
}

void print_type_name(struct type_nameNode* typeName)
{
}

void print_id_list(struct id_listNode* idList)
{
	//printf("%s ",idList->id);
	if (idList->id_list != NULL)
	{	//printf(", ");
		print_id_list(idList->id_list);
	}
}

void print_stmt_list(struct stmt_listNode* stmt_list)
{

    //cout<<"\nprint stmt list\n";
	print_stmt(stmt_list->stmt);	
	if (stmt_list->stmt_list != NULL)
		print_stmt_list(stmt_list->stmt_list);

}

void print_assign_stmt(struct assign_stmtNode* assign_stmt)
{

	//printf("%s ", assign_stmt->id);
	//printf("= ");

    //cout << "\nprint assign stmt\n";

    
    //cout << "Rightop info "<<selected_id<< " is "<< rightop_type<<"\n";

    if(typevalue_to_typeid_map[assign_stmt->id] == 0)
      {

        typevalue_to_typeid_map[assign_stmt->id] = start_val + 1;
        start_val = start_val + 1;

        temp_id_list.clear();
        temp_id_list.push_back(assign_stmt->id);

        //cout << "Inserting  "<<start_val<< "  "<<assign_stmt->id<<"\n";

        varsec_typeid_to_ids_list_map[start_val] = temp_id_list;

        temp_id_list.clear();

        varsec_typename_order_list.push_back(assign_stmt->id);

        leftop_type = start_val;

        temp_id_set.clear();
        temp_id_set.insert(assign_stmt->id);

        typeid_to_ids_set_map[leftop_type] = temp_id_set;

        if(var_decl_map.count(assign_stmt->id)==0)
        {
            var_decl_map[assign_stmt->id] = 0;
            var_implicit_list.push_back(assign_stmt->id);
            var_implicit_pos_map[assign_stmt->id] = var_implicit_list.size();
        
            
        }
      }
    
    else
      {
        leftop_type = typevalue_to_typeid_map[assign_stmt->id];
      }
	
	rightop_type = print_expression_prefix(assign_stmt->expr);
    
    //cout<< "Leftop info  "<<assign_stmt->id<<" is "<< leftop_type<<"\n";
    //cout<<"\n Right info "<<rightop_type<<"\n\n";
    
	if (((leftop_type > UD) && (rightop_type > UD)) && (leftop_type != rightop_type))
	{
            if (var_implicit_pos_map.count(selected_id)>0 && var_implicit_pos_map.count(selected_id)>0)
            {
                if(var_implicit_pos_map[selected_id] > var_implicit_pos_map[assign_stmt->id])
                {
                    //cout << "\nINSIDEEEE IF-- ----- IF --- FLOWWWW\n";
		            update_builtin_id_type(rightop_type, leftop_type);	
                    
                }
                else 
                {
                    //cout << "\nINSIDEEEE IF ---------- ELSE ----- FLOWWWW\n";
		            update_builtin_id_type(leftop_type, rightop_type);	
                
                }
            }
            else
            {
                //cout<< "\nINSIDEEEE ELSE FLOOWWWW\n";
		        update_builtin_id_type(rightop_type, leftop_type);	
            }
	}		
			
	else if (leftop_type > UD)
	{
		/**** Here *****User defined = Builtin type ******* please fix it *****/

		update_builtin_id_type(leftop_type, rightop_type);
	}

	else if (rightop_type > UD)
	{
		//cout << "If condition over";
		update_builtin_id_type(rightop_type, leftop_type);
	}

	else if (leftop_type != rightop_type)
	{
		error_code_list.push_back(3);
        cout<< "ERROR CODE 3\n";
		exit(0);
	}

	//printf("; \n");
}

void print_condition(struct conditionNode* condition)
{
    leftop_type = 0;
    rightop_type = 0;

    //cout<<"WHILE ";
    if (condition->left_operand != NULL)
    {
        leftop_type = print_operand(condition->left_operand);
       // cout<< "\nLeftop type  "<<leftop_type<<"\n";

        if (leftop_type == ID)
        {
            new_val = start_val + 1;
            start_val = new_val;
            temp_typename = condition->left_operand->id;
            typevalue_to_typeid_map[temp_typename] = new_val;
            leftop_type = new_val;

            // Order mainteinance addition
            var_decl_map[temp_typename] = 0;
            var_implicit_list.push_back(temp_typename);

            //Content addition
            
            // Need to add in three places

            // First place
            temp_id_set.clear();
            temp_id_set.insert(temp_typename);
            typeid_to_ids_set_map[leftop_type] = temp_id_set;
            
            // Second place
            temp_id_list.clear();
            temp_id_list.push_back(temp_typename);
            if(varsec_typeid_to_ids_list_map.count(leftop_type)>0)
            {
                temp_id_list = varsec_typeid_to_ids_list_map[leftop_type];
                temp_id_list.push_back(temp_typename);
            }
            varsec_typeid_to_ids_list_map[leftop_type] = temp_id_list;

            //Third place
            varsec_typename_order_list.push_back(temp_typename);
            
        
            //cout<<"\nAdded "<<temp_typename<<" from leftop type and the value is "<<new_val<<"\n";
        }
    }
     if (condition -> right_operand != NULL)
    {
        //printf("%d", condition->relop);
        rightop_type = print_operand(condition->right_operand);
        //cout<< "\nRightop type  "<<rightop_type<<"\n";

        if (rightop_type == ID)
        {
            new_val = start_val + 1;
            start_val = new_val;
            temp_typename = condition->right_operand->id;
            typevalue_to_typeid_map[temp_typename] = new_val;
            rightop_type = new_val; 

            //Order maintenance addition
            var_decl_map[temp_typename] = 0;
            var_implicit_list.push_back(temp_typename);

            //Content addition
            
            //Need to add in three places

            //First place

            temp_id_set.clear();
            temp_id_set.insert(temp_typename);
            typeid_to_ids_set_map[rightop_type] = temp_id_set;

            //Second place
            temp_id_list.clear();
            temp_id_list.push_back(temp_typename);
            if(varsec_typeid_to_ids_list_map.count(rightop_type)>0)
            {
                temp_id_list = varsec_typeid_to_ids_list_map[rightop_type];
                temp_id_list.push_back(temp_typename);
            } 
            varsec_typeid_to_ids_list_map[leftop_type] = temp_id_list;
            varsec_typename_order_list.push_back(temp_typename);

        }
    }
    //cout<<"\n";

    if(leftop_type != 0 && rightop_type !=0)
    {
        //both the operands present
        if (leftop_type > UD && rightop_type > UD)
            {
                //cout<<"\nENTERING IN TO FIRST FLOWWWW\n";
                update_builtin_id_type(rightop_type, leftop_type);  
                update_builtin_id_type(leftop_type, BOOLEAN);      
            }
        else if((leftop_type <= UD || rightop_type <= UD) && (leftop_type != rightop_type))
            {
                cout<<"ERROR CODE 3\n";
                exit(0);
            }
    }
    else if(leftop_type !=0)
    {
        //only single operand present

        if(leftop_type >= UD)
        {
            update_builtin_id_type(leftop_type, BOOLEAN);
        }
        else
        {
            cout<< "ERROR CODE 3\n";
            exit(0);
        }
    }

}


int print_operand(struct primaryNode* primary)
{
   
    if (primary->tag == ID)
    {
        //printf(" %s ", primary->id);
        if(typevalue_to_typeid_map.count(primary->id)>0)
        {
            return typevalue_to_typeid_map[primary->id];
        }

        else
        {
            return primary->tag;
        }
    }   
    else if(primary->tag == NUM)
    {
        //printf(" %d ", primary->ival);
        return primary->tag;
    }
    else if(primary->tag == REALNUM)
    {
        //printf(" %.4f ", primary->fval);
        return primary->tag;
    }
}

void print_while_stmt(struct while_stmtNode* while_stmt)
{

   print_condition(while_stmt->condition);
   print_body(while_stmt->body); 

}

void print_stmt(struct stmtNode* stmt)
{
    //cout << "\nprint stmt\n";
	if (stmt->stmtType == ASSIGN)
    {
        //cout<<"\n If assign stmt \n";
		print_assign_stmt(stmt->assign_stmt);
    }
    else if (stmt->stmtType == WHILE)
        print_while_stmt(stmt->while_stmt);
}

int print_expression_prefix(struct exprNode* expr)
{
	selected_type = 0;

	if (expr->tag == EXPR)
	{
		leftop_type = print_expression_prefix(expr->leftOperand);
        //cout << "\nLeftop type is "<<leftop_type<<"\n";

		//printf("%s ", reserved[expr->oper]);

		rightop_type = print_expression_prefix(expr->rightOperand);
		//cout << "\nRightop type is "<<rightop_type<<"\n";

		if (((leftop_type > UD) && (rightop_type > UD)) && (leftop_type != rightop_type))
		{
                update_builtin_id_type(rightop_type, leftop_type);	
		}

		else if (leftop_type > UD)
		{
            update_builtin_id_type(leftop_type, rightop_type);
            selected_type = rightop_type;
		}
		else if (rightop_type > UD) 
		{
            //cout << "Expected else if right op is greater than UD\n";
            update_builtin_id_type(rightop_type, leftop_type);
            selected_type = leftop_type;
   		}
	
		else if (leftop_type == rightop_type)
		{
			selected_type = leftop_type;
		}
	
		else if (leftop_type != rightop_type)
		{
			error_code_list.push_back(3);
		}

		
	} else	if (expr->tag == PRIMARY)
	{
		if (expr->primary->tag == ID)
		{
			//printf("%s ", expr->primary->id);
			selected_id = expr->primary->id;
			if(typevalue_to_typeid_map.count(selected_id) == 0)
            {
                typevalue_to_typeid_map[selected_id] = start_val + 1;
                start_val = start_val + 1;

                temp_id_list.clear();
                temp_id_list.push_back(selected_id);

                varsec_typeid_to_ids_list_map[start_val] = temp_id_list;

                temp_id_list.clear();
           
                if(var_decl_map.count(selected_id)==0)
                {
                    var_decl_map[selected_id] = 0;
                    var_implicit_list.push_back(selected_id);
                    var_implicit_pos_map[selected_id] = var_implicit_list.size();
                }
 
                   
                return start_val;
            }
            else
            {
                temp_typeid = typevalue_to_typeid_map[selected_id];
                return temp_typeid;
            }
		}
		else if (expr->primary->tag == NUM)
		{
			//printf("%d ", expr->primary->ival);
			selected_id = expr->primary->ival;
			return INT;
		}
		else if (expr->primary->tag == REALNUM)
		{
			//printf("%.4f ", expr->primary->fval);
			selected_id = expr->primary->fval;
			return REAL;
		}
	}
	return selected_type;
}

void update_builtin_id_type(int old_id, int new_id)
{

    if (old_id == new_id)
        return;

    temp_id_set.clear();
    temp_id_set_1.clear();

    
    //cout <<"\n old id is "<<old_id<<"\n";

    //cout<< "\n New id is"<<new_id<<"\n";


    //TYPESEC TYPID TO IDS LIST

	temp_id_list = typesec_typeid_to_ids_list_map[old_id];
	for(sl_it = temp_id_list.begin(); sl_it != temp_id_list.end(); sl_it++)
	{
        //cout << "\nfirst for looop\n";
		typevalue_to_typeid_map[*sl_it] = new_id;
        temp_id_set.insert(*sl_it);
	}	
    
    
    //VAR SEC TYPE ID TO IDS LIST
    //cout<<"\nExisting old one - type sec id set \n"<<old_id;
    //print_set(temp_id_set);
	temp_id_list = varsec_typeid_to_ids_list_map[old_id];
    //print_list(temp_id_list);
	for(sl_it_2 = temp_id_list.begin(); sl_it_2 != temp_id_list.end(); sl_it_2++)
	{
        //cout<<"\n old id value is \n"<<old_id<<" "<<*sl_it_2;
        //cout<< "\nsecond for looop"<<*sl_it_2<<"\n";
		typevalue_to_typeid_map[*sl_it_2] = new_id;
        temp_id_set_1.insert(*sl_it_2);
	}
	
    //cout<<"\nExisting old one - var sec id set\n"<<old_id;
    //print_set(temp_id_set_1);
    temp_id_set.insert(temp_id_set_1.begin(), temp_id_set_1.end());

    temp_id_set_1.clear();    

    //cout << "\nout of second for looop\n";


    //TYPE ID TO IDS SET - FOR OLD_ID

    temp_id_set_1 = typeid_to_ids_set_map[old_id];    
    temp_id_set.insert(temp_id_set_1.begin(), temp_id_set_1.end());
    temp_id_set_1.clear();    


    // DEALING WITH VALUES OF  NEW ID
    temp_id_set_1 = typeid_to_ids_set_map[new_id];

    //cout<<"\nExisting new one -  \n";
    //print_set(temp_id_set_1);

    temp_id_set.insert(temp_id_set_1.begin(), temp_id_set_1.end());
    typeid_to_ids_set_map[new_id] = temp_id_set;
 
    //cout<<"\nFinal combined new one \n"; 
    //print_set(temp_id_set);
 
    //print_set(temp_id_set);

    temp_id_set.clear();
    
    typeid_to_ids_set_map[old_id] = temp_id_set;

    temp_id_set_1.clear();

    //cout<<"\n------------------------------\n";
    //cout<< "\nEnd of update built in method\n";
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

struct conditionNode* make_conditionNode()
{
    return (struct conditionNode*) malloc(sizeof(struct conditionNode));
}

/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
  PARSING AND BUILDING PARSE TREE
---------------------------------------------------------------------*/
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

struct primaryNode* primary()
{
    //cout<<"\n Inside Primary node\n";

    struct primaryNode* primNode;

    ttype = getToken();
    //cout<<"\n Got TOken " << ttype <<"\n";
    if (ttype == ID)
    {
        primNode = make_primaryNode();
        primNode->tag = ID;
        primNode->id = (char *) malloc((tokenLength+1)*sizeof(char));
        strcpy(primNode->id, token);
        return primNode;
    }

    else if (ttype == NUM)
    {
        primNode = make_primaryNode();
        primNode->tag = NUM;
        primNode->ival = atoi(token);
        return primNode; 
    }
    else if (ttype == REALNUM)
    {
        primNode = make_primaryNode();
        primNode->tag = REALNUM;
        primNode->fval = atof(token);
        return primNode;
    }
    else
    {
        syntax_error("NUM, REALNUM or ID expected", line_no);
        exit(0);
    }
}


struct conditionNode* condition()
{
    struct conditionNode* condNode;

    //cout << "\n Inside condition node\n";

    condNode = make_conditionNode();
    ttype = getToken();

    if(ttype == ID || ttype == NUM || ttype == REALNUM)
    {
        ungetToken();
        //cout << "\ncalling primary for left op\n";
        condNode -> left_operand = primary();
        
        ttype = getToken();
        if(ttype == GREATER || ttype == GTEQ || ttype ==LESS || ttype == NOTEQUAL || ttype == LTEQ)
        {

            //cout<<"\nGot relop as" << ttype<<"\n";
            condNode->relop = ttype;
        }
        else
        {
            ungetToken();
            condNode->right_operand = NULL;
            return condNode;
        }    

        //cout << "\ncalling primary for right op\n";
            
        condNode -> right_operand = primary();
        
        //cout<< "\nreturning cond node\n"; 
        return condNode;
    }
    else
    {
        syntax_error("Bug in conidtion node parsing", line_no);
    }
}

struct while_stmtNode* while_stmt()
{
     struct while_stmtNode* whileStmt;
    
     whileStmt = make_while_stmtNode();     
  
     //cout << "\nInside while stmt\n";
 
     ttype = getToken();
     if (ttype == WHILE)
     { 
        //cout << "\nCalling condition\n";
        whileStmt->condition = condition();


        //cout<< "\ncalling body\n";
        whileStmt->body = body();
        return whileStmt; 
     }
     else
     {
        syntax_error("while_stmt. WHILE expected", line_no);
        exit(0);
     }
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
        return stm;	
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
		if (ttype == ID || ttype == WHILE)
		{	ungetToken();
			stmtList->stmt_list = stmt_list();
			return stmtList;
		} else	// If the next token is not in FOLLOW(stmt_list), 
			// let the caller handle it. 
		{	ungetToken();
            stmtList -> stmt_list = NULL;
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
	{	    
        //cout << "Getting "<<ttype<<"\n";
        syntax_error("body. LBRACE expected", line_no);
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
            
		    typesec_typename_to_idlist_map[temp_type_string] = temp_id_list;

            uo_typesec_map.push_back(make_pair(temp_type_string, temp_id_list));

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
	} 
	else
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

            if(varsec_typename_to_idlist_map.count(temp_type_string)>0)
            {
                old_id_list = varsec_typename_to_idlist_map[temp_type_string];
                for(sl_it = old_id_list.begin(); sl_it != old_id_list.end(); sl_it++)
                {
                    temp_id_list.push_back(*sl_it);
                }
            }
			varsec_typename_to_idlist_map[temp_type_string] = temp_id_list;

            uo_varsec_map.push_back(make_pair(temp_type_string, temp_id_list));

			temp_id_list.clear();	
			ttype = getToken();
			if (ttype == SEMICOLON)
			{	
                return varDecl;
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
            varDeclList->var_decl_list = NULL;
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
            typeDeclList->type_decl_list = NULL;
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
    		} 
        else
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
    		} 
            else
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


void play_with_order_ds()
{

    for(usl_it = uo_typesec_map.begin(); usl_it != uo_typesec_map.end(); usl_it++)
    {
        temp_typename = (*usl_it).first;
        if(type_decl_map.count(temp_typename)==0 && built_in_types_set.count(temp_typename)==1)
          {
                type_decl_map[temp_typename] = 1;
                type_explicit_list.push_back(temp_typename);

          }
        else if (type_decl_map.count(temp_typename)==0)
          {
                type_decl_map[temp_typename] = 0;
                type_implicit_list.push_back(temp_typename);
          }

            
        for(sl_it = (*usl_it).second.begin(); sl_it != (*usl_it).second.end(); sl_it++)
        {
            if(type_decl_map.count(*sl_it)==0)
                {
                    type_decl_map[*sl_it] = 1;
                    type_explicit_list.push_back(*sl_it);
                }
        }
    }



   for(usl_it = uo_varsec_map.begin(); usl_it != uo_varsec_map.end(); usl_it++)
   {
        temp_typename = (*usl_it).first; 
        if(type_decl_map.count(temp_typename)==0)
        {
            if(built_in_types_set.count(temp_typename) == 0)
            {
                type_decl_map[temp_typename] = 0;
                type_implicit_list.push_back(temp_typename);
            }
            else 
            {
                type_decl_map[temp_typename] =1;
                type_explicit_list.push_back(temp_typename);
            }

        }
        
        for(sl_it = (*usl_it).second.begin(); sl_it != (*usl_it).second.end(); sl_it++)
        {
            if(var_decl_map.count(*sl_it)==0)
            {
                var_decl_map[*sl_it] = 1;
                var_explicit_list.push_back(*sl_it);
            }

        }
   }

}


void play_with_ds()
{

	// Populate typsec_typelist and typesec_idlist 

	for(slm_it = typesec_typename_to_idlist_map.begin(); slm_it!=typesec_typename_to_idlist_map.end(); slm_it++)
	{

        temp_typename = (*slm_it).first;
		typesec_typelist.push_back(temp_typename);



        temp_typeid = typevalue_to_typeid_map[(*slm_it).first];
        
    			
		for(sl_it = (*slm_it).second.begin(); sl_it != (*slm_it).second.end(); sl_it++)
		{
			typesec_idlist.push_back(*sl_it);
			typesec_idset.insert(*sl_it);
            typevalue_to_typeid_map[*sl_it] = temp_typeid;

		}	
	}

	// Populate varsec_typelist and varsec_idlist

	for(slm_it = varsec_typename_to_idlist_map.begin(); slm_it!=varsec_typename_to_idlist_map.end(); slm_it++)
	{

        temp_typename = (*slm_it).first;
		varsec_typelist.push_back(temp_typename);

		
        temp_typeid = typevalue_to_typeid_map[(*slm_it).first];

		for(sl_it = (*slm_it).second.begin(); sl_it != (*slm_it).second.end(); sl_it++)
		{
			varsec_idlist.push_back(*sl_it);
			varsec_idset.insert(*sl_it);
            typevalue_to_typeid_map[*sl_it] = temp_typeid;


		}
	}
}


void print_order_ds()
{

    //cout<<"UO TYPESEC MAP \n";
    for(usl_it = uo_typesec_map.begin(); usl_it != uo_typesec_map.end(); usl_it++)
    {
        //cout<<((*usl_it).first)<<" :";

        for(sl_it = (*usl_it).second.begin(); sl_it != (*usl_it).second.end(); sl_it++)
        {
            //cout<<(*sl_it)<<" ,";
        }
        //cout<<"\n";
    
    }
    //cout<<"\n\n";

    //cout<<"UO VARSEC MAP \n";
    for(usl_it = uo_varsec_map.begin(); usl_it != uo_varsec_map.end(); usl_it++)
    {
        //cout<<((*usl_it).first)<<" :";

        for(sl_it = (*usl_it).second.begin(); sl_it != (*usl_it).second.end(); sl_it++)
        {
            //cout<<(*sl_it)<<" ,";
        }
        //cout<<"\n";
    
    }
    //cout<<"\n\n";
}

void print_ds()
{
	// Print typsec_typelist and typesec_idlist 

	//cout<< "After first for loop in print ds"<<"\n";
    //cout << "\nType sec typename to idlist map\n";
	for(slm_it = typesec_typename_to_idlist_map.begin(); slm_it!=typesec_typename_to_idlist_map.end(); slm_it++)
	{
		//cout<<((*slm_it).first)<<" :";
			
		for(sl_it = (*slm_it).second.begin(); sl_it != (*slm_it).second.end(); sl_it++)
		{
			//cout<<(*sl_it)<<" ,";			
		}
		//cout<<"\n";	
	}
	//cout<<"\n\n";

	// Print varsec_typelist and varsec_idlist
	//cout<< "After second for loop in print ds"<<"\n";
    //cout << "\nVar sec typename to idlist map\n";
	for(slm_it = varsec_typename_to_idlist_map.begin(); slm_it!=varsec_typename_to_idlist_map.end(); slm_it++)
	{
		//cout<<((*slm_it).first)<<" :";
		
		for(sl_it = (*slm_it).second.begin(); sl_it != (*slm_it).second.end(); sl_it++)
		{
			//cout<<(*sl_it)<<" ,";
		}
		//cout<<"\n";
	}
	//cout<<"\n\n";


	//Print typevalue_to_typeid_map
	//cout<< "After third for loop in print ds"<<"\n";
	
    //cout<< "\n Typevalue to typeid map\n";
	for(nid_it = typevalue_to_typeid_map.begin(); nid_it != typevalue_to_typeid_map.end(); nid_it++)
	{
		//cout<<(*nid_it).first<<"   "<<(*nid_it).second<<" "<<"\n";
	}	
	//cout<<"\n\n";
}



void check_for_error_typesec()
{
	//cout<< "\n"<<"Check for error type section"<<"\n";
	for(sl_it = typesec_typename_order_list.begin(); sl_it != typesec_typename_order_list.end(); sl_it++)
	{
		temp_typename = *sl_it;
		temp_typeid = typevalue_to_typeid_map[temp_typename];

		seen_typesec_typename_set.insert(temp_typename);

		//check for error code 0
		temp_id_list = typesec_typename_to_idlist_map[temp_typename];
		for(sl_it_2 = temp_id_list.begin(); sl_it_2 != temp_id_list.end(); sl_it_2++)
		{
			if (seen_typesec_ids_set.count(*sl_it_2)!=0)
			{
				error_code_list.push_back(0);
                cout<< "ERROR CODE 0\n";
				exit(0);
			}
			seen_typesec_ids_set.insert(*sl_it_2);
		}
	
	
		//check for error code 1
		if ((seen_typesec_ids_set.count(temp_typename)==0) && (typesec_idset.count(temp_typename)>0))

		{
			error_code_list.push_back(1);
            cout<< "ERROR CODE 0\n";
			exit(0);

		}

	} 
}


void check_for_error_varsec()
{
	//cout<<"\n"<<"check for error var sec"<<"\n";

	for(sl_it = varsec_typename_order_list.begin(); sl_it!=varsec_typename_order_list.end(); sl_it++)
	{
		//cout<<"\n"<<"Inside for loop of check error for var sec"<<*sl_it<<"\n";
		temp_typename = *sl_it;
		temp_typeid = typevalue_to_typeid_map[temp_typename];

        if(seen_varsec_typename_set.count(temp_typename)>0)
        {
            continue;
        }
	

		//check for error code 1
		if ((seen_varsec_ids_set.count(temp_typename)==0) && (varsec_idset.count(temp_typename)>0))

		{
			error_code_list.push_back(1);
            cout<<"ERROR CODE 1\n";
            exit(0);

		}

		//check for error code 2
		temp_id_list = varsec_typename_to_idlist_map[temp_typename];
		for(sl_it_2 = temp_id_list.begin(); sl_it_2 != temp_id_list.end(); sl_it_2++)
		{
			if (seen_varsec_ids_set.count(*sl_it_2)!=0)
			{
				error_code_list.push_back(2);
                cout<<"ERROR CODE 2\n";
                exit(0);
			}
			if (seen_typesec_typename_set.count(*sl_it_2)>0 || seen_typesec_ids_set.count(*sl_it_2)>0)
			{
				error_code_list.push_back(1);
                cout<<"ERROR CODE 1\n";
                exit(0);
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
	//cout << "typeconversion part";
	//TYPE results table manipulation

	
	for(sl_it = typesec_typename_order_list.begin(); sl_it != typesec_typename_order_list.end(); sl_it++)
	{
		temp_typename = *sl_it;
		temp_typeid = typevalue_to_typeid_map[temp_typename];

		if (temp_typeid==ID || temp_typeid <14)
		{
            if (temp_typeid==ID)
            {
                new_val = start_val + 1;
                start_val = new_val;
                typevalue_to_typeid_map[temp_typename] = new_val;
		    }

            else 
            {
                new_val = temp_typeid;

            }

			//update this change to rest of the children
			temp_id_list = typesec_typename_to_idlist_map[temp_typename];
			for(sl_it_2 = temp_id_list.begin(); sl_it_2 != temp_id_list.end(); sl_it_2++)
			{
				typevalue_to_typeid_map[*sl_it_2] = new_val;
            
                if(typesec_typename_to_idlist_map.count(*sl_it_2)>0)
                {
                    temp_id_list_2 = typesec_typename_to_idlist_map[*sl_it_2];
                    for(sl_it_3 = temp_id_list_2.begin(); sl_it_3 != temp_id_list_2.end(); sl_it_3++)
                    {
                        typevalue_to_typeid_map[*sl_it_3] = new_val;
                    }
                }

                if (varsec_typename_to_idlist_map.count(*sl_it_2)>0)
                {
                    temp_id_list_2 = varsec_typename_to_idlist_map[*sl_it_2];
                    for(sl_it_3 = temp_id_list_2.begin(); sl_it_3 != temp_id_list_2.end(); sl_it_3++)
                    {
                        typevalue_to_typeid_map[*sl_it_3] = new_val;
                    }
                }
		    }
	   }
    }
}


void get_typesec_typeid_to_ids_list_map()
{
	for(slm_it = typesec_typename_to_idlist_map.begin(); slm_it != typesec_typename_to_idlist_map.end(); slm_it++)
	{
		temp_typename = (*slm_it).first;
		temp_id_list = (*slm_it).second;
		temp_id_list.push_back(temp_typename);

		temp_typeid = typevalue_to_typeid_map[temp_typename]; 
		
		if(typesec_typeid_to_ids_list_map.count(temp_typeid)>0)
		{
			old_id_list = typesec_typeid_to_ids_list_map[temp_typeid];
			for (sl_it = old_id_list.begin(); sl_it != old_id_list.end(); sl_it++)
			{
				temp_id_list.push_back(*sl_it);
			}
		}


		typesec_typeid_to_ids_list_map[temp_typeid] = temp_id_list; 
        
        typeid_to_ids_list_map[temp_typeid] = temp_id_list;

		temp_id_set.clear();
		for(sl_it_2 = temp_id_list.begin(); sl_it_2 != temp_id_list.end(); sl_it_2++)
		{
			temp_id_set.insert(*sl_it_2);
		}

		if(typeid_to_ids_set_map.count(temp_typeid)>0)
		{
			old_id_set = typeid_to_ids_set_map[temp_typeid];
			for(ss_it = old_id_set.begin(); ss_it != old_id_set.end(); ss_it++)
			{
				temp_id_set.insert(*ss_it);
			}
		}

		typeid_to_ids_set_map[temp_typeid] = temp_id_set;

	}
}


void get_varsec_typeid_to_ids_list_map()
{
	for(slm_it = varsec_typename_to_idlist_map.begin(); slm_it != varsec_typename_to_idlist_map.end(); slm_it++)
	{
		temp_typename = (*slm_it).first;
		temp_id_list = (*slm_it).second;
		temp_id_list.push_back(temp_typename);

		temp_typeid = typevalue_to_typeid_map[temp_typename]; 
		
		if(varsec_typeid_to_ids_list_map.count(temp_typeid)>0)
		{
			old_id_list = varsec_typeid_to_ids_list_map[temp_typeid];
			for (sl_it = old_id_list.begin(); sl_it != old_id_list.end(); sl_it++)
			{
				temp_id_list.push_back(*sl_it);
			}
		}

		varsec_typeid_to_ids_list_map[temp_typeid] = temp_id_list; 

 
		temp_id_set.clear();

		for(sl_it_2 = temp_id_list.begin(); sl_it_2 != temp_id_list.end(); sl_it_2++)
		{
			temp_id_set.insert(*sl_it_2);
		}

		if(typeid_to_ids_set_map.count(temp_typeid)>0)
		{
			old_id_set = typeid_to_ids_set_map[temp_typeid];
			for(ss_it = old_id_set.begin(); ss_it != old_id_set.end(); ss_it++)
			{
				temp_id_set.insert(*ss_it);
			}
		}

		typeid_to_ids_set_map[temp_typeid] = temp_id_set;
	}
}


void print_new_maps()
{

    //cout << "\ntypesec_typeid_to_ids_list_map\n";

	for(ilm_it = typesec_typeid_to_ids_list_map.begin(); ilm_it != typesec_typeid_to_ids_list_map.end(); ilm_it++)
	{
		//cout<< (*ilm_it).first<<":";
		
		temp_id_list = (*ilm_it).second;
		for (sl_it = temp_id_list.begin(); sl_it != temp_id_list.end(); sl_it++)
		{
			//cout<< (*sl_it)<<",";
		}
		//cout<<"\n";
	}

    //cout << "\n";


    //cout<<"varsec_typeid_to_ids_list_map\n";
	for(ilm_it = varsec_typeid_to_ids_list_map.begin(); ilm_it != varsec_typeid_to_ids_list_map.end(); ilm_it++)
	{
		//cout<< (*ilm_it).first<<":";
		
		temp_id_list = (*ilm_it).second;
		for (sl_it = temp_id_list.begin(); sl_it != temp_id_list.end(); sl_it++)
		{
			//cout<< (*sl_it)<<",";
		}
		//cout<<"\n";
	}

	//cout<<"\n";

    //cout<<"typeid_to_ids_set_map\n";
	for(ism_it = typeid_to_ids_set_map.begin(); ism_it != typeid_to_ids_set_map.end(); ism_it++)
	{
		//cout<< (*ism_it).first<<":";
		
		old_id_set = (*ism_it).second;
		for (ss_it = old_id_set.begin(); ss_it != old_id_set.end(); ss_it++)
		{
			//cout<< (*ss_it)<<",";
		}
		//cout<<"\n";
	}

    //cout <<"\n";
}

void get_new_maps()
{
	get_typesec_typeid_to_ids_list_map();
	get_varsec_typeid_to_ids_list_map();
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
			new_val = start_val + 1;
			start_val = new_val;
			typevalue_to_typeid_map[temp_typename] = new_val;
		
			//update this change to rest of the vaues having this type
			temp_id_list = varsec_typename_to_idlist_map[temp_typename];
			for(sl_it_2 = temp_id_list.begin(); sl_it_2 != temp_id_list.end(); sl_it_2++)
			{
				typevalue_to_typeid_map[*sl_it_2] = new_val;
                
                if(seen_varsec_typename_set.count(*sl_it_2)>0)
                 {
                    temp_id_list_2 = varsec_typename_to_idlist_map[*sl_it_2];
                    
                    for(sl_it_3 = temp_id_list_2.begin(); sl_it_3 != temp_id_list.end(); sl_it_3++)
                    {
                        typevalue_to_typeid_map[*sl_it_3] = new_val;
                    }

                }

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
	for(il_it=error_code_list.begin(); il_it!=error_code_list.end(); il_it++)
	{
		//cout<<"ERROR CODE "<<*il_it<<"\n";
        exit(0);
	}
}

void check_for_error()
{
    seen_typesec_typename_set.clear();
    seen_varsec_typename_set.clear();
    seen_typesec_ids_set.clear();
    seen_varsec_ids_set.clear();

	check_for_error_typesec();
	check_for_error_varsec();
	//print_error();
}

void generate_final_output()
{

    for(sl_it = typesec_typename_order_list.begin(); sl_it != typesec_typename_order_list.end(); sl_it++)
    {
        temp_typename = *sl_it;
        temp_typeid = typevalue_to_typeid_map[temp_typename]; 
        
        temp_id_set = typeid_to_ids_set_map[temp_typeid];
        
        output_map[temp_typename] = temp_id_set;
    }

    for(sl_it = varsec_typename_order_list.begin(); sl_it != varsec_typename_order_list.end(); sl_it++)
    {
        temp_typename = *sl_it;

        //cout << "temp typname "<<temp_typename<<"\n";
        temp_typeid = typevalue_to_typeid_map[temp_typename];
        
        //cout<< "temp type id "<<temp_typeid<<"\n";        

        temp_id_set = typeid_to_ids_set_map[temp_typeid];

        if (output_map.count(temp_typename)>0)
        {
            temp_id_set_1 = output_map[temp_typename];
           
            temp_id_set_1.insert(temp_id_set.begin(), temp_id_set.end());
            
            output_map[temp_typename] = temp_id_set_1;
        }
        else
        {
            output_map[temp_typename] = temp_id_set;

        }
    }

    //print_list(varsec_typename_order_list);
    //print_list(typesec_typename_order_list);

}


void print_final_output()
{
    published_op_set.clear();
    for(vec_it = output_vector.begin(); vec_it != output_vector.end(); vec_it++)
    {

        if(published_op_set.count((*vec_it).first)>0)
        {
            continue;
        }

        cout<< (*vec_it).first<<" : ";

        published_op_set.insert((*vec_it).first);

        temp_id_list = (*vec_it).second;
        for (sl_it = temp_id_list.begin(); sl_it!= temp_id_list.end(); sl_it++)
        {

            if( built_in_types_set.count(*sl_it) == 0)
            {
                cout<<*sl_it<<" ";
                published_op_set.insert(*sl_it);
            }
        }
        cout<< "#\n";
    }
    cout<< "\n";
}

void print_set(set<string> temp_id_set)
{
    //cout<<"\n\nPrinting temp id set\n";
    for(ss_it = temp_id_set.begin(); ss_it != temp_id_set.end(); ss_it++)
    {
        cout<<" "<<*ss_it<<" ";
    }
    //cout<<"\n\n";

}

void print_list(list<string> temp_id_list)
{

    //cout<<"\n Printing temp id list\n";
    for(sl_it = temp_id_list.begin(); sl_it != temp_id_list.end(); sl_it++)
    {
        //cout<<" "<<*sl_it<<" ";
    }
    //cout<<"\n";

}

void order_output()
{

    // FOR BUILTIN TYPES

    output_vector.clear();

    for(sl_it_2 = built_in_types_list.begin(); sl_it_2 != built_in_types_list.end(); sl_it_2++)
    {
        temp_typename = *sl_it_2;
        if(output_map.count(temp_typename)>0)
        {
            temp_id_set = output_map[temp_typename];
            temp_id_set.erase(temp_typename);
            //print_set(temp_id_set);
            temp_id_list = order_id_list(temp_id_set);
            //print_list(temp_id_list);
            output_vector.push_back(make_pair(temp_typename, temp_id_list)); 
        }
    }


    // TYPE IMPLICIT 

    for(sl_it_2 = type_implicit_list.begin(); sl_it_2!= type_implicit_list.end(); sl_it_2++)
    {
        temp_typename = *sl_it_2;
        if(output_map.count(temp_typename)>0)
        {
            temp_id_set = output_map[temp_typename];
            temp_id_set.erase(temp_typename);
            temp_id_list = order_id_list(temp_id_set);

             output_vector.push_back(make_pair(temp_typename, temp_id_list)); 
        }
    }

    // VAR IMPLICIT
    
    for(sl_it_2 = var_implicit_list.begin(); sl_it_2 != var_implicit_list.end(); sl_it_2++)
    {
       
        temp_typename = *sl_it_2;
        if(output_map.count(temp_typename)>0)
        {
            temp_id_set = output_map[temp_typename];
            temp_id_set.erase(temp_typename);
            temp_id_list = order_id_list(temp_id_set);

             output_vector.push_back(make_pair(temp_typename, temp_id_list)); 
        }

    }
}

list<string> order_id_list(set<string> temp_id_set)
{

    //cout<< "\n Entered into order id list\n";
    temp_id_list.clear();
    
    for(sl_it = type_explicit_list.begin(); sl_it != type_explicit_list.end(); sl_it++)
    {
        if(temp_id_set.count(*sl_it)>0 && built_in_types_set.count(*sl_it)==0)
            {
                temp_id_list.push_back(*sl_it);
            }
        //cout<<"\n type explicit\n";

    }

    for(sl_it = type_implicit_list.begin(); sl_it != type_implicit_list.end(); sl_it++)
    {
        if(temp_id_set.count(*sl_it)>0)
        {
            temp_id_list.push_back(*sl_it);
        }
       //cout<<"\n type implicit\n";
    }

    for(sl_it = var_explicit_list.begin(); sl_it != var_explicit_list.end(); sl_it++)
    {
        if(temp_id_set.count(*sl_it)>0)
        {
            temp_id_list.push_back(*sl_it);
        }
        //cout<<"\n var explicit\n";
    }

    for(sl_it = var_implicit_list.begin(); sl_it != var_implicit_list.end(); sl_it++)
    {
        if(temp_id_set.count(*sl_it)>0)
        {
            temp_id_list.push_back(*sl_it);
        }
        //cout<<"\n var implicit\n";
    }
   
    //cout<<"\nReturning from order id lsit\n"; 
    return temp_id_list;
}

void fill_typevalues_for_built_in()
{

typevalue_to_typeid_map["INT"]= INT;
typevalue_to_typeid_map["REAL"]= REAL;
typevalue_to_typeid_map["STRING"]= STRING;
typevalue_to_typeid_map["BOOLEAN"]= BOOLEAN;
}

void generate_output_content()
{

    output_map.clear();
    seen_id_set.clear();


    fill_typevalues_for_built_in();
   // FOR BUILT IN TYPES
    for(sl_it = built_in_types_list.begin(); sl_it != built_in_types_list.end(); sl_it++)
    {
        temp_typename = *sl_it;
        temp_typeid = typevalue_to_typeid_map[temp_typename];
        temp_id_set = typeid_to_ids_set_map[temp_typeid];
        
        seen_id_set.insert(temp_typeid);   
    
        if (temp_typeid ==0)
        {
            //cout<<"\nvalue is 0 for"<<temp_typename<<"\n";
            continue;
         } 
           
        if (temp_id_set.size()>0)
            output_map[temp_typename] = temp_id_set;        
    }

   
    //print_output_map();
 
    //FOR TYPE IMPLICIT

    for(sl_it = type_implicit_list.begin(); sl_it != type_implicit_list.end(); sl_it++)
    {
        temp_typename = *sl_it;
        temp_typeid = typevalue_to_typeid_map[temp_typename];

        if(seen_id_set.count(temp_typeid)==0)
        {
            temp_id_set = typeid_to_ids_set_map[temp_typeid];
            seen_id_set.insert(temp_typeid);        
            if (temp_id_set.size()>0)
                output_map[temp_typename] = temp_id_set;        
        } 
        if (temp_typeid ==0)
            continue;
            
    }

    //print_output_map();

    //FOR VAR IMPLICIT
    for(sl_it = var_implicit_list.begin(); sl_it != var_implicit_list.end(); sl_it++)
    {

        temp_typename = *sl_it;
        temp_typeid = typevalue_to_typeid_map[temp_typename];
        if(seen_id_set.count(temp_typeid)==0)
        {
            temp_id_set = typeid_to_ids_set_map[temp_typeid];
            seen_id_set.insert(temp_typeid);        
            if (temp_id_set.size()>0)
                output_map[temp_typename] = temp_id_set;        
        }
        if (temp_typeid ==0)
            continue;

    }
    
    //print_output_map();

}


void format_output()
{
    //generate_final_output();
	//print_new_maps();
    generate_output_content();
    //print_output_map();
    order_output();
    print_final_output();
    //print_output_map();
}


void populate_built_in_types_set()
{
    built_in_types_set.insert("INT");
    built_in_types_set.insert("REAL");
    built_in_types_set.insert("STRING");
    built_in_types_set.insert("BOOLEAN");

    built_in_types_list.push_back("INT");
    built_in_types_list.push_back("REAL");
    built_in_types_list.push_back("STRING");
    built_in_types_list.push_back("BOOLEAN");
}

void print_order_explicit_or_implicit_info()
{
    //PRINT type_decl_map

    //cout<<"\n TYPE DECL MAP";
    for(nid_it = type_decl_map.begin(); nid_it != type_decl_map.end(); nid_it++)
     {
        temp_typename = (*nid_it).first; 
        //cout<<"\n"<<temp_typename<<" : "<< (*nid_it).second;

     } 

    //PRINT var_decl_map

    //cout<<"\n VAR DECL MAP";
    for(nid_it = var_decl_map.begin(); nid_it != var_decl_map.end(); nid_it++)
    {
        //cout<<"\n"<<(*nid_it).first<<" : "<<(*nid_it).second;
    }


    //PRINT type_explicit_list

    //cout<<"\n TYPE EXPLICIT LIST";
    for(sl_it = type_explicit_list.begin(); sl_it != type_explicit_list.end(); sl_it++)
    {
        //cout<<" "<<*sl_it<<" ";
    }
    //cout<<"\n";

    //PRINT type_implicit_list
    //cout<<" TYPE IMPLICIT LIST";
    for(sl_it = type_implicit_list.begin(); sl_it != type_implicit_list.end(); sl_it++)
    {
        //cout<<" "<<*sl_it<<" ";
    }
    //cout<<"\n";

    //PRINT var_explicit_list
    //cout<<"VAR EXPLICIT LIST";
    for(sl_it = var_explicit_list.begin(); sl_it != var_explicit_list.end(); sl_it++)
    {
        //cout<<" "<<*sl_it<<" ";
    }
    //cout<<"\n";

    //PRINT var_implicit_list
    //cout<<"VAR IMPLICIT LIST";
    for(sl_it = var_implicit_list.begin(); sl_it != var_implicit_list.end(); sl_it++)
    {
        //cout<<" "<<*sl_it<<" ";
    }
    //cout<<"\n";
}

void print_order_list()
{
    //cout<<"PRINTING TYPESEC TYPENAME ORDER LIST";
    for(sl_it = typesec_typename_order_list.begin(); sl_it != typesec_typename_order_list.end(); sl_it++)
    {
        //cout<< "  "<<*sl_it<<" ";
    }
    //cout<< "\n";

    //cout<<"PRINTING VARSEC TYPENAME ORDER LIST";
    for(sl_it = varsec_typename_order_list.begin(); sl_it != varsec_typename_order_list.end(); sl_it++)
    {
        //cout<<" "<<*sl_it<<" ";
    }
    //cout<<"\n";

}

void print_output_map()
{

    //cout<<"\nOutput map \n";
    for(ssm_it = output_map.begin(); ssm_it != output_map.end(); ssm_it++)
    {
        //cout<<"\n"<< (*ssm_it).first<<" : ";
        for(ss_it = (*ssm_it).second.begin(); ss_it != (*ssm_it).second.end(); ss_it++)
            {
                //cout<<" "<<*ss_it<<" ";
            }
    }
    //cout<<"\n\n";

}

int main()
{
	struct programNode* parseTree;
	parseTree = program();

	/****POPULATING DATA STRUCTURES***/
    populate_built_in_types_set();
	play_with_ds();	
	copy_varsec_typename_order_list();
    //print_ds();
	check_for_error();

	/****TYPE CONVERSIONS****/
	type_typeconversion();
    var_typeconversions();
    //print_ds();

	/***POPULATING DATA STRUCTURES***/
	get_new_maps();
	//print_new_maps();

	/****BODY TYPECHECK****/
    print_parse_tree(parseTree);
	//check_for_error();
	//print_new_maps();
    //print_ds();
    //print_order_list();
    //print_order_ds();
    play_with_order_ds();
    //print_order_explicit_or_implicit_info();    

    /*****OUTPUT FORMATTING*****/
    format_output();

	//printf("\nSUCCESSFULLY PARSED INPUT!\n");
	return 0;
}

