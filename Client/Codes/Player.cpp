#include "stdafx.h"
#include "Player.h"
#include "Export_Engine.h"
#include "Action_Camera.h"
#include "Default_Camera.h"
#include "Boss.h"
#include "PlayerTrace.h"
#include "BossDevil.h"
#include "Stage_Scene.h"
#include "Phantom_Trail.h"
#include "Party_SprayA.h"
#include "LightningCircle.h"
#include "LightningAni.h"
#include "RingWind.h"
#include "ChargeParticle_LightGlow.h"
#include "SimpleAura.h"
#include "Atk4Swing.h"
#include "Atk4Throw.h"
#include "ChargeLight.h"
#include "ShapeFire.h"
#include "BuffSelector.h"
#include "TrailParticle.h"
#include "Spiral.h"
#include "Atk4Final.h"
#include "ChargeParticle_LightRay.h"
#include "Blade1.h"
#include "Atk2Strong.h"
#include "RingDisk.h"
#include "Atk2Throw.h"
#include "WindBlade.h"
#include "Critical.h"
#include "TwistedLine.h"
#include "Distortion.h"
#include "HitEffect.h"
#include "ExpEffect.h"
#include "BossTeleBegin.h"
#include "Distortion_Mgr.h"
#include "Fog.h"
#include "WaterFall.h"
#include "SkyFog.h"
#include "StarGlow.h"
#include "EventMap.h"
#include "Reaper.h"
#include "BloodAni.h"
#include "RedScreen.h"
#include "SoundMgr.h"


CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
,m_fAngleAcc(D3DX_PI/2.f)
,m_CurIndex(10)
,m_eMyState(IDLE)
,m_isNextCombo(false)
,m_pShaderCom(NULL)
,m_isOnce(false)
,m_fScale(0.f)
,m_dwDashEffectCnt(0)
,m_pTargetCollider(NULL)
,m_fTrailSpawnTimeAcc(0.f)
,m_eAtkState(FIRE)
,m_fBlurTimeAcc(0.f)
,m_fFadeTimeAcc(0.f)
,m_pBossWorld(NULL)
,m_iAtkCollisionCtrl(0)
,m_ePrevState(IDLE)
,m_iSkillCtrl(0)
,m_bBlurOn(true)
,m_ePrevAtkState(ATK1)
,m_isSnatch(false)
,m_iSoundCtrl(0)
,m_iDebug(0)
{

}

CPlayer::~CPlayer(void)
{

}

HRESULT CPlayer::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pDynamicMeshCom = (Engine::CDynamicMesh*)Engine::Clone(COM_STAGE,L"Component_Mesh_Hagie");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Mesh_Hagie",m_pDynamicMeshCom);

	m_pBoxColliderCom = (Engine::CBoxCollider*)Engine::Clone(COM_STAGE, L"Component_BoxCollider");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_BoxCollider",m_pBoxColliderCom);
	_matrix matTemp;
	m_pBoxColliderCom->Compute_MinMax(m_pDynamicMeshCom,*D3DXMatrixIdentity(&matTemp),4);
	//m_pBoxColliderCom->Compute_MinMax(m_pDynamicMeshCom,*m_pDynamicMeshCom->GetInitMatrix(),4);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Mesh");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Mesh",m_pShaderCom);

	m_pShaderForRenderTarget = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_RenderTarget");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_RenderTarget",m_pShaderForRenderTarget);

	m_pShaderForShadow = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Shadow");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Shadow",m_pShaderForShadow);

	m_pRendererCom->SetBlurEffect(m_pShaderForShadow->GetEffect());

	m_fScale = 0.09f;

	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,_vec3(m_fScale,m_fScale,m_fScale));
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(0.f,m_fAngleAcc,0.f));
	//m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,_vec3(-200.f,1.f,0.f));
	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,_vec3(0.f,1.f,0.f));
	m_pTransformCom->Update_Component(0.f);

	m_pDynamicMeshCom->Set_AnimationSet(m_eMyState);
	m_fBlurFocus[0] = 0.5f;
	m_fBlurFocus[1] = 0.5f;

	m_fDistToLight = 50.f;
	m_pRendererCom->SetPlayerWorldMat(m_pTransformCom->GetWorldMatrix());
	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	m_fRememberTime -= fTimeDelta;
	if(Engine::Get_KeyMgr()->KeyPressed(KEY_G))
	{
		Engine::CGameObject* pGameObject = NULL;
		/*CDistortion* pDistortion = CDistortion::Create(m_pGraphicDev,_vec3(10.f,10.f,10.f),_vec3(D3DX_PI/2.f,0.f,0.f),*(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0],200.f,0.02f,1,false);
		Engine::CGameObject* pDistortionMgr = CDistortion_Mgr::Create(m_pGraphicDev,1.f);
		((CDistortion_Mgr*)pDistortionMgr)->AddDistortion(pDistortion);
		Engine::GetLayer(L"Layer_Distortion")->Add_Object(pDistortionMgr);*/

		//((CDefault_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(1.f);


	//	Engine::CGameObject* pGameObject = NULL;
		/*pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART4, m_pTransformCom->GetWorldMatrix());
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART5, m_pTransformCom->GetWorldMatrix());
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);*/

		/*pGameObject = CRedScreen::Create(m_pGraphicDev);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);*/
		
		
	//	Engine::CGameObject* pGameObject = NULL;
		/*pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY0, m_pTransformCom->GetWorldMatrix());
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);*/

		


		//m_pHumanBoss = dynamic_cast<CBoss*>(Engine::GetList(L"Layer_Boss")->front());

		//pGameObject = CBloodAni::Create(m_pGraphicDev,CBloodAni::RIGHTHAND,2.f);
		//Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
	}
	if((m_eMyState == SNATCH && m_pDynamicMeshCom->Get_Occupied() == false))// || ((CStage_Scene*)Engine::GetCurrentScene())->GetisFade() == false)
	{
		m_fFadeTimeAcc = 0.f;
		m_pRendererCom->SetisFadeOut(false);

		//if(Engine::GetList(L"Layer_Dragon")->size() == 0)
			m_pRendererCom->SetisGaussianBlur(false);
		
		m_eMyState = IDLE;
	}
	else if(((CStage_Scene*)Engine::GetCurrentScene())->GetisFade() == false)
		m_pRendererCom->SetisFadeOut(false);


	if(m_iAtkCollisionCtrl == 0 && m_pDynamicMeshCom->Get_Occupied() == true)
		m_iAtkCollisionCtrl = 1;
	else if(m_eMyState != m_ePrevState)
	{
		m_iAtkCollisionCtrl = 1;
	}
	else if(m_pDynamicMeshCom->Get_Occupied() == false && m_iAtkCollisionCtrl >= 1 )
		m_iAtkCollisionCtrl = 0;

	m_ePrevState = m_eMyState;

	SetUpOnce();
	SetMyStateWhenGetAttacked();
	SetPositonOnDashState();

	if(Engine::Get_KeyMgr()->KeyDown(KEY_Q) && m_pDynamicMeshCom->Get_Occupied() == false && m_eMyState != BUFF && Engine::GetList(L"Layer_Buff")->size() == 0)
	{
		m_eMyState = BUFF;

		Engine::CGameObject* pGameObject = CBuffSelector::Create(m_pGraphicDev);
		Engine::GetLayer(L"Layer_Buff")->Add_Object(pGameObject);
		
	}

	if(Engine::Get_KeyMgr()->KeyDown(KEY_T))
	{
		m_eMyState = REQ_BEGIN;
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false,0.5f);
	}
	if(m_pDynamicMeshCom->Get_Occupied() == true && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.7 && m_eMyState == REQ_BEGIN)
	{
		m_eMyState = REQ_END;
		m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false,0.5f);
	}

	if((m_eMyState == ATK3) && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() < 0.4)
	{
		_vec3 vLocalDashPoint = *(_vec3*)&m_pDashMoveMat->m[3][0] * m_fScale *1.5f;
		D3DXVec3TransformCoord(&vLocalDashPoint,&vLocalDashPoint,m_pTransformCom->GetWorldMatrix());
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vLocalDashPoint);
	}
	else if((m_eMyState == ATK3STRONG) && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() < 0.3)
	{
		_vec3 vLocalDashPoint = *(_vec3*)&m_pDashMoveMat->m[3][0] * m_fScale;
		D3DXVec3TransformCoord(&vLocalDashPoint,&vLocalDashPoint,m_pTransformCom->GetWorldMatrix());
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vLocalDashPoint);
	}
	else if((m_eMyState == ATK4 || m_eMyState == ATK3STRONG_FINAL) && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() < 0.3)
	{
		_vec3 vLocalDashPoint = *(_vec3*)&m_pDashMoveMat->m[3][0] * m_fScale *2.5f;
		D3DXVec3TransformCoord(&vLocalDashPoint,&vLocalDashPoint,m_pTransformCom->GetWorldMatrix());
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vLocalDashPoint);
	}
	else if((m_eMyState == ATK4STRONG || m_eMyState == ATK4STRONG_FINAL) && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.2 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() < 0.3)
	{
		_vec3 vLocalDashPoint = *(_vec3*)&m_pDashMoveMat->m[3][0] * m_fScale *0.5f;
		D3DXVec3TransformCoord(&vLocalDashPoint,&vLocalDashPoint,m_pTransformCom->GetWorldMatrix());
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vLocalDashPoint);
	}
	else if((m_eMyState == ATK2BACK || m_eMyState == ATK2FRONT || m_eMyState == ATK2LEFT || m_eMyState == ATK2LEFTEND || m_eMyState == ATK2RIGHT || m_eMyState == ATK2RIGHTEND))// && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.2 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() < 0.3)
	{
		_vec3 vLocalDashPoint = *(_vec3*)&m_pDashMoveMat->m[3][0] * m_fScale *0.5f;
		D3DXVec3TransformCoord(&vLocalDashPoint,&vLocalDashPoint,m_pTransformCom->GetWorldMatrix());
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vLocalDashPoint);
	}
	
	
	m_fTimeDelta = fTimeDelta;

	
	MovePattern(fTimeDelta);
	if(m_eMyState != m_ePrevState)
	{
		m_iSkillCtrl = 0;
		m_iSoundCtrl = 0;
	}

	MakeSound();
	MakeTrailEffect();
	
	
	Engine::CGameObject::Update_GameObject(fTimeDelta); //Component Update

	//-----------------Light Info Update----------------///

	D3DLIGHT9 tLightInfo = Engine::GetLightInfo(1);
	tLightInfo.Position = *(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0] + _vec3(0.f,5.f,0.f);
	Engine::Change_LightInfo(tLightInfo,1);

	////////////////////////////////////////////////////////

	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_NONALPHA); //Render enable
	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_SHADOW);

	return 0;
}

