#ifndef MUTEX_H
#define MUTEX_H
#include "Nocopyable.h"
#include <pthread.h>
SUNNY_BEGIN
class Mutex : sunny::Nocopyable
{
private:
  pthread_mutex_t mutex_;
  pthread_mutexattr_t attr_;
  friend class Condition;
public:
  enum RecursionMode { NonRecursive, Recursive };
  explicit Mutex(RecursionMode mode = NonRecursive)
  {
    if (mode == NonRecursive)
      pthread_mutex_init(&mutex_, NULL);
    else {
      pthread_mutexattr_init(&attr_);
      pthread_mutexattr_settype(&attr_, PTHREAD_MUTEX_RECURSIVE);
      pthread_mutex_init(&mutex_, &attr_);
    }
  }

  ~Mutex()
  {
    pthread_mutexattr_destroy(&attr_);
    pthread_mutex_destroy(&mutex_);
  }

  bool isRecursive()
  {
    int type;
    pthread_mutexattr_gettype(&attr_, &type);
    return type == PTHREAD_MUTEX_RECURSIVE;
  }

  void lock()
  {
    pthread_mutex_lock(&mutex_);
  }

  void unlock()
  {
    pthread_mutex_unlock(&mutex_);
  }

  pthread_mutex_t* getBaseMutex()
  {
    &mutex_;
  }
};

class MutexLocker : sunny::Nocopyable
{
private:
  Mutex& mutex_;
public:
  MutexLocker(Mutex& mutex)
    : mutex_(mutex)
  {
    mutex_.lock();
  }

  ~MutexLocker()
  {
    mutex_.unlock();
  }
};

SUNNY_END
#endif // MUTEX_H
