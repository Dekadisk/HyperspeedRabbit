#pragma once

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
#include <vector>
#include <string>

class SoundManager
{
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

public:
	SoundManager(HWND _hwnd);
	~SoundManager();

	enum Volume
	{
		Off = 0,
		Low = 1,
		Medium = 2,
		High = 3
	};

	void lowerMusic();
	void higherMusic();

	void lowerSound();
	void higherSound();

	std::wstring getVolumeMusicString() const;
	std::wstring getVolumeSoundString() const;

	void Shutdown();
	bool PlayWaveSound(IDirectSoundBuffer8* _soundBuffer);
	bool PlayWaveMusic(IDirectSoundBuffer8* _soundBuffer);
	void StopWaveFile(IDirectSoundBuffer8* _soundBuffer);

	bool playMusicMenu();
	bool playMusicInGame();
	bool playMusicVictory();

	bool playSoundBadCoinGet();
	bool playSoundCoinGet();
	bool playSoundCoinIsShot();
	bool playSoundObstacleDisappears(); // obstacle is shot
	bool playSoundObstacleExplodes(); // obstacle is on contact with the ship
	bool playSoundShipCollideObstacle();
	bool playSoundShoot();

	void stopMusicMenu();
	void stopMusicInGame();

private:
	bool InitializeDirectSound(HWND);
	void ShutdownDirectSound();

	bool LoadWaveFile(char*, IDirectSoundBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**);

	void updateVolumeMusic();
	void updateVolumeSound();

	void updateVolumeMusicText();
	void updateVolumeSoundText();

	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;

	// Musics
	IDirectSoundBuffer8* m_musicMenuBuffer;
	IDirectSoundBuffer8* m_musicInGameBuffer;
	IDirectSoundBuffer8* m_musicVictoryBuffer;

	// Sounds
	IDirectSoundBuffer8* m_soundBadCoinGetBuffer;
	IDirectSoundBuffer8* m_soundCoinGetBuffer;
	IDirectSoundBuffer8* m_soundCoinIsShotBuffer;
	IDirectSoundBuffer8* m_soundObstacleDisappearsBuffer;
	IDirectSoundBuffer8* m_soundObstacleExplodesBuffer;
	IDirectSoundBuffer8* m_soundShipCollideObstacleBuffer;
	IDirectSoundBuffer8* m_soundShootBuffer;

	Volume m_volumeMusic;
	Volume m_volumeSound;
	std::wstring m_volumeSoundText;
	std::wstring m_volumeMusicText;
	std::vector<LONG> m_volumeValues = { DSBVOLUME_MIN, -5500, -3500, -2500 };
};