class cVarRefNode : public cExprNode {
  protected:
    cVarRefNode* m_base;
    cSymbol* m_ref;
  public:
    cVarRefNode(cSymbol* ref) : m_ref(ref) {
      AddChild(m_ref);
    }
    cVarRefNode(cVarRefNode* base, cSymbol* ref):m_base(base), m_ref(ref)
    {
      AddChild(m_base);
      AddChild(m_ref);
    }
    void AddMember(cSymbol* sym)
    {
      AddChild(sym);
    }
    virtual string NodeType() override { return "varref"; }
    virtual void Visit(cVisitor* visitor) override {
        visitor->Visit(this);
    }
};