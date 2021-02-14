#pragma once
#include "PitchFilter.h"


PitchFilter::PitchFilter(int segment_length, int buffer_length) :
	FFTFilter(segment_length, buffer_length)
	, pitch_table(new int[segment_length])
{
	for (int i = 0; i < segment_length; i++) {
		pitch_table.get()[i] = i;
	}
}

PitchFilter::~PitchFilter()
{
}

void PitchFilter::setPitchShift(float pitch, float if_samplerate)
{
	int fft_size = FFTFilter::fftSize;
	float target = (float)((if_samplerate / 2.0) / pitch);
	float area_min = (float)((if_samplerate / 2.0) / (fft_size / 2));
	float step = target / (fft_size / 2 - 1);
	{
		std::lock_guard<std::mutex> lock(flag_lock);
		// clear
		for (auto i = 0; i < fft_size; i++) {
			pitch_table.get()[i] = -1;
		}
		// pitch shift table
		int post_fq = 0;
		for (auto i = 0; i < (fft_size / 2 - 1); i++) {
			int pos1 = (fft_size / 2) - 1 - i;
			int pos2 = (fft_size / 2) + 1 + i;
			int fq = juce::jmap((int)target, 0, (int)(if_samplerate / 2.0), 1, (fft_size / 2) - 1);
			if (post_fq != fq) {
				pitch_table.get()[pos1] = fq;
				pitch_table.get()[pos2] = fft_size - fq;
			}
			post_fq = fq;
			// next
			target = juce::jmax((target - step), area_min);
		}
	}
}

void PitchFilter::effect(const juce::HeapBlock<juce::dsp::Complex<float>>& in_data, juce::HeapBlock<juce::dsp::Complex<float>>& out_data, int size, int side)
{
	{
		std::lock_guard<std::mutex> lock(flag_lock);
		// pitch shifting
		for (int i = 0; i < size; i++) {
			if (pitch_table.get()[i] < 0) {
				in_data.get()[i].real(0);
				in_data.get()[i].imag(0);
			}
			else {
				in_data.get()[i].real(out_data.get()[pitch_table.get()[i]].real());
				in_data.get()[i].imag(out_data.get()[pitch_table.get()[i]].imag());
			}
		}
	}
}