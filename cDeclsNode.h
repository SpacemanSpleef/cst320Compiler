#pragma once
//**************************************
// cDeclsNode.h
//
// Defines a class to represent a list of declarations.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cDeclNode.h"

class cDeclsNode : public cAstNode
{
    public:
        // param is the first decl in this decls
        cDeclsNode(cDeclNode *decl) : cAstNode()
        {
            AddChild(decl);
        }

        // Add a decl to the list
        void Insert(cDeclNode *decl)
        {
            AddChild(decl);
        }

        virtual string NodeType() { return string("decls"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        cSymbol* Find(string name) 
        {
            for (int i = 0; i < NumChildren(); i++) {
                cDeclNode* decl = dynamic_cast<cDeclNode*>(GetChild(i));
                
                if (decl != nullptr) {
                    if (decl->GetSymbol()->GetName() == name) {
                        return decl->GetSymbol();
                    }
                }
            }
            return nullptr;
        }
};
