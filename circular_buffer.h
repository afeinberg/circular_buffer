// -*-c++-*-

#ifndef AF_CIRCULAR_BUFFER_H_
#define AF_CIRCULAR_BUFFER_H_

#include "common.h"

#include <cstdlib>
#include <memory>

namespace af {

template<typename T, class Alloc = std::allocator<T> >
class CircularBuffer: private Noncopyable {
  public:
    explicit CircularBuffer(size_t capacity,
                            bool overflow = false,
                            const Alloc& alloc = Alloc());
    virtual ~CircularBuffer();

    size_t GetSize() const;

    size_t GetCapacity() const;

    bool IsEmpty() const;

    bool IsFull() const;
    
    bool PushBack(const T& item);

    bool Front(T& item) const;

    bool PopFront();

    void Empty();
    
  private:
    size_t capacity_;
    bool overflow_;
    Alloc alloc_;
    size_t size_;
    size_t head_;
    size_t tail_;
    T* buffer_;
};

template<typename T, class Alloc>
CircularBuffer<T, Alloc>::CircularBuffer(size_t capacity,
                                         bool overflow,
                                         const Alloc& alloc)
        :capacity_(capacity),
         overflow_(overflow),
         alloc_(alloc),
         size_(0),
         head_(0),
         tail_(0) {
    buffer_ = alloc_.allocate(capacity_);
}

template<typename T, class Alloc>
CircularBuffer<T, Alloc>::~CircularBuffer() {
    Empty();
    alloc_.deallocate(buffer_, capacity_);
}

template<typename T, class Alloc>
size_t CircularBuffer<T, Alloc>::GetSize() const {
    return size_;
}

template<typename T, class Alloc>
size_t CircularBuffer<T, Alloc>::GetCapacity() const {
    return capacity_;
}

template<typename T, class Alloc>
bool CircularBuffer<T, Alloc>::IsEmpty() const {
    return size_ == 0;
}

template<typename T, class Alloc>
bool CircularBuffer<T, Alloc>::IsFull() const {
    return size_ >= capacity_;
}

template<typename T, class Alloc>
bool CircularBuffer<T, Alloc>::PushBack(const T& item) {
    bool ret = false;
    if (overflow_ || !IsFull()) {
        alloc_.construct(&buffer_[tail_], item);
        ++tail_;
        tail_ %= capacity_;
        ++size_;
        ret = true;
    }
    return ret;
}

template<typename T, class Alloc>
bool CircularBuffer<T, Alloc>::Front(T& item) const {
    bool ret = false;
    if (!IsEmpty()) {
        item = buffer_[head_];       
        ret = true;
    }
    return ret;
}

template<typename T, class Alloc>
bool CircularBuffer<T, Alloc>::PopFront() {
    bool ret = false;
    if (!IsEmpty()) {
        alloc_.destroy(&buffer_[head_]);
        ++head_;
        head_ %= capacity_;
        --size_;
        ret = true;
    }
    return ret;
}

template<typename T, class Alloc>
void CircularBuffer<T, Alloc>::Empty() {
    while (!IsEmpty()) {
        PopFront();
    }
}

}

#endif // AF_CIRCULAR_BUFFER_H_
