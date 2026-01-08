#include "cSymbolTable.h"

symbolTable_t* cSymbolTable::IncreaseScope()
{
    return newScope;
}

symbolTable_t* cSymbolTable::DecreaseScope()
{
    return outerMostScope;
}
void cSymbolTable::Insert(cSymbol * sym)
{
    return;
}

cSymbol* cSymbolTable::Find(string name)
{
    return nullptr;
}

cSymbol* cSymbolTable::FindLocal(string name)
{
    return nullptr;
}
