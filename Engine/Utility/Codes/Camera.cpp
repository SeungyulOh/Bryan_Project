#include "Camera.h"

using namespace Engine;

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
:CGameObject(pGraphicDev)
,m_fAspect(0.f)
,m_fNear(0.f)
,m_fFar(0.f)
,m_fFovY(0.f)
{
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);
	ZeroMemory(&m_vCamPos,sizeof(_vec3));
	ZeroMemory(&m_vTarget,sizeof(_vec3));
	m_vUp = _vec3(0.f,1.f,0.f);
	m_vCamDir = _vec3(0.f,0.f,1.f);
}

Engine::CCamera::~CCamera()
{
	
}

HRESULT Engine::CCamera::Ready_GameObject(void)
{
	D3DXMatrixLookAtLH(&matView,&m_vCamPos,&m_vTarget,&m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW,&matView);

	D3DXMatrixPerspectiveFovLH(&matProj,m_fFovY,m_fAspect,m_fNear,m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION,&matProj);

	return S_OK;
}

_int Engine::CCamera::Update_GameObject(const _float& fTimeDelta)
{
	D3DXMatrixLookAtLH(&matView,&m_vCamPos,&m_vTarget,&m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW,&matView);

	return 0;
}

void Engine::CCamera::Free(void)
{
	Engine::CGameObject::Free();
}

