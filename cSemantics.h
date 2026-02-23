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
    virtual void Visit(cFuncCallNode *node){};
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