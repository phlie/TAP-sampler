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

    mAttackSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mAttackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mAttackSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::darkslateblue);
    mAttackSlider.setRange(0.0f, 5.0f, 0.01f);
    mAttackSlider.addListener(this);
    addAndMakeVisible(mAttackSlider);

    mAttackLabel.setFont(10.0f);
    mAttackLabel.setText("Attack", juce::dontSendNotification);
    mAttackLabel.setJustificationType(juce::Justification::centredTop);
    mAttackLabel.attachToComponent(&mAttackSlider, false);
    mAttackLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::springgreen);
    addAndMakeVisible(mAttackLabel);

    mDecaySlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mDecaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mDecaySlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::darkslateblue);
    mDecaySlider.setRange(0.0f, 5.0f, 0.01f);
    mDecaySlider.addListener(this);
    addAndMakeVisible(mDecaySlider);

    mDecayLabel.setFont(10.0f);
    mDecayLabel.setText("Decay", juce::dontSendNotification);
    mDecayLabel.setJustificationType(juce::Justification::centredTop);
    mDecayLabel.attachToComponent(&mDecaySlider, false);
    mDecayLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::springgreen);
    addAndMakeVisible(mDecayLabel);

    mSustainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mSustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mSustainSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::darkslateblue);
    mSustainSlider.setRange(0.0f, 1.0f, 0.01f);
    mSustainSlider.addListener(this);
    addAndMakeVisible(mSustainSlider);

    mSustainLabel.setFont(10.0f);
    mSustainLabel.setText("Sustain", juce::dontSendNotification);
    mSustainLabel.setJustificationType(juce::Justification::centredTop);
    mSustainLabel.attachToComponent(&mSustainSlider, false);
    mSustainLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::springgreen);
    addAndMakeVisible(mSustainLabel);

    mReleaseSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mReleaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mReleaseSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::darkslateblue);
    mReleaseSlider.setRange(0.0f, 5.0f, 0.01f);
    mReleaseSlider.addListener(this);
    addAndMakeVisible(mReleaseSlider);

    mReleaseLabel.setFont(10.0f);
    mReleaseLabel.setText("Release", juce::dontSendNotification);
    mReleaseLabel.setJustificationType(juce::Justification::centredTop);
    mReleaseLabel.attachToComponent(&mReleaseSlider, false);
    mReleaseLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::springgreen);
    addAndMakeVisible(mReleaseLabel);
    
    setSize (600, 200);
}

TAPsamplerAudioProcessorEditor::~TAPsamplerAudioProcessorEditor()
{
}

juce::Path TAPsamplerAudioProcessorEditor::drawPath()
{
    juce::Path p;
    //mAudioPoints.clear();

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
    
        g.strokePath(drawPath(), juce::PathStrokeType(1));
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
    const auto startX = 0.6f;
    const auto startY = 0.6f;
    const auto dialWidth = 0.1f;
    const auto dialHeight = 0.4f;

    mAttackSlider.setBoundsRelative(startX, startY, dialWidth, dialHeight);
    mDecaySlider.setBoundsRelative(startX + dialWidth, startY, dialWidth, dialHeight);
    mSustainSlider.setBoundsRelative(startX + 2*dialWidth, startY, dialWidth, dialHeight);
    mReleaseSlider.setBoundsRelative(startX + 3*dialWidth, startY, dialWidth, dialHeight);
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

void TAPsamplerAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &mAttackSlider)
    {
        audioProcessor.attack = mAttackSlider.getValue();
    }
    else if (slider == &mDecaySlider)
    {
        audioProcessor.release = mReleaseSlider.getValue();
    }
    else if (slider == &mSustainSlider)
    {
        audioProcessor.sustain = mSustainSlider.getValue();
    }
    else if (slider == &mReleaseSlider)
    {
        audioProcessor.release = mReleaseSlider.getValue();
    }
}