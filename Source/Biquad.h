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

class Biquad
{
public:
    enum FilterType
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

    Biquad() {};
    Biquad(FilterType filterType, float Q) : filterType(filterType), Q(Q) {};
    ~Biquad() {};

    // Functions for Biquad Filter
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
        filterType = *selection;
    }

    void setFilterType (FilterType filterTypeParam)
    {
        if (filterType == 1){
            this->filterType = HPF;
        }
        if(filterType ==  2){
            this->filterType = LPF;
        }
        if(filterType == 3){
            this->filterType = BPF1;
        }
        if(filterType == 4){
            this->filterType = BPF2;
        }
        if(filterType == 5){
            this->filterType = NOTCH;
        }
//        else{
//            this->filterType = HPF;
//        }
    }
    

private:
    int filterType = 3;

    float Fs = 48000.0f; // Sampling Rate

    // Variables for User to Modify Filter
    float freq = 20.0f; // frequency in Hz
    float freq2 = 20.f;
    float Q = 2.f; // Q => [0.1 - 10]
    float ampdB = 0.f; // Amplitude on dB scale

    // Intermediate Variables
    float w0 = (float) (2.0 * M_PI) * freq / Fs; // Normalize frequency
    float alpha = std::sin(w0) / (2.0f * Q); // Bandwidth/slope/resonance parameter
    float A = std::pow(10.0f, ampdB / 40.0f); // Linear amplitude

    // Variables for Biquad Implementation
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
