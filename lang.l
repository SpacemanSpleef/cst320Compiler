%{
//**************************************
// lang.l
//
// Scanner definition file
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

// Illustration showing how to use {} to control scope
// NOTE: this is pseudo-code illustrating how you can adapt your current lang.l

#include "cSymbol.h"
#include "cSymbolTable.h"
#include "lex.h"
#include "tokens.h"

// Macros that can be used to create debug output from the scanner
// Uncomment the next line of code to generate debug output.


int Return(int val);
static int ProcessIdentifier(char * identifier);
%}

%option noyywrap
%option noinput
%option nounput
%option yylineno

    /* definitions go here. whitespace is an example. */
whitespace      '[\t\n\r" " ]'
digit           [0-9]
alphabet        [a-z]

%%
{whitespace}+               {} /* Ignore whitespace */
"//".*$ {} /*Ignore comments*/

program   return PROGRAM;
if return IF;
else return ELSE;
endif return ENDIF;
while return WHILE;
print return PRINT;
prints return PRINTS;
char return CHAR;
int return INT;
long return LONG;
float return FLOAT;
double return DOUBLE;
struct return STRUCT;
array return ARRAY;
return return RETURN;

"!=" return NOT_EQUALS;
"==" return EQUALS;
"&&" return AND;
"||" return OR;
">=" return GE;
"<=" return LE;

"(" return '(';
")" return ')';
"{" {
    g_symbolTable.IncreaseScope();
    return '{';
    }

"}" { 
    g_symbolTable.DecreaseScope();
    return '}';
    }

"[" return '[';
"]" return ']';
";" return ';';
"," return ',';
"." return '.';
"+" return '+';
"-" return '-';
"*" return '*';
"/" return '/';
"%" return '%';
"=" return '=';
">" return '>';
"<" return '<';

[1-9]*[0-9]+  return INT_VAL;
[A-Za-z_]+  return ProcessIdentifier(yytext);
[1-9]*[0-9].[0-9]*[1-9] return FLOAT_VAL;

[:@#$] return JUNK_TOKEN;
%%

//******************************************************
// ProcessIdentifier: create symbols in symbol table
static int ProcessIdentifier(char * inputIdentifier)
{
    string name(inputIdentifier);
    cSymbol *sym;

    if (g_insert)
    {
        sym = g_symbolTable.FindLocal(name);
        if (sym == nullptr)
        {
            sym = new cSymbol(name);
            g_symbolTable.Insert(sym);
        }
    }
else
    {
        sym = new cSymbol(name);
    }
    yylval.symbol = sym;

    return IDENTIFIER;
   
}

// This function allows us to do extra processing on each token
// It is used to generate lexer debug info
int Return(int val)
{
    printf("Scanned '%s': %d\n", yytext, val);
    return val;
}

