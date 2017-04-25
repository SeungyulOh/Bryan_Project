#ifndef Target_h__
#define Target_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CTarget : public CBase
{
private:
	explicit CTarget(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTarget(void);
public:
	HRESULT Ready_Target(const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format, D3DXCOLOR Color);
	HRESULT Ready_DebugBuffer(const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
	void Render_DebugBuffer(void);
	void Clear_RenderTarget(void);
	void SetUp_OnGraphicDev(const _uint& iIndex);
	void Release_OnGraphicDev(const _uint& iIndex);
	void ReleaseOldSurface(void);
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName);
	inline LPDIRECT3DSURFACE9 GetTargetSurface(void){return m_pTargetSurface;}
private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	LPDIRECT3DTEXTURE9			m_pTargetTexture;

	LPDIRECT3DSURFACE9			m_pOldSurface;
	LPDIRECT3DSURFACE9			m_pOldCopySurface;
	LPDIRECT3DSURFACE9			m_pTargetSurface;
private:
	D3DXCOLOR					m_ClearColor;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

public:
	static CTarget* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format, D3DXCOLOR Color);
private:
	virtual void Free(void);
};


END

#endif // Target_h__
