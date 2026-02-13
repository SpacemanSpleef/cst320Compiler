#include "cSymbolTable.h"
#include "cBaseTypeNode.h"
#include <iostream>

cSymbolTable::cSymbolTable()
{
    this->IncreaseScope();
    std::string baseTypes[] = {"char", "int", "float", "long", "double"};
    int iterator = 0;
    int sizes[5] = {1, 4, 4, 8, 8};
    for (const std::string& typeName : baseTypes)
    {
        cSymbol* sym = new cSymbol(typeName);
        cDeclNode * type= new cBaseTypeNode(typeName, sizes[iterator], (typeName == "float"));
        sym->SetType(type);
        iterator++;
        this->Insert(sym);
    }

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
    if(FindLocal(sym->GetName()) != nullptr)
    {return;}

    symbolTable_t* outerScope = tables.top();
    cSymbol *symbolToInsert = sym;
    if (Find(sym->GetName()) != nullptr) {
        symbolToInsert = new cSymbol(sym->GetName());
    }
    outerScope->symbols.insert(std::pair<string, cSymbol*>(symbolToInsert->GetName(), symbolToInsert));

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
            sym = curScope->symbols.at(name);
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
        sym = scope->symbols.at(name);
    }
    catch(std::out_of_range &e)
    {
        sym = nullptr;
   
    }
    //tables.push(scope);
    return sym;
}
