#include "Target.h"

using namespace Engine;

Engine::CTarget::CTarget(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
, m_pTargetTexture(NULL)
, m_pTargetSurface(NULL)
, m_pOldSurface(NULL)
, m_pOldCopySurface(NULL)
, m_pVB(NULL)
, m_pIB(NULL)
{
	m_pGraphicDev->AddRef();	
}

Engine::CTarget::~CTarget(void)
{

}

HRESULT Engine::CTarget::Ready_Target(const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format, D3DXCOLOR Color)
{
	// 텍스쳐를 생성한다.
	if(FAILED(D3DXCreateTexture(m_pGraphicDev, iSizeX, iSizeY, 1, D3DUSAGE_RENDERTARGET, Format, D3DPOOL_DEFAULT, &m_pTargetTexture)))
		return E_FAIL;

	// 렌더타겟을 뽑는다.
	m_pTargetTexture->GetSurfaceLevel(0, &m_pTargetSurface);

	Engine::Safe_Release(m_pTargetTexture);

	m_ClearColor = Color;

	return S_OK;
}

HRESULT Engine::CTarget::Ready_DebugBuffer(const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY)
{
	if(FAILED(m_pGraphicDev->CreateVertexBuffer(sizeof(BRUSH) * 4, 0, D3DFVF_BRUSH, D3DPOOL_MANAGED, &m_pVB, NULL)))
		return E_FAIL;

	// 할당된 메모리에 접근을 하자.
	BRUSH*				pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec4(fX - 0.5f, fY - 0.5f, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec4(fX + fSizeX - 0.5f, fY - 0.5f, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(fX + fSizeX - 0.5f, fY + fSizeY - 0.5f, 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(fX - 0.5f, fY + fSizeY - 0.5f, 0.f, 1.f);
	pVertex[3].vTexUV = _vec2(0.0f, 1.0f);

	m_pVB->Unlock();

	if(FAILED(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL)))
		return E_FAIL;

	INDEX16*			pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();	

	return S_OK;

}

void Engine::CTarget::Render_DebugBuffer(void)
{
	m_pGraphicDev->SetTexture(0, m_pTargetTexture);
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(BRUSH));
	m_pGraphicDev->SetFVF(D3DFVF_BRUSH);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

void Engine::CTarget::Clear_RenderTarget(void)
{
	m_pGraphicDev->GetRenderTarget(0, &m_pOldSurface);
	m_pGraphicDev->GetRenderTarget(1, &m_pOldCopySurface);

	m_pGraphicDev->SetRenderTarget(0, m_pTargetSurface);
	m_pGraphicDev->SetRenderTarget(1, NULL);

	m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET, m_ClearColor, 1.f, 0);

	m_pGraphicDev->SetRenderTarget(0, m_pOldSurface);
	m_pGraphicDev->SetRenderTarget(1, m_pOldCopySurface);

	Engine::Safe_Release(m_pOldSurface);
	Engine::Safe_Release(m_pOldCopySurface);
}

void Engine::CTarget::SetUp_OnGraphicDev(const _uint& iIndex)
{
	m_pGraphicDev->GetRenderTarget(iIndex, &m_pOldSurface);

	m_pGraphicDev->SetRenderTarget(iIndex, m_pTargetSurface);	
}

void Engine::CTarget::Release_OnGraphicDev(const _uint& iIndex)
{
	m_pGraphicDev->SetRenderTarget(iIndex, m_pOldSurface);	

	Engine::Safe_Release(m_pOldSurface);
}

HRESULT Engine::CTarget::SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName)
{
	pEffect->SetTexture(pConstantName, m_pTargetTexture);

	return S_OK;
}

CTarget* Engine::CTarget::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format, D3DXCOLOR Color)
{
	CTarget*	pInstance = new CTarget(pGraphicDev);

	if(FAILED(pInstance->Ready_Target(iSizeX, iSizeY, Format, Color)))
	{
		MSG_BOX("CTarget Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CTarget::Free(void)
{
	Engine::Safe_Release(m_pGraphicDev);

	_ulong dwRefCnt = Engine::Safe_Release(m_pTargetTexture);
	

	dwRefCnt = Engine::Safe_Release(m_pVB);
	dwRefCnt = Engine::Safe_Release(m_pIB);


}

void Engine::CTarget::ReleaseOldSurface(void)
{
	m_pOldSurface->Release();
}

