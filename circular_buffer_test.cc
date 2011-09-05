#include "circular_buffer.h"
#include "trivial_allocator.h"
#include "test_common.h"
#include "gtest/gtest.h"

namespace {

using af::CircularBuffer;
using af::TrivialAllocator;
using af::Person;

class CircularBufferTest: public ::testing::Test {
  protected:
    typedef unsigned char byte;
    
    enum { kCapacity = 16, kBuffSize = 4096 };

    CircularBufferTest()
            :buffer_(static_cast<byte*>(std::calloc(kBuffSize, sizeof(byte)))),
             integers_(kCapacity),
             overflow_(kCapacity, true),
             people_(kCapacity),
             custom_alloc_(kCapacity,
                           false,
                           TrivialAllocator<Person>(kBuffSize, buffer_)) {
    }

    virtual void SetUp() {
        Person doe(21, "John Doe");
        people_.PushBack(doe);
    }

    // virtual void TearDown() {
    //   
    // }
    
    virtual ~CircularBufferTest() {
        std::free(buffer_);
    }

    byte* buffer_;
    CircularBuffer<int> integers_;
    CircularBuffer<int> overflow_;
    CircularBuffer<Person> people_;
    CircularBuffer<Person, TrivialAllocator<Person> > custom_alloc_;
};

} // namespace

TEST_F(CircularBufferTest, GetCapacity) {
    EXPECT_EQ(kCapacity, integers_.GetCapacity());
    EXPECT_EQ(kCapacity, people_.GetCapacity());
}

TEST_F(CircularBufferTest, GetSize) {
    EXPECT_EQ(0u, integers_.GetSize());
    EXPECT_EQ(1u, people_.GetSize());
}

TEST_F(CircularBufferTest, IsEmpty) {
    EXPECT_TRUE(integers_.IsEmpty());
    EXPECT_FALSE(people_.IsEmpty());
}

TEST_F(CircularBufferTest, TestFront) {
    Person expected(21, "John Doe");
    Person actual;

    EXPECT_TRUE(people_.Front(actual));
    EXPECT_EQ(expected, actual);
}

TEST_F(CircularBufferTest, TestPushPop) {
    int actual;

    // Test push back
    EXPECT_TRUE(integers_.PushBack(0));
    EXPECT_TRUE(integers_.PushBack(1));
    EXPECT_TRUE(integers_.Front(actual));
    EXPECT_EQ(0, actual);

    // Test pop front
    EXPECT_TRUE(integers_.PopFront());
    EXPECT_TRUE(integers_.Front(actual));
    EXPECT_EQ(1, actual);
    EXPECT_TRUE(integers_.PopFront());
    EXPECT_FALSE(integers_.PopFront());
    EXPECT_TRUE(integers_.IsEmpty());
}

TEST_F(CircularBufferTest, TestCustomAlloc) {
    Person expected(21, "John Doe");
    Person actual;
    
    EXPECT_TRUE(custom_alloc_.PushBack(expected));
    EXPECT_TRUE(custom_alloc_.Front(actual));
    EXPECT_EQ(expected, actual);
    EXPECT_TRUE(custom_alloc_.PopFront());
}

TEST_F(CircularBufferTest, TestIsFull) {
    unsigned int i;
    for (i = 0; i < kCapacity; ++i) {
        EXPECT_FALSE(integers_.IsFull());
        EXPECT_TRUE(integers_.PushBack(i));
    }
    EXPECT_TRUE(integers_.IsFull());
    EXPECT_FALSE(integers_.PushBack(i));
}

TEST_F(CircularBufferTest, TestOverflow) {
    int i;
    for (i = 0; i < kCapacity; ++i) {
        EXPECT_FALSE(overflow_.IsFull());
        EXPECT_TRUE(overflow_.PushBack(i));
    }
    EXPECT_TRUE(overflow_.IsFull());
    EXPECT_TRUE(overflow_.PushBack(i));
    EXPECT_TRUE(overflow_.PushBack(i));
    EXPECT_EQ(kCapacity + 2u, overflow_.GetSize());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
