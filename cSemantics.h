#pragma once
#include "cVisitor.h"
#include "astnodes.h"

class cSemantics : public cVisitor
{
public:
    cSemantics() : cVisitor() {}

    virtual void Visit(cAssignmentNode *node)
    {
        node->VisitAllChildren(this);
        cDeclNode *lvalType = node->GetLVal()->GetType();
        cDeclNode *exprType = node->GetExpr()->GetType();
        if (lvalType != nullptr && exprType != nullptr)
        {
            if (!lvalType->IsCompatibleWith(exprType))
            {
                SemanticParseError("Cannot assign " + exprType->GetName() + " to " + lvalType->GetName());
            }
        }
    }
    virtual void Visit(cVarRefNode *node){};
    void Visit(cFuncCallNode *node) 
    {
        node->VisitAllChildren(this);
        cSymbol *sym = node->GetSymbol();
        cFuncDeclNode *decl = dynamic_cast<cFuncDeclNode*>(sym->GetDecl());
        if (decl == nullptr) return; 
        if (!decl->HasDefinition()) {
            SemanticParseError("Function " + sym->GetName() + " not fully defined");
        }
        if (node->GetNumArgs() != decl->GetNumParams()) {
            SemanticParseError(sym->GetName() + " called with wrong number of arguments");
            return; 
        }
        for (int i = 0; i < node->GetNumArgs(); i++) {
            cDeclNode *argType = node->GetArg(i)->GetType();
            cDeclNode *paramType = decl->GetParam(i)->GetType();

            if (!paramType->IsCompatibleWith(argType)) {
                SemanticParseError("function " + sym->GetName() + " called with incompatible argument");
            }
        }
    }
    void Visit(cIndexNode *node)
    {
        node->VisitAllChildren(this);

        cDeclNode* baseType = node->GetBase()->GetType();
        if (baseType != nullptr && !baseType->IsArray())
        {
            SemanticParseError(node->GetBaseName() + " is not an array");
        }
        cDeclNode* indexType = node->GetIndex()->GetType();
        if (indexType != nullptr && !indexType->IsInt())
        {
            SemanticParseError("Index of " + node->GetBaseName() + " is not an int");
        }
    }
};