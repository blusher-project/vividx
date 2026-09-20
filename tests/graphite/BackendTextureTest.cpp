/*
 * Copyright 2021 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "tests/Test.h"

#include "include/core/SkColorSpace.h"
#include "include/core/SkSurface.h"
#include "include/gpu/MutableTextureState.h"
#include "include/gpu/graphite/BackendTexture.h"
#include "include/gpu/graphite/Context.h"
#include "include/gpu/graphite/Image.h"
#include "include/gpu/graphite/Recorder.h"
#include "include/gpu/graphite/Surface.h"
#include "include/gpu/graphite/vk/VulkanGraphiteTypes.h"
#include "include/gpu/vk/VulkanMutableTextureState.h"
#include "include/gpu/vk/VulkanTypes.h"
#include "src/gpu/graphite/Caps.h"
#include "src/gpu/graphite/ContextPriv.h"
#include "src/gpu/graphite/Image_Graphite.h"
#include "src/gpu/graphite/ResourceTypes.h"
#include "src/gpu/graphite/vk/VulkanGraphiteUtils.h"

using namespace skgpu;
using namespace skgpu::graphite;

namespace {
    const SkISize kSize = {16, 16};
}

DEF_GRAPHITE_TEST_FOR_ALL_CONTEXTS(BackendTextureTest, reporter, context,
                                   CtsEnforcement::kApiLevel_202504) {
    auto caps = context->priv().caps();
    auto recorder = context->makeRecorder();

    Protected isProtected = Protected(context->supportsProtectedContent());

    TextureInfo info = caps->getDefaultSampledTextureInfo(VX_COLOR_TYPE_RGBA_8888,
                                                          /*mipmapped=*/Mipmapped::kNo,
                                                          isProtected,
                                                          Renderable::kNo);
    REPORTER_ASSERT(reporter, info.isValid());

    auto texture1 = recorder->createBackendTexture(kSize, info);
    REPORTER_ASSERT(reporter, texture1.isValid());

    // We make a copy to do the remaining tests so we still have texture1 to safely delete the
    // backend object.
    auto texture1Copy = texture1;
    REPORTER_ASSERT(reporter, texture1Copy.isValid());
    REPORTER_ASSERT(reporter, texture1 == texture1Copy);

    auto texture2 = recorder->createBackendTexture(kSize, info);
    REPORTER_ASSERT(reporter, texture2.isValid());

    REPORTER_ASSERT(reporter, texture1Copy != texture2);

    // Test state after assignment
    texture1Copy = texture2;
    REPORTER_ASSERT(reporter, texture1Copy.isValid());
    REPORTER_ASSERT(reporter, texture1Copy == texture2);

    BackendTexture invalidTexture;
    REPORTER_ASSERT(reporter, !invalidTexture.isValid());

    texture1Copy = invalidTexture;
    REPORTER_ASSERT(reporter, !texture1Copy.isValid());

    texture1Copy = texture1;
    REPORTER_ASSERT(reporter, texture1Copy.isValid());
    REPORTER_ASSERT(reporter, texture1 == texture1Copy);

    recorder->deleteBackendTexture(texture1);
    recorder->deleteBackendTexture(texture2);

    // Test that deleting is safe from the Context or a different Recorder.
    texture1 = recorder->createBackendTexture(kSize, info);
    context->deleteBackendTexture(texture1);

    auto recorder2 = context->makeRecorder();
    texture1 = recorder->createBackendTexture(kSize, info);
    recorder2->deleteBackendTexture(texture1);
}

