#include "stdafx.h"
#include "Boss.h"
#include "Export_Engine.h"
#include "Player.h"
#include "Stage_Scene.h"
#include "ExpEffect.h"
#include "BloodAni.h"
#include "Default_Camera.h"
#include "Action_Camera.h"
#include "SoundMgr.h"

CBoss::CBoss(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
,m_eMyState(IDLE)
,m_pShaderCom(NULL)
,m_pTransformCom(NULL)
,m_pRendererCom(NULL)
,m_pDynamicMeshCom(NULL)
,m_fTimeAcc(0.f)
,m_isOnce(false)
,m_fBlurTimeAcc(0.f)
,m_bBlurOn(false)
,m_fRedScreenTime(0.f)
,m_isRedScreen(false)
,m_iBlurCtrl(0)
,m_fFadeOutTime(0.f)
,m_isFadeOutIn(false)
,m_fWhiteScreenTime(0.f)
,m_isWhiteScreen(false)
{

}

CBoss::~CBoss(void)
{

}

HRESULT CBoss::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pDynamicMeshCom = (Engine::CDynamicMesh*)Engine::Clone(COM_STAGE,L"Component_Mesh_Kadan");
	if(m_pDynamicMeshCom == NULL)
		return S_OK;
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Mesh_Kadan",m_pDynamicMeshCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Mesh");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Mesh",m_pShaderCom);

	m_pShaderForRenderTarget = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_RenderTarget");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_RenderTarget",m_pShaderForRenderTarget);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_KadanFaceNormal");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_KadanFaceNormal",m_pTextureCom);

	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,_vec3(0.1f,0.1f,0.1f));
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(0.f,0.f,0.f));
	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,_vec3(0.f,0.5f,0.f));
	m_pTransformCom->Update_Component(0.f);

	m_pDynamicMeshCom->Set_AnimationSet(IDLE);
	

	return S_OK;
}

_int CBoss::Update_GameObject(const _float& fTimeDelta)
{
	if(m_pDynamicMeshCom == NULL)
		return 0;

	if(m_isOnce == false)
	{
		m_isOnce = true;
		Engine::CGameObject* pGameObject = NULL;
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10,CExpEffect::EP10_1, m_pTransformCom->GetWorldMatrix());
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

	/*	pGameObject = CBloodAni::Create(m_pGraphicDev,CBloodAni::RIGHTHAND,10.f);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);*/

		Engine::D3DXFRAME_DERIVED* pFrameFound;
		pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(m_pDynamicMeshCom->GetRootFrame(),"ValveBiped_Bip01_R_Hand");
		m_RightHandMat = &pFrameFound->CombinedMatrix;

		pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(m_pDynamicMeshCom->GetRootFrame(),"ValveBiped_Bip01_L_Hand");
		m_LeftHandMat = &pFrameFound->CombinedMatrix;

		pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(m_pDynamicMeshCom->GetRootFrame(),"ValveBiped_Bip01_Head");
		m_HeadMat = &pFrameFound->CombinedMatrix;
	}

	m_fTimeDelta = fTimeDelta;
	m_fTimeAcc += fTimeDelta;


	m_PrevState = m_eMyState;
	FirstEventCam();

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_TotalMatForEffect = *m_RightHandMat * *m_pTransformCom->GetWorldMatrix();
	m_TotalMatForEffectLeft = *m_LeftHandMat * *m_pTransformCom->GetWorldMatrix();
	m_TotalMatForHead = *m_HeadMat * *m_pTransformCom->GetWorldMatrix();

	if(((CStage_Scene*)Engine::GetCurrentScene())->GetDragonSpawn() == false)
		m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_NONALPHA);


	if(GetAsyncKeyState('B'))
	{
		Engine::CGameObject* pGameObject = NULL;
		
		/*pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART4, &m_TotalMatForHead);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);*/

		/*_float fValue = 
		m_pRendererCom->SetRedScreen(cosf(m_f))*/
		//m_isFadeOutIn = true;
		m_isWhiteScreen = true;
	}

	if(m_isRedScreen)
	{
		m_fRedScreenTime += fTimeDelta;

		m_pRendererCom->SetRedScreen(0.5f*cosf(m_fRedScreenTime*16)+0.5f);

		if(m_fRedScreenTime > D3DX_PI/8.f)
			m_isRedScreen = false;
	}
	if(m_isRedScreen == false && m_fRedScreenTime != 0.f)
	{
		m_pRendererCom->SetisRedScreen(false);
		m_fRedScreenTime = 0.f;
	}

	if(m_isWhiteScreen)
	{
		m_fWhiteScreenTime += fTimeDelta;

		m_pRendererCom->SetWhiteScreen(0.5f*sinf(m_fWhiteScreenTime)+0.5f);

		if(m_fWhiteScreenTime > D3DX_PI*2.f)
			m_isWhiteScreen = false;
	}
	if(m_isWhiteScreen == false && m_fWhiteScreenTime != 0.f)
	{
		m_pRendererCom->SetisWhiteScreen(false);
		m_fWhiteScreenTime = 0.f;
	}

	if(m_isFadeOutIn)
	{
		m_fFadeOutTime += fTimeDelta;

		m_pRendererCom->SetFadeIn(0.5f*cosf(m_fFadeOutTime*2) + 0.5f);

		if(m_fFadeOutTime > D3DX_PI)
			m_isFadeOutIn = false;
	}
	if(m_isFadeOutIn == false && m_fFadeOutTime != 0.f)
	{
		m_pRendererCom->SetisFadeIn(false);
		m_fFadeOutTime = 0.f;
	}

	return 0;
}

