/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class TAPsamplerAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                        public juce::FileDragAndDropTarget,
                                        public juce::Slider::Listener
{
public:
    TAPsamplerAudioProcessorEditor (TAPsamplerAudioProcessor&);
    ~TAPsamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    // Used to indicate that files are being drag and dropped
    bool isInterestedInFileDrag(const juce::StringArray& files) override;

    // When the files are dropped, actually get their file names
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    // Whenever a slider value has changed call the proper parameters in the audio processor class
    void sliderValueChanged(juce::Slider* slider) override;

private:
    // The button for loading a file using the browser.
    juce::TextButton mLoadButton{ "Load" };

    // A vector containing the right amount of audio points to draw the waveform
    std::vector<float> mAudioPoints;

    // If true it starts the painting operation
    bool mShouldBePainting{ false };

    // Returns a path object and is used to draw the waveform.
    juce::Path drawPath();

    // The sliders and their corresponding labels.
    juce::Slider mAttackSlider, mDecaySlider, mSustainSlider, mReleaseSlider;
    juce::Label mAttackLabel, mDecayLabel, mSustainLabel, mReleaseLabel;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TAPsamplerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TAPsamplerAudioProcessorEditor)
};
