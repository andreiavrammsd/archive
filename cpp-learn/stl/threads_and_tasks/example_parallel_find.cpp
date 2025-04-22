/*
 The C++ Programming Language, 4th Edition
 */

#include <vector>
#include <algorithm>
#include <cassert>
#include <future>
#include <random>
#include <functional>
#include <iostream>

const int grain = 50000;

enum class Color {
    green,
    red,
    blue,
};

struct Record {
    int id;
    double price;
    Color color;
};

std::ostream &operator<<(std::ostream &os, const Record &rec) {
    return os << "{" << rec.id << ": " << rec.price << ", " << static_cast<int>(rec.color) << "}";
}

template<typename Pred>
Record *find_rec(std::vector<Record> &vr, int first, int last, Pred pr) {
    std::vector<Record>::iterator p = std::find_if(vr.begin() + first, vr.begin() + last, pr);

    if (p == vr.begin() + last) {
        return nullptr; // at end: no record found
    }

    return &*p; // found: return a pointer to the element
}

template<typename Pred>
Record *pfind(std::vector<Record> &vr, Pred pr) {
    assert(vr.size() % grain == 0);

    std::vector<std::future<Record *>> res;

    for (size_t i = 0; i != vr.size(); i += grain) {
        res.push_back(std::async(find_rec<Pred>, std::ref(vr), i, i + grain, pr));
    }

    for (size_t i = 0; i != res.size(); ++i) { // look for a result in the futures
        if (auto p = res[i].get()) { // did the task find a match?
            return p;
        }
    }

    return nullptr; // no match found
}

template<typename T>
int wait_for_any(std::vector<std::future<T>> &vf, std::chrono::steady_clock::duration d);

template<typename Pred>
Record *pfind_any(std::vector<Record> &vr, Pred pr) {
    std::vector<std::future<Record *>> res;

    for (size_t i = 0; i != vr.size(); i += grain) {
        res.push_back(std::async(find_rec<Pred>, std::ref(vr), i, i + grain, pr));
    }

    for (int count = res.size(); count; --count) {
        int i = wait_for_any(res, std::chrono::microseconds{10}); // find a completed task

        if (auto p = res[i].get()) { // did the task find a match?
            return p;
        }
    }

    return nullptr; // no match found
}

template<typename Pred>
std::vector<Record *> find_all_rec(std::vector<Record> &vr, int first, int last, Pred pr) {
    std::vector<Record *> res;

    for (int i = first; i != last; ++i) {
        if (pr(vr[i])) {
            res.push_back(&vr[i]);
        }
    }

    return res;
}

template<typename T>
std::vector<T> wait_for_all(std::vector<std::future<T>> &vf);

template<typename Pred>
std::vector<Record *> pfind_all(std::vector<Record> &vr, Pred pr) {
    std::vector<std::future<std::vector<Record *>>> res;

    for (size_t i = 0; i != vr.size(); i += grain) {
        res.push_back(std::async(find_all_rec<Pred>, std::ref(vr), i, i + grain, pr));
    }

    std::vector<std::vector<Record *>> r2 = wait_for_all(res);

    std::vector<Record *> r;
    for (auto &x : r2) { // merge results
        for (auto p : x) {
            r.push_back(p);
        }
    }

    return r;
}

struct price_color_filter {
    double price;
    Color color;
    std::function<bool(double, double)> price_cmp = std::less_equal<>{};

    bool operator()(const Record &rec) {
        return price_cmp(rec.price, price) && rec.color == color;
    }
};

Record *find_cheap_red(std::vector<Record> &goods) {
    assert(goods.size() % grain == 0);

    Record *p = pfind(goods, price_color_filter{200, Color::red});

    return p;
}

Record *find_any_cheap_red(std::vector<Record> &goods) {
    assert(goods.size() % grain == 0);

    return pfind_any(goods, price_color_filter{200, Color::red});
}

std::vector<Record *> find_all_cheap_red(std::vector<Record> &goods) {
    assert(goods.size() % grain == 0);

    return pfind_all(goods, price_color_filter{200, Color::red});
}

std::vector<Record> generate_goods(int num);

int main() {
    std::vector<Record> goods; // data to be searched
    goods = generate_goods(250000);

    auto rec = find_cheap_red(goods);
    if (rec) {
        std::cout << *rec << std::endl;
    } else {
        std::cout << "not found" << std::endl;
    }

    rec = find_any_cheap_red(goods);
    if (rec) {
        std::cout << *rec << std::endl;
    } else {
        std::cout << "not found" << std::endl;
    }

    auto recs = find_all_cheap_red(goods);
    std::cout << "found#: " << recs.size() << std::endl;
}

std::vector<Record> generate_goods(int num) {
    std::vector<Record> goods;

    auto sec = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    auto engine = std::default_random_engine{};
    engine.seed(sec);

    auto price = std::bind(
            std::normal_distribution<>{100, 500},
            engine
    );

    auto color = std::bind(
            std::uniform_int_distribution<>{0, 2},
            engine
    );

    for (int i = 0; i != num; ++i) {
        auto good = Record{i + 1, std::abs(price()), Color(color())};
        goods.push_back(good);
    }

    return goods;
}

template<typename T>
int wait_for_any(std::vector<std::future<T>> &vf, std::chrono::steady_clock::duration d) {
    while (true) {
        for (size_t i = 0; i != vf.size(); ++i) {
            if (!vf[i].valid()) {
                continue;
            }

            switch (vf[i].wait_for(std::chrono::seconds{0})) {
                case std::future_status::ready:
                    return i;
                case std::future_status::timeout:
                    break;
                case std::future_status::deferred:
                    throw std::runtime_error{"wait_for_any(): deferred future"};
            }
        }

        std::this_thread::sleep_for(d);
    }
}

template<typename T>
std::vector<T> wait_for_all(std::vector<std::future<T>> &vf) {
    std::vector<T> res;
    res.reserve(vf.size());
    for (auto &fu : vf) {
        res.push_back(fu.get());
    }
    return res;
}
