#include "stdafx.h"
#include "SoundController.h"


SoundController::SoundController()
{
	SDL_Init(SDL_INIT_AUDIO);

	sampleBufferSize = 1024;
	sampleBuffer0 = new Sint16[sampleBufferSize]();
	sampleBuffer1 = new Sint16[sampleBufferSize]();
	curSampleBuffer = 0;

	SDL_AudioSpec desiredSpec;
	desiredSpec.freq = 44100;
	desiredSpec.format = AUDIO_S16SYS;
	desiredSpec.channels = 1;
	desiredSpec.samples = sampleBufferSize;
	desiredSpec.callback = AudioCallback;
	desiredSpec.userdata = this;

	SDL_OpenAudio(&desiredSpec, &obtainedSpec);
	
	// Unpause
	SDL_PauseAudio(0);

	AudioEnabled = 1;

	LeftSpeakerEnabled = 0;
	LeftSpeakerVolume = 0;
	RightSpeakerEnabled = 0;
	RightSpeakerVolume = 0;
	SoundOutput = 0xFF;

	ch1_FrameSequencerTicks = 0;
	ch1_FrameSequencerStep = 0;
	ch1_Enabled = 0;
	ch1_LengthCounter = 0;
	ch1_CounterOrConsecutiveSelection = 0;
	ch1_FrequencyCounter = 0.0;
	ch1_EnvelopeDirection = 0;
	ch1_WavePattern = 0;
	ch1_Duty = squareDutyMap[ch1_WavePattern];
	ch1_lo = true;
	ch1_FrequencyLo = 0;
	ch1_FrequencyHi = 0;
	ch1_SweepShadow = 0;
	ch1_SweepPeriod = 0;
	ch1_SweepDirection = 0;
	ch1_SweepTimer = 0;
	ch1_SweepEnabled = 0;
	ch1_SweepShift = 0;

	ch2_FrameSequencerTicks = 0;
	ch2_FrameSequencerStep = 0;
	ch2_Enabled = false;
	ch2_LengthCounter = 0;
	ch2_CounterOrConsecutiveSelection = 0;
	ch2_FrequencyCounter = 0.0;
	ch2_EnvelopeDirection = 0;
	ch2_WavePattern = 0;
	ch2_Duty = squareDutyMap[ch2_WavePattern];
	ch2_lo = true;
	ch2_FrequencyLo = 0;
	ch2_FrequencyHi = 0;

	ch3_FrameSequencerTicks = 0;
	ch3_FrameSequencerStep = 0;
	ch3_FrequencyCounter = 0.0;
	ch3_Enabled = 0;
	ch3_WaveDataPosition = 0;
	ch3_LengthCounter = 0;
	ch3_Volume = 0;
	ch3_FrequencyLo = 0;
	ch3_FrequencyHi = 0;
	ch3_CounterOrConsecutiveSelection = 0;

	ch4_FrameSequencerTicks = 0;
	ch4_FrameSequencerStep = 0;
	ch4_FrequencyCounter = 0;
	ch4_Enabled = 0;
	ch4_LengthCounter = 0;
	ch4_CounterOrConsecutiveSelection = 0;
	ch4_InitialVolume = 0;
	ch4_EnvelopeVolume = 0;
	ch4_EnvelopeDirection = 0;
	ch4_EnvelopePeriod = 0;
	ch4_EnvelopeCounter = 0;
	ch4_ClockShift = 0;
	ch4_WidthMode = 0;
	ch4_DivisorCode = 0;
	ch4_LengthEnable = 0;
	ch4_LFSR = 0b011010100101011;

	std::thread fillSamplesThread(&SoundController::FillSamples, this);
	fillSamplesThread.detach();
}

SoundController::~SoundController()
{
}

