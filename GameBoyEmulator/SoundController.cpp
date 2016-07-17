#include "stdafx.h"
#include "SDL.h"

#include "SoundController.h"


SoundController::SoundController()
{
	gbAPU = new Gb_Apu();
	gbBuffer = new Stereo_Buffer();
	gbSoundQueue = new Sound_Queue();
	sampleBuffer = new blip_sample_t[audioBufferSize];

	gbAPU->treble_eq(-10);
	gbBuffer->bass_freq(100);
	gbAPU->output(gbBuffer->center(), gbBuffer->left(), gbBuffer->right());
	gbBuffer->clock_rate(4194304);
	gbBuffer->set_sample_rate(44100);

	gbSoundQueue->start(44100, 2);

	audioTime = 0;
	absoluteAudioTime = 0;

	SDL_Init(SDL_INIT_AUDIO);

	AudioEnabled = 1;
}

SoundController::~SoundController()
{
}

void SoundController::WriteByte(int value, long address)
{
	gbAPU->write_register(audioTime, address, value);

	if (address == 0xFF26)
	{
		AudioEnabled = ((value >> 7) & 1);
		return;
	}
}

int SoundController::ReadByte(long address)
{
	return gbAPU->read_register(audioTime, address);
}

void SoundController::Tick(int cpuCycles, int cpuCyclesPerSecond)
{
	audioTime += cpuCycles;
	absoluteAudioTime += cpuCycles;

	if (audioTime >= frameLength)
	{
		EndFrame();
		audioTime -= frameLength;
		absoluteAudioTime = 0;
	}
}

void SoundController::EndFrame()
{
	gbAPU->end_frame(absoluteAudioTime);
	gbBuffer->end_frame(absoluteAudioTime);

	if (gbBuffer->samples_avail() >= audioBufferSize)
	{
		long numSamples = gbBuffer->read_samples(sampleBuffer, audioBufferSize);
		if (AudioEnabled)
		{
			gbSoundQueue->write(sampleBuffer, numSamples);
		}
	}
}
