//
// SceneManager.cpp
// V[Η
// Author : Masaya Hayashi
//

// ===== CN[h =====
#include "SceneManager.h"
#include "../CrtDebug/CrtDebug.h"
#include "../KeyBoard/Keyboard.h"
#include "../Scene/Title/SceneTitle.h"
#include "../Scene/Main/SceneMain.h"
#include "../Scene/Select/SceneSelect.h"
#include "../Scene/Result/C_SceneResult.h"
#include "../Scene/StageEdit/SceneStageEdit.h"
#include "../Fade/FadeUI.h"

// ===== ΓIo =====
std::unique_ptr<SceneManager>	SceneManager::SceneManagerInstancePtr(nullptr);
std::unique_ptr<SceneBase>		SceneManager::currentScenePtr(nullptr);
std::unique_ptr<FadeUI>			SceneManager::fadePtr(nullptr);

SceneManager::SceneState	  SceneManager::nextSceneType	 = SceneManager::SceneState::SceneTitle;
SceneManager::SceneState	  SceneManager::currentSceneType = SceneManager::SceneState::SceneTitle;

bool	SceneManager::debugMode		  = false;
INT		SceneManager::frameAdvanceCnt = DebugvelocityOnFream;

//
// RXgN^
//
SceneManager::SceneManager()
{
	debugMode		= false;
	frameAdvanceCnt = DebugvelocityOnFream;

	loadSettingFile();
	makeStartScene();

	fadePtr.reset(NEW FadeUI());
}

//
// fXgN^
//
SceneManager::~SceneManager()
{
	fadePtr->finalize();
}

//
// CX^XΆ¬
//
bool SceneManager::create()
{
	if (SceneManagerInstancePtr.get() == nullptr)
	{
		SceneManagerInstancePtr.reset(NEW SceneManager());
		return true;
	}
	else
	{
		return false;
	}
}

//
// V[ϊ»
//
void SceneManager::initialize()
{
	fadePtr->initialize();

	currentScenePtr->initialize();
}

//
// V[IΉ
//
void SceneManager::finalize()
{
	// V[γ
	currentScenePtr->finalize();
}

//
// V[XV
//
void SceneManager::update()
{
	// fobO[hΨΦ¦
	if (Keyboard::getTrigger(DIK_V))
	{
		debugMode = !debugMode;
	}

	// fobO[h»θ
	if (debugMode)
	{
		if (Keyboard::getPress(DIK_SPACE))
		{
			frameAdvanceCnt--;
		}

		if (frameAdvanceCnt <= 0)
		{
			frameAdvanceCnt = DebugvelocityOnFream;
			currentScenePtr->update();
		}
	}
	else
	{
		// V[XV
		currentScenePtr->update();
	}
}

//
// tF[hXV
//
void SceneManager::updateFade()
{
	fadePtr->update();
}

//
// V[`ζ
//
void SceneManager::draw()
{
	currentScenePtr->draw();
}

//
// tF[hV[`ζ
//
void  SceneManager::drawFade()
{
	if (fadePtr->getFadeState() != FadeUI::FadeType::FadeNone)
	{
		fadePtr->draw();
	}
}

//
// V[Zbg
//
void SceneManager::changeScene(SceneState Scene)
{
	switch (Scene)
	{
	case SceneState::SceneTitle:
		currentScenePtr.reset(NEW SceneTitle());
		break;
	case SceneState::SceneMain:
		currentScenePtr.reset(NEW SceneMain());
		break;
	case SceneState::SceneSelect:
		currentScenePtr.reset(NEW SceneSelect());
		break;
	case SceneState::SceneStageEdit:
		currentScenePtr.reset(NEW SceneStageEdit());
		break;
	/*
	case SceneState::SceneResult:
		delete currentScenePtr.get();
		currentScenePtr = NEW SceneResult();
		break;
	case SceneState::SceneStageEdit:
		delete currentScenePtr.get();
		currentScenePtr = NEW SceneStageEdit();
		break;
		*/
	// αO
	default:
		break;
	}
	
}

//
// »έΜV[ΜCX^XζΎ
//
SceneBase* SceneManager::getInstanse()
{
	return currentScenePtr.get();
}

//
// »έΜV[enumζΎ
//
const SceneManager::SceneState SceneManager::getCurrentSceneType()
{
	return currentSceneType;
}

//
// O©ηΜV[πZbg([hV[p)
//
void SceneManager::setCurrentScene(SceneState setStateType)
{
	currentSceneType = setStateType;
}

//
// ΜV[πZbg
//
void SceneManager::setNextScene(SceneState setNextSceneType)
{
	nextSceneType = setNextSceneType;
	fadePtr->setFade(FadeUI::FadeType::FadeOut);
}

//
// ΜV[ζΎ([hV[p)
//
const SceneManager::SceneState SceneManager::getNextScene()
{
	return nextSceneType;
}

//
// tF[hζΎ
//
const FadeUI* SceneManager::getFade() const
{
	return fadePtr.get();
}

//
// init@C[h
//
void SceneManager::loadSettingFile()
{
	DWORD  ret;
	ret = GetPrivateProfileString("Scene", "StartScene", "΅ΑΟ’", StartScene, sizeof(StartScene),"Data/debugSetting.ini");

	if (!ret)
	{
		MessageBox(nullptr, TEXT("Error"), TEXT("V[ΗέέG["), MB_ICONERROR);
	}
}

//
// ϊV[έθ
//
void SceneManager::makeStartScene()
{
	switch (sceneTypeMap[StartScene])
	{
	case SceneState::SceneTitle :
		currentScenePtr.reset(NEW SceneTitle());
		break;
	case SceneState::SceneMain	:
		currentScenePtr.reset(NEW SceneMain());
		break;
	case SceneState::SceneStageEdit: 
 		currentScenePtr.reset(NEW SceneStageEdit());
		break;
	case SceneState::SceneSelect:
		currentScenePtr.reset(NEW SceneSelect());
		break;
	case SceneState::SceneResult:


		break;
	default:
		break;
	}

	currentSceneType = sceneTypeMap[StartScene];
	nextSceneType = sceneTypeMap[StartScene];
}