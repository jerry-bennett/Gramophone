/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GramophoneAudioProcessorEditor::GramophoneAudioProcessorEditor (GramophoneAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    thumbnail = new AudioThumbnailComp(processor.formatManager, processor.transportSource, processor.thumbnailCache, processor.currentlyLoadedFile);
    addAndMakeVisible(thumbnail);
    thumbnail->addChangeListener(this);

    addAndMakeVisible(startStopButton);
    startStopButton.setButtonText("Play/Stop");
    startStopButton.addListener(this);
    startStopButton.setColour(TextButton::buttonColourId, Colour(0xff79ed7f));
    
    startStopButton.setButtonText("Play/Stop");
    startStopButton.addListener(this);
    startStopButton.setColour(TextButton::buttonColourId, Colour(0xff79ed7f));
    
    setOpaque(true);

    setSize(512, 220);
}

GramophoneAudioProcessorEditor::~GramophoneAudioProcessorEditor()
{
    thumbnail->removeChangeListener(this);
}

//==============================================================================
void GramophoneAudioProcessorEditor::paint (juce::Graphics& g)
{
    
    processor.transportSource.setPosition(0);
    processor.transportSource.start();
    
//    thumbnail = new AudioThumbnailComp(processor.formatManager, processor.transportSource, processor.thumbnailCache, processor.currentlyLoadedFile);
//    thumbnail->addChangeListener(this);
//    startStopButton.setBounds(10, 50, 50, 100);
//    addAndMakeVisible(startStopButton);
    
    
    addAndMakeVisible(thumbnail);
    
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    filterSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    filterSlider.setBounds(10, 10, 150, 150);
    filterSlider.setRange(200.f, 18000.f, 10);
    //filterSlider.setValue(processor.freqVal);
    filterSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 30);
    //addAndMakeVisible(filterSlider);
    
    filterSlider2.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    filterSlider2.setBounds(10, 150, 150, 150);
    filterSlider2.setRange(200.f, 18000.f, 10);
    //filterSlider.setValue(processor.freqVal);
    filterSlider2.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 30);
    //addAndMakeVisible(filterSlider2);
    
    filterLabel1.setText("HIGH PASS", dontSendNotification);
    filterLabel1.setBounds(10, 170, 50, 150);
    //addAndMakeVisible(filterLabel1);
    
    
    qSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    qSlider.setBounds(300, 100, 150, 150);
    qSlider.setRange(0.1f, 10.f, 0.1f);
    qSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 30);
    //addAndMakeVisible(qSlider);
    
    
    sliderAttachment.emplace_back(new AudioProcessorValueTreeState::SliderAttachment(processor.state,"FREQ",filterSlider));
    sliderAttachment.emplace_back(new AudioProcessorValueTreeState::SliderAttachment(processor.state,"FREQ2",filterSlider2));
    sliderAttachment.emplace_back(new AudioProcessorValueTreeState::SliderAttachment(processor.state,"Q",qSlider));
    
}

void GramophoneAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void GramophoneAudioProcessorEditor::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == &startStopButton)
    {
        if (processor.transportSource.isPlaying())
        {
            processor.transportSource.stop();
        }
        else
        {
            processor.transportSource.setPosition(0);
            processor.transportSource.start();
        }
    }
}

void GramophoneAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* source)
{
    if (source == thumbnail)
    {
        processor.loadFileIntoTransport(thumbnail->getLastDroppedFile());
        thumbnail->setFile(thumbnail->getLastDroppedFile());
    }
}
