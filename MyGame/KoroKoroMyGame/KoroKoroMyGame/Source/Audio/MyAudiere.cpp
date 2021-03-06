//
// C_Audio.h
// I[fBIÖA
// Author : Masaya Hayashi
//

// ===== CN[h =====
#include "MyAudiere.h"
#include <string>
#include <array>

// ===== ÃIoÏ =====
std::unordered_map <UINT, audiere::OutputStreamPtr> MyAudiere::bgm;
std::unordered_map <UINT, audiere::SoundEffectPtr>  MyAudiere::se;

//
// RXgN^
//
MyAudiere::MyAudiere()
{
	audioDeviceObj = audiere::OpenDevice();
	
	// áO
	if (!audioDeviceObj)
	{
		return;
	}

	// BGMpXÝè
	std::array<AUDIO_DATA, MaxSound> bgmFilePass =
	{
		"data/BGM/Title.wav"  ,
		"data/BGM/Edit.wav "  ,
		"data/BGM/Main.wav "  ,
	};

	// BGMú»
	for (INT i = 0; i < MaxSound; i++)
	{
		if (!bgmFilePass[i].filePassPtr)
		{
			break;
		}

		std::pair<UINT, audiere::OutputStreamPtr> setPair = std::make_pair(i, audiere::OpenSound(audioDeviceObj.get(), bgmFilePass[i].filePassPtr));
		bgm.insert(setPair);
	}

	// SEpXÝè
	std::array<AUDIO_DATA, MaxSound> seFilePass =
	{
		"data/SE/Hokou.wav"		,	// às
		"data/SE/Jump.wav"		,	// Wv
	};

	// SEú»
	for (INT i = 0; i < MaxSound; i++)
	{
		if (!seFilePass[i].filePassPtr)
		{
			break;
		}

		std::pair<UINT, audiere::SoundEffectPtr> setPair = std::make_pair(i, audiere::OpenSoundEffect(audioDeviceObj.get(), seFilePass[i].filePassPtr, audiere::SoundEffectType::MULTIPLE));
		se.insert(setPair);
	}
}

//
// fXgN^
//
MyAudiere::~MyAudiere()
{
	bgm.clear();
	se.clear();
}

//
// BGMæ¾
//
audiere::OutputStreamPtr MyAudiere::getBgm(UINT index)
{
	return bgm[index];
}

//
// SEæ¾
//
audiere::SoundEffectPtr MyAudiere::getSe(UINT index)
{
	return se[index];
}
