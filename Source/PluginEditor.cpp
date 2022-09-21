/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TAPsamplerAudioProcessorEditor::TAPsamplerAudioProcessorEditor (TAPsamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    //mLoadButton.onClick = [&]() { audioProcessor.loadFile(); };
    //addAndMakeVisible(mLoadButton);
    setSize (300, 200);
}

TAPsamplerAudioProcessorEditor::~TAPsamplerAudioProcessorEditor()
{
}

juce::Path TAPsamplerAudioProcessorEditor::drawPath()
{
    juce::Path p;
    mAudioPoints.clear();

    // New File dropped?
    //If yes
        //Get the waveform from the processor
        //Sample = 44100 (1 sec)... x axis of window = 600, 44100/600 = 73.5 increments per pixel
        //Values of audio file are between -1 to 1 but our window is 200 big so we need to scale it up. 200 to 0.
        //Use the ratio to take values from the audio buffer and put it in vector to display.
        //Draw the waveform

    auto waveform = audioProcessor.getWaveForm();
    auto ratio = waveform.getNumSamples() / getWidth();
    auto buffer = waveform.getReadPointer(0);

    // Scale audio file along the x axis to conform with the window
    for (float sample = 0; sample < waveform.getNumSamples(); sample += ratio)
    {
        mAudioPoints.push_back(buffer[(int)sample]);
    }

    p.startNewSubPath(0, getHeight() / 2);

    // Scale on Y-Axis
    for (int sample = 0; sample < mAudioPoints.size(); ++sample)
    {
        auto point = juce::jmap<float>(mAudioPoints[sample], -1.0, 1.0f, 200, 0);

        p.lineTo(sample, point);
    }

    return p;


}

//==============================================================================
void TAPsamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);
    g.setColour(juce::Colours::white);
    


    if (mShouldBePainting)
    {
    
        g.strokePath(drawPath(), juce::PathStrokeType(2));
        mShouldBePainting = false;
    }
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
    //mLoadButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);
}

bool TAPsamplerAudioProcessorEditor::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (auto file : files)
    {
        if (file.contains(".wav") || file.contains(".mp3") || file.contains(".aif"))
        {
            return true;
        }
    }
    return false;
}

void TAPsamplerAudioProcessorEditor::filesDropped(const juce::StringArray& files, int x, int y)
{
    for (auto file : files)
    {
        if (isInterestedInFileDrag(juce::StringArray{ file }))
        {
            audioProcessor.loadFile(file);
            mShouldBePainting = true;
        }
    }

    repaint();

}
