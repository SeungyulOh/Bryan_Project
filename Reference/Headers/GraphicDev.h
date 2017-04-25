#ifndef Engine_GraphicDev_h__
#define Engine_GraphicDev_h__

#include "Engine_Defines.h"
#include "Base.h"

// 용도 : 내 컴퓨터에 있는 그래픽카드장치를 초기화한다.
// 1. 그래픽카드의 정보를 조사하낟
// 2. 조사한 내용을 바타응로 장치를 사용할 수 있도록 준비시키낟.

BEGIN(Engine)

class ENGINE_DLL CGraphicDev : public CBase
{
	DECLARE_SINGLETON(CGraphicDev)
public:
	enum WINMODE {MODE_FULL, MODE_WIN};	
private:
	explicit CGraphicDev(void);
	virtual ~CGraphicDev(void);
public:
	LPDIRECT3DDEVICE9 Get_GraphicDev(void) const {
		return m_pGraphicDev;}
	void Set_RenderState(D3DRENDERSTATETYPE eType, _ulong dwFlag) {
		m_pGraphicDev->SetRenderState(eType, dwFlag);}
	inline LPD3DXLINE GetLine(void){return m_pLine;}
public:
	HRESULT Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, Engine::CGraphicDev** ppGraphicDev);
	void Render_Begin(D3DXCOLOR Color);
	void Render_End(void);
private:
	// m_pGrpahicDev장치를 생성해주는 역활
	// 장치의 성능을 조사하는 역활
	LPDIRECT3D9						m_pSDK;

	//장치를 컨트롤 할 수 있는(장치에 명령을 내릴 수 있는) 객체를 제공해준다.	
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	LPD3DXLINE m_pLine;
public:
	inline virtual void Free(void);
};

END

#endif // GraphicDev_h__
