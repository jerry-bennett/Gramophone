/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GramaphoneAudioProcessorEditor::GramaphoneAudioProcessorEditor (GramaphoneAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

GramaphoneAudioProcessorEditor::~GramaphoneAudioProcessorEditor()
{
}

//==============================================================================
void GramaphoneAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    filterSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    filterSlider.setBounds(10, 10, 150, 150);
    filterSlider.setRange(200.f, 18000.f, 10);
    //filterSlider.setValue(processor.freqVal);
    filterSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 30);
    addAndMakeVisible(filterSlider);
    
    filterSlider2.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    filterSlider2.setBounds(10, 150, 150, 150);
    filterSlider2.setRange(200.f, 18000.f, 10);
    //filterSlider.setValue(processor.freqVal);
    filterSlider2.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 30);
    addAndMakeVisible(filterSlider2);
    
    filterLabel1.setText("HIGH PASS", dontSendNotification);
    filterLabel1.setBounds(10, 170, 50, 150);
    addAndMakeVisible(filterLabel1);
    
    
    qSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    qSlider.setBounds(300, 100, 150, 150);
    qSlider.setRange(0.1f, 10.f, 0.1f);
    qSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 30);
    addAndMakeVisible(qSlider);
    
    sliderAttachment.emplace_back(new AudioProcessorValueTreeState::SliderAttachment(processor.state,"FREQ",filterSlider));
    sliderAttachment.emplace_back(new AudioProcessorValueTreeState::SliderAttachment(processor.state,"FREQ2",filterSlider2));
    sliderAttachment.emplace_back(new AudioProcessorValueTreeState::SliderAttachment(processor.state,"Q",qSlider));
    
}

void GramaphoneAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
