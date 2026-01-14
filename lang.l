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
// Note: for this to work, use DO_RETURN(val) for all the return statements
//       for your tokens. See sample below for "program"
//#define DEBUG_OUTPUT
#ifdef DEBUG_OUTPUT
    #define DO_RETURN(a) { return Return(a); }
#else
    #define DO_RETURN(a) { return (a); }
#endif

int Return(int val);
static int ProcessID();
%}

%option noyywrap
%option noinput
%option nounput
%option yylineno

    /* definitions go here. whitespace is an example. */
whitespace      '[\t\n\r ]'
letters         [a-z]
%%
{whitespace}+               {} /* Ignore whitespace */
;                           {} /* Ignore semicolons */
    /* control scope with curly braces
     NOTE: This functionality will eventually move to the parser
    */
PROGRAM                   DO_RETURN(PROGRAM);
"{"                         DO_RETURN(OPEN);
"}"                         DO_RETURN(CLOSE);
LOCAL                       DO_RETURN(LOCAL);
GLOBAL                      DO_RETURN(GLOBAL);
LOOKUP                      DO_RETURN(LOOKUP);
INSERT                      DO_RETURN(INSERT);
[a-z]+                      {
                                DO_RETURN(ProcessID());
                            }

%%

//******************************************************
// ProcessID: create symbols in symbol table
static int ProcessID()
{
    string name(yytext);
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

