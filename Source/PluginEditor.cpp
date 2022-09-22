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
    // Temporarily disabled but used to load a file instead of drag and drop
    //mLoadButton.onClick = [&]() { audioProcessor.loadFile(); };
    //addAndMakeVisible(mLoadButton);

    // All the properties of the attack slider that we want set up.
    mAttackSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mAttackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mAttackSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::darkslateblue);
    //mAttackSlider.setRange(0.0f, 5.0f, 0.01f);
    // Attach the listener to the mAttackSlider.
    //mAttackSlider.addListener(this);
    // Finally Make it visible and add it
    addAndMakeVisible(mAttackSlider);

    // Set the properties of the label
    mAttackLabel.setFont(18.0f);
    mAttackLabel.setText("Attack", juce::dontSendNotification);
    mAttackLabel.setJustificationType(juce::Justification::centredTop);
    mAttackLabel.attachToComponent(&mAttackSlider, false);
    mAttackLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::springgreen  );
    addAndMakeVisible(mAttackLabel);

    mAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "ATTACK", mAttackSlider);

    mDecaySlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mDecaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mDecaySlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::darkslateblue);
    //mDecaySlider.setRange(0.0f, 5.0f, 0.01f);
    //mDecaySlider.addListener(this);
    addAndMakeVisible(mDecaySlider);

    mDecayLabel.setFont(18.0f);
    mDecayLabel.setText("Decay", juce::dontSendNotification);
    mDecayLabel.setJustificationType(juce::Justification::centredTop);
    mDecayLabel.attachToComponent(&mDecaySlider, false);
    mDecayLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::springgreen);
    addAndMakeVisible(mDecayLabel);

    mDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "DECAY", mDecaySlider);

    mSustainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mSustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mSustainSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::darkslateblue);
    //mSustainSlider.setRange(0.0f, 1.0f, 0.01f);
    //mSustainSlider.addListener(this);
    addAndMakeVisible(mSustainSlider);

    mSustainLabel.setFont(18.0f);
    mSustainLabel.setText("Sustain", juce::dontSendNotification);
    mSustainLabel.setJustificationType(juce::Justification::centredTop);
    mSustainLabel.attachToComponent(&mSustainSlider, false);
    mSustainLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::springgreen);
    addAndMakeVisible(mSustainLabel);

    mSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "SUSTAIN", mSustainSlider);

    mReleaseSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mReleaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mReleaseSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::darkslateblue);
    //mReleaseSlider.setRange(0.0f, 5.0f, 0.01f);
    //mReleaseSlider.addListener(this);
    addAndMakeVisible(mReleaseSlider);

    mReleaseLabel.setFont(18.0f);
    mReleaseLabel.setText("Release", juce::dontSendNotification);
    mReleaseLabel.setJustificationType(juce::Justification::centredTop);
    mReleaseLabel.attachToComponent(&mReleaseSlider, false);
    mReleaseLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::springgreen);
    addAndMakeVisible(mReleaseLabel);
    
    mReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "RELEASE", mReleaseSlider);

    // The size can be anything and the drawing of the waveform while automatically update.
    setSize (1200, 600);
}

TAPsamplerAudioProcessorEditor::~TAPsamplerAudioProcessorEditor()
{
}

juce::Path TAPsamplerAudioProcessorEditor::drawPath()
{
    // Create a Path
    juce::Path p;
    //mAudioPoints.clear();

    // New File dropped?
    //If yes
        //Get the waveform from the processor
        //Sample = 44100 (1 sec)... x axis of window = 600, 44100/600 = 73.5 increments per pixel
        //Values of audio file are between -1 to 1 but our window is 200 big so we need to scale it up. 200 to 0.
        //Use the ratio to take values from the audio buffer and put it in vector to display.
        //Draw the waveform

    // Get the wavefrom from the back end and calculate its ratio and finally get a read buffer to it.
    auto waveform = audioProcessor.getWaveForm();
    const float ratio = waveform.getNumSamples() / getWidth();
    auto buffer = waveform.getReadPointer(0);

    // Scale audio file along the x axis to conform with the window
    for (float sample = 0; sample < waveform.getNumSamples(); sample += ratio)
    {
        // Add the correct amount of audio points to fit the screen in the x direction
        mAudioPoints.push_back(buffer[(int)sample]);
    }

    // Start the path at the left middle
    p.startNewSubPath(0, getHeight() / 2);

    // Scale on Y-Axis
    for (int sample = 0; sample < mAudioPoints.size(); ++sample)
    {
        // Automatically map the sample's Y position to the desired position between 200 (negative -1.0) and 0 (positive 1.0)
        auto point = juce::jmap<float>(mAudioPoints[sample], -1.0, 1.0f, getHeight(), 0);

        // Draw a line for each point
        p.lineTo(sample, point);
    }

    return p;
}

//==============================================================================
void TAPsamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);
    // Set the path color as white
    g.setColour(juce::Colours::white);

    // If a new file has been updated
    if (mShouldBePainting)
    {
        // Stroke the path with the path generated from drawPath() and a Stroke Type of 1
        g.strokePath(drawPath(), juce::PathStrokeType(1));
        // Don't call this part of code again until a new file has been uploaded.
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
    // The constants for setting up the GUI Positions of the Sliders and Labels
    const auto startX = 0.6f;
    const auto startY = 0.6f;
    const auto dialWidth = 0.1f;
    const auto dialHeight = 0.4f;

    // Set Bounds Relative simply uses percentages of the screen to decide where and how big the elements while be.
    mAttackSlider.setBoundsRelative(startX, startY, dialWidth, dialHeight);
    mDecaySlider.setBoundsRelative(startX + dialWidth, startY, dialWidth, dialHeight);
    mSustainSlider.setBoundsRelative(startX + 2*dialWidth, startY, dialWidth, dialHeight);
    mReleaseSlider.setBoundsRelative(startX + 3*dialWidth, startY, dialWidth, dialHeight);
    //mLoadButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);
}

bool TAPsamplerAudioProcessorEditor::isInterestedInFileDrag(const juce::StringArray& files)
{
    // Loop through all the files
    for (auto file : files)
    {
        // If any contain a recognized music format return true.
        if (file.contains(".wav") || file.contains(".mp3") || file.contains(".aif"))
        {
            // Return true since it is a music file.
            return true;
        }
    }
    // No music files were found, so return false.
    return false;
}

void TAPsamplerAudioProcessorEditor::filesDropped(const juce::StringArray& files, int x, int y)
{
    // For each file that has been dropped
    for (auto file : files)
    {
        // Check if the file is of the appropriate type
        if (isInterestedInFileDrag(juce::StringArray{ file }))
        {
            // Load the file into the sampler
            audioProcessor.loadFile(file);
            // Draw the waveform on the GUI.
            mShouldBePainting = true;
        }
    }

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