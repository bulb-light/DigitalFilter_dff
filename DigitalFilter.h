// SPDX-FileCopyrightText: 2025 David Chirme Sisa ([dff-laoise](https://github.com/bulb-light)) 
// SPDX-License-Identifier: MIT
// For full license text, see the LICENSE file in the repository root
// or https://opensource.org/licenses/MIT

/**
 * @file DigitalFilter.h
 * @brief Library for implementing discrete digital filters.
 *
 * This library provides a class to implement and compute outputs for
 * Moving Average filter and Exponentially Weighted Moving Average (EWMA) Low Pass filter.
 *
 * @note This class is designed to be used in embedded systems, such as Arduino and ESP32 boards.
 */

#ifndef DFF_DIGITALFILTER_H
#define DFF_DIGITALFILTER_H

/**
 * @class DigitalFilter
 * @brief A class to implement discrete digital filters for signal processing.
 *
 * This class supports two types of filters:
 * - Moving Average Filter
 * - Exponentially Weighted Moving Average (EWMA) Low Pass Filter
 * 
 * Example usage:
 * @code
 * DigitalFilter avgFilter(DigitalFilter::Type::MovingAverage, 5); // 5-point Moving Average Filter
 * float output = avgFilter.computeFilterOut(inputSample);
 * @endcode
 */
class DigitalFilter {
    public:
        /**
         * @enum Type
         * @brief Enumeration for the types of digital filters supported.
         */
        enum class Type {
            MovingAverage, ///< Simple n-point Moving Average Filter
            EWMALowPass, ///< Exponentially Weighted Moving Average Low Pass Filter
        };

        /**
         * @brief Constructor for the Moving Average Filter.
         * @param type The type of filter (should be Type::MovingAverage).
         * @param windowSize The size of the moving average window.
         */
        DigitalFilter(Type type, int windowSize);

        /**
         * @brief Constructor for the EWMA Low Pass Filter.
         * @param type The type of filter (should be Type::EWMALowPass).
         * @param cutoffFrequency The cutoff frequency for the low pass filter.
         * @param samplingRate The sampling rate of the input signal.
         */
        DigitalFilter(Type type, float cutoffFrequency, float samplingRate);

        /**
         * @brief Destructor for the DigitalFilter class.
         */
        ~DigitalFilter();

        /**
         * @brief Reset the internal states of the filter.
         */
        void reset();

        /**
         * @brief Compute the output of the filter for a given input sample.
         * @param input The input sample to process.
         * @return The filtered output.
         */
        float computeFilterOut(float input);

    private:
        Type mFilterType; ///< The type of the filter.

        // For Moving Average Filter states
        int mWindowSize; ///< The size of the moving average window.
        float* mBuffer; ///< Circular buffer for storing input samples.
        int mBufferIndex; ///< Current index in the circular buffer.
        float mBufferSum; ///< Sum of the values in the buffer.

        // For EWMA Low Pass Filter states
        float mAlpha; ///< Smoothing factor for the EWMA filter.
        float mPrevOutput; ///< Previous output value for the EWMA filter.

        /**
         * @brief Internal helper function to compute the Moving Average filter output.
         * @param input The input sample to process.
         * @return The filtered output.
         */
        float applyMovingAverage(float input);

        /**
         * @brief Internal helper function to compute the EWMA Low Pass filter output.
         * @param input The input sample to process.
         * @return The filtered output.
         */
        float applyEWMALowPass(float input);
};

#endif // DFF_DIGITALFILTER_H
