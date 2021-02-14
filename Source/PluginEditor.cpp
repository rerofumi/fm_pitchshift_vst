/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Fm_pitchshift_vstAudioProcessorEditor::Fm_pitchshift_vstAudioProcessorEditor (Fm_pitchshift_vstAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(536, 340);
    addAndMakeVisible(gui_component);
    startTimerHz(10);
    gui_component.setSize(this->getWidth(), this->getHeight());
    gui_component.setProcesser(p);
}

Fm_pitchshift_vstAudioProcessorEditor::~Fm_pitchshift_vstAudioProcessorEditor()
{
}

//==============================================================================
void Fm_pitchshift_vstAudioProcessorEditor::paint (juce::Graphics& g)
{
}

void Fm_pitchshift_vstAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void Fm_pitchshift_vstAudioProcessorEditor::timerCallback()
{
    repaint();
}