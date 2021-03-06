//
// C_TitleSelectUI.cpp
// ^CgUINX
// Author : MasayaHayashi
//

// ===== CN[h =====
#include "TitleSelectUI.h"
#include "../Application/Application.h"
#include <stdio.h>

// ===== èE}Nè` =====

//
// RXgN^
//
TitleSelectUI::TitleSelectUI()
{
	strcpy_s(fileName, TextureFilePass.c_str());
	vertexBoard.pos				 = D3DXVECTOR3(Application::ScreenCenterX * 0.5f, Application::ScreenHeight, 0.0f);
	vertexBoard.rotDeg			 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale			 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.size			 = D3DXVECTOR3(640.0f, 95.0f, 0.0f);
	vertexBoard.radAngle = 0.0f;
	posDestBoard				 = D3DXVECTOR3(Application::ScreenCenterX * 0.5f, Application::ScreenCenterY * 1.25f, 0.0f);
	isAlphaBlend				 = false;
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
TitleSelectUI::~TitleSelectUI()
{

}

//
// ú»
//
void TitleSelectUI::initialize()
{
	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createTexture(texture, fileName);

	setVtx();
	setTexture();
}

//
// ^CgI¹
//
void TitleSelectUI::finalize()
{
	Board::finalize();
}

//
// XV
//
void TitleSelectUI::update()
{
	static FLOAT cnt = 0;
	
	// SÚ®
	vertexBoard.pos.y += MoveSpeed;

	// I¹»è
	if (vertexBoard.pos.y > posDestBoard.y)
	{
		vertexBoard.pos.y = posDestBoard.y;
	}

	// ¡Ú®
	cnt += 0.03f;
	vertexBoard.pos.x = sinf(cnt) * 11.0f + Application::ScreenCenterX;
	
	setVtx();
	setTexture();
}

//
// `æ
//
void TitleSelectUI::draw()
{
	Board::draw();
}