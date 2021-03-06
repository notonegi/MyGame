//
// StarUI.h
// UIpÌ¯æ
// Author : Masaya Hayashi
//

// ===== ½dCN[hh~ =====
#ifndef STAR_UI_H
#define STAR_UI_H

// ===== CN[h =====
#include "../Board/Board.h"

// ===== NXÌOûè` =====

// ===== NXè` =====
class StarUI final : public Board
{
public:
	StarUI();
	StarUI(size_t indexNumber);
	~StarUI();

	void initialize();
	void finalize();
	void update();
	void draw();

private:
	bool awake = false;

	const std::string TextureFilePass = "Data/Texture/GoldStar.png";
	const FLOAT MoveSpeed = 1.5f;

};

#endif