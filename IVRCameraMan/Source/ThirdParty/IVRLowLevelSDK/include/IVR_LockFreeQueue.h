#ifndef IVR_LOCKFREEQUEUE_H
#define IVR_LOCKFREEQUEUE_H

#include "IVRLowLevelSDK_global.h"

template <class T, size_t N>
class IVRLOWLEVELSDK_EXPORT LockFreeQueue {
public:
  LockFreeQueue() : read_pos_{0}, write_pos_{0}, size_{0} {
    assert(size_.is_lock_free());
  }

  T emptyT;

  auto size() const { return size_.load(); }

  // Writer thread
  auto push(const T& t)
  {
    if (size_.load() >= N)
    {
      throw std::overflow_error("Queue is full");
    }
    buffer_[write_pos_] = t;
    write_pos_ = (write_pos_ + 1) % N;
    size_.fetch_add(1);
  }

  // Reader thread
  auto& front() const
  {
    auto s = size_.load();
    if (s == 0)
    {
      //throw std::underflow_error("Queue is empty");
      return emptyT;
    }
    return buffer_[read_pos_];
  }

  // Reader thread
  void clean()
  {
      buffer_[read_pos_] = emptyT;
  }

  // Reader thread
  auto pop() {
    if (size_.load() == 0) {
      throw std::underflow_error("Queue is empty");
    }
    read_pos_ = (read_pos_ + 1) % N;
    size_.fetch_sub(1);
  }
private:
  std::array<T, N> buffer_{};  // Used by both threads
  std::atomic<size_t> size_{}; // Used by both threads
  size_t read_pos_ = 0;        // Used by reader thread
  size_t write_pos_ = 0;       // Used by writer thread
};

#endif // IVR_LOCKFREEQUEUE_H
