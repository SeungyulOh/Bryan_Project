#ifndef Export_System_h__
#define Export_System_h__

#include "Engine_Defines.h"
#include "Timer_Mgr.h"
#include "Frame_Mgr.h"
#include "GraphicDev.h"
#include "Font_Manager.h"
#include "Input_Device.h"

namespace Engine
{
	//For.TimeMgr
	//Getter//
	inline _float Get_TimeDelta(const _tchar* pTagTimer);
	//Setter//
	//General//
	inline HRESULT Ready_TimeMgr(const _tchar* pTagTimer);
	inline void SetUp_TimeDelta(const _tchar* pTagTimer);
	
	//For_FrameMgr
	//Getter//
	//Setter//
	//General//
	inline HRESULT Ready_FrameMgr(const _tchar* pTagFrame, const _float& fCallPerSec);
	inline bool Permit_Call(const _tchar* pTagFrame, const _float& fTimeDelta);

	//For_FontMgr
	//Getter//
	//Setter//
	//General//
	inline HRESULT Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFontTag
		, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);
	inline void Render_Font(const _tchar* pFontTag, const _tchar* pString, const _vec2* pPosition, D3DXCOLOR Color);

	//For_InputDevice
	//Getter//
	inline _byte Get_DIKeyState(_ubyte byKeyID);
	inline _byte Get_DIMouseState(CInput_Device::MOUSEKEYSTATE byMouseID);
	inline _long Get_DIMouseMove(CInput_Device::MOUSEMOVESTATE byMouseMoveState);
	//Setter//
	//General//
	inline HRESULT Ready_InputDevice(HINSTANCE hInst, HWND hWnd);
	inline void SetUp_InputState(void);


	//For_GraphicDev
	//Getter//
	inline LPDIRECT3DDEVICE9 Get_GraphicDev(void);
	inline LPD3DXLINE GetLine(void);
	//Setter//
	//General//
	inline HRESULT Ready_GraphicDev(HWND hWnd, CGraphicDev::WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, Engine::CGraphicDev** ppGraphicDev = NULL);
	inline void Render_Begin(D3DXCOLOR Color);
	inline void Render_End(void);
	inline void Set_RenderState(D3DRENDERSTATETYPE eType, _ulong dwFlag);


	//Release
	inline void Release_System();

	#include "Export_System.inl"
}



#endif // Export_System_h__
