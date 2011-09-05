// -*-c++-*-

#ifndef AF_TEST_COMMON_H_
#define AF_TEST_COMMON_H_

#include <string>

namespace af {

struct Person {
    Person() {
    }
    
    Person(unsigned int age, const char *name)
            :age_(age),
             name_(name) {
    }

    bool operator==(const Person& other) const {
        return age_ == other.age_ && name_ == other.name_;
    }

    unsigned int age_;
    std::string name_;
};

}

#endif // AF_TEST_COMMON_H_
