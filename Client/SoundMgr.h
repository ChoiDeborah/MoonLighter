#pragma once

#include "Include.h"

class CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr)


public:
	CSoundMgr();
	~CSoundMgr();

public:
	enum CHANNELID { BGM, PLAYER, ROCK, PUNCH, GOLEM, WAVE, WEAPON, GUARDIAN, EFFECT, UI, MAXCHANNEL };
public:
	void Initialize();
	void Update();
	void Release();

public:
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID);
	void PlayBGM(TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();
private:
	void LoadSoundFile();
private:
	// ���� ���ҽ� ���� ���� �ִ� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannel[MAXCHANNEL];
	//���� ä�� ��ü�� ��ġ�� ������ ��ü 
	FMOD_SYSTEM* m_pSystem;
};