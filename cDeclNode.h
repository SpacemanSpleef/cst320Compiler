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
    public:
        cDeclNode() : cAstNode() {}
        virtual cSymbol* GetSymbol() = 0;
        virtual bool IsArray()  { return false; }
        virtual bool IsStruct() { return false; }
        virtual bool IsType()   { return false; }
        virtual bool IsFunc()   { return false; }
        virtual bool IsVar()    { return false; }
        virtual bool IsFloat()  { return false; }
        virtual bool IsInt()    { return false; }
        virtual bool IsChar()   { return false; }
        virtual int  GetSize()  { return 0; }
        virtual cDeclNode *GetType() = 0;
        virtual cDeclNode* GetDecl() = 0;
        virtual std::string GetName() = 0;
        // bool IsCompatibleWith(cDeclNode *type) 
        // {
        //     if (this == type) return true;

        //     // if (this->IsFloat())
        //     // {
        //     //     if (type->IsInt() || type->IsChar() || type->IsFloat())
        //     //     {
        //     //         if (type->IsFloat() && type->GetSize() > this->GetSize())
        //     //             return false;

        //     //         return true; 
        //     //     }
        //     // }
        //     if (this->IsFloat())
        //     {
        //         // If the parameter is float/double, accept ANY numeric type 
        //         // (float, double, int, long, char).
        //         if (type->IsFloat() || type->IsInt() || type->IsChar())
        //         {
        //             return true; 
        //         }
        //     }
        
        //     if (this->IsInt())
        //     {
        //         if (type->IsInt() || type->IsChar())
        //             return (this->GetSize() >= type->GetSize());
        //     }
        
        //     return false;
        // }
        bool IsCompatibleWith(cDeclNode *type) 
        {
            if (this == type) return true;
        
            // IMPORTANT: Get the actual type of the argument
            // This turns "bb" into "long" and "cc" into "double"
            cDeclNode *argType = type->GetType();
        
            if (this->IsFloat())
            {
                // Double (8) -> Float (4) is blocked
                if (argType->IsFloat())
                    return (this->GetSize() >= argType->GetSize());
            
                // Long (8) or Int (4) -> Float (4) is allowed
                if (argType->IsInt() || argType->IsChar())
                    return true;
            }
        
            if (this->IsInt())
            {
                // Float -> Int is blocked
                if (argType->IsFloat()) return false;
            
                // Long (8) -> Int (4) is blocked by size check
                if (argType->IsInt() || argType->IsChar())
                    return (this->GetSize() >= argType->GetSize());
            }
        
            return false;
        }
    };
