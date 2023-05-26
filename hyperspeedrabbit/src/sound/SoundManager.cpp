#include "stdafx.h"
#include "SoundManager.h"

SoundManager::SoundManager(HWND _hwnd) : m_volumeMusic{ Volume::Medium }, m_volumeSound{ Volume::Medium }
{
	// Initialize direct sound and the primary sound buffer.
	if (!InitializeDirectSound(_hwnd))
		exit(-789456);

	////// Musics
	if (!LoadWaveFile("assets/sounds/musicMenu.wav", &m_musicMenuBuffer))
		exit(-1111);

	if (!LoadWaveFile("assets/sounds/musicInGame.wav", &m_musicInGameBuffer))
		exit(-11112);

	if (!LoadWaveFile("assets/sounds/musicVictory.wav", &m_musicVictoryBuffer))
		exit(-11112);

	////// Sounds
	if (!LoadWaveFile("assets/sounds/soundBadCoinGet.wav", &m_soundBadCoinGetBuffer))
		exit(-222222);

	if (!LoadWaveFile("assets/sounds/soundCoinGet.wav", &m_soundCoinGetBuffer))
		exit(-222222);

	if (!LoadWaveFile("assets/sounds/soundCoinIsShot.wav", &m_soundCoinIsShotBuffer))
		exit(-222222);

	if (!LoadWaveFile("assets/sounds/soundObstacleDisappears.wav", &m_soundObstacleDisappearsBuffer))
		exit(-222222);

	if (!LoadWaveFile("assets/sounds/soundObstacleExplodes.wav", &m_soundObstacleExplodesBuffer))
		exit(-222222);

	if (!LoadWaveFile("assets/sounds/soundShipCollideObstacle.wav", &m_soundShipCollideObstacleBuffer))
		exit(-1111);

	if (!LoadWaveFile("assets/sounds/soundShoot.wav", &m_soundShootBuffer))
		exit(-11112);

	updateVolumeMusicText();
	updateVolumeSoundText();}

SoundManager::~SoundManager()
{
	Shutdown();
}

void SoundManager::lowerMusic()
{
	switch (m_volumeMusic)
	{
	case Volume::Low:
		m_volumeMusic = Volume::Off;
		break;

	case Volume::Medium:
		m_volumeMusic = Volume::Low;
		break;

	case Volume::High:
		m_volumeMusic = Volume::Medium;
		break;
	default:
		break;
	}
	updateVolumeMusicText();
	updateVolumeMusic();
}

void SoundManager::higherMusic()
{
	switch (m_volumeMusic)
	{
	case Volume::Off:
		m_volumeMusic = Volume::Low;
		break;

	case Volume::Low:
		m_volumeMusic = Volume::Medium;
		break;

	case Volume::Medium:
		m_volumeMusic = Volume::High;
		break;
	default:
		break;
	}
	updateVolumeMusicText();
	updateVolumeMusic();
}

void SoundManager::lowerSound()
{
	switch (m_volumeSound)
	{
	case Volume::Low:
		m_volumeSound = Volume::Off;
		break;

	case Volume::Medium:
		m_volumeSound = Volume::Low;
		break;

	case Volume::High:
		m_volumeSound = Volume::Medium;
		break;
	default:
		break;
	}
	updateVolumeSoundText();
	updateVolumeSound();
}

void SoundManager::higherSound()
{
	switch (m_volumeSound)
	{
	case Volume::Off:
		m_volumeSound = Volume::Low;
		break;

	case Volume::Low:
		m_volumeSound = Volume::Medium;
		break;

	case Volume::Medium:
		m_volumeSound = Volume::High;
		break;
	default:
		break;
	}
	updateVolumeSoundText();
	updateVolumeSound();
}


void SoundManager::updateVolumeMusic()
{
	m_musicMenuBuffer->SetVolume(m_volumeValues[m_volumeMusic]);
	m_musicInGameBuffer->SetVolume(m_volumeValues[m_volumeMusic]);
	m_musicVictoryBuffer->SetVolume(m_volumeValues[m_volumeMusic]);
}

