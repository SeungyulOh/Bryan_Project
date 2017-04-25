#include "stdafx.h"
#include "Reaper.h"
#include "Export_Engine.h"
#include "Player.h"
#include "Stage_Scene.h"
#include "BlueEye.h"
#include "Phantom_Trail.h"
#include "EventMap.h"
#include "StarGlow.h"
#include "HitEffect.h"

CReaper* CReaper::m_pSelectedReaper = NULL;

CReaper::CReaper(LPDIRECT3DDEVICE9 pGraphicDev, EYECOLOR eColor)
:Engine::CGameObject(pGraphicDev)
,m_eMyState(IDLE)
,m_pShaderCom(NULL)
,m_pTransformCom(NULL)
,m_pRendererCom(NULL)
,m_pDynamicMeshCom(NULL)
,m_fChangeColorTimeAcc(0.f)
,m_isOnce(false)
,m_fTimeAcc(0.f)
,m_eMyColor(eColor)
,m_pBoxColliderCom(NULL)
,m_bMakeMoving(false)
,m_fChangeDirectionTime(0.f)
,m_fAngleOffSet(0.f)
{
	_float fRandAngle = D3DXToRadian(rand()%361);
	_matrix matRot;
	D3DXMatrixRotationY(&matRot,fRandAngle);

	m_vTargetDir = _vec3(0.f,0.f,1.f);
	D3DXVec3TransformNormal(&m_vTargetDir,&m_vTargetDir,&matRot);
}

CReaper::~CReaper(void)
{

}

HRESULT CReaper::Ready_GameObject(_vec3 vFirstPos, _float fOffSetAngle)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pDynamicMeshCom = (Engine::CDynamicMesh*)Engine::Clone(COM_STAGE,L"Component_Mesh_Reaper");
	if(m_pDynamicMeshCom == NULL)
		return S_OK;
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Mesh_Reaper",m_pDynamicMeshCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Mesh");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Mesh",m_pShaderCom);

	m_pShaderForShadow = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Shadow");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Shadow",m_pShaderForShadow);


	m_pBoxColliderCom = (Engine::CBoxCollider*)Engine::Clone(COM_STAGE, L"Component_BoxCollider");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_BoxCollider",m_pBoxColliderCom);
	_matrix matTemp;
	m_pBoxColliderCom->Compute_MinMax(m_pDynamicMeshCom,*D3DXMatrixIdentity(&matTemp),0);


	Engine::CTransform* pPlayerTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
	m_pPlayerWorldMat = pPlayerTransform->GetWorldMatrix();
	
	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,_vec3(0.1f,0.1f,0.1f));
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(0.f,fOffSetAngle,0.f));

	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vFirstPos);
	m_pTransformCom->Update_Component(0.f);

	m_eMyState = IDLE;
	m_pDynamicMeshCom->Set_AnimationSet(m_eMyState);

	
	///////

	Engine::CGameObject* pGameObject = m_pRightEye = CBlueEye::Create(m_pGraphicDev,CBlueEye::R_EYE);
	((CBlueEye*)pGameObject)->SetParentTransform(m_pTransformCom);
	((CBlueEye*)pGameObject)->SetParentMesh(m_pDynamicMeshCom);
	Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

	pGameObject = m_pLeftEye = CBlueEye::Create(m_pGraphicDev,CBlueEye::L_EYE);
	((CBlueEye*)pGameObject)->SetParentTransform(m_pTransformCom);
	((CBlueEye*)pGameObject)->SetParentMesh(m_pDynamicMeshCom);
	Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
	
	m_pPlayer = Engine::GetList(L"Layer_Player")->front();

	//Make COlor

	if(m_eMyColor == RED)
	{
		m_pRightEye->SetEyeColorChange(11);
		m_pLeftEye->SetEyeColorChange(11);

		pGameObject = m_pStarGlow =CStarGlow::Create(m_pGraphicDev,m_pTransformCom->GetWorldMatrix(),CStarGlow::RED,m_pDynamicMeshCom);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
	}
	else if(m_eMyColor == GREEN)
	{
		m_pRightEye->SetEyeColorChange(6);
		m_pLeftEye->SetEyeColorChange(6);

		pGameObject = m_pStarGlow =CStarGlow::Create(m_pGraphicDev,m_pTransformCom->GetWorldMatrix(),CStarGlow::GREEN,m_pDynamicMeshCom);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
	}
	else if(m_eMyColor == BLUE)
	{
		m_pRightEye->SetEyeColorChange(0);
		m_pLeftEye->SetEyeColorChange(0);

		pGameObject = m_pStarGlow = CStarGlow::Create(m_pGraphicDev,m_pTransformCom->GetWorldMatrix(),CStarGlow::BLUE,m_pDynamicMeshCom);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
	}


	m_pTargetList = ((CEventMap*)Engine::GetList(L"Layer_Event")->front())->GetTargetList();

	return S_OK;
}

