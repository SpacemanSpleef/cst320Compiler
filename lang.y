%code requires {
    class cAstNode;
    class cProgramNode;
    class cBlockNode;
    class cStmtsNode;
    class cPrintNode;
    class cExprNode;
    class cIntExprNode;
    class cFloatExprNode;
    class cSymbol;
    class cBinaryExprNode;
    class cVarDeclNode;
    class cVarRefNode;
    struct symbolTable_t;
    class cIfStmtNode;
    class cReturnNode;
    class cWhileNode;
    class cAssignmentNode;
    class cStructDeclNode;
    class cFuncDeclNode;
    class cFuncCallNode;
    class cParamsNode;
    class cArrayDeclNode;
    class cPrintsNode;
    class cIndexNode;
}

%{
//**************************************
// lang.y
//
// Parser definition file. bison uses this file to generate the parser.
//
// Author: Phil Howard 
//

#include <iostream>
#include "lex.h"
#include "astnodes.h"
#include "cSymbolTable.h"

#define CHECK_ERROR() { if (g_semanticErrorHappened) \
    { g_semanticErrorHappened = false; } }
#define PROP_ERROR() { if (g_semanticErrorHappened) \
    { g_semanticErrorHappened = false; YYERROR; } }

%}
%left '.' '['
%locations
%debug 

/* union defines the type for lexical values */
%union{
    int             int_val;
    float           float_val;
    std::string*    str_val;
    cAstNode*       ast_node;
    cProgramNode*   program_node;
    cBlockNode*     block_node;
    cStmtsNode*     stmts_node;
    cStmtNode*      stmt_node;
    cExprNode*      expr_node;
    cIntExprNode*   int_node;
    cSymbol*        symbol;
    cFloatExprNode* float_node;
    int             char_val;
    cDeclNode*      decl_node;
    cDeclsNode*     decls_node;
    symbolTable_t*  sym_table;
    cVarRefNode*    var_ref;
    cParamsNode*    param_ref;
    cFuncDeclNode*  func_decl;
    cFuncCallNode*  func_call;
    cArrayDeclNode* array_decl;
    }

%{
    int yyerror(const char *msg);
    static bool g_semanticErrorHappened = false;
    cAstNode *yyast_root;
%}

%start  program

%token <symbol>    IDENTIFIER
%token <symbol>    TYPE_ID
%token <int_val>   CHAR_VAL
%token <int_val>   INT_VAL
%token <float_val> FLOAT_VAL
%token <int_val>   LE
%token <int_val>   GE
%token <int_val>   AND
%token <int_val>   OR
%token <int_val>   EQUALS
%token <int_val>   NOT_EQUALS
%token <str_val>   STRING_LIT

%token  PROGRAM
%token  PRINT PRINTS
%token  WHILE IF ELSE ENDIF
%token  STRUCT ARRAY
%token  RETURN
%token  JUNK_TOKEN

%type <program_node> program
%type <block_node> block
%type <sym_table> open
%type <sym_table> close
%type <decls_node> decls
%type <decl_node> decl
%type <decl_node> var_decl
%type <decl_node> struct_decl
%type <array_decl> array_decl
%type <func_decl> func_decl
%type <func_decl> func_header
%type <func_decl> func_prefix
%type <func_call> func_call
%type <param_ref> paramsspec
%type <ast_node> paramspec
%type <stmts_node> stmts
%type <stmt_node> stmt
%type <ast_node> lval
%type <param_ref> params
%type <ast_node> param
%type <expr_node> expr
%type <expr_node> addit
%type <expr_node> term
%type <expr_node> fact
%type <var_ref> varref
%type <symbol> varpart

%%

program: PROGRAM block
                                { 
                                    $$ = new cProgramNode($2);
                                  yyast_root = $$;
                                  if (yynerrs == 0) 
                                      YYACCEPT;
                                  else
                                      YYABORT;
                                }
block:  open decls stmts close
                                { 
                                    $$ = new cBlockNode($2, $3);
                                }
    |   open stmts close
                                { $$ = new cBlockNode(nullptr, $2); }

open:   '{'
                                {  $$ = g_symbolTable.IncreaseScope(); }

close:  '}'
                                { $$ = g_symbolTable.DecreaseScope(); }

decls:      decls decl
                                { 
                                    $$ = $1;
                                    $$->Insert($2);
                                 }
        |   decl
                                { $$ = new cDeclsNode($1); }
decl:       var_decl ';'
                                { $$ = $1; }
        |   array_decl ';'
                            { $$ = $1; }
        |   struct_decl ';'
                            { $$ = $1; }
        |   func_decl
                            { $$ = $1; }
        |   error ';'
                            {  }

var_decl:   TYPE_ID IDENTIFIER
                                    { 
                                        g_symbolTable.Insert($2);
                                        $$ = new cVarDeclNode($1, g_symbolTable.FindLocal($2->GetName())); 
                                    }
struct_decl:  STRUCT open decls close IDENTIFIER
                                { 
                                        g_symbolTable.Insert($5);
                                        $$ = new cStructDeclNode($5, $3);                                
                                }
array_decl:   ARRAY TYPE_ID '[' INT_VAL ']' IDENTIFIER
                                {  
                                    $$ = new cArrayDeclNode($2, $4, $6);
                                }

