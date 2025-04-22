#include <cstdlib>
#include <string>

#include "MyService.h"
#include "Queue.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class MyServiceTest : public ::testing::Test {
};

template <typename T>
class QueueMock : public Queue<T> {
   public:
    MOCK_METHOD(void, write, (const T &msg));
    MOCK_METHOD(size_t, size, ());
    MOCK_METHOD(T, read, ());
};

using ::testing::Return;

TEST_F(MyServiceTest, run_with_string)
{
    QueueMock<std::string> queue;
    MyService<std::string> service(queue);

    EXPECT_CALL(queue, size()).Times(3).WillOnce(Return(2)).WillOnce(Return(1)).WillOnce(Return(0));

    EXPECT_CALL(queue, read()).Times(2).WillOnce(Return("a")).WillOnce(Return("b"));

    EXPECT_EQ("ab", service.run());
}

TEST_F(MyServiceTest, all_even)
{
    std::vector<int> numbers = {2, 4, 6};
    EXPECT_TRUE(all_even(numbers));

    std::vector<int> numbers2 = {2, 3, 6};
    EXPECT_FALSE(all_even(numbers2));
}
TEST_F(MyServiceTest, insert)
{
    QueueMock<std::string> queue;
    MyService<std::string> service(queue);

    std::string data = "value";

    EXPECT_CALL(queue, write(data)).Times(1);

    service.insert(data);
}

TEST_F(MyServiceTest, run_with_int)
{
    QueueMock<int> queue;
    MyService<int> service(queue);

    EXPECT_CALL(queue, size()).Times(3).WillOnce(Return(2)).WillOnce(Return(1)).WillOnce(Return(0));

    EXPECT_CALL(queue, read()).Times(2).WillOnce(Return(1)).WillOnce(Return(2));

    EXPECT_EQ(3, service.run());
}
