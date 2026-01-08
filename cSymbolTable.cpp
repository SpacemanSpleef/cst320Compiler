#include "cSymbolTable.h"

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
