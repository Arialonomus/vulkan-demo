module;

#include <algorithm>
#include <iterator>
#include <limits>

module swapchain;

namespace eng::swap {
    SwapchainComponents createSwapchain(const GPU& gpu,
                                        const vk::Device& device,
                                        const vk::SurfaceKHR& surface,
                                        const vk::Extent2D& window_extent,
                                        const vk::ImageUsageFlags usage,
                                        const vk::SwapchainKHR& old_swapchain)
    {
        const auto surface_capabilities{ gpu.getDevice().getSurfaceCapabilitiesKHR(surface) };
        const auto [ color_format, color_space ] = selectSurfaceFormat(gpu.getDevice().getSurfaceFormatsKHR(surface));
        const auto image_extent{ getImageExtent(surface_capabilities, window_extent) };
        vk::SwapchainCreateInfoKHR create_info{ };
        create_info.setSurface( surface )
            .setMinImageCount( chooseImageCount(surface_capabilities) )
            .setImageFormat( color_format )
            .setImageColorSpace( color_space )
            .setImageExtent( image_extent )
            .setImageArrayLayers( 1 )
            .setImageUsage( usage )
            .setPreTransform( surface_capabilities.currentTransform )
            .setCompositeAlpha( vk::CompositeAlphaFlagBitsKHR::eOpaque )
            .setPresentMode( selectPresentMode(gpu.getDevice().getSurfacePresentModesKHR(surface)) )
            .setClipped( true )
            .setOldSwapchain( old_swapchain );
        if (gpu.getGraphicsFamilyIndex() == gpu.getPresentFamilyIndex()) {
            create_info.setImageSharingMode( vk::SharingMode::eExclusive );
        } else {
            create_info.setImageSharingMode( vk::SharingMode::eConcurrent );
            create_info.setQueueFamilyIndexCount( 2 );
            create_info.setPQueueFamilyIndices(
                std::array{ gpu.getGraphicsFamilyIndex(), gpu.getPresentFamilyIndex() }.data()
            );
        }

        const vk::SwapchainKHR swapchain{ device.createSwapchainKHR(create_info) };
        const std::vector images{ device.getSwapchainImagesKHR(swapchain) };
        return {
            color_format,
            image_extent,
            swapchain,
            images,
            createImageViews(images, color_format, device)
        };
    }

    uint32_t chooseImageCount(const vk::SurfaceCapabilitiesKHR& capabilities)
    {
        const auto min_images{ capabilities.minImageCount };
        const auto max_images{ capabilities.maxImageCount };
        return min_images + 1 < max_images ? min_images + 1 : max_images;
    }

    vk::SurfaceFormatKHR selectSurfaceFormat(const std::span<const vk::SurfaceFormatKHR> supported_formats)
    {
        constexpr std::array target_formats{
            vk::Format::eB8G8R8A8Srgb,
            vk::Format::eR8G8B8A8Srgb
        };
        constexpr auto target_color_space{ vk::ColorSpaceKHR::eSrgbNonlinear };

        // Locate the first matching target format among the supported formats
        for (const auto& target : target_formats) {
            if (const auto iter = std::ranges::find_if(supported_formats, [target](const auto& supported) {
                return supported.format == target && supported.colorSpace == target_color_space;
            }); iter != supported_formats.end())
                return *iter;
        }

        // Fallback to the first supported format if no matches found
        return supported_formats.front();
    }

    vk::PresentModeKHR selectPresentMode(const std::span<const vk::PresentModeKHR> supported_present_modes)
    {
        constexpr auto target_present_mode{
            vk::PresentModeKHR::eMailbox
        };

        for (const auto& present_mode : supported_present_modes) {
            if (present_mode == target_present_mode)
                return present_mode;
        }

        // Fallback to FIFO mode, which is guaranteed to be supported
        return vk::PresentModeKHR::eFifo;
    }

    vk::Extent2D getImageExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const vk::Extent2D& window_size)
    {
        // Check if the surface extent potentially differs from the actual window size
        if (constexpr uint32_t uint32_max{ std::numeric_limits<uint32_t>::max() };
            capabilities.currentExtent.width != uint32_max && capabilities.currentExtent.height != uint32_max)
            return capabilities.currentExtent;

        // Ensure the extent is within the bounds of the surface capabilities
        const auto [ width, height ] = window_size;
        return {
            std::clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
            std::clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
        };
    }

    std::vector<vk::ImageView> createImageViews(
        const std::span<const vk::Image> images,
        const vk::Format& color_format,
        const vk::Device& device)
    {
        std::vector<vk::ImageView> image_views;
        image_views.reserve(images.size());

        // Create views for each image
        vk::ImageViewCreateInfo create_info(
            {},
            {},
            vk::ImageViewType::e2D,
            color_format,
            {},
            { vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 }
        );
        for (const auto& image : images) {
            create_info.image = image;
            image_views.push_back(device.createImageView(create_info));
        }
        return image_views;
    }
}
