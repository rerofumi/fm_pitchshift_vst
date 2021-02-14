/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PluginView.h"

//==============================================================================
/**
*/
class Fm_pitchshift_vstAudioProcessorEditor  :
    public juce::AudioProcessorEditor,
    private juce::Timer
{
public:
    Fm_pitchshift_vstAudioProcessorEditor (Fm_pitchshift_vstAudioProcessor&);
    ~Fm_pitchshift_vstAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Fm_pitchshift_vstAudioProcessor& audioProcessor;

    PluginView gui_component;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Fm_pitchshift_vstAudioProcessorEditor)
};
