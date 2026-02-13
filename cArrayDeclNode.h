#pragma once
#include "cDeclNode.h"
#include "cSymbol.h"

class cArrayDeclNode : public cDeclNode
{
    public:
        cArrayDeclNode(cSymbol *type, int size, cSymbol *name) 
            : cDeclNode()
        {
            m_baseType = type;
            m_size = size;
            m_name = name;
        }

        virtual string NodeType() { return string("array_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        virtual string AttributesToString()
        {
            return " name=\"" + m_name->GetName() + "\"" + 
                   " size=\"" + std::to_string(m_size) + "\"";
        }
        cSymbol* GetSymbol(){return m_name;}
        // Often useful for semantic analysis later
        cSymbol* GetBaseType() { return m_baseType; }
        int GetSize() { return m_size; }
        virtual bool IsArray() override {return true;}
        virtual cDeclNode* GetDecl() override {return this;}

    protected:
        cSymbol *m_baseType;
        int m_size;
        cSymbol *m_name;
};