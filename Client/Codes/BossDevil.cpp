#include "stdafx.h"
#include "BossDevil.h"
#include "Export_Engine.h"
#include "Player.h"
#include "Stage_Scene.h"
#include "Phantom_Trail.h"
#include "HitEffect.h"
#include "ExpEffect.h"
#include "Default_Camera.h"
#include "Distortion.h"
#include "BossTeleBegin.h"
#include "Distortion_Mgr.h"
#include "BossWeapon.h"
#include "RedEye.h"
#include "SoundMgr.h"

CBossDevil::CBossDevil(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
,m_fAngleAcc(0.f)
,m_CurIndex(10)
,m_eMyState(IDLE)
,m_pShaderCom(NULL)
,m_isOnce(false)
,m_LocalMatForMove(NULL)
,m_isInitSetBeforeStartAnim(false)
,m_iCheckForSwing(0)
,m_pTransformCom(NULL)
,m_pRendererCom(NULL)
,m_pDynamicMeshCom(NULL)
,m_pBoxColliderCom(NULL)
,m_isVisible(true)
,m_fTeleportAtkDelay(0.f)
,m_iSkillCntCtrl(0)
,m_fBreakTime(0.f)
,m_isMad(false)
,m_pShaderForRenderTarget(NULL)
,m_fTimeAcc(0.f)
,m_fHitColorTime(0.f)
,m_iHp(1000)
,m_fDamagedStrongAcc(0.f)
,m_isSendPlayer(false)
,m_isMadOnce(false)
{
	m_iMaxHp = m_iHp;
}

CBossDevil::~CBossDevil(void)
{

}

HRESULT CBossDevil::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pDynamicMeshCom = (Engine::CDynamicMesh*)Engine::Clone(COM_STAGE,L"Component_Mesh_KadanDevil");
	if(m_pDynamicMeshCom == NULL)
		return S_OK;
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Mesh_Kadan",m_pDynamicMeshCom);

	m_pRageModeMesh = (Engine::CDynamicMesh*)Engine::Clone(COM_STAGE,L"Component_Mesh_KadanRage");
	if(m_pRageModeMesh == NULL)
		return S_OK;


	m_pShaderForRenderTarget = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_RenderTarget");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_RenderTarget",m_pShaderForRenderTarget);


	m_pBoxColliderCom = (Engine::CBoxCollider*)Engine::Clone(COM_STAGE, L"Component_BoxCollider");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_BoxCollider",m_pBoxColliderCom);
	
	_matrix matTemp;
	m_pBoxColliderCom->Compute_MinMax(m_pDynamicMeshCom,*D3DXMatrixIdentity(&matTemp),0,_vec3(2.f,2.f,6.f));
	//m_pBoxColliderCom->Compute_MinMax(m_pDynamicMeshCom,*m_pDynamicMeshCom->GetInitMatrix(),0);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Mesh");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Mesh",m_pShaderCom);

	m_pShaderForShadow = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Shadow");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Shadow",m_pShaderForShadow);


	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,_vec3(0.1f,0.1f,0.1f));
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(0.f,m_fAngleAcc,0.f));
	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,_vec3(0.f,0.5f,0.f));
	m_pTransformCom->Update_Component(0.f);

	m_pDynamicMeshCom->Set_AnimationSet(IDLE);
	
	return S_OK;
}

_int CBossDevil::Update_GameObject(const _float& fTimeDelta)
{
	//if(/*m_iHp < m_iMaxHp * 0.7f && */m_isSendPlayer == false)
	//{
	//	m_isSendPlayer = true;
	//	if((m_pDynamicMeshCom->Get_Occupied() == true && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.8 )|| m_pDynamicMeshCom->Get_Occupied() == false)
	//	{
	//		m_iSkillCntCtrl = 0;
	//		m_eMyState = SENDPLAYER;
	//		m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
	//		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
	//	}
	//}

	m_ePreStateForSound = m_eMyState;

	if(m_eMyState == DAMAGEDSTRONG && m_fDamagedStrongAcc > 0.f)
		m_fDamagedStrongAcc -= fTimeDelta*10.f;
	else
		m_fDamagedStrongAcc = 20.f;


	//여기서추가
	if(m_eMyState == SENDPLAYER && m_pDynamicMeshCom->Get_Occupied() == false)
	{
		m_eMyState = IDLE;
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState);
	}

	m_fTimeAcc += fTimeDelta;
	if(m_pDynamicMeshCom == NULL)
		return 0;
	if(m_isOnce == false)
	{
		m_isOnce = true;
		SetUpOnce();
	}
	if(m_isMad == false && m_pDynamicMeshCom->Get_Occupied() == false && m_eMyState == MAD)
		m_isMad = true;

	if(*((CPlayer*)m_pPlayer)->GetState() == CPlayer::SNATCH)
	{
		m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_NONALPHA);
		return 0;
	}

	m_fDistToPlayer = D3DXVec3Length(&(*m_pPlayerPos - *m_pTransformCom->GetInfomation(Engine::CTransform::POSITION)));
	MovePattern(fTimeDelta);
		
	m_fTimeDelta = fTimeDelta;
	if(m_isMad && m_eMyState !=EXPLOSION && m_eMyState != SENDPLAYER)
		m_fTimeDelta *= 1.25f;

	Make_Effect();
	
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_LeftHandWorldMat = *m_pLeftHandMat * *m_pTransformCom->GetWorldMatrix();
	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_NONALPHA);
	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_SHADOW);


	// ---------------------- For Point Light -------------------
	D3DLIGHT9 tLightInfo = Engine::GetLightInfo(2);
	tLightInfo.Position = *(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0] + _vec3(0.f,5.f,0.f);
	Engine::Change_LightInfo(tLightInfo,2);
	if(m_isVisible == false)
		Engine::SetisRender(2,false);
	else
		Engine::SetisRender(2,true);
	///////////////////////////////////////////////

	return 0;
}