void SoundController::WriteByte(int value, long address)
{
	if (address == 0xFF26)
	{
		AudioEnabled = ((value >> 7) & 1);
		return;
	}
	else if (AudioEnabled == 0)
	{
		return;
	}

	switch (address)
	{
	case 0xff10:
		ch1_SweepPeriod = ((value >> 4) & 7);
		ch1_SweepDirection = ((value >> 3) & 1);
		ch1_SweepShift = (value & 7);
		break;
	case 0xff11:
		ch1_WavePattern = ((value >> 6) & 0b11);
		ch1_Duty = squareDutyMap[ch1_WavePattern];
		ch1_LengthCounter = (value & 0b00111111);
		break;
	case 0xff12:
		ch1_EnvelopeCounter = (value & 0x7);
		ch1_EnvelopePeriod = ch1_EnvelopeCounter;
		ch1_EnvelopeDirection = ((value >> 3) & 1);
		ch1_InitialVolume = ((value >> 4) & 0xF);
		ch1_EnvelopeVolume = ch1_InitialVolume;
		break;
	case 0xff13:
		ch1_FrequencyLo = value;
		break;
	case 0xff14:
		if ((value & 0b10000000) > 0)
		{
			// Restart sound.
			ch1_Enabled = 1;
			ch1_LengthCounter = 63;
			ch1_FrequencyCounter = 0;
			ch1_FrequencyHi = (value & 7);
			ch1_SweepShadow = (ch1_FrequencyHi << 8) | ch1_FrequencyLo;
			ch1_SweepTimer = ch1_SweepPeriod;
			if (ch1_SweepPeriod > 0 || ch1_SweepShift > 0)
			{
				ch1_SweepEnabled = 1;
			}
		}
		ch1_CounterOrConsecutiveSelection = ((value >> 6) & 1);
		break;
	case 0xff16:
		ch2_WavePattern = ((value >> 6) & 0b11);
		ch2_Duty = squareDutyMap[ch2_WavePattern];
		ch2_LengthCounter = (value & 0b00111111);
		break;
	case 0xff17:
		ch2_EnvelopeCounter = (value & 0x7);
		ch2_EnvelopePeriod = ch2_EnvelopeCounter;
		ch2_EnvelopeDirection = ((value >> 3) & 1);
		ch2_InitialVolume = ((value >> 4) & 0xF);
		ch2_EnvelopeVolume = ch2_InitialVolume;
		break;
	case 0xff18:
		ch2_FrequencyLo = value;
		break;
	case 0xff19:
		if ((value & 0b10000000) > 0)
		{
			// Restart sound.
			ch2_Enabled = true;
			ch2_LengthCounter = 63;
			ch2_FrequencyCounter = 0;
		}
		ch2_CounterOrConsecutiveSelection = ((value >> 6) & 1);
		ch2_FrequencyHi = (value & 7);
		break;
	case 0xFF1A:
		ch3_Enabled = value > 0 ? 1 : 0;
		break;
	case 0xFF1B:
		ch3_LengthCounter = value;
		break;
	case 0xFF1C:
		ch3_Volume = ((value >> 5) & 3);
		break;
	case 0xFF1D:
		ch3_FrequencyLo = value;
		break;
	case 0xFF1E:
		if ((value & 0b10000000) > 0)
		{
			// Restart sound.
			ch3_Enabled = true;
			ch3_LengthCounter = 0xFF;
			ch3_WaveDataPosition = 0;
		}
		ch3_CounterOrConsecutiveSelection = ((value >> 6) & 1);
		ch3_FrequencyHi = (value & 7);
		break;
	case 0xFF20:
		ch4_LengthCounter = value;
		break;
	case 0xFF21:
		ch4_EnvelopeCounter = (value & 0x7);
		ch4_EnvelopePeriod = ch4_EnvelopeCounter;
		ch4_EnvelopeDirection = ((value >> 3) & 1);
		ch4_InitialVolume = ((value >> 4) & 0xF);
		ch4_EnvelopeVolume = ch4_InitialVolume;
		break;
	case 0xFF22:
		ch4_ClockShift = ((value >> 4) & 0xF);
		ch4_WidthMode = ((value >> 3) & 1);
		ch4_DivisorCode = (value & 0x7);
		break;
	case 0xFF23:
		if ((value & 0b10000000) > 0)
		{
			// Restart sound.
			ch4_Enabled = true;
			ch4_LengthCounter = 0xFF;
		}
		ch4_CounterOrConsecutiveSelection = ((value >> 6) & 1);
		break;
	case 0xFF24:
		LeftSpeakerEnabled = ((value >> 7) & 1);
		LeftSpeakerVolume = ((value >> 4) & 0x7);
		RightSpeakerEnabled = ((value >> 3) & 1);
		RightSpeakerVolume = (value & 0x7);
		break;
	case 0xFF25:
		SoundOutput = value;
		break;
	}

	if (address >= 0xFF30 && address < 0xFF40)
	{
		int index = 2 * (address - 0xFF30);
		ch3_WaveData[index] = (value >> 4);
		ch3_WaveData[index + 1] = (value & 0xF);
	}
}

