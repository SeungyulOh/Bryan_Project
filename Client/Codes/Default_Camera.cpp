#include "stdafx.h"
#include "Default_Camera.h"
#include "Export_Engine.h"
#include "Player.h"

CDefault_Camera::CDefault_Camera(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CCamera(pGraphicDev)
,m_pTarget(NULL)
,m_isOnce(false)
,m_pTerrainMesh(NULL)
,m_isShake(false)
,m_fShakeTime(0.f)
,m_pRendererCom(NULL)
{

}

CDefault_Camera::CDefault_Camera(const CDefault_Camera& rhs)
:Engine::CCamera(rhs.m_pGraphicDev)
,m_pRendererCom(rhs.m_pRendererCom)
,m_pTarget(rhs.m_pTarget)
,m_vDist(rhs.m_vDist)
,m_isOnce(rhs.m_isOnce)
,m_pPlayerWorldMat(rhs.m_pPlayerWorldMat)
,m_matScale(rhs.m_matScale)
,m_pTerrainMesh(rhs.m_pTerrainMesh)
,m_isHit(rhs.m_isHit)
,m_pMatCam(rhs.m_pMatCam)
,m_pMatTarget(rhs.m_pMatTarget)
,m_pPlayerState(rhs.m_pPlayerState)
,m_isShake(rhs.m_isShake)
,m_fShakeTime(rhs.m_fShakeTime)
{
	
	m_pRendererCom->AddRefCnt();
	m_pTerrainMesh->AddRef();
}

CDefault_Camera::~CDefault_Camera()
{

}

HRESULT CDefault_Camera::Ready_GameObject(_vec3 vCamPos, _vec3 vTarget, _vec3 vUp, _float fFovY, _float fAspect, _float fNear, _float fFar)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_vCamPos = vCamPos;
	m_vTarget = vTarget;
	m_vUp = vUp;

	m_fAspect = fAspect;
	m_fFovY = fFovY;
	m_fNear = 1.f;
	m_fFar = 1500.f;
	m_vDist = 25.f;

	Engine::CCamera::Ready_GameObject();
	m_vCamDir = _vec3(1.f,0.f,0.f);

	
	D3DXMatrixScaling(&m_matScale,10.f,10.f,10.f);

	return S_OK;
}

_int CDefault_Camera::Update_GameObject(const _float& fTimeDelta)
{
	if(Engine::Get_KeyMgr()->KeyPressed(KEY_K))
	{
		m_isDestroy = true;
		Engine::CGameObject* pGameObject = new CDefault_Camera(*this);
		Engine::GetLayer(L"Layer_Cam")->Add_Object(pGameObject);
	}
	if(m_isOnce == false)
	{
		m_isOnce = true;

		Engine::CStaticMesh* pTerrainMesh = (Engine::CStaticMesh*)Engine::GetList(L"Layer_Back")->front()->FindComponent(Engine::CGameObject::NONUPDATE,L"Component_Mesh_BossMap");
		if(pTerrainMesh==NULL)
			return 0;

		Engine::CDynamicMesh* pPlayerMesh = (Engine::CDynamicMesh*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::NONUPDATE,L"Component_Mesh_Hagie");
		Engine::CTransform* pTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");

		Engine::D3DXFRAME_DERIVED* pFoundFrame = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(pPlayerMesh->GetRootFrame(),"ValveBiped_Anim_Attachment_CAM");
		//m_pTarget = (_vec3*)&pFoundFrame->CombinedMatrix.m[3][0];		
		m_pTarget = (_vec3*)&pTransform->GetWorldMatrix()->m[3][0];
		m_pPlayerWorldMat = pTransform->GetWorldMatrix();

		m_pTerrainMesh = pTerrainMesh->GetMesh();
		m_pTerrainMesh->AddRef();

		pFoundFrame = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(pPlayerMesh->GetRootFrame(),"ValveBiped_Anim_Attachment_CAM");
		m_pMatTarget = &pFoundFrame->CombinedMatrix;

		pFoundFrame = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(pPlayerMesh->GetRootFrame(),"ValveBiped_Anim_Attachment_CAM_Origin");
		m_pMatCam = &pFoundFrame->CombinedMatrix;

		m_pPlayerState = ((CPlayer*)Engine::GetList(L"Layer_Player")->front())->GetState();
	}

	if(*m_pPlayerState != CPlayer::REQ_BEGIN && *m_pPlayerState != CPlayer::REQ_END)
		KeyInput(fTimeDelta);
	else
	{
		/*m_vCamPos = *(_vec3*)&((*m_pMatCam* *m_pPlayerWorldMat).m[3][0]);
		m_vTarget = *(_vec3*)&((*m_pMatTarget* *m_pPlayerWorldMat).m[3][0]);*/

		Engine::CDynamicMesh* pPlayerMesh = (Engine::CDynamicMesh*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::NONUPDATE,L"Component_Mesh_Hagie");
		if(pPlayerMesh->Get_CurrentRatioFromAnimation() > 0.01)
		{
			_matrix matRotY,matTrans;
			D3DXMatrixRotationY(&matRotY, D3DX_PI);
			D3DXMatrixTranslation(&matTrans,30.f,0.f,-30.f);
			m_vCamPos = *(_vec3*)&((*m_pMatCam * matRotY * *m_pPlayerWorldMat).m[3][0]);
			m_vTarget = m_vCamPos + *(_vec3*)&((*m_pMatTarget * matRotY* *m_pPlayerWorldMat).m[3][0]) * 10.f;
		}
	}


	if(m_pTarget == NULL)
	{
		Engine::CCamera::Update_GameObject(fTimeDelta);
		return 0;
	}

	//D3DXVec3TransformCoord(&m_vTarget,m_pTarget,m_pPlayerWorldMat);
	m_vTarget = *m_pTarget + _vec3(0.f,5.f,0.f);

	if(m_isShake == true && m_fShakeTime > 0)
	{
		m_fShakeTime -= fTimeDelta;
		float fX = (_float)(rand()%3 - 2);
		float fY = (_float)(rand()%3 - 2);
		float fZ = (_float)(rand()%3 - 2);
		m_vCamPos += _vec3(fX*0.3f,fY*0.3f,fZ*0.3f);
	}
	else if(m_fShakeTime <= 0)
		m_isShake = false;

	Engine::CCamera::Update_GameObject(fTimeDelta);


	/////Camera Collision With Terrain
	_float fDist = 0.f;
	_vec3 vRayDir, vRayPos;
	D3DXVec3Normalize(&vRayDir,&(m_vCamPos-m_vTarget));

	
	D3DXVec3TransformCoord(&vRayPos,&m_vTarget,&m_matScale);
	D3DXVec3TransformNormal(&vRayDir,&vRayDir,&m_matScale);
	D3DXVec3Normalize(&vRayDir,&vRayDir);

	if(m_pTerrainMesh == NULL)
		return 0;

	D3DXIntersect(m_pTerrainMesh,&vRayPos,&vRayDir,&m_isHit,NULL,NULL,NULL,&fDist,NULL,NULL);
	if(m_isHit && fDist < m_vDist*10.f - 5.f)
	{
		m_vDist = fDist / 10.f;
	}

	if(m_vDist < 1.f)
		m_vDist = 1.f;

	if(m_vCamPos == _vec3(0.f,0.f,0.f))
	{
		m_vCamPos = *m_pTarget - *(_vec3*)&m_pPlayerWorldMat->m[2][0] * m_vDist;
	}

	
	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_UI);
	
	
	return 0;
}

