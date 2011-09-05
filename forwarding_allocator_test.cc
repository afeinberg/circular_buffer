#include "test_common.h"
#include "forwarding_allocator.h"
#include "gtest/gtest.h"

#include <vector>
#include <cstdlib>
#include <memory>

namespace {

using af::ForwardingAllocator;
using af::Person;

typedef std::allocator<Person> StdAlloc;
typedef ForwardingAllocator<Person, StdAlloc> ForwardingAlloc;

class ForwardingAllocatorTest: public ::testing::Test {
  protected:

    ForwardingAllocatorTest()
            :alloc_(std::shared_ptr<StdAlloc>(new StdAlloc)) {
    }
    
    ForwardingAlloc alloc_;    
};

TEST_F(ForwardingAllocatorTest, VectorTest) {
    std::vector<Person, ForwardingAlloc> vec(alloc_);
    Person doe(21, "John Doe");
    vec.push_back(doe);
    EXPECT_EQ(doe, vec[0]);
}

} // namespace

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