int SoundController::ReadByte(long address)
{
	if (address == 0xFF26)
	{
		int ch1On = ch1_LengthCounter > 0 ? 1 : 0;
		int ch2On = ch2_LengthCounter > 0 ? 1 : 0;
		int ch3On = ch3_LengthCounter > 0 ? 1 : 0;
		int ch4On = ch4_LengthCounter > 0 ? 1 : 0;
		return (AudioEnabled << 7) | (ch4On << 3) | (ch3On << 2) | (ch2On << 1) | ch1On;
	}
	else if (AudioEnabled == 0)
	{
		return 0;
	}

	switch (address)
	{
	case 0xff10:
		return (ch1_SweepPeriod << 4) | (ch1_SweepDirection << 3) | ch1_SweepShift;
	case 0xff11:
		return (ch1_WavePattern << 6) | 0b00111111;
	case 0xff12:
		return (ch1_InitialVolume << 4) | (ch1_EnvelopeDirection << 3) | ch1_EnvelopePeriod;
	case 0xff13:
		return 0xFF;
	case 0xff14:
		return (ch1_CounterOrConsecutiveSelection << 6) | 0b10111111;
	case 0xff16:
		return (ch2_WavePattern << 6) | 0b00111111;
	case 0xff17:
		return (ch2_InitialVolume << 4) | (ch2_EnvelopeDirection << 3) | ch2_EnvelopePeriod;
	case 0xff18:
		return 0xFF;
	case 0xff19:
		return (ch2_CounterOrConsecutiveSelection << 6) | 0b10111111;
	case 0xFF1A:
		return 0b11111110 | ch3_Enabled;
	case 0xFF1B:
		return ch3_LengthCounter;
	case 0xFF1C:
		return 0b10011111 | (ch3_Volume << 5);
	case 0xFF1D:
		return 0xFF;
	case 0xFF1E:
		return (ch3_CounterOrConsecutiveSelection << 6) | 0b10111111;
	case 0xFF20:
		return ch4_LengthCounter;
	case 0xFF21:
		return (ch4_InitialVolume << 4) | (ch4_EnvelopeDirection << 3) | ch4_EnvelopePeriod;
	case 0xFF22:
		return (ch4_ClockShift << 4) | (ch4_WidthMode << 3) | ch4_DivisorCode;
	case 0xFF23:
		return (ch4_CounterOrConsecutiveSelection << 6) | 0b10111111;
	case 0xFF24:
		return (LeftSpeakerEnabled << 7) | (LeftSpeakerVolume < 4) | (RightSpeakerEnabled << 3) | RightSpeakerVolume;
	case 0xFF25:
		return SoundOutput;
	}

	if (address >= 0xFF30 && address < 0xFF40)
	{
		int index = 2 * (address - 0xFF30);
		return (ch3_WaveData[index] << 4) | ch3_WaveData[index + 1];
	}

	return 0;
}

void SoundController::Tick(int cpuCycles, int cpuCyclesPerSecond)
{
	if (AudioEnabled > 0)
	{
		TickChannel1(cpuCycles, cpuCyclesPerSecond);
		TickChannel2(cpuCycles, cpuCyclesPerSecond);
		TickChannel3(cpuCycles, cpuCyclesPerSecond);
		TickChannel4(cpuCycles, cpuCyclesPerSecond);
	}
}