_int CBoss::LastUpdate_GameObject(const _float& fTimeDelta)
{
	if(m_eMyState != m_PrevState)
		m_iStateCtrl = 0;

	if(m_bBlurOn)
	{
		_int iMaxCnt = 0;
		_float fBlurTime = 0.f;
		m_fBlurTimeAcc += fTimeDelta;

		if(m_iBlurCtrl == 0)
		{
			iMaxCnt = 13 - max((_int(10*sinf(m_fBlurTimeAcc * 4.f))),0);
			fBlurTime = 1.7f;

			Engine::GetRenderer()->SetGaussianBlurOn(m_pShaderForRenderTarget->GetEffect(),-0.1f,iMaxCnt);
			if(m_fBlurTimeAcc > fBlurTime)
			{
				m_bBlurOn = false;
				m_fBlurTimeAcc = 0.f;
				Engine::GetRenderer()->SetisGaussianBlur(false);
			}
		}
		else if(m_iBlurCtrl == 1)
		{
			iMaxCnt = 13;
			fBlurTime = 2.5f;

			Engine::GetRenderer()->SetGaussianBlurOn(m_pShaderForRenderTarget->GetEffect(),-0.1f,iMaxCnt);
			if(m_fBlurTimeAcc > fBlurTime)
			{
				m_bBlurOn = false;
				m_fBlurTimeAcc = 0.f;
				Engine::GetRenderer()->SetisGaussianBlur(false);
			}
		}
		if(m_iBlurCtrl == 2)
		{
			iMaxCnt = 13 - max((_int(12*sinf(m_fBlurTimeAcc*0.5f))),0);//1;//_int(6.5f*cosf(m_fFadeOutTime*8) + 6.5f);
			fBlurTime = D3DX_PI*2.f;

			Engine::GetRenderer()->SetGaussianBlurOn(m_pShaderForRenderTarget->GetEffect(),-0.1f,iMaxCnt);
			if(m_fBlurTimeAcc > fBlurTime)
			{
				m_bBlurOn = false;
				m_fBlurTimeAcc = 0.f;
				Engine::GetRenderer()->SetisGaussianBlur(false);
			}
		}
	}

	if(Engine::Get_KeyMgr()->KeyPressed(KEY_A))
		((CAction_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(1.f);


	if(m_eMyState == EVENT2 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.15 && m_iStateCtrl == 0)
	{
		++m_iStateCtrl;
		m_bBlurOn = true;
		m_iBlurCtrl = 0;
		//((CAction_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(1.f);

		Engine::CGameObject* pGameObject = NULL;
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART0, &m_TotalMatForEffect);
		m_EvilEyeList.push_back(pGameObject);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART1, &m_TotalMatForEffect);
		m_EvilEyeList.push_back(pGameObject);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART2, &m_TotalMatForEffect);
		m_EvilEyeList.push_back(pGameObject);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART3, &m_TotalMatForEffect);
		m_EvilEyeList.push_back(pGameObject);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

		((CAction_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(0.5f);
		m_isRedScreen = true;
	}
	else if(m_eMyState == EVENT4 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.2 && m_iStateCtrl == 0)
	{

		++m_iStateCtrl;
		Engine::CGameObject* pGameObject = NULL;
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART4, &m_TotalMatForHead);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART5, &m_TotalMatForHead);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
	}
	else if(m_eMyState == EVENT4 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.4 && m_iStateCtrl == 1)
	{
		++m_iStateCtrl;
		Engine::CGameObject* pGameObject = NULL;
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART4, &m_TotalMatForHead);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART5, &m_TotalMatForHead);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
	}
	else if(m_eMyState == EVENT4 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.6 && m_iStateCtrl == 2)
	{
		++m_iStateCtrl;
	
		list<CGameObject*>::iterator iter = m_EvilEyeList.begin();
		list<CGameObject*>::iterator iter_end = m_EvilEyeList.end();
		for ( ; iter != iter_end ; ++iter)
			(*iter)->SetDestroy(true);

		m_EvilEyeList.clear();

		Engine::CGameObject* pGameObject = NULL;
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART4, &m_TotalMatForHead);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART5, &m_TotalMatForHead);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

		m_isRedScreen = true;
		((CAction_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(0.3f);


		//CSoundMgr::GetInstance()->PlaySound(L"kadan_hurtsound_event.wav",CSoundMgr::HAGIE_ATK2);
	}
	else if(m_eMyState == EVENT5 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.15 && m_iStateCtrl == 0)
	{
		++m_iStateCtrl;
		((CAction_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(0.5f);
		m_isRedScreen = true;
		Engine::CGameObject* pGameObject = NULL;
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART4, &m_TotalMatForHead);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART5, &m_TotalMatForHead);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
	}

	else if(m_eMyState == EVENT5 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.35 && m_iStateCtrl == 1)
	{
		++m_iStateCtrl;
		((CAction_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(0.5f);
		Engine::CGameObject* pGameObject = NULL;
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART4, &m_TotalMatForHead);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART5, &m_TotalMatForHead);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		m_isRedScreen = true;
	}
	else if(m_eMyState == EVENT5 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.45 && m_iStateCtrl == 2)
	{
		++m_iStateCtrl;
		m_isRedScreen = true;
		((CAction_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(0.5f);
	}
	else if(m_eMyState == EVENT5 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.65 && m_iStateCtrl == 3)
	{
		++m_iStateCtrl;
		m_isFadeOutIn = true;
	}
	else if(m_eMyState == EVENT6 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0 && m_iStateCtrl == 0)
	{
		++m_iStateCtrl;
		m_bBlurOn = true;
		m_iBlurCtrl = 1;
		((CAction_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(0.3f);
	}
	else if(m_eMyState == EVENT6 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.2 && m_iStateCtrl == 1)
	{
		++m_iStateCtrl;
		Engine::CGameObject* pGameObject = NULL;
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART0, &m_TotalMatForEffect);
		m_EvilEyeList.push_back(pGameObject);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART1, &m_TotalMatForEffect);
		m_EvilEyeList.push_back(pGameObject);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART3, &m_TotalMatForEffect);
		m_EvilEyeList.push_back(pGameObject);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART0, &m_TotalMatForEffectLeft);
		m_EvilEyeList.push_back(pGameObject);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART1, &m_TotalMatForEffectLeft);
		m_EvilEyeList.push_back(pGameObject);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::EP10_HEART,CExpEffect::EP10_HEART3, &m_TotalMatForEffectLeft);
		m_EvilEyeList.push_back(pGameObject);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);

		m_isFadeOutIn = true;
	}
	else if(m_eMyState == EVENT6 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.8 && m_iStateCtrl == 2)
	{
		++m_iStateCtrl;
		m_isFadeOutIn = true;
	}


	else if(m_eMyState == EVENT7 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.7 && m_iStateCtrl == 0)
	{
		++m_iStateCtrl;
		m_isFadeOutIn = true;
	}

	else if(m_eMyState == EVENT8 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.1 && m_iStateCtrl == 0)
	{
		++m_iStateCtrl;
		m_isWhiteScreen = true;
	}
	else if(m_eMyState == EVENT8 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.3 && m_iStateCtrl == 1)
	{
		++m_iStateCtrl;
		m_isRedScreen = true;
		((CAction_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(0.3f);
	}
	else if(m_eMyState == EVENT8 && m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() > 0.3 && m_iStateCtrl == 1)
	{
		++m_iStateCtrl;
		m_isRedScreen = true;
		((CAction_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(0.3f);
	}
	
	/*else if(m_eMyState != EVENT2)
		m_bBlurOn = false;*/

	return 0;
}


void CBoss::Render_GameObject(void)
{
	D3DXTRACK_DESC tInfo;
	ZeroMemory(&tInfo,sizeof(D3DXTRACK_DESC));
	LPD3DXANIMATIONCONTROLLER pAnimationControl = *m_pDynamicMeshCom->GetAnimationCtrl()->Get_AnimationCtrl();
	pAnimationControl->GetTrackDesc(m_pDynamicMeshCom->GetAnimationCtrl()->GetCurrentTrack(),&tInfo);
	
	/*TCHAR szTemp[256] = L"%d";
	wsprintf(szTemp,L"BOSS : %d",_uint(m_pDynamicMeshCom->Get_CurrentRatioFromAnimation() * 100));
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(200.f,430.f),D3DXCOLOR(1.f,1.f,1.f,1.f));

	wsprintf(szTemp,L"BOSS State: %d",(_uint)m_eMyState);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(400.f,430.f),D3DXCOLOR(1.f,1.f,1.f,1.f));*/

	m_pTransformCom->SetUp_OnGraphicDev();
	m_pDynamicMeshCom->Play_Animation(m_fTimeDelta);
	
	m_pDynamicMeshCom->SetUp_ConstantTable(m_pShaderCom->GetEffect());
	//m_pShaderCom->GetEffect()->SetTexture("g_normalTexture",m_pTextureCom->GetTexture());
	m_pDynamicMeshCom->Render_MeshWithShader(m_pShaderCom->GetEffect(),0);

}

void CBoss::FirstEventCam(void)
{
	if(m_eMyState <= EVENT8 && false == m_pDynamicMeshCom->Get_Occupied())
	{
		++m_eMyState;
		m_pDynamicMeshCom->Set_AnimationSet(m_eMyState,false);
	}
	/*else if(m_eMyState == EVENT9 && false == m_pDynamicMeshCom->Get_Occupied())
	{
		CStage_Scene* pCurrentScene = (CStage_Scene*)Engine::GetCurrentScene();
		pCurrentScene->SetEventEndFromBossClass();
	}*/


}

CBoss* CBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss* pInst = new CBoss(pGraphicDev);

	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CBoss::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pDynamicMeshCom);
	Engine::Safe_Release(m_pShaderCom);
	Engine::Safe_Release(m_pTextureCom);

	Engine::Safe_Release(m_pShaderForRenderTarget);

	Engine::CGameObject::Free();

	((CPlayer*)Engine::GetList(L"Layer_Player")->front())->SetHumanBossNULL();


	list<CGameObject*>::iterator iter = m_EvilEyeList.begin();
	list<CGameObject*>::iterator iter_end = m_EvilEyeList.end();
	for ( ; iter != iter_end ; ++iter)
		(*iter)->SetDestroy(true);

	m_EvilEyeList.clear();

}

void CBoss::SetState(STATE eState)
{
	m_eMyState = eState;
}

void CBoss::GetState(_uint* pState)
{
	*pState = m_eMyState;
}

