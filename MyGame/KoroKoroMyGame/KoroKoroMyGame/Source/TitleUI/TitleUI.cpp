//
// C_TitleUI.cpp
// ^CgUINX
// Author : MasayaHayashi
//

// ===== CN[h =====
#include "TitleUI.h"
#include "../Application/Application.h"
#include <stdio.h>	// t@CüoÍp

// ===== èE}Nè` =====
#define TEX_FILENAME "data/TEXTURE/Title.png"
#define TITLE_LOGO_velocitySPEED (2.3f)	// ^CgSÚ®Ê

//
// RXgN^
//
TitleUI::TitleUI()
{
	strcpy_s(fileName, TEX_FILENAME);
	vertexBoard.pos					 = D3DXVECTOR3(Application::ScreenCenterX * 0.5f, 0.0f, 0.0f);
	vertexBoard.rot					 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale					 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.size					 = D3DXVECTOR3(921.0f, 177.0f, 0.0f);
	vertexBoard.radAngle = 0.0f;
	posDestBoard				 = D3DXVECTOR3(Application::ScreenCenterX * 0.5f, Application::ScreenCenterY * 0.25f, 0.0f);
	isAlphaBlend			 = false;
	vertexBoard.boardType		 = boardType::Polygon2d;

	texPatternDivideX = 1;
	texPatternDivideY = 1;
	texUV_SizeX = 1.0f / texPatternDivideX;
	texUV_SizeY = 1.0f / texPatternDivideY;
	animPattern = texPatternDivideX * texPatternDivideY;
	intervalChangePattern = 1;

	isUsed = true;
}

//
// fXgN^
//
TitleUI::~TitleUI()
{

}

//
// ú»
//
void TitleUI::initialize()
{
//	ResourceManager *pResourceMgr = GetResourceManager();

	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createTexture(texture, fileName);

	setVtx();
	setTexture();
}

//
// ^CgI¹
//
void TitleUI::finalize()
{
	Board::finalize();
}

//
// XV
//
void TitleUI::update()
{
	static FLOAT fCnt = 0;
	
	// SÚ®
	vertexBoard.pos.y += TITLE_LOGO_velocitySPEED;

	// I¹»è
	if (vertexBoard.pos.y > posDestBoard.y)
		vertexBoard.pos.y = posDestBoard.y;

	// ¡Ú®
	fCnt += 0.03f;
	vertexBoard.pos.x = sinf(fCnt) * 11.0f + Application::ScreenCenterX;
	
	setVtx();
	setTexture();
}

//
// `æ
//
void TitleUI::draw()
{
	Board::draw();
}