// Tests the wrapping of a BackendTexture in an SkSurface
DEF_GRAPHITE_TEST_FOR_ALL_CONTEXTS(SurfaceBackendTextureTest,
                                   reporter,
                                   context,
                                   CtsEnforcement::kApiLevel_202604) {
    // TODO: Right now this just tests very basic combinations of surfaces. This should be expanded
    // to cover a much broader set of things once we add more support in Graphite for different
    // formats, color types, etc.

    auto caps = context->priv().caps();
    std::unique_ptr<Recorder> recorder = context->makeRecorder();

    TextureInfo info = caps->getDefaultSampledTextureInfo(VX_COLOR_TYPE_RGBA_8888,
                                                          /*mipmapped=*/Mipmapped::kNo,
                                                          Protected::kNo,
                                                          Renderable::kYes);

    auto texture = recorder->createBackendTexture(kSize, info);
    REPORTER_ASSERT(reporter, texture.isValid());

    sk_sp<SkSurface> surface = SkSurfaces::WrapBackendTexture(recorder.get(),
                                                              texture,
                                                              /*colorSpace=*/nullptr,
                                                              /*props=*/nullptr);
    REPORTER_ASSERT(reporter, surface);

    surface.reset();

    recorder->deleteBackendTexture(texture);

    // We should fail to wrap a non-renderable texture in a surface.
    info = caps->getDefaultSampledTextureInfo(VX_COLOR_TYPE_RGBA_8888,
                                              /*mipmapped=*/Mipmapped::kNo,
                                              Protected::kNo,
                                              Renderable::kNo);
    texture = recorder->createBackendTexture(kSize, info);
    REPORTER_ASSERT(reporter, texture.isValid());

    surface = SkSurfaces::WrapBackendTexture(recorder.get(),
                                             texture,
                                             /*colorSpace=*/nullptr,
                                             /*props=*/nullptr);

    REPORTER_ASSERT(reporter, !surface);
    recorder->deleteBackendTexture(texture);
}

