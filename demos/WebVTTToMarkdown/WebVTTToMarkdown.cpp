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

#include <TimedText/WebVTTParser.h>
#include <TimedText/SynchronousBuffer.h>
#include <fstream>
#include <string>
#include <cstdio>

using namespace TimedText;

int
main(int argc, char **argv) {
  // Use first command line parameter as filename
  if(argc < 2)
    return 1;

  // Client object is notified
  struct WebVTTFileParser : public Client
  { 
    WebVTTFileParser(const char *file)
      : fileName(file)
    { 
      parser = new WebVTTParser(buffer, this);
    }

    ~WebVTTFileParser()
    {
      delete parser;
    }

    // Read and parse the file in chunks of 4096 bytes
    bool parse()
    {
      file.open(fileName.c_str(), std::ios::in|std::ios::binary);
      if(!file.good())
        return false;
      char buf[0x1000] = "";
      while(true) {
        bool final = false;
        std::streamsize n = file.readsome(buf, sizeof(buf));
        if(n < 0x1000)
          final = true;
        buffer.refill(buf, n, final);
        if(!parser->parse())
          return false;
        if(final)
          break;
      }
      return true;
    }

    // This message is called by the parser to notify the client that
    // there are new Cues available
    void cuesAvailable()
    {
      List<Cue> newCues;
      parser->parsedCues(newCues);
      myCues += newCues;
    }
    SynchronousBuffer buffer;
    WebVTTParser *parser;
    std::string fileName;
    std::ifstream file;
    List<Cue> myCues;
  };

  WebVTTFileParser p(argv[1]);
  if(p.parse()) {
    // Create a Node visitor to visit and perform some action on each Node in the
    // tree of CueText Nodes.
    //
    // This one will print TextNodes at indents depending on what level branch
    // they are on.
    class Visitor : public NodeVisitor
    {
    public:
      Visitor() {}
      bool enter(const Node &node)
      {
        stack.push(node);
        return true;
      }
      void leave(const Node &node)
      {
        stack.pop();
      }
      void visit(Node &node)
      {
        if(node.element() == TextNode) {
          Node parent;
          sb.setText(node.text());

          // Do nothing for empty text
          if(sb.isEmpty())
            return;
          // Use HTML newlines instead
          sb.replaceAll('\n', "<br />");
          // Escape markdown characters
          sb.replaceAll('*', "&#42;");
          sb.replaceAll('_', "&#95;");
          sb.replaceAll('-', "&#45;");
          if(stack.lastItem(parent)) {
            switch(parent.element()) {
              case BoldNode:
                // Emphasize text
                sb.prepend("**");
                sb.append("**");
                break;

              case ItalicNode:
                sb.prepend("*");
                sb.append("*");
                break;

              default:
                // Markdown doesn't support underline out of the box :(
                break;
            }
          }
          ::printf("%s", sb.text());
        }
      }
      StringBuilder sb;
      List<Node> stack;
    };
  
    for(List<Cue>::iterator it = p.myCues.begin(); it < p.myCues.end(); ++it) {
      // Render the cue in Markdown
      // Parse the CueText for the given cue, and walk it.
      TimedText::WebVTTParser::parseCuetext(*it);
      Visitor v;
      if(!it->id().isEmpty())
        ::printf("#### %s<br />", it->id().text());
      // Print the start time and end time of the cue, in seconds (double)
      ::printf("*%f --> %f*<br />", it->startTime().toSeconds(),
               it->endTime().toSeconds());
      it->visit(v);
      ::printf("\n\n---\n\n");
    }
  }
}
