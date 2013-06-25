#ifndef __TimedText_IncrementalBuffer_Detail__
#define __TimedText_IncrementalBuffer_Detail__

#include <TimedText/IncrementalBuffer.h>

namespace TimedText
{

// This object is shared between the thread controlling the
// parser, and the parser's thread itself (they should not be
// the same thread).
//
// Details regarding locking/unlocking of buffers is left up to
// the implementation.
class IncrementalBufferDetail
{
public:
  virtual ~Detail() = 0;

  // Return true if the end of the document has been reached.
  // Otherwise return false
  virtual bool eof() const = 0;

  // Discard bytes from the front of the buffer
  virtual void discard(int bytes) = 0;

  // Read up to 'maximum' bytes from the front of the buffer
  // do not discard them.
  virtual int read(char buffer[], int maximum) = 0;
};

} // TimedText

#endif // __TimedText_IncrementalBuffer_Detail__
