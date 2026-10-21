#pragma once

#include <cstddef>

enum class SurroundMode {
    Direct,
    StereoWidening,
    MatrixSurround
};

struct StereoSample {
    float left = 0.0f;
    float right = 0.0f;
};

struct VirtualSurroundSample {
    float frontLeft = 0.0f;
    float center = 0.0f;
    float frontRight = 0.0f;
    float surround = 0.0f;
};

// Small, allocation-free prototype DSP helper.
// This is NOT a certified Dolby decoder and does not implement Pro Logic II steering.
class SurroundProcessor {
public:
    void setMode(SurroundMode mode) noexcept { mode_ = mode; }
    SurroundMode mode() const noexcept { return mode_; }

    // Convert one stereo sample into a virtual 3-channel-plus-surround representation.
    // The caller still needs a proper renderer/mixer to produce stereo or binaural output.
    VirtualSurroundSample process(StereoSample input) noexcept;

    void reset() noexcept;

private:
    SurroundMode mode_ = SurroundMode::Direct;
    float surroundLowPass_ = 0.0f;
    float previousMid_ = 0.0f;
};
