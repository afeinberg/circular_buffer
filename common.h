// -*-c++-*-

#ifndef AF_COMMON_H_
#define AF_COMMON_H_

namespace af {

// based on boost::noncopyable

class Noncopyable {
  protected:
    Noncopyable() {
    }
    
    ~Noncopyable() {
    }
    
  private:
    Noncopyable(const Noncopyable&);
    const Noncopyable& operator=(const Noncopyable&);
};
    
}

#endif // AF_COMMON_H_
