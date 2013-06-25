#ifndef __TimedText_String__
#define __TimedText_String__

#include <TimedText/Unicode.h>
#include <cstring>

namespace TimedText
{

class StringBuilder;

// String is an immutable, shared, UTF-8 encoded representation of text.
class String
{
public:
	explicit String(const char *utf8, int len=-1);
	String(const String &str);
	String &operator=(const String &str);

	// Returns true if the string is empty or NULL.
	inline bool isEmpty() const {
    return d->length == 0;
  }

  inline int length() const {
    return d->length;
  }

  inline int size() const {
    return d->length;
  }

	inline const char *text() const {
		return d->text;
	}

  int indexOf(const char *text, int len=-1) const;
  inline int indexOf(const String &str) const {
    return indexOf(str.text(), str.length());
  }

  // Return true if string contains 'text'
  bool contains(const char *text, int len=-1) const;
  inline bool contains(const String &other) const {
    return contains(other.text(),other.length());
  }
  template <size_t N>
  inline bool contains(const char (&arr)[N]) {
    if(N>0 && arr[N-1]=='\0')
      return contains(arr,N-1);
    return contains(arr, N);
  }
  bool contains(unsigned long ucs4) const;

  // Return true if string starts with 'text'
  bool startsWith(const char *text, int len=-1) const;
  inline bool startsWith(const String &other) const {
    return startsWith(other.text(),other.length());
  }
  template <size_t N>
  inline bool startsWith(const char (&arr)[N]) {
    if(N>0 && arr[N-1]=='\0')
      return startsWith(arr,N-1);
    return startsWith(arr, N);
  }
  bool startsWith(unsigned long ucs4) const;

  // Return true if string ends with 'text'
  bool endsWith(const char *text, int len=-1) const;
  inline bool endsWith(const String &other) const {
    return endsWith(other.text(),other.length());
  }
  template <size_t N>
  inline bool endsWith(const char (&arr)[N]) {
    if(N>0 && arr[N-1]=='\0')
      return endsWith(arr,N-1);
    return endsWith(arr, N);
  }
  bool endsWith(unsigned long ucs4) const;

private:
  friend class StringBuilder;
  static int findString(const char *bucket, int bucket_len, int from,
                        const char *needle, int needle_len);
  static int findStringBoyerMoore(const char *bucket, int bucket_len, int from,
                                  const char *needle, int needle_len);
	struct Data
	{
		int ref;
		int length;
		char text[1];
	};
	Data *d;
  static Data sharedEmpty;
  static Data sharedNull;
};

} // TimedText

#endif // __TimedText_String__