/*
  ==============================================================================
 
    PlaylistComponent.cpp
    Created: 23 Feb 2025 9:27:46pm
    Author:  ugesh
 
  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    // For demonstration, we create dummy track titles.
    trackTitles.push_back("Track 1");
    trackTitles.push_back("Track 2");
    trackTitles.push_back("Track 3");
    trackTitles.push_back("Track 4");
    trackTitles.push_back("Track 5");
    trackTitles.push_back("Track 6");
    trackTitles.push_back("Track 7");
    trackTitles.push_back("Track 8");
    trackTitles.push_back("Track 9");
    trackTitles.push_back("Track 10");

    // Initialize trackDurations with empty strings.
    trackDurations.resize(trackTitles.size());
    
    // Configure header columns:
    tableComponent.getHeader().addColumn("Track title", 1, 450, 450, 450);
    tableComponent.getHeader().addColumn("Duration", 2, 10, 100, 500);
    tableComponent.getHeader().addColumn("Load", 3, 200, 200, 200);
    tableComponent.getHeader().addColumn("Import", 4, 200, 200, 200);
        
    // Activate stretch-to-fit so that extra space goes to "Track title".
    tableComponent.getHeader().setStretchToFitActive(true);

    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(14.0f));
    g.drawText("PlaylistComponent", getLocalBounds(), juce::Justification::centred, true);
}

void PlaylistComponent::resized()
{
    tableComponent.setBounds(0, 0, getWidth(), getHeight());
}

int PlaylistComponent::getNumRows()
{
    return static_cast<int>(trackTitles.size());
}

void PlaylistComponent::paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colours::orange);
    else
        g.fillAll(juce::Colours::limegreen);
}

void PlaylistComponent::paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    if (columnId == 1)
    {
        // Draw the track title.
        g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
    else if (columnId == 2)
    {
        // Draw the duration.
        if (rowNumber < static_cast<int>(trackDurations.size()))
            g.drawText(trackDurations[rowNumber], 2, 0, width - 4, height, juce::Justification::left, true);
    }
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate)
{
    if (columnId == 3) // "Load" column
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton("Load");
            btn->addListener(this);
            btn->setComponentID(String(rowNumber));
            // Set the load button colors: white background with black text.
            btn->setColour(TextButton::buttonColourId, Colours::white);
            btn->setColour(TextButton::textColourOffId, Colours::black);
            existingComponentToUpdate = btn;
        }
    }
    else if (columnId == 4) // "Import" column
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton("Import");
            btn->addListener(this);
            // Set a unique ID by prefixing with "import_".
            btn->setComponentID("import_" + String(rowNumber));
            // Set the import button colors: black background with white text.
            btn->setColour(TextButton::buttonColourId, Colours::black);
            btn->setColour(TextButton::textColourOffId, Colours::white);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button * button)
{
    juce::String compID = button->getComponentID();
    
    if (compID.startsWith("import_"))
    {
        // "Import" button was clicked.
        int row = compID.fromFirstOccurrenceOf("_", false, false).getIntValue();
        
        // Use the member FileChooser to launch the file dialog asynchronously.
        fChooser.launchAsync(juce::FileBrowserComponent::canSelectFiles,
            [this, row](const FileChooser& chooser)
            {
                File selectedFile = chooser.getResult();
                DBG("Import chosen file: " << selectedFile.getFullPathName());
                if (selectedFile.exists())
                {
                    // Update the track title.
                    trackTitles[row] = selectedFile.getFileName().toStdString();

                    // Use AudioFormatManager to get the actual song duration.
                    AudioFormatManager formatManager;
                    formatManager.registerBasicFormats();
                    std::unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(selectedFile));
                    if (reader != nullptr)
                    {
                        double durationSeconds = reader->lengthInSamples / reader->sampleRate;
                        int minutes = static_cast<int>(durationSeconds) / 60;
                        int seconds = static_cast<int>(durationSeconds) % 60;
                        char durationStr[16];
                        snprintf(durationStr, sizeof(durationStr), "%d:%02d", minutes, seconds);
                        trackDurations[row] = durationStr;
                    }
                    else
                    {
                        trackDurations[row] = "";
                    }
                    
                    // Update the URL for this row.
                    if (row < static_cast<int>(trackURLs.size()))
                        trackURLs[row] = URL(selectedFile);
                    else
                        trackURLs.push_back(URL(selectedFile));
                    
                    tableComponent.updateContent();
                    DBG("Imported track: " << selectedFile.getFileName());
                }
                else
                {
                    DBG("No valid file selected.");
                }
            });
    }
    else // "Load" button was clicked.
    {
        int row = button->getComponentID().getIntValue();
        if (row < static_cast<int>(trackTitles.size()))
        {
            // Define a custom component for deck selection with a DJ look.
            class DeckSelectionComponent : public Component, public Button::Listener
            {
            public:
                DeckSelectionComponent(std::function<void(int)> callbackFunction)
                    : callback(callbackFunction)
                {
                    addAndMakeVisible(deck1Button);
                    addAndMakeVisible(deck2Button);
                    
                    deck1Button.setButtonText("Deck 1");
                    deck2Button.setButtonText("Deck 2");
                    
                    deck1Button.setColour(TextButton::buttonColourId, Colours::red);
                    deck1Button.setColour(TextButton::textColourOffId, Colours::white);
                    deck2Button.setColour(TextButton::buttonColourId, Colours::blue);
                    deck2Button.setColour(TextButton::textColourOffId, Colours::white);
                    
                    deck1Button.addListener(this);
                    deck2Button.addListener(this);
                }
                
                void resized() override
                {
                    auto area = getLocalBounds().reduced(10);
                    // Reserve a region below the title for the buttons.
                    auto buttonArea = area.removeFromTop(60);
                    int gap = 10;
                    int buttonWidth = (buttonArea.getWidth() - gap) / 2;
                    int buttonHeight = 40;
                    int yOffset = buttonArea.getHeight() - 10;
                    
                    deck1Button.setBounds(buttonArea.removeFromLeft(buttonWidth).withTrimmedTop(yOffset).withHeight(buttonHeight));
                    buttonArea.removeFromLeft(gap);
                    deck2Button.setBounds(buttonArea.withTrimmedTop(yOffset).withHeight(buttonHeight));
                }
                
                void paint(Graphics& g) override
                {
                    // Draw a red-to-black gradient background.
                    ColourGradient gradient(Colours::green, 0, 0, Colours::black, getWidth(), getHeight(), false);
                    g.setGradientFill(gradient);
                    g.fillAll();

                    auto titleArea = getLocalBounds().removeFromTop(40);
                    g.setColour(Colours::white);
                    g.setFont(Font("Arial", 18.0f, Font::FontStyleFlags::bold));
                    g.drawText("Which deck would you like to use?", titleArea, Justification::centred);
                }
                
                void buttonClicked(Button* btn) override
                {
                    int chosenDeck = (btn == &deck1Button) ? 1 : 2;
                    if (callback)
                        callback(chosenDeck);
                    if (auto* dw = findParentComponentOfClass<DialogWindow>())
                        dw->exitModalState(chosenDeck);
                }
                
            private:
                TextButton deck1Button, deck2Button;
                std::function<void(int)> callback;
            };

            // Create a DialogWindow for deck selection.
            auto* deckSelector = new DeckSelectionComponent([this, row](int deckNumber)
            {
                if (trackSelectionCallback)
                    trackSelectionCallback(trackURLs[row], deckNumber);
            });
            
            DialogWindow::LaunchOptions options;
            options.content.setOwned(deckSelector);
            options.dialogTitle = "Choose Deck";
            options.dialogBackgroundColour = Colours::black;
            options.escapeKeyTriggersCloseButton = true;
            options.useNativeTitleBar = false;
            options.content->setSize(300, 150);
            options.launchAsync();
        }
    }
}