_int CPlayer::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if(Engine::GetList(L"Layer_EventMonster")->size() == 0)
	{
		if(m_pTargetCollider == NULL)
			m_pTargetCollider = (Engine::CBoxCollider*)Engine::GetList(L"Layer_Boss")->back()->FindComponent(Engine::CGameObject::NONUPDATE,L"Component_BoxCollider");
		else
			m_isCol = m_pBoxColliderCom->CollisionCheck_OBB(m_pTargetCollider);
	}

	if(((CBossDevil*)Engine::GetList(L"Layer_Boss")->front())->GetState() == CBossDevil::SENDPLAYER && m_isSnatch == false)
	{
		m_eMyState = SNATCH;
		m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
		m_isSnatch = true;
	}
	if(m_eMyState == SNATCH)
	{
		m_fFadeTimeAcc += fTimeDelta;
		_float fFade = 1 - m_fFadeTimeAcc*0.5f;
		m_pRendererCom->SetFadeOut(fFade);
		m_pRendererCom->SetGaussianBlurOn(m_pShaderForRenderTarget->GetEffect());

		if(fFade < 0.2)
		{
			if(Engine::GetList(L"Layer_Event")->size() == 0)
			{
				Engine::CGameObject* pGameObject = CEventMap::Create(m_pGraphicDev);
				Engine::GetLayer(L"Layer_Event")->Add_Object(pGameObject);
				Engine::GetLayer(L"Layer_Event")->SetisAlive(true);
				Engine::GetCurrentScene()->SetisEventOn(true);

				_float fDistance = 10.f;
				_vec3 vPos = *(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0] + *D3DXVec3Normalize(&_vec3(), (_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0]) * fDistance;
				vPos.y = 3.f;
				pGameObject = CReaper::Create(m_pGraphicDev,vPos,m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->y - D3DX_PI/2.f,CReaper::GREEN);
				Engine::GetLayer(L"Layer_EventMonster")->Add_Object(pGameObject);

				vPos += *D3DXVec3Normalize(&_vec3(), (_vec3*)&m_pTransformCom->GetWorldMatrix()->m[0][0]) * fDistance;
				vPos.y = 3.f;
				pGameObject = CReaper::Create(m_pGraphicDev,vPos,m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->y - D3DX_PI/2.f,CReaper::BLUE);
				Engine::GetLayer(L"Layer_EventMonster")->Add_Object(pGameObject);

				vPos -= *D3DXVec3Normalize(&_vec3(), (_vec3*)&m_pTransformCom->GetWorldMatrix()->m[0][0]) * fDistance * 2;
				vPos.y = 3.f;
				pGameObject = CReaper::Create(m_pGraphicDev,vPos,m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->y - D3DX_PI/2.f,CReaper::RED);
				Engine::GetLayer(L"Layer_EventMonster")->Add_Object(pGameObject);
				
				((CBossDevil*)Engine::GetList(L"Layer_Boss")->front())->SetState(CBossDevil::IDLE);
				
				//pGameObject = CReaper::Create(m_pGraphicDev,vPos);
				//Engine::GetLayer(L"Layer_EventMonster")->Add_Object(pGameObject);
				//pGameObject = CReaper::Create(m_pGraphicDev,vPos);
				//Engine::GetLayer(L"Layer_EventMonster")->Add_Object(pGameObject);
			}
		}
	}
	
	
	return 0;
}


void CPlayer::Render_GameObject(void)
{
	m_pTransformCom->SetUp_OnGraphicDev();
	m_pDynamicMeshCom->Play_Animation(m_fTimeDelta*2.5f);
	m_pDynamicMeshCom->SetUp_ConstantTable(m_pShaderCom->GetEffect());
	m_pDynamicMeshCom->Render_MeshWithShader(m_pShaderCom->GetEffect());

	
	//if(((CStage_Scene*)Engine::GetCurrentScene())->GetBoolLine() == true)
		m_pBoxColliderCom->Render(Engine::CBoxCollider::BOX_OBB,*m_pTransformCom->GetWorldMatrix(),m_isCol,false);

	if(Engine::Get_KeyMgr()->KeyPressed(KEY_P))
		m_bBlurOn = !m_bBlurOn;

	if(m_eMyState == DASH_N && m_bBlurOn == true)
	{
		m_fBlurTimeAcc += m_fTimeDelta;
		m_fBlurFocus[0] = 0.5f;
		m_fBlurFocus[1] = 0.2f;
		m_pShaderForRenderTarget->GetEffect()->SetFloat("g_fBlurPower",0.02f);
		m_pShaderForRenderTarget->GetEffect()->SetFloatArray("g_fFocusPoint",m_fBlurFocus,2);
		
		m_pRendererCom->SetBlurOn(m_pShaderForRenderTarget->GetEffect(),3,max(_int(6 - m_fBlurTimeAcc*5.f),1));
	}
	else if(m_eMyState == DASH_S && m_bBlurOn == true)
	{
		m_fBlurTimeAcc += m_fTimeDelta;
		m_fBlurFocus[0] = 0.5f;
		m_fBlurFocus[1] = 0.8f;
		m_pShaderForRenderTarget->GetEffect()->SetFloat("g_fBlurPower",0.02f);
		m_pShaderForRenderTarget->GetEffect()->SetFloatArray("g_fFocusPoint",m_fBlurFocus,2);
		m_pRendererCom->SetBlurOn(m_pShaderForRenderTarget->GetEffect(),3,max(_int(6 - m_fBlurTimeAcc*5.f),1));
	}
	else if(m_eMyState == DASH_E && m_bBlurOn == true)
	{
		m_fBlurTimeAcc += m_fTimeDelta;
		m_fBlurFocus[0] = 0.8f;
		m_fBlurFocus[1] = 0.5f;
		m_pShaderForRenderTarget->GetEffect()->SetFloat("g_fBlurPower",0.02f);
		m_pShaderForRenderTarget->GetEffect()->SetFloatArray("g_fFocusPoint",m_fBlurFocus,2);
		m_pRendererCom->SetBlurOn(m_pShaderForRenderTarget->GetEffect(),3,max(_int(6 - m_fBlurTimeAcc*5.f),1));
	}
	else if(m_eMyState == DASH_W && m_bBlurOn == true)
	{
		m_fBlurTimeAcc += m_fTimeDelta;
		m_fBlurFocus[0] = 0.2f;
		m_fBlurFocus[1] = 0.5f;
		m_pShaderForRenderTarget->GetEffect()->SetFloat("g_fBlurPower",0.02f);
		m_pShaderForRenderTarget->GetEffect()->SetFloatArray("g_fFocusPoint",m_fBlurFocus,2);
		m_pRendererCom->SetBlurOn(m_pShaderForRenderTarget->GetEffect(),3,max(_int(6 - m_fBlurTimeAcc*5.f),1));
	}
	else if(m_eMyState == ATK4STRONG_FINAL && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.3 && m_bBlurOn == true)
	{
		m_fBlurTimeAcc += m_fTimeDelta;
		m_fBlurFocus[0] = 0.5f;
		m_fBlurFocus[1] = 0.4f;
		m_pShaderForRenderTarget->GetEffect()->SetFloat("g_fBlurPower",0.02f);
		m_pShaderForRenderTarget->GetEffect()->SetFloatArray("g_fFocusPoint",m_fBlurFocus,2);

		m_pRendererCom->SetBlurOn(m_pShaderForRenderTarget->GetEffect(),3,max(_int(6 - m_fBlurTimeAcc*5.f),1));
	}
	else
	{
		if(((CStage_Scene*)Engine::GetCurrentScene())->GetBossEventBlur() == true)
			return;

		m_fBlurTimeAcc = 0.f;
		m_pRendererCom->SetisBlur(false);
	}

	TCHAR szTemp[256] = L"%d";
	/*wsprintf(szTemp,L"Player X : %d",(_uint)m_pDashMoveMat->_41);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(200.f,290.f),D3DXCOLOR(1.f,1.f,1.f,1.f));

	wsprintf(szTemp,L"Player Y: %d",(_uint)m_pDashMoveMat->_42);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(200.f,310.f),D3DXCOLOR(1.f,1.f,1.f,1.f));

	wsprintf(szTemp,L"Player Z : %d",(_uint)m_pDashMoveMat->_43);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(200.f,330.f),D3DXCOLOR(1.f,1.f,1.f,1.f));*/

	/*wsprintf(szTemp,L"Sound : %d",(_uint)m_iSoundCtrl);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(200.f,350.f),D3DXCOLOR(1.f,1.f,1.f,1.f));

	wsprintf(szTemp,L"iDebug : %d",(_uint)m_iDebug);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(200.f,390.f),D3DXCOLOR(1.f,1.f,1.f,1.f));

	wsprintf(szTemp,L"State : %d",(_uint)m_eMyState);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(200.f,500.f),D3DXCOLOR(1.f,1.f,1.f,1.f));*/

}
void CPlayer::Render_ShadowDepth()
{

	m_pTransformCom->SetUp_OnGraphicDev();
	m_pDynamicMeshCom->SetUp_ConstantTable(m_pShaderForShadow->GetEffect());
	
	m_pShaderForShadow->GetEffect()->SetMatrix("g_matLightView",m_pRendererCom->GetLightView());
	m_pShaderForShadow->GetEffect()->SetMatrix("g_matLightProj",m_pRendererCom->GetLightProj());

	m_pDynamicMeshCom->Render_MeshWithShader(m_pShaderForShadow->GetEffect());

}

