#include "stdafx.h"
#include "EffectBack.h"
#include "Export_Engine.h"

CEffectBack::CEffectBack(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
,m_pPlayerTool(NULL)
{
}

CEffectBack::~CEffectBack(void)
{

}

void CEffectBack::Ready_EffectBack(void)
{
	m_pPlayerTool = CPlayer_Tool::Create(m_pGraphicDev);
}

void CEffectBack::Render_GameObject()
{
	m_pPlayerTool->Render_GameObject();
	Render_AxisLine();
}

void CEffectBack::Render_AxisLine()
{
	_matrix matView,matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION,&matProj);

	D3DXVECTOR3 vCenter = D3DXVECTOR3(0.f,0.f,0.f);
	D3DXVec3TransformCoord(&vCenter,&vCenter,&matView);
	D3DXVec3TransformCoord(&vCenter,&vCenter,&matProj);

	D3DXVECTOR3 vAxisX = D3DXVECTOR3(4.f,0.f,0.f);
	D3DXVec3TransformCoord(&vAxisX,&vAxisX,&matView);
	D3DXVec3TransformCoord(&vAxisX,&vAxisX,&matProj);

	D3DXVECTOR3 vAxisY = D3DXVECTOR3(0.f,4.f,0.f);
	D3DXVec3TransformCoord(&vAxisY,&vAxisY,&matView);
	D3DXVec3TransformCoord(&vAxisY,&vAxisY,&matProj);

	D3DXVECTOR3 vAxisZ = D3DXVECTOR3(0.f,0.f,4.f);
	D3DXVec3TransformCoord(&vAxisZ,&vAxisZ,&matView);
	D3DXVec3TransformCoord(&vAxisZ,&vAxisZ,&matProj);

	D3DXMATRIX matViewPort;
	D3DXMatrixIdentity(&matViewPort);
	matViewPort._11 = WINCX/2;
	matViewPort._22 = -WINCY/2;
	matViewPort._33 = 1;
	matViewPort._41 = WINCX/2;
	matViewPort._42 = WINCY/2;
	matViewPort._43 = 0.f;
	matViewPort._44 = 1.f;

	D3DXVec3TransformCoord(&vAxisX,&vAxisX,&matViewPort);
	D3DXVec3TransformCoord(&vAxisY,&vAxisY,&matViewPort);
	D3DXVec3TransformCoord(&vAxisZ,&vAxisZ,&matViewPort);
	D3DXVec3TransformCoord(&vCenter,&vCenter,&matViewPort);

	m_AxisX[0] = D3DXVECTOR2(vCenter.x,vCenter.y);
	m_AxisX[1] = D3DXVECTOR2(vAxisX.x,vAxisX.y);

	m_AxisY[0] = D3DXVECTOR2(vCenter.x,vCenter.y);
	m_AxisY[1] = D3DXVECTOR2(vAxisY.x,vAxisY.y);

	m_AxisZ[0] = D3DXVECTOR2(vCenter.x,vCenter.y);
	m_AxisZ[1] = D3DXVECTOR2(vAxisZ.x,vAxisZ.y);

	Engine::GetLine()->Begin();
	Engine::GetLine()->Draw(m_AxisX,2,D3DCOLOR_ARGB(255,255,0,0));
	Engine::GetLine()->Draw(m_AxisY,2,D3DCOLOR_ARGB(255,0,255,0));
	Engine::GetLine()->Draw(m_AxisZ,2,D3DCOLOR_ARGB(255,0,0,0));
	Engine::GetLine()->End();
}

CEffectBack* CEffectBack::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectBack* pInst = new CEffectBack(pGraphicDev);
	pInst->Ready_EffectBack();
	return pInst;
}

void CEffectBack::Free(void)
{
	Engine::Safe_Release(m_pPlayerTool);
	Engine::CGameObject::Free();
}

