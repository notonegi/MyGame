//
// Xinput.h
// XINPUTüÍ
// Author : Masaya Hayashi
//

// ===== ½dCN[hh~ =====
#ifndef XINPUT_H
#define XINPUT_H

// ===== CN[h ======
#include "../Linklib.h"
#include <Windows.h>
#include <Xinput.h>
#include <memory>

// ===== NXè` =====
class Xinput final
{
public:
	static void update();

	// {^óÔæ¾
	static bool getButtonPress(WORD uButtonCode);
	static bool getButtonTriger(WORD uButtonCode);
	static bool getButtonRelease(WORD uButtonCode);
		 
	static bool getButton_A_Press();
	static bool getButton_A_Triger();
	static bool getButton_A_Release();
	 	 
	static bool getButton_B_Press();
	static bool getButton_B_Triger();
	static bool getButton_B_Release();
	 	 
	static bool getButton_X_Press();
	static bool getButton_X_Triger();
	static bool getButton_X_Release();
	 	 
	static bool getButton_Y_Press();
	static bool getButton_Y_Triger();
	static bool getButton_Y_Release();
	 	 
	static bool getButton_START_Press();
	static bool getButton_START_Triger();
	static bool getButton_START_Release();
	 	 
	static bool getButton_BACK_Press();
	static bool getButton_BACK_Triger();
	static bool getButton_BACK_Release();
	 	 
	static bool getButton_LB_Press();
	static bool getButton_LB_Triger();
	static bool getButton_LB_Release();
	 	 
	static bool getButton_RB_Press();
	static bool getButton_RB_Triger();
	static bool getButton_RB_Release();
	 
	static bool getButton_LeftThumb_Press();
	static bool getButton_LeftThumb_Triger();
	static bool getButton_LeftThumb_Release();
 	 
	static bool getButton_RightThumb_Press();
	static bool getButton_RightThumb_Triger();
	static bool getButton_RightThumb_Release();
 
	// \L[(DPad){^»è
	static bool getButton_DpadUp_Press();
	static bool getButton_DpadUp_Triger();
	static bool getButton_DpadUp_Release();
	
	static bool getButton_DpadDown_Press();
	static bool getButton_DpadDown_Triger();
	static bool getButton_DpadDown_Release();

	static bool getButton_DpadLeft_Press();
	static bool getButton_DpadLeft_Triger();
	static bool getButton_DpadLeft_Release();
 	 
	static bool getButton_DpadRight_Press();
	static bool getButton_DpadRight_Triger();
	static bool getButton_DpadRight_Release();
 
	static bool isRightLS();
	static bool isLeftLS();

	// fW^üÍlæ¾
	static BYTE  getLeftTrigger();	// 0`255 LT
	static BYTE  getRightTrigger();	// 0`255 RT
	static SHORT getThumbLX();		// -32768 ` 32767@¶XeBbN¡²
	static SHORT getThumbLY();		// -32768 ` 32767  ¶XeBbNc²
	static SHORT getThumbRX();		// -32768 ` 32767  EXeBbN¡²
	static SHORT getThumbRY();		// -32768 ` 32767  ¶XeBbNc²
	 
	// U®
	static void vibration_Left_Start(UINT nVibLevel);		// ¶[^[U®
	static void vibration_Right_Start(UINT nVibLevel);		// E[^[U®
	static void vibration_Left_Stop();
	static void vibration_Right_Stop();
	static void vibration_Start(UINT nVibLevel);						// ¼ûÌ[^[U®
	static void vibration_Stop();										// ¼ûâ~
	static void vibration_AnyTime(UINT uVibTime, USHORT nVibLevel);	// CÓÌÔU®

	static Xinput& getInstance();
	
protected:

private:
 	static XINPUT_STATE			state;
	static XINPUT_VIBRATION		vibration;
	static XINPUT_GAMEPAD		WorkStatePrev;		// 1t[OÌóÔÞðp
	static XINPUT_GAMEPAD		WorkStateTrigger;		// Q[pbhÌóÔðó¯æé[N
	static XINPUT_GAMEPAD		WorkStateRelease;		// Q[pbhÌóÔðó¯æé[N
	static XINPUT_GAMEPAD		WorkStateRepeat;		// Q[pbhÌóÔðó¯æé[N
	static bool bConnected;	  		// Ú±©
	static bool bCurrentViberation;	// U®©
	static INT  nVibCnt;			// U®pJE^
	

	static constexpr INT MaxControllers = 4;
	static constexpr INT MaxVibration = 65535;

	static Xinput *pInstance;	// CX^X
	Xinput();
	~Xinput();
};

#endif