//
//  Biquad2.h

/*
    This class implements the RBJ Audio EQ Cookbook Formulas

        http://shepazu.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html
 
    There are options for all of the standard filter types: LPF, HPF, etc. These
    filters are great as the building blocks for more complicated (i.e. higher
    order) filters because the RBJ filters are stable and work will when
    parameters are changed.
 
    Note: A Butterworth filter (flat passband) can be created by setting the RBJ
    filter's Q=sqrt(2)/2
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <cmath>

class Biquad2
{
public:
    enum FilterType2
    {
        LPF,
        HPF,
        BPF1,
        BPF2,
        NOTCH,
        LSHELF,
        HSHELF,
        PEAK,
        APF
    };

    Biquad2() {};
    Biquad2(FilterType2 filterType, float Q) : filterType2(filterType2), Q(Q) {};
    ~Biquad2() {};

    // Functions for Biquad2 Filter
    void processBuffer(float * samples, const int numSamples, const int channel);
        
    float processSample(float x, int channel);

    void setFs(float Fs);
    float getFs();

    void setFreq(float freq);
    void setFreq2(float freq2);
    float getFreq();
    float getFreq2();

    void setQ(float Q);
    float getQ();

    void setAmpdB(float ampdB);
    float getAmpdB();
    
    void getFilterType(float* selection){
        filterType2 = *selection;
    }

    void setFilterType (FilterType2 filterTypeParam)
    {
        if (filterType2 == 1){
            this->filterType2 = HPF;
        }
        if(filterType2 ==  2){
            this->filterType2 = LPF;
        }
        if(filterType2 == 3){
            this->filterType2 = BPF1;
        }
        if(filterType2 == 4){
            this->filterType2 = BPF2;
        }
//        else{
//            this->filterType = HPF;
//        }
    }
    

private:
    int filterType2 = 1;

    float Fs = 48000.0f; // Sampling Rate

    // Variables for User to Modify Filter
    float freq = 20.0f; // frequency in Hz
    float freq2 = 20.f;
    float Q = 2.f; // Q => [0.1 - 10]
    float ampdB = 10.f; // Amplitude on dB scale

    // Intermediate Variables
    float w0 = (float) (2.0 * M_PI) * freq / Fs; // Normalize frequency
    float alpha = std::sin(w0) / (2.0f * Q); // Bandwidth/slope/resonance parameter
    float A = std::pow(10.0f, ampdB / 40.0f); // Linear amplitude

    // Variables for Biquad2 Implementation
    // 2 channels - L,R : Up to 2nd Order
    float x1[2] = { 0.0f }; // 1 sample of delay feedforward
    float x2[2] = { 0.0f }; // 2 samples of delay feedforward
    float y1[2] = { 0.0f }; // 1 sample of delay feedback
    float y2[2] = { 0.0f }; // 2 samples of delay feedback

    // Filter coefficients
    float b0 = 1.0f; // initialized to pass signal
    float b1 = 0.0f; // without filtering
    float b2 = 0.0f;
    float a0 = 1.0f;
    float a1 = 0.0f;
    float a2 = 0.0f;

    void updateCoefficients();
};
