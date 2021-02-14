#pragma once
#pragma once
#include "FFTFilter.h"
#include <JuceHeader.h>

class PitchFilter :
    public FFTFilter
{
public:
	PitchFilter(int segment_length, int buffer_length);
	virtual ~PitchFilter();

	/* --- method*/
	void setPitchShift(float pitch, float if_samplerate);

	/* --- member */
private:
	/* --- method*/
	void effect(const juce::HeapBlock<juce::dsp::Complex<float>>& in_data, juce::HeapBlock<juce::dsp::Complex<float>>& out_data, int size, int side) override;

	/* --- member */
	std::shared_ptr<int> pitch_table;

};

