/*
  ==============================================================================

    WaveThumbnail.cpp
    Created: 22 Sep 2022 7:57:03am
    Author:  phlie

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveThumbnail.h"

//==============================================================================
// Needs to set the processor in its initialization list.
WaveThumbnail::WaveThumbnail(TAPsamplerAudioProcessor& p) : processor(p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

WaveThumbnail::~WaveThumbnail()
{
}

void WaveThumbnail::paint (juce::Graphics& g)
{
    // Makes for a nice look for the waveform
    g.fillAll(juce::Colours::blanchedalmond);

    // Get the wavefrom from the back end and calculate its ratio and finally get a read buffer to it.
    auto waveform = processor.getWaveForm();

    if (processor.getWaveForm().getNumSamples() > 0)
    {
        // Create a Path
        juce::Path p;
        //mAudioPoints.clear();
        //mAudioPoints.clear();

        // New File dropped?
        //If yes
            //Get the waveform from the processor
            //Sample = 44100 (1 sec)... x axis of window = 600, 44100/600 = 73.5 increments per pixel
            //Values of audio file are between -1 to 1 but our window is 200 big so we need to scale it up. 200 to 0.
            //Use the ratio to take values from the audio buffer and put it in vector to display.
            //Draw the waveform
        // The ratio between the number of samples in the audio file and the WaveThumbnail Component's width.
        const float ratio = waveform.getNumSamples() / getWidth();
        
        // Create a read pointer to waveform
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
        // Black makes for an easy to see waveform
        g.setColour(juce::Colours::black);
        g.strokePath(p, juce::PathStrokeType(2));
        // Don't call this part of code again until a new file has been uploaded.
        mShouldBePainting = false;

        // The text should stand out from the background and waveform
        g.setColour(juce::Colours::darkslategrey);
        g.setFont(25.f);
        // Reduce the bounds of the text area by 10 on each side.
        auto textBounds = getLocalBounds().reduced(10, 10);
        // Set the text to within textBounds and to the top right.
        g.drawFittedText(mFileName, textBounds, juce::Justification::topRight, 1);
    }
    else
    {
        // This section is for before an audio file has been loaded in
        g.setColour(juce::Colours::black);
        g.setFont(50.0f);
        g.drawFittedText("Drop an Audio file to load...", getLocalBounds(), juce::Justification::centred, 1);
    }
    
}

bool WaveThumbnail::isInterestedInFileDrag(const juce::StringArray& files)
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

void WaveThumbnail::filesDropped(const juce::StringArray& files, int x, int y)
{
    // For each file that has been dropped
    for (auto file : files)
    {
        // Check if the file is of the appropriate type
        if (isInterestedInFileDrag(juce::StringArray{ file }))
        {
            auto myFile = std::make_unique<juce::File>(file);
            // Load the file into the sampler
            processor.loadFile(file);

            mFileName = myFile->getFileNameWithoutExtension();

            repaint();
            
            
            // Draw the waveform on the GUI.
            //mShouldBePainting = true;
        }
    }


}

void WaveThumbnail::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