_int CReaper::Update_GameObject(const _float& fTimeDelta)
{
	if(m_pDynamicMeshCom == NULL)
		return 0;

	m_fTimeAcc += fTimeDelta;

	if(m_isOnce == false)
	{
		m_isOnce = true;

		Engine::D3DXFRAME_DERIVED* pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(m_pDynamicMeshCom->GetRootFrame(),"lamp_light"); 
		m_pLampMat = &pFrameFound->CombinedMatrix;
	}

	m_fTimeDelta = fTimeDelta;
	m_fChangeColorTimeAcc += fTimeDelta;
	
	//if(m_fChangeColorTimeAcc > 4.f)
	//{
	//	// Red 11 , White 8 , Blue 0

	//	_int iIndex = 0;
	//	if(*m_pRightEye->GetEyeColor() == 11)
	//	{
	//		_uint iRand = rand()%2;
	//		if(iRand == 0)
	//			iIndex = 8;
	//		else
	//			iIndex = 0;
	//	}
	//	else if(*m_pRightEye->GetEyeColor() == 8)
	//	{
	//		_uint iRand = rand()%2;
	//		if(iRand == 0)
	//			iIndex = 11;
	//		else
	//			iIndex = 0;
	//	}
	//	else if(*m_pRightEye->GetEyeColor() == 0)
	//	{
	//		_uint iRand = rand()%2;
	//		if(iRand == 0)
	//			iIndex = 8;
	//		else
	//			iIndex = 11;
	//	}
	//	
	//	m_fChangeColorTimeAcc = 0.f;
	//			
	//	m_pRightEye->SetEyeColorChange(iIndex);
	//	m_pLeftEye->SetEyeColorChange(iIndex);
	//}

	//_vec3 vNewPos = *m_pTransformCom->GetInfomation(Engine::CTransform::POSITION) + *(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0] * m_fTimeDelta * 10.f;
	//m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vNewPos);

	
	if(m_pSelectedReaper == this/*&& m_pTargetList->size() < 6*/)
	{
		m_pRightEye->SetAlpha(1.f);
		m_pLeftEye->SetAlpha(1.f);
	}
	else if(((CEventMap*)Engine::GetList(L"Layer_Event")->front())->GetStartChaseFlag() == true)
	{
		m_pRightEye->SetAlpha(1.f);
		m_pLeftEye->SetAlpha(1.f);
	}
	else// if(m_pSelectedReaper != this && m_pTargetList->size() < 6)
	{
		m_pRightEye->SetAlpha(0.f);
		m_pLeftEye->SetAlpha(0.f);
	}
	/*else if(m_pTargetList->size() >= 6 && *m_pChaseFlag == true)
	{
		m_pRightEye->SetAlpha(1.f);
		m_pLeftEye->SetAlpha(1.f);
	}*/



	if(m_bMakeMoving == true)
	{
		m_fChangeDirectionTime += fTimeDelta;
		if(m_fChangeDirectionTime > 5.f)
		{
			m_fChangeDirectionTime = 0.f;
			_matrix matRot;

			_float fRandAngle = _float(rand()%61) - 30.f;

			D3DXMatrixRotationY(&matRot,D3DXToRadian(fRandAngle));
			D3DXVec3TransformNormal(&m_vTargetDir,&m_vTargetDir,&matRot);

		}
		m_eMyState = RUN;
		m_pDynamicMeshCom->Set_AnimationSet(RUN);
		_vec3 vDist = *(_vec3*)&m_pPlayerWorldMat->m[3][0] - *(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0];
		_float fDist = D3DXVec3Length(&vDist);

		if(m_pTransformCom->GetWorldMatrix()->_41 < -180.f || m_pTransformCom->GetWorldMatrix()->_41 > 180.f || m_pTransformCom->GetWorldMatrix()->_43 > 150.f || m_pTransformCom->GetWorldMatrix()->_43 < -150.f)
		{
			m_vTargetDir *= -1.f;
			_vec3 vPos = *(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0] + m_vTargetDir * m_fTimeDelta * (300/fDist);
			m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vPos);
		}
		
		_float fCos = D3DXVec3Dot(&m_vTargetDir, &_vec3(0.f,0.f,1.f));
		_float fAngle = 0.f;
		if(m_vTargetDir.x > 0)
			fAngle = acosf(fCos);
		else
			fAngle = D3DX_PI*2.f - acosf(fCos);

		m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(0.f,fAngle + D3DX_PI/2.f,0.f));
		_vec3 vPos = *(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0] + m_vTargetDir * m_fTimeDelta * (300/fDist);
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vPos);
	}


	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_NONALPHA);
	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_SHADOW);

	if(Engine::Get_KeyMgr()->KeyPressed(KEY_5))
	{
		m_eMyState++;
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState);
	}
	if(Engine::Get_KeyMgr()->KeyPressed(KEY_6))
	{
		m_eMyState--;
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState);
	}


	// FOr Light Info/////

	if(m_eMyColor == RED)
	{
		D3DLIGHT9 tLightInfo = Engine::GetLightInfo(4);
		_matrix matTotal = *m_pLampMat * *m_pTransformCom->GetWorldMatrix();
		tLightInfo.Position = *(_vec3*)&matTotal.m[3][0];
		Engine::Change_LightInfo(tLightInfo,4);
	}
	else if(m_eMyColor == GREEN)
	{
		D3DLIGHT9 tLightInfo = Engine::GetLightInfo(5);
		_matrix matTotal = *m_pLampMat * *m_pTransformCom->GetWorldMatrix();
		tLightInfo.Position = *(_vec3*)&matTotal.m[3][0];
		Engine::Change_LightInfo(tLightInfo,5);
	}
	else if(m_eMyColor == BLUE)
	{
		D3DLIGHT9 tLightInfo = Engine::GetLightInfo(6);
		_matrix matTotal = *m_pLampMat * *m_pTransformCom->GetWorldMatrix();
		tLightInfo.Position = *(_vec3*)&matTotal.m[3][0];
		Engine::Change_LightInfo(tLightInfo,6);
	}
	

	return 0;
}

