#include "MyQueue.h"

#include "gtest/gtest.h"

class MyQueueTest : public ::testing::Test {

};

TEST_F(MyQueueTest, test) {
    MyQueue<std::string> queue;

    queue.write("message");

    EXPECT_EQ(1, queue.size());
    EXPECT_EQ("message", queue.read());
    EXPECT_EQ(0, queue.size());
}
