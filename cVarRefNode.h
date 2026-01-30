class cVarRefNode : public cExprNode {
  protected:
    cSymbol* m_ref;
  public:
    cVarRefNode(cSymbol* ref) : m_ref(ref) {
      AddChild(m_ref);
    }
    virtual string NodeType() override { return "varref"; }
    virtual void Visit(cVisitor* visitor) override {
        visitor->Visit(this);
    }
};