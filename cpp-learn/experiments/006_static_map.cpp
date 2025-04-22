#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <stdexcept>

// https://github.com/ChoppinBlockParty/ctm

namespace msd {

template <typename Key, typename Value, typename ValuesIterator, typename Values, typename Keys, typename Set>
struct static_map_iterator {
    ValuesIterator it_;
    Values& values_;
    Keys& keys_;
    Set& set_;

    void operator++() noexcept
    {
        do {
            ++it_;

            const auto idx = static_cast<std::size_t>(std::distance(values_.begin(), it_));
            auto stop = idx >= set_.size() || set_[idx];

            if (stop) {
                break;
            }
        } while (true);
    }

    std::pair<Key&, Value&> operator*()
    {
        const auto idx = std::distance(values_.begin(), it_);
        auto& key = keys_[idx];

        auto& element = *it_;

        return {key, element};
    }

    bool operator==(const static_map_iterator&) const noexcept { return it_ == values_.end(); }

    bool operator!=(const static_map_iterator&) const noexcept { return !(operator==(*this)); }
};

struct static_map_default_indexer {
    template <typename Key>
    std::size_t operator()(const Key& key) const noexcept
    {
        return static_cast<std::size_t>(key);
    }
};

template <typename Key, typename Value, std::size_t Size, typename Indexer = static_map_default_indexer>
class static_map {
    static_assert(Size > 0, "Size must be at least one");

    using keys_type = std::array<Key, Size>;
    using values_type = std::array<Value, Size>;
    using set_type = std::array<bool, Size>;

   public:
    using iterator = static_map_iterator<Key, Value, typename values_type::iterator, values_type, keys_type, set_type>;

    static_map() = default;
    static_map(static_map&) = default;
    static_map(static_map&&) noexcept = default;

    static_map& operator=(const static_map&) = default;
    static_map& operator=(static_map&&) noexcept = default;

    static_map(std::initializer_list<std::pair<Key, Value>> list)
    {
        std::for_each(list.begin(), list.end(), [this](const std::pair<Key, Value>& element) {
            this->operator[](element.first) = element.second;
        });
    }

    Value& at(const Key& key)
    {
        const auto idx = index_(key);

        try {
            if (!set_.at(idx)) {
                throw std::out_of_range{"out of range at map.at()"};
            }

            return values_.at(idx);
        }
        catch (const std::out_of_range&) {
            throw std::out_of_range{"out of range at map.at()"};
        }
    }

    Value& operator[](const Key& key)
    {
        const auto idx = index_(key);

        keys_[idx] = key;
        set_[idx] = true;

        return values_[idx];
    }

    bool empty() const noexcept
    {
        return std::none_of(set_.cbegin(), set_.cend(), [](bool set) noexcept { return set; });
    }

    bool contains(const Key& key) const noexcept
    {
        const auto idx = index_(key);
        return set_[idx];
    }

    std::size_t size() const noexcept { return std::count(set_.cbegin(), set_.cend(), true); }

    constexpr std::size_t max_size() const noexcept { return Size; }

    iterator begin() noexcept { return iterator{values_.begin(), values_, keys_, set_}; }

    iterator end() noexcept { return iterator{values_.end(), values_, keys_, set_}; }

    void clear() noexcept { set_.fill(false); }

    iterator find(const Key& key)
    {
        const auto k = std::find(keys_.cbegin(), keys_.cend(), key);
        const auto idx = std::distance(keys_.cbegin(), k);
        return iterator{std::next(values_.begin(), idx), values_, keys_, set_};
    }

   private:
    values_type values_{};
    keys_type keys_{};
    set_type set_{};
    Indexer index_;
};

}  // namespace msd

enum class PointType {
    a,
    b,
    c,
    d,
    e,
};

struct Point {
    int x;
    int y;
};

int main()
{
    msd::static_map<PointType, Point, 5> init = {
        {PointType::a, {1, 0}},
    };

    msd::static_map<PointType, Point, 5> map;

    map = std::move(init);

    map[PointType::b] = {2, 3};
    map[PointType::b] = {5, 6};
    map[PointType::c] = {7, 8};
    map[PointType::e] = {99, 98};

    assert(map.at(PointType::b).x == 5);
    try {
        map.at(PointType::d);
        throw std::runtime_error{"should have thrown"};
    }
    catch (const std::out_of_range&) {
    }

    assert(map[PointType::b].x == 5);

    for (auto item : map) {
        std::cout << static_cast<int>(item.first) << ": ";
        std::cout << "(" << item.second.x << ", " << item.second.y << ")\n";
    }

    assert(!map.empty());

    auto item = map.find(PointType::b);
    assert(item != map.end());
    assert((*item).first == PointType::b);
    assert((*item).second.x == 5);
    assert((*item).second.y == 6);

    auto missing_item = map.find(PointType::d);
    assert(missing_item == map.end());

    static_assert(map.max_size() == 5, "max size failed");
    assert(map.size() == 4);

    assert(map.contains(PointType::b));
    assert(!map.contains(PointType::d));

    const auto x_sum = std::accumulate(
        map.begin(), map.end(), 0,
        [](int accumulator, const std::pair<PointType, Point>& item) noexcept { return accumulator + item.second.x; });
    assert(x_sum == 112);

    std::fill(map.begin(), map.end(), std::make_pair(PointType::b, Point{1, 2}));

    for (auto item2 : map) {
        std::cout << static_cast<int>(item2.first) << ": ";
        std::cout << "(" << item2.second.x << ", " << item2.second.y << ")\n";
    }

    map.clear();
    assert(map.empty());
    assert(map.size() == 0);
    assert(!map.contains(PointType::b));
}
