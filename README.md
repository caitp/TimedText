TimedText
=========

1. [Build Status](#build-status)
2. [About](#about)
3. [API Reference](#api-reference)
4. [License](#license)

### Build Status [![Build Status](https://travis-ci.org/caitp/TimedText.png?branch=master)](https://travis-ci.org/caitp/TimedText)

### About

This project is intended as a replacement for the [Mozilla WebVTT](https://github.com/mozilla/webvtt/)
library.

The general goals of the project are to A) include support for the TTML
specification, and B) to improve API usability for WebVTT.

This library includes libexpat, though support for alternative XML
libraries, though support for alternative XML libraries such as
[PugiXML](http://pugixml.org/) may be added later on, for certain use-cases. As the primary
use-case is use within a browser, stream parsing is a nice thing to
have.

[back to top...](#timedtext)

### API Reference
I've added a demo, for your viewing pleasure! This will be moved to the Wiki page soon,
but I thought it would be a good idea to give an idea how this library can be used. It
is of course capable of much more than this, of course. An easier to read API reference
is coming soon.

Example stream-oriented WebVTT Parsing and displaying on the console:
```C++

#include <TimedText/WebVTTParser.h>
#include <TimedText/SynchronousBuffer.h>
#include <fstream>
#include <string>
#include <cstdio>

int
main(int argc, char **argv) {
  // Use first command line parameter as filename
  if(argc < 2)
    return 1;

  // Client object is notified
  struct WebVTTFileParser : public TimedText::Client
  { 
    WebVTTFileParser(const char *file)
      : fileName(file)
    { 
      parser = new TimedText::WebVTTParser(buffer, this);
    }

    ~WebVTTFileParser()
    {
      delete parser;
    }

    // Read and parse the file in chunks of 4096 bytes
    bool parse() {
      file.open(fileName, std::ios::in|std::ios::binary);
      if(!file.good())
        return false;
      char buf[0x1000] = "";
      while(!file.eof()) {
        std::streamsize n = file.readsome(buf, sizeof(buf));
        buffer.refill(buf, n, file.eof());
        if(!parser->parse())
          return false;
      }
      return true;
    }
    
    typedef TimedText::List<TimedText::Cue> CueList;
    // This message is called by the parser to notify the client that
    // there are new Cues available
    void parsedCues() {
      CueList newCues;
      parser->parsedCues(newCues);
      myCues += newCues;
    }
    TimedText::SynchronousBuffer buffer;
    TimedText::WebVTTParser *parser;
    std::string fileName;
    std::ifstream file;
    CueList myCues;
  };

  WebVTTFileParser p(argv[1]);
  if(p.parse()) {
    // Create a Node visitor to visit and perform some action on each Node in the
    // tree of CueText Nodes.
    //
    // This one will print TextNodes at indents depending on what level branch
    // they are on.
    class Visitor : public TimedText::NodeVisitor
    {
    public:
      Visitor() : indent(0) {}
      bool enter(const Node &node) {
        indent += 2;
        return true;
      }
      void leave(const Node &node) {
        indent -= 2;
      }
      void visit(Node &node) {
        if(node.element() == TimedText::TextNode) {
          ::printf("%*s%s%c", indent, "", node.text(),
                   node.text().endsWith('\n') ? '\0' : '\n');
        }
      }
      int indent;
    };
    for(TimedText::List<TimedText::Cue>::iterator it = p.myCues.begin(); it < p.myCues.end(); ++it) {
      // Print the Cue ID, if one is present
      if(!it->id().isEmpty())
        ::printf("%s\n", it->id());
      // Print the start time and end time of the cue, in seconds (double)
      ::printf("%f --> %\nf", it->startTime().toSeconds(), it->endTime().toSeconds());
      // Parse the CueText for the given cue, and walk it.
      TimedText::WebVTTParser::parseCuetext(*it);
      Visitor v;
      it->visit(v);
    }
  }
}
```
[back to top...](#timedtext)

### License
Copyright (c) 2013 Caitlin Potter and Contributors
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

 * Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

[back to top...](#timedtext)
