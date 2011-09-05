// -*-c++-*-

#ifndef AF_TRIVIAL_ALLOCATOR_H_
#define AF_TRIVIAL_ALLOCATOR_H_

#include <cstdlib>
#include <climits>

namespace af {

template<typename T>
class TrivialAllocator {
  public:
    typedef unsigned char byte;
    typedef T value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef T* pointer;
    typedef const T* const_pointer;

    typedef T& reference;
    typedef const T& const_reference;

    TrivialAllocator(size_t size_bytes, byte* underlying)
        :size_bytes_(size_bytes),
         underlying_(underlying),
         pos_(0) {       
    }
    
    virtual ~TrivialAllocator() {        
    }

    template<typename U>
    TrivialAllocator(const TrivialAllocator<U>& other)
            :underlying_(other.GetUnderlying()),
             size_bytes_(other.GetSizeBytes()),
             pos_(other.GetPos()) {
    }

    template<typename U>
    struct rebind {
        typedef TrivialAllocator<U> other;
    };
   
    byte* GetUnderlying() const {
        return underlying_;
    }
    
    size_t GetSizeBytes() const {
        return size_bytes_;
    }
    
    size_t GetPos() const {
        return pos_;
    }
    
    pointer address(reference r) const {
        return &r;
    }
    
    const_pointer address(const_reference r) const {
        return &r;
    }

    pointer allocate(size_type n, const void* /*hint*/ = NULL) {
        pointer ret = NULL;
        size_t requested = n * sizeof(value_type);
        if ((pos_ + requested) < size_bytes_) {
            ret = reinterpret_cast<pointer>(&underlying_[pos_]);
            pos_ += requested;
        }
        if (ret == NULL) {
            throw std::bad_alloc();
        }
        return ret;                
    }
    
    void deallocate(pointer /*hint*/, size_type /*hint*/) {
        // Nothing to do, we're re-using the space
    }

    void construct(pointer p, const T& val) {
        // Using placement new
        new (p) T(val);
    }
    
    void destroy(pointer p) {
        p->~T();
    }

    size_type max_size() const {
        return ULONG_MAX / sizeof(T);
    }
    
private:
    size_t size_bytes_;
    byte* underlying_;    
    size_t pos_;
};

}

#endif // AF_TRIVIAL_ALLOCATOR_H_
