#include "Cube_Tex.h"

using namespace Engine;

Engine::CCube_Tex::CCube_Tex(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CVIBuffer(pGraphicDev)
{
}

Engine::CCube_Tex::CCube_Tex(const CCube_Tex& rhs)
:Engine::CVIBuffer(rhs)
{
}

Engine::CCube_Tex::~CCube_Tex(void)
{

}

HRESULT Engine::CCube_Tex::Ready_Buffer(void)
{
	m_dwVtxCnt = 8;
	m_dwVtxSize = sizeof(VTXCUBETEX);
	m_dwVtxFVF = D3DFVF_VTXCUBETEX;

	m_dwTriCnt = 12;
	m_dwIdxSize = sizeof(INDEX32) * m_dwTriCnt;
	m_IndexFmt = D3DFMT_INDEX32;
	
	if(FAILED(Engine::CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXCUBETEX* pVertex = NULL;

	m_pVB->Lock(0,0,(void**)&pVertex,0);

	pVertex[0].vPos = _vec3(-1.f, 1.f, -1.f);
	pVertex[0].vTexUV = pVertex[0].vPos;

	pVertex[1].vPos = _vec3(1.f, 1.f, -1.f);
	pVertex[1].vTexUV = pVertex[1].vPos;

	pVertex[2].vPos = _vec3(1.f, -1.f, -1.f);
	pVertex[2].vTexUV = pVertex[2].vPos;

	pVertex[3].vPos = _vec3(-1.f, -1.f, -1.f);
	pVertex[3].vTexUV = pVertex[3].vPos;

	pVertex[4].vPos = _vec3(-1.f, 1.f, 1.f);
	pVertex[4].vTexUV = pVertex[4].vPos;

	pVertex[5].vPos = _vec3(1.f, 1.f, 1.f);
	pVertex[5].vTexUV = pVertex[5].vPos;

	pVertex[6].vPos = _vec3(1.f, -1.f, 1.f);
	pVertex[6].vTexUV = pVertex[6].vPos;

	pVertex[7].vPos = _vec3(-1.f, -1.f, 1.f);
	pVertex[7].vTexUV = pVertex[7].vPos;

	m_pVB->Unlock();

	INDEX32* pIndex = NULL;

	m_pIB->Lock(0,0,(void**)&pIndex,0);

	pIndex[0]._0 = 3;
	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;

	pIndex[1]._0 = 3;
	pIndex[1]._1 = 1;
	pIndex[1]._2 = 2;

	pIndex[2]._0 = 4;
	pIndex[2]._1 = 7;
	pIndex[2]._2 = 6;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 6;
	pIndex[3]._2 = 5;

	pIndex[4]._0 = 7;
	pIndex[4]._1 = 3;
	pIndex[4]._2 = 2;

	pIndex[5]._0 = 7;
	pIndex[5]._1 = 2;
	pIndex[5]._2 = 6;

	pIndex[6]._0 = 0;
	pIndex[6]._1 = 4;
	pIndex[6]._2 = 5;

	pIndex[7]._0 = 0;
	pIndex[7]._1 = 5;
	pIndex[7]._2 = 1;

	pIndex[8]._0 = 2;
	pIndex[8]._1 = 1;
	pIndex[8]._2 = 5;

	pIndex[9]._0 = 2;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 6;

	pIndex[10]._0 = 7;
	pIndex[10]._1 = 4;
	pIndex[10]._2 = 0;

	pIndex[11]._0 = 7;
	pIndex[11]._1 = 0;
	pIndex[11]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void Engine::CCube_Tex::Render_Buffer(void)
{
	m_pGraphicDev->SetStreamSource(0,m_pVB,0,m_dwVtxSize);
	m_pGraphicDev->SetFVF(D3DFVF_VTXCUBETEX);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,m_dwVtxCnt,0,m_dwTriCnt);
}

Engine::CComponent* Engine::CCube_Tex::Clone(void)
{
	return new CCube_Tex(*this);
}

Engine::CCube_Tex* Engine::CCube_Tex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCube_Tex* pInst = new CCube_Tex(pGraphicDev);

	if(FAILED(pInst->Ready_Buffer()))
		return NULL;

	return pInst;
}

void Engine::CCube_Tex::Free(void)
{
	Engine::CVIBuffer::Free();
}

void Engine::CCube_Tex::GetVtxInfo(VTXCUBETEX* pVertex)
{
	void* pTemp = NULL;
	m_pVB->Lock(0,0,(void**)&pTemp,0);
	memcpy(pVertex,pTemp,sizeof(VTXCUBETEX)*8);
	m_pVB->Unlock();
}

void Engine::CCube_Tex::GetIdxInfo(INDEX32* pIndex)
{
	void* pTemp = NULL;
	m_pIB->Lock(0,0,(void**)&pTemp,0);
	memcpy(pIndex,pTemp,sizeof(INDEX32)*12);
	m_pIB->Unlock();
}

