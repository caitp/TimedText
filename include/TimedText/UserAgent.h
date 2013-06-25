#ifndef __TimedText_UserAgent__
#define __TimedText_UserAgent__

#include <TimedText/Cue.h>
#include <TimedText/Error.h>

namespace TimedText
{

class UserAgent
{
public:
  virtual ~UserAgent() {}
  virtual void parsedCue(Cue *cue);
  virtual void reportError(Error *error);
};

}

#endif // __TimedText_UserAgent__
