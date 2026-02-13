#pragma once
//**************************************
// cBaseTypeNode
//
// Defines virtual base class for all declarations.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 28, 2015
//

#include "cDeclNode.h"

class cBaseTypeNode : public cDeclNode
{
    public:
        cBaseTypeNode(string name, int size, bool isFloat): cDeclNode() 
        {
            m_name = name;
            m_size = size;
            m_isFloat = isFloat;
        }

        // return various Is* values
        bool IsFloat() override { return m_isFloat; }
        bool IsInt()  override { return !m_isFloat; }
        bool IsChar() override { return (!m_isFloat && m_size==1); }
        bool IsType() override { return true; }

        // return the symbol for the type
        virtual cSymbol* GetSymbol() override { return nullptr;}
        // return the name of the item that is declared
        virtual string  GetName() { return m_name; }

        virtual string NodeType() { return "type"; }
        // return a string representation of the node
        virtual string AttributeToString()
        {
            return " name=\"" + m_name + "\" size=\"" + 
                std::to_string(m_size) +
                "\" isFloat=\"" + std::to_string(m_isFloat);
        }

        // return size of data item
        virtual int GetSize() { return m_size; }
        virtual cDeclNode* GetDecl() override {return this;}
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    protected:
        string m_name;
        int    m_size;
        bool   m_isFloat;
};
