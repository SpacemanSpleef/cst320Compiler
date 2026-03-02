#pragma once
//**************************************
// cDeclNode
//
// Defines base class for all declarations.
// Future labs will add features to this class.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"

class cDeclNode : public cAstNode
{
    protected:
        int m_size;
        int m_offset;
    public:
        cDeclNode() : cAstNode(), m_size(0), m_offset(0) {}
        void SetSize(int size) { m_size = size; }
        void SetOffset(int offset) { m_offset = offset; }
        int GetOffset() { return m_offset; }
        virtual cSymbol* GetSymbol() = 0;
        virtual bool IsArray()  { return false; }
        virtual bool IsStruct() { return false; }
        virtual bool IsType()   { return false; }
        virtual bool IsFunc()   { return false; }
        virtual bool IsVar()    { return false; }
        virtual bool IsFloat()  { return false; }
        virtual bool IsInt()    { return false; }
        virtual bool IsChar()   { return false; }
        virtual int  GetSize()  { return m_size; }
        virtual cDeclNode *GetType() = 0;
        virtual cDeclNode* GetDecl() = 0;
        virtual std::string GetName() = 0;
        bool IsCompatibleWith(cDeclNode *type) 
        {
            if (this == type) return true;
        
            cDeclNode *argType = type->GetType();
        
            if (this->IsFloat())
            {
                if (argType->IsFloat())
                    return (this->GetSize() >= argType->GetSize());
            
                if (argType->IsInt() || argType->IsChar())
                    return true;
            }
        
            if (this->IsInt())
            {
                if (argType->IsFloat()) return false;
            
                if (argType->IsInt() || argType->IsChar())
                    return (this->GetSize() >= argType->GetSize());
            }
        
            return false;
        }
        virtual string AttributesToString() override 
        {
            if (m_size == 0 && m_offset == 0) return "";
            return " size=\"" + std::to_string(m_size) + 
                   "\" offset=\"" + std::to_string(m_offset) + "\"";
        }
    };
