#include "test_common.h"
#include "trivial_allocator.h"
#include "gtest/gtest.h"

#include <cstdint>
#include <vector>
#include <cstdlib>

namespace {

using af::TrivialAllocator;
using af::Person;

class TrivialAllocatorTest: public ::testing::Test {
  protected:
    typedef unsigned char byte;
    
    enum { kBytes = 4096 };

    TrivialAllocatorTest()
            :underlying_(static_cast<byte*>(std::calloc(kBytes, sizeof(byte)))),
             integers_(kBytes, underlying_),
             people_(kBytes, underlying_),
             bytes_(kBytes, underlying_) {
    }

    virtual ~TrivialAllocatorTest() {
        std::free(underlying_);
    }
    
    byte* underlying_;
    TrivialAllocator<uint32_t> integers_;
    TrivialAllocator<Person> people_;
    TrivialAllocator<byte> bytes_;
};

// Test that the allocator works for integral types
TEST_F(TrivialAllocatorTest, IntegralTypes) {
    std::vector<uint32_t, TrivialAllocator<uint32_t> > vec(integers_);
    for (uint32_t i = 0; i < 256; ++i) {
        vec.push_back(i);
        EXPECT_EQ(i, vec[i]);
    }
}

// Test that the allocator works with user defined types
TEST_F(TrivialAllocatorTest, UserDefinedTypes) {
    std::vector<Person, TrivialAllocator<Person> > vec(people_);
    Person doe(21, "John Doe");
    vec.push_back(doe);
    EXPECT_EQ(doe, vec[0]);
}

// Test that the allocator throws bad alloc when we overflow
TEST_F(TrivialAllocatorTest, TestThrowBadAlloc) {
    std::vector<byte, TrivialAllocator<byte> > vec(bytes_);
    byte b = 0;
    for (int i = 0; i < kBytes / 2; ++i) {
        vec.push_back(b);
    }
    EXPECT_THROW(vec.push_back(b), std::bad_alloc);
}
    
} // namespace

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
