//****************************************************
// Starter code for a symbol table definition.
// You are not required to use any of these names, but you will need the 
// described functionality
//
#include <map>
#include "cSymbol.h"
#include <stack>

struct symbolTable_t
{
    std::map<string, cSymbol> symbols;        
};

class cSymbolTable
{
    public:
        // Construct an empty symbol table
        cSymbolTable();

        // Increase the scope: add a level to the nested symbol table
        // Return value is the newly created scope
        symbolTable_t *IncreaseScope();

        // Decrease the scope: remove the outer-most scope.
        // Returned value is the outer-most scope AFTER the pop.
        //
        // NOTE: do NOT clean up memory after poping the table. Parts of the
        // AST will probably contain pointers to symbols in the popped table.
        symbolTable_t *DecreaseScope();

        // insert a symbol into the table
        // Assumes the symbol is not already in the table
        void Insert(cSymbol *sym);

        // Do a lookup in the nested table. 
        // NOTE: This starts at the inner-most scope and works its way out until
        // a match is found.
        // Return the symbol for the inner-most match. 
        // Returns nullptr if no match is found.
        cSymbol *Find(string name);

        // Find a symbol in the inner-most scope.
        // NOTE: This ONLY searches the inner-most scope.
        // Returns nullptr if the symbol is not found.
        cSymbol *FindLocal(string name);

    private:
        std::stack<symbolTable_t*> tables;
};

/*
cSymbolTable::cSymbolTable()
{
}

symbolTable_t* cSymbolTable::IncreaseScope()
{
    symbolTable_t* newScope = new symbolTable_t();
    tables.push(newScope);
    return newScope;
}

symbolTable_t* cSymbolTable::DecreaseScope()
{
    symbolTable_t* outerMostScope = tables.pop();
    return outerMostScope;
}
void cSymbolTable::Insert(cSymbol * sym)
{
    symbolTable_t* outerScope = tables.pop();
    outerScope.symbols.insert(std::pair<string, cSymbol>(sym.GetName, sym));
    tables.push(outerScope);
    return;
}

cSymbol* cSymbolTable::Find(string name)
{

    return nullptr;
}

cSymbol* cSymbolTable::FindLocal(string name)
{
    symbolTable_t* scope = tables.pop();
    cSymbol* sym = nullptr;
    try
    {
        sym = scope.symbols.at(name);
    }
    catch(std::exception e)
    {
        sym = nullptr;
        std::cout<< e.What() << std::endl;
    }
    tables.push(scope);
    return sym;
}
*/
// declare the global symbol table. The definition will have to be in a cpp file
extern cSymbolTable g_symbolTable;