void CPlayer::MovePattern(const _float& fTimeDelta)
{

	


	if(m_pDynamicMeshCom->Get_Occupied() == false)
	{
		//m_iSkillCtrl = 0;

		if(m_eMyState == ATK2LEFT)
		{
			m_eMyState = ATK2LEFTEND;
			m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
			
		}
		if(m_eMyState == ATK2RIGHT)
		{
			m_eMyState = ATK2RIGHTEND;
			m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
		}

		if(m_isNextCombo == false)
		{
			if(Engine::Get_KeyMgr()->KeyPressed(KEY_LBUTTON))
			{
				if(Engine::Get_DIKeyState(DIK_W) & 0x80)
				{
					_vec3 vCamDir = *m_pCamTarget - *m_pCamPos;
					m_pTransformCom->SetAngleWithLook(*D3DXVec3Normalize(&vCamDir,&vCamDir));
				}
				else if(Engine::Get_DIKeyState(DIK_S) & 0x80)
				{
					_vec3 vCamDir = *m_pCamTarget - *m_pCamPos;
					m_pTransformCom->SetAngleWithLook(*D3DXVec3Normalize(&vCamDir,&vCamDir) * -1.f);
				}else if(Engine::Get_DIKeyState(DIK_D) & 0x80)
				{
					_vec3 vCamDir = *m_pCamTarget - *m_pCamPos;
					m_pTransformCom->SetAngleWithLook(*D3DXVec3Normalize(&vCamDir,&vCamDir));
					m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,*m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE) + _vec3(0.f,D3DX_PI/2.f,0.f));
				}
				else if(Engine::Get_DIKeyState(DIK_A) & 0x80)
				{
					_vec3 vCamDir = *m_pCamTarget - *m_pCamPos;
					m_pTransformCom->SetAngleWithLook(*D3DXVec3Normalize(&vCamDir,&vCamDir));
					m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,*m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE) - _vec3(0.f,D3DX_PI/2.f,0.f));
				}

				if(m_fRememberTime > 0.f && m_ePrevAtkState != ATK4)
				{
					if(m_ePrevAtkState == ATK3 && m_eAtkState == FIRE)
						m_eMyState = ++m_ePrevAtkState;
					else if(m_ePrevAtkState == ATK2 || m_ePrevAtkState == ATK1)
					{
						m_eMyState = ++m_ePrevAtkState;
						if(m_ePrevAtkState == ATK1)
						{
							CSoundMgr::GetInstance()->PlaySound(L"hagie_attack_02.wav",CSoundMgr::HAGIE_ATK2);
						}
					}
				}
				else
				{
					m_eMyState = ATK1;

					CSoundMgr::GetInstance()->PlaySound(L"hagie_attack_01.wav",CSoundMgr::HAGIE_ATK1);
					CSoundMgr::GetInstance()->PlaySound(L"hagie_spin_phantomdagger_01.wav",CSoundMgr::HWEAPON1);
				}

				m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
			}
			else if(!(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80))
			{
				NormalMoving(fTimeDelta);
			}
		}
		else if(m_isNextCombo == true)
		{
			//m_pDynamicMeshCom->Set_AnimationSet(++m_eMyState,false);
			m_isNextCombo = false;
		}
		ForceAction(fTimeDelta);

		if(Engine::Get_KeyMgr()->KeyDown(KEY_RBUTTON))
		{

			_vec3 vCamDir = *m_pCamTarget - *m_pCamPos;
			m_pTransformCom->SetAngleWithLook(*D3DXVec3Normalize(&vCamDir,&vCamDir));
			
			if(m_fRememberTime > 0.f)
			{
				if(m_ePrevAtkState == ATK2 && m_eAtkState == ICE)
				{
					if(Engine::Get_DIKeyState(DIK_S) & 0x80)
						m_eMyState = ATK2BACK;
					else if(Engine::Get_DIKeyState(DIK_W) & 0x80)
						m_eMyState = ATK2FRONT;
					else if(Engine::Get_DIKeyState(DIK_A) & 0x80)
						m_eMyState = ATK2LEFT;
					else if(Engine::Get_DIKeyState(DIK_D) & 0x80)
						m_eMyState = ATK2RIGHT;
					else
						m_eMyState = ATK2STRONG;

					CSoundMgr::GetInstance()->PlaySound(L"hagie_attack_02",CSoundMgr::HAGIE_ATK2);
					CSoundMgr::GetInstance()->PlaySound(L"hagie_attack_magic_02",CSoundMgr::HAGIE_ATK2STRONG);
					m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
					m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
				}
				else if(m_ePrevAtkState == ATK3 && m_eAtkState == LIGHTNING)
				{
					m_eMyState = ATK3STRONG;
					m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
					m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
				}
				else if(m_ePrevAtkState == ATK4 && m_eAtkState == FIRE)
				{
					m_eMyState = ATK4STRONG;
					m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
					m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
				}
				else if(m_ePrevAtkState == ATK3STRONG && m_eAtkState == LIGHTNING)
				{
					m_eMyState = ATK3STRONG_DURING;
					m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
					m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
				}
				else if(m_ePrevAtkState == ATK4STRONG && m_eAtkState == FIRE)
				{
					m_eMyState = ATK4STRONG_DURING;
					m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
					m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
				}
			}
		}
		if(GetAsyncKeyState(VK_RBUTTON))
		{
			
			_vec3 vCamDir = *m_pCamTarget - *m_pCamPos;
			m_pTransformCom->SetAngleWithLook(*D3DXVec3Normalize(&vCamDir,&vCamDir));
			
			if(m_ePrevAtkState == ATK3STRONG_DURING)
			{
				m_eMyState = ATK3STRONG_DURING;
				m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
				m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
			}
			if(m_ePrevAtkState == ATK4STRONG_DURING)
			{
				m_eMyState = ATK4STRONG_DURING;
				m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
				m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
			}
		}
		
	}
	else if(m_pDynamicMeshCom->Get_Occupied() == true)
	{
		if(Engine::Get_KeyMgr()->KeyDown(KEY_LBUTTON) && m_eMyState != DASH_E && m_eMyState != DASH_W && m_eMyState != DASH_N && m_eMyState != DASH_S && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.2)
		{
			if(m_eMyState == ATK1 || m_eMyState == ATK2 || (m_eMyState == ATK3 && m_eAtkState == FIRE))
			{
				if(Engine::Get_DIKeyState(DIK_W) & 0x80)
				{
					_vec3 vCamDir = *m_pCamTarget - *m_pCamPos;
					m_pTransformCom->SetAngleWithLook(*D3DXVec3Normalize(&vCamDir,&vCamDir));
				}
				else if(Engine::Get_DIKeyState(DIK_S) & 0x80)
				{
					_vec3 vCamDir = *m_pCamTarget - *m_pCamPos;
					m_pTransformCom->SetAngleWithLook(*D3DXVec3Normalize(&vCamDir,&vCamDir) * -1.f);
				}else if(Engine::Get_DIKeyState(DIK_D) & 0x80)
				{
					_vec3 vCamDir = *m_pCamTarget - *m_pCamPos;
					m_pTransformCom->SetAngleWithLook(*D3DXVec3Normalize(&vCamDir,&vCamDir));
					m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,*m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE) + _vec3(0.f,D3DX_PI/2.f,0.f));
				}
				else if(Engine::Get_DIKeyState(DIK_A) & 0x80)
				{
					_vec3 vCamDir = *m_pCamTarget - *m_pCamPos;
					m_pTransformCom->SetAngleWithLook(*D3DXVec3Normalize(&vCamDir,&vCamDir));
					m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,*m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE) - _vec3(0.f,D3DX_PI/2.f,0.f));
				}
				m_isNextCombo = true;
				m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
				m_pDynamicMeshCom->Set_AnimationSet(++m_eMyState,false);
				if(m_eMyState == ATK2)
				{
					CSoundMgr::GetInstance()->PlaySound(L"hagie_attack_02.wav",CSoundMgr::HAGIE_ATK2);
					CSoundMgr::GetInstance()->PlaySound(L"hagie_spin_phantomdagger_02.wav",CSoundMgr::HWEAPON2);
				}
				if(m_eMyState == ATK3)
				{
					CSoundMgr::GetInstance()->PlaySound(L"hagie_attack_03.wav",CSoundMgr::HAGIE_ATK3);
					CSoundMgr::GetInstance()->PlaySound(L"hagie_spin_phantomdagger_03.wav",CSoundMgr::HWEAPON3);
				}
				m_fTrailSpawnTimeAcc = 0.f;
			}
		}
		else if(Engine::Get_KeyMgr()->KeyDown(KEY_RBUTTON))
		{
			if(m_eMyState != DASH_E && m_eMyState != DASH_W && m_eMyState != DASH_N && m_eMyState != DASH_S && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.2)
			{
				if(m_eMyState == ATK3 && m_eAtkState == LIGHTNING)
				{
					m_eMyState = ATK3STRONG;
					m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
					m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);						

				}
				else if(m_eMyState == ATK4 && m_eAtkState == FIRE)
				{
					m_eMyState = ATK4STRONG;
					m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
					m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
				}
				else if(m_eMyState == ATK2 && m_eAtkState == ICE && !(Engine::Get_DIKeyState(DIK_W) & 0x80) && !(Engine::Get_DIKeyState(DIK_A) & 0x80)&& !(Engine::Get_DIKeyState(DIK_S) & 0x80)&& !(Engine::Get_DIKeyState(DIK_D) & 0x80))
				{
					m_eMyState = ATK2STRONG;
					m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
					m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
					CSoundMgr::GetInstance()->PlaySound(L"hagie_attack_strong_icevoice.wav",CSoundMgr::HAGIE_ATK2);
					CSoundMgr::GetInstance()->PlaySound(L"hagie_attack_magic_02.wav",CSoundMgr::HAGIE_ATK2STRONG);
				}
				else if((m_eMyState == ATK2 || m_eMyState == ATK2STRONG || m_eMyState == ATK2LEFTEND || m_eMyState == ATK2RIGHTEND || m_eMyState == ATK2FRONT || m_eMyState == ATK2BACK ) && m_eAtkState == ICE)
				{
					if(Engine::Get_DIKeyState(DIK_S) & 0x80)
						m_eMyState = ATK2BACK;
					else if(Engine::Get_DIKeyState(DIK_W) & 0x80)
						m_eMyState = ATK2FRONT;
					else if(Engine::Get_DIKeyState(DIK_A) & 0x80)
						m_eMyState = ATK2LEFT;
					else if(Engine::Get_DIKeyState(DIK_D) & 0x80)
						m_eMyState = ATK2RIGHT;

					m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
					m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
					CSoundMgr::GetInstance()->PlaySound(L"hagie_attack_strong_icevoice.wav",CSoundMgr::HAGIE_ATK2);
					CSoundMgr::GetInstance()->PlaySound(L"hagie_attack_magic_02.wav",CSoundMgr::HAGIE_ATK2STRONG);
				}
			}			
		}
		else if(m_eMyState == ATK3STRONG && GetAsyncKeyState(VK_RBUTTON) && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.7)
		{
			m_eMyState = ATK3STRONG_DURING;
			m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
			m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);

			CSoundMgr::GetInstance()->PlaySound(L"effect_lightning_charge.wav",CSoundMgr::HAGIE_ATK3);
		}
		else if(m_eMyState == ATK3STRONG_DURING)
		{
			if(m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.8)
			{
				m_eMyState = ATK3STRONG_FINAL;
				m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
				m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
				/*m_pDynamicMeshCom->isNewTrackPositionZero();*/
				CSoundMgr::GetInstance()->PlaySound(L"hagie_attack_strong_07.wav",CSoundMgr::HAGIE_ATK4);
			}
			if(!GetAsyncKeyState(VK_RBUTTON) && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.3)
			{
				m_eMyState = ATK3STRONG_FINAL;
				m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
				m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
				CSoundMgr::GetInstance()->PlaySound(L"hagie_attack_strong_07.wav",CSoundMgr::HAGIE_ATK3);
			}
		}
		else if(m_eMyState == ATK4STRONG && GetAsyncKeyState(VK_RBUTTON) && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.4)
		{
			m_eMyState = ATK4STRONG_DURING;
			m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
			m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
		}
		else if(m_eMyState == ATK4STRONG_DURING)
		{
			if(m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.8)
			{
				m_eMyState = ATK4STRONG_FINAL;
				m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
				m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
			}
			if(!GetAsyncKeyState(VK_RBUTTON) && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.3)
			{
				m_eMyState = ATK4STRONG_FINAL;
				m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
				m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
			}
		}
		
		ForceAction(fTimeDelta);
	}

	if(m_eMyState == ATK1 || m_eMyState == ATK2 || m_eMyState == ATK3 || m_eMyState == ATK4 || m_eMyState == ATK3STRONG || m_eMyState == ATK3STRONG_DURING || m_eMyState == ATK4STRONG || m_eMyState == ATK4STRONG_DURING)
	{
		m_ePrevAtkState = m_eMyState;
		m_fRememberTime = 1.f;
	}
}



CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer* pInst = new CPlayer(pGraphicDev);

	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CPlayer::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pDynamicMeshCom);
	Engine::Safe_Release(m_pBoxColliderCom);
	Engine::Safe_Release(m_pShaderCom);
	Engine::Safe_Release(m_pShaderForRenderTarget);
	Engine::Safe_Release(m_pShaderForShadow);

	Engine::CGameObject::Free();
}


void CPlayer::NormalMoving(const _float& fTimeDelta)
{
	if(Engine::Get_DIKeyState(DIK_W) & 0x80 && Engine::Get_DIKeyState(DIK_D) & 0x80)
	{
		if(true == m_pTransformCom->Align_TargetDirection(m_pTransformCom,m_CurIndex,Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_LOOK) + Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_RIGHT),fTimeDelta))
			Engine::Get_NaviMgr()->MoveOnNavi(m_pTransformCom,m_CurIndex,fTimeDelta);
		m_eMyState = RUN;
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState);
	}
	else if(Engine::Get_DIKeyState(DIK_W) & 0x80 && Engine::Get_DIKeyState(DIK_A) & 0x80)
	{
		if(true == m_pTransformCom->Align_TargetDirection(m_pTransformCom,m_CurIndex,Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_LOOK) - Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_RIGHT),fTimeDelta))
			Engine::Get_NaviMgr()->MoveOnNavi(m_pTransformCom,m_CurIndex,fTimeDelta);
		m_eMyState = RUN;
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState);
	}
	else if(Engine::Get_DIKeyState(DIK_S) & 0x80 && Engine::Get_DIKeyState(DIK_D) & 0x80)
	{
		if(true == m_pTransformCom->Align_TargetDirection(m_pTransformCom,m_CurIndex,-Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_LOOK) + Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_RIGHT),fTimeDelta))
			Engine::Get_NaviMgr()->MoveOnNavi(m_pTransformCom,m_CurIndex,fTimeDelta);
		m_eMyState = RUN;
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState);
	}
	else if(Engine::Get_DIKeyState(DIK_S) & 0x80 && Engine::Get_DIKeyState(DIK_A) & 0x80)
	{
		if(true == m_pTransformCom->Align_TargetDirection(m_pTransformCom,m_CurIndex,-Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_LOOK) - Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_RIGHT),fTimeDelta))
			Engine::Get_NaviMgr()->MoveOnNavi(m_pTransformCom,m_CurIndex,fTimeDelta);
		m_eMyState = RUN;
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState);
	}
	else if(Engine::Get_DIKeyState(DIK_W) & 0x80)
	{
		if(true == m_pTransformCom->Align_TargetDirection(m_pTransformCom,m_CurIndex,Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_LOOK),fTimeDelta))
			Engine::Get_NaviMgr()->MoveOnNavi(m_pTransformCom,m_CurIndex,fTimeDelta);
		m_eMyState = RUN;
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState);
	}
	else if(Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		if(true == m_pTransformCom->Align_TargetDirection(m_pTransformCom,m_CurIndex,-Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_LOOK),fTimeDelta))
			Engine::Get_NaviMgr()->MoveOnNavi(m_pTransformCom,m_CurIndex,fTimeDelta);
		m_eMyState = RUN;
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState);
	}
	else if(Engine::Get_DIKeyState(DIK_D) & 0x80)
	{
		if(true == m_pTransformCom->Align_TargetDirection(m_pTransformCom,m_CurIndex,Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_RIGHT),fTimeDelta))
			Engine::Get_NaviMgr()->MoveOnNavi(m_pTransformCom,m_CurIndex,fTimeDelta);
		m_eMyState = RUN;
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState);
	}
	else if(Engine::Get_DIKeyState(DIK_A) & 0x80)
	{
		if(true == m_pTransformCom->Align_TargetDirection(m_pTransformCom,m_CurIndex,-Engine::MyGetCamPosition(m_pGraphicDev,Engine::VEC_RIGHT),fTimeDelta))
			Engine::Get_NaviMgr()->MoveOnNavi(m_pTransformCom,m_CurIndex,fTimeDelta);
		m_eMyState = RUN;
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState);
	}
	else if(m_isNextCombo == false)
	{
		m_eMyState = IDLE;
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState);
	}
}

