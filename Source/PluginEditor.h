/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DoomVstAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    DoomVstAudioProcessorEditor (DoomVstAudioProcessor&);
    ~DoomVstAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DoomVstAudioProcessor& audioProcessor;
    juce::DrawableImage frameBuffer;
    juce::Label errorMessage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DoomVstAudioProcessorEditor)
};
