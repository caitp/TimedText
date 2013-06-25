#ifndef __TimedText_Cue__
#define __TimedText_Cue__

#include <TimedText/String.h>

namespace TimedText
{

class Cue
{
public:
  ~Cue();

  String id;
  Timestamp startTime;
  Timestamp endTime;
  bool pauseOnExit;
};

}

#endif // __TimedText_Cue__