void SoundManager::updateVolumeSound()
{
	m_soundBadCoinGetBuffer->SetVolume(m_volumeValues[m_volumeSound]);
	m_soundCoinGetBuffer->SetVolume(m_volumeValues[m_volumeSound]);
	m_soundCoinIsShotBuffer->SetVolume(m_volumeValues[m_volumeSound]);
	m_soundObstacleDisappearsBuffer->SetVolume(m_volumeValues[m_volumeSound]);
	m_soundObstacleExplodesBuffer->SetVolume(m_volumeValues[m_volumeSound]);
	m_soundShipCollideObstacleBuffer->SetVolume(m_volumeValues[m_volumeSound]);
	m_soundShootBuffer->SetVolume(m_volumeValues[m_volumeSound]);
}

void SoundManager::updateVolumeSoundText()
{
	switch (m_volumeSound)
	{
	case Volume::Off:
		m_volumeSoundText = L"Off";
		break;
	case Volume::Low:
		m_volumeSoundText = L"Low";
		break;
	case Volume::Medium:
		m_volumeSoundText = L"Medium";
		break;
	case Volume::High:
		m_volumeSoundText = L"High";
		break;
	}
}

void SoundManager::updateVolumeMusicText()
{
	switch (m_volumeMusic)
	{
	case Volume::Off:
		m_volumeMusicText = L"Off";
		break;
	case Volume::Low:
		m_volumeMusicText = L"Low";
		break;
	case Volume::Medium:
		m_volumeMusicText = L"Medium";
		break;
	case Volume::High:
		m_volumeMusicText = L"High";
		break;
	}
}

std::wstring SoundManager::getVolumeMusicString() const
{
	return m_volumeMusicText;
}

std::wstring SoundManager::getVolumeSoundString() const
{
	return m_volumeSoundText;
}

void SoundManager::Shutdown()
{
	ShutdownWaveFile(&m_musicMenuBuffer);
	ShutdownWaveFile(&m_musicInGameBuffer);
	ShutdownWaveFile(&m_musicVictoryBuffer);

	ShutdownWaveFile(&m_soundBadCoinGetBuffer);
	ShutdownWaveFile(&m_soundCoinGetBuffer);
	ShutdownWaveFile(&m_soundCoinIsShotBuffer);
	ShutdownWaveFile(&m_soundObstacleDisappearsBuffer);
	ShutdownWaveFile(&m_soundObstacleExplodesBuffer);
	ShutdownWaveFile(&m_soundShipCollideObstacleBuffer);
	ShutdownWaveFile(&m_soundShootBuffer);

	// Shutdown the Direct Sound API.
	ShutdownDirectSound();

	return;
}

bool SoundManager::InitializeDirectSound(HWND hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;


	// Initialize the direct sound interface pointer for the default sound device.
	result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level to priority so the format of the primary sound buffer can be modified.
	result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the primary buffer description.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Get control of the primary sound buffer on the default sound device.
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the format of the primary sound bufffer.
	// In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the primary buffer to be the wave format specified.
	result = m_primaryBuffer->SetFormat(&waveFormat);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void SoundManager::ShutdownDirectSound()
{
	// Release the primary sound buffer pointer.
	if (m_primaryBuffer)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = 0;
	}

	// Release the direct sound interface pointer.
	if (m_DirectSound)
	{
		m_DirectSound->Release();
		m_DirectSound = 0;
	}

	return;
}

