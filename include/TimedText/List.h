//
// Copyright (c) 2013 Caitlin Potter and Contributors
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//  * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#ifndef __TimedText_List__
#define __TimedText_List__

#include <TimedText/Types.h>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <new>

//
// List container object, to avoid depending on the STL.
// Work inspired by QList, and certain hacks are taken directly from there.
// It is different in that it operates in an entirely exception-free
// environment, and is intended to be used with objects whose constructors
// never throw.
//
// Additionally, the atomic int infrastructure is hidden in order to simplify
// the use of the library, just as it is in the case of the String class.
//
// Further differences from the Qt implementation include method names similar to
// those from perl (shift/unshift/push/pop) for manipulating lists, no assertions,
// entirely unsafe code is private. Any unsafe action has a boolean result, which
// should be checked in all cases.
//

namespace TimedText
{

// ListData is PRIVATE, and is only exposed to allow typed List operations to be
// implemented entirely in the header file. Don't break it!
struct ListData
{
  struct Data;
  Data *d;
  static Data *sharedEmpty();
  static Data *sharedNull();

  Data *detach(int alloc);
  Data *detach_grow(int *i, int n);
  bool realloc(int alloc);
  void **erase(void **xi);
  inline void **append(const ListData &other) {
    return append(size());
  }
  void **append(int n);
  void **append();
  void **insert(int i);
  void **prepend();
  bool remove(int i);
  bool remove(int i, int n);
  int size() const;
  int alloc() const;
  void **begin() const;
  void **end() const;
  void **at(int i) const;

  // Wrappers for internal routines which can't be visible
  // to the header
  bool ref();
  bool deref();
  bool unique() const;

  void kill();
  void undo_unshift();
  void undo_push();
  void undo_push(int n);
};

template <typename T>
class List
{
  struct Node {
    void *v;
    inline T &get() {
      return *reinterpret_cast<T*>(
        TypeInfo<T>::isLarge || TypeInfo<T>::isStatic ? v : this
      );
    }
  };

public:
  List() : d(ListData::sharedEmpty()) { p.ref(); }
  List(const List<T> &other) : d(other.d) {
    p.ref();
  }

  List &operator=(const List &other) {
    ListData x = { d };
    d = other.d;
    p.ref();
    if(!x.deref()) {
      freeData(x.d);
    }
    return *this;
  }
  ~List() {
    if(d && !p.deref()) {
      freeData(d);
    }
  }

  List<T> &operator+=(const List<T> &other)
  {
    if(!other.isEmpty()) {
      if(isEmpty())
        *this = other;
      else {
        Node *n = p.unique()
                ? detachHelperGrow(INT_MAX, other.size())
                : reinterpret_cast<Node *>(p.append(other.p));
        if(!nodeCopy(n, reinterpret_cast<Node *>(p.end()),
                     reinterpret_cast<Node *>(other.p.begin())))
          p.undo_push(int(reinterpret_cast<Node *>(p.end()) - n));
      }
    }
    return *this;
  }

  void clear() { *this = List<T>(); }

  bool detach() {
    return p.unique() || detachHelper();
  }

  // All synonyms for the same thing (length of list)
  inline int length() const { return p.size(); }
  inline int size() const { return length(); }
  inline int count() const { return length(); }

  inline bool isEmpty() const {
    return length() == 0;
  }

  // Return true if the list contains 'item'
  inline bool contains(const T &item) const {
    Node *b = reinterpret_cast<Node *>(p.begin());
    Node *i = reinterpret_cast<Node *>(p.end());
    while(i-- != b)
      if(i->get() == item)
        return true;
    return false;
  }

  inline bool itemAt(int i, T &result) const {
    if(i < 0 || i >= p.size())
      return false;
    result = reinterpret_cast<Node *>(p.at(i))->get();
    return true;
  }

  inline bool ptrTo(int i, T *&result) {
    if(i < 0 || i >= p.size()) {
      result = 0;
      return false;
    }
    result = &reinterpret_cast<Node *>(p.at(i))->get();
    return true;
  }