void CDefault_Camera::KeyInput(const _float& fTimeDelta)
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
	m_vCamPos = m_vTarget - m_vDist*m_vCamDir;

	POINT pt = {WINCX/2,WINCY/2};
	ClientToScreen(g_hWnd,&pt);
	SetCursorPos(pt.x,pt.y);
	
}


CDefault_Camera* CDefault_Camera::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vCamPos, _vec3 vTarget, _vec3 vUp, _float fFovY, _float fAspect, _float fNear, _float fFar)
{
	CDefault_Camera* pInst = new CDefault_Camera(pGraphicDev);
	if(FAILED(pInst->Ready_GameObject(vCamPos,vTarget,vUp,fFovY,fAspect,fNear,fFar)))
		return NULL;

	return pInst;
}

void CDefault_Camera::Free(void)
{
	Engine::Safe_Release(m_pTerrainMesh);
	Engine::Safe_Release(m_pRendererCom);

	Engine::CCamera::Free();
}

_int CDefault_Camera::LastUpdate_GameObject(const _float& fTimeDelta)
{

	return 0;
}

void CDefault_Camera::ShakeCam(_float fShakeTime)
{
	m_isShake = true;
	m_fShakeTime = fShakeTime;
}

void CDefault_Camera::Render_GameObject()
{

	/*TCHAR szTemp[256] = L"%d";
	wsprintf(szTemp,L"CamPos X : %d",(_uint)m_vCamPos.x);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(10.f,90.f),D3DXCOLOR(1.f,1.f,1.f,1.f));

	wsprintf(szTemp,L"CamPos Y: %d",(_uint)m_vCamPos.y);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(10.f,110.f),D3DXCOLOR(1.f,1.f,1.f,1.f));

	wsprintf(szTemp,L"CamPos Z : %d",(_uint)m_vCamPos.z);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(10.f,130.f),D3DXCOLOR(1.f,1.f,1.f,1.f));

	wsprintf(szTemp,L"Dist : %d",(_uint)m_vDist);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(10.f,150.f),D3DXCOLOR(1.f,1.f,1.f,1.f));


	wsprintf(szTemp,L"Target X : %d",(_uint)m_vTarget.x);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(10.f,170.f),D3DXCOLOR(1.f,1.f,1.f,1.f));

	wsprintf(szTemp,L"Target Y: %d",(_uint)m_vTarget.y);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(10.f,190.f),D3DXCOLOR(1.f,1.f,1.f,1.f));

	wsprintf(szTemp,L"Target Z : %d",(_uint)m_vTarget.z);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(10.f,210.f),D3DXCOLOR(1.f,1.f,1.f,1.f));*/


}

