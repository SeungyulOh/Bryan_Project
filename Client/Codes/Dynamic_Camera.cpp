#include "stdafx.h"
#include "Dynamic_Camera.h"
#include "Export_Engine.h"

CDynamic_Camera::CDynamic_Camera(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CCamera(pGraphicDev)
,m_vDist(10.f)
{

}

CDynamic_Camera::~CDynamic_Camera()
{

}

HRESULT CDynamic_Camera::Ready_GameObject()
{
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&m_FirstView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION,&m_FirstProj);
	matView = m_FirstView;
	matProj = m_FirstProj;

	_matrix matInvView, matInvProj;
	D3DXMatrixInverse(&matInvView,0,&matView);

	m_vCamPos = Engine::MyGetVec3FromMat(Engine::VEC_POSITION,matInvView);
	m_vCamDir = Engine::MyGetVec3FromMat(Engine::VEC_LOOK,matInvView);
	m_vTarget = m_vCamPos + m_vCamDir * m_vDist;
	m_vUp = _vec3(0.f,1.f,0.f);
	
	return S_OK;
}

_int CDynamic_Camera::Update_GameObject(const _float& fTimeDelta)
{
	KeyInput(fTimeDelta);



	Engine::CCamera::Update_GameObject(fTimeDelta);

	return 0;
}

void CDynamic_Camera::KeyInput(const _float& fTimeDelta)
{
	
	_long lMoveDist = 0;
	if(lMoveDist = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_Z))
	{
		m_vDist -= lMoveDist/50.f;
	}
	if(lMoveDist = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_X))
	{
		_matrix matRot;
		D3DXMatrixRotationAxis(&matRot,&Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_UP),lMoveDist/500.f);
		D3DXVec3TransformNormal(&m_vCamDir,&m_vCamDir,&matRot);
	}

	if(lMoveDist = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_Y))
	{
		_matrix matRot;
		D3DXMatrixRotationAxis(&matRot,&Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_RIGHT),lMoveDist/500.f);
		D3DXVec3TransformNormal(&m_vCamDir,&m_vCamDir,&matRot);
	}

	if(Engine::Get_DIKeyState(DIK_UP))
	{
		m_vCamDir = Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_LOOK);
		m_vCamPos += m_vCamDir * fTimeDelta * 50.f;
	}
	if(Engine::Get_DIKeyState(DIK_DOWN))
	{
		m_vCamDir = Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_LOOK);
		m_vCamPos -= m_vCamDir * fTimeDelta * 50.f;
	}
	if(Engine::Get_DIKeyState(DIK_LEFT))
	{
		_vec3 vDir = -Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_RIGHT);
		m_vCamPos += vDir * fTimeDelta * 50.f;
	}
	if(Engine::Get_DIKeyState(DIK_RIGHT))
	{
		_vec3 vDir = Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_RIGHT);
		m_vCamPos += vDir * fTimeDelta * 50.f;
	}
	m_vTarget = m_vCamPos + m_vCamDir * m_vDist;

	POINT pt = {WINCX/2,WINCY/2};
	ClientToScreen(g_hWnd,&pt);
	SetCursorPos(pt.x,pt.y);
	
}


CDynamic_Camera* CDynamic_Camera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDynamic_Camera* pInst = new CDynamic_Camera(pGraphicDev);
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CDynamic_Camera::Free(void)
{
	m_pGraphicDev->SetTransform(D3DTS_VIEW,&m_FirstView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION,&m_FirstProj);

	Engine::CCamera::Free();
}

