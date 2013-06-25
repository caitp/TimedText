#ifndef __TimedText_Format__
#define __TimedText_Format__

#include <TimedText/String.h>

namespace TimedText
{

enum Format
{
	FormatUnknown = 0,
	FormatWebVTT,
	FormatTTML,
};

Format detectFormat(const char *buffer, int len=-1);

static Format detectFormat(const String &string)
{
	return detectFormat(string.text(),string.length());
}

} // TimedText

#endif // __TimedText_Format__
