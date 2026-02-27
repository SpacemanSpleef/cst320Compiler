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

        cExprNode* lval = node->GetLVal();
        cExprNode* expr = node->GetExpr();
        if (lval == nullptr || expr == nullptr) return;

        cDeclNode* lvalType = lval->GetType();
        cDeclNode* exprType = expr->GetType();
        if (lvalType == nullptr || exprType == nullptr) return;

        lvalType = lvalType->GetType();
        exprType = exprType->GetType();
        if (lvalType == nullptr || exprType == nullptr) return;

        if (!lvalType->IsCompatibleWith(exprType)) {
            node->SemanticError("Cannot assign " + exprType->GetName() + 
                                " to " + lvalType->GetName());
        }
    }
    virtual void Visit(cVarRefNode *node) {
    if (node->GetSymbol() == nullptr) return;
    cDeclNode* decl = node->GetSymbol()->GetDecl();
    if (decl == nullptr) return;
    if (decl->IsFunc()) {
        node->SemanticError("Symbol " + node->GetSymbol()->GetName() + 
                           " is a function, not a variable");
    }
}
    void Visit(cFuncCallNode *node) 
    {
        node->VisitAllChildren(this);
        cSymbol *sym = node->GetSymbol();
        cFuncDeclNode *decl = dynamic_cast<cFuncDeclNode*>(sym->GetDecl());
        if (decl == nullptr) return; 
        if (!decl->HasDefinition()) {
            node->SemanticError("Function " + sym->GetName() + " is not fully defined");
        }
        if (node->GetNumArgs() != decl->GetNumParams()) {
            node->SemanticError(sym->GetName() + " called with wrong number of arguments");
            return; 
        }
        for (int i = 0; i < node->GetNumArgs(); i++) {
            cDeclNode *argType = node->GetArg(i)->GetType();
            cDeclNode *paramType = decl->GetParam(i)->GetType();

            if (!paramType->IsCompatibleWith(argType)) {
                node->SemanticError(sym->GetName() + " called with incompatible argument");
            }
        }
    }
    void Visit(cIndexNode *node) 
    {
        node->VisitAllChildren(this);

        cVarRefNode* base = dynamic_cast<cVarRefNode*>(node->GetBase());
        if (base == nullptr || base->GetSymbol() == nullptr) return;

        cDeclNode* varDecl = base->GetSymbol()->GetDecl();
        cDeclNode* varType = varDecl->GetType();  

        if (varType == nullptr || !varType->IsArray()) {
            node->SemanticError(node->GetBaseName() + " is not an array");
            return;
        }

        cDeclNode* indexType = node->GetIndex()->GetType();
        if (indexType != nullptr && !indexType->IsInt() && !indexType->IsChar()) {
            node->SemanticError("Index of " + node->GetBaseName() + " is not an int");
        }
    }
    void Visit(cBinaryExprNode *node) 
    {
        node->VisitAllChildren(this);
 
    }   
};