_int CReaper::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if(m_isDestroy)
		return 0;

	if(Engine::GetList(L"Layer_Trail")->size() == 0)
	{
		m_isCol = false;
		return 0;
	}
	if(((CPhantom_Trail*)Engine::GetList(L"Layer_Trail")->back())->GetTrailList()->size() < 4)
	{
		m_isCol = false;
		return 0;
	}

	Engine::CBoxCollider* pTarget = (Engine::CBoxCollider*)Engine::GetList(L"Layer_Trail")->back()->FindComponent(Engine::CGameObject::NONUPDATE,L"Component_BoxCollider");
	if(pTarget == NULL)
		return 0;


	if(((CPlayer*)m_pPlayer)->GetCollisionPossibility() == 1)
	{
		m_isCol = m_pBoxColliderCom->CollisionCheck_AABB(pTarget);
		
		if(m_isCol == true)
			((CPlayer*)m_pPlayer)->StopAtkCollision();
	}
	else
		m_isCol = false;
	
	if(m_isCol == true)
	{
		if(m_pTargetList->front() == this->m_eMyColor)
 			m_pTargetList->pop_front();

		if(m_pTargetList->size() == 0)
		{
			((CEventMap*)Engine::GetList(L"Layer_Event")->front())->SetEventEnd();
		}
	}

	if(m_isCol == true)
	{
		//_vec3 vColPoint = *(_vec3*)&((CPhantom_Trail*)Engine::GetList(L"Layer_Trail")->back())->GetTrailList()->back();
		_float fRandX = _float(rand()%3 - 1) * _float(rand()%10) * 0.1f;
		_float fRandY = 3.f + _float(rand()%3 - 1) * _float(rand()%10) * 0.1f;
		_float fRandZ = _float(rand()%3 - 1) * _float(rand()%10) * 0.1f;

		Engine::CGameObject* pGameObject = NULL;
		if(*((CPlayer*)Engine::GetList(L"Layer_Player")->front())->GetAtkState() == CPlayer::FIRE)
		{
			pGameObject = CHitEffect::Create(m_pGraphicDev,1,*(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0] + _vec3(fRandX,fRandY,fRandZ));
			Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		}
		else if(*((CPlayer*)Engine::GetList(L"Layer_Player")->front())->GetAtkState() == CPlayer::ICE)
		{
			pGameObject = CHitEffect::Create(m_pGraphicDev,1,*(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0] + _vec3(fRandX,fRandY,fRandZ),1);
			Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		}
		else if(*((CPlayer*)Engine::GetList(L"Layer_Player")->front())->GetAtkState() == CPlayer::LIGHTNING)
		{
			pGameObject = CHitEffect::Create(m_pGraphicDev,1,*(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0] + _vec3(fRandX,fRandY,fRandZ),2,6.f);
			Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		}
		//--m_iHp;

	}


	return 0;
}