void CPlayer::ForceAction(const _float& fTimeDelta)
{
	if(Engine::Get_DIKeyState(DIK_W) & 0x80 && Engine::Get_KeyMgr()->KeyPressed(KEY_SPACE))
	{
		m_eMyState = DASH_N;
		m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false,2.f);
		m_dwDashEffectCnt = 0;

		CSoundMgr::GetInstance()->PlaySound(L"Action_Dash.wav",CSoundMgr::BUFF);

		

	}
	else if(Engine::Get_DIKeyState(DIK_D) & 0x80 && Engine::Get_KeyMgr()->KeyPressed(KEY_SPACE))
	{

		m_eMyState = DASH_E;
		m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false,2.f);
		m_dwDashEffectCnt = 0;
		CSoundMgr::GetInstance()->PlaySound(L"Action_Dash.wav",CSoundMgr::BUFF);
	}
	else if(Engine::Get_DIKeyState(DIK_S) & 0x80 && Engine::Get_KeyMgr()->KeyPressed(KEY_SPACE))
	{

		m_eMyState = DASH_S;
		m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false,2.f);
		m_dwDashEffectCnt = 0;
		CSoundMgr::GetInstance()->PlaySound(L"Action_Dash.wav",CSoundMgr::BUFF);
	}
	else if(Engine::Get_DIKeyState(DIK_A) & 0x80 && Engine::Get_KeyMgr()->KeyPressed(KEY_SPACE))
	{

		m_eMyState = DASH_W;
		m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false,2.f);
		m_dwDashEffectCnt = 0;
		CSoundMgr::GetInstance()->PlaySound(L"Action_Dash.wav",CSoundMgr::BUFF);
	}
}

void CPlayer::SetUpOnce(void)
{
	if(m_isOnce == false)
	{
		m_isOnce = true;

		list<CGameObject*>* pList = Engine::GetList(L"Layer_Boss");
		if(NULL == pList)
			return;

		Engine::CGameObject* pBoss = NULL;
		if(((CStage_Scene*)Engine::GetCurrentScene())->GetBossSpawn() == false)
			pBoss = dynamic_cast<CBoss*>(pList->front());
		else
			pBoss = dynamic_cast<CBossDevil*>(pList->front());
		
		if(NULL == pBoss)
			return;

		Engine::CTransform* pBossTransform = (Engine::CTransform*)pBoss->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
		m_pBossWorld = pBossTransform->GetWorldMatrix();

		Engine::D3DXFRAME_DERIVED* pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(m_pDynamicMeshCom->GetRootFrame(),"ValveBiped_Anim_Attachment_CAM_Origin");
		m_pDashMoveMat = &pFrameFound->CombinedMatrix;

		m_pCamPos = ((CDefault_Camera*)Engine::GetList(L"Layer_Cam")->front())->GetCamPos();
		m_pCamTarget = ((CDefault_Camera*)Engine::GetList(L"Layer_Cam")->front())->GetCamTarget();

		//Effect Layer Add
		m_pEffectLayer = Engine::GetLayer(L"Layer_Effect");
	}
}

void CPlayer::SetPositonOnDashState(void)
{
	if((m_eMyState == DASH_N || m_eMyState == DASH_E || m_eMyState == DASH_S || m_eMyState == DASH_W) && m_pDynamicMeshCom->Get_Occupied() == true)
	{
		/*m_pDynamicMeshCom->Play_Animation(0.f);
		if(m_dwDashEffectCnt == 0)
		{
			m_dwDashEffectCnt = 1;
			CPlayerTrace* pPlayerTrace = CPlayerTrace::Create(m_pGraphicDev,m_pTransformCom,m_pDynamicMeshCom,m_eMyState);
			Engine::GetLayer(L"Layer_Effect")->Add_Object(pPlayerTrace);
		}

		_float fAlpha = (_float)m_pDynamicMeshCom->Get_CurrentRatioFromAnimation();
		if(fAlpha > 0.5f)
			m_pShaderCom->GetEffect()->SetFloat("g_AlphaForPlayer",fAlpha);
		else
			m_pShaderCom->GetEffect()->SetFloat("g_AlphaForPlayer",0.f);*/

		_vec3 vLocalDashPoint = *(_vec3*)&m_pDashMoveMat->m[3][0] * m_fScale;
		_matrix matView,matScale,matTrans;
		m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
		D3DXMatrixInverse(&matView,0,&matView);
		_matrix CamRot = Engine::MyGetRotMatFromWorldMat(matView);
		_vec3 vCamLook = Engine::MyGetVec3FromMat(Engine::VEC_LOOK,CamRot);
		vCamLook.y = 0;
		memcpy(&CamRot.m[2][0],D3DXVec3Normalize(&vCamLook,&vCamLook),sizeof(_float)*3);
		D3DXMatrixScaling(&matScale,m_fScale*1.5f,m_fScale*1.5f,m_fScale*1.5f);
		D3DXMatrixTranslation(&matTrans,m_pTransformCom->GetWorldMatrix()->_41,m_pTransformCom->GetWorldMatrix()->_42,m_pTransformCom->GetWorldMatrix()->_43);
		_matrix ModifiedWorldMat = matScale * CamRot * matTrans;

		D3DXVec3TransformCoord(&vLocalDashPoint,&vLocalDashPoint,&ModifiedWorldMat);
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vLocalDashPoint);
	}
	else if((_float)m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() < 1.f)
		m_pShaderCom->GetEffect()->SetFloat("g_AlphaForPlayer",1.f);
}