_int CBossDevil::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if(m_pDynamicMeshCom == NULL)
		return 0;

	//if(m_eMyState == MAD)
	//	m_pRendererCom->SetGaussianBlurOn(m_p)

	//0402 ForTesting temp disenable

	AttackPattern(fTimeDelta);
	//must be enable after collision effect works
	if(m_eMyState != m_ePreStateForSound)
		m_iSoundCtrl = 0;

	Make_Sound();


	//보스매드모드?

	if(m_iHp < m_iMaxHp * 0.3f && m_eMyState != MAD && m_isMadOnce == false)
	{
		m_isMadOnce = true;
		m_iSkillCntCtrl = 0;

		MAPCOMPONENT::iterator iter = m_MapComponent[CGameObject::NONUPDATE].find(L"Component_Mesh_Kadan");
		if(iter == m_MapComponent[CGameObject::NONUPDATE].end())
			return 0;
		Engine::Safe_Release(iter->second);
		Engine::Safe_Release(iter->second);
		m_MapComponent[CGameObject::NONUPDATE].erase(iter);

		m_pDynamicMeshCom = m_pRageModeMesh;
		m_pRageModeMesh = NULL;
		Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Mesh_Kadan",m_pDynamicMeshCom);
		SetUpOnce();
		m_eMyState = MAD;
		m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);

		((CBossWeapon*)Engine::GetList(L"Layer_Boss")->back())->SetWeaponMad();

		Engine::CGameObject* pGameObject = CREdEye::Create(m_pGraphicDev);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
	}


	if(Engine::Get_KeyMgr()->KeyPressed(KEY_1))
	{
		m_iSkillCntCtrl = 0;

		MAPCOMPONENT::iterator iter = m_MapComponent[CGameObject::NONUPDATE].find(L"Component_Mesh_Kadan");
		if(iter == m_MapComponent[CGameObject::NONUPDATE].end())
			return 0;
		Engine::Safe_Release(iter->second);
		Engine::Safe_Release(iter->second);
		m_MapComponent[CGameObject::NONUPDATE].erase(iter);

		m_pDynamicMeshCom = m_pRageModeMesh;
		m_pRageModeMesh = NULL;
		Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Mesh_Kadan",m_pDynamicMeshCom);
		SetUpOnce();
		m_eMyState = MAD;
		m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);

		((CBossWeapon*)Engine::GetList(L"Layer_Boss")->back())->SetWeaponMad();

		Engine::CGameObject* pGameObject = CREdEye::Create(m_pGraphicDev);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
	}
	if(Engine::Get_KeyMgr()->KeyPressed(KEY_2))
	{
		m_iSkillCntCtrl = 0;
		m_eMyState = SENDPLAYER;
		m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
	}
	if(Engine::Get_KeyMgr()->KeyPressed(KEY_3))
	{
		m_iSkillCntCtrl = 0;
		m_eMyState = DAMAGEDSTRONG;
		m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false,0.7f);
	}
	/*if(GetAsyncKeyState('4'))
		m_iHp -= 10;*/

	Engine::CBoxCollider* pTarget = NULL;
	if(*((CPlayer*)m_pPlayer)->GetState() == CPlayer::ATK1 || *((CPlayer*)m_pPlayer)->GetState() == CPlayer::ATK2 || *((CPlayer*)m_pPlayer)->GetState() == CPlayer::ATK3)
	{
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

		pTarget = (Engine::CBoxCollider*)Engine::GetList(L"Layer_Trail")->back()->FindComponent(Engine::CGameObject::NONUPDATE,L"Component_BoxCollider");
		if(pTarget == NULL)
			return 0;
	}
	

	if(((CPlayer*)m_pPlayer)->GetCollisionPossibility() == 1 && m_isCol == false && pTarget != NULL)
	{
		m_isCol = m_pBoxColliderCom->CollisionCheck_OBB(pTarget);

		if(m_isCol == true)
			((CPlayer*)m_pPlayer)->StopAtkCollision();
	}
	else if(((CPlayer*)m_pPlayer)->GetCollisionPossibility() != 1)
		m_isCol = false;

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
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		}
		else if(*((CPlayer*)Engine::GetList(L"Layer_Player")->front())->GetAtkState() == CPlayer::ICE)
		{
			pGameObject = CHitEffect::Create(m_pGraphicDev,1,*(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0] + _vec3(fRandX,fRandY,fRandZ),1);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		}
		else if(*((CPlayer*)Engine::GetList(L"Layer_Player")->front())->GetAtkState() == CPlayer::LIGHTNING)
		{
			pGameObject = CHitEffect::Create(m_pGraphicDev,1,*(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0] + _vec3(fRandX,fRandY,fRandZ),2,6.f);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		}
		//--m_iHp;

		if(m_eMyState != SENDPLAYER && m_eMyState != DAMAGEDFRONT && (*((CPlayer*)m_pPlayer)->GetState() == CPlayer::ATK3STRONG_FINAL || *((CPlayer*)m_pPlayer)->GetState() == CPlayer::ATK4STRONG_FINAL))
		{
			m_eMyState =DAMAGEDFRONT;
			m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
			m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false,0.6f);
		}
	}
	/*else
	{
		m_eMyState =IDLE;
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState);
	}*/

	return 0;
}


