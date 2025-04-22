/*
 https://www.modernescpp.com/index.php/race-condition-versus-data-race
 */

#include <atomic>
#include <thread>
#include <cassert>

struct Account {
    std::atomic<int> balance{};
};

void transferMoney(int amount, Account &from, Account &to) {
    using namespace std::chrono_literals;

    if (from.balance >= amount) {
        from.balance -= amount;

        std::this_thread::sleep_for(1ns);

        to.balance += amount;
    }
}

int main() {
    /**
     * Race condition: A race condition is a situation, in which the result of an operation
     * depends on the interleaving of certain individual operations.
     *
     * Data race: A data race is a situation, in which at least two threads access a
     * shared variable at the same time. At least on thread tries to modify the variable.
     */

    Account account1{100};
    Account account2{100};

    std::thread th1{transferMoney, 50, std::ref(account1), std::ref(account2)};
    std::thread th2{transferMoney, 130, std::ref(account2), std::ref(account1)};

    th1.join();
    th2.join();

    assert(account1.balance == 50);
    assert(account2.balance == 150);
}
