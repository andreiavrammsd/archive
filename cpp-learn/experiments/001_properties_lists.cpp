#ifndef MSD_PARAMETERS_LISTS_HPP_
#define MSD_PARAMETERS_LISTS_HPP_

#include <array>
#include <iostream>
#include <tuple>

namespace msd {

/**
 * Static lists
 *
 * @tparam ListSize: Number of parameters in each list
 * @tparam Indexer: Functor to convert a custom index_ type to size_t
 * @tparam Ts: Types of parameters for each list
 */
template <std::size_t ListSize, typename Indexer, typename... Ts>
class PropertiesLists {
    static_assert(sizeof...(Ts) > 0, "No parameters types provided");

   public:
    /**
     * @brief Get parameters for index_ (immutable overload)
     *
     * @tparam T: PointType of parameters to retrieve
     * @tparam Index: PointType of index_ for retrieving parameters from list
     *
     * @param index: Index for retrieving parameters from list
     *
     * @throws: any exception thrown by Indexer
     *
     * @return: Constant reference to parameters
     */

    template <typename T, typename Index>
    const auto &GetParameters(const Index index) const
    {
        auto &params = std::get<list<T>>(parameters);
        const auto idx = indexer(index);

        return params[idx];
    }

    /**
     * @brief Get parameters for index_ (mutable overload)
     *
     * @tparam T: PointType of parameters to retrieve
     * @tparam Index: PointType of index_ for retrieving parameters from list
     *
     * @param index: Index for retrieving parameters from list
     *
     * @throws: Any exception thrown by Indexer
     *
     * @return: Reference to parameters
     */
    template <typename T, typename Index>
    auto &GetParameters(const Index index)
    {
        auto &params = std::get<list<T>>(parameters);
        const auto idx = indexer(index);

        return params[idx];
    }

   private:
    // Template alias for declaring an array of parameters
    template <typename T>
    using list = std::array<T, ListSize>;

    // Lists of parameters
    std::tuple<list<Ts>...> parameters;

    // Indexer functor
    Indexer indexer;
};

}  // namespace msd

#endif  // MSD_PARAMETERS_LISTS_HPP_

enum class Color {
    Red,
    Green,
    Blue,
};

enum class Size {
    Small,
    Medium,
    Large,
};

enum class Protocol {
    HTTP,
    TCP,
    UDP,
};

int main()
{
    struct indexer {
        auto operator()(Protocol item) { return static_cast<std::size_t>(item); }
    };
    msd::PropertiesLists<10, indexer, Color, Size> lists;
    lists.GetParameters<Color>(Protocol::HTTP) = Color::Green;
    std::cout << (lists.GetParameters<Color>(Protocol::HTTP) == Color::Green) << "\n";

    // Designated initializers (C++20)
    struct A {
        int x{};
        int y{};
        int z{};
    };
}
