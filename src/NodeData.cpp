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

// Dummy implementations of various Node routines, which should be overridden
// by specific node types

NodeData::NodeData(NodeType _type, NodeElementType _elem)
  : ref(AtomicInt(1)), type(_type), element(_elem)
{
}

NodeData::~NodeData()
{
}

Timestamp
NodeData::timestamp() const
{
  return MalformedTimestamp;
}

String
NodeData::voice() const
{
  return String();
}

String
NodeData::text() const
{
  return String();
}

String
NodeData::lang() const
{
  return String();
}

List<String>
NodeData::applicableClasses() const
{
  return List<String>();
}

bool
NodeData::setTimestamp(const Timestamp &ts)
{
  return false;
}

bool
NodeData::setVoice(const String &ts)
{
  return false;
}

bool
NodeData::setText(const String &text)
{
  return false;
}

bool
NodeData::setLang(const String &lang)
{
  return false;
}

bool
NodeData::setApplicableClasses(const List<String> &style)
{
  return false;
}

const List<Node> &
NodeData::children() const
{
  static const List<Node> empty_list;
  return empty_list;
}

bool
NodeData::push(const Node &node)
{
  return false;
}

bool
NodeData::pop(Node &result)
{
  return false;
}

bool
NodeData::unshift(const Node &node)
{
  return false;
}

bool
NodeData::shift(Node &result)
{
  return false;
}

bool
NodeData::insert(int i, const Node &node)
{
  return false;
}

bool
NodeData::take(int i, Node &result)
{
  return false;
}

NodeData::iterator
NodeData::begin()
{
  return List<Node>::iterator();
}

NodeData::iterator
NodeData::end()
{
  return List<Node>::iterator();
}

NodeData::const_iterator
NodeData::begin() const
{
  return List<Node>::const_iterator();
}

NodeData::const_iterator
NodeData::end() const
{
  return List<Node>::const_iterator();
}

int
NodeData::childCount() const
{
  return 0;
}

// InternalNode specialization

InternalNodeData::InternalNodeData(NodeElementType elem)
  : NodeData(InternalNode, elem)
{
}

InternalNodeData::~InternalNodeData()
{
}

const List<Node> &
InternalNodeData::children() const
{
  return nodes;
}

bool
InternalNodeData::push(const Node &node)
{
  return nodes.push(node);
}

bool
InternalNodeData::pop(Node &result)
{
  return nodes.pop(result);
}

bool
InternalNodeData::unshift(const Node &node)
{
  return nodes.unshift(node);
}

bool
InternalNodeData::shift(Node &result)
{
  return nodes.shift(result);
}

bool
InternalNodeData::insert(int i, const Node &node)
{
  return nodes.insert(i, node);
}

bool
InternalNodeData::take(int i, Node &result)
{
  return nodes.take(i, result);
}

NodeData::iterator
InternalNodeData::begin()
{
  return nodes.begin();
}

NodeData::iterator
InternalNodeData::end()
{
  return nodes.end();
}

NodeData::const_iterator
InternalNodeData::begin() const
{
  return nodes.begin();
}

NodeData::const_iterator
InternalNodeData::end() const
{
  return nodes.end();
}

ElementNodeData::ElementNodeData(NodeElementType type)
  : InternalNodeData(type)
{
}

ElementNodeData::~ElementNodeData()
{
}

int
InternalNodeData::childCount() const
{
  return nodes.count();
}

List<String>
ElementNodeData::applicableClasses() const
{
  return _applicableClasses;
}

bool
ElementNodeData::setApplicableClasses(const List<String> &styles)
{
  // TODO:
  // Normalize classes (separate class names by single U+0020 space)
  _applicableClasses = styles;
  return true;
}

VoiceNodeData::VoiceNodeData()
  : ElementNodeData(VoiceNode)
{
}

VoiceNodeData::~VoiceNodeData()
{
}

String
VoiceNodeData::voice() const
{
  return speaker;
}

bool
VoiceNodeData::setVoice(const String &voice)
{
  speaker = voice;
  return true;
}

LangNodeData::LangNodeData()
  : ElementNodeData(LangNode)
{
}

LangNodeData::~LangNodeData()
{
}

String
LangNodeData::lang() const
{
  return language;
}

bool
LangNodeData::setLang(const String &lang)
{
  language = lang;
  return true;
}

// Leaf node specializations
TextNodeData::TextNodeData()
  : NodeData(LeafNode, TextNode)
{
}

TextNodeData::~TextNodeData()
{
}

String
TextNodeData::text() const
{
  return body;
}

bool
TextNodeData::setText(const String &text)
{
  body = text;
  return true;
}

TimestampNodeData::TimestampNodeData()
  : NodeData(LeafNode,TimestampNode)
{
}

TimestampNodeData::~TimestampNodeData()
{
}

Timestamp
TimestampNodeData::timestamp() const
{
  return time;
}

bool
TimestampNodeData::setTimestamp(const Timestamp &ts)
{
  time = ts;
  return true;
}

} // TimedText
