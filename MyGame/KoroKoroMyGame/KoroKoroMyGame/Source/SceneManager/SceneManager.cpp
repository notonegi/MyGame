//
// C_GameSceneManager.cpp
// Author : Masaya Hayashi
//

// ===== CN[h =====
#include "SceneManager.h"
#include "../CrtDebug/CrtDebug.h"
#include "../KeyBoard/Keyboard.h"
#include "../Scene/Title/SceneTitle.h"
#include "../Scene/Main/C_SceneMain.h"
#include "../Scene/Result/C_SceneResult.h"
#include "../Fade/FadeUI.h"

// ===== ΓIo =====
std::unique_ptr<SceneManager> SceneManager::sceneManagerInstancePtr(nullptr);
std::unique_ptr<C_SCENE_BASE> SceneManager::currentScenePtr(nullptr);
std::unique_ptr <FadeUI>	  SceneManager::fadePtr(nullptr);

SceneManager::SceneState	  SceneManager::nextSceneType	 = SceneManager::SceneState::SceneTitle;
SceneManager::SceneState	  SceneManager::currentSceneType = SceneManager::SceneState::SceneTitle;

bool	SceneManager::debugMode		  = false;
INT		SceneManager::frameAdvanceCnt = DebugMoveOnFream;

//
// RXgN^
//
SceneManager::SceneManager()
{
	debugMode		= false;
	frameAdvanceCnt = DebugMoveOnFream;

	// C_SCENE_STAGE_EDIT
	// SCENE_STAGE_EDIT

	// C_SCENE_MAIN
	// SCENE_MAIN

	// C_SCENE_TITLE
	// SCENE_TITLE


	// V[έθϊ»
	currentScenePtr.reset(NEW SceneTitle());
	currentSceneType	= SceneState::SceneTitle;
	nextSceneType		= SceneState::SceneTitle;
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
	if (sceneManagerInstancePtr.get() == nullptr)
	{
		sceneManagerInstancePtr.reset(NEW SceneManager());
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
	// tF[hϊ»
	fadePtr.reset(NEW FadeUI);
	fadePtr->initialize();

	// V[ϊ»
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
			frameAdvanceCnt = DebugMoveOnFream;
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
	fadePtr->updateObject();
}

//
// V[`ζ
//
void SceneManager::draw()
{
	// V[`ζ
	currentScenePtr->draw();
}

//
// tF[hV[`ζ
//
void  SceneManager::drawFade()
{
	if (fadePtr->getFadeState() != FadeUI::FadeType::FadeNone)
	{
		// tF[h`ζ
		fadePtr->drawObject();
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
		/*
	case SceneState::SceneMain:
		delete currentScenePtr.get();
		currentScenePtr = NEW SceneMain();
		break;
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
C_SCENE_BASE* SceneManager::getInstanse()
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