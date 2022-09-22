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
                        // In the intialization list, the apvts gets passed in.
                       ), apvts(*this, nullptr, "PARAMETERS", createParameters())
#endif
{
    // Register the basic formats allowed for sampled sounds
    mFormatManager.registerBasicFormats();

    // Adds this processor as a listener for changes in the apvts value tree.
    apvts.state.addListener(this);

    // Loop through the max number of voices and add a voice to synthesiser class mSampler for each one.
    for (int i = 0; i < mNumVoices; i++)
    {
        mSampler.addVoice(new juce::SamplerVoice());
    }
}

TAPsamplerAudioProcessor::~TAPsamplerAudioProcessor()
{
    // In case something happens, mFormatReader needs to not have a memory leak.
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
    // Sets the current playback sample rate
    mSampler.setCurrentPlaybackSampleRate(sampleRate);

    // Do an inital update of the ADSR values
    updateADSRValue();

    /*for (int i = 0; i < mSampler.getNumSounds(); ++i)
    {
        // Dynamically cast the Synthesiser class current sound to try and cast it to SampleSound*, although it needs a pointer which is what get() does.
        // This is necessary because it only works on SamplerSounds not synthesiser sounds
        if (auto sound = dynamic_cast<juce::SamplerSound*>(mSampler.getSound(i).get()))
        {
            // Set the ADSR Envelope for the current paramters.
            sound->setEnvelopeParameters(mADSRparams);
        }
    }*/
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
    // Get the input and output channels
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear the buffer at the start for now
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Only when a change is detected...
    if (mShouldUpdate)
    {
        // ... actually update the ADSR values.
        updateADSRValue();
    }

    // Call the sampler's render next block
    mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool TAPsamplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TAPsamplerAudioProcessor::createEditor()
{
    // When the Editor is created it needs a reference to this processor for its constructor
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

// This is used when the load button is clicked.
void TAPsamplerAudioProcessor::loadFile()
{
    // Clear the current sounds that the sampler may have
    mSampler.clearSounds();

    // Allow the user to choose a file.
    juce::FileChooser chooser{ "Please load a file." };
    
    // For this to work, "JUCE_MODAL_LOOPS_PERMITTED=1" needs to be in the JUCE Settings Preprocessor Definitons
    // Although there is a more modern way to do this using an async method.
    if (chooser.browseForFileToOpen())
    {
        // Get the result of the user choosing the file
        auto file = chooser.getResult();
        // Create a reader for the file
        mFormatReader = mFormatManager.createReaderFor(file);
    }

    // This creates a BigInteger holding the range of keys on a MIDI keyboard, at most 0 to 128
    juce::BigInteger range;
    range.setRange(0, 128, true);

    // Finally add the sound to the synthesiser class using the default juce Sampler Sound class
    mSampler.addSound(new juce::SamplerSound("Sample", *mFormatReader, range, 60, 0.1f, 0.1f, 20.f));
    //updateADSRValue();
}

// This is used for drag and drop
void TAPsamplerAudioProcessor::loadFile(const juce::String& path)
{
    // Clear the current sounds
    mSampler.clearSounds();

    // Get the path from the drag and dropped file
    auto file = juce::File(path);

    // Create a Format Reader for the file
    mFormatReader = mFormatManager.createReaderFor(file);

    //auto sampleLength = static_cast<int>(mFormatReader->lengthInSamples);

    // The sampeLength is set to 1 second of data to display in the waveform editor.
    auto sampleLength = 44100;

    // This is necessary because mWaveForm is a vector and it needs a size equal or bigger to the max data storage
    // Remember that the first int is for the number of new channels, which here is 1
    mWaveForm.setSize(1, sampleLength);
    // Transfer the data to mWaveForm up to sampleLength
    mFormatReader->read(&mWaveForm, 0, sampleLength, 0, true, false);

    //auto buffer = mWaveForm.getReadPointer(0);

    /*for (int sample = 0; sample < mWaveForm.getNumSamples(); ++sample)
    {
        // DBG(buffer[sample]); Used for debugging.
    }*/

    // Create the range of MIDI notes
    juce::BigInteger range;
    range.setRange(0, 128, true);

    // Add the sound using the default Sampler Sound class with the reader, range, normalized key, etc.
    mSampler.addSound(new juce::SamplerSound("Sample", *mFormatReader, range, 60, 0.1f, 0.1f, 20.f));
}

void TAPsamplerAudioProcessor::updateADSRValue()
{
    /*mADSRparams.attack = attack;
    mADSRparams.decay = decay;
    mADSRparams.sustain = sustain;
    mADSRparams.release = release;
    */

    // All the ADSR params must get the value of the knobs and since they are atomic, they need to use load()
    mADSRparams.attack = apvts.getRawParameterValue("ATTACK")->load();
    mADSRparams.decay = apvts.getRawParameterValue("DECAY")->load();
    mADSRparams.sustain = apvts.getRawParameterValue("SUSTAIN")->load();
    mADSRparams.release = apvts.getRawParameterValue("RELEASE")->load();


    // Loop through the total number of sounds
    for (int i = 0; i < mSampler.getNumSounds(); ++i)
    {
        // Dynamically cast the Synthesiser class current sound to try and cast it to SampleSound*, although it needs a pointer which is what get() does.
        // This is necessary because it only works on SamplerSounds not synthesiser sounds.
        // We wouldn't be able to call setEnvelopeParameters without it
        if (auto sound = dynamic_cast<juce::SamplerSound*>(mSampler.getSound(i).get()))
        {
            // Set the ADSR Envelope for the current paramters.
            sound->setEnvelopeParameters(mADSRparams);
        }
    }

}

juce::AudioProcessorValueTreeState::ParameterLayout TAPsamplerAudioProcessor::createParameters()
{
    // Create a place to store the Parameter Layout
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

    // Add each of the float parameters.
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", 0.0f, 5.0f, 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", 0.0, 5.0f, 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", 0.0f, 1.0f, 0.9f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 0.0f, 5.0f, 0.0));

    // Return the vector starting with the first param, and ending with the last.
    return { parameters.begin(), parameters.end() };
}

// When the Value Tree has a property changed, this gets called.
void TAPsamplerAudioProcessor::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    // Notifying the render function that it should update the ADSR values.
    mShouldUpdate = true;
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TAPsamplerAudioProcessor();
}