void SoundController::TickChannel1(int cpuCycles, int cpuCyclesPerSecond)
{
	int cyclesPerStep = cpuCyclesPerSecond / 512;  // Frame Sequencer is clocked at 512 Hz
	ch1_FrameSequencerTicks += cpuCycles;
	if (ch1_FrameSequencerTicks > cyclesPerStep)
	{
		ch1_FrameSequencerTicks -= cyclesPerStep;
		ch1_FrameSequencerStep = (ch1_FrameSequencerStep + 1) % 8;

		switch (ch1_FrameSequencerStep)
		{
		case 0:
			Ch1_ClockLengthCounter();
			break;
		case 2:
			Ch1_ClockLengthCounter();
			Ch1_ClockSweep();
			break;
		case 4:
			Ch1_ClockLengthCounter();
			break;
		case 6:
			Ch1_ClockLengthCounter();
			Ch1_ClockSweep();
			break;
		case 7:
			Ch1_ClockEnvelope();
			break;
		}
	}
}

void SoundController::Ch1_ClockLengthCounter()
{
	if (ch1_CounterOrConsecutiveSelection == 1 && ch1_LengthCounter > 0)
	{
		ch1_LengthCounter -= 1;
		if (ch1_LengthCounter == 0)
		{
			ch1_Enabled = 0;
		}
	}
}

void SoundController::Ch1_ClockEnvelope()
{
	if (ch1_EnvelopePeriod > 0)
	{
		ch1_EnvelopeCounter -= 1;
		if (ch1_EnvelopeCounter == 0)
		{
			ch1_EnvelopeCounter = ch1_EnvelopePeriod;
			if (ch1_EnvelopeDirection > 0 && ch1_EnvelopeVolume < 0xF)
			{
				ch1_EnvelopeVolume += 1;
			}
			else if (ch1_EnvelopeDirection == 0 && ch1_EnvelopeVolume > 0)
			{
				ch1_EnvelopeVolume -= 1;
			}
		}
	}
}

void SoundController::Ch1_ClockSweep()
{
	if (ch1_SweepPeriod)
	{
		ch1_SweepTimer -= 1;
		if (ch1_SweepTimer == 0)
		{
			ch1_SweepTimer = ch1_SweepPeriod;
			if (ch1_SweepEnabled && ch1_SweepPeriod > 0)
			{
				int newFrequency = Ch1_RecalculateSweep();
				if (newFrequency > 2047)
				{
					ch1_Enabled = 0;
				}
				else
				{
					ch1_SweepShadow = newFrequency;
					ch1_FrequencyLo = (newFrequency & 0xFF);
					ch1_FrequencyHi = ((newFrequency >> 8) & 0xFF);
				}
			}
		}
	}
}

int SoundController::Ch1_RecalculateSweep()
{
	int direction = ch1_SweepDirection == 0 ? 1 : -1;
	int newFrequency = direction * (ch1_SweepShadow >> ch1_SweepShift) + ch1_SweepShadow;
	return newFrequency;
}

void SoundController::TickChannel2(int cpuCycles, int cpuCyclesPerSecond)
{
	int cyclesPerStep = cpuCyclesPerSecond / 512;  // Frame Sequencer is clocked at 512 Hz
	ch2_FrameSequencerTicks += cpuCycles;
	if (ch2_FrameSequencerTicks > cyclesPerStep)
	{
		ch2_FrameSequencerTicks -= cyclesPerStep;
		ch2_FrameSequencerStep = (ch2_FrameSequencerStep + 1) % 8;

		switch (ch2_FrameSequencerStep)
		{
		case 0:
			Ch2_ClockLengthCounter();
			break;
		case 2:
			Ch2_ClockLengthCounter();
			break;
		case 4:
			Ch2_ClockLengthCounter();
			break;
		case 6:
			Ch2_ClockLengthCounter();
			break;
		case 7:
			Ch2_ClockEnvelope();
			break;
		}
	}
}

