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

#ifndef __TimedText_Node__
#define __TimedText_Node__

#include <TimedText/Types.h>
#include <TimedText/String.h>
#include <TimedText/Timestamp.h>
#include <TimedText/List.h>

namespace TimedText
{

class NodeData;

enum NodeType
{
  EmptyNode = 0,
  InternalNode,
  LeafNode,
};

enum NodeElementType
{
  NullNode = 0,
  FirstNodeElementType = NullNode,

  // WebVTT internal nodes
  BoldNode,
  ItalicNode,
  UnderlineNode,
  VoiceNode,

  // WebVTT leaf nodes
  TextNode,
  TimestampNode,

  // TTML elements
  TTNode,
  HeadNode,
  BodyNode,
  DivNode,
  PNode,
  SpanNode,
  BRNode,
  RegionNode,

  LastNodeElementType,
};

class Node
{
public:
  typedef List<Node>::iterator iterator;
  typedef List<Node>::const_iterator const_iterator;

  Node();
  Node(NodeType type, NodeElementType elementType);

  NodeType type() const;
  NodeElementType elementType() const;
  inline NodeElementType element() const { return elementType(); }

  // Timestamp if elementType == TimestampNode, else MalformedTime
  Timestamp timestamp() const;
  // Element annotation if elementType == VoiceNode, else null String
  String voice() const;
  // Text if elementType == TextNode
  String text() const;
  // space-separated CSS classes, for internal nodes -- else null String
  // TTML does not support the 'class' attribute in its model for any
  // internal nodes, and so styleClasses() for TTML nodes will always
  // return the null String. In the case of TTML nodes, refer to the 'style'
  // attribute instead.
  String styleClasses() const;

  // Will fail for non-TimestampNodes
  bool setTimestamp(const Timestamp &ts);
  // Will fail for non-VoiceNodes
  bool setVoice(const String &voice);
  // Will fail for non TextNodes
  bool setText(const String &text);
  // Will fail for non-InternalNodes
  bool setStyleClasses(const String &style);

  // Return copy of node list -- always empty in the
  // case of leaf nodes.
  List<Node> children() const;

  // Wrappers for list routines in internal nodes.
  // Will always fail for leaf nodes.
  bool push(const Node &node);
  bool pop(Node &result);
  bool unshift(const Node &node);
  bool shift(Node &result);
  bool insert(int i, const Node &node);
  bool take(int i, Node &result);

  // Iterators over child nodes
  // These iterators will be invalid in the case of leafs
  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

private:
  NodeData *d;
};

TT_DECLARE_TYPEINFO(Node,TT_MOVABLE_TYPE);

}

#endif // __TimedText_Node__