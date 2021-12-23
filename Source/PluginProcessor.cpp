/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "maximilian.h"

//==============================================================================
GramophoneAudioProcessor::GramophoneAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),state(*this,nullptr,Identifier("ChainParameters"),createParameterLayout()),
                        treeState(*this, nullptr, ProjectInfo::projectName, createParameterLayout()),
                        thumbnailCache(1),
                        readAheadThread("transport read ahead")
                        {
                            formatManager.registerBasicFormats();
                            readAheadThread.startThread(3);
                        }
#endif
//{
//}

GramophoneAudioProcessor::~GramophoneAudioProcessor()
{
    transportSource.setSource(nullptr);
}

AudioProcessorValueTreeState::ParameterLayout GramophoneAudioProcessor::createParameterLayout(){
    
    std::vector<std::unique_ptr<RangedAudioParameter>> params;
    
    //lowpass
    params.push_back(std::make_unique<AudioParameterInt>("FREQ","Freq",200,18000,10));
    
    //highpass
    params.push_back(std::make_unique<AudioParameterInt>("FREQ2","Freq2",400,16000,10));
    
    params.push_back(std::make_unique<AudioParameterFloat>("Q","Q",0.1,10,0.1));
    
    return {params.begin() , params.end()};
}

//==============================================================================
const juce::String GramophoneAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GramophoneAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GramophoneAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GramophoneAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GramophoneAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GramophoneAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GramophoneAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GramophoneAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GramophoneAudioProcessor::getProgramName (int index)
{
    return {};
}

void GramophoneAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GramophoneAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    transportSource.prepareToPlay(samplesPerBlock, sampleRate);
    maxiSample.load("/Users/jerrybennett/JuceTestPlugins/Gramophone/Source/crackle.wav");
}

void GramophoneAudioProcessor::releaseResources()
{
    transportSource.releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GramophoneAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void GramophoneAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //block for playing audio
    
    //transportSource.getNextAudioBlock(AudioSourceChannelInfo(buffer));
    
    freqVal = 6000;
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
    
    //double w = maxiSample.play();
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        for (int n = 0; n < buffer.getNumSamples() ; n++){
            float x = buffer.getReadPointer(channel)[n];
            //transportSource.getNextAudioBlock(AudioSourceChannelInfo(buffer));
            float z = biquad2.processSample(x, channel);
            float y = biquad.processSample(z, channel);
            float w = biquad.processSample(maxiSample.play(), channel);
            
            buffer.getWritePointer(channel)[n] = y + w;
        }
        //buffer.getWritePointer(channel)[channel] = maxiSample.play();
    }
}

//==============================================================================
bool GramophoneAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GramophoneAudioProcessor::createEditor()
{
    return new GramophoneAudioProcessorEditor (*this);
}

//==============================================================================
void GramophoneAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    XmlElement xml("plugin-settings");

    xml.setAttribute("audiofile", currentlyLoadedFile.getFullPathName());

    copyXmlToBinary(xml, destData);
}

void GramophoneAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName("plugin-settings"))
        {
            currentlyLoadedFile = File::createFileWithoutCheckingPath(xmlState->getStringAttribute("audiofile"));
            if (currentlyLoadedFile.existsAsFile())
            {
                loadFileIntoTransport(currentlyLoadedFile);
            }
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GramophoneAudioProcessor();
}

void GramophoneAudioProcessor::loadFileIntoTransport(const File& audioFile)
{
    // unload the previous file source and delete it..
    transportSource.stop();
    transportSource.setSource(nullptr);
    currentAudioFileSource = nullptr;

    AudioFormatReader* reader = formatManager.createReaderFor(audioFile);
    currentlyLoadedFile = audioFile;

    if (reader != nullptr)
    {
        currentAudioFileSource = new AudioFormatReaderSource(reader, true);

        // ..and plug it into our transport source
        transportSource.setSource(
            currentAudioFileSource,
            32768,                   // tells it to buffer this many samples ahead
            &readAheadThread,        // this is the background thread to use for reading-ahead
            reader->sampleRate);     // allows for sample rate correction
    }
}
