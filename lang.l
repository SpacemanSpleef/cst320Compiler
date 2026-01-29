%{
//**************************************
// lang.l
//
// scanner definition file. flex uses this file to create the scanner
//
// Author: Theodore Gwynn
//

#include "lex.h"
#include "langparse.h"

// Macros that can be used to create debug output from the scanner
// The first one does NOT create debug output, the second does.
// Uncomment one or the other then use DO_RETURN(val) for all your return 
// statements
#define DO_RETURN(a) { return (a); }
//#define DO_RETURN(a) { return Return(a); }

int Return(int val);

%}

%option noyywrap
%option noinput
%option nounput
%option yylineno


%%

    // token definitions. I gave "program" as an example
[ \t\r\n]+    ;
program                     DO_RETURN(PROGRAM);
print                       DO_RETURN(PRINT);
[0-9]+\.[0-9]+              { yylval.float_val = atof(yytext); DO_RETURN(FLOAT_VAL); }
[0-9]+                      {yylval.int_val = atoi(yytext); DO_RETURN(INT_VAL);}

[{};()*+/-]                 DO_RETURN(yytext[0]);

%%

// This function allows us to do extra processing on each token
// It is used to generate lexer debug info
int Return(int val)
{
    printf("Scanned '%s': %d\n", yytext, val);
    return val;
}
