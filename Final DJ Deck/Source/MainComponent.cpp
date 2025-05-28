/*
  ==============================================================================

    MainComponent.cpp
    Created: 23 Feb 2025 9:27:46pm
    Author:  ugesh

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Set the main window size
    setSize (1280, 720);

    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
         && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
            [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        setAudioChannels (0, 2);
    }

    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);
    
    deckGUI1.setDeckNumber(1);
    deckGUI2.setDeckNumber(2);

    formatManager.registerBasicFormats();

    // Connect the playlist's track selection callback.
    // Loads the selected track into the chosen deck.
    playlistComponent.trackSelectionCallback = [this](const URL& trackURL, int deckNumber)
    {
        DBG("Loading track " << trackURL.toString(true) << " to deck " << deckNumber);
        if (deckNumber == 1)
            deckGUI1.loadTrack(trackURL);
        else if (deckNumber == 2)
            deckGUI2.loadTrack(trackURL);
    };
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
    g.setColour(Colours::white);
    g.setFont(14.0f);
    g.drawText("PlaylistComponent", getLocalBounds(), Justification::centred, true);
}

void MainComponent::resized()
{
    int deckHeight = getHeight() * 3 / 4;
    
    // Decks occupy the top 3/4 of the window.
    deckGUI1.setBounds(0, 0, getWidth() / 2, deckHeight);
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, deckHeight);
    
    // Playlist occupies the bottom 1/4.
    playlistComponent.setBounds(0, deckHeight, getWidth(), getHeight() - deckHeight);
}
