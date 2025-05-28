/*
  ==============================================================================
 
    DeckGUI.cpp
    Created: 23 Feb 2025 9:27:46pm
    Author:  ugesh
 
  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 AudioFormatManager& formatManagerToUse,
                 AudioThumbnailCache& cacheToUse)
    : waveformDisplay(formatManagerToUse, cacheToUse), player(_player)
{
    // Component visibility
    addAndMakeVisible(playButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(restartButton);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(rewind5Button);
    addAndMakeVisible(forward5Button);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(nowPlayingLabel);
    addAndMakeVisible(timeLabel);
    addAndMakeVisible(forwardLabel);
    addAndMakeVisible(rewindLabel);
    addAndMakeVisible(volumeLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(positionLabel);

    // Attach listeners
    playButton.addListener(this);
    pauseButton.addListener(this);
    restartButton.addListener(this);
    loopButton.addListener(this);
    loadButton.addListener(this);
    rewind5Button.addListener(this);
    forward5Button.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    // Configure sliders
    volSlider.setRange(0, 100, 1);
    volSlider.setValue(50);
    speedSlider.setRange(0.5, 2.0, 0.5);
    speedSlider.setValue(1.0);
    speedSlider.setNumDecimalPlacesToDisplay(2);
    posSlider.setRange(0, 100, 1);

    // Configure labels...
    nowPlayingLabel.setText("Now playing: None", dontSendNotification);
    nowPlayingLabel.setJustificationType(Justification::centredLeft);
    nowPlayingLabel.setColour(Label::textColourId, Colours::white);
    nowPlayingLabel.setFont(Font("Arial", 16.0f, Font::bold));

    timeLabel.setText("0:00 / 0:00", dontSendNotification);
    timeLabel.setJustificationType(Justification::centredLeft);
    timeLabel.setColour(Label::textColourId, Colours::white);
    timeLabel.setFont(Font("Arial", 16.0f, Font::bold));
    
    forwardLabel.setText("Fast Forward", dontSendNotification);
    forwardLabel.setJustificationType(Justification::centred);
    forwardLabel.setColour(Label::textColourId, Colours::white);
    forwardLabel.setFont(Font("Arial", 16.0f, Font::bold));
    
    rewindLabel.setText("Rewind", dontSendNotification);
    rewindLabel.setJustificationType(Justification::centred);
    rewindLabel.setColour(Label::textColourId, Colours::white);
    rewindLabel.setFont(Font("Arial", 16.0f, Font::bold));

    volumeLabel.setText("Volume", dontSendNotification);
    volumeLabel.setJustificationType(Justification::centred);
    volumeLabel.setColour(Label::textColourId, Colours::white);
    volumeLabel.setFont(Font("Arial", 16.0f, Font::bold));

    speedLabel.setText("Speed", dontSendNotification);
    speedLabel.setJustificationType(Justification::centred);
    speedLabel.setColour(Label::textColourId, Colours::white);
    speedLabel.setFont(Font("Arial", 16.0f, Font::bold));

    positionLabel.setText("Position", dontSendNotification);
    positionLabel.setJustificationType(Justification::centred);
    positionLabel.setColour(Label::textColourId, Colours::white);
    positionLabel.setFont(Font("Arial", 16.0f, Font::bold));
    
    // Set color of the buttons
    playButton.setColour(TextButton::buttonColourId, Colours::darkorange);
    pauseButton.setColour(TextButton::buttonColourId, Colours::purple);
    restartButton.setColour(TextButton::buttonColourId, Colours::darkblue);
    rewind5Button.setColour(TextButton::buttonColourId, Colours::red);
    forward5Button.setColour(TextButton::buttonColourId, Colours::red);
    loadButton.setColour(TextButton::buttonColourId, Colours::darkgreen);

    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    ColourGradient gradient;
    if (deckNumber == 1)
    {
        // Deck 1: white-to-black gradient
        gradient = ColourGradient(Colours::grey, 0, 0, Colours::black, getWidth(), getHeight(), false);
    }
    else if (deckNumber == 2)
    {
        // Deck 2: black-to-white gradient
        gradient = ColourGradient(Colours::black, 0, 0, Colours::grey, getWidth(), getHeight(), false);
    }
    
    g.setGradientFill(gradient);
    g.fillAll();

    // Draw the deck title
    g.setColour(Colours::white);
    g.setFont(Font("Arial", 16.0f, Font::bold));

    String title;
    if (deckNumber == 1)
        title = "Deck 1";
    else if (deckNumber == 2)
        title = "Deck 2";

    g.drawFittedText(title, getLocalBounds().reduced(10), Justification::topRight, 1);
}

void DeckGUI::resized()
{
    int totalRows = 15;
    int rowH = getHeight() / totalRows;
    int width = getWidth();
    int row = 0; // row index starting at 0

    // Row 1: Now Playing label
    nowPlayingLabel.setBounds(0, row * rowH, width - 80, rowH);
    nowPlayingLabel.setMinimumHorizontalScale(0.5f);
    row++;

    // Row 2: Time label
    timeLabel.setBounds(0, row * rowH, width, rowH);
    row++;

    // Rows 3-5: Waveform display
    waveformDisplay.setBounds(0, row * rowH, width, rowH * 3);
    row += 3;

    // Row 6: Volume label
    volumeLabel.setBounds(0, row * rowH, width, rowH);
    row++;

    // Row 7: Volume slider
    volSlider.setBounds(0, row * rowH, width, rowH);
    row++;

    // Row 8: Speed label
    speedLabel.setBounds(0, row * rowH, width, rowH);
    row++;

    // Row 9: Speed slider
    speedSlider.setBounds(0, row * rowH, width, rowH);
    row++;

    // Row 10: Position label
    positionLabel.setBounds(0, row * rowH, width, rowH);
    row++;

    // Row 11: Position slider
    posSlider.setBounds(0, row * rowH, width, rowH);
    row++;

    // Row 12: Control buttons (play, pause, restart, loop)
    int quarterWidth = width / 4;
    playButton.setBounds(0, row * rowH, quarterWidth, rowH);
    pauseButton.setBounds(quarterWidth, row * rowH, quarterWidth, rowH);
    restartButton.setBounds(2 * quarterWidth, row * rowH, quarterWidth, rowH);
    loopButton.setBounds(3 * quarterWidth, row * rowH, quarterWidth, rowH);
    row++;

    // Row 13: Rewind and Forward labels
    int halfWidth = width / 2;
    rewindLabel.setBounds(0, row * rowH, halfWidth, rowH);
    forwardLabel.setBounds(halfWidth, row * rowH, halfWidth, rowH);
    row++;

    // Row 14: Rewind and Forward buttons
    rewind5Button.setBounds(0, row * rowH, halfWidth, rowH);
    forward5Button.setBounds(halfWidth, row * rowH, halfWidth, rowH);
    row++;

    // Row 15: Load button
    loadButton.setBounds(0, row * rowH, width, rowH);
}

void DeckGUI::buttonClicked (Button* button)
{
    if (button == &playButton)
    {
        DBG("Play button was clicked");
        player->start();
    }
    else if (button == &pauseButton)
    {
        DBG("Pause button was clicked");
        player->stop();
    }
    else if (button == &restartButton)
    {
        DBG("Restart button was clicked");
        player->setPosition(0);
        player->start();
    }
    else if (button == &loopButton)
    {
        isLoopOn = !isLoopOn;
        if (isLoopOn)
        {
            loopButton.setColour(TextButton::buttonColourId, Colours::green);
            DBG("Loop turned ON");
        }
        else
        {
            loopButton.setColour(TextButton::buttonColourId, Colours::red);
            DBG("Loop turned OFF");
        }
    }
    else if (button == &loadButton)
    {
        auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            File chosenFile = chooser.getResult();
            if (chosenFile.exists())
            {
                player->loadURL(URL{chooser.getResult()});
                waveformDisplay.loadURL(URL{chooser.getResult()});
                nowPlayingLabel.setText("Now playing: " + chosenFile.getFileName(), dontSendNotification);
            }
        });
    }
    // Rewind 5s
    else if (button == &rewind5Button)
    {
        double currentPos = player->getPositionRelative() * player->getTotalLength();
        double newPos = currentPos - 5.0;
        if (newPos < 0.0)
            newPos = 0.0;
        player->setPosition(newPos);
        DBG("Rewind 5 seconds");
    }
    // Forward 5s
    else if (button == &forward5Button)
    {
        double currentPos = player->getPositionRelative() * player->getTotalLength();
        double totalLen = player->getTotalLength();
        double newPos = currentPos + 5.0;
        if (newPos > totalLen)
            newPos = totalLen;
        player->setPosition(newPos);
        DBG("Forward 5 seconds");
    }
}

void DeckGUI::sliderValueChanged (Slider* slider)
{
    if (slider == &volSlider)
    {
        player->setGain(volSlider.getValue() / 100.0);
    }
    else if (slider == &speedSlider)
    {
        player->setSpeed(speedSlider.getValue());
    }
    else if (slider == &posSlider)
    {
        player->setPositionRelative(posSlider.getValue() / 100.0);
    }

}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
    std::cout << "DeckGUI::filesDropped" << std::endl;
    if (files.size() == 1)
    {
        player->loadURL(URL{File{files[0]}});
    }
}

void DeckGUI::timerCallback()
{
    double posRel = player->getPositionRelative();
    waveformDisplay.setPositionRelative(posRel);
    if (!posSlider.isMouseButtonDown())
        posSlider.setValue(posRel * 100, dontSendNotification);
    
    if (isLoopOn && posRel >= 0.99)
    {
        player->setPosition(0);
        player->start();
    }
    
    // Update the time label if total length is available.
    double totalSeconds = player->getTotalLength();
    double currentSeconds = posRel * totalSeconds;
    int currentMin = static_cast<int>(currentSeconds) / 60;
    int currentSec = static_cast<int>(currentSeconds) % 60;
    int totalMin = static_cast<int>(totalSeconds) / 60;
    int totalSec = static_cast<int>(totalSeconds) % 60;
    String timeString = String::formatted("%d:%02d / %d:%02d", currentMin, currentSec, totalMin, totalSec);
    timeLabel.setText(timeString, dontSendNotification);
}


void DeckGUI::loadTrack(const URL& trackURL)
{
    player->loadURL(trackURL);
    waveformDisplay.loadURL(trackURL);
    nowPlayingLabel.setText("Now Playing: " + trackURL.getLocalFile().getFileName(), dontSendNotification);

}

void DeckGUI::setDeckNumber(int newDeckNumber)
{
    deckNumber = newDeckNumber;
    if (deckNumber == 1)
    {
        waveformDisplay.setGradientColors(Colours::darkblue, Colours::black);
        waveformDisplay.setWaveformColour(Colours::red);
    }
    else
    {
        waveformDisplay.setGradientColors(Colours::black, Colours::darkred);
        waveformDisplay.setWaveformColour(Colours::blue);
    }
}
