//����������������������������������������������������������������
// CrtDebug.h
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef CRT_DEBUG_H
#define CRT_DEBUG_H

// ===== �C���N���[�h�� =====
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// ===== �萔�E�}�N����` =====
#if defined(DEBUG) || defined(_DEBUG)
#define NEW  NEW(_NORMAL_BLOCK, __FILE__, __LINE__)  // NEW �ɂ�郁�������[�N���o
#else
#define NEW  NEW									 // �����[�X�̏ꍇ�ʏ�̃������m��
#endif

#endif