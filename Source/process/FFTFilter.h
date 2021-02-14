#pragma once
#include "FilterBase.h"
#include <JuceHeader.h>

class FFTFilter : public FilterBase
{
public:
	FFTFilter(int segment_length, int buffer_length);
	virtual ~FFTFilter();

	/* --- method*/
	std::shared_ptr<float> getSpectrum(void);
	/* --- member */
	enum
	{
		fftOrder = 12,
		fftSize = 1 << fftOrder,  //4096
		spectrumNum = 512
	};
private:
	/* --- method*/
	void filter_process(float *in_data, float *out_data, int size, int side) override;
	virtual void effect(const juce::HeapBlock<juce::dsp::Complex<float>> &in_data, juce::HeapBlock<juce::dsp::Complex<float>> &out_data, int size, int side);
	/* --- member */
	std::shared_ptr<float> fftbuf;
	std::shared_ptr<float> level;
	std::shared_ptr<int> pitch_table;
	juce::dsp::FFT forwardFFT;
	juce::dsp::WindowingFunction<float> window;
	//
	juce::HeapBlock<juce::dsp::Complex<float>> fftIn, fftOut;
};
