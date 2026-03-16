// SPDX-FileCopyrightText: 2025 David Chirme Sisa ([dff-laoise](https://github.com/bulb-light)) 
// SPDX-License-Identifier: MIT
// For full license text, see the LICENSE file in the repository root
// or https://opensource.org/licenses/MIT

/**
 * @file DigitalFilterExample.cpp
 * @brief Example code to demonstrate the usage of the DigitalFilter library
 *  with the Arduino framework.
 *
 * This example shows how to use both the Moving Average Filter and the
 * Exponentially Weighted Moving Average (EWMA) Low Pass Filter.
 */

#include <Arduino.h>
#include "DigitalFilter.h"

// Define constants
const int windowSize = 10; // Window size for Moving Average Filter
const float cutoffFrequency = 1.0; // Cutoff frequency for EWMA Low Pass Filter (Hz)
const float samplingRate = 10.0; // Sampling rate (Hz)
const int numSamples = 100; // Number of samples to generate

// Create filter instances
DigitalFilter movingAverageFilter(DigitalFilter::Type::MovingAverage, windowSize);
DigitalFilter ewmaLowPassFilter(DigitalFilter::Type::EWMALowPass, cutoffFrequency, samplingRate);

// Function to generate a noisy sine wave
void generateNoisySineWave(float* signal, int length, float amplitude, float frequency, float noiseLevel) {
    for (int i = 0; i < length; ++i) {
        float t = i / samplingRate; // Time in seconds
        float sineValue = amplitude * sin(2 * PI * frequency * t);
        // Random noise from -0.5*noiseLevel to +0.5*noiseLevel
        float noise = noiseLevel * ((float)rand() / RAND_MAX - 0.5f);
        signal[i] = sineValue + noise;
    }
}

// Function to generate a ramp signal
void generateTestArray(float* signal, int length) {
    for (int i = 0; i < length; ++i) {
        signal[i] = (float)i; // Simple ramp signal
    }
}

void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    while (!Serial) {
        ; // Wait for serial port to connect (needed for some boards)
    }

    // Serial.println("\nDigitalFilter Library Test");
    // Serial.println("===========================");

    // Test EWMA Low Pass Filter and Moving Average Filter with a noisy sine wave
    // Generate a noisy sine wave
    float testSignal[numSamples];
    // Amplitude=1.0, Frequency=0.5Hz, NoiseLevel=0.3
    generateNoisySineWave(testSignal, numSamples, 1.0, 0.5, 0.3);
    // Process the noisy sine wave with the EWMA Low Pass Filter
    Serial.println("\nNoisy Sine Wave Filtered Output:");
    for (int i = 0; i < numSamples; ++i) {
        float filteredOutputEWMA = ewmaLowPassFilter.computeFilterOut(testSignal[i]);
        float filteredOutputMA = movingAverageFilter.computeFilterOut(testSignal[i]);
        // Serial.print("Input: ");
        Serial.print(testSignal[i]);
        // Serial.print(" -> Filtered Output: ");
        Serial.print(" ");
        Serial.print(filteredOutputEWMA);
        Serial.print(" ");
        Serial.println(filteredOutputMA);

        // Simulate real-time sampling (100 ms delay for 10Hz sampling rate)
        delay((int)(1/samplingRate * 1000));
    };

    // Reset the filter states
    ewmaLowPassFilter.reset();
    movingAverageFilter.reset();

    // NOTE: Uncomment the following block to test the Moving Average Filter
    // with a ramp signal

    // Test Moving Average Filter with a ramp signal
    // Generate a ramp signal
    // float rampSignal[numSamples];
    // generateTestArray(rampSignal, numSamples);
    // // Process the ramp signal with the Moving Average Filter
    // Serial.println("\nRamp Signal Filtered Output (Moving Average filter):");
    // for (int i = 0; i < numSamples; ++i) {
    //     float filteredOutput = movingAverageFilter.computeFilterOut(rampSignal[i]);
    //     Serial.print(rampSignal[i]);
    //     Serial.print(" ");
    //     Serial.println(filteredOutput);

    //     // Simulate real-time sampling (arbitrary 10 ms delay)
    //     delay(10);
    // }

    // Reset the filter states
    movingAverageFilter.reset();
}

void loop() {
    // Do nothing in the loop
}