void SoundController::Ch2_ClockLengthCounter()
{
	if (ch2_CounterOrConsecutiveSelection == 1 && ch2_LengthCounter > 0)
	{
		ch2_LengthCounter -= 1;
		if (ch2_LengthCounter == 0)
		{
			ch2_Enabled = false;
		}
	}
}

void SoundController::Ch2_ClockEnvelope()
{
	ch2_EnvelopeCounter -= 1;
	if (ch2_EnvelopeCounter == 0)
	{
		ch2_EnvelopeCounter = ch2_EnvelopePeriod;
		if (ch2_EnvelopeDirection > 0 && ch2_EnvelopeVolume < 0xF)
		{
			ch2_EnvelopeVolume += 1;
		}
		else if (ch2_EnvelopeDirection == 0 && ch2_EnvelopeVolume > 0)
		{
			ch2_EnvelopeVolume -= 1;
		}
	}
}

void SoundController::TickChannel3(int cpuCycles, int cpuCyclesPerSecond)
{
	int cyclesPerStep = cpuCyclesPerSecond / 512;  // Frame Sequencer is clocked at 512 Hz
	ch3_FrameSequencerTicks += cpuCycles;
	if (ch3_FrameSequencerTicks > cyclesPerStep)
	{
		ch3_FrameSequencerTicks -= cyclesPerStep;
		ch3_FrameSequencerStep = (ch3_FrameSequencerStep + 1) % 8;
		switch (ch3_FrameSequencerStep)
		{
		case 0:
			Ch3_ClockLengthCounter();
			break;
		case 2:
			Ch3_ClockLengthCounter();
			break;
		case 4:
			Ch3_ClockLengthCounter();
			break;
		case 6:
			Ch3_ClockLengthCounter();
			break;
		}
	}
}

void SoundController::Ch3_ClockLengthCounter()
{
	if (ch3_CounterOrConsecutiveSelection == 1 && ch3_LengthCounter > 0)
	{
		ch3_LengthCounter -= 1;
		if (ch3_LengthCounter == 0)
		{
			ch3_Enabled = false;
		}
	}
}

void SoundController::TickChannel4(int cpuCycles, int cpuCyclesPerSecond)
{
	int cyclesPerStep = cpuCyclesPerSecond / 512;  // Frame Sequencer is clocked at 512 Hz
	ch4_FrameSequencerTicks += cpuCycles;
	if (ch4_FrameSequencerTicks > cyclesPerStep)
	{
		ch4_FrameSequencerTicks -= cyclesPerStep;
		ch4_FrameSequencerStep = (ch4_FrameSequencerStep + 1) % 8;
		switch (ch4_FrameSequencerStep)
		{
		case 0:
			Ch4_ClockLengthCounter();
			break;
		case 2:
			Ch4_ClockLengthCounter();
			break;
		case 4:
			Ch4_ClockLengthCounter();
			break;
		case 6:
			Ch4_ClockLengthCounter();
			break;
		case 7:
			Ch4_ClockEnvelope();
			break;
		}
	}
}

void SoundController::Ch4_ClockLengthCounter()
{
	if (ch4_CounterOrConsecutiveSelection == 1 && ch4_LengthCounter > 0)
	{
		ch4_LengthCounter -= 1;
		if (ch4_LengthCounter == 0)
		{
			ch4_Enabled = false;
		}
	}
}

void SoundController::Ch4_ClockEnvelope()
{
	ch4_EnvelopeCounter -= 1;
	if (ch4_EnvelopeCounter == 0)
	{
		ch4_EnvelopeCounter = ch4_EnvelopePeriod;
		if (ch4_EnvelopeDirection > 0 && ch4_EnvelopeVolume < 0xF)
		{
			ch4_EnvelopeVolume += 1;
		}
		else if (ch4_EnvelopeDirection == 0 && ch4_EnvelopeVolume > 0)
		{
			ch4_EnvelopeVolume -= 1;
		}
	}
}

