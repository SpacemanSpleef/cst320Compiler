#include "cSymbolTable.h"
#include "cBaseTypeNode.h"
#include <iostream>

cSymbolTable::cSymbolTable()
{
    // Global scope for base types
    symbolTable_t* globalScope = new symbolTable_t();
    tables.push(globalScope);

    std::string baseTypes[] = {"char", "int", "float", "long", "double"};
    int sizes[] = {1, 4, 4, 8, 8};

    for (int i = 0; i < 5; i++)
    {
        cSymbol* sym = new cSymbol(baseTypes[i]);
        // Requirement 4: Create cBaseTypeNode and set as the decl
        bool isFloat = (baseTypes[i] == "float" || baseTypes[i] == "double");
        cDeclNode* typeDecl = new cBaseTypeNode(sym, sizes[i], isFloat);
        
        sym->SetDecl(typeDecl); // Linked via pointer now, not a bool
        globalScope->symbols.insert({baseTypes[i], sym});
    }
}
symbolTable_t* cSymbolTable::IncreaseScope()
{
    symbolTable_t* newScope = new symbolTable_t(); // fresh empty scope
    tables.push(newScope);
    return newScope;
}

symbolTable_t* cSymbolTable::DecreaseScope()
{
    if (tables.empty()) return nullptr;
    symbolTable_t* top = tables.top();
    tables.pop();
    return top;
}

void cSymbolTable::Insert(cSymbol *sym)
{
    if (tables.empty()) return;
        symbolTable_t* curScope = tables.top();
    
    curScope->symbols[sym->GetName()] = sym;
}

cSymbol* cSymbolTable::FindLocal(string name)
{
    // Only check top-most program scope
    symbolTable_t* curScope = tables.top();
    auto it = curScope->symbols.find(name);
    if (it != curScope->symbols.end())
        return it->second;
    return nullptr;
}

cSymbol* cSymbolTable::Find(string name)
{
    // searches top -> bottom
    std::stack<symbolTable_t*> backup;
    cSymbol* sym = nullptr;

    while (!tables.empty() && !sym)
    {
        symbolTable_t* curScope = tables.top();
        auto it = curScope->symbols.find(name);
        if (it != curScope->symbols.end()) sym = it->second;
        tables.pop();
        backup.push(curScope);
    }

    while (!backup.empty())
    {
        tables.push(backup.top());
        backup.pop();
    }

    return sym;
}