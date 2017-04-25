#include "VIBuffer.h"

using namespace Engine;

Engine::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
, m_pVB(NULL)
, m_pIB(NULL)
, m_dwVtxCnt(0)
, m_dwVtxSize(0)
, m_dwVtxFVF(0)
{
	m_pGraphicDev->AddRef();
}

Engine::CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
: m_pGraphicDev(rhs.m_pGraphicDev)
, m_pVB(rhs.m_pVB)
, m_pIB(rhs.m_pIB)
, m_dwVtxCnt(rhs.m_dwVtxCnt)
, m_dwVtxSize(rhs.m_dwVtxSize)
, m_dwVtxFVF(rhs.m_dwVtxFVF)
, m_dwIdxSize(rhs.m_dwIdxSize)
, m_dwTriCnt(rhs.m_dwTriCnt)
, m_IndexFmt(rhs.m_IndexFmt)
{
	m_pGraphicDev->AddRef();
	m_pVB->AddRef();
	m_pIB->AddRef();
}

Engine::CVIBuffer::~CVIBuffer(void)
{

}

HRESULT Engine::CVIBuffer::Ready_Buffer(void)
{
	// CreateVertexBuffer : 정점(Vertex)들을 보관하기위한 메모리공간(Buffer)을 생성하낟.	
	if(FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt, 0, m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, NULL)))
		return E_FAIL;
	if(FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize, 0, m_IndexFmt, D3DPOOL_MANAGED, &m_pIB, NULL)))
		return E_FAIL;
		
	return S_OK;
}

void Engine::CVIBuffer::Free(void)
{
	Engine::CComponent::Free();

	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pVB);
	Engine::Safe_Release(m_pIB);
}
