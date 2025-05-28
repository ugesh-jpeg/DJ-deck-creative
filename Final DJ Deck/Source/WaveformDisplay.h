/*
  ==============================================================================

    WaveformDisplay.h
    Created: 23 Feb 2025 9:27:46pm
    Author:  ugesh

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class WaveformDisplay    : public Component, 
                           public ChangeListener
{
public:
    WaveformDisplay( AudioFormatManager & 	formatManagerToUse,
                    AudioThumbnailCache & 	cacheToUse );
    ~WaveformDisplay();

    void paint (Graphics&) override;
    void resized() override;

    void changeListenerCallback (ChangeBroadcaster *source) override;

    void loadURL(URL audioURL);

    /** set the relative position of the playhead*/
    void setPositionRelative(double pos);
    
    void setGradientColors(Colour startColour, Colour endColour);
    void setWaveformColour(Colour newColour);

private:
    AudioThumbnail audioThumb;
    bool fileLoaded; 
    double position;
    
    Colour gradientStartColour { Colours::darkblue };
    Colour gradientEndColour { Colours::black };
    Colour waveformColour { Colours::red };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
