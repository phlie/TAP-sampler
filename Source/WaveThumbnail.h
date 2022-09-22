/*
  ==============================================================================

    WaveThumbnail.h
    Created: 22 Sep 2022 7:57:03am
    Author:  phlie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class WaveThumbnail  : public juce::Component,
                       public juce::FileDragAndDropTarget
{
public:
    // The constructor needs a reference to the Audio Processor
    WaveThumbnail(TAPsamplerAudioProcessor& p);
    ~WaveThumbnail() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    // Used to indicate that files are being drag and dropped
    bool isInterestedInFileDrag(const juce::StringArray& files) override;

    // When the files are dropped, actually get their file names
    void filesDropped(const juce::StringArray& files, int x, int y) override;

private:
    // A vector containing the right amount of audio points to draw the waveform
    std::vector<float> mAudioPoints;

    // If true it starts the painting operation
    bool mShouldBePainting{ false };

    // Used to store the file name.
    juce::String mFileName{ "" };

    // A reference to the Audio Processor
    TAPsamplerAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveThumbnail)
};
