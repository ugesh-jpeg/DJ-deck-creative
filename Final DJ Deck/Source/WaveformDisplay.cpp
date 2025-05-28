/*
  ==============================================================================
 
    WaveformDisplay.cpp
    Created: 23 Feb 2025 9:27:46pm
    Author:  ugesh
 
  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager & formatManagerToUse,
                                 AudioThumbnailCache & cacheToUse)
    : audioThumb(1000, formatManagerToUse, cacheToUse),
      fileLoaded(false),
      position(0)
{
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::setGradientColors(Colour startColour, Colour endColour)
{
    gradientStartColour = startColour;
    gradientEndColour = endColour;
    repaint();
}

void WaveformDisplay::paint(Graphics& g)
{
    ColourGradient grad(gradientStartColour, 0, 0, gradientEndColour, getWidth(), getHeight(), false);
    g.setGradientFill(grad);
    g.fillAll();

    // Draw an outline with rounded corners.
    g.setColour(Colours::grey);
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 10.0f, 2.0f);

    if (fileLoaded)
    {
        // Draw the waveform using the configurable waveformColour.
        g.setColour(waveformColour);
        audioThumb.drawChannel(g,
                               getLocalBounds(),
                               0,
                               audioThumb.getTotalLength(),
                               0,
                               1.0f);

        g.setColour(waveformColour);
        int playheadX = static_cast<int>(position * getWidth());
        g.fillRect(playheadX - 1, 0, 3, getHeight());
    }
    else
    {
        g.setColour(Colours::lightgrey);
        g.setFont(20.0f);
        g.drawText("No File Loaded", getLocalBounds(), Justification::centred, true);
    }
}

void WaveformDisplay::setWaveformColour(Colour newColour)
{
    waveformColour = newColour;
    repaint();
}


void WaveformDisplay::resized()
{
    // Layout code if additional child components are added in the future.
}

void WaveformDisplay::loadURL(URL audioURL)
{
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));
    if (fileLoaded)
    {
        std::cout << "WaveformDisplay: loaded!" << std::endl;
        repaint();
    }
    else
    {
        std::cout << "WaveformDisplay: not loaded!" << std::endl;
    }
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source)
{
    std::cout << "WaveformDisplay: change received!" << std::endl;
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position)
    {
        position = pos;
        repaint();
    }
}
