module;

#include <vector>

export module list_utils;

import concepts;

namespace util {
    /**
     * Returns the size of a standard library-compatible container
     * @tparam Container any standard library container that implements .size() or forward iterators
     * @param container the container to size
     * @return the size, bytes, of the container
     */
    template <typename Container>
    constexpr auto getContainerSize(const Container& container) -> size_t {
        if constexpr (requires { container.size(); }) {
            return container.size();
        } else {
            return std::distance(std::begin(container), std::end(container));
        }
    }

    /**
     * Flattens one or more containers of a single type to a std::vector of that type
     * @tparam T the type of the data stored in each container
     * @tparam Containers one or more standard library-compatible types holding objects of type T
     * @param containers the containers to flatten
     * @return a std::vector containing all elements in the passed-in containers
     */
    export template <typename T, concepts::CompatibleRange<T>... Containers>
    auto flattenToVector(const Containers&... containers) -> std::vector<T> {
        // Estimate total size and reserve
        size_t total_size = (getContainerSize(containers) + ...);
        std::vector<T> result;
        result.reserve(total_size);

        // Insert contents
        ((result.insert(result.end(), std::begin(containers), std::end(containers))), ...);

        return result;
    }
}
