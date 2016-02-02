#pragma once
#include <mutex>

#include "SDL.h"

class SoundController
{
public:
	SoundController();
	~SoundController();
	void WriteByte(int value, long address);
	int ReadByte(long address);
	void Tick(int cpuCycles, int cpuCyclesPerSecond);
	void TickChannel1(int cpuCycles, int cpuCyclesPerSecond);
	void TickChannel2(int cpuCycles, int cpuCyclesPerSecond);
	void TickChannel3(int cpuCycles, int cpuCyclesPerSecond);
	void TickChannel4(int cpuCycles, int cpuCyclesPerSecond);
	void FillSamples(Sint16 *stream, int numSamples);
private:
	// Channel 1 (Tone & Sweep)
	int NR10; // 0xFF10

	void Ch1_ClockLengthCounter();
	void Ch1_ClockEnvelope();
	void Ch1_ClockSweep();
	int Ch1_RecalculateSweep();
	void Ch2_ClockLengthCounter();
	void Ch2_ClockEnvelope();
	void Ch3_ClockLengthCounter();

	Sint16 SoundController::ProduceChannel1Sample(float cycleSamples, float cycleSamplesLow, float cyclesSamplesHigh);
	Sint16 SoundController::ProduceChannel2Sample(float cycleSamples, float cycleSamplesLow, float cyclesSamplesHigh);
	Sint16 SoundController::ProduceChannel3Sample(float samplesPerWaveDataSample);

	// Wave synthesis helpers.
	const int SQUARE_AMPLITUDE = 4000;
	int squareAmplitudes[16] = {						 0,		 SQUARE_AMPLITUDE / 16,  2 * SQUARE_AMPLITUDE / 16,  3 * SQUARE_AMPLITUDE / 16,
								 4 * SQUARE_AMPLITUDE / 16,  5 * SQUARE_AMPLITUDE / 16,  6 * SQUARE_AMPLITUDE / 16,  7 * SQUARE_AMPLITUDE / 16,
								 8 * SQUARE_AMPLITUDE / 16,  9 * SQUARE_AMPLITUDE / 16, 10 * SQUARE_AMPLITUDE / 16, 11 * SQUARE_AMPLITUDE / 16,
								12 * SQUARE_AMPLITUDE / 16, 13 * SQUARE_AMPLITUDE / 16, 14 * SQUARE_AMPLITUDE / 16, 15 * SQUARE_AMPLITUDE / 16 };
	float squareDutyMap[4] = { 0.125, 0.25, 0.50, 0.75 };

	// Channel 1 (Square Wave)
	int ch1_FrameSequencerTicks;
	int ch1_FrameSequencerStep;
	int ch1_Enabled;
	int ch1_LengthCounter;
	int ch1_CounterOrConsecutiveSelection;
	float ch1_FrequencyCounter;
	int ch1_EnvelopeDirection;
	int ch1_InitialVolume;
	int ch1_EnvelopeVolume;
	int ch1_EnvelopePeriod;
	int ch1_EnvelopeCounter;
	float ch1_Duty;
	int ch1_WavePattern;
	bool ch1_lo;
	int ch1_FrequencyLo;
	int ch1_FrequencyHi;
	int ch1_SweepShadow;
	int ch1_SweepTimer;
	int ch1_SweepPeriod;
	int ch1_SweepDirection;
	int ch1_SweepEnabled;
	int ch1_SweepShift;

	// Channel 2 (Square Wave)
	int ch2_FrameSequencerTicks;
	int ch2_FrameSequencerStep;
	int ch2_Enabled;
	int ch2_LengthCounter;
	int ch2_CounterOrConsecutiveSelection;
	float ch2_FrequencyCounter;
	int ch2_EnvelopeDirection;
	int ch2_InitialVolume;
	int ch2_EnvelopeVolume;
	int ch2_EnvelopePeriod;
	int ch2_EnvelopeCounter;
	float ch2_Duty;
	int ch2_WavePattern;
	bool ch2_lo;
	int ch2_FrequencyLo;
	int ch2_FrequencyHi;

	// Channel 3 (Programmable Wave)
	int ch3_FrameSequencerTicks;
	int ch3_FrameSequencerStep;
	float ch3_FrequencyCounter;
	int ch3_Enabled;
	int ch3_CounterOrConsecutiveSelection;
	int ch3_WaveData[32] = {};
	int ch3_WaveDataPosition;
	int ch3_LengthCounter;
	float ch3_VolumeMap[4] = { 0.0, 1.0, 0.5, 0.25 };
	int ch3_Volume;
	int ch3_FrequencyLo;
	int ch3_FrequencyHi;

	SDL_AudioSpec obtainedSpec;

	std::mutex ch1Lock;
};

void AudioCallback(void* userData, Uint8* stream, int numSamples);

