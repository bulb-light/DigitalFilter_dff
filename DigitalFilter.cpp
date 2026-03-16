// SPDX-FileCopyrightText: 2025 David Chirme Sisa ([dff-laoise](https://github.com/bulb-light)) 
// SPDX-License-Identifier: MIT
// For full license text, see the LICENSE file in the repository root
// or https://opensource.org/licenses/MIT

/**
 * @file DigitalFilter.cpp
 * @brief Implementation of the DigitalFilter class.
 */

#include "DigitalFilter.h"

// Moving average constructor
DigitalFilter::DigitalFilter(Type type, int windowSize)
    :   mFilterType(type),
        mWindowSize(windowSize),
        mBuffer(nullptr),
        mBufferIndex(0),
        mBufferSum(0),
        mAlpha(0),
        mPrevOutput(0)
{
    // Only allocate buffer for Moving Average Filter if needed
    if (mFilterType == Type::MovingAverage && mWindowSize > 0) {
        mBuffer = new float[mWindowSize];
        // Initialize buffer to zero manually
        for (int i = 0; i < mWindowSize; ++i) {
            mBuffer[i] = 0.0f;
        }
    }
}

// EWMA constructor
DigitalFilter::DigitalFilter(Type type, float cutoffFrequency, float samplingRate)
    :   mFilterType(type),
        mWindowSize(0),
        mBuffer(nullptr),
        mBufferIndex(0),
        mBufferSum(0),
        mPrevOutput(0)
{
    if (mFilterType == Type::EWMALowPass) {
        // Calculate alpha based on cutoff frequency and sampling rate
        // Simple difference form derive from continuous-time RC low-pass filter
        // wc = 2 * pi * cutoffFrequency and fs = samplingRate
        // alphga = (wc) / (wc + fs)
        float wc = 2.0f * 3.14159f * cutoffFrequency;
        mAlpha = wc / (wc + samplingRate);
    }
}

DigitalFilter::~DigitalFilter() {
    if (mBuffer) {
        delete[] mBuffer;
    }
}

void DigitalFilter::reset() {
    // Reset Moving Average states
    mBufferIndex = 0;
    mBufferSum = 0;
    // Reset buffer values to zero
    for (int i = 0; i < mWindowSize; ++i) {
        mBuffer[i] = 0.0f;
    }
    // Reset EWMA state
    mPrevOutput = 0.0f;
}

float DigitalFilter::computeFilterOut(float input) {
    switch (mFilterType) {
        case Type::MovingAverage:
            return applyMovingAverage(input);
        case Type::EWMALowPass:
            return applyEWMALowPass(input);
        default:
            return input; // Default case (should not happen)
    }
    return input; // Default case (should not happen)
}

float DigitalFilter::applyMovingAverage(float input) {
    // Subtract the oldest value from the sum
    mBufferSum -= mBuffer[mBufferIndex];
    // Add the new input to the buffer and sum
    mBuffer[mBufferIndex] = input;
    mBufferSum += input;
    // Move the buffer index forward
    mBufferIndex = (mBufferIndex + 1) % mWindowSize;
    // Return the average
    return mBufferSum / mWindowSize;
}

float DigitalFilter::applyEWMALowPass(float input) {
    // Apply EWMA formula
    float output = mAlpha * input + (1.0f - mAlpha) * mPrevOutput;
    mPrevOutput = output;
    return output;
}
