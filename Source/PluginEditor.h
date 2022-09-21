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
                                        public juce::FileDragAndDropTarget
{
public:
    TAPsamplerAudioProcessorEditor (TAPsamplerAudioProcessor&);
    ~TAPsamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;


    void filesDropped(const juce::StringArray& files, int x, int y) override;

    juce::Path drawPath();

private:
    juce::TextButton mLoadButton{ "Load" };
    std::vector<float> mAudioPoints;
    bool mShouldBePainting{ false };

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TAPsamplerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TAPsamplerAudioProcessorEditor)
};