void CBossDevil::Render_GameObject(void)
{
	if(m_eMyState == MAD && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.6 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() < 0.8)
		m_pRendererCom->SetGaussianBlurOn(m_pShaderForRenderTarget->GetEffect(),-0.05f,10);
	else
		m_pRendererCom->SetisGaussianBlur(false);

	D3DXTRACK_DESC tInfo;
	ZeroMemory(&tInfo,sizeof(D3DXTRACK_DESC));
	LPD3DXANIMATIONCONTROLLER pAnimationControl = *m_pDynamicMeshCom->GetAnimationCtrl()->Get_AnimationCtrl();
	pAnimationControl->GetTrackDesc(m_pDynamicMeshCom->GetAnimationCtrl()->GetCurrentTrack(),&tInfo);
	
	/*TCHAR szTemp[256] = L"%d";
	wsprintf(szTemp,L"Proceed : %d",(_uint)(m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() * 100.f));
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(200.f,30.f),D3DXCOLOR(1.f,1.f,1.f,1.f));

	wsprintf(szTemp,L"BOSS State: %d",(_uint)m_eMyState);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(400.f,30.f),D3DXCOLOR(1.f,1.f,1.f,1.f));

	wsprintf(szTemp,L"Distance: %d",(_uint)m_fDistToPlayer);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(200.f,60.f),D3DXCOLOR(1.f,1.f,1.f,1.f));

	wsprintf(szTemp,L"Boss Skill Ctrl : %d",(_uint)m_iSkillCntCtrl);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(200.f,400.f),D3DXCOLOR(1.f,1.f,1.f,1.f));

	wsprintf(szTemp,L"isCol : %d",(_uint)m_isCol);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(200.f,430.f),D3DXCOLOR(1.f,1.f,1.f,1.f));

	wsprintf(szTemp,L"BOSS HP : %d",(_uint)m_iHp);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(200.f,460.f),D3DXCOLOR(1.f,1.f,1.f,1.f));

	wsprintf(szTemp,L"SoundCtrl : %d",(_uint)m_iSoundCtrl);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(200.f,500.f),D3DXCOLOR(1.f,1.f,1.f,1.f));*/

	m_pTransformCom->SetUp_OnGraphicDev();
	m_pDynamicMeshCom->Play_Animation(m_fTimeDelta*1.5f);

	if(m_isVisible == false)
		return;
	
	m_pDynamicMeshCom->SetUp_ConstantTable(m_pShaderCom->GetEffect());

	if(m_isCol == true)
		m_fHitColorTime = 1.f;	
	else
		m_fHitColorTime -= m_fTimeDelta;
	
	if(m_fHitColorTime > 0.f)
		m_pShaderCom->GetEffect()->SetVector("g_vHitColor",&_vec4(1.f,0.5f,0.5f,1.f));
	else
		m_pShaderCom->GetEffect()->SetVector("g_vHitColor",&_vec4(1.f,1.f,1.f,1.f));


	m_pDynamicMeshCom->Render_MeshWithShader(m_pShaderCom->GetEffect(),7);


	//if(((CStage_Scene*)Engine::GetCurrentScene())->GetBoolLine() == true)
		m_pBoxColliderCom->Render(Engine::CBoxCollider::BOX_AABB,*m_pTransformCom->GetWorldMatrix(),m_isCol,false);
	

	/*wsprintf(szTemp,L"BreakTime : %d",(_uint)(m_fBreakTime * 100.f));
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(400.f,90.f),D3DXCOLOR(1.f,1.f,1.f,1.f));*/

}

void CBossDevil::AttackPattern(const _float& fTimeDelta)
{
	if(m_pDynamicMeshCom->Get_Occupied() == true)
		return;
	/*else if(m_eMyState == SENDPLAYER)
		return;*/
	else if(m_eMyState != TELEPORTBEGIN)
	{
		m_eMyState = IDLE;
		m_pDynamicMeshCom->Set_AnimationSet(IDLE);
	}
	
		

	if(m_eMyState == DAMAGEDFRONT)
		return;

	if(m_eMyState == TELEPORTBEGIN)
	{
		m_fTeleportAtkDelay += fTimeDelta;
		if(m_fTeleportAtkDelay < 3.f)
			return;

		m_fTeleportAtkDelay = 0.f;

		int iRandAtkAfterTele = rand()%2;
		if(iRandAtkAfterTele == 0)
			m_eMyState = TELEPORTATK1;
		if(iRandAtkAfterTele == 1)
			m_eMyState = TELEPORTATK3;
		if(iRandAtkAfterTele == 2)
			m_eMyState = DASHUPPER_R;
		
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
		m_isVisible = true;
		m_iSkillCntCtrl = 0;

		_vec3 vDir = _vec3(0.f,0.f,1.f);
		_float fRandAngle = D3DXToRadian(rand()%361);
		_matrix matRot;
		D3DXMatrixRotationY(&matRot,fRandAngle);
		D3DXVec3TransformNormal(&vDir,&vDir,&matRot);
		_vec3 vBossNewPos = *m_pPlayerPos + vDir * 17.f;
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vBossNewPos);
		m_pTransformCom->Update_Component(0.f);

		return;
	}
	if(m_eMyState == DASHUPPER_R)
	{
		m_eMyState = DASHUPPER_L;
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
		m_isVisible = true;
		m_iSkillCntCtrl = 0;
		m_fBreakTime = 0.f;
		return;
	}



	// Temp 0410 지울것
	/*if(m_pDynamicMeshCom->Get_Occupied() == false && m_eMyState != SWING)
	{
		m_eMyState = IDLE;
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState);
	}*/
	//return;
	//

	m_fBreakTime += fTimeDelta;
	if(m_fBreakTime < 1.f)
		return;

	m_isVisible = true;
	m_iSkillCntCtrl = 0;
	m_fBreakTime = 0.f;
	if(m_fDistToPlayer < 100.f)
	{
		_uint iRandMad = rand()%2;
		if(m_isMad == true)
		{
			_uint iRandState = rand()%11;
			if(iRandState > 7 && m_eMyPreState != EXPLOSION)
				m_eMyState = EXPLOSION;
			else if(iRandState > 5  && m_eMyPreState != TELEPORTBEGIN)
				m_eMyState = TELEPORTBEGIN;
			else if(iRandState > 2 && m_eMyState != DASHUPPER_R)
				m_eMyState = DASHUPPER_R;
			else if(iRandState > 0 && m_eMyState != BOLT)
				m_eMyState = BOLT;
			else
				m_eMyState = SWING;
		}
		else if(m_fDistToPlayer > 40.f)
		{
			int iAtkRand = rand()%2;
			if(iAtkRand == 0)
				if(m_eMyPreState !=TELEPORTBEGIN)
					m_eMyState = TELEPORTBEGIN;
				else
					m_eMyState = BOLT;
			else
				if(m_eMyPreState !=BOLT)
					m_eMyState = BOLT;
				else
					m_eMyState = TELEPORTBEGIN;
		}
		else
		{
			if(m_fDistToPlayer > 30.f)
			{
				int iAtkRand = rand()%2;
				if(iAtkRand == 0)
					if(m_eMyPreState !=TELEPORTBEGIN)
						m_eMyState = TELEPORTBEGIN;
					else
						m_eMyState = DASHUPPER_R;
				else
					if(m_eMyPreState !=SWING)
						m_eMyState = SWING;
					else
						m_eMyState = BOLT;
			}
			else if(m_fDistToPlayer > 15.f)
			{
				int iAtkRand = rand()%3;
				if(iAtkRand == 0)
					if(m_eMyPreState !=SWING)
						m_eMyState = SWING;
					else
						m_eMyState = DASHUPPER_R;
				else
					if(m_eMyPreState !=DASHUPPER_R)
						m_eMyState = DASHUPPER_R;
					else
						m_eMyState = TELEPORTBEGIN;
			}
			else
				m_eMyState = DASHUPPER_L;

		}

		


		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
	}
	else
	{
		m_isVisible = true;
		m_iSkillCntCtrl = 0;
		m_eMyState = IDLE;
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState);
	}

	m_eMyPreState = m_eMyState;

	
}

