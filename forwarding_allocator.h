// -*-c++-*-

#ifndef AF_FORWARDING_ALLOCATOR_H_
#define AF_FORWARDING_ALLOCATOR_H_

#include <cstdlib>
#include <climits>
#include <boost/shared_ptr.hpp>

namespace af {

template<typename T, class Alloc>
class ForwardingAllocator {
  public:
    typedef T value_type;   
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef T* pointer;
    typedef const T* const_pointer;

    typedef T& reference;
    typedef const T& const_reference;

    ForwardingAllocator(boost::shared_ptr<Alloc> p_alloc)
            :p_alloc_(p_alloc) {
    }

    boost::shared_ptr<Alloc> GetAlloc() const {
        return p_alloc_;
    }
    
    virtual ~ForwardingAllocator() {        
    }

    ForwardingAllocator(const ForwardingAllocator& other)
            :p_alloc_(other.GetAlloc()) {
    }

    template<typename U> 
    ForwardingAllocator(const ForwardingAllocator<U, Alloc>& other)
            :p_alloc_(other.GetAlloc()) {
    }

    template<typename U> struct rebind {
        typedef ForwardingAllocator<U, Alloc> other;
    };
    
    pointer address(reference r) const {
        return p_alloc_->address(r);
    }

    const_pointer address(const_reference r) const {
        return p_alloc_->address(r);
    }

    pointer allocate(size_type n, const void* p = NULL) {
        return p_alloc_->allocate(n, p);
    }

    void deallocate(pointer p, size_type n) {
        p_alloc_->deallocate(p, n);
    }

    void construct(pointer p, const T& t) {
        p_alloc_->construct(p, t);
    }

    void destroy(pointer p) {
        p_alloc_->destroy(p);
    }

    size_type max_size() const {
        return p_alloc_->max_size();
    }
    
  private:
    boost::shared_ptr<Alloc> p_alloc_;
};

}

#endif // AF_FORWARDING_ALLOCATOR_H_
