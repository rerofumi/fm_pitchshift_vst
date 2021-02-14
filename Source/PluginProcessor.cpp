/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Fm_pitchshift_vstAudioProcessor::Fm_pitchshift_vstAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	)
#endif
	, spectrum(juce::Image::RGB, 512, 256, true)
	, myFilter(PitchFilter::fftSize, PitchFilter::fftSize * 3)
	, if_samplerate(48000.0)
	, if_samplesPerBlock(512)
{
}

Fm_pitchshift_vstAudioProcessor::~Fm_pitchshift_vstAudioProcessor()
{
}

//==============================================================================
const juce::String Fm_pitchshift_vstAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool Fm_pitchshift_vstAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool Fm_pitchshift_vstAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool Fm_pitchshift_vstAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double Fm_pitchshift_vstAudioProcessor::getTailLengthSeconds() const
{
	int buffer_chunk = ((if_samplesPerBlock - 1) + FFTFilter::fftSize) / if_samplesPerBlock;
	return (double)buffer_chunk / if_samplerate;
}

int Fm_pitchshift_vstAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int Fm_pitchshift_vstAudioProcessor::getCurrentProgram()
{
	return 0;
}

void Fm_pitchshift_vstAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String Fm_pitchshift_vstAudioProcessor::getProgramName(int index)
{
	return {};
}

void Fm_pitchshift_vstAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void Fm_pitchshift_vstAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
	if_samplerate = sampleRate;
	if_samplesPerBlock = samplesPerBlock;
}

void Fm_pitchshift_vstAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Fm_pitchshift_vstAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
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

void Fm_pitchshift_vstAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	/* --- my processing code*/
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	auto data_size = buffer.getNumSamples();
	float* channel_1 = buffer.getWritePointer(0);
	float* channel_2 = buffer.getWritePointer(1);
	// do work -----
	myFilter.process(channel_1, channel_2, data_size);
	myFilter.output(channel_1, channel_2, data_size);
	// -----
}

//==============================================================================
bool Fm_pitchshift_vstAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Fm_pitchshift_vstAudioProcessor::createEditor()
{
	return new Fm_pitchshift_vstAudioProcessorEditor(*this);
}

//==============================================================================
void Fm_pitchshift_vstAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void Fm_pitchshift_vstAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new Fm_pitchshift_vstAudioProcessor();
}

//==============================================================================
// add my methods
//==============================================================================
/*---
* change pitch shift table
*/
void Fm_pitchshift_vstAudioProcessor::setPitchShift(float pitch)
{
	myFilter.setPitchShift(pitch, (float)if_samplerate);
}

/* ---
* rendering spectrum view
*/
juce::Image& Fm_pitchshift_vstAudioProcessor::getSpectrumView()
{
	if (myFilter.getProcessFlag()) {
		// input level
		auto level = myFilter.getSpectrum();
		spectrum.clear(juce::Rectangle<int>(512, 256), juce::Colour(0, 0, 0));
		drawSpectrumGraph(spectrum, level, juce::Colour(0, 200, 0));
		//
		myFilter.setProcessFlag(false);
	}
	return spectrum;
}

/* --- draw graph */
void Fm_pitchshift_vstAudioProcessor::drawSpectrumGraph(juce::Image view, std::shared_ptr<float> power, juce::Colour color)
{
	int post_point = 0;
	float post_level = 0.0f;
	juce::Graphics g(view);

	for (int x = 1; x < 512; x++) {
		auto skewedProportionX = 1.0f - std::exp(std::log(1.0f - (float)x / 512.0f) * 0.2f);
		auto fftDataIndex = juce::jlimit(1, (int)PitchFilter::spectrumNum, (int)(skewedProportionX * (float)PitchFilter::spectrumNum));
		auto lv = power.get()[fftDataIndex];
		if (std::fabs(post_level - lv) > 0.0001) {
			g.setColour(color);
			g.setOpacity(1.0);
			g.drawLine(
				(float)post_point,
				juce::jmap(post_level, 0.0f, 1.0f, 256.0f, 0.0f),
				(float)x,
				juce::jmap(lv, 0.0f, 1.0f, 256.0f, 0.0f)
			);
			{
				g.setOpacity(0.5);
				juce::Path pen;
				pen.startNewSubPath(juce::Point<float>((float)post_point, juce::jmap(post_level, 0.0f, 1.0f, 256.0f, 0.0f)));
				pen.lineTo(juce::Point<float>((float)x, juce::jmap(lv, 0.0f, 1.0f, 256.0f, 0.0f)));
				pen.lineTo(juce::Point<float>((float)x, 256.0f));
				pen.lineTo(juce::Point<float>((float)post_point, 256.0f));
				pen.closeSubPath();
				g.fillPath(pen);
			}
			post_point = x;
			post_level = lv;
		}
	}
	g.setOpacity(1.0);
}