void CBossDevil::MovePattern(const _float& fTimeDelta)
{
	if(m_pDynamicMeshCom->Get_Occupied() == true && m_eMyState != DAMAGEDFRONT && m_eMyState != DAMAGEDSTRONG && m_eMyState != SENDPLAYER)
	{
		_float fOffSet = 0.1f;
		if(m_isInitSetBeforeStartAnim == false)
		{
			if(m_eMyState == DAMAGEDFRONT)
				fOffSet *= 0.1f;

			Calculate_AngleToPlayer();

			memcpy(&m_vFirstPosBeforeAction,&m_pTransformCom->GetWorldMatrix()->m[3][0],sizeof(float)*3);
			D3DXTRACK_DESC tInfo,tInfo2;
			ZeroMemory(&tInfo,sizeof(D3DXTRACK_DESC));
			LPD3DXANIMATIONCONTROLLER pAnimationControl = *m_pDynamicMeshCom->GetAnimationCtrl()->Get_AnimationCtrl();
			pAnimationControl->GetTrackDesc(0,&tInfo);
			pAnimationControl->GetTrackDesc(1,&tInfo2);

			if(tInfo.Enable + tInfo2.Enable > 1)
				return;
			else
			{
				m_vOffSetForAtkAnim = _vec3(m_LocalMatForMove->_41,m_LocalMatForMove->_42,m_LocalMatForMove->_43) * fOffSet;
				m_isInitSetBeforeStartAnim = true;
			}			
		}
		_vec3 vMovingPos = *(_vec3*)&m_LocalMatForMove->m[3][0] *fOffSet - m_vOffSetForAtkAnim;
		D3DXVec3TransformCoord(&vMovingPos,&vMovingPos,&Engine::MyGetRotMatFromWorldMat(*m_pTransformCom->GetWorldMatrix()));
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vMovingPos + m_vFirstPosBeforeAction);
	}
	else if (m_eMyState == DAMAGEDSTRONG && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() < 0.5)
	{
		_vec3 vNewPos = *(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0] - *(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0] * m_fDamagedStrongAcc* 1.6f;
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vNewPos);
	}
	else
		m_isInitSetBeforeStartAnim = false;

}
void CBossDevil::Calculate_AngleToPlayer(void)
{
	//calculating angle to player
	_vec3 MyPos = *m_pTransformCom->GetInfomation(Engine::CTransform::POSITION);
	_vec3 vDirToPlayer = *m_pPlayerPos - MyPos;
	float fCos = D3DXVec3Dot(D3DXVec3Normalize(&vDirToPlayer,&vDirToPlayer), &_vec3(0.f,0.f,1.f));
	float fAngle = 0;
	if(MyPos.x < m_pPlayerPos->x)
		fAngle = acosf(fCos);
	else
		fAngle = D3DX_PI * 2 - acosf(fCos);

	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(0.f,fAngle,0.f));
	/////////////
}

CBossDevil* CBossDevil::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossDevil* pInst = new CBossDevil(pGraphicDev);

	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CBossDevil::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pDynamicMeshCom);
	Engine::Safe_Release(m_pBoxColliderCom);
	Engine::Safe_Release(m_pShaderCom);
	Engine::Safe_Release(m_pShaderForShadow);
	Engine::Safe_Release(m_pShaderForRenderTarget);

	if(m_isMad == false)
		Engine::Safe_Release(m_pRageModeMesh);

	Engine::CGameObject::Free();
}

void CBossDevil::SetState(STATE eState)
{
	m_eMyState = eState;
}

void CBossDevil::SetStateWithAnimation(STATE eState)
{
	m_eMyState = eState;
	m_pDynamicMeshCom->GetAnimationCtrl()->SetOccupied();
	m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false,0.5f);
	m_iSkillCntCtrl = 0;
}

void CBossDevil::GetState(_uint* pState)
{
	*pState = m_eMyState;
}

void CBossDevil::Render_ShadowDepth()
{
	if(m_isVisible == false)
		return;

	m_pTransformCom->SetUp_OnGraphicDev();
	m_pDynamicMeshCom->SetUp_ConstantTable(m_pShaderForShadow->GetEffect());

	m_pShaderForShadow->GetEffect()->SetMatrix("g_matLightView",m_pRendererCom->GetLightView());
	m_pShaderForShadow->GetEffect()->SetMatrix("g_matLightProj",m_pRendererCom->GetLightProj());

	m_pDynamicMeshCom->Render_MeshWithShader(m_pShaderForShadow->GetEffect());

	
}

