#pragma once
//**************************************
// cSymbol.h
//
// Defines class used to represent symbols.
// Later labs will add features to this class.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include <string>
#include "cAstNode.h"
#include "cDeclNode.h"
using std::string;

class cSymbol : public cAstNode
{
    public:
        // param is name of symbol
        cSymbol(string name) : cAstNode()
        {
            m_id = ++nextId;        // get next available ID
            m_name = name;
            m_decl = nullptr;
        }

        virtual string AttributesToString()
        {
            string result(" id=\"");
            result += std::to_string(m_id);
            result += "\" name=\"" + m_name + "\"";
            return result;
        }
        virtual string NodeType() { return string("sym"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        cDeclNode* GetDecl() { return m_decl; }
        void SetDecl(cDeclNode* decl) { m_decl = decl; }    
        void SetType(cDeclNode* decl) { m_decl = decl; }
        void SetIsType(bool t){ isType = t; }
        bool IsType() const {return isType;}
        virtual std::string GetName() {return m_name;}
    protected:
        static long long nextId;        // Next avail symbol ID
        long long m_id;                 // Unique ID for this symbol
        string m_name;                  // name of symbol
        cDeclNode* m_decl = nullptr;
        bool isType = false;
};
