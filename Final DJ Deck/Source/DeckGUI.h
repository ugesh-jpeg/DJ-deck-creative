/*
  ==============================================================================

    DeckGUI.h
    Created: 23 Feb 2025 9:27:46pm
    Author:  ugesh

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

class DeckGUI    : public Component,
                   public Button::Listener,
                   public Slider::Listener,
                   public FileDragAndDropTarget,
                   public Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
            AudioFormatManager& formatManagerToUse,
            AudioThumbnailCache& cacheToUse);
    ~DeckGUI();

    void paint (Graphics&) override;
    void resized() override;

    void buttonClicked (Button*) override;
    void sliderValueChanged (Slider* slider) override;
    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override;
    void timerCallback() override;
    
    void loadTrack(const URL& trackURL);
    
    // New setter to specify which deck this instance represents.
    void setDeckNumber(int newDeckNumber);

private:
    juce::FileChooser fChooser{"Select a file..."};
    TextButton playButton{"PLAY"};
    TextButton pauseButton{"PAUSE"};
    TextButton restartButton{"RESTART"};
    TextButton loopButton{"LOOP"};
    TextButton loadButton{"LOAD"};
    
    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;
    
    // NEW: 5-second jump buttons
    TextButton rewind5Button{"<< 5s"};
    TextButton forward5Button{">> 5s"};

    WaveformDisplay waveformDisplay;
    DJAudioPlayer* player;

    Label nowPlayingLabel;
    Label timeLabel;
    Label forwardLabel;
    Label rewindLabel;

    // NEW: Volume, speed and position labels
    Label volumeLabel;
    Label speedLabel;
    Label positionLabel;

    bool isLoopOn { false };

    // NEW: Deck number to display in the title.
    int deckNumber { 0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