void CBossDevil::Make_Effect(void)
{
	switch(m_eMyState)
	{
	case TELEPORTBEGIN:
		if(m_iSkillCntCtrl == 0 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 10.f/61.f)
		{
			++m_iSkillCntCtrl;
			Engine::CGameObject* pGameObject = NULL;
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::TELEPORT,CExpEffect::KADANTELE0, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::TELEPORT,CExpEffect::KADANTELE1, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::TELEPORT,CExpEffect::KADANTELE2, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::TELEPORT,CExpEffect::KADANTELE3, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::TELEPORT,CExpEffect::KADANTELE5, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::TELEPORT,CExpEffect::KADANTELE7, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			/*pGameObject = CDistortion::Create(m_pGraphicDev,_vec3(10.f,10.f,10.f),_vec3(0.f,0.f,0.f),_vec3(-5.f,5.f,0.f),10.f,0.07f,1);
			Engine::GetLayer(L"Layer_Distortion")->Add_Object(pGameObject);*/
			m_isVisible = false;

			((CDefault_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(0.3f);

			/*pGameObject = CBossTeleBegin::Create(m_pGraphicDev,100);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);*/

			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::TELEPORT,CExpEffect::KADANTELE8, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

			CDistortion* pDistortion = CDistortion::Create(m_pGraphicDev,_vec3(5.f,5.f,5.f),_vec3(0.f,0.f,0.f),*(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0],200.f,0.05f,1,true);
			Engine::CGameObject* pDistortionMgr = CDistortion_Mgr::Create(m_pGraphicDev,0.3f);
			((CDistortion_Mgr*)pDistortionMgr)->AddDistortion(pDistortion);
			Engine::GetLayer(L"Layer_Distortion")->Add_Object(pDistortionMgr);
		}
		break;
	case TELEPORTATK1:
		if(m_iSkillCntCtrl == 0)
		{
			Engine::CGameObject* pGameObject = NULL;
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::TELEPORT,CExpEffect::KADANTELE0, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev,CChildEffect::TELEPORT, CExpEffect::KADANTELE1, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::TELEPORT,CExpEffect::KADANTELE2, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::TELEPORT,CExpEffect::KADANTELE3, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::TELEPORT,CExpEffect::KADANTELE5, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::TELEPORT,CExpEffect::KADANTELE7, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			++m_iSkillCntCtrl;
			((CDefault_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(0.1f);
			CDistortion* pDistortion = CDistortion::Create(m_pGraphicDev,_vec3(5.f,5.f,5.f),_vec3(0.f,0.f,0.f),*(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0],200.f,0.05f,1,true);
			Engine::CGameObject* pDistortionMgr = CDistortion_Mgr::Create(m_pGraphicDev,0.3f);
			((CDistortion_Mgr*)pDistortionMgr)->AddDistortion(pDistortion);
			Engine::GetLayer(L"Layer_Distortion")->Add_Object(pDistortionMgr);
		}
		else if(m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 49.f / 104.f && m_iSkillCntCtrl == 1)
		{
			m_iSkillCntCtrl++;
			Engine::CGameObject* pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SWING04DUST,CExpEffect::SWING4DUST4, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SWING04DUST,CExpEffect::SWING4DUST5, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SWING04DUST,CExpEffect::SWING4DUST2, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SWING04DUST,CExpEffect::SWING4DUST3, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

			((CDefault_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(0.1f);
		}
		break;

	case SWING:
		//0.25   65
		if(m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.30 && m_iSkillCntCtrl == 0)
		{
			++m_iSkillCntCtrl;
			Engine::CGameObject* pGameObject = NULL;
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SPRINTRUN,CExpEffect::SPRINTRUN0, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SPRINTRUN,CExpEffect::SPRINTRUN1, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SPRINTRUN,CExpEffect::SPRINTRUN2, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);



			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SPRINTRUN,CExpEffect::SPRINTRUN6, m_pWeaponWorldMat);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			CSoundMgr::GetInstance()->PlaySound(L"kadan_shout_01.wav",CSoundMgr::BOSS3);
		}
		if(m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 66.f/249.f && m_iSkillCntCtrl == 1)
		{
			++m_iSkillCntCtrl;
			Engine::CGameObject* pGameObject = NULL;
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SPRINTRUN,CExpEffect::SPRINTRUN0, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SPRINTRUN,CExpEffect::SPRINTRUN1, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SPRINTRUN,CExpEffect::SPRINTRUN2, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			CSoundMgr::GetInstance()->PlaySound(L"kadan_shout_04.wav",CSoundMgr::BOSS3);
		}
		if(m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.5 && m_iSkillCntCtrl == 2)
		{
			m_iSkillCntCtrl++;
			Engine::CGameObject* pGameObject = NULL;
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SPRINTRUN,CExpEffect::SPRINTRUN0, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SPRINTRUN,CExpEffect::SPRINTRUN1, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SPRINTRUN,CExpEffect::SPRINTRUN2, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			CSoundMgr::GetInstance()->PlaySound(L"kadan_shout_04.wav",CSoundMgr::BOSS3);

		}
		if(m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.65f && m_iSkillCntCtrl == 3)
		{
			m_iSkillCntCtrl++;
			Engine::CGameObject* pGameObject = NULL;
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SPRINTRUN,CExpEffect::SPRINTRUN0, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SPRINTRUN,CExpEffect::SPRINTRUN1, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SPRINTRUN,CExpEffect::SPRINTRUN2, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			CSoundMgr::GetInstance()->PlaySound(L"kadan_shout_04.wav",CSoundMgr::BOSS3);


		}
		if(m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.72 && m_iSkillCntCtrl == 4)
		{
			++m_iSkillCntCtrl;
		
			Engine::CGameObject* pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SWING04DUST,CExpEffect::SWING4DUST4, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SWING04DUST,CExpEffect::SWING4DUST5, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SWING04DUST,CExpEffect::SWING4DUST2, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SWING04DUST,CExpEffect::SWING4DUST3, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			((CDefault_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(0.1f);

			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::ATKLEFT,CExpEffect::ATKRIGHT0, m_pWeaponWorldMat);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			CSoundMgr::GetInstance()->PlaySound(L"kadan_shout_04.wav",CSoundMgr::BOSS3);

		}

		break;

	case DASHUPPER_L:
		if(m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.26 && m_iSkillCntCtrl == 0)
		{
			++m_iSkillCntCtrl;
			Engine::CGameObject* pGameObject = NULL;

			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::ATKLEFT,CExpEffect::ATKLEFT7, m_pWeaponWorldMat);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::ATKLEFT,CExpEffect::ATKLEFT6, m_pWeaponWorldMat);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::ATKLEFT,CExpEffect::ATKLEFT0, m_pWeaponWorldMat);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::ATKLEFT,CExpEffect::ATKLEFT2, m_pWeaponWorldMat);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::ATKLEFT,CExpEffect::ATKLEFT4, m_pWeaponWorldMat);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

			((CDefault_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(0.1f);

			_vec3 vWeaponColRegion = *(_vec3*)&m_pWeaponWorldMat->m[3][0] - 100.f * *(_vec3*)&m_pWeaponWorldMat->m[2][0];
			_float fDist = D3DXVec3Length(&(*m_pPlayerPos - vWeaponColRegion));
			if(fDist < 17.f)
				m_pPlayer->SetisCol(true);

		}
		break;

	case DASHUPPER_R:
		if(m_iSkillCntCtrl == 0)
		{
			++m_iSkillCntCtrl;
			Engine::CGameObject* pGameObject = NULL;
			/*pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY3,m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY2,m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY1,m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY0,m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);*/
		}
		if(m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.36 && m_iSkillCntCtrl == 1)
		{
			++m_iSkillCntCtrl;
			Engine::CGameObject* pGameObject = NULL;

			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::ATKLEFT,CExpEffect::ATKLEFT7, m_pWeaponWorldMat);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::ATKLEFT,CExpEffect::ATKRIGHT0, m_pWeaponWorldMat);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::ATKLEFT,CExpEffect::ATKLEFT0, m_pWeaponWorldMat);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::ATKLEFT,CExpEffect::ATKLEFT2, m_pWeaponWorldMat);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::ATKLEFT,CExpEffect::ATKLEFT4, m_pWeaponWorldMat);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

			((CDefault_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(0.1f);

			_vec3 vWeaponColRegion = *(_vec3*)&m_pWeaponWorldMat->m[3][0] - 100.f * *(_vec3*)&m_pWeaponWorldMat->m[2][0];
			_float fDist = D3DXVec3Length(&(*m_pPlayerPos - vWeaponColRegion));
			if(fDist < 17.f)
				m_pPlayer->SetisCol(true);
		}
		if(m_isMad == false)
			return;
		if(m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.4 && m_iSkillCntCtrl == 2)
		{
			++m_iSkillCntCtrl;
			Engine::CGameObject* pGameObject = NULL;
			_vec3 vLook = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0]);
			_float fDist = 15.f;
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY3,m_pTransformCom->GetWorldMatrix());	
			((CExpEffect*)pGameObject)->SetOffSetWorldPos(vLook * fDist);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY2,m_pTransformCom->GetWorldMatrix());
			((CExpEffect*)pGameObject)->SetOffSetWorldPos(vLook * fDist);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY1,m_pTransformCom->GetWorldMatrix());
			((CExpEffect*)pGameObject)->SetOffSetWorldPos(vLook * fDist);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY0,m_pTransformCom->GetWorldMatrix());
			((CExpEffect*)pGameObject)->SetOffSetWorldPos(vLook * fDist);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		}
		if(m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.45 && m_iSkillCntCtrl == 3)
		{
			++m_iSkillCntCtrl;
			Engine::CGameObject* pGameObject = NULL;
			_vec3 vLook = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0]);
			_float fDist = 30.f;
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY3,m_pTransformCom->GetWorldMatrix());	
			((CExpEffect*)pGameObject)->SetOffSetWorldPos(vLook * fDist);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY2,m_pTransformCom->GetWorldMatrix());
			((CExpEffect*)pGameObject)->SetOffSetWorldPos(vLook * fDist);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY1,m_pTransformCom->GetWorldMatrix());
			((CExpEffect*)pGameObject)->SetOffSetWorldPos(vLook * fDist);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY0,m_pTransformCom->GetWorldMatrix());
			((CExpEffect*)pGameObject)->SetOffSetWorldPos(vLook * fDist);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		}
		if(m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.5 && m_iSkillCntCtrl == 4)
		{
			++m_iSkillCntCtrl;
			Engine::CGameObject* pGameObject = NULL;
			_vec3 vLook = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0]);
			_float fDist = 45.f;
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY3,m_pTransformCom->GetWorldMatrix());	
			((CExpEffect*)pGameObject)->SetOffSetWorldPos(vLook * fDist);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY2,m_pTransformCom->GetWorldMatrix());
			((CExpEffect*)pGameObject)->SetOffSetWorldPos(vLook * fDist);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY1,m_pTransformCom->GetWorldMatrix());
			((CExpEffect*)pGameObject)->SetOffSetWorldPos(vLook * fDist);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY0,m_pTransformCom->GetWorldMatrix());
			((CExpEffect*)pGameObject)->SetOffSetWorldPos(vLook * fDist);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		}
		if(m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.55 && m_iSkillCntCtrl == 5)
		{
			++m_iSkillCntCtrl;
			Engine::CGameObject* pGameObject = NULL;
			_vec3 vLook = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0]);
			_float fDist = 60.f;
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY3,m_pTransformCom->GetWorldMatrix());	
			((CExpEffect*)pGameObject)->SetOffSetWorldPos(vLook * fDist);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY2,m_pTransformCom->GetWorldMatrix());
			((CExpEffect*)pGameObject)->SetOffSetWorldPos(vLook * fDist);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY1,m_pTransformCom->GetWorldMatrix());
			((CExpEffect*)pGameObject)->SetOffSetWorldPos(vLook * fDist);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY0,m_pTransformCom->GetWorldMatrix());
			((CExpEffect*)pGameObject)->SetOffSetWorldPos(vLook * fDist);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		}
		break;

	case BOLT:
		if(m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.15f && m_iSkillCntCtrl == 0)
		{
			++m_iSkillCntCtrl;

			Engine::CGameObject* pGameObject = NULL;

			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::BOLT,CExpEffect::BOLT0, &m_LeftHandWorldMat);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::BOLT,CExpEffect::BOLT1, &m_LeftHandWorldMat);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

			
		}
		if(m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.67f && m_iSkillCntCtrl == 1)
		{
			++m_iSkillCntCtrl;
			Engine::CGameObject* pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::BOLT,CExpEffect::BOLT2, &m_LeftHandWorldMat);
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);



			CDistortion* pDistortion = CDistortion::Create(m_pGraphicDev,_vec3(1,1,1),_vec3(0.f,0.f,0.f),*(_vec3*)&m_LeftHandWorldMat.m[3][0],60.f,0.1f,1,true,0.2f);
			Engine::CGameObject* pDistortionMgr = CDistortion_Mgr::Create(m_pGraphicDev,0.2f,0.1f);
			((CDistortion_Mgr*)pDistortionMgr)->AddDistortion(pDistortion);
			Engine::GetLayer(L"Layer_Distortion")->Add_Object(pDistortionMgr);
			/*for(_uint i=0; i<5; ++i)
			{
				_float fRandX = _float(rand()%3 - 1);
				_float fRandY = _float(rand()%3 - 1);
				_float fRandZ = _float(rand()%3 - 1);
				_float fRandSpeed = rand()%10 + 35.f;
				_int iRandImg = rand()%2;
				pGameObject = CDistortion::Create(m_pGraphicDev,_vec3(fRandX,fRandY,fRandZ),_vec3(0.f,0.f,0.f),*(_vec3*)&m_LeftHandWorldMat.m[3][0] + _vec3(fRandX*3.f,fRandY*3.f,fRandZ*3.f),fRandSpeed,0.1f,iRandImg,true,0.2f);
				Engine::GetLayer(L"Layer_Distortion")->Add_Object(pGameObject);
			}*/

			//Engine::CGameObject* pGameObject = NULL;
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::FIREBALL,CExpEffect::FIREBALL0,((CPlayer*)m_pPlayer)->GetWorldMat());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::FIREBALL,CExpEffect::FIREBALL1,((CPlayer*)m_pPlayer)->GetWorldMat());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			
		}

		break;

	case SWORDWAVE:
		if(m_iSkillCntCtrl == 0)
		{
			/*++m_iSkillCntCtrl;
			Engine::CGameObject* pGameObject = NULL;
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY3,m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY2,m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY1,m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY0,m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);*/
		}
		break;
	case MAD:
		if(m_iSkillCntCtrl == 0 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.6)
		{
			++m_iSkillCntCtrl;
			Engine::CGameObject* pGameObject = NULL;
			((CDefault_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(1.f);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT10,m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::TELEPORT,CExpEffect::KADANTELE0, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev,CChildEffect::TELEPORT, CExpEffect::KADANTELE1, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::TELEPORT,CExpEffect::KADANTELE2, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::TELEPORT,CExpEffect::KADANTELE3, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::TELEPORT,CExpEffect::KADANTELE5, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::TELEPORT,CExpEffect::KADANTELE7, m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

			CDistortion* pDistortion = CDistortion::Create(m_pGraphicDev,_vec3(10.f,10.f,10.f),_vec3(D3DX_PI/2.f,0.f,0.f),*(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0],200.f,0.2f,1,false);
			Engine::CGameObject* pDistortionMgr = CDistortion_Mgr::Create(m_pGraphicDev,1.f);
			((CDistortion_Mgr*)pDistortionMgr)->AddDistortion(pDistortion);
			Engine::GetLayer(L"Layer_Distortion")->Add_Object(pDistortionMgr);
		}
		break;

	case EXPLOSION:
		if(m_iSkillCntCtrl == 0 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.4)
		{
			++m_iSkillCntCtrl;
			Engine::CGameObject* pGameObject = NULL;
			for(_uint i=0; i<2 ; ++i)
			{
				_float fRandAngle = D3DXToRadian(rand()% 361);
				_float fRandDist = rand()%10 + 10.f;
				pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY3,m_pTransformCom->GetWorldMatrix());
				_vec3 vFirstLook = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0]);
				_matrix matRotY;
				D3DXMatrixRotationY(&matRotY , fRandAngle);
				D3DXVec3TransformNormal(&vFirstLook,&vFirstLook,&matRotY);
				((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
				Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
				pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY2,m_pTransformCom->GetWorldMatrix());
				((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
				Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
				pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY1,m_pTransformCom->GetWorldMatrix());
				((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
				Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
				pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY0,m_pTransformCom->GetWorldMatrix());
				((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
				Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

				pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::KADANEXPLOSION,CExpEffect::KADANEXPLOSION1, m_pTransformCom->GetWorldMatrix());
				Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
				pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::KADANEXPLOSION,CExpEffect::KADANEXPLOSION2, m_pTransformCom->GetWorldMatrix());
				Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
				pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::KADANEXPLOSION,CExpEffect::KADANEXPLOSION3, m_pTransformCom->GetWorldMatrix());
				Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
				pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::KADANEXPLOSION,CExpEffect::KADANEXPLOSION4, m_pTransformCom->GetWorldMatrix());
				Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
				pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::KADANEXPLOSION,CExpEffect::KADANEXPLOSION5, m_pTransformCom->GetWorldMatrix());
				Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

				CDistortion* pDistortion = CDistortion::Create(m_pGraphicDev,_vec3(10.f,10.f,10.f),_vec3(D3DX_PI/2.f,0.f,0.f),*(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0],100.f,0.2f,1,false);
				Engine::CGameObject* pDistortionMgr = CDistortion_Mgr::Create(m_pGraphicDev,0.6f);
				((CDistortion_Mgr*)pDistortionMgr)->AddDistortion(pDistortion);
				Engine::GetLayer(L"Layer_Distortion")->Add_Object(pDistortionMgr);

				
			}
		}
		if(m_iSkillCntCtrl == 1 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.5)
		{
			++m_iSkillCntCtrl;
			Engine::CGameObject* pGameObject = NULL;
			for(_uint i=0; i<2 ; ++i)
			{
				_float fRandAngle = D3DXToRadian(rand()% 361);
				_float fRandDist = rand()%10 + 20.f;
				pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY3,m_pTransformCom->GetWorldMatrix());
				_vec3 vFirstLook = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0]);
				_matrix matRotY;
				D3DXMatrixRotationY(&matRotY , fRandAngle);
				D3DXVec3TransformNormal(&vFirstLook,&vFirstLook,&matRotY);
				((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
				Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
				pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY2,m_pTransformCom->GetWorldMatrix());
				((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
				Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
				pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY1,m_pTransformCom->GetWorldMatrix());
				((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
				Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
				pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY0,m_pTransformCom->GetWorldMatrix());
				((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
				Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			}
		}
		if(m_iSkillCntCtrl == 2 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.6)
		{
			++m_iSkillCntCtrl;
			Engine::CGameObject* pGameObject = NULL;
			for(_uint i=0; i<2 ; ++i)
			{
				_float fRandAngle = D3DXToRadian(rand()% 361);
				_float fRandDist = rand()%10 + 30.f;
				pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY3,m_pTransformCom->GetWorldMatrix());
				_vec3 vFirstLook = *D3DXVec3Normalize(&_vec3(),(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[2][0]);
				_matrix matRotY;
				D3DXMatrixRotationY(&matRotY , fRandAngle);
				D3DXVec3TransformNormal(&vFirstLook,&vFirstLook,&matRotY);
				((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
				Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
				pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY2,m_pTransformCom->GetWorldMatrix());
				((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
				Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
				pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY1,m_pTransformCom->GetWorldMatrix());
				((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
				Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
				pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEREADY,CExpEffect::WAVEREADY0,m_pTransformCom->GetWorldMatrix());
				((CExpEffect*)pGameObject)->SetOffSetWorldPos(vFirstLook * fRandDist);
				Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);	
			}
		}
		break;

	case DAMAGEDSTRONG:
		if(m_iSkillCntCtrl == 0 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.3)
		{
			Engine::CGameObject* pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SWING04DUST,CExpEffect::SWING4DUST4, m_pTransformCom->GetWorldMatrix());
			((CExpEffect*)pGameObject)->SetFirstPositionOffSet(_vec3(3.f,0.f,3.f));
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SWING04DUST,CExpEffect::SWING4DUST5, m_pTransformCom->GetWorldMatrix());
			((CExpEffect*)pGameObject)->SetFirstPositionOffSet(_vec3(3.f,0.f,3.f));
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SWING04DUST,CExpEffect::SWING4DUST2, m_pTransformCom->GetWorldMatrix());
			((CExpEffect*)pGameObject)->SetFirstPositionOffSet(_vec3(3.f,0.f,3.f));
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SWING04DUST,CExpEffect::SWING4DUST3, m_pTransformCom->GetWorldMatrix());
			((CExpEffect*)pGameObject)->SetFirstPositionOffSet(_vec3(3.f,0.f,3.f));
			Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		}
		break;
	}
}