bool SoundManager::LoadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer)
{
	FILE* filePtr;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	// Open the wave file in binary.
	if (fopen_s(&filePtr, filename, "rb") != 0)
		return false;

	// Read in the wave file header.
	if (fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr) != 1)
		return false;

	// Check that the chunk ID is the RIFF format.
	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
		return false;

	// Check that the file format is the WAVE format.
	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
		return false;

	// Check that the sub chunk ID is the fmt format.
	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
		return false;

	// Check that the audio format is WAVE_FORMAT_PCM.
	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
		return false;

	// Check that the wave file was recorded in stereo format.
	if (waveFileHeader.numChannels != 2)
		return false;

	// Check that the wave file was recorded at a sample rate of 44.1 KHz.
	if (waveFileHeader.sampleRate != 44100)
		return false;

	// Ensure that the wave file was recorded in 16 bit format.
	if (waveFileHeader.bitsPerSample != 16)
		return false;

	// Check for the data chunk header.
	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
		return false;

	// Set the wave format of secondary buffer that this wave file will be loaded onto.
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Create a temporary sound buffer with the specific buffer settings.
	if (FAILED(m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL)))
		return false;

	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	if (FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer)))
		return false;

	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;

	// Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// Create a temporary buffer to hold the wave file data.
	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
		return false;

	// Read in the wave file data into the newly created buffer.
	if (fread(waveData, 1, waveFileHeader.dataSize, filePtr) != waveFileHeader.dataSize)
		return false;

	// Close the file once done reading.
	if (fclose(filePtr) != 0)
		return false;

	// Lock the secondary buffer to write wave data into it.
	if (FAILED((*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0)))
		return false;

	// Copy the wave data into the buffer.
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// Unlock the secondary buffer after the data has been written to it.
	if (FAILED((*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0)))
		return false;

	// Release the wave data since it was copied into the secondary buffer.
	delete[] waveData;
	waveData = 0;

	return true;
}

void SoundManager::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer)
{
	// Release the secondary sound buffer.
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}

	return;
}

bool SoundManager::playMusicMenu()
{
	return PlayWaveMusic(m_musicMenuBuffer);
}

bool SoundManager::playMusicInGame()
{
	return PlayWaveMusic(m_musicInGameBuffer);
}

bool SoundManager::playMusicVictory()
{
	return PlayWaveMusic(m_musicVictoryBuffer);
}

bool SoundManager::playSoundBadCoinGet()
{
	return PlayWaveSound(m_soundBadCoinGetBuffer);
}

bool SoundManager::playSoundCoinGet()
{
	return PlayWaveSound(m_soundCoinGetBuffer);
}

bool SoundManager::playSoundCoinIsShot()
{
	return PlayWaveSound(m_soundCoinIsShotBuffer);
}

bool SoundManager::playSoundObstacleDisappears()
{
	return PlayWaveSound(m_soundObstacleDisappearsBuffer);
}

bool SoundManager::playSoundObstacleExplodes()
{
	return PlayWaveSound(m_soundObstacleExplodesBuffer);
}

bool SoundManager::playSoundShipCollideObstacle()
{
	return PlayWaveSound(m_soundShipCollideObstacleBuffer);
}

bool SoundManager::playSoundShoot()
{
	return PlayWaveSound(m_soundShootBuffer);
}

void SoundManager::stopMusicMenu()
{
	StopWaveFile(m_musicMenuBuffer);
}

void SoundManager::stopMusicInGame()
{
	StopWaveFile(m_musicInGameBuffer);
}

bool SoundManager::PlayWaveMusic(IDirectSoundBuffer8* _soundBuffer)
{
	// Set position at the beginning
	if (FAILED(_soundBuffer->SetCurrentPosition(0)))
		return false;

	// Set volume of the buffer to 100%.
	if (FAILED(_soundBuffer->SetVolume(m_volumeValues[m_volumeMusic])))
		return false;

	// Play the contents
	if (FAILED(_soundBuffer->Play(0, 0, 0)))
		return false;

	return true;
}

bool SoundManager::PlayWaveSound(IDirectSoundBuffer8* _soundBuffer)
{
	// Set position at the beginning
	if (FAILED(_soundBuffer->SetCurrentPosition(0)))
		return false;

	// Set volume of the buffer to 100%.
	if (FAILED(_soundBuffer->SetVolume(m_volumeValues[m_volumeSound])))
		return false;

	// Play the contents
	if (FAILED(_soundBuffer->Play(0, 0, 0)))
		return false;

	return true;
}

void SoundManager::StopWaveFile(IDirectSoundBuffer8* _soundBuffer)
{
	_soundBuffer->Stop();
}