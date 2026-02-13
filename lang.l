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
#include "cSymbol.h"
#include "cSymbolTable.h"

// Macros that can be used to create debug output from the scanner
// The first one does NOT create debug output, the second does.
// Uncomment one or the other then use DO_RETURN(val) for all your return 
// statements
#define DO_RETURN(a) { return (a); }
//#define DO_RETURN(a) { return Return(a); }

int Return(int val);
int ProcessIdentifier( int type);
%}

%option noyywrap
%option noinput
%option nounput
%option yylineno


%%

    // token definitions. I gave "program" as an example
[ \t\r\n]+    ;
"//".* ;

program                     DO_RETURN(PROGRAM);
prints                      DO_RETURN(PRINTS);
print                       DO_RETURN(PRINT);
if                          DO_RETURN(IF);
endif                       DO_RETURN(ENDIF);
return                      DO_RETURN(RETURN);
else                        DO_RETURN(ELSE);
while                       DO_RETURN(WHILE);
struct                      DO_RETURN(STRUCT);
array                       DO_RETURN(ARRAY);

\"([^\\\"]|\\.)*\"          {yylval.str_val = new std::string(yytext + 1, yyleng - 2); DO_RETURN(STRING_LIT);}

[0-9]+\.[0-9]+              { yylval.float_val = atof(yytext); DO_RETURN(FLOAT_VAL); }
[0-9]+                      {yylval.int_val = atoi(yytext); DO_RETURN(INT_VAL);}
[a-zA-Z_][a-zA-Z0-9_]*       { DO_RETURN(ProcessIdentifier(IDENTIFIER)); }
"=="        DO_RETURN(EQUALS);
"!="        DO_RETURN(NOT_EQUALS);
"<="        DO_RETURN(LE);
">="        DO_RETURN(GE);
"&&"        DO_RETURN(AND);
"||"        DO_RETURN(OR);

[{};()*+/%="."-]                 DO_RETURN(yytext[0]);

%%

// This function allows us to do extra processing on each token
// It is used to generate lexer debug info
int Return(int val)
{
    printf("Scanned '%s': %d\n", yytext, val);
    return val;
}

int ProcessIdentifier(int type)
{
    cSymbol *sym = g_symbolTable.Find(yytext);
    
    if (sym == nullptr) 
    {
        sym = new cSymbol(yytext);
        g_symbolTable.Insert(sym); 
    } 
    else 
    {    
        if (sym->GetType() != nullptr) 
        {
            if (sym->GetType()->IsType())
            { 
                type = TYPE_ID;
            }
            else if (sym->GetType()->GetDecl() != nullptr && sym->GetType()->GetDecl()->IsStruct()) 
            {
                type = TYPE_ID;
            }
        }
    }
    
    yylval.symbol = sym;
    return type;
}