/*
  ==============================================================================

    PlaylistComponent.h
    Created: 23 Feb 2025 9:27:46pm
    Author:  ugesh

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include <functional>

class PlaylistComponent  : public juce::Component,
                           public TableListBoxModel,
                           public Button::Listener
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    int getNumRows() override;
    void paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    
    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;
    void buttonClicked(Button * button) override;
    
    // The callback receives the track URL and the chosen deck number (1 or 2).
    std::function<void (const URL& trackURL, int deckNumber)> trackSelectionCallback;
    
private:
    TableListBox tableComponent;
    std::vector<std::string> trackTitles;
    std::vector<std::string> trackDurations;
    std::vector<URL> trackURLs;
    
    // Add a FileChooser member so the import works like the load function.
    juce::FileChooser fChooser { "Select an audio file...", juce::File(), "*.mp3;*.wav" };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
