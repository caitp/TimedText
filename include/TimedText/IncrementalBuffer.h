#ifndef __TimedText_IncrementalBuffer__
#define __TimedText_IncrementalBuffer__

#include <TimedText/String.h>

namespace TimedText
{

// Forward declare base IncrementalBufferDetail.
class IncrementalBufferDetail;

// The IncrementalBuffer is a type of buffer designed to
// support asynchronous parsing.
class IncrementalBuffer
{
public:
  IncrementalBuffer(IncrementalBufferDetail *d);
  ~IncrementalBuffer();

	bool eof() const;
  String getline() const;
  int next();
  int read(char buffer[], int maximum);
  void discard(int bytes);
  void bump();

private:
  IncrementalBufferDetail *d_ptr;
};

} // TimedText

#endif // __TimedText_IncrementalBuffer__
