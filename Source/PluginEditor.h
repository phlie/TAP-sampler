/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "WaveThumbnail.h"
#include "ADSRComponent.h"

//==============================================================================
/**
*/
class TAPsamplerAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                        public juce::Timer  // Timer is used to create a Timer Callback function.
                                        //public juce::Slider::Listener     // Used to create a listener fro knob turns
{
public:
    // Needs to be passed the Audio Processor reference when it is constructed.
    TAPsamplerAudioProcessorEditor (TAPsamplerAudioProcessor&);
    ~TAPsamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    // This comes from the Timer abstract class and has to be overriden with our own code for when the timer is called.
    void timerCallback() override;

    // Whenever a slider value has changed call the proper parameters in the audio processor class
    //void sliderValueChanged(juce::Slider* slider) override;

private:
    // The button for loading a file using the browser.
    //juce::TextButton mLoadButton{ "Load" };

    // The two visua component classes and their objects.
    WaveThumbnail mWaveThumbnail;
    ADSRComponent mADSR;

    // Used to hold the image component, in our case the Almost Music Logo
    juce::ImageComponent mImageComponent;

    // Returns a path object and is used to draw the waveform.
    //juce::Path drawPath();

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TAPsamplerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TAPsamplerAudioProcessorEditor)
};
