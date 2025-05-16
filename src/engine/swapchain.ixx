module;

#include <vector>

export module swapchain;

// External Dependencies
import vulkan_hpp;

// Internal Dependencies
import gpu;
import vulkan_utils;

namespace eng::swap {
    export struct SwapchainComponents
    {
        vk::Format color_format;
        vk::SwapchainKHR swapchain;
        std::vector<vk::Image> images;
        std::vector<vk::ImageView> image_views;
    };

    /* Swapchain Creation Methods */

    /**
     * Creates a new swapchain meeting minimum specifications
     * @param gpu the tagret GPU
     * @param device the logical device which provides the presentation queue
     * @param surface the surface to which images will be presented
     * @param window_extent the extent of the window corresponding to the surface
     * @param usage the usage flags for this swapchain
     * @param old_swapchain optional parameter, a reference to an existing swapchain being recreated
     * @return a SwapchainComponents struct containing handles to the swapchain, images and image views,
     *         as well as relevant data for recreating the swapchain if necessary
     */
    export [[nodiscard]] SwapchainComponents
    createSwapchain(const GPU& gpu,
                    const vk::Device& device,
                    const vk::SurfaceKHR& surface,
                    const vk::Extent2D& window_extent,
                    const vk::ImageUsageFlags usage,
                    const vk::SwapchainKHR& old_swapchain = {});

    /* Creation Helper Methods */

    /**
     * Selects a swapchain image count within the bounds of the surface capabilities
     * @param capabilities the surface capabilities for the target GPU
     * @return the number of images to create in the swapchain
     */
    [[nodiscard]] uint32_t
    chooseImageCount(const vk::SurfaceCapabilitiesKHR& capabilities);

    /**
     * Selects a targeted color format and color space from the list of supported surface formats,
     * or falls back to the first supported format if no target surface format is supported
     * @param supported_formats the list of surface formats supported by the target GPU
     * @return the selected surface format for the swapchain
     */
    [[nodiscard]] vk::SurfaceFormatKHR
    selectSurfaceFormat(std::span<const vk::SurfaceFormatKHR> supported_formats) ;

    /**
     * Selects a targeted presentation mode from the list of supported modes, or falls back to
     * the default if the targeted mode(s) is not supported
     * @param supported_present_modes the list of presentation modes supported by the target GPU
     * @return the selected present mode for the swapchain
     */
    [[nodiscard]] vk::PresentModeKHR
    selectPresentMode(std::span<const vk::PresentModeKHR> supported_present_modes);

    /**
     * Calculates the extent for images in the swapchain, handling the case where screen coordinates
     * might not correspond to actual pixel size
     * @param capabilities the surface capabilities for the target GPU
     * @param window_size the extent of the window, in pixels, as reported by the window manager
     * @return the extent for the swapchain images
     */
    [[nodiscard]] vk::Extent2D
    getImageExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const vk::Extent2D& window_size);

    /**
     * Creates views for each image in the swapchain to be used as render targets
     * @param images a handle to the underlying swapchain image array
     * @param color_format the color format for the swapchain images
     * @param device the logical device managing the swapchain images
     * @return a vector of ImageViews corresponding to the underlying swapchain images
     */
    [[nodiscard]] std::vector<vk::ImageView>
    createImageViews(std::span<const vk::Image> images, const vk::Format& color_format, const vk::Device& device);
}
