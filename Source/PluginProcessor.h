/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class TAPsamplerAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    TAPsamplerAudioProcessor();
    ~TAPsamplerAudioProcessor() override;

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

    void loadFile();
    void loadFile(const juce::String& path);

    // Returns the total number of sounds attached to the synthesiser
    int getNumSamplerSounds() { return mSampler.getNumSounds(); };

    // Used to return a reference to the mWaveForm for use in drawing its diagram
    juce::AudioBuffer<float>& getWaveForm() { return mWaveForm; };

    // Called to update all the ADSR values
    void updateADSRValue();

    // Used to pass the ADSR Params to the Editor
    juce::ADSR::Parameters& getADSRParams() { return mADSRparams; };

    // The values for the ADSR to be inserted
    float attack{ 0.0f };
    float decay{ 0.0f };
    float sustain{ 0.0f };
    float release{ 0.0f };

private:
    // Create a Synthesiser class
    juce::Synthesiser mSampler;

    // Declare the maximum number of voices
    const int mNumVoices{ 3 };
    juce::AudioBuffer<float> mWaveForm;

    // Holds all the ADSR parameters
    juce::ADSR::Parameters mADSRparams;

    // The Format Manager responsible for file types and the reader, initially set to nullptr.
    juce::AudioFormatManager mFormatManager;
    juce::AudioFormatReader* mFormatReader = nullptr;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TAPsamplerAudioProcessor)
};