void SoundController::FillSamples()
{
	int sampleRate = obtainedSpec.freq;
	float secondsPerSample = 1.0 / sampleRate;

	while (true)
	{
		// TODO: This loop is just spinning if audio is disabled. Might want a more elegant solution to this.
		if (AudioEnabled == 0)
		{
			continue;
		}

		for (int bufferIndex = 0; bufferIndex < numSampleBuffers; bufferIndex++)
		{
			if (emptyBuffers[bufferIndex] > 0)
			{
				Sint16 *buffer = sampleBuffer0;
				switch (bufferIndex)
				{
				case 0:
					buf1Lock.lock();
					buffer = sampleBuffer0;
					break;
				case 1:
					buf2Lock.lock();
					buffer = sampleBuffer1;
					break;
				}

				double ch1SquareFrequency = 131072.0 / (2048.0 - (((ch1_FrequencyHi & 0x7) << 8) | ch1_FrequencyLo));
				double ch1CycleSamples = sampleRate / ch1SquareFrequency;
				double ch1CycleSamplesLow = ch1CycleSamples * ch1_Duty;
				double ch1CyclesSamplesHigh = ch1CycleSamples - ch1CycleSamplesLow;

				double ch2SquareFrequency = 131072.0 / (2048.0 - (((ch2_FrequencyHi & 0x7) << 8) | ch2_FrequencyLo));
				double ch2CycleSamples = sampleRate / ch2SquareFrequency;
				double ch2CycleSamplesLow = ch2CycleSamples * ch2_Duty;
				double ch2CyclesSamplesHigh = ch2CycleSamples - ch2CycleSamplesLow;

				double ch3Frequency = 65536.0 / (2048.0 - (((ch3_FrequencyHi & 0x7) << 8) | ch3_FrequencyLo));
				double ch3SamplesPerWaveDataSample = sampleRate / ch3Frequency / 32.0;

				double ch4SamplesPerFrequencyClock = 512.0 / ((8 * (ch4_DivisorCode + 1)) << ch4_ClockShift);

				for (int i = 0; i < sampleBufferSize; i++)
				{
					Sint16 sample = 0;

					if ((SoundOutput & 0x10) > 0)
					{
						sample += ProduceChannel1Sample(ch1CycleSamples, ch1CycleSamplesLow, ch1CyclesSamplesHigh);
					}
					if ((SoundOutput & 0x20) > 0)
					{
						sample += ProduceChannel2Sample(ch2CycleSamples, ch2CycleSamplesLow, ch2CyclesSamplesHigh);
					}
					if ((SoundOutput & 0x40) > 0)
					{
						sample += ProduceChannel3Sample(ch3SamplesPerWaveDataSample);
					}
					if ((SoundOutput & 0x80) > 0)
					{
						sample += ProduceChannel4Sample(ch4SamplesPerFrequencyClock);
					}

					buffer[i] = sample * (LeftSpeakerVolume / 7.0);
				}

				// Mark sample buffer as no longer empty.
				emptyBuffers[bufferIndex] = 0;

				switch (bufferIndex)
				{
				case 0:
					buf1Lock.unlock();
					break;
				case 1:
					buf2Lock.unlock();
					break;
				}
			}
		}
	}
}

Sint16 SoundController::ProduceChannel1Sample(float cycleSamples, float cycleSamplesLow, float cyclesSamplesHigh)
{
	Sint16 sample;
	if (ch1_Enabled > 0 && ch1_EnvelopeVolume > 0)
	{
		Sint16 amplitude = squareAmplitudes[ch1_EnvelopeVolume];
		if (ch1_lo)
		{
			sample = 0;
		}
		else
		{
			sample = amplitude;
		}
	}
	else
	{
		sample = 0;
	}

	ch1_FrequencyCounter += 1.0;

	if (ch1_lo && ch1_FrequencyCounter > cycleSamplesLow)
	{
		ch1_FrequencyCounter -= cycleSamplesLow;
		ch1_lo = !ch1_lo;
	}
	else if (!ch1_lo && ch1_FrequencyCounter > cyclesSamplesHigh)
	{
		ch1_FrequencyCounter -= cyclesSamplesHigh;
		ch1_lo = !ch1_lo;
	}

	return sample;
}

