#include "NaviCell.h"

Engine::CNaviCell::CNaviCell(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3* pPointA,D3DXVECTOR3* pPointB,D3DXVECTOR3* pPointC,bool _ToolMode)
:m_pDevice(pDevice)
,bToolMode(_ToolMode)
{
	//memcpy(m_vPoint,pPointArray,sizeof(D3DXVECTOR3)*3);
	//memcpy(m_vConPoint,pPointArray,sizeof(D3DXVECTOR3)*3);
	

	m_pPoint[0] = pPointA;
	m_pPoint[1] = pPointB;
	m_pPoint[2] = pPointC;

	m_vPoint[0] = *m_pPoint[0];
	m_vPoint[1] = *m_pPoint[1];
	m_vPoint[2] = *m_pPoint[2];
}

Engine::CNaviCell::~CNaviCell()
{

}

Engine::CNaviCell* Engine::CNaviCell::Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3* pPointA,D3DXVECTOR3* pPointB,D3DXVECTOR3* pPointC,bool _ToolMode)
{
	CNaviCell* pMine = new CNaviCell(pDevice,pPointA,pPointB,pPointC,_ToolMode);
	pMine->Initialize();
	return pMine;
}

void Engine::CNaviCell::Initialize()
{
	D3DXCreateLine(m_pDevice,&m_pLine);
}

void Engine::CNaviCell::Render()
{
	D3DXMATRIX matView,matProj,matFinal;
	m_pDevice->GetTransform(D3DTS_VIEW,&matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION,&matProj);

	D3DXMATRIX matInvView;
	D3DXMatrixInverse(&matInvView,0,&matView);


	if(bToolMode == true)
	{
		m_vPoint[0] = *m_pPoint[0];
		m_vPoint[1] = *m_pPoint[1];
		m_vPoint[2] = *m_pPoint[2];
	}

	for(int i=0; i<3; ++i)
	{
		D3DXVec3TransformCoord(&m_vConPoint[i],&m_vPoint[i],&matView);

		if(m_vConPoint[i].z < 0.1f)
			m_vConPoint[i].z = 0.1f;

		D3DXVec3TransformCoord(&m_vConPoint[i],&m_vConPoint[i],&matProj);
	}
	m_vConPoint[3] = m_vConPoint[0];

	m_pLine->Begin();

	m_pLine->SetWidth(5.f);
	m_pLine->DrawTransform(m_vConPoint,4,D3DXMatrixIdentity(&matFinal),D3DCOLOR_ARGB(255,255,0,0));


	m_pLine->End();
}

void Engine::CNaviCell::SetNeighbor(CNaviCell* pNeighbor)
{
	m_NeighborVec.push_back(pNeighbor);
}

D3DXVECTOR3* Engine::CNaviCell::GetPoint()
{
	return m_vPoint;
}

void Engine::CNaviCell::SetMyIndex(DWORD _Index)
{
	m_myIndex = _Index;
}

DWORD Engine::CNaviCell::GetIndex()
{
	return m_myIndex;
}

std::vector<Engine::CNaviCell*>* Engine::CNaviCell::GetNeighbor()
{
	return &m_NeighborVec;
}

void Engine::CNaviCell::Free(void)
{
	m_pLine->Release();
}

