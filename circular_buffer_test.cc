#include "circular_buffer.h"
#include "trivial_allocator.h"
#include "forwarding_allocator.h"
#include "counting_allocator.h"
#include "test_common.h"
#include "gtest/gtest.h"

namespace {

using af::CircularBuffer;
using af::TrivialAllocator;
using af::ForwardingAllocator;
using af::CountingAllocator;
using af::Person;

typedef std::allocator<Person> StdAlloc;
typedef CountingAllocator<Person, StdAlloc> CountingAlloc;
typedef ForwardingAllocator<Person, CountingAlloc> DelegateAlloc;

std::shared_ptr<DelegateAlloc> CreateDelegateAlloc() {
    std::shared_ptr<StdAlloc> p_alloc(new StdAlloc());
    std::shared_ptr<CountingAlloc> p_counting_alloc(new CountingAlloc(p_alloc));
    return std::shared_ptr<DelegateAlloc>(new DelegateAlloc(p_counting_alloc));
}
    
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
                           TrivialAllocator<Person>(kBuffSize, buffer_)),
             delegate_alloc_(CreateDelegateAlloc()),
             counting_buffer_(kCapacity,
                              false,
                              *(delegate_alloc_)) {
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

    CountingAlloc& GetCountingAlloc() const {
        return *(delegate_alloc_->GetAlloc());
    }
                      
    byte* buffer_;
    CircularBuffer<int> integers_;
    CircularBuffer<int> overflow_;
    CircularBuffer<Person> people_;
    CircularBuffer<Person, TrivialAllocator<Person> > custom_alloc_;
    
    std::shared_ptr<DelegateAlloc> delegate_alloc_;
    CircularBuffer<Person, DelegateAlloc> counting_buffer_;
};

} // namespace

TEST_F(CircularBufferTest, TestNumAllocations) {
    Person doe(21, "John Doe");
    EXPECT_TRUE(counting_buffer_.PushBack(doe));
    EXPECT_EQ(1u, GetCountingAlloc().GetNumAllocations());
    EXPECT_EQ(0u, GetCountingAlloc().GetNumDeallocations());
}

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
