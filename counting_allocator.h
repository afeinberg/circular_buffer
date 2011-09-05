// -*-c++-*-

#ifndef AF_COUNTING_ALLOCATOR_H_
#define AF_COUNTING_ALLOCATOR_H_

#include "forwarding_allocator.h"

namespace af {

template<typename T, class Alloc>
class CountingAllocator: public ForwardingAllocator<T, Alloc> {
    typedef ForwardingAllocator<T, Alloc> inherit;
  public:    
    
    CountingAllocator(boost::shared_ptr<Alloc> p_alloc)
            :inherit(p_alloc),
             num_allocations_(0),
             num_deallocations_(0),
             num_constructions_(0),
             num_destroys_(0) {
    }

    CountingAllocator(const CountingAllocator& other)
            :inherit(other.GetAlloc()),
             num_allocations_(other.GetNumAllocations()),
             num_deallocations_(other.GetNumDeallocations()),
             num_constructions_(other.GetNumConsturctions()),
             num_destroys_(other.GetNumDestroys()) {
    }

    template<typename U>
    CountingAllocator(const CountingAllocator<U, Alloc>& other)
            :inherit(other.GetAlloc),
             num_allocations_(other.GetNumAllocations()),
             num_deallocations_(other.GetNumDeallocations()),
             num_constructions_(other.GetNumConsturctions()),
             num_destroys_(other.GetNumDestroys()) {
    }

    template<typename U> struct rebind {
        typedef CountingAllocator<U, Alloc> other;
    };

    boost::shared_ptr<Alloc> GetAlloc() const {
        return inherit::GetAlloc();
    }
    
    virtual ~CountingAllocator() {
    }

    size_t GetNumAllocations() const {
        return num_allocations_;        
    }

    size_t GetNumDeallocations() const {
        return num_deallocations_;
    }

    size_t GetNumConstructions() const {
       return num_constructions_;
    }

    size_t GetNumDestroys() const {
        return num_destroys_;
    }
    
    T* allocate(size_t n, const void* p = NULL) {
        ++num_allocations_;
        return GetAlloc()->allocate(n, p);
    }

    void deallocate(T* p, size_t n) {
        ++num_deallocations_;
        GetAlloc()->deallocate(p, n);
    }

    void construct(T* p, const T& t) {
        ++num_constructions_;
        GetAlloc()->construct(p, t);
    }

    void destroy(T* p) {
        ++num_destroys_;
        GetAlloc()->destroy(p);
    }

    
  private:
    size_t num_allocations_;
    size_t num_deallocations_;
    size_t num_constructions_;
    size_t num_destroys_;
};

}
#endif // AF_COUNTING_ALLOCATOR_H_
