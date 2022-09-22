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
// It needs to inherit from ValueTree::Listener because that is how we know the values contained in the ValueTreeState have changed.
class TAPsamplerAudioProcessor  : public juce::AudioProcessor,
                                  public juce::ValueTree::Listener
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

    // The two functions responsible for normal opening of files, and drag and drop.
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

    // Contains a reference to the Value Tree State
    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; };

    // Returns atomic values and is simply used to get a reference to the internally held atomic data.
    std::atomic<bool>& isNotePlayed() { return mIsNotePlayed; };
    std::atomic<int>& getSampleCount() { return mSampleCount; };
private:
    // Create a Synthesiser class
    juce::Synthesiser mSampler;

    // Declare the maximum number of voices
    const int mNumVoices{ 3 };
    // Declare an AudioBuffer to hold the wave form
    juce::AudioBuffer<float> mWaveForm;

    // Holds all the ADSR parameters
    juce::ADSR::Parameters mADSRparams;

    // The Format Manager responsible for file types and the reader, initially set to nullptr.
    juce::AudioFormatManager mFormatManager;
    juce::AudioFormatReader* mFormatReader = nullptr;

    // The value tree state and its parameters
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    // A Value Tree and State also makes it so the variables are able to be automated in a DAW
    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;

    // An atomic variable is a thread safe manner of passing variables or data between two different processes.
    std::atomic<bool> mShouldUpdate{ false };

    // The std::atomic is a much heavier data type that should only be used when the variable is being accessed by two threads.
    std::atomic<bool> mIsNotePlayed{ false };

    // Sample count simply counts the number of samples since the note was on.
    std::atomic<int> mSampleCount{ 0 };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TAPsamplerAudioProcessor)
};
