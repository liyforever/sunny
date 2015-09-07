#ifndef LOGSTREAM_H
#define LOGSTREAM_H
#include "Nocopyable.h"
#include <string.h>
#include <algorithm>
#include <stdlib.h>
SUNNY_BEGIN
const char _digitsTable[] = "9876543210123456789";
const char* _zero = _digitsTable + 9;
const char _hexTable[] = "0123456789ABCDEF";

template <class Type>
size_t _convert(char *buf, Type val)
{
  Type curVal = val;
  char* curPos = buf;
  do {
    int lastVal = static_cast<int>(curVal%10);
    curVal /= 10;
    *curPos++ = _zero[lastVal];
  } while (curVal!=0);
  if (val < 0)
    *curPos++ ='-';
  *curPos = '\0';
  std::reverse(buf, curPos);
  return curPos - buf;
}

template <class Type>
size_t _convertHex(char *buf, Type val)
{
  Type curVal = val;
  char* curPos = buf;
  do {
    int lastVal = static_cast<int>(curVal%16);
    curVal /= 10;
    *curPos++ = _hexTable[lastVal];
  } while (curVal!=0);

  *curPos = '\0';
  std::reverse(buf, curPos);
  return curPos - buf;
}

template<int SIZE>
class Buffer : sunny::Nocopyable
{
private:
  char data_[SIZE];
  char* curPos_;
public:
  explicit Buffer()
    : curPos_(data_)
  {}

  ~Buffer()
  {}

  const char* data() const { return data_; }
  int length() const { return static_cast<int>(curPos_ - data_);}
  char* current() { return curPos_; }
  int avail() const { return static_cast<int>(end() - curPos_); }
  void reset() { curPos_ = data_; }
  void bzero() { ::bzero(data_, sizeof(data_)); }
  void add(size_t len) { curPos_ += len; }
  void append(const char* buf, size_t len)
  {
    memcpy(curPos_, buf, len);
    curPos_ += len;
  }

private:
  const char* end() const { return data_ + sizeof(data_); }
};

class LogStream : sunny::Nocopyable
{
private:
  typedef Buffer<50> Buffer;
  typedef LogStream Self;
  Buffer buffer_;
public:
  Self& operator<<(bool val)
  {
    buffer_.append(val ? "1" : "0", 1);
    return *this;
  }
  Self& operator<<(int val)
  {
    appendInteger(val);
    return *this;
  }
  Self& operator<<(unsigned int val)
  {
    appendInteger(val);
    return *this;
  }

  Self& operator<<(short val)
  {
    *this << static_cast<int>(val);
    return *this;
  }

  Self& operator<<(unsigned short val)
  {
    *this << static_cast<unsigned int>(val);
    return *this;
  }

  Self& operator<<(long val)
  {
    appendInteger(val);
    return *this;
  }

  Self& operator<<(unsigned long val)
  {
    appendInteger(val);
    return *this;
  }

  Self& operator<<(long long val)
  {
    appendInteger(val);
    return *this;
  }

  Self& operator<<(unsigned long long val)
  {
    appendInteger(val);
    return *this;
  }

  Self& operator<<(const void* val)
  {

  }

  Self& operator<<(float val)
  {
    *this << static_cast<double>(val);
    return *this;
  }
  Self& operator<<(double val);
  Self& operator<<(long double val);
  Self& operator<<(char val)
  {
    buffer_.append(&val, 1);
    return *this;
  }

  Self& operator<<(const char* str)
  {
    if (str) {
      buffer_.append(str, strlen(str));
    } else {
      buffer_.append("(null)", 6);
    }
    return *this;
  }

  Self& operator<<(const unsigned char* str)
  {
    return operator<<(reinterpret_cast<const char*>(str));
  }

  Self& operator<<(const string& str)
  {
    buffer_.append(str.c_str(), str.size());
    return *this;
  }

  void append(const char* data, int len) { buffer_.append(data, len); }
  const Buffer& buffer() const { return buffer_; }
  void resetBuffer() { buffer_.reset(); }
private:
  template <class Type>
  void appendInteger(Type val)
  {
    if (buffer_.avail() >= 40) {
      size_t len = _convert(buffer_.current(), val);
      buffer_.add(len);
    }
  }
};

SUNNY_END
#endif // LOGSTREAM_H