// Tests the wrapping of a BackendTexture in an SkImage
DEF_GRAPHITE_TEST_FOR_ALL_CONTEXTS(ImageBackendTextureTest,
                                   reporter,
                                   context,
                                   CtsEnforcement::kApiLevel_202604) {
    // TODO: Right now this just tests very basic combinations of images. This should be expanded
    // to cover a much broader set of things once we add more support in Graphite for different
    // formats, color types, etc.

    const Caps* caps = context->priv().caps();
    std::unique_ptr<Recorder> recorder = context->makeRecorder();

    for (Mipmapped mipmapped : { Mipmapped::kYes, Mipmapped::kNo }) {
        for (Renderable renderable : { Renderable::kYes, Renderable::kNo }) {

            TextureInfo info = caps->getDefaultSampledTextureInfo(VX_COLOR_TYPE_RGBA_8888,
                                                                  mipmapped,
                                                                  Protected::kNo,
                                                                  renderable);

            BackendTexture texture = recorder->createBackendTexture(kSize, info);
            REPORTER_ASSERT(reporter, texture.isValid());

            sk_sp<SkImage> image = SkImages::WrapTexture(recorder.get(),
                                                         texture,
                                                         VX_ALPHA_TYPE_PREMULTIPLIED,
                                                         /*colorSpace=*/nullptr);
            REPORTER_ASSERT(reporter, image);
            REPORTER_ASSERT(reporter, image->hasMipmaps() == (mipmapped == Mipmapped::kYes));
            REPORTER_ASSERT(reporter, image->colorType() == VX_COLOR_TYPE_RGBA_8888);
            REPORTER_ASSERT(reporter, image->alphaType() == VX_ALPHA_TYPE_PREMULTIPLIED);

            image.reset();

            // We should switch to k888x when possible for forceOpaque
            image = SkImages::WrapTexture(recorder.get(),
                                          texture,
                                          VX_ALPHA_TYPE_UNKNOWN,
                                          /*colorSpace=*/nullptr);
            REPORTER_ASSERT(reporter, image);
            REPORTER_ASSERT(reporter, image->hasMipmaps() == (mipmapped == Mipmapped::kYes));
            REPORTER_ASSERT(reporter, image->colorType() == VX_COLOR_TYPE_RGB_888X);
            REPORTER_ASSERT(reporter, image->alphaType() == VX_ALPHA_TYPE_OPAQUE);

            image.reset();
            recorder->deleteBackendTexture(texture);

            // We should still be flagged as opaque and have a A=1 swizzle for color types that
            // don't have an A->X variant
            info = caps->getDefaultSampledTextureInfo(VX_COLOR_TYPE_ARGB_4444,
                                                      mipmapped,
                                                      Protected::kNo,
                                                      renderable);

            texture = recorder->createBackendTexture(kSize, info);
            // 4444 is not universally supported however
            if (texture.isValid()) {
                image = SkImages::WrapTexture(recorder.get(),
                                              texture,
                                              VX_ALPHA_TYPE_UNKNOWN,
                                              /*colorSpace=*/nullptr);
                REPORTER_ASSERT(reporter, image);
                REPORTER_ASSERT(reporter, image->hasMipmaps() == (mipmapped == Mipmapped::kYes));
                REPORTER_ASSERT(reporter, image->colorType() == VX_COLOR_TYPE_ARGB_4444);
                REPORTER_ASSERT(reporter, image->alphaType() == VX_ALPHA_TYPE_OPAQUE);

                Swizzle readSwizzle =
                        static_cast<Image*>(image.get())->textureProxyView().swizzle();
                REPORTER_ASSERT(reporter, readSwizzle[3] == '1');

                image.reset();
                recorder->deleteBackendTexture(texture);
            }

            // Now test handling of red format ambiguity
            info = caps->getDefaultSampledTextureInfo(VX_COLOR_TYPE_R8_UNORM,
                                                      mipmapped,
                                                      Protected::kNo,
                                                      renderable);
            texture = recorder->createBackendTexture(kSize, info);

            // Opaque/unknown becomes red + opaque
            image = SkImages::WrapTexture(recorder.get(),
                                          texture,
                                          VX_ALPHA_TYPE_OPAQUE,
                                          /*colorSpace=*/nullptr);
            REPORTER_ASSERT(reporter, image);
            REPORTER_ASSERT(reporter, image->hasMipmaps() == (mipmapped == Mipmapped::kYes));
            REPORTER_ASSERT(reporter, image->colorType() == VX_COLOR_TYPE_R8_UNORM);
            REPORTER_ASSERT(reporter, image->alphaType() == VX_ALPHA_TYPE_OPAQUE);

            image = SkImages::WrapTexture(recorder.get(),
                                          texture,
                                          VX_ALPHA_TYPE_UNKNOWN,
                                          /*colorSpace=*/nullptr);
            REPORTER_ASSERT(reporter, image);
            REPORTER_ASSERT(reporter, image->hasMipmaps() == (mipmapped == Mipmapped::kYes));
            REPORTER_ASSERT(reporter, image->colorType() == VX_COLOR_TYPE_R8_UNORM);
            REPORTER_ASSERT(reporter, image->alphaType() == VX_ALPHA_TYPE_OPAQUE);

            // Premul/unpremul becomes alpha-only
            image = SkImages::WrapTexture(recorder.get(),
                                          texture,
                                          VX_ALPHA_TYPE_PREMULTIPLIED,
                                          /*colorSpace=*/nullptr);
            REPORTER_ASSERT(reporter, image);
            REPORTER_ASSERT(reporter, image->hasMipmaps() == (mipmapped == Mipmapped::kYes));
            REPORTER_ASSERT(reporter, image->colorType() == VX_COLOR_TYPE_ALPHA_8);
            REPORTER_ASSERT(reporter, image->alphaType() == VX_ALPHA_TYPE_PREMULTIPLIED);

            image = SkImages::WrapTexture(recorder.get(),
                                          texture,
                                          VX_ALPHA_TYPE_UNPREMULTIPLIED,
                                          /*colorSpace=*/nullptr);
            REPORTER_ASSERT(reporter, image);
            REPORTER_ASSERT(reporter, image->hasMipmaps() == (mipmapped == Mipmapped::kYes));
            REPORTER_ASSERT(reporter, image->colorType() == VX_COLOR_TYPE_ALPHA_8);
            REPORTER_ASSERT(reporter, image->alphaType() == VX_ALPHA_TYPE_UNPREMULTIPLIED);

            image.reset();
            recorder->deleteBackendTexture(texture);

            // Test how an A8 texture format behaves, which does not have the red/alpha ambiguity
            // and stays as an alpha-only colortype regardless of SkAlphaType. A8 is not always
            // available, so skip it if the TextureInfo selects an R8 texture again.
            info = caps->getDefaultSampledTextureInfo(VX_COLOR_TYPE_ALPHA_8,
                                                      mipmapped,
                                                      Protected::kNo,
                                                      renderable);
            if (TextureInfoPriv::ViewFormat(info) != TextureFormat::kA8) {
                // A8 isn't always available and if we picked R8, we'd really just be re-testing
                // the above scenarios.
                continue;
            }
            texture = recorder->createBackendTexture(kSize, info);

            // Opaque/unknown becomes opaque alpha (not super useful)
            image = SkImages::WrapTexture(recorder.get(),
                                          texture,
                                          VX_ALPHA_TYPE_OPAQUE,
                                          /*colorSpace=*/nullptr);
            REPORTER_ASSERT(reporter, image);
            REPORTER_ASSERT(reporter, image->hasMipmaps() == (mipmapped == Mipmapped::kYes));
            REPORTER_ASSERT(reporter, image->colorType() == VX_COLOR_TYPE_ALPHA_8);
            REPORTER_ASSERT(reporter, image->alphaType() == VX_ALPHA_TYPE_OPAQUE);

            image = SkImages::WrapTexture(recorder.get(),
                                          texture,
                                          VX_ALPHA_TYPE_UNKNOWN,
                                          /*colorSpace=*/nullptr);
            REPORTER_ASSERT(reporter, image);
            REPORTER_ASSERT(reporter, image->hasMipmaps() == (mipmapped == Mipmapped::kYes));
            REPORTER_ASSERT(reporter, image->colorType() == VX_COLOR_TYPE_ALPHA_8);
            REPORTER_ASSERT(reporter, image->alphaType() == VX_ALPHA_TYPE_OPAQUE);
             Swizzle readSwizzle =
                        static_cast<Image*>(image.get())->textureProxyView().swizzle();
                REPORTER_ASSERT(reporter, readSwizzle[3] == '1');

            // Premul/unpremul stay alpha-only
            image = SkImages::WrapTexture(recorder.get(),
                                          texture,
                                          VX_ALPHA_TYPE_PREMULTIPLIED,
                                          /*colorSpace=*/nullptr);
            REPORTER_ASSERT(reporter, image);
            REPORTER_ASSERT(reporter, image->hasMipmaps() == (mipmapped == Mipmapped::kYes));
            REPORTER_ASSERT(reporter, image->colorType() == VX_COLOR_TYPE_ALPHA_8);
            REPORTER_ASSERT(reporter, image->alphaType() == VX_ALPHA_TYPE_PREMULTIPLIED);

            image = SkImages::WrapTexture(recorder.get(),
                                          texture,
                                          VX_ALPHA_TYPE_UNPREMULTIPLIED,
                                          /*colorSpace=*/nullptr);
            REPORTER_ASSERT(reporter, image);
            REPORTER_ASSERT(reporter, image->hasMipmaps() == (mipmapped == Mipmapped::kYes));
            REPORTER_ASSERT(reporter, image->colorType() == VX_COLOR_TYPE_ALPHA_8);
            REPORTER_ASSERT(reporter, image->alphaType() == VX_ALPHA_TYPE_UNPREMULTIPLIED);

            image.reset();
            recorder->deleteBackendTexture(texture);
        }
    }
}