void CBossDevil::SetUpOnce(void)
{
	Engine::D3DXFRAME_DERIVED* pMoveFrame = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(m_pDynamicMeshCom->GetRootFrame(),"ValveBiped_Anim_Attachment_GRAPPLING");
	m_LocalMatForMove = &pMoveFrame->CombinedMatrix;	

	m_pPlayer = Engine::GetList(L"Layer_Player")->front();

	Engine::CTransform* pPlayerTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
	m_pPlayerPos = (_vec3*)&pPlayerTransform->GetWorldMatrix()->m[3][0];

	Engine::CTransform* pWeaponTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Boss")->back()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
	m_pWeaponWorldMat = pWeaponTransform->GetWorldMatrix();

	Engine::D3DXFRAME_DERIVED* pLEFTHAND = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(m_pDynamicMeshCom->GetRootFrame(),"ValveBiped_Anim_Attachment_LH");
	m_pLeftHandMat = &pLEFTHAND->CombinedMatrix;


	Engine::D3DXFRAME_DERIVED* pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(m_pDynamicMeshCom->GetRootFrame(),"ValveBiped_Anim_Attachment_CAM_Origin");
	m_pDashMoveMat = &pFrameFound->CombinedMatrix;
}

_matrix CBossDevil::GetWorldMatrix(void)
{
	return *m_pTransformCom->GetWorldMatrix();
}

