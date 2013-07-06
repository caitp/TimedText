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

#ifndef __TimedText_NodeData__
#define __TimedText_NodeData__

#include <TimedText/Node.h>
#include "Atomic.h"

namespace TimedText
{

// The base NodeData class, which assumes that no
// operation is valid
class NodeData
{
public:
  typedef Node::iterator iterator;
  typedef Node::const_iterator const_iterator;
  NodeData(NodeType _type, NodeElementType _elem);
  virtual ~NodeData();
  virtual Timestamp timestamp() const;
  virtual String voice() const;
  virtual String text() const;
  virtual String styleClasses() const;
  virtual bool setTimestamp(const Timestamp &ts);
  virtual bool setVoice(const String &ts);
  virtual bool setText(const String &ts);
  virtual bool setStyleClasses(const String &ts);
  virtual List<Node> children() const;
  virtual bool push(const Node &node);
  virtual bool pop(Node &result);
  virtual bool unshift(const Node &node);
  virtual bool shift(Node &result);
  virtual bool insert(int i, const Node &node);
  virtual bool take(int i, Node &result);
  virtual iterator begin();
  virtual iterator end();
  virtual const_iterator begin() const;
  virtual const_iterator end() const;
  virtual int childCount() const;
  AtomicInt ref;
  NodeType type : 2;
  NodeElementType element : 30;
};

// The InternalNode class, which has a list of child nodes,
// and implements the interface to access them.
class InternalNodeData : public NodeData
{
public:
  InternalNodeData(NodeType type, NodeElementType elem);
  ~InternalNodeData();

  String voice() const;
  bool setVoice(const String &voice);
  String styleClasses() const;
  bool setStyleClasses(const String &ts);
  List<Node> children() const;
  bool push(const Node &node);
  bool pop(Node &result);
  bool unshift(const Node &node);
  bool shift(Node &result);
  bool insert(int i, const Node &node);
  bool take(int i, Node &result);
  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;
  int childCount() const;

  String annotation;
  String classes;
  List<Node> nodes;
};

// Small number of LeafNode specializations
class TextNodeData : public NodeData
{
public:
  TextNodeData();
  ~TextNodeData();

  String text() const;
  bool setText(const String &text);

  String body;
};

class TimestampNodeData : public NodeData
{
public:
  TimestampNodeData();
  ~TimestampNodeData();

  Timestamp timestamp() const;
  bool setTimestamp(const Timestamp &ts);

  Timestamp time;
};

} // TimedText

#endif // __TimedText_NodeData__