  // WARNING: This routine performs no bounds checking.
  // Only call if absolutely certain that it's within bounds.
  inline T &operator[](int i) {
    return reinterpret_cast<Node *>(p.at(i))->get();
  }

  // Insert into arbitrary position
  inline bool insert(int i, const T &item) {
    if(!p.unique()) {
      Node *n = detachHelperGrow(i, 1);
      if(!n)
        return false;
      if(!nodeConstruct(n, item)) {
        p.remove(i);
        return false;
      }
    } else {
      if(TypeInfo<T>::isLarge || TypeInfo<T>::isStatic) {
        Node *n = reinterpret_cast<Node *>(p.insert(i));
        if(!n)
          return false;
        if(!nodeConstruct(n,item)) {
          p.remove(i);
          return false;
        }
      } else {
        Node *n, copy;
        nodeConstruct(&copy, item);
        if(!(n = reinterpret_cast<Node *>(p.insert(i)))) {
          nodeDestruct(&copy);
          return false;
        }
        *n = copy;
      }
    }
    return true;
  }

  // Take from arbitrary position
  inline bool take(int i, T &result) {
    if(i < 0 || i >= p.size())
      return false;
    detach();
    Node *n = reinterpret_cast<Node *>(p.at(i));
    if(!n)
      return false;
    result = n->get();
    nodeDestruct(n);
    p.remove(i);
    return true;
  }

  // Add to beginning or end
  inline bool unshift(const T &item) {
    if(!p.unique()) {
      Node *n = detachHelperGrow(0, 1);
      if(!n)
        return false;
      if(!nodeConstruct(n, item)) {
        p.undo_unshift();
        return false;
      }
    } else {
      if(TypeInfo<T>::isLarge || TypeInfo<T>::isStatic) {
        Node *n = reinterpret_cast<Node *>(p.prepend());
        if(!n)
          return false;
        if(!nodeConstruct(n, item)) {
          p.undo_unshift();
          return false;
        }
      } else {
        Node *n, copy;
        nodeConstruct(&copy, item);
        if(!(n = reinterpret_cast<Node *>(p.prepend()))) {
          nodeDestruct(&copy);
          return false;
        }
        *n = copy;
      }
    }
    return true;
  }

  inline bool push(const T &item) {
    if(!p.unique()) {
      Node *n = detachHelperGrow(INT_MAX, 1);
      if(!n)
        return false;
      if(!nodeConstruct(n, item)) {
        p.undo_push();
        return false;
      }
    } else {
      if(TypeInfo<T>::isLarge || TypeInfo<T>::isStatic) {
        Node *n = reinterpret_cast<Node *>(p.append());
        if(!n)
          return false;
        if(!nodeConstruct(n, item)) {
          p.undo_push();
          return false;
        }
      } else {
        Node *n, copy;
        nodeConstruct(&copy, item);
        if(!(n = reinterpret_cast<Node *>(p.append()))) {
          nodeDestruct(&copy);
          return false;
        }
        *n = copy;
      }
    }
    return true;
  }

  inline bool shift() {
    if(!p.size())
      return false;
    removeFirst();
    return true;
  }
  // Take from beginning or end
  inline bool shift(T &result) {
    if(!p.size())
      return false;
    result = first();
    removeFirst();
    return true;
  }

  inline bool pop() {
    if(!p.size())
      return false;
    removeLast();
    return true;
  }
  inline bool pop(T &result) {
    if(!p.size())
      return false;
    result = last();
    removeLast();
    return true;
  }

  class iterator
  {
  public:
    Node *i;
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;

    inline iterator() : i(0) {}
    inline iterator(Node *n) : i(n) {}
    inline iterator(const iterator &o) : i(o.i) {}
    inline T &operator*() const { return i->get(); }
    inline T *operator->() const { return &i->get(); }
    inline T &operator[](int j) const { return (i+j)->get(); }
    inline bool operator==(const iterator &o) const { return i == o.i; }
    inline bool operator!=(const iterator &o) const { return i != o.i; }
    inline bool operator<(const iterator &o) const { return i < o.i; }
    inline bool operator<=(const iterator &o) const { return i <= o.i; }
    inline bool operator>(const iterator &o) const { return i > o.i; }
    inline bool operator>=(const iterator &o) const { return i >= o.i; }

