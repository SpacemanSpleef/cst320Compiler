#include "cSymbolTable.h"
#include <iostream>

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
    symbolTable_t* outerMostScope = tables.top();
    tables.pop();
    return outerMostScope;
}
void cSymbolTable::Insert(cSymbol * sym)
{
    symbolTable_t* outerScope = tables.top();
    outerScope->symbols.insert(std::pair<string, cSymbol>(sym->GetName(), *sym));

    return;
}

cSymbol* cSymbolTable::Find(string name)
{
    cSymbol* sym = nullptr;
    std::stack<symbolTable_t*> backup;
    while(sym == nullptr && !tables.empty())
    {
        symbolTable_t* curScope = tables.top();
        try
        {
            sym = &curScope->symbols.at(name);
        }
        catch(std::out_of_range &e)
        {
            sym = nullptr;
            tables.pop();
            backup.push(curScope);
        }
    }
    while(!backup.empty())
    {
        tables.push(backup.top());
        backup.pop();
    }
    return sym;
}

cSymbol* cSymbolTable::FindLocal(string name)
{

    symbolTable_t* scope = tables.top();
    cSymbol* sym = nullptr;
    try
    {
        sym = &scope->symbols.at(name);
    }
    catch(std::out_of_range &e)
    {
        sym = nullptr;
   
    }
    //tables.push(scope);
    return sym;
}
