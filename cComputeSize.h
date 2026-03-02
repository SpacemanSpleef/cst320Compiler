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

    node->VisitAllChildren(this);

    int highWaterIncrease = m_highWaterMark - startingHighWater;
    int offsetIncrease = m_offset - startingOffset;
    int blockSize = (highWaterIncrease > offsetIncrease) ? highWaterIncrease : offsetIncrease;
    
    node->SetSize(blockSize);
    
    if (m_offset > m_highWaterMark)
    {
        m_highWaterMark = m_offset;
    }

    m_offset = startingOffset; 
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
            cDeclNode *baseType = node->GetBaseType();
            if (baseType == nullptr) return;

            int elementSize = baseType->GetSize();
            int totalSize = elementSize * node->GetArraySize();  // Changed from GetSize()

            // Align offset before assigning
            m_offset = Align(m_offset, elementSize);

            node->SetOffset(m_offset);
            node->SetSize(totalSize);

            m_offset += totalSize;
        }
        
        virtual void Visit(cFuncDeclNode *node)
        {
            int oldOffset = m_offset;
            int oldHighWater = m_highWaterMark;
        
            m_offset = 0;
            m_highWaterMark = 0;
        
            node->VisitAllChildren(this);
        
            node->SetSize(m_highWaterMark);
        
            m_offset = oldOffset;
            m_highWaterMark = oldHighWater;
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
            
            node->SetSize(decl->GetSize());
            node->SetOffset(decl->GetOffset());
        }

        virtual void Visit(cIndexNode *node)
        {
            node->VisitAllChildren(this);
            
            // For array indexing, size is the element size
            // Offset is the base array's offset (runtime computation needed for actual element)
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