func_decl:  func_header ';'
            { 
                g_symbolTable.DecreaseScope(); // Close scope opened in func_prefix
                $$ = $1; 
            }
        |   func_header '{' decls stmts '}'
            { 
                cBlockNode* body = new cBlockNode($3, $4);
                $1->AddBody(body);
                g_symbolTable.DecreaseScope(); // Close scope opened in func_prefix
                $$ = $1;    
            }
        |   func_header '{' stmts '}'
            { 
                cBlockNode* body = new cBlockNode(nullptr, $3);
                $1->AddBody(body);
                g_symbolTable.DecreaseScope(); // Close scope opened in func_prefix
                $$ = $1;
            }

func_header: func_prefix paramsspec ')'
                                { 
                                    $1->AddParams($2);
                                    $$ = $1; 
                                }
        |    func_prefix ')'
                            { $$ = $1; }
func_prefix: TYPE_ID IDENTIFIER '('
                                { 
                                    g_symbolTable.Insert($2);
                                    cSymbol* funcSym = g_symbolTable.FindLocal($2->GetName());
                                    g_symbolTable.IncreaseScope();
                                    $$ = new cFuncDeclNode($1, funcSym);
                                }
paramsspec:  paramsspec ',' paramspec
                                { 
                                    $$ = $1;
                                    $$->AddParam($3); 
                                }
        |   paramspec
                            { 
                                $$ = new cParamsNode($1);
                            }

paramspec: TYPE_ID IDENTIFIER
    {
        g_symbolTable.Insert($2);
        cSymbol* sym = g_symbolTable.FindLocal($2->GetName());
        $$ = new cVarDeclNode($1, sym);
    }
stmts:      stmts stmt
                                { 
                                    $$ = $1;
                                    $$->Insert($2);
                                }
        |   stmt
                            { $$ = new cStmtsNode($1); }
        | decl 
        {}

stmt:       IF '(' expr ')' stmts ENDIF ';'
                                { $$ = new cIfStmtNode($3, $5); }
        |   IF '(' expr ')' stmts ELSE stmts ENDIF ';'
                                { $$ = new cIfStmtNode($3, $5, $7); }
        |   WHILE '(' expr ')' stmt
                                { $$ = new cWhileNode($3, $5); }
        |   PRINT '(' expr ')' ';'
                                { $$ = new cPrintNode($3); }
        |   PRINTS '(' STRING_LIT ')' ';'
                                { $$ = new cPrintsNode($3); }
        |   lval '=' expr ';'
                            { $$ = new cAssignmentNode($1, $3); }
        |   func_call ';'
                            {  $$ = $1; }
        |   block
                            {  }
        |   RETURN expr ';'
                            { $$ = new cReturnNode($2); }
        |   error ';'
                            {}

func_call:  IDENTIFIER '(' params ')'
                                    { 
                                        cSymbol* sym = g_symbolTable.Find($1->GetName());
                                        if(sym == nullptr)
                                        {
                                            yyerror("Function not declared");
                                        }
                                        $$ = new cFuncCallNode(sym, $3);
                                    }
        |   IDENTIFIER '(' ')'
                            { 
                                        cSymbol* sym = g_symbolTable.Find($1->GetName());
                                        if(sym == nullptr)
                                        {
                                            yyerror("Function not declared");
                                        }
                                        $$ = new cFuncCallNode(sym, nullptr);

                             }

varref:   varref '.' varpart
                                {
                                    $$ = $1;
                                    $$->AddMember($3);
                                }
        | varref '[' expr ']'
                            { $$ = new cIndexNode($1, $3); }
        | varpart
                            { $$ = new cVarRefNode($1); }

varpart:  IDENTIFIER
                                { $$ = $1;  }
        | TYPE_ID 
            {$$ = $1; }
lval:     varref
                                { $$ = $1; }

params:   params ',' param
                                { $$ = $1;
                                    $$->AddParam($3);
                                 }
        |   param
                            { $$ = new cParamsNode($1); }

param:      expr
                                { $$ = $1; }

expr:       expr EQUALS addit
                                { $$ = new cBinaryExprNode($1, EQUALS, $3); }
        |   addit
                            { $$ = $1; }

addit:      addit '+' term
                                { 
                                  $$ = new cBinaryExprNode($1, '+', $3);  
                                }
        |   addit '-' term
                            {  $$ = new cBinaryExprNode($1, '-', $3); }
        |   term
                            { $$ = $1; }

term:       term '*' fact
                                { 
                                   $$ = new cBinaryExprNode($1, '*', $3);  

                                }
        |   term '/' fact
                            { 
                               $$ = new cBinaryExprNode($1, '/', $3);  
                            }
        |   term '%' fact
                            { 
                                $$ = new cBinaryExprNode($1, '%', $3);  
                            }
        |   fact
                            { $$ = $1; }

fact:       '(' expr ')'
                                { $$ = $2;}
        |   INT_VAL
                            { $$ = new cIntExprNode($1); }
        |   FLOAT_VAL
                            { $$ = new cFloatExprNode($1); }
        |   varref
                            {  $$ = $1;}
        |   func_call
                            { $$ = dynamic_cast<cExprNode*>($1); }

%%

// Function to format error messages
int yyerror(const char *msg)
{
    std::cerr << "ERROR: " << msg << " at symbol "
        << yytext << " on line " << yylineno << "\n";

    return 0;
}


void SemanticParseError(string error)
{
    std::cout << "ERROR: " << error << " near line " << yylineno << "\n";
    g_semanticErrorHappened = true;
    yynerrs++;
}