void CPlayer::SetMyStateWhenGetAttacked(void)
{
	if(m_pBossWorld != NULL &&m_isCol == true && m_eMyState != DAMAGED_FRONT && m_eMyState != DAMAGED_BACK && m_eMyState != DAMAGED_LEFT && m_eMyState != DAMAGED_RIGHT && m_eMyState != ATK3STRONG_DURING && m_eMyState != ATK4STRONG_DURING)
	{
		_vec3 vDirFromPlayerToBoss = *(_vec3*)&m_pBossWorld->m[3][0] - *(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0];
		_vec3 vPlayerLook = *(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0];
		_float fCos = D3DXVec3Dot(D3DXVec3Normalize(&vDirFromPlayerToBoss,&vDirFromPlayerToBoss),D3DXVec3Normalize(&vPlayerLook,&vPlayerLook));
		_float fAngle = 0.f;
		if((*D3DXVec3Cross(&_vec3(),&vDirFromPlayerToBoss,&vPlayerLook)).y > 0.f) // 
			fAngle = acosf(fCos);
		else
			fAngle = D3DX_PI * 2.f - acosf(fCos);

		if(fAngle > D3DX_PI/4.f && fAngle < D3DX_PI/4.f + D3DX_PI/2.f)
			m_eMyState = DAMAGED_LEFT;
		else if(fAngle > 3.f*D3DX_PI/4.f && fAngle < 3.f*D3DX_PI/4.f + D3DX_PI/2.f)
			m_eMyState = DAMAGED_BACK;
		else if(fAngle > 5.f*D3DX_PI/4.f && fAngle < 5.f*D3DX_PI/4.f + D3DX_PI/2.f)
			m_eMyState = DAMAGED_RIGHT;
		else if(fAngle > 7.f*D3DX_PI/4.f && fAngle < D3DX_PI*2 || fAngle < D3DX_PI/4.f)
			m_eMyState = DAMAGED_FRONT;



		//CSoundMgr::GetInstance()->PlaySound(L"hagie_hurt_weak_01.wav",CSoundMgr::HAGIE_ATK1);
		//m_eMyState = DAMAGED_BACK;
		m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
	}

	if(m_eMyState == DAMAGED_BACK || m_eMyState == DAMAGED_FRONT || m_eMyState == DAMAGED_LEFT || m_eMyState == DAMAGED_RIGHT)
	{
		//CSoundMgr::GetInstance()->PlaySound(L"hagie_hurt_weak_02.wav",CSoundMgr::HAGIE_ATK1);
		_vec3 vLocalDashPoint = *(_vec3*)&m_pDashMoveMat->m[3][0] * m_fScale;
		D3DXVec3TransformCoord(&vLocalDashPoint,&vLocalDashPoint,m_pTransformCom->GetWorldMatrix());
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vLocalDashPoint);
	}
}

