#include "stdafx.h"
#include "SoundClass.h"


SoundClass::SoundClass()
{
}

SoundClass::SoundClass(const SoundClass &)
{
}


SoundClass::~SoundClass()
{
}

bool SoundClass::Initialize(HWND hwnd)
{
	if (!InitializeDirectSound(hwnd))
	{
		return false;
	}

	if (!LoadWaveFile("./data/sound01.wav", &m_secondaryBuffer1))
	{
		return false;
	}
	return PlayWaveFile();
}

void SoundClass::Shutdown()
{
	ShutdownWaveFile(&m_secondaryBuffer1);

	ShutdownDirectSound();
}

bool SoundClass::InitializeDirectSound(HWND hwnd)
{
	if (FAILED(DirectSoundCreate8(NULL, &m_DirectSound, NULL)))
	{
		return false;
	}

	if (FAILED(m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY)))
	{
		return false;
	}

	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	if (FAILED(m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL)))
	{
		return false;
	}

	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8)* waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;
	
	if (FAILED(m_primaryBuffer->SetFormat(&waveFormat)))
	{
		return false;
	}
	return true;
}

void SoundClass::ShutdownDirectSound()
{
	if (m_primaryBuffer)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = 0;
	}

	if (m_secondaryBuffer1)
	{
		m_secondaryBuffer1->Release();
		m_secondaryBuffer1 = 0;
	}
}

bool SoundClass::LoadWaveFile(char *filename, IDirectSoundBuffer8 ** secondaryBuffer)
{
	FILE* filePtr = nullptr;
	int error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	WaveHeaderType waveFileHeader;
	unsigned int count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		return false;
	}

	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		return false;
	}

	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		return false;
	}

	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}

	if (waveFileHeader.numChannels != 2)
	{
		return false;
	}

	if (waveFileHeader.sampleRate != 44100)
	{
		return false;
	}

	if (waveFileHeader.bitsPerSample != 16)
	{
		return false;
	}

	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return false;
	}

	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8)* waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	IDirectSoundBuffer * tempBuffer = nullptr;
	if (FAILED(m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL)))
	{
		return false;
	}

	if (FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer)))
	{
		return false;
	}

	tempBuffer->Release();
	tempBuffer = 0;

	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	unsigned char* waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
	{
		return false;
	}

	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
	{
		return false;
	}

	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	unsigned char* bufferPtr = nullptr;
	unsigned long bufferSize = 0;
	if (FAILED((*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0)))
	{
		return false;
	}

	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	if (FAILED((*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0)))
	{
		return false;
	}
	delete[] waveData;
	waveData = 0;

	return true;
}

void SoundClass::ShutdownWaveFile(IDirectSoundBuffer8 **secondaryBuffer)
{
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}
}

bool SoundClass::PlayWaveFile()
{
	if (FAILED(m_secondaryBuffer1->SetCurrentPosition(0)))
	{
		return false;
	}

	if (FAILED(m_secondaryBuffer1->SetVolume(DSBVOLUME_MAX)))
	{
		return false;
	}

	if (FAILED(m_secondaryBuffer1->Play(0, 0, 0)))
	{
		return false;
	}

	return true;
}
