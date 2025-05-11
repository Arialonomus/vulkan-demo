module;

#include <concepts>
#include <ranges>

export module concepts;

export namespace concepts {

    /**
     * Type is a container of elements convertible to T
     */
    template <typename Container, typename T>
    concept CompatibleRange =
        std::ranges::range<Container> &&
        std::convertible_to<std::ranges::range_value_t<Container>, T>;
}