void CPlayer::MakeTrailEffect(void)
{
	/*if(m_fTrailSpawnTimeAcc == 0.f && m_eMyState == DASH_W)
	{
		Engine::CGameObject* pGameObject = CAtk4Swing::Create(m_pGraphicDev,20);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
	}*/
	if(m_iSkillCtrl == 0 && m_eMyState == ATK1)
	{
		++m_iSkillCtrl;

		for(_uint i = 0 ; i<1; ++i)
		{
			vector<EFFECTDATA>::iterator iter = ((CStage_Scene*)Engine::GetCurrentScene())->GetEffectVector(L"Atk1")->begin();
			vector<EFFECTDATA>::iterator iter_end = ((CStage_Scene*)Engine::GetCurrentScene())->GetEffectVector(L"Atk1")->end();
			for(; iter!=iter_end ; ++iter)
			{
				Engine::CGameObject* pGameObject = CPhantom_Trail::Create(m_pGraphicDev,m_pDynamicMeshCom,(*iter));
				Engine::GetLayer(L"Layer_Trail")->Add_Object(pGameObject);
			}
		}

		Engine::CGameObject* pGameObject = CChargeLight::Create(m_pGraphicDev,CChargeLight::RIGHTHAND,&_vec3(0.f,0.f,0.f),5,0.3f,m_eAtkState);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

	}
	else if(m_iSkillCtrl == 0 && m_eMyState == ATK2)
	{
		++m_iSkillCtrl;

		for(_uint i = 0 ; i<1; ++i)
		{
			vector<EFFECTDATA>::iterator iter = ((CStage_Scene*)Engine::GetCurrentScene())->GetEffectVector(L"Atk2")->begin();
			vector<EFFECTDATA>::iterator iter_end = ((CStage_Scene*)Engine::GetCurrentScene())->GetEffectVector(L"Atk2")->end();
			for(; iter!=iter_end ; ++iter)
			{
				Engine::CGameObject* pGameObject = CPhantom_Trail::Create(m_pGraphicDev,m_pDynamicMeshCom,(*iter));
				Engine::GetLayer(L"Layer_Trail")->Add_Object(pGameObject);
			}
		}

		Engine::CGameObject* pGameObject = CChargeLight::Create(m_pGraphicDev,CChargeLight::LEFTHAND,&_vec3(0.f,0.f,0.f),5,0.3f,m_eAtkState);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
	}
	else if(m_iSkillCtrl == 0 && m_eMyState == ATK3)
	{
		m_iSkillCtrl++;

		for(_uint i = 0 ; i<1; ++i)
		{
			vector<EFFECTDATA>::iterator iter = ((CStage_Scene*)Engine::GetCurrentScene())->GetEffectVector(L"Atk3")->begin();
			vector<EFFECTDATA>::iterator iter_end = ((CStage_Scene*)Engine::GetCurrentScene())->GetEffectVector(L"Atk3")->end();
			for(; iter!=iter_end ; ++iter)
			{
				EFFECTDATA tEffectData = (*iter);
				tEffectData.fLifeTime *= 0.5f;
				Engine::CGameObject* pGameObject = CPhantom_Trail::Create(m_pGraphicDev,m_pDynamicMeshCom,tEffectData);
				Engine::GetLayer(L"Layer_Trail")->Add_Object(pGameObject);
			}
		}

		Engine::CGameObject* pGameObject = CChargeLight::Create(m_pGraphicDev,CChargeLight::RIGHTHAND,&_vec3(0.f,0.f,0.f),5,0.3f,m_eAtkState);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

		pGameObject = CChargeLight::Create(m_pGraphicDev,CChargeLight::LEFTHAND,&_vec3(0.f,0.f,0.f),5,0.3f,m_eAtkState);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
	}
	else if(m_iSkillCtrl == 0 && m_eMyState == ATK4)
	{
		++m_iSkillCtrl;

		Engine::CGameObject* pGameObject = NULL;
		_float fRandAngle = D3DX_PI/2.f;
		_float fRandDist = 10.f;
		
		_vec3 vFirstLook = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0]);
		_matrix matRotY;
		D3DXMatrixRotationY(&matRotY , fRandAngle);
		D3DXVec3TransformNormal(&vFirstLook,&vFirstLook,&matRotY);

		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT12,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);


		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT13,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);



		
		pGameObject = CChargeLight::Create(m_pGraphicDev,CChargeLight::RIGHTHAND,&_vec3(0.f,0.f,0.f),5,0.3f,m_eAtkState);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

		pGameObject = CChargeLight::Create(m_pGraphicDev,CChargeLight::LEFTHAND,&_vec3(0.f,0.f,0.f),5,0.3f,m_eAtkState);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
	}
	else if(m_iSkillCtrl == 1 && m_eMyState == ATK4 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.05)
	{
		++m_iSkillCtrl;

		Engine::CGameObject* pGameObject = NULL;

		_float fRandAngle = D3DX_PI/2.f - D3DXToRadian(15.f);
		_float fRandDist = 10.f;

		_vec3 vFirstLook = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0]);
		_matrix matRotY;
		D3DXMatrixRotationY(&matRotY , fRandAngle);
		D3DXVec3TransformNormal(&vFirstLook,&vFirstLook,&matRotY);

		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT12,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT13,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		
	}
	else if(m_iSkillCtrl == 2 && m_eMyState == ATK4 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.1)
	{
		++m_iSkillCtrl;

		Engine::CGameObject* pGameObject = NULL;

		_float fRandAngle = D3DX_PI/2.f - D3DXToRadian(30.f);
		_float fRandDist = 10.f;

		_vec3 vFirstLook = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0]);
		_matrix matRotY;
		D3DXMatrixRotationY(&matRotY , fRandAngle);
		D3DXVec3TransformNormal(&vFirstLook,&vFirstLook,&matRotY);

		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT12,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT13,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

	}
	else if(m_iSkillCtrl == 3 && m_eMyState == ATK4 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.15)
	{
		++m_iSkillCtrl;

		Engine::CGameObject* pGameObject = NULL;

		_float fRandAngle = D3DX_PI/2.f - D3DXToRadian(45.f);
		_float fRandDist = 10.f;

		_vec3 vFirstLook = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0]);
		_matrix matRotY;
		D3DXMatrixRotationY(&matRotY , fRandAngle);
		D3DXVec3TransformNormal(&vFirstLook,&vFirstLook,&matRotY);

		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT12,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT13,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

	}
	else if(m_iSkillCtrl == 4 && m_eMyState == ATK4 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.2)
	{
		++m_iSkillCtrl;

		Engine::CGameObject* pGameObject = NULL;

		_float fRandAngle = D3DX_PI/2.f - D3DXToRadian(60.f);
		_float fRandDist = 10.f;

		_vec3 vFirstLook = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0]);
		_matrix matRotY;
		D3DXMatrixRotationY(&matRotY , fRandAngle);
		D3DXVec3TransformNormal(&vFirstLook,&vFirstLook,&matRotY);

		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT12,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT13,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

	}
	else if(m_iSkillCtrl == 5 && m_eMyState == ATK4 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.25)
	{
		++m_iSkillCtrl;

		Engine::CGameObject* pGameObject = NULL;

		_float fRandAngle = D3DX_PI/2.f - D3DXToRadian(75.f);
		_float fRandDist = 10.f;

		_vec3 vFirstLook = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0]);
		_matrix matRotY;
		D3DXMatrixRotationY(&matRotY , fRandAngle);
		D3DXVec3TransformNormal(&vFirstLook,&vFirstLook,&matRotY);

		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT12,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT13,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

	}
	else if(m_iSkillCtrl == 6 && m_eMyState == ATK4 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.3)
	{
		++m_iSkillCtrl;

		Engine::CGameObject* pGameObject = NULL;

		_float fRandAngle = D3DX_PI/2.f - D3DXToRadian(90.f);
		_float fRandDist = 10.f;

		_vec3 vFirstLook = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0]);
		_matrix matRotY;
		D3DXMatrixRotationY(&matRotY , fRandAngle);
		D3DXVec3TransformNormal(&vFirstLook,&vFirstLook,&matRotY);

		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT12,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT13,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

	}
	else if(m_iSkillCtrl == 7 && m_eMyState == ATK4 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.35)
	{
		++m_iSkillCtrl;

		Engine::CGameObject* pGameObject = NULL;

		_float fRandAngle = D3DX_PI/2.f - D3DXToRadian(105.f);
		_float fRandDist = 10.f;

		_vec3 vFirstLook = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0]);
		_matrix matRotY;
		D3DXMatrixRotationY(&matRotY , fRandAngle);
		D3DXVec3TransformNormal(&vFirstLook,&vFirstLook,&matRotY);

		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT12,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT13,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
	}
	else if(m_iSkillCtrl == 8 && m_eMyState == ATK4 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.4)
	{
		++m_iSkillCtrl;

		Engine::CGameObject* pGameObject = NULL;

		_float fRandAngle = D3DX_PI/2.f - D3DXToRadian(120.f);
		_float fRandDist = 10.f;

		_vec3 vFirstLook = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0]);
		_matrix matRotY;
		D3DXMatrixRotationY(&matRotY , fRandAngle);
		D3DXVec3TransformNormal(&vFirstLook,&vFirstLook,&matRotY);

		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT12,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT13,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
	}
	else if(m_iSkillCtrl == 9 && m_eMyState == ATK4 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.45)
	{
		++m_iSkillCtrl;

		Engine::CGameObject* pGameObject = NULL;

		_float fRandAngle = D3DX_PI/2.f - D3DXToRadian(135.f);
		_float fRandDist = 10.f;

		_vec3 vFirstLook = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0]);
		_matrix matRotY;
		D3DXMatrixRotationY(&matRotY , fRandAngle);
		D3DXVec3TransformNormal(&vFirstLook,&vFirstLook,&matRotY);

		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT12,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT13,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
	}
	else if(m_iSkillCtrl == 10 && m_eMyState == ATK4 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.5)
	{
		++m_iSkillCtrl;

		Engine::CGameObject* pGameObject = NULL;

		_float fRandAngle = D3DX_PI/2.f - D3DXToRadian(150.f);
		_float fRandDist = 10.f;

		_vec3 vFirstLook = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0]);
		_matrix matRotY;
		D3DXMatrixRotationY(&matRotY , fRandAngle);
		D3DXVec3TransformNormal(&vFirstLook,&vFirstLook,&matRotY);

		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT12,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT13,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
	}



	else if(m_eMyState == ATK3STRONG && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.1 && m_iSkillCtrl == 0)
	{
		++m_iSkillCtrl;
		for(_uint i=0; i<4; ++i)
		{
			Engine::CGameObject* pGameObject = CRingWind::Create(m_pGraphicDev,i * D3DX_PI/2.f);
			m_pEffectLayer->Add_Object(pGameObject);
			pGameObject = CRingWind::Create(m_pGraphicDev,i * D3DX_PI/2.f);
			m_pEffectLayer->Add_Object(pGameObject);
			pGameObject = CRingWind::Create(m_pGraphicDev,i * D3DX_PI/2.f);
			m_pEffectLayer->Add_Object(pGameObject);
		}

		Engine::CGameObject* pGameObject = CChargeLight::Create(m_pGraphicDev,CChargeLight::RIGHTHAND,&_vec3(0.f,0.f,0.f),5,0.3f,m_eAtkState);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

		pGameObject = CChargeLight::Create(m_pGraphicDev,CChargeLight::LEFTHAND,&_vec3(0.f,0.f,0.f),5,0.3f,m_eAtkState);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
	}
	else if(m_eMyState == ATK3STRONG && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.15 && m_iSkillCtrl == 1)
	{
		++m_iSkillCtrl;
		for(_uint i=0; i<4; ++i)
		{
			Engine::CGameObject* pGameObject = CRingWind::Create(m_pGraphicDev,i * D3DX_PI/2.f);
			m_pEffectLayer->Add_Object(pGameObject);
			pGameObject = CRingWind::Create(m_pGraphicDev,i * D3DX_PI/2.f);
			m_pEffectLayer->Add_Object(pGameObject);
			pGameObject = CRingWind::Create(m_pGraphicDev,i * D3DX_PI/2.f);
			m_pEffectLayer->Add_Object(pGameObject);
		}
	}
	else if(m_eMyState == ATK3STRONG && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.2 && m_iSkillCtrl == 2)
	{
		++m_iSkillCtrl;
		for(_uint i=0; i<4; ++i)
		{
			Engine::CGameObject* pGameObject = CRingWind::Create(m_pGraphicDev,i * D3DX_PI/2.f);
			m_pEffectLayer->Add_Object(pGameObject);
			pGameObject = CRingWind::Create(m_pGraphicDev,i * D3DX_PI/2.f);
			m_pEffectLayer->Add_Object(pGameObject);
			pGameObject = CRingWind::Create(m_pGraphicDev,i * D3DX_PI/2.f);
			m_pEffectLayer->Add_Object(pGameObject);
		}
	}
	else if(m_eMyState == ATK3STRONG_DURING && m_iSkillCtrl == 0)
	{
		++m_iSkillCtrl;
		Engine::CGameObject* pGameObject = CLightningCircle::Create(m_pGraphicDev);
		m_pEffectLayer->Add_Object(pGameObject);
		pGameObject = CLightningCircle::Create(m_pGraphicDev);
		m_pEffectLayer->Add_Object(pGameObject);
		pGameObject = CLightningCircle::Create(m_pGraphicDev);
		m_pEffectLayer->Add_Object(pGameObject);

		for(_uint i = 0; i<1; ++i)
		{
			pGameObject = CLightningAni::Create(m_pGraphicDev,CLightningAni::RIGHTHAND);
			m_pEffectLayer->Add_Object(pGameObject);
		}

		pGameObject = CChargeParticle_LightGlow::Create(m_pGraphicDev,CChargeParticle_LightGlow::RIGHTHAND,&_vec3(10.f,0.f,0.f),10,1.f);
		m_pEffectLayer->Add_Object(pGameObject);
	}
	else if(m_eMyState == ATK3STRONG_FINAL && m_iSkillCtrl ==0 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.35)
	{
		++m_iSkillCtrl;
		//Engine::CGameObject* pGameObject = CParty_SprayA::Create(m_pGraphicDev,20);
		//m_pEffectLayer->Add_Object(pGameObject);

		Engine::CGameObject* pGameObject = NULL;
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SWING04DUST,CExpEffect::SWING4DUST2, m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetLookDist(0.f);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SWING04DUST,CExpEffect::SWING4DUST3, m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetLookDist(0.f);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SWING04DUST,CExpEffect::SWING4DUST4, m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetLookDist(0.f);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SWING04DUST,CExpEffect::SWING4DUST5, m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetLookDist(0.f);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

		((CDefault_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(1.f);

		pGameObject = CSimpleAura::Create(m_pGraphicDev);
		m_pEffectLayer->Add_Object(pGameObject);

		for(_uint i=0; i<4; ++i)
		{
			pGameObject = CLightningAni::Create(m_pGraphicDev,CLightningAni::ATTACK,0.8f);
			m_pEffectLayer->Add_Object(pGameObject);
		}

		CDistortion* pDistortion = CDistortion::Create(m_pGraphicDev,_vec3(10.f,10.f,10.f),_vec3(D3DX_PI/2.f,0.f,0.f),*(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0],50.f,0.05f,1,false);
		Engine::CGameObject* pDistortionMgr = CDistortion_Mgr::Create(m_pGraphicDev,0.4f);
		((CDistortion_Mgr*)pDistortionMgr)->AddDistortion(pDistortion);
		Engine::GetLayer(L"Layer_Distortion")->Add_Object(pDistortionMgr);
	}
	else if(m_eMyState == ATK4STRONG && m_iSkillCtrl == 0)
	{
		++m_iSkillCtrl;
		Engine::CGameObject* pGameObject = CChargeLight::Create(m_pGraphicDev,CChargeLight::RIGHTHAND,&_vec3(0.f,0.f,0.f),2,0.3f,m_eAtkState);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

		pGameObject = CChargeLight::Create(m_pGraphicDev,CChargeLight::LEFTHAND,&_vec3(0.f,0.f,0.f),2,0.3f,m_eAtkState);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
	}
	else if(m_eMyState == ATK4STRONG && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.15 && m_iSkillCtrl == 1)
	{
		++m_iSkillCtrl;
	/*	Engine::CGameObject* pGameObject = CAtk4Throw::Create(m_pGraphicDev,50);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);*/
		_float fAngle = 0.f;
		_vec3 vLook = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0]);
		if(vLook.x > 0)
			fAngle = acosf(vLook.z);
		else
			fAngle = D3DX_PI * 2.f - acosf(vLook.z);

		Engine::CGameObject* pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WATERFALL,CExpEffect::WATERFALL3,m_pTransformCom->GetWorldMatrix());
		((CExpEffect*)pGameObject)->SetOffSetAngleY(fAngle);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);	

	}
	else if(m_eMyState == ATK4STRONG && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.25 && m_iSkillCtrl == 2)
	{
		++m_iSkillCtrl;
		((CDefault_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(0.1f);
	}
	else if(m_eMyState == ATK4STRONG_DURING && m_iSkillCtrl == 0)
	{
		++m_iSkillCtrl;
		Engine::CGameObject* pGameObject = CChargeParticle_LightGlow::Create(m_pGraphicDev,CChargeParticle_LightGlow::LEFTHAND,&_vec3(10.f,0.f,0.f),10,0.35f);
		m_pEffectLayer->Add_Object(pGameObject);
	}
	else if(m_eMyState == ATK4STRONG_FINAL && m_iSkillCtrl == 0)
	{
		++m_iSkillCtrl;
		Engine::CGameObject* pGameObject = CChargeLight::Create(m_pGraphicDev,CChargeLight::LEFTHAND,&_vec3(0.f,0.f,0.f),2,0.3f,m_eAtkState);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

		pGameObject = CChargeParticle_LightRay::Create(m_pGraphicDev,&_vec3(0.f,0.f,0.f),20,CChargeParticle_LightRay::FOOTL);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

		pGameObject = CChargeParticle_LightRay::Create(m_pGraphicDev,&_vec3(0.f,0.f,0.f),20,CChargeParticle_LightRay::FOOTR);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
	}
	else if(m_eMyState == ATK4STRONG_FINAL && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.3 && m_iSkillCtrl == 1)
	{
		++m_iSkillCtrl;
		((CDefault_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(0.5f);
		Engine::CGameObject* pGameObject = CSpiral::Create(m_pGraphicDev);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

		_vec3 vColPoint = *(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0] + *(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0] * 25.f;
		_float fDistance = D3DXVec3Length(&(*(_vec3*)&m_pBossWorld->m[3][0] - vColPoint));
		if(fDistance < 25.f)
		{
			CBossDevil* pBossDevil = ((CBossDevil*)Engine::GetList(L"Layer_Boss")->front());
			_float fAngleX = pBossDevil->GetTransform()->GetInfomation(Engine::CTransform::ANGLE)->x;
			_float fAngleY = m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->y + D3DX_PI;
			_float fAngleZ = pBossDevil->GetTransform()->GetInfomation(Engine::CTransform::ANGLE)->z;
			pBossDevil->GetTransform()->SetInfomation(Engine::CTransform::ANGLE,_vec3(fAngleX,fAngleY,fAngleZ));

			pBossDevil->SetStateWithAnimation(CBossDevil::DAMAGEDSTRONG);
			pBossDevil->DecreaseHp(50);
		}

		
	}
	
	else if((m_eMyState == ATK2STRONG) &&/* m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.1 && */m_iSkillCtrl == 0 && m_ePrevState != m_eMyState)
	{
		++m_iSkillCtrl;

		Engine::CGameObject* pGameObject = CAtk2Strong::Create(m_pGraphicDev);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

		pGameObject = CRingDisk::Create(m_pGraphicDev);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

		pGameObject = CAtk2Throw::Create(m_pGraphicDev,2,*(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0],*(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0],2.f);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
	}
	else if((m_eMyState == ATK2BACK || m_eMyState == ATK2FRONT || m_eMyState == ATK2LEFTEND || m_eMyState == ATK2RIGHTEND) && m_iSkillCtrl == 0 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation()>0.1)
	{
		++m_iSkillCtrl;
		Engine::CGameObject* pGameObject = NULL; 

		for(_uint i=0; i<1;++i)
		{
			pGameObject= CAtk2Strong::Create(m_pGraphicDev);
			Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

			pGameObject = CRingDisk::Create(m_pGraphicDev);
			Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		}
		pGameObject = CAtk2Throw::Create(m_pGraphicDev,2,*(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0],*(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0],2.f);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		
	}
	else if(m_eMyState == SNATCH && m_fTrailSpawnTimeAcc == 0.f)
	{
		m_fTrailSpawnTimeAcc += 1.f;
		Engine::CGameObject* pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT11, m_pTransformCom->GetWorldMatrix());
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);	
	}
	else if((m_eMyState == DASH_N || m_eMyState == DASH_S) && m_fTrailSpawnTimeAcc == 0.f)
	{
		m_fTrailSpawnTimeAcc = 1.f;
		Engine::CGameObject* pGameObject = NULL;
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EXPLOSION,CExpEffect::EXPLOSION0,m_pTransformCom->GetWorldMatrix());
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EXPLOSION,CExpEffect::EXPLOSION1,m_pTransformCom->GetWorldMatrix());
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EXPLOSION,CExpEffect::EXPLOSION2,m_pTransformCom->GetWorldMatrix());
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
	}
	else if(m_eMyState != ATK1 && m_eMyState != ATK2 && m_eMyState != ATK3 && m_eMyState != ATK4 && m_eMyState != ATK3STRONG_FINAL && m_eMyState != ATK3STRONG_DURING && m_eMyState != ATK3STRONG && m_eMyState != ATK4STRONG_DURING && m_eMyState != ATK4STRONG_FINAL && m_eMyState != DASH_W && m_eMyState != ATK2STRONG && m_eMyState != ATK2BACK  && m_eMyState != ATK2FRONT  && m_eMyState != ATK2LEFT&& m_eMyState != ATK2LEFTEND&& m_eMyState != ATK2RIGHT&& m_eMyState != ATK2RIGHTEND)
		m_fTrailSpawnTimeAcc = 0.f;
	
}


