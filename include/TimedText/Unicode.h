#ifndef __TimedText_Unicode__
#define __TimedText_Unicode__

namespace TimedText
{

// Unicode contains static methods for converting between unicode encodings
class Unicode
{
public:
  enum {
  	// Surrogate markers
  	HighSurrogateBegin = 0xD800,
  	HighSurrogateEnd = 0xDBFF,
  	LowSurrogateBegin = 0xDC00,
  	LowSurrogateEnd = 0xDFFF,

  	// Allocated block of unicode non-characters in Arabic Presentation Forms script
  	NonCharBegin = 0xFDD0,
  	NonCharEnd = 0xFDEF,

    // Mask used to easily determine if a codepoint is within the non-character block
    // for its script.
  	NonCharMask = 0xFFFE,

    // Characters beyond this point are not allocated unicode characters
  	CodepointLimit = 0x10FFFF,
  };

  // Returns true if a single UCS4 codepoint is a unicode non-character, otherwise
  // return false.
  static bool isNonChar(unsigned long ucs4)
  {
  	return ucs4 >= NonCharBegin && 
  	      (ucs4 <= NonCharEnd || (ucs4 & NonCharMask) == NonCharMask) &&
  	      ucs4 <= CodepointLimit;
  }

  // Return true if a single UCS4 codepoint is a unicode character, otherwise
  // return false.
  static bool isChar(unsigned long ucs4)
  {
  	return ucs4 < HighSurrogateBegin ||
  		   (ucs4 > LowSurrogateEnd && ucs4 <= CodepointLimit &&
  		    !isNonChar(ucs4));
  }

  // Return length in bytes of Unicode character encoded as UTF8
  // The character is assumed to be a valid Unicode character, and
  // does not test with isChar().
  static int utf8Length(unsigned long ucs4)
  {
  	// Ternary madness.
  	return ucs4 < 0x80 ? 1
  	     : ucs4 < 0x800 ? 2
  	     : ucs4 < 0xD800 ? 3
  	     : ucs4 < 0xE000 || ucs4 > CodepointLimit ? 0
  	     : ucs4 < 0x10000 ? 3 : 4;
  }

  static bool utf8IsLead(char c)
  {
  	return ((unsigned char)(c - 0xC0)) < 0x3E;
  }

  static bool utf8IsTrail(char c)
  {
  	return ((unsigned char)(c & 0xC0)) == 0x80;
  }

  static bool utf8IsSingle(char c)
  {
  	return ((unsigned char)(c & 0x80)) == 0;
  }

  static int utf8NumTrailBytes(char leadByte)
  {
  	unsigned char c = (unsigned char)leadByte;
  	return (c < 0xF0 ? (c >= 0xC0) + (c >= 0xE0) :
  		   (c < 0xFE ? 3 + (c >= 0xF8) + (c >= 0xFC) : 0));
  }

  static void utf8MaskLeadByte(char &leadByte, int trailBytes)
  {
  	unsigned char c = (unsigned char)leadByte;
  	leadByte = char(c & (1 << (6 - trailBytes)) - 1);
  }

  // Return length in bytes of validated UTF8 string (Unicode non-characters
  // are converted to replacement character)
  static int utf8Length(const char *utf8, int len);

  // Convert a potentially invalid UTF8 string into a valid UTF8 string,
  // assuming an adequate buffer size. Unicode non-characters are converted
  // to replacement character.
  static bool toValidUtf8(char *out, int alloc, int &olen,
  	                      const char *in, int len);

  // Convert a single UCS4 character to UTF8 ('out' is assumed to be
  // large enough to contain a UTF8 character). Returns false if the
  // character is not a unicode character, in which case the replacement
  // character is stored in the buffer.
  static bool toUtf8(unsigned long ucs4, char *out, int &len);
};

}

#endif
