//
// Copyright (c) 2013 Caitlin Potter and Contributors
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//  * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#include "NodeData.h"

namespace TimedText
{

class EmptyNodeData : public NodeData
{
public:
  EmptyNodeData() : NodeData(EmptyNode,NullNode) {}
} emptyNode;

Node::Node()
  : d(&emptyNode)
{
  d->ref.ref();
}

static bool isInternalNode(NodeElementType type)
{
  if(type > FirstNodeElementType && type < LastNodeElementType) {
    // There are currently only two leaf nodes...
    if(type == TextNode || type == TimestampNode)
      return false;
    return true;
  }
  return false;
}

static NodeData *internalNodeData(NodeElementType type)
{
  if(isInternalNode(type)) {
    if(type == InternalTextNode)
      return new InternalNodeData(InternalTextNode);
    if(type == VoiceNode)
      return new VoiceNodeData();
    if(type == LangNode)
      return new LangNodeData();
    else
      return new ElementNodeData(type);
  }
  return 0;
}

static NodeData *leafNodeData(NodeElementType type)
{
  if(type == TextNode)
    return new TextNodeData();
  if(type == TimestampNode)
    return new TimestampNodeData();
  return 0;
}

static NodeData *createNodeData(NodeType type, NodeElementType elem)
{
  NodeData *d = 0;
  if(type == InternalNode && (d = internalNodeData(elem)))
    return d;
  if(type == LeafNode && (d = leafNodeData(elem)))
    return d;
  d = &emptyNode;
  d->ref.ref();
  return d; 
}

static NodeData *createNodeData(NodeElementType type)
{
  NodeData *d = 0;
  if((d = internalNodeData(type)))
    return d;
  if((d = leafNodeData(type)))
    return d;
  d = &emptyNode;
  d->ref.ref();
  return d;
}

Node::Node(NodeType type, NodeElementType elementType)
  : d(createNodeData(type,elementType))
{
}

Node::Node(NodeElementType type)
  : d(createNodeData(type))
{ 
}

Node::Node(const Node &other)
  : d(other.d)
{
  d->ref.ref();
}

Node::~Node()
{
  if(!d->ref.deref())
    delete d;
}

Node &
Node::operator=(const Node &other)
{
  other.d->ref.ref();
  if(!d->ref.deref())
    delete d;
  d = other.d;
  return *this;
}

NodeType
Node::type() const
{
  return d->type;
}

NodeElementType
Node::elementType() const
{
  return d->element;
}

Timestamp
Node::timestamp() const
{
  return d->timestamp();
}

String
Node::voice() const
{
  return d->voice();
}

String
Node::text() const
{
  return d->text();
}

String
Node::lang() const
{
  return d->lang();
}

List<String>
Node::applicableClasses() const
{
  return d->applicableClasses();
}

bool
Node::setTimestamp(const Timestamp &ts)
{
  return d->setTimestamp(ts);
}

bool
Node::setVoice(const String &voice)
{
  return d->setVoice(voice);
}

bool
Node::setText(const String &text)
{
  return d->setText(text);
}

bool
Node::setLang(const String &lang)
{
  return d->setLang(lang);
}

bool
Node::setApplicableClasses(const List<String> &classes)
{
  return d->setApplicableClasses(classes);
}

const List<Node> &
Node::children() const
{
  return d->children();
}

bool
Node::push(const Node &node)
{
  return d->push(node);
}

bool
Node::pop(Node &result)
{
  return d->pop(result);
}

bool
Node::unshift(const Node &node)
{
  return d->unshift(node);
}

bool
Node::shift(Node &result)
{
  return d->shift(result);
}

bool
Node::insert(int i, const Node &node)
{
  return d->insert(i, node);
}

bool
Node::take(int i, Node &result)
{
  return d->take(i, result);
}

bool
Node::itemAt(int i, Node &result)
{
  return d->itemAt(i, result);
}

Node::iterator
Node::begin()
{
  return d->begin();
}

Node::iterator
Node::end()
{
  return d->end();
}

Node::const_iterator
Node::begin() const
{
  const NodeData *x = d;
  return x->begin();
}

Node::const_iterator
Node::end() const
{
  const NodeData *x = d;
  return x->end();
}

int
Node::childCount() const
{
  return d->childCount();
}

struct VisitState
{
public:
  inline VisitState() : i(0) {}
  inline VisitState(const VisitState &v) : n(v.n), i(v.i) {}
  inline VisitState(const Node &node) : n(node), i(0) {}
  inline VisitState(const Node &node, int pos) : n(node), i(pos) {}
  inline bool hasChildren() const { return n.childCount() > 0; }
  inline bool isLeaf() const { return n.type() == LeafNode; }
  inline bool isInternal() const { return n.type() == InternalNode; }
  inline bool isInternalText() const {
    return n.element() == InternalTextNode;
  }
  inline bool isEmpty() const { return n.type() == EmptyNode; }
  inline bool next(VisitState &result) {
    Node tmp;
    if(n.itemAt(i++, tmp)) {
      result = VisitState(tmp);
      return true;
    }
    return false;
  }
  operator Node &() { return n; }
  operator const Node &() const { return n; }

  Node n;
  int i;
};

void
Node::visit(NodeVisitor &visitor)
{
  List<VisitState> stack;
  stack.push(*this);
  VisitState *top;
  while(stack.lastPtr(top)) {
    VisitState it;
    if(top->next(it)) {
      // Don't visit empty nodes.
      if(it.isEmpty())
        continue;
      // Visit the item before descending
      visitor.visit(it);

      // We might need to descend, if it's
      if(it.isInternal()) {
        if(it.hasChildren() && (it.isInternalText() || visitor.enter(it)))
          stack.push(it);
      }
    } else {
      // Notify visitor that we are leaving a branch of the tree
      if(!top->isInternalText())
        visitor.leave(*top);
      stack.pop(it);
    }
  }
}


} // TimedText