_matrix* CPlayer::GetWorldMat()
{
	return m_pTransformCom->GetWorldMatrix();
}

void CPlayer::MakeSound()
{
	if(m_eMyState == ATK3STRONG_FINAL && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.3 && m_iSoundCtrl == 0)
	{
		m_iSoundCtrl++;
		CSoundMgr::GetInstance()->PlaySound(L"effect_lightning_explosion.wav",CSoundMgr::HAGIE_ATK3);
		m_iDebug = 1;
	}
	else if(m_eMyState == ATK3STRONG && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.2 && m_iSoundCtrl == 0)
	{
		m_iSoundCtrl++;
		CSoundMgr::GetInstance()->PlaySound(L"hagie_swing_phantomdagger_strong_03.wav",CSoundMgr::HAGIE_ATK3);
	}

	else if(m_eMyState == ATK4 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.1 && m_iSoundCtrl == 0)
	{
		m_iSoundCtrl++;
		CSoundMgr::GetInstance()->PlaySound(L"hagie_attack_04.wav",CSoundMgr::HAGIE_ATK4);
		//CSoundMgr::GetInstance()->PlaySound(L"magic_firebolt_fire.wav",CSoundMgr::HAGIE_ATK4);
		//CSoundMgr::GetInstance()->PlaySound(L"hagie_attack_04.wav",CSoundMgr::HWEAPON4);
	}
	else if(m_eMyState == ATK4STRONG && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.25&& m_iSoundCtrl == 0)
	{
		m_iSoundCtrl++;
		CSoundMgr::GetInstance()->PlaySound(L"wisp_fire.wav",CSoundMgr::HAGIE_ATK4);
	}
	else if(m_eMyState == ATK4STRONG_FINAL && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.05 && m_iSoundCtrl == 0)
	{
		m_iSoundCtrl++;
		CSoundMgr::GetInstance()->PlaySound(L"hagie_attack_strong_08.wav",CSoundMgr::HAGIE_ATK4);
	}
	else if(m_eMyState == ATK4STRONG_FINAL && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.35 && m_iSoundCtrl == 1)
	{
		m_iSoundCtrl++;
		CSoundMgr::GetInstance()->PlaySound(L"magic_firebolt_fire.wav",CSoundMgr::HAGIE_ATK4);
	}
}

