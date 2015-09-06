#ifndef THREAD_H
#define THREAD_H
#include <assert.h>
#include <string>
#include <functional>
#include <atomic>
#include "Nocopyable.h"
SUNNY_BEGIN
using std::string;
class Thread : sunny::Nocopyable
{
public:
  typedef std::function<void ()> ThreadFunc;
private:
  bool started_;
  bool joined_;
  pthread_t tid_;
  ThreadFunc func_;
  string name_;
public:
  explicit Thread(const ThreadFunc& func, const string& threadName=string())
    : started_(false),
      joined_(false),
      func_(func),
      tid_(0),
      name_(threadName)
  {

  }
  
  explicit Thread(ThreadFunc&& func, const std::string& threadName=string())
    : started_(false),
      joined_(false),
      func_(std::move(func)),
      tid_(0),
      name_(threadName)
  {
    
  }

  ~Thread()
  {
    if (started_ && !joined_)
      pthread_detach(&tid_);
  }
  
  static void* callBackFunc(void* obj)
  {
    Thread* thread = static_cast<Thread*>(obj);
    thread->func_();
    return nullptr;
  }

  void start()
  {
    assert(!started_);
    started_ = true;
    if (pthread_create(&tid_, NULL, callBackFunc, this))
    {
      started_ = false;

    }
  }
  
  bool isStart() const
  {
    return started_;
  }
  
  int join()
  {
    assert(started_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(tid_, NULL);
  }

  bool isJoin() const
  {
    return joined_;
  }
};

SUNNY_END
#endif // THREAD_H
