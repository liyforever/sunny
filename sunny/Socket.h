#ifndef SOCKET_H
#define SOCKET_H
#include "Nocopyable.h"
SUNNY_BEGIN
class Socket : sunny::Nocopyable
{
private:
  const int sockfd_;
public:
  explicit Socket(int sockfd)
    : sockfd_(sockfd)
  {}

  ~Socket();

  int getSockfd() const
  {
    return sockfd_;
  }
};

SUNNY_END
#endif // SOCKET_H
