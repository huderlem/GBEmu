#pragma once
#include <mutex>
#include <thread>

#include "Blargg_Gb_Snd_Emu\Gb_Apu.h"
#include "Blargg_Gb_Snd_Emu\Multi_Buffer.h"
#include "Blargg_Gb_Snd_Emu\Sound_Queue.h"

class SoundController
{
public:
	SoundController();
	~SoundController();
	void WriteByte(int value, long address);
	int ReadByte(long address);
	void Tick(int cpuCycles, int cpuCyclesPerSecond);
	void EndFrame();

	int AudioEnabled;
private:
	Gb_Apu *gbAPU;
	Stereo_Buffer *gbBuffer;
	Sound_Queue *gbSoundQueue;
	blip_sample_t *sampleBuffer;

	int audioTime;
	const int frameLength = 1000;
	const int audioBufferSize = 2048;
	long long absoluteAudioTime;
};
