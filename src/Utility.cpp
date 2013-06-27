#include "Utility.h"

namespace TimedText
{

int
allocMore(int alloc, int extra)
{
  const int pageSize = 1 << 12; // 4 kB
  int n;
  alloc += extra;
  if (alloc < 1<<6) {
    // Small allocation
    n = (1<<3) + ((alloc >>3) << 3);
  } else  {
    // Medium or large allocations
    n = (alloc < pageSize) ? 1 << 3 : pageSize;
    while (n < alloc) {
      // Test for wrap around, do not exceed INT_MAX
      if (n <= 0)
        return INT_MAX;
      n <<= 1;
    }
  }
  return n - extra;
}


} // TimedText
