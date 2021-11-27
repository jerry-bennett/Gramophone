/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GramaphoneAudioProcessor::GramaphoneAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),state(*this,nullptr,Identifier("ChainParameters"),createParameterLayout()),
                        treeState(*this, nullptr, ProjectInfo::projectName, createParameterLayout())
#endif
{
}

GramaphoneAudioProcessor::~GramaphoneAudioProcessor()
{
}

AudioProcessorValueTreeState::ParameterLayout GramaphoneAudioProcessor::createParameterLayout(){
    
    std::vector<std::unique_ptr<RangedAudioParameter>> params;
    
    //lowpass
    params.push_back(std::make_unique<AudioParameterInt>("FREQ","Freq",200,18000,10));
    
    //highpass
    params.push_back(std::make_unique<AudioParameterInt>("FREQ2","Freq2",400,16000,10));
    
    params.push_back(std::make_unique<AudioParameterFloat>("Q","Q",0.1,10,0.1));
    
    return {params.begin() , params.end()};
}

//==============================================================================
const juce::String GramaphoneAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GramaphoneAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GramaphoneAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GramaphoneAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GramaphoneAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GramaphoneAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GramaphoneAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GramaphoneAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GramaphoneAudioProcessor::getProgramName (int index)
{
    return {};
}

void GramaphoneAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GramaphoneAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void GramaphoneAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GramaphoneAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void GramaphoneAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    freqVal = 4500;
    freqVal2 = 1300;
    QVal = 9.5f;
    ampVal = 1.f;
    
    biquad.setFreq(freqVal);
    biquad.setQ(QVal);
    biquad.setAmpdB(ampVal);
    biquad.setFilterType(filterType);
    
    biquad2.setFreq(freqVal2);
    biquad2.setQ(QVal);
    biquad2.setAmpdB(ampVal2);
    biquad2.setFilterType(filterType2);
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        for (int n = 0; n < buffer.getNumSamples() ; n++){
            float x = buffer.getReadPointer(channel)[n];
            float z = biquad2.processSample(x, channel);
            float y = biquad.processSample(z, channel);
            buffer.getWritePointer(channel)[n] = 0.5 * y;
        }

    }
}

//==============================================================================
bool GramaphoneAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GramaphoneAudioProcessor::createEditor()
{
    return new GramaphoneAudioProcessorEditor (*this);
}

//==============================================================================
void GramaphoneAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GramaphoneAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GramaphoneAudioProcessor();
}