void CReaper::Render_GameObject(void)
{
	m_pTransformCom->SetUp_OnGraphicDev();
	m_pDynamicMeshCom->Play_Animation(m_fTimeDelta);
	
	m_pDynamicMeshCom->SetUp_ConstantTable(m_pShaderCom->GetEffect());
	//m_pShaderCom->GetEffect()->SetTexture("g_normalTexture",m_pTextureCom->GetTexture());
	
	if(m_isCol == true)
		m_pShaderCom->GetEffect()->SetVector("g_vHitColor",&_vec4(1.f,0.5f,0.5f,1.f));
	else
		m_pShaderCom->GetEffect()->SetVector("g_vHitColor",&_vec4(1.f,1.f,1.f,1.f));

	m_pDynamicMeshCom->Render_MeshWithShader(m_pShaderCom->GetEffect(),7);

	m_pBoxColliderCom->Render(Engine::CBoxCollider::BOX_AABB,*m_pTransformCom->GetWorldMatrix(),m_isCol,false);
}

CReaper* CReaper::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vFirstPos, _float fOffSetAngle, EYECOLOR eColor)
{
	CReaper* pInst = new CReaper(pGraphicDev,eColor);

	if(FAILED(pInst->Ready_GameObject(vFirstPos,fOffSetAngle)))
		return NULL;

	return pInst;
}

void CReaper::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pDynamicMeshCom);
	Engine::Safe_Release(m_pShaderCom);
	Engine::Safe_Release(m_pBoxColliderCom);
	Engine::Safe_Release(m_pShaderForShadow);

	m_pRightEye->SetDestroy(true);
	m_pLeftEye->SetDestroy(true);
	m_pStarGlow->SetDestroy(true);

	/*Engine::Safe_Release(m_pRightEye);
	Engine::Safe_Release(m_pLeftEye);*/
	
	Engine::CGameObject::Free();
}

void CReaper::SetState(STATE eState)
{
	m_eMyState = eState;
}

void CReaper::GetState(_uint* pState)
{
	*pState = m_eMyState;
}

void CReaper::Render_ShadowDepth()
{
	m_pTransformCom->SetUp_OnGraphicDev();
	m_pDynamicMeshCom->SetUp_ConstantTable(m_pShaderForShadow->GetEffect());

	m_pShaderForShadow->GetEffect()->SetMatrix("g_matLightView",m_pRendererCom->GetLightView());
	m_pShaderForShadow->GetEffect()->SetMatrix("g_matLightProj",m_pRendererCom->GetLightProj());

	m_pDynamicMeshCom->Render_MeshWithShader(m_pShaderForShadow->GetEffect());
}

