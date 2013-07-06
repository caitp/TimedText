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

Node::Node(NodeType type, NodeElementType elementType)
  : d(0)
{
  if(type == InternalNode && isInternalNode(elementType))
    d = new InternalNodeData(type, elementType);
  else if(type == LeafNode) {
    if(elementType == TextNode)
      d = new TextNodeData();
    else if(elementType == TimestampNode)
      d = new TimestampNodeData();
  }

  if(!d) {
    d = &emptyNode;
    d->ref.ref();
  }
}

Node::~Node()
{
  if(!d->ref.deref())
    delete d;
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
Node::styleClasses() const
{
  return d->styleClasses();
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
Node::setStyleClasses(const String &style)
{
  return d->setStyleClasses(style);
}

List<Node>
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

} // TimedText
