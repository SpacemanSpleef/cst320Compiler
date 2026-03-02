#pragma once
#include "cVisitor.h"
#include "astnodes.h"

class cComputeSize : public cVisitor
{
    protected:
        int m_offset;        
        int m_highWaterMark; 
        
    public:
        cComputeSize() : cVisitor() 
        {
            m_offset = 0;
            m_highWaterMark = 0;
        }

        // Align offset to 4-byte boundary if size > 1
        int Align(int offset, int size) 
        {
            if (size > 1 && offset % 4 != 0) 
            {
                return offset + (4 - (offset % 4));
            }
            return offset;
        }

        virtual void Visit(cProgramNode *node)
        {
            m_offset = 0;
            m_highWaterMark = 0;
            
            node->VisitAllChildren(this);
            
            // Program size is the aligned high water mark
            int totalSize = m_highWaterMark;
            if (totalSize > 1) {
                totalSize = Align(totalSize, 4);
            }
            node->SetSize(totalSize);
        }

        virtual void Visit(cBlockNode *node)
        {
            int startingOffset = m_offset;
            int startingHighWater = m_highWaterMark;
            
            // Set highwater to current offset at block start
            m_highWaterMark = m_offset;
        
            node->VisitAllChildren(this);
        
            // Update if current offset is higher
            if (m_offset > m_highWaterMark)
            {
                m_highWaterMark = m_offset;
            }
        
            // Block size is the peak reached within this block
            int blockSize = m_highWaterMark - startingOffset;
            
            node->SetSize(blockSize);
        
            // Restore offset
            m_offset = startingOffset;
            
            // Restore or update global highwater
            if (m_highWaterMark > startingHighWater) {
                startingHighWater = m_highWaterMark;
            }
            m_highWaterMark = startingHighWater;
        }

        virtual void Visit(cDeclsNode *node)
        {
            int startingOffset = m_offset;
            
            node->VisitAllChildren(this);
            
            int declsSize = m_offset - startingOffset;
            node->SetSize(declsSize);
            
            // Update high water mark if needed
            if (m_offset > m_highWaterMark)
            {
                m_highWaterMark = m_offset;
            }
        }

        virtual void Visit(cVarDeclNode *node) 
        {
            cDeclNode *type = node->GetType();
            if (type == nullptr) return;

            int size = type->GetSize();
        
            // Align offset before assigning
            m_offset = Align(m_offset, size);
        
            node->SetOffset(m_offset);
            node->SetSize(size);        
            m_offset += size;
        }

        virtual void Visit(cArrayDeclNode *node)
        {
            node->VisitAllChildren(this);
        }
        
        virtual void Visit(cFuncDeclNode *node)
        {
            int oldOffset = m_offset;
            int oldHighWater = m_highWaterMark;

            int paramOffset = -8;
            cParamsNode* params = node->GetParams();
            if (params != nullptr)
            {
                for (int i = 0; i < params->NumChildren(); i++)
                {
                    cDeclNode* param = dynamic_cast<cDeclNode*>(params->GetChild(i));
                    if (param != nullptr)
                    {
                        int size = param->GetType()->GetSize();

                        int alignedSize = (size < 4) ? 4 : size;
                        if (alignedSize > 4) 
                        {
                            alignedSize = Align(alignedSize, 4);
                        }

                        paramOffset -= alignedSize;  
                        param->SetOffset(paramOffset);
                        param->SetSize(size); 
                    }
                }

                params->SetSize(-(paramOffset + 8));
            }

            
            m_offset = 0;
            m_highWaterMark = 0;
        
            int startChild = 2;  
            if (params != nullptr) startChild++;  
            
            for (int i = startChild; i < node->NumChildren(); i++)
            {
                cAstNode* child = node->GetChild(i);
                if (child != nullptr)
                {
                    child->Visit(this);
                }
            }
        
            node->SetSize(m_highWaterMark);
            node->SetOffset(0);  
        
            m_offset = oldOffset;
            m_highWaterMark = oldHighWater;
        }
        virtual void Visit(cParamsNode *node)
        {
            // For function call sites, compute size of parameters
            int totalSize = 0;
            
            node->VisitAllChildren(this);
            
            for (int i = 0; i < node->NumChildren(); i++)
            {
                cExprNode* param = dynamic_cast<cExprNode*>(node->GetChild(i));
                if (param != nullptr)
                {
                    cDeclNode* type = param->GetType();
                    if (type != nullptr)
                    {
                        totalSize += type->GetSize();
                    }
                }
            }
            
            node->SetSize(totalSize);
        }

        
        virtual void Visit(cStructDeclNode *node)
        {
            int oldOffset = m_offset;
            m_offset = 0;
        
            // Visit the members to compute their offsets
            cDeclsNode* members = node->GetMembers();
            if (members != nullptr)
            {
                members->Visit(this);
            }
        
            int totalSize = m_offset;
            // Structs must be 4-byte aligned if size > 1
            if (totalSize > 1) {
                totalSize = Align(totalSize, 4);
            }
        
            node->SetSize(totalSize);
            m_offset = oldOffset; 
        }
        
        virtual void Visit(cVarRefNode *node)
        {
            node->VisitAllChildren(this);

            cSymbol* sym = node->GetSymbol();
            if (sym == nullptr) return;

            cDeclNode *decl = sym->GetDecl();
            if (decl == nullptr) return;

            if (node->IsArrayAccess()) {
                cDeclNode* arrayType = decl->GetType();
                if (arrayType != nullptr && arrayType->IsArray()) {
                    cArrayDeclNode* arrDecl = dynamic_cast<cArrayDeclNode*>(arrayType);
                    if (arrDecl != nullptr) {
                        int elementSize = arrDecl->GetBaseType()->GetSize();
                        node->SetSize(decl->GetSize());  
                        node->SetOffset(decl->GetOffset());  
                        node->SetRowSize(elementSize);  
                    }
                }
            } else 
            {
                node->SetSize(decl->GetSize());
                node->SetOffset(decl->GetOffset());
            }
        }

        virtual void Visit(cIndexNode *node)
        {
            node->VisitAllChildren(this);
            
            cDeclNode* type = node->GetType();
            if (type != nullptr)
            {
                node->SetSize(type->GetSize());
            }
            
            cVarRefNode* base = dynamic_cast<cVarRefNode*>(node->GetBase());
            if (base != nullptr)
            {
                cSymbol* sym = base->GetSymbol();
                if (sym != nullptr)
                {
                    cDeclNode* decl = sym->GetDecl();
                    if (decl != nullptr)
                    {
                        node->SetOffset(decl->GetOffset());
                    }
                }
            }
        }
};