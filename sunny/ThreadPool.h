#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <deque>
#include <algorithm>
#include "Condition.h"
#include "Thread.h"
#include <stdio.h>
#include <vector>
SUNNY_BEGIN
using std::string;
class ThreadPool : sunny::Nocopyable
{
private:
  bool running_;
  string name_;
  mutable Mutex mutex_;
  Condition notEmpty_;
  Condition notFull_;
  std::deque<Task> taskQueue_;
  int maxThreadSize_;
  Task callback_;
  std::vector<Thread*> thread_;
public:
  typedef std::function<void()> Task;
  explicit ThreadPool(const string& name = string("ThreadPool"))
    : mutex_(),
      notEmpty_(mutex_),
      notFull_(mutex_),
      name_(name),
      maxThreadSize_(0),
      running_(false)
  { }

  ~ThreadPool()
  {
    if (running_) {
      stop();
    }
    std::for_each(thread_.begin(), thread_.end(),
                  [](Thread* ptr){delete ptr;});
  }

  Task getNextTask()
  {
    MutexLocker lock(mutex_);
    while (taskQueue_.empty() && running_)
      notEmpty_.wait();
    Task task;
    if (!taskQueue_.empty()) {
      task = taskQueue_.front();
      taskQueue_.pop_front();
      notFull_.notify();
    }
    return task;
  }

  void start(int threadCount)
  {
    assert(thread_.empty());
    assert(threadCount>=0);
    running_ = true;
    thread_.reserve(threadCount);
    for (int i=0; i!=threadCount; ++i) {
      char threadId[32];
      snprintf(threadId, sizeof(threadId), "%d", i+1);
      thread_.push_back(new Thread([=](){this->ThreadCallBack();}));
      thread_[i]->start();
    }
  }

  void stop()
  {
    {
      MutexLocker lock(mutex_);
      running_ = false;
      emptyNum_.notifyAll();
    }
    std::for_each(thread_.begin(), thread_.end(),
                  [](Thread* ptr){ptr->join();});
  }

  void ThreadCallBack()
  {
    if (callback_)
      callback_();
    while (running_) {
      Task task(getNextTask());
      if (task)
        task();
    }
  }

  void addTask(const Task& task)
  {
    if (thread_.empty())
      task();
    else {
      MutexLocker lock(mutex_);
      while (isFull())
        notFull_.wait();
      taskQueue_.push_back(task);
      notEmpty_.notify();
    }
  }

  void addTask(Task&& task)
  {
    if (thread_.empty())
      task();
    else {
      MutexLocker lock(mutex_);
      while (isFull())
        notFull_.wait();
      taskQueue_.push_back(std::move(task));
      notEmpty_.notify();
    }
  }

  bool isFull() const
  {
    MutexLocker lock(mutex_);
    return maxThreadSize_ > 0 &&
        taskQueue_.size() >= maxThreadSize_;
  }

  size_t waitTaskCount() const
  {
    MutexLocker lock(mutex_);
    return taskQueue_.size();
  }

  const string& name() const { return name_; }
  int getMaxThreadCount() const
  { return maxThreadSize_; }
  void setMaxThreadCount(int maxThreadCount)
  { maxThreadSize_ = maxThreadCount; }

};

SUNNY_END
#endif // THREADPOOL_H
