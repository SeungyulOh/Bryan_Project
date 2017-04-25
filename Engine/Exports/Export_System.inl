////////////////Time MGr//////////////////////////////////////

using namespace Engine;

HRESULT Engine::Ready_TimeMgr(const _tchar* pTagTimer)
{
	return Engine::CTimer_Mgr::GetInstance()->Ready_TimerMgr(pTagTimer);
}
void Engine::SetUp_TimeDelta(const _tchar* pTagTimer)
{
	Engine::CTimer_Mgr::GetInstance()->SetUp_TimeDelta(pTagTimer);
}
_float Engine::Get_TimeDelta(const _tchar* pTagTimer)
{
	return Engine::CTimer_Mgr::GetInstance()->Get_TimeDelta(pTagTimer);
}


////////////////////Frame Mgr//////////////////////////////////
HRESULT Engine::Ready_FrameMgr(const _tchar* pTagFrame, const _float& fCallPerSec)
{
	return Engine::CFrame_Mgr::GetInstance()->Ready_FrameMgr(pTagFrame,fCallPerSec);
}
bool Engine::Permit_Call(const _tchar* pTagFrame, const _float& fTimeDelta)
{
	return Engine::CFrame_Mgr::GetInstance()->Permit_Call(pTagFrame,fTimeDelta);
}



//For_GraphicDev
//Getter//
LPDIRECT3DDEVICE9 Engine::Get_GraphicDev(void)
{
	return Engine::CGraphicDev::GetInstance()->Get_GraphicDev();
}
LPD3DXLINE Engine::GetLine(void)
{
	return Engine::CGraphicDev::GetInstance()->GetLine();
}
//Setter//
//General//
HRESULT Engine::Ready_GraphicDev(HWND hWnd, CGraphicDev::WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, Engine::CGraphicDev** ppGraphicDev)
{
	return Engine::CGraphicDev::GetInstance()->Ready_GraphicDev(hWnd,eMode,iSizeX,iSizeY,ppGraphicDev);
}
void Engine::Render_Begin(D3DXCOLOR Color)
{
	Engine::CGraphicDev::GetInstance()->Render_Begin(Color);
}
void Engine::Render_End(void)
{
	Engine::CGraphicDev::GetInstance()->Render_End();
}

void Engine::Set_RenderState(D3DRENDERSTATETYPE eType, _ulong dwFlag)
{
	Engine::CGraphicDev::GetInstance()->Set_RenderState(eType,dwFlag);
}

//For_FontMgr
//Getter//
//Setter//
//General//
HRESULT Engine::Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFontTag
						  , const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	return Engine::CFont_Manager::GetInstance()->Ready_Font(pGraphicDev,pFontTag,pFontType,iWidth,iHeight,iWeight);
}
void Engine::Render_Font(const _tchar* pFontTag, const _tchar* pString, const _vec2* pPosition, D3DXCOLOR Color)
{
	Engine::CFont_Manager::GetInstance()->Render_Font(pFontTag,pString,pPosition,Color);
}


//For_InputDevice
//Getter//
_byte Engine::Get_DIKeyState(_ubyte byKeyID)
{
	return Engine::CInput_Device::GetInstance()->Get_DIKeyState(byKeyID);
}
_byte Engine::Get_DIMouseState(CInput_Device::MOUSEKEYSTATE byMouseID)
{
	return Engine::CInput_Device::GetInstance()->Get_DIMouseState(byMouseID);
}
_long Engine::Get_DIMouseMove(CInput_Device::MOUSEMOVESTATE byMouseMoveState)
{
	return Engine::CInput_Device::GetInstance()->Get_DIMouseMove(byMouseMoveState);
}
//Setter//
//General//
HRESULT Engine::Ready_InputDevice(HINSTANCE hInst, HWND hWnd)
{
	return Engine::CInput_Device::GetInstance()->Ready_InputDevice(hInst,hWnd);
}
void Engine::SetUp_InputState(void)
{
	Engine::CInput_Device::GetInstance()->SetUp_InputState();
}



///////////Release////////////////////
void Engine::Release_System()
{
	if(0 != Engine::CTimer_Mgr::GetInstance()->DestroyInstance())
		MSG_BOX("CTimer_Mgr Destroy Failed");

	if(0 != Engine::CFrame_Mgr::GetInstance()->DestroyInstance())
		MSG_BOX("CFrame_Mgr Destroy Failed");

	if(0 != Engine::CFont_Manager::GetInstance()->DestroyInstance())
		MSG_BOX("CFont_Manager Destroy Failed");

	if(0 != Engine::CInput_Device::GetInstance()->DestroyInstance())
		MSG_BOX("CInput_Device Destroy Failed");
	
	if(0 != Engine::CGraphicDev::GetInstance()->DestroyInstance())
		MSG_BOX("CGraphicDev Destroy Failed");
}