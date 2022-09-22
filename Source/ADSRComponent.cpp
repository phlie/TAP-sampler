/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 22 Sep 2022 8:47:23am
    Author:  phlie

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ADSRComponent.h"

//==============================================================================
// Sets the passed audio processor argument, p, to this classes Audio Processor reference, processor.
ADSRComponent::ADSRComponent(TAPsamplerAudioProcessor& p) : processor(p)
{
    // Setting up all the sliders look and feel
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
    mAttackLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::springgreen);
    addAndMakeVisible(mAttackLabel);

    // Using the Audio Processor Value Tree State : Slider Attachment to connect the slider to the back end.
    mAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.getAPVTS(), "ATTACK", mAttackSlider);

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

    mDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.getAPVTS(), "DECAY", mDecaySlider);

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

    mSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.getAPVTS(), "SUSTAIN", mSustainSlider);

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

    mReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.getAPVTS(), "RELEASE", mReleaseSlider);

}

ADSRComponent::~ADSRComponent()
{
}

void ADSRComponent::paint (juce::Graphics& g)
{
    // All that is need here is to choose a background color.
    g.fillAll (juce::Colours::crimson);   // clear the background
}

void ADSRComponent::resized()
{
    // The constants for setting up the GUI Positions of the Sliders and Labels
    const auto startX = 0.6f;
    const auto startY = 0.2f;
    const auto dialWidth = 0.1f;
    const auto dialHeight = 0.75f;

    // Set Bounds Relative simply uses percentages of the screen to decide where and how big the elements while be.
    mAttackSlider.setBoundsRelative(startX, startY, dialWidth, dialHeight);
    mDecaySlider.setBoundsRelative(startX + dialWidth, startY, dialWidth, dialHeight);
    mSustainSlider.setBoundsRelative(startX + 2 * dialWidth, startY, dialWidth, dialHeight);
    mReleaseSlider.setBoundsRelative(startX + 3 * dialWidth, startY, dialWidth, dialHeight);
}
