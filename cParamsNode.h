#pragma once
class cParamsNode : public cAstNode {
public:
    cParamsNode(cAstNode* firstParam) : cAstNode() {
        if (firstParam != nullptr) AddChild(firstParam);
    }
    
    void AddParam(cAstNode* param) {
        if (param != nullptr) AddChild(param);
    }

    virtual string NodeType() override { return "params"; }
    virtual void Visit(cVisitor* visitor) override { visitor->Visit(this); }
};