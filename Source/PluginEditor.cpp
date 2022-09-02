/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "doomgeneric.h"
#include "doomgeneric_vst.h"

extern "C" {
    void D_DoomLoop_SingleFrame(void);
}

//==============================================================================
DoomVstAudioProcessorEditor::DoomVstAudioProcessorEditor (DoomVstAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    if (audioProcessor.wadFound) {
        Timer::startTimerHz(FRAMES_PER_SEC);
        addAndMakeVisible(frameBuffer);
    }
    else {
        juce::String message = juce::String("Could not find a Doom IWAD file in the current directory. Copy doom.wad or doom2.wad into the directory:\r\n\r\n") + juce::File::getCurrentWorkingDirectory().getFullPathName() + "\r\n\r\nand restart the plugin.";
        errorMessage.setText(message, juce::NotificationType::dontSendNotification);
        addAndMakeVisible(errorMessage);
    }

    setSize (DOOMGENERIC_RESX, DOOMGENERIC_RESY);
}

DoomVstAudioProcessorEditor::~DoomVstAudioProcessorEditor()
{
}

//==============================================================================
void DoomVstAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    if (!audioProcessor.wadFound)
        return;

    D_DoomLoop_SingleFrame();
    juce::Image image(juce::Image::PixelFormat::ARGB, DOOMGENERIC_RESX, DOOMGENERIC_RESY, false);
    for (int y = 0; y < DOOMGENERIC_RESY; y++) {
        for (int x = 0; x < DOOMGENERIC_RESX; x++) {
            uint32_t pixel = mainFrameBuffer[y * DOOMGENERIC_RESX + x] | 0xFF000000UL;

            image.setPixelAt(x, y, juce::Colour(pixel));
        }
    }
    frameBuffer.setImage(image);
}

void DoomVstAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    if (audioProcessor.wadFound) {
        frameBuffer.setBounds(0, 0, DOOMGENERIC_RESX, DOOMGENERIC_RESY);
    }
    else {
        errorMessage.setBounds(0, 0, DOOMGENERIC_RESX, DOOMGENERIC_RESY);
    }

    
}

void DoomVstAudioProcessorEditor::timerCallback()
{
    repaint();
}
