#include "FFTFilter.h"

FFTFilter::FFTFilter(int segment_length, int buffer_length) :
	FilterBase(segment_length, buffer_length)
	,forwardFFT(fftOrder)
	,window(fftSize, juce::dsp::WindowingFunction<float>::hann)
	,fftbuf(new float[segment_length])
	,level(new float[spectrumNum])
{
	fftIn.calloc(segment_length);
	fftOut.calloc(segment_length);
}

FFTFilter::~FFTFilter()
{
	fftOut.free();
	fftIn.free();
}

std::shared_ptr<float> FFTFilter::getSpectrum(void)
{
	auto mindB = -100.0f;
	auto maxdB = 0.0f;
	for (int i = 0; i < spectrumNum; i++) {
		auto pos = (int)((segment_length / 2) * i / spectrumNum);
		auto power = juce::jmap(
			juce::jlimit(
				mindB,
				maxdB,
				juce::Decibels::gainToDecibels(fftbuf.get()[pos]) - juce::Decibels::gainToDecibels((float)fftSize)
			),
			mindB, maxdB, 0.0f, 1.0f
		);
		level.get()[i] = power;
	}
	setProcessFlag(false);
	return level;
}

void FFTFilter::filter_process(float *in_data, float *out_data, int size, int side)
{
	{
		for (int i = 0; i < size; i++) {
			fftIn.getData()[i].real(in_data[i]);
			fftIn.getData()[i].imag(0.0f);
		}
		forwardFFT.perform(fftIn.getData(), fftOut.getData(), false);
		// Spectrum Analyzer data
		if (side == 0) {
			for (int i = 0; i < size; i++) {
				fftbuf.get()[i] = std::sqrtf(fftOut.get()[i].real() * fftOut.get()[i].real() + fftOut.get()[i].imag() * fftOut.get()[i].imag());
			}
		}
		// effect work
		effect(fftIn, fftOut, size, side);
		// inverse FFT
		forwardFFT.perform(fftIn.getData(), fftOut.getData(), true);
		// copy to output
		for (int i = 0; i < size; i++) {
			out_data[i] = (float)(fftOut.get()[i].real());
		}
	}
}

void FFTFilter::effect(const juce::HeapBlock<juce::dsp::Complex<float>>& in_data, juce::HeapBlock<juce::dsp::Complex<float>>& out_data, int size, int side)
{
	// no imprement
}
