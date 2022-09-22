/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// The editor gets a reference to the Audio Processor class and passes it down to itself, the WaveThumbnail component
// and the ADSR component as well as assigning p to its own internal audioProcessor.
TAPsamplerAudioProcessorEditor::TAPsamplerAudioProcessorEditor (TAPsamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), mWaveThumbnail(p), mADSR(p), audioProcessor (p)
{
    // Temporarily disabled but used to load a file instead of drag and drop
    //mLoadButton.onClick = [&]() { audioProcessor.loadFile(); };
    //addAndMakeVisible(mLoadButton);
    
    // Adds the almost music logo to the variable which is stored in BinaryData. It needs the size which is also available.
    auto almostMusicLogo = juce::ImageCache::getFromMemory(BinaryData::almostmusiclogo_png, BinaryData::almostmusiclogo_pngSize);
    
    // If the image has been loaded from memory
    if (!almostMusicLogo.isNull())
    {
        // Set the image and set it to stretch to fill its bounds
        mImageComponent.setImage(almostMusicLogo, juce::RectanglePlacement::stretchToFit);
    }
    else
        // If the image is not loaded quit because of error.
        jassert(!almostMusicLogo.isNull());

    // Adds the two front end components to the Editor component
    addAndMakeVisible(mWaveThumbnail);
    addAndMakeVisible(mADSR);

    // An image component is pretty much the same as any other component.
    addAndMakeVisible(mImageComponent);

    // Call the timer callback 30 times a second.
    startTimerHz(30);

    // The size can be anything and the drawing of the waveform while automatically update.
    setSize (1200, 600);
}

// Disable the timer when the editor window is not in use.
TAPsamplerAudioProcessorEditor::~TAPsamplerAudioProcessorEditor()
{
    stopTimer();
}

//juce::Path TAPsamplerAudioProcessorEditor::drawPath()
//{
//    // Create a Path
//    juce::Path p;
//    //mAudioPoints.clear();
//
//    // New File dropped?
//    //If yes
//        //Get the waveform from the processor
//        //Sample = 44100 (1 sec)... x axis of window = 600, 44100/600 = 73.5 increments per pixel
//        //Values of audio file are between -1 to 1 but our window is 200 big so we need to scale it up. 200 to 0.
//        //Use the ratio to take values from the audio buffer and put it in vector to display.
//        //Draw the waveform
//
//    // Get the wavefrom from the back end and calculate its ratio and finally get a read buffer to it.
//    auto waveform = audioProcessor.getWaveForm();
//    const float ratio = waveform.getNumSamples() / getWidth();
//    auto buffer = waveform.getReadPointer(0);
//
//    // Scale audio file along the x axis to conform with the window
//    for (float sample = 0; sample < waveform.getNumSamples(); sample += ratio)
//    {
//        // Add the correct amount of audio points to fit the screen in the x direction
//        mAudioPoints.push_back(buffer[(int)sample]);
//    }
//
//    // Start the path at the left middle
//    p.startNewSubPath(0, getHeight() / 2);
//
//    // Scale on Y-Axis
//    for (int sample = 0; sample < mAudioPoints.size(); ++sample)
//    {
//        // Automatically map the sample's Y position to the desired position between 200 (negative -1.0) and 0 (positive 1.0)
//        auto point = juce::jmap<float>(mAudioPoints[sample], -1.0, 1.0f, getHeight(), 0);
//
//        // Draw a line for each point
//        p.lineTo(sample, point);
//    }
//
//    return p;
//}

//==============================================================================
void TAPsamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);
    // Set the path color as white
    //g.setColour(juce::Colours::white);

    
    /*g.setFont(15.0f);
    g.setColour(juce::Colours::white);

    if (audioProcessor.getNumSamplerSounds() > 0)
    {
        g.fillAll(juce::Colours::red);

        g.drawText("Sound Loaded!", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, juce::Justification::centred);
    }
    else
    {
        g.drawText("Load a sound...", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, juce::Justification::centred);
    } */
    // If we have a sound
        // "Sound loaded".
    // Else
        // "Load a sound".
}

void TAPsamplerAudioProcessorEditor::resized()
{
    // Now all that is needed is to set the bounds of each component.
    mWaveThumbnail.setBoundsRelative(0.0f, 0.25f, 1.0f, 0.5f);
    mADSR.setBoundsRelative(0.0, 0.75f, 1.0f, 0.25f);

    // Set the relative bounds of the image component.
    mImageComponent.setBoundsRelative(0.0f, 0.0f, 1.0f, 0.25f);

    //mLoadButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);
}

void TAPsamplerAudioProcessorEditor::timerCallback()
{
    // Every time the timer is called, repaint the entire interface, it could run faster if it only updated the required components.
    repaint();
}


/*
void TAPsamplerAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    // If the slider is an mAttackSlider
    if (slider == &mAttackSlider)
    {
        // Update the audio processors ADSR attack value by getting the value of the attack slider with .getValue()
        audioProcessor.getADSRParams().attack = mAttackSlider.getValue();
    }
    else if (slider == &mDecaySlider)
    {
        audioProcessor.getADSRParams().release = mReleaseSlider.getValue();
    }
    else if (slider == &mSustainSlider)
    {
        audioProcessor.getADSRParams().sustain = mSustainSlider.getValue();
    }
    else if (slider == &mReleaseSlider)
    {
        audioProcessor.getADSRParams().release = mReleaseSlider.getValue();
    }

    // Since a Slider value has changed, update all the slider values in the backend ADSR
    audioProcessor.updateADSRValue();
} */