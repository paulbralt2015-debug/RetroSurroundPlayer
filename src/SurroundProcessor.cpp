#include "SurroundProcessor.h"

#include <algorithm>

VirtualSurroundSample SurroundProcessor::process(StereoSample input) noexcept {
    const float left = std::clamp(input.left, -1.0f, 1.0f);
    const float right = std::clamp(input.right, -1.0f, 1.0f);

    const float mid = 0.5f * (left + right);
    const float side = 0.5f * (left - right);

    if (mode_ == SurroundMode::Direct) {
        return {left, 0.0f, right, 0.0f};
    }

    if (mode_ == SurroundMode::StereoWidening) {
        // Deliberately subtle decorrelation/ambience experiment, not a decoder.
        surroundLowPass_ += 0.12f * (side - surroundLowPass_);
        const float widenedLeft = std::clamp(mid + 1.15f * side, -1.0f, 1.0f);
        const float widenedRight = std::clamp(mid - 1.15f * side, -1.0f, 1.0f);
        const float ambience = 0.18f * (surroundLowPass_ - previousMid_ * 0.15f);
        previousMid_ = mid;
        return {widenedLeft, 0.0f, widenedRight, ambience};
    }

    // Early matrix-surround experiment:
    // center-like content is correlated L+R; surround-like content is L-R.
    // Legacy Dolby Surround encoding and modern stereo mixes are not reliably
    // distinguishable from samples alone, so this mode is intentionally labelled experimental.
    surroundLowPass_ += 0.08f * (side - surroundLowPass_);
    const float center = std::clamp(mid, -1.0f, 1.0f);
    const float surround = std::clamp(0.65f * surroundLowPass_, -1.0f, 1.0f);
    const float frontLeft = std::clamp(left - 0.35f * center, -1.0f, 1.0f);
    const float frontRight = std::clamp(right - 0.35f * center, -1.0f, 1.0f);
    previousMid_ = mid;
    return {frontLeft, center, frontRight, surround};
}

void SurroundProcessor::reset() noexcept {
    surroundLowPass_ = 0.0f;
    previousMid_ = 0.0f;
}
