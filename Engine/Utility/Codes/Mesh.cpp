#include "Mesh.h"

Engine::CMesh::CMesh(LPDIRECT3DDEVICE9 pGraphicDev)
:m_pGraphicDev(pGraphicDev)
,m_isClone(false)
{
	m_pGraphicDev->AddRef();
}

Engine::CMesh::CMesh(const CMesh& rhs)
:m_pGraphicDev(rhs.m_pGraphicDev)
,m_isClone(true)
{
	m_pGraphicDev->AddRef();
}

Engine::CMesh::~CMesh(void)
{

}

void Engine::CMesh::Free(void)
{
	Engine::Safe_Release(m_pGraphicDev);

	Engine::CComponent::Free();
}