    inline iterator &operator++() { ++i; return *this; }
    inline iterator operator++(int) { Node *n = i; ++i; return n; }
    inline iterator &operator--() { --i; return *this; }
    inline iterator operator--(int) { Node *n = i; --i; return n; }
    inline iterator &operator+=(int j) { i += j; return *this; }
    inline iterator operator+(int j) const { return iterator(i+j); }
    inline iterator &operator-=(int j) { i -= j; return *this; }
    inline iterator operator-(int j) const { return iterator(i-j); }
    inline int operator+(iterator j) const { return int(i + j.i); }
    inline int operator-(iterator j) const { return int(i - j.i); }
  };

  class const_iterator
  {
  public:
    Node *i;
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;

    inline const_iterator() : i(0) {}
    inline const_iterator(Node *n) : i(n) {}
    inline const_iterator(const const_iterator &o) : i(o.i) {}
    inline T &operator*() const { return i->get(); }
    inline T *operator->() const { return &i->get(); }
    inline T &operator[](int j) const { return (i+j)->get(); }
    inline bool operator==(const const_iterator &o) const { return i == o.i; }
    inline bool operator!=(const const_iterator &o) const { return i != o.i; }
    inline bool operator<(const const_iterator &o) const { return i < o.i; }
    inline bool operator<=(const const_iterator &o) const { return i <= o.i; }
    inline bool operator>(const const_iterator &o) const { return i > o.i; }
    inline bool operator>=(const const_iterator &o) const { return i >= o.i; }

    inline const_iterator &operator++() { ++i; return *this; }
    inline const_iterator operator++(int) { Node *n = i; ++i; return n; }
    inline const_iterator &operator--() { --i; return *this; }
    inline const_iterator operator--(int) { Node *n = i; --i; return n; }
    inline const_iterator &operator+=(int j) { i += j; return *this; }
    inline const_iterator operator+(int j) const { return const_iterator(i+j); }
    inline const_iterator &operator-=(int j) { i -= j; return *this; }
    inline const_iterator operator-(int j) const { return const_iterator(i-j); }
    inline int operator+(const_iterator j) const { return int(i + j.i); }
    inline int operator-(const_iterator j) const { return int(i - j.i); }
  };

  inline iterator begin() {
    detach();
    return reinterpret_cast<Node *>(p.begin()); 
  }

  inline const_iterator begin() const {
    return reinterpret_cast<Node *>(p.begin()); 
  }

  inline iterator end() {
    detach();
    return reinterpret_cast<Node *>(p.end());
  }

  inline const_iterator end() const {
    return reinterpret_cast<Node *>(p.end());
  }

  iterator insert(iterator before, const T &item) {
    int i = int(before.i - reinterpret_cast<Node *>(p.begin()));
    Node *n = reinterpret_cast<Node *>(p.insert(i));
    if(!n)
      return end();
    if(!nodeConstruct(n, item)) {
      p.remove(i);
      return end();
    }
    return n;
  }

  iterator erase(iterator it) {
    nodeDestruct(it.i);
    return reinterpret_cast<Node *>(p.erase(reinterpret_cast<void **>(it.i)));
  }

  iterator erase(iterator first, iterator last) {
    for(Node *n = first.i; n < last.i; ++n)
      nodeDestruct(n);
    int idx = first - begin();
    p.remove(idx, last - first);
    return begin() + idx;
  }

  inline void removeFirst() {
    if(!isEmpty())
      erase(begin());
  }

  inline void removeLast() {
    if(!isEmpty())
      erase(--end());
  }

  // Return the last item without removing it.
  inline bool lastItem(T &result) {
    return itemAt(length()-1, result);
  }

  inline bool lastPtr(T *&result) {
    return ptrTo(length()-1, result);
  }

  // Return the first item without removing it.
  inline bool firstItem(T &result) {
    return itemAt(0, result);
  }

  inline bool firstPtr(T *&result) {
    return ptrTo(0, result);
  }

private:
  union { ListData p; ListData::Data *d; };

