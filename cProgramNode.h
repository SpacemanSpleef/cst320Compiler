#pragma once
//**************************************
// cProgramNode.h
//
// Defines AST node for a lang program
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cBlockNode.h"

class cProgramNode : public cAstNode
{
    public:
        // param is the block that makes up the program
        cProgramNode(cBlockNode *block) : cAstNode(), m_size(0)
        {
            AddChild(block);
        }

        virtual string NodeType() { return string("program"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

                int GetSize() { return m_size; }
        void SetSize(int size) { m_size = size; }
        
        virtual string AttributesToString() override 
        {
            if (m_size == 0) return "";
            return " size=\"" + std::to_string(m_size) + "\"";
        }
        
    protected:
        int m_size;

};
