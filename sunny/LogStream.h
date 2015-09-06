#ifndef LOGSTREAM_H
#define LOGSTREAM_H
#include "Nocopyable.h"
#include <string.h>
SUNNY_BEGIN
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
  Self& operator<<(bool v);
  Self& operator<<(short);
  Self& operator<<(unsigned short);
  Self& operator<<(int);
  Self& operator<<(unsigned int);
  Self& operator<<(long);
  Self& operator<<(unsigned long);
  Self& operator<<(long long);
  Self& operator<<(unsigned long long);
  Self& operator<<(const void*);
  Self& operator<<(float v)
  {
    *this << static_cast<double>(v);
    return *this;
  }
  Self& operator<<(double);
  Self& operator<<(long double);
  Self& operator<<(char v)
  {
    buffer_.append(&v, 1);
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

  Self& operator<<(const string& v)
  {
    buffer_.append(v.c_str(), v.size());
    return *this;
  }

  void append(const char* data, int len) { buffer_.append(data, len); }
  const Buffer& buffer() const { return buffer_; }
  void resetBuffer() { buffer_.reset(); }
};

SUNNY_END
#endif // LOGSTREAM_H
