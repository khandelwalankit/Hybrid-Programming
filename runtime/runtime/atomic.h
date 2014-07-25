#ifndef _ATOMIC_H
#define _ATOMIC_H

/**
 * Atomic type adapted from http://golubenco.org/2007/06/14/atomic-operations/
 */

template <class T>
class atomic_t
{
 public:
  atomic_t() : counter(0) { }
  atomic_t(T counter) : counter(counter) { }

  T read() const { return counter; }
  void set(T i) { counter = i; }

  // increment by i and return new value
  T add(T i) { return __sync_add_and_fetch(&counter, i); }
  T operator+=(T i) { return add(i); }
  // decrement by i and return new value
  T sub(T i) { return __sync_sub_and_fetch(&counter, i); }
  T operator-=(T i) { return sub(i); }

  /**
   * Atomically subtracts @i and returns
   * true if the result is zero, or false for all
   * other cases.
   */
  bool sub_and_test(T i) { return !(__sync_sub_and_fetch(&counter, i)); }

  // increment by 1 and return new value
  T inc() { return __sync_add_and_fetch(&counter, 1); }
  // decrement by 1 and return new value
  T dec() { return __sync_sub_and_fetch(&counter, 1); }

  /**
   * Atomically decrements by 1 and
   * returns true if the result is 0, or false for all other
   * cases.
   */
  bool dec_and_test() { return !(__sync_sub_and_fetch(&counter, 1)); }

  /**
   * Atomically increments by 1
   * and returns true if the result is zero, or false for all
   * other cases.
   */
  bool inc_and_test() { return !(__sync_add_and_fetch(&counter, 1)); }

  /**
   * Atomically adds @i and returns true
   * if the result is negative, or false when
   * result is greater than or equal to zero.
   */
  bool add_negative(T i) { return (__sync_add_and_fetch(&counter, i) < 0); }
  
 private:
  volatile T counter;
};


#endif
