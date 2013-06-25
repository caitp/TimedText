#include <TimedText/Unicode.h>
#include <cstring>

namespace TimedText
{

static const int numUtf8Sequences = 7;
static const int maxUtf8Bytes = 4;
static const int offsetUtf8Sequences[maxUtf8Bytes + 1] =
{
  0, // 1 byte
  1, // 2 bytes
  2, // 3 bytes
  4, // 4 bytes
  numUtf8Sequences, // must be last
};

static const struct validUtf8Sequence
{
  unsigned long lowChar;
  unsigned long highChar;
  int numBytes;
  char validBytes[8];
} validUtf8[numUtf8Sequences] =
{
//Low        High      #, Byte 1       Byte 2      Byte 3      Byte 4
  {0x0000,   0x007F,   1, {0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
  {0x0080,   0x07FF,   2, {0xC2, 0xDF, 0x80, 0xBF, 0x00, 0x00, 0x00, 0x00}},
  {0x0800,   0x0FFF,   3, {0xE0, 0xE0, 0xA0, 0xBF, 0x80, 0xBF, 0x00, 0x00}},
  {0x1000,   0xFFFF,   3, {0xE1, 0xEF, 0x80, 0xBF, 0x80, 0xBF, 0x00, 0x00}},
  {0x10000,  0x3FFFF,  4, {0xF0, 0xF0, 0x90, 0xBF, 0x80, 0xBF, 0x80, 0xBF}},
  {0x40000,  0xFFFFF,  4, {0xF1, 0xF3, 0x80, 0xBF, 0x80, 0xBF, 0x80, 0xBF}},
  {0x100000, 0x10FFFF, 4, {0xF4, 0xF4, 0x80, 0x8F, 0x80, 0xBF, 0x80, 0xBF}} 
};

bool
Unicode::toUtf8(unsigned long ucs4, char *out, int &len)
{
  const char replacement[] = {0xEF,0xBF,0xBD};
  bool isUC = isChar(ucs4);
  if(!isUC) {
    memcpy(out,replacement,3);
    len = 3;
    return false;
  } else {
    if(ucs4 < 0x80) {
      out[0] = (char)ucs4;
      len = 1;
    } else if(ucs4 < 0x800) {
      out[0] = 0xC0 | (ucs4 >> 6);
      out[1] = 0x80 | (ucs4 & 0x3F);
      len = 2;
    } else if(ucs4 < 0x10000) {
      out[0] = 0xE0 | (ucs4 >> 12);
      out[1] = 0x80 | ((ucs4 >> 6) & 0x3F);
      out[2] = 0x80 | (ucs4 & 0x3F);
      len = 3;
    } else if(ucs4 < 0x200000) {
      out[0] = 0xF0 | (ucs4 >> 18);
      out[1] = 0x80 | ((ucs4 >> 12) & 0x3F);
      out[2] = 0x80 | ((ucs4 >> 6) & 0x3F);
      out[3] = 0x80 | (ucs4 & 0x3F);
      len = 4;
    } else {
      // This should not be possible, because the non-character
      // branch would be used instead.
    }
    return true;
  }
}

int
Unicode::utf8Length(const char *utf8, int len)
{
  int n = 0;
  if(!utf8)
    return 0;
  if(len < 0)
    len = ::strlen(utf8);
  if(len == 0)
    return 0;
  for(int i=0; i<len;) {
    char c = utf8[i++];
    if(utf8IsLead(c)) {
      // If it's a lead byte, check that it's a valid unicode char,
      // or else add 3 to length for replacement char.
      int t = utf8NumTrailBytes(c);
      utf8MaskLeadByte(c,t);
      unsigned long uc = c & 0xFF;
      int j=0;
      for(; j<t && i < len; ++j) {
        char next = utf8[i++];
        if(!utf8IsTrail(next)) {
          --i;
          break;
        }
        uc = ((uc << 6) | (next & 0x3F));
      }
      if(j==t && isChar(uc)) {
        n += utf8Length(uc);
      } else {
        n += 3;
      }
    } else if( utf8IsSingle(c) ) {
      ++n;
    } else {
      n += 3;
      for( ; i < len && !(utf8IsSingle(utf8[i]) || utf8IsLead(utf8[i])); ++i );
    }
  }
  return n;
}

bool
Unicode::toValidUtf8(char *out, int alloc, int &olen,
                     const char *in, int len)
{
  const char replacement[] = {0xEF,0xBF,0xBD};
  olen = 0;
  if(!in || !out || alloc <= 0)
    return false;
  out[0] = '\0';
  if(len < 0)
    len = ::strlen(in);
  if(len == 0)
    return true;

  for(int i=0; i<len;) {
    char c = in[i++];
    if(utf8IsLead(c)) {
      int start = i-1;
      // If it's a lead byte, check that it's a valid unicode char,
      // or else add 3 to length for replacement char.
      int t = utf8NumTrailBytes(c);
      utf8MaskLeadByte(c,t);
      unsigned long uc = c & 0xFF;
      int j = 0;
      for(; j<t && i < len; ++j) {
        char next = in[i++];
        if(!utf8IsTrail(next)) {
          --i;
          break;
        }
        uc = ((uc << 6) | (next & 0x3F));
      }
      if(j==t && isChar(uc) && olen + (t+1) < alloc) {
        ::memcpy(out + olen, in + start, t+1);
        olen += (j + 1);
      } else if(olen + 3 < alloc) {
        ::memcpy(out + olen, replacement, 3);
        olen += 3;
      } else {
        // Die if we don't have room for a multibyte character
        return false;
      }
    } else if( utf8IsSingle(c) ) {
      if(olen + 1 < alloc) {
        out[olen++] = c;
      } else {
        return false;
      }
    } else if(olen + 3 < alloc) {
      ::memcpy(out + olen, replacement, 3);
      olen += 3;
      // Skip other trailing characters.
      for( ; i < len && utf8IsTrail(in[i]); ++i );
    } else {
      return false;
    }
  }
  // Null-terminate if possible.
  if(olen<alloc)
    out[olen] = '\0';
  return true;
}

} // TimedText