#ifdef SK_VULKAN
DEF_GRAPHITE_TEST_FOR_VULKAN_CONTEXT(VulkanBackendTextureMutableStateTest, reporter, context,
                                     CtsEnforcement::kApiLevel_202404) {
    VulkanTextureInfo info(VK_SAMPLE_COUNT_1_BIT,
                           /*mipmapped=*/Mipmapped::kNo,
                           /*flags=*/0,
                           VK_FORMAT_R8G8B8A8_UNORM,
                           VK_IMAGE_TILING_OPTIMAL,
                           VK_IMAGE_USAGE_SAMPLED_BIT,
                           VK_SHARING_MODE_EXCLUSIVE,
                           VK_IMAGE_ASPECT_COLOR_BIT,
                           /*ycbcrConversionInfo*/{});

    BackendTexture texture = BackendTextures::MakeVulkan({16, 16},
                                                         info,
                                                         VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                                         /*queueFamilyIndex=*/1,
                                                         VK_NULL_HANDLE,
                                                         skgpu::VulkanAlloc());

    REPORTER_ASSERT(reporter, texture.isValid());
    REPORTER_ASSERT(
            reporter,
            BackendTextures::GetVkImageLayout(texture) == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    REPORTER_ASSERT(reporter, BackendTextures::GetVkQueueFamilyIndex(texture) == 1);

    skgpu::MutableTextureState newState =
            skgpu::MutableTextureStates::MakeVulkan(VK_IMAGE_LAYOUT_GENERAL, 0);
    BackendTextures::SetMutableState(&texture, newState);

    REPORTER_ASSERT(reporter,
                    BackendTextures::GetVkImageLayout(texture) == VK_IMAGE_LAYOUT_GENERAL);
    REPORTER_ASSERT(reporter, BackendTextures::GetVkQueueFamilyIndex(texture) == 0);

    // TODO: Add to this test to check that the setMutableState calls also update values we see in
    // wrapped VulkanTextures once we have them. Also check that updates in VulkanTexture are also
    // visible in the getters of BackendTexture. We will need a real VkImage to do these tests.
}
#endif // SK_VULKAN
