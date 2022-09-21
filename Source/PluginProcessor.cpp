/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TAPsamplerAudioProcessor::TAPsamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    mFormatManager.registerBasicFormats();

    for (int i = 0; i < mNumVoices; i++)
    {
        mSampler.addVoice(new juce::SamplerVoice());
    }
}

TAPsamplerAudioProcessor::~TAPsamplerAudioProcessor()
{
    mFormatReader = nullptr;
}

//==============================================================================
const juce::String TAPsamplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TAPsamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TAPsamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TAPsamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TAPsamplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TAPsamplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TAPsamplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TAPsamplerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TAPsamplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void TAPsamplerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TAPsamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSampler.setCurrentPlaybackSampleRate(sampleRate);
}

void TAPsamplerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TAPsamplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void TAPsamplerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool TAPsamplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TAPsamplerAudioProcessor::createEditor()
{
    return new TAPsamplerAudioProcessorEditor (*this);
}

//==============================================================================
void TAPsamplerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TAPsamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void TAPsamplerAudioProcessor::loadFile()
{
    mSampler.clearSounds();

    juce::FileChooser chooser{ "Please load a file." };
    
    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        mFormatReader = mFormatManager.createReaderFor(file);


    }

    juce::BigInteger range;
    range.setRange(0, 128, true);

    mSampler.addSound(new juce::SamplerSound("Sample", *mFormatReader, range, 60, 0.1f, 0.1f, 20.f));
}

void TAPsamplerAudioProcessor::loadFile(const juce::String& path)
{
    mSampler.clearSounds();

    auto file = juce::File(path);
    mFormatReader = mFormatManager.createReaderFor(file);

    //auto sampleLength = static_cast<int>(mFormatReader->lengthInSamples);
    auto sampleLength = 44100;
    mWaveForm.setSize(1, sampleLength);
    mFormatReader->read(&mWaveForm, 0, sampleLength, 0, true, false);

    //auto buffer = mWaveForm.getReadPointer(0);

    /*for (int sample = 0; sample < mWaveForm.getNumSamples(); ++sample)
    {
        // DBG(buffer[sample]); Used for debugging.
    }*/

    juce::BigInteger range;
    range.setRange(0, 128, true);

    mSampler.addSound(new juce::SamplerSound("Sample", *mFormatReader, range, 60, 0.1f, 0.1f, 20.f));
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TAPsamplerAudioProcessor();
}