Sint16 SoundController::ProduceChannel2Sample(float cycleSamples, float cycleSamplesLow, float cyclesSamplesHigh)
{
	Sint16 sample;
	if (ch2_Enabled && ch2_EnvelopeVolume > 0)
	{
		Sint16 amplitude = squareAmplitudes[ch2_EnvelopeVolume];
		if (ch2_lo)
		{
			sample = 0;
		}
		else
		{
			sample = amplitude;
		}
	}
	else
	{
		sample = 0;
	}

	ch2_FrequencyCounter += 1.0;

	if (ch2_lo && ch2_FrequencyCounter > cycleSamplesLow)
	{
		ch2_FrequencyCounter -= cycleSamplesLow;
		ch2_lo = !ch2_lo;
	}
	else if (!ch2_lo && ch2_FrequencyCounter > cyclesSamplesHigh)
	{
		ch2_FrequencyCounter -= cyclesSamplesHigh;
		ch2_lo = !ch2_lo;
	}

	return sample;
}

Sint16 SoundController::ProduceChannel3Sample(float samplesPerWaveDataSample)
{
	Sint16 sample;
	sample = (ch3_WaveData[ch3_WaveDataPosition] / 15.0) * SQUARE_AMPLITUDE * ch3_VolumeMap[ch3_Volume];
	if (ch3_Enabled && ch3_Volume > 0)
	{
		ch3_FrequencyCounter += 1.0;

		if (ch3_FrequencyCounter > samplesPerWaveDataSample)
		{
			ch3_FrequencyCounter -= samplesPerWaveDataSample;
			ch3_WaveDataPosition = (ch3_WaveDataPosition + 1) % 32;
		}
	}

	return sample;
}

Sint16 SoundController::ProduceChannel4Sample(float samplesPerFrequencyClock)
{
	Sint16 sample;
	if (ch4_Enabled && ch4_EnvelopeVolume > 0)
	{
		sample = squareAmplitudes[ch4_EnvelopeVolume] * (1 - (ch4_LFSR & 1));
	}
	else
	{
		sample = 0;
	}

	ch4_FrequencyCounter += 1.0;
	if (ch4_FrequencyCounter > samplesPerFrequencyClock)
	{
		ch4_FrequencyCounter -= samplesPerFrequencyClock;

		int xorBit = ((ch4_LFSR >> 1) & 1) ^ (ch4_LFSR & 1);
		ch4_LFSR = (xorBit << 14) | (ch4_LFSR >> 1);
		if (ch4_WidthMode > 0)
		{
			ch4_LFSR = (ch4_LFSR & 0x7FFF) | (xorBit << 6);
		}
	}
	
	return sample;
}

void AudioCallback(void * _soundController, Uint8 * _stream, int _numSamples)
{
	Sint16 *stream = (Sint16*)_stream;
	int numSamples = _numSamples / 2;
	SoundController *soundController = (SoundController *)_soundController;

	if (soundController->AudioEnabled == 0)
	{
		return;
	}

	if (soundController->curSampleBuffer == 0)
	{
		soundController->buf1Lock.lock();
		memcpy(stream, soundController->sampleBuffer0, sizeof(Sint16) * soundController->sampleBufferSize);
		soundController->emptyBuffers[0] = 1;
		soundController->buf1Lock.unlock();
	}
	else if (soundController->curSampleBuffer == 1)
	{
		soundController->buf2Lock.lock();
		memcpy(stream, soundController->sampleBuffer1, sizeof(Sint16) * soundController->sampleBufferSize);
		soundController->emptyBuffers[1] = 1;
		soundController->buf2Lock.unlock();
	}

	soundController->curSampleBuffer = (soundController->curSampleBuffer + 1) % soundController->numSampleBuffers;
}
