//
// C_WhatStageSaveUI.h
// Z[uXe[WIðpUI
// Author : Masaya Hayashi
//

// ===== ½dCN[hh~ =====
#ifndef WHAT_STAGE_SAVE_UI_H
#define WHAT_STAGE_SAVE_UI_H

// ===== CN[h =====
#include "../Board/Board.h"

// ===== NXÌOûè` =====

// ===== NXè` =====
class WhatStageSaveUi final : public Board
{
public:
	WhatStageSaveUi();		// RXgN^
	~WhatStageSaveUi();		// fXgN^

	void initialize();
	void finalize();
	void update();
	void draw();

private:
	const std::string TexturePass = "data/TEXTURE/WhatStageLoad.png";
	FLOAT moveCnt = 0;
};

#endif