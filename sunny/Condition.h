#ifndef CONDITION_H
#define CONDITION_H
#include <errno.h>
#include "Mutex.h"
SUNNY_BEGIN
class Condition : sunny::Nocopyable
{
private:
  Mutex& mutex_;
  pthread_cond_t cond_;
public:
  explicit Condition(Mutex& mutex)
    : mutex_(mutex)
  {
    pthread_cond_init(&cond_, nullptr);
  }
  
  ~Condition()
  {
    pthread_cond_destroy(&cond_);
  }
  
  void wait()
  {
    pthread_cond_wait(&cond_, mutex_.getBaseMutex());
  }
  
  bool waitForSeconds(int seconds)
  {
    struct timespec absTime;
    clock_gettime(CLOCK_REALTIME, &asbTime);
    absTime.tv_sec += seconds;
    return ETIMEDOUT == pthread_cond_timewait(&cond_, mutex_.getBaseMutex(), &absTime);
  }
  
  void notify()
  {
    pthread_cond_signal(&cond_);
  }
  
  void notifyAll()
  {
    pthread_cond_broadcast(&cond_);
  }
};

SUNNY_END
#endif // CONDITION_H
