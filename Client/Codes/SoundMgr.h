#pragma once

#include "Defines.h"

class CSoundMgr
{
public:
	enum SOUNDTYPE {HAGIE_ATK1,HAGIE_ATK2,HAGIE_ATK3,HAGIE_ATK4,
		HWEAPON1,HWEAPON2,HWEAPON3,HWEAPON4,
		HAGIE_ATK2STRONG,BUFF,
		BOSS1,BOSS2,BOSS3,BOSS4,SOUND_END};

private:
	FMOD_SYSTEM*	m_pSystem;	//fmod �ý��� ����� ������
	FMOD_CHANNEL*	m_pEffect[SOUND_END];	//fmod ä�� ����� ������
	FMOD_CHANNEL*	m_pBgm;		//fmod ä�� ����� ������
	
	unsigned int	m_iVersion;

	//������� ������ map �����̳�...
	map<TCHAR*, FMOD_SOUND*>		m_mapSound;
public:
	map<TCHAR*, FMOD_SOUND*>*	GetSoundMap(void)
	{
		return &m_mapSound;
	}
public:
	void	Initialize(void);
	void	LoadSoundFile();
	void	PlaySound(TCHAR* pSoundKey, _uint SoundIdx);
	void	PlayBGM(TCHAR*	pSoundKey);
	void	StopBGM(TCHAR*	pSoundKey);
	void	StopSound(TCHAR* pSoundKey, _uint SoundIdx);
	void	StopSoundAll(void);
	void	Release(void);
private:
	static CSoundMgr*	m_pInstance;
public:
	static CSoundMgr*	GetInstance(void)
	{
		if(m_pInstance == NULL)
			m_pInstance = new CSoundMgr;

		return m_pInstance;
	}

	void	DestroyInstance(void)
	{
		if(m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}
public:
	CSoundMgr(void);
	~CSoundMgr(void);
};
