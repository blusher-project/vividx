/*
 * Copyright 2021 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "src/gpu/ganesh/GrYUVATextureProxies.h"

#include "include/core/SkColor.h"
#include "include/core/SkTypes.h"
#include "include/gpu/ganesh/GrBackendSurface.h"
#include "include/private/gpu/ganesh/GrTypesPriv.h"
#include "src/core/SkYUVAInfoLocation.h"
#include "src/gpu/ganesh/GrTextureProxy.h"

#include <cstddef>
#include <cstdint>
#include <utility>

#ifdef SK_DEBUG
static int num_channels(uint32_t channelFlags) {
    switch (channelFlags) {
        case VX_COLOR_CHANNEL_FLAG_RED        : return 1;
        case VX_COLOR_CHANNEL_FLAG_ALPHA      : return 1;
        case VX_COLOR_CHANNEL_FLAG_GRAY       : return 1;
        case VX_COLOR_CHANNEL_FLAGS_GRAY_ALPHA : return 2;
        case VX_COLOR_CHANNEL_FLAGS_RG        : return 2;
        case VX_COLOR_CHANNEL_FLAGS_RGB       : return 3;
        case VX_COLOR_CHANNEL_FLAGS_RGBA      : return 4;

        default:
            SkDEBUGFAILF("Unexpected channel combination 0x%08x", channelFlags);
            return 0;
    }
}
#endif

GrYUVATextureProxies::GrYUVATextureProxies(const SkYUVAInfo& yuvaInfo,
                                           sk_sp<GrSurfaceProxy> proxies[SkYUVAInfo::kMaxPlanes],
                                           GrSurfaceOrigin textureOrigin)
        : fYUVAInfo(yuvaInfo), fTextureOrigin(textureOrigin) {
    int n = yuvaInfo.numPlanes();
    if (n == 0) {
        *this = {};
        SkASSERT(!this->isValid());
        return;
    }
    uint32_t textureChannelMasks[SkYUVAInfo::kMaxPlanes];
    for (int i = 0; i < n; ++i) {
        if (!proxies[i]) {
            *this = {};
            SkASSERT(!this->isValid());
            return;
        }
        textureChannelMasks[i] = proxies[i]->backendFormat().channelMask();
    }
    fYUVALocations = yuvaInfo.toYUVALocations(textureChannelMasks);
    if (fYUVALocations[0].fPlane < 0) {
        *this = {};
        SkASSERT(!this->isValid());
        return;
    }
    fMipmapped = skgpu::Mipmapped::kYes;
    for (size_t i = 0; i < static_cast<size_t>(n); ++i) {
        if (!proxies[i]) {
            *this = {};
            SkASSERT(!this->isValid());
            return;
        }
        SkASSERT(proxies[i]->asTextureProxy());
        if (proxies[i]->asTextureProxy()->mipmapped() == skgpu::Mipmapped::kNo) {
            fMipmapped = skgpu::Mipmapped::kNo;
        }
        fProxies[i] = std::move(proxies[i]);
    }
    SkASSERT(this->isValid());
}

GrYUVATextureProxies::GrYUVATextureProxies(const SkYUVAInfo& yuvaInfo,
                                           GrSurfaceProxyView views[SkYUVAInfo::kMaxPlanes],
                                           GrColorType colorTypes[SkYUVAInfo::kMaxPlanes])
        : fYUVAInfo(yuvaInfo) {
    uint32_t pixmapChannelMasks[SkYUVAInfo::kMaxPlanes];
    int n = yuvaInfo.numPlanes();
    if (n == 0) {
        *this = {};
        SkASSERT(!this->isValid());
        return;
    }
    fMipmapped = skgpu::Mipmapped::kYes;
    for (int i = 0; i < n; ++i) {
        pixmapChannelMasks[i] = GrColorTypeChannelFlags(colorTypes[i]);
        SkASSERT(num_channels(pixmapChannelMasks[i]) <=
                 num_channels(views[i].proxy()->backendFormat().channelMask()));
        if (!views[i] || views[i].origin() != views[0].origin()) {
            *this = {};
            SkASSERT(!this->isValid());
            return;
        }
        if (views[i].proxy()->asTextureProxy()->mipmapped() == skgpu::Mipmapped::kNo) {
            fMipmapped = skgpu::Mipmapped::kNo;
        }
    }
    // Initial locations refer to the CPU pixmap channels.
    fYUVALocations = yuvaInfo.toYUVALocations(pixmapChannelMasks);
    if (fYUVALocations[0].fPlane < 0) {
        *this = {};
        SkASSERT(!this->isValid());
        return;
    }

    // Run each location through the proxy view's swizzle to get the actual texture format channel.
    for (int i = 0; i < SkYUVAInfo::kYUVAChannelCount; ++i) {
        int plane = fYUVALocations[i].fPlane;
        if (plane >= 0) {
            int chanAsIdx = static_cast<int>(fYUVALocations[i].fChannel);
            switch (views[plane].swizzle()[chanAsIdx]) {
                case 'r': fYUVALocations[i].fChannel = VX_COLOR_CHANNEL_R; break;
                case 'g': fYUVALocations[i].fChannel = VX_COLOR_CHANNEL_G; break;
                case 'b': fYUVALocations[i].fChannel = VX_COLOR_CHANNEL_B; break;
                case 'a': fYUVALocations[i].fChannel = VX_COLOR_CHANNEL_A; break;

                default:
                    SkDEBUGFAILF("Unexpected swizzle value: %c", views[i].swizzle()[chanAsIdx]);
                    *this = {};
                    SkASSERT(!this->isValid());
                    return;
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        fProxies[i] = views[i].detachProxy();
    }
    fTextureOrigin = views[0].origin();
    SkASSERT(this->isValid());
}
