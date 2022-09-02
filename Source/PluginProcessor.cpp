/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginMain.h"
#include "doomgeneric_vst.h"
#include "doomkeys.h"
#include <queue>

std::queue<keyEvent_t> keyEventQueue;

extern "C"
{
    #include "m_argv.h"
    #include "d_iwad.h"

    void D_DoomMain(void);
    void D_DoomLoop_Start(void);
    void D_DoomLoop_SingleFrame(void);
    void dg_Create();

    /*enum GameMission_t;
    char* D_FindIWAD(int mask, GameMission_t* mission);*/
}

uint32_t getElapsedMillis() {
    return juce::Time::getMillisecondCounter();
}

int getNextKeyEvent(keyEvent_t* event) {
    if (!keyEventQueue.empty()) {
        *event = keyEventQueue.front();
        keyEventQueue.pop();
        return 1;
    }
    return 0;
}

//==============================================================================
DoomVstAudioProcessor::DoomVstAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
    , wadFound(false)
#endif
{
    myargc = 0;
    myargv = nullptr;

    GameMission_t mission = (GameMission_t)0;
    if (D_FindIWAD(IWAD_MASK_DOOM, &mission) == NULL) {
        wadFound = false;
        return;
    }
    else {
        wadFound = true;
    }

    dg_Create();
    D_DoomMain();
    D_DoomLoop_Start();
}

DoomVstAudioProcessor::~DoomVstAudioProcessor()
{
}

//==============================================================================
const juce::String DoomVstAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DoomVstAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DoomVstAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DoomVstAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DoomVstAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DoomVstAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DoomVstAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DoomVstAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DoomVstAudioProcessor::getProgramName (int index)
{
    return {};
}

void DoomVstAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DoomVstAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void DoomVstAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DoomVstAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DoomVstAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    int i = 0;
    auto nextMidiMessage = midiMessages.findNextSamplePosition(0);
    while (nextMidiMessage != midiMessages.cend()) {
        auto msg = (*nextMidiMessage).getMessage();
        processMidiNote(msg);
        nextMidiMessage = midiMessages.findNextSamplePosition((*nextMidiMessage).samplePosition + 1);
    }
}

void DoomVstAudioProcessor::processMidiNote(juce::MidiMessage& msg)
{
    if (!msg.isNoteOnOrOff())
        return;

    int note = msg.getNoteNumber();
    int isOn = msg.isNoteOn();
    switch (note) {
    case 48: // C3
        keyEventQueue.push(keyEvent_t{ isOn, KEY_LEFTARROW });
        break;
    case 50: // D3
        keyEventQueue.push(keyEvent_t{ isOn, KEY_UPARROW });
        break;
    case 52: // E3
        keyEventQueue.push(keyEvent_t{ isOn, KEY_RIGHTARROW });
        break;
    case 53: // F3
        keyEventQueue.push(keyEvent_t{ isOn, KEY_DOWNARROW });
        break;

    case 60: // C4
        keyEventQueue.push(keyEvent_t{ isOn, KEY_ENTER });
        break;
    case 62: // D4
        keyEventQueue.push(keyEvent_t{ isOn, KEY_FIRE });
        break;
    case 64: // E4
        keyEventQueue.push(keyEvent_t{ isOn, KEY_USE });
        break;
    case 65: // F4
        keyEventQueue.push(keyEvent_t{ isOn, KEY_RSHIFT });
        break;
    }
}

//==============================================================================
bool DoomVstAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DoomVstAudioProcessor::createEditor()
{
    return new DoomVstAudioProcessorEditor (*this);
}

//==============================================================================
void DoomVstAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DoomVstAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DoomVstAudioProcessor();
}
