/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class GramaphoneAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    GramaphoneAudioProcessorEditor (GramaphoneAudioProcessor&);
    ~GramaphoneAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    Slider filterSlider;
    Slider filterSlider2;
    Label filterLabel1;
    Slider qSlider;
    std::vector<std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>> sliderAttachment;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GramaphoneAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GramaphoneAudioProcessorEditor)
};