void CBossDevil::Make_Sound()
{
	if(m_eMyState == TELEPORTBEGIN && m_iSoundCtrl == 0)
	{
		++m_iSoundCtrl;
		CSoundMgr::GetInstance()->PlaySound(L"darkness.wav",CSoundMgr::BOSS1);
	}
	if(m_eMyState == DASHUPPER_L && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.0 && m_iSoundCtrl == 0)
	{
		++m_iSoundCtrl;
		CSoundMgr::GetInstance()->PlaySound(L"Yulkes_sword_scratch_02.wav",CSoundMgr::BOSS2);
	}
	//if(m_eMyState == DASHUPPER_L && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.2 && m_iSoundCtrl == 1)
	//{
	//	++m_iSoundCtrl;
	//	CSoundMgr::GetInstance()->PlaySound(L"kadan_shout_02.wav",CSoundMgr::BOSS3);
	//}
	if(m_eMyState == DASHUPPER_R && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.0 && m_iSoundCtrl == 0)
	{
		++m_iSoundCtrl;
		CSoundMgr::GetInstance()->PlaySound(L"Yulkes_sword_scratch_01.wav",CSoundMgr::BOSS4);
	}
	if(m_eMyState == SWING && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.1 && m_iSoundCtrl == 0)
	{
		++m_iSoundCtrl;
		CSoundMgr::GetInstance()->PlaySound(L"Yulkes_sword_swing_02.wav",CSoundMgr::BOSS3);
	}
	if(m_eMyState == TELEPORTATK1 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.1 && m_iSoundCtrl == 0)
	{
		++m_iSoundCtrl;
		CSoundMgr::GetInstance()->PlaySound(L"Yulkes_sword_swing_02.wav",CSoundMgr::BOSS4);
	}
	if(m_eMyState == TELEPORTATK3 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.1 && m_iSoundCtrl == 0)
	{
		++m_iSoundCtrl;
		CSoundMgr::GetInstance()->PlaySound(L"Yulkes_sword_swing_02.wav",CSoundMgr::BOSS1);
	}
	if(m_eMyState == BOLT && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.1 && m_iSoundCtrl == 0)
	{
		++m_iSoundCtrl;
		CSoundMgr::GetInstance()->PlaySound(L"Yulkes_sword_swing_02.wav",CSoundMgr::BOSS1);
	}
	if(m_eMyState == EXPLOSION && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.1 && m_iSoundCtrl == 0)
	{
		++m_iSoundCtrl;
		CSoundMgr::GetInstance()->PlaySound(L"Yulkes_sword_swing_02.wav",CSoundMgr::BOSS1);
	}
	if(m_eMyState == DAMAGEDSTRONG && m_iSoundCtrl == 0)
	{
		++m_iSoundCtrl;
		CSoundMgr::GetInstance()->PlaySound(L"kadan_hurt_01.wav",CSoundMgr::BOSS4);
	}
	if(m_eMyState == MAD && m_iSoundCtrl == 0)
	{
		++m_iSoundCtrl;
		CSoundMgr::GetInstance()->PlaySound(L"kadan_shout_01.wav",CSoundMgr::BOSS3);
	}
}

