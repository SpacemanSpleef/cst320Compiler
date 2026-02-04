#pragma once
#include "cDeclNode.h"
#include "cParamsNode.h"
class cFuncDeclNode : public cDeclNode {
protected:
    cSymbol* m_returnType;
    cSymbol* m_name;
    cParamsNode* m_params;
    cBlockNode* m_body;

public:
    cFuncDeclNode(cSymbol* type, cSymbol* name) : 
        m_returnType(type), m_name(name), m_params(nullptr), m_body(nullptr) {}

    void AddParams(cParamsNode* params) { m_params = params; AddChild(params); }
    void AddBody(cBlockNode* body)     { m_body = body; AddChild(body); }

    virtual cSymbol* GetSymbol() override { return m_name; }
    virtual string NodeType() override { return "func_decl"; }
    virtual void Visit(cVisitor* visitor) override { visitor->Visit(this); }
};