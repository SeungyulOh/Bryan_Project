#include "stdafx.h"
#include "Action_Camera.h"
#include "Export_Engine.h"
#include "Engine_Struct.h"
#include "Boss.h"

CAction_Camera::CAction_Camera(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CCamera(pGraphicDev)
,m_isOnce(false)
,m_pDynamicMeshCom(NULL)
,m_iState(0)
,m_pBoss(NULL)
,m_fShakeTime(0.f)
,m_isShake(false)
{

}

CAction_Camera::~CAction_Camera()
{

}

HRESULT CAction_Camera::Ready_GameObject()
{
	m_pDynamicMeshCom = (Engine::CDynamicMesh*)Engine::Clone(COM_STAGE,L"Component_Mesh_StartCam");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Mesh_StartCam",m_pDynamicMeshCom);

	//Engine::D3DXFRAME_DERIVED* pFrameCat = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(m_pDynamicMeshCom->GetRootFrame(),"root");
	Engine::D3DXFRAME_DERIVED* pFrameCam = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(m_pDynamicMeshCom->GetRootFrame(),"attach_cat");
	Engine::D3DXFRAME_DERIVED* pFrameFov = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(m_pDynamicMeshCom->GetRootFrame(),"attach_fov");
	
	m_pCamWorld = &pFrameCam->CombinedMatrix;
	m_pTarget = &pFrameFov->CombinedMatrix;

	m_vCamPos = _vec3(0.f,0.f,0.f);
	m_vTarget = _vec3(0.f,0.f,0.f);
	m_vUp = _vec3(0.f,1.f,0.f);

	m_fAspect = (_float)WINCX/(_float)WINCY;
	m_fFovY = D3DX_PI/4.f;
	m_fNear = 0.1f;
	m_fFar = 1500.f;

	Engine::CCamera::Ready_GameObject();

	m_pDynamicMeshCom->Set_AnimationSet(m_iState);

	D3DXMatrixScaling(&matScale,0.1f,0.1f,0.1f);

	
	return S_OK;
}

_int CAction_Camera::Update_GameObject(const _float& fTimeDelta)
{

	if(m_isOnce == false)
	{
		m_isOnce = true;
		//m_pBoss = (CBoss*)Engine::GetList(L"Layer_Boss")->front();
		
		list<CGameObject*>* pList = Engine::GetList(L"Layer_Boss");
		if(NULL == pList)
			return 0;

		m_pBoss = dynamic_cast<CBoss*>(pList->front());
		if(NULL == m_pBoss)
			return 0;

	}
	


	_matrix matCam = *m_pCamWorld * matScale;

	if(m_isShake == false)
		m_vCamPos = *(_vec3*)&matCam.m[3][0];
	
	_matrix matRot;
	D3DXMatrixRotationAxis(&matRot,(_vec3*)&matCam.m[0][0],D3DX_PI/2.f);

	if(m_isShake == false)
		m_vTarget = m_vCamPos - *D3DXVec3TransformNormal(&_vec3(),(_vec3*)&matCam.m[2][0],&matRot);


	if(m_isShake == true && m_fShakeTime > 0)
	{
		m_fShakeTime -= fTimeDelta;
		float fX = (_float)(rand()%3 - 1);
		float fY = (_float)(rand()%3 - 1);
		float fZ = (_float)(rand()%3 - 1);
		m_vCamPos += _vec3(fX*0.006f,fY*0.006f,fZ*0.006f);
	}
	else if(m_fShakeTime <= 0)
		m_isShake = false;
	

	Engine::CCamera::Update_GameObject(fTimeDelta);

	list<CGameObject*> pCamList = *Engine::GetList(L"Layer_Cam");

	/*if(m_pBoss == NULL)
		return -1;
	if(m_iState < 8 && m_iState != m_pBoss->GetState())
	{
		m_iState = m_pBoss->GetState();
		m_pDynamicMeshCom->Set_AnimationSet(m_iState);
	}

	m_pDynamicMeshCom->Play_Animation(fTimeDelta);*/

	return 0;
}

_int CAction_Camera::LastUpdate_GameObject(const _float& fTimeDelta)
{	
	if(m_pBoss == NULL)
		return -1;
	if(m_iState < 8 && m_iState != m_pBoss->GetState())
	{
		m_iState = m_pBoss->GetState();
		m_pDynamicMeshCom->Set_AnimationSet(m_iState);
	}

	m_pDynamicMeshCom->Play_Animation(fTimeDelta);
	return 0;
}


CAction_Camera* CAction_Camera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAction_Camera* pInst = new CAction_Camera(pGraphicDev);
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CAction_Camera::Free(void)
{
	Engine::Safe_Release(m_pDynamicMeshCom);

	Engine::CCamera::Free();
}

void CAction_Camera::ShakeCam(_float fShakeTime)
{
	m_isShake = true;
	m_fShakeTime = fShakeTime;
}