  inline const T &at(int i) const {
    return reinterpret_cast<Node *>(p.at(i))->get();
  }
  inline T &first() { return *begin(); }
  inline const T &first() const { return at(0); }
  inline T &last() { return *(--end()); }
  inline const T &last() const { return at(count() - 1); }

  inline bool nodeConstruct(Node *n, const T &item) {
    if(TypeInfo<T>::isLarge || TypeInfo<T>::isStatic)
      return !!(n->v = new T(item));
    else if(TypeInfo<T>::isComplex) new (n) T(item);
#if (defined(__GNUC__) || defined(__INTEL_COMPILER) || \
     defined(__IBMCPP__)) && !defined(__OPTIMIZE__)
    // This violates pointer aliasing rules, but it is known to be safe (and
    // silent) in unoptimized GCC builds (-fno-strict-aliasing). The other
    // compilers which set the same define are assumed to be safe.
    else *reinterpret_cast<T *>(n) = item;
#else
    // This is always safe, but penalizes unoptimized builds a lot.
    else ::memcpy(n, &item, sizeof(T));
#endif
    return true;
  }

  inline void nodeDestruct(Node *n) {
    if(TypeInfo<T>::isLarge || TypeInfo<T>::isStatic)
      delete reinterpret_cast<T*>(n->v);
    else if(TypeInfo<T>::isComplex)
      reinterpret_cast<T*>(n)->~T();
  }

  inline void nodeDestruct(Node *from, Node *to) {
    if(TypeInfo<T>::isLarge || TypeInfo<T>::isStatic)
      while(from != to) --to, delete reinterpret_cast<T *>(to->v);
    else if(TypeInfo<T>::isComplex)
      while(from != to) --to, reinterpret_cast<T *>(to)->~T();
  }

  inline bool nodeCopy(Node *from, Node *to, Node *src) {
    Node *current = from;
    if(TypeInfo<T>::isLarge || TypeInfo<T>::isStatic) {
      while(current != to) {
        if(!(current->v = new T(*reinterpret_cast<T *>(src->v)))) {
          // If this fails, roll back.
          while(current-- != from)
            delete reinterpret_cast<T *>(current->v);
          return false;
        }
        ++current;
        ++src;
      }
    } else if(TypeInfo<T>::isComplex) {
      while(current != to) {
        new (current) T(*reinterpret_cast<T *>(src));
        ++current;
        ++src;
      }
    } else {
      if(src != from && to - from > 0)
        ::memcpy(from, src, (to - from) * sizeof(Node *));
    }
    return true;
  }

  Node *detachHelperGrow(int i, int c) {
    Node *n = reinterpret_cast<Node *>(p.begin());
    ListData::Data *x = p.detach_grow(&i, c);
    if(!nodeCopy(reinterpret_cast<Node *>(p.begin()),
                 reinterpret_cast<Node *>(p.begin() + i), n)) {
      freeData(d);
      d = x;
      return 0;
    }
    if(!nodeCopy(reinterpret_cast<Node *>(p.begin() + i + c),
                 reinterpret_cast<Node *>(p.end()), n + i)) {
      freeData(d);
      d = x;
      return 0;
    }
    ListData tmp = { x };
    if(!tmp.deref())
      freeData(x);

    return reinterpret_cast<Node *>(p.begin() + i);
  }

  bool detachHelper(int alloc) {
    Node *n = reinterpret_cast<Node *>(p.begin());
    ListData::Data *x = p.detach(alloc);
    if(!x)
      return false;
    if(!nodeCopy(reinterpret_cast<Node *>(p.begin()),
                 reinterpret_cast<Node *>(p.end()), n)) {
      freeData(d);
      d = x;
      return false;
    }
    ListData tmp = { x };
    if(!tmp.deref())
      freeData(x);
    return true;
  }

  inline bool detachHelper() {
    return detachHelper(p.alloc());
  }

  void freeData(ListData::Data *x) {
    ListData tmp = { x };
    nodeDestruct(reinterpret_cast<Node *>(tmp.begin()),
                 reinterpret_cast<Node *>(tmp.end()));

    tmp.kill();
  }
};

} // TimedText

#endif // __TimedText_List__
