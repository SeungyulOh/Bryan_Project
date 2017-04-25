#include "Trail_Texture.h"

using namespace Engine;

Engine::CTrail_Texture::CTrail_Texture(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CVIBuffer(pGraphicDev)
{

}
Engine::CTrail_Texture::CTrail_Texture(const CTrail_Texture& rhs)
: Engine::CVIBuffer(rhs)
{

}

Engine::CTrail_Texture::~CTrail_Texture(void)
{

}

HRESULT Engine::CTrail_Texture::Ready_Buffer(void)
{

	m_dwVtxCnt = 40;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxFVF = D3DFVF_VTXTEX;

	m_dwTriCnt = (m_dwVtxCnt/2 - 1)  * 2;
	m_dwIdxSize = sizeof(INDEX16) * m_dwTriCnt;
	m_IndexFmt = D3DFMT_INDEX16;

	if(FAILED(Engine::CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	// 할당된 메모리에 접근을 하자.
	VTXTEX*				pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_int iTotalY = (m_dwVtxCnt-1) / 2;
	for(_uint i =0; i<m_dwVtxCnt ; ++i)
	{
		_int iX = i % 2;
		_int iY = i / 2;
		
		pVertex[i].vPos = _vec3(-0.5f + iX, 1.f - (1/(float)iTotalY) * (float)iY , 0.f);
		pVertex[i].vTexUV = _vec2(0.f + iX, 0.f + ((float)iY / (float)iTotalY));
	}

	/*pVertex[0].vPos = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec3(0.5f, 0.5f, 0.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[2].vTexUV = _vec2(0.f, 1.f);

	pVertex[3].vPos = _vec3(0.5f, -0.5f, 0.f);
	pVertex[3].vTexUV = _vec2(1.f, 1.f);*/
	

	m_pVB->Unlock();

	INDEX16*			pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for(_uint i =0; i<m_dwTriCnt; i = i+2)
	{
		int iIndex = 2 * (i/2);
		pIndex[i]._0 = iIndex;
		pIndex[i]._1 = iIndex+3;
		pIndex[i]._2 = iIndex+2;

		pIndex[i+1]._0 = iIndex;
		pIndex[i+1]._1 = iIndex+1;
		pIndex[i+1]._2 = iIndex+3;
	}

	
	

	m_pIB->Unlock();
	
	return S_OK;
}

void Engine::CTrail_Texture::Render_Buffer(void)
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

CTrail_Texture* Engine::CTrail_Texture::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTrail_Texture*		pInstance = new CTrail_Texture(pGraphicDev);

	if(FAILED(pInstance->Ready_Buffer()))
	{
		MSG_BOX("CRect_Texture Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* Engine::CTrail_Texture::Clone(void)
{
	return new CTrail_Texture(*this);
}

void Engine::CTrail_Texture::Free(void)
{
	Engine::CVIBuffer::Free();

	

}

