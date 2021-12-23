/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioThumbnailComp.h"
#include "Biquad.h"
#include "Biquad2.h"
#include "maximilian.h"

//==============================================================================
/**
*/
class GramophoneAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    GramophoneAudioProcessor();
    ~GramophoneAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    AudioProcessorValueTreeState state;
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    float freqVal;
    float QVal;
    float ampVal = 1.f;
    
    float freqVal2;
    float QVal2 = 2.f;
    float ampVal2 = 0.1f;
    
    AudioTransportSource transportSource;
    AudioFormatManager formatManager;
    File currentlyLoadedFile;
    AudioThumbnailCache thumbnailCache;
    
    void loadFileIntoTransport(const File& audioFile);
    
    Biquad::FilterType filterType = Biquad::FilterType::HPF;
    Biquad2::FilterType2 filterType2 = Biquad2::FilterType2::LPF;

private:
    AudioProcessorValueTreeState treeState;
    
    Biquad biquad;
    Biquad2 biquad2;
    
    maxiSample maxiSample;
    
    ScopedPointer<AudioFormatReaderSource> currentAudioFileSource;
    TimeSliceThread readAheadThread;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GramophoneAudioProcessor)
};
