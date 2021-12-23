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
class GramophoneAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                        private Button::Listener,
                                        private ChangeListener
{
public:
    GramophoneAudioProcessorEditor (GramophoneAudioProcessor&);
    ~GramophoneAudioProcessorEditor() override;

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
    GramophoneAudioProcessor& processor;

    ScopedPointer<AudioThumbnailComp> thumbnail;

    TextButton startStopButton;

    void buttonClicked(Button* buttonThatWasClicked) override;

    void changeListenerCallback(ChangeBroadcaster* source) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GramophoneAudioProcessorEditor)
};
