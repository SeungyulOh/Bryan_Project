#include "stdafx.h"
#include "Stage_Scene.h"
#include "Export_Engine.h"
#include "Terrain.h"
#include "Default_Camera.h"
#include "SkyBox.h"
#include "BossMap.h"
#include "Player.h"
#include "TwistedLine.h"
#include "Dagger.h"
#include "Dynamic_Camera.h"
#include "Boss.h"
#include "BossWeapon.h"
#include "Action_Camera.h"
#include "Dragon.h"
#include "Verafim.h"
#include "NpcTi.h"
#include "BossDevil.h"
#include "Phantom_Trail.h"
#include "Snow.h"
#include "ChargeParticle_LightGlow.h"
#include "ChargeParticle_LightRay.h"
#include "Spirit.h"
#include "Party_SprayA.h"
#include "LightningCircle.h"
#include "LightningAni.h"
#include "RingWind.h"
#include "SimpleAura.h"
#include "ShapeFire.h"
#include "Blade1.h"
#include "Atk4Swing.h"
#include "Fog.h"
#include "WaterFall.h"
#include "SkyFog.h"
#include "ChildEffect.h"
#include "ExpEffect.h"
#include "EventMap.h"
#include "SoundMgr.h"

CStage_Scene::CStage_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CScene(pGraphicDev)
,m_pLoading(NULL)
,m_bDragonSpawn(false)
,m_isBossSpawn(false)
,m_pDefaultCam(NULL)
,m_bEventEndFromBossClass(false)
,m_pBoss(NULL)
,m_isRenderLine(false)
,m_fWaterPumpCoolTime(0.f)
,m_fFadeTime(0.f)
,m_isFade(false)
,m_fTimeDelta(0.f)
,m_bBossEventBlur_Ing(false)
{

}

CStage_Scene::~CStage_Scene()
{

}

HRESULT CStage_Scene::Ready_Scene()
{
	//Change to true when you are ready to show your works
	m_bDragonSpawn = true;
	//m_isBossSpawn = true; // Delete When you finish debugging 20170414;

	if(FAILED(Ready_RenderState()))
		return E_FAIL;

	if(FAILED(Ready_Layer()))
		return E_FAIL;

	//m_pLoading = CLoading::Create(m_pGraphicDev,CLoading::LOADING_DRAGON);
	
	Load_Effect();

	CSoundMgr::GetInstance()->StopBGM(L"bgm_ep10_kadan_prev.wav");
	CSoundMgr::GetInstance()->PlayBGM(L"bgm_ep10_kadan.wav");

	


	return S_OK;
}

_int CStage_Scene::Update_Scene(const _float& fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;
	Engine::CScene::Update_Scene(fTimeDelta);

	m_fWaterPumpCoolTime += fTimeDelta;
	if(m_fWaterPumpCoolTime > 7.f)
	{
		_int fRandX = rand()%30;
		_int fRandZ = rand()%30;
		D3DXMatrixTranslation(&m_matWaterTrans,(_float)fRandX,0.f,(_float)fRandZ);

		Engine::CGameObject* pGameObject = NULL;
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SPRINTRUN,CExpEffect::SPRINTRUN5,&m_matWaterTrans);
		Engine::GetLayer(L"Layer_EffectNotInEvent")->Add_Object(pGameObject);
		m_fWaterPumpCoolTime = 0.f;
	}

	/*if(NULL != m_pLoading && true == m_pLoading->Get_Finish())
	{
		WaitForSingleObject(m_pLoading->Get_Handle(), INFINITE);
		CloseHandle(m_pLoading->Get_Handle());	
		DeleteCriticalSection(m_pLoading->Get_CriticalSection());
		Engine::Safe_Release(m_pLoading);
		m_bDragonSpawn = true;
	}*/

	if(Engine::Get_KeyMgr()->KeyPressed(KEY_ESCAPE))
		m_isTimeStop = !m_isTimeStop;

	if(m_pBoss == NULL)
		return 0;

	if((m_pBoss->GetState() <= CBoss::EVENT9 && GetAsyncKeyState('0') & 0x8000) || m_bEventEndFromBossClass == true)
	{
		CSoundMgr::GetInstance()->StopBGM(L"bgm_kadan_start.wav");
		CSoundMgr::GetInstance()->PlayBGM(L"bgm_ep10_kadan.wav");
		AddDefaultCamAfterEvent();
		m_bEventEndFromBossClass = false;
	}	
	return 0;
}

void CStage_Scene::Render_Scene()
{
	//event start
	if(m_isBossSpawn== false && Engine::Get_KeyMgr()->KeyPressed(KEY_9))
	{
		m_isFade = true;
		CSoundMgr::GetInstance()->PlayBGM(L"bgm_kadan_start.wav");
	}
	if(m_isFade)
	{
		m_fFadeTime += m_fTimeDelta;
		Engine::GetRenderer()->SetFadeOut(cosf(m_fFadeTime));

		if(m_fFadeTime > D3DX_PI * 1.5f && m_bDragonSpawn == true)
		{
			m_bDragonSpawn = false;
			StartBossEvent();

			m_pPlayer->GetTransform()->SetInfomation(Engine::CTransform::ANGLE,_vec3(0.f,D3DX_PI,0.f));
			m_pPlayer->GetTransform()->SetInfomation(Engine::CTransform::POSITION,_vec3(0.f,1.f,150.f));
		}
		if(m_fFadeTime > D3DX_PI * 2.f)
			m_isFade = false;
	}

	if(GetAsyncKeyState(VK_SHIFT))
		m_isRenderLine = true;
	else
		m_isRenderLine = false;

	if(m_isRenderLine == true)
		Engine::Get_NaviMgr()->Render();

	/*TCHAR szTemp[256] = L"%d";
	wsprintf(szTemp,L"Player X : %d",(_uint)m_fWaterPumpCoolTime);
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(200.f,490.f),D3DXCOLOR(1.f,1.f,1.f,1.f));*/
}

HRESULT CStage_Scene::Ready_RenderState(void)
{
	/*Engine::Set_RenderState(D3DRS_ZENABLE,TRUE);
	Engine::Set_RenderState(D3DRS_ZWRITEENABLE,TRUE);
	Engine::Set_RenderState(D3DRS_LIGHTING,TRUE);

	m_pGraphicDev->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);*/

	//Engine::Set_RenderState(D3DRS_CULLMODE,D3DCULL_NONE);

	D3DLIGHT9 tLightInfo;
	ZeroMemory(&tLightInfo,sizeof(D3DLIGHT9));
	
	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	D3DXCOLOR color = D3DXCOLOR(0.1f,0.1f,0.1f,1.f);
	tLightInfo.Diffuse = D3DXCOLOR(0.3f,0.3f,0.45f,1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.2f,0.2f,0.2f,1.f);
	tLightInfo.Specular = D3DXCOLOR(0.3f,0.3f,0.45f,1.f);
	tLightInfo.Direction = _vec3(0.f,-1.f,-2.f);
	
	Engine::AddLight(m_pGraphicDev,tLightInfo);

	////Á¡±¤¿ø For Player
	ZeroMemory(&tLightInfo,sizeof(D3DLIGHT9));
	tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Diffuse = D3DXCOLOR(1.f,1.f,0.8f,1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.f,0.f,0.f,1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f,1.f,0.8f,1.f);
	tLightInfo.Range = 10.f;
	tLightInfo.Position = _vec3(-250.f,5.f,0.f);

	Engine::AddLight(m_pGraphicDev,tLightInfo);


	//For Boss
	ZeroMemory(&tLightInfo,sizeof(D3DLIGHT9));
	tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Diffuse = D3DXCOLOR(0.6f,0.f,0.f,1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.f,0.f,0.f,1.f);
	tLightInfo.Specular = D3DXCOLOR(0.6f,0.f,0.f,1.f);
	tLightInfo.Range = 10.f;
	tLightInfo.Position = _vec3(-250.f,5.f,0.f);

	Engine::AddLight(m_pGraphicDev,tLightInfo);


	// For Center Terrain
	ZeroMemory(&tLightInfo,sizeof(D3DLIGHT9));
	tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Diffuse = D3DXCOLOR(0.3f,0.3f,0.3f,1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.f,0.f,0.f,1.f);
	tLightInfo.Specular = D3DXCOLOR(0.3f,0.3f,0.3f,1.f);
	tLightInfo.Range = 200.f;
	tLightInfo.Position = _vec3(0.f,50.f,0.f);
	Engine::AddLight(m_pGraphicDev,tLightInfo);

	return S_OK;
}

HRESULT CStage_Scene::Ready_Layer(void)
{
	Engine::CLayer* pLayer = NULL;
	Engine::CGameObject* pGameObject = NULL;
	//---------------Layer_Cam
	
	pLayer = Engine::CLayer::Create();
	pLayer->SetisAlive(true);
	
	/*Engine::CGameObject* pActionCam = CAction_Camera::Create(m_pGraphicDev);
	pLayer->Add_Object(pActionCam);*/

	Engine::CGameObject* pCamera = CDefault_Camera::Create(m_pGraphicDev,_vec3(0.f,10.f,-10.f),_vec3(0.f,0.f,0.f));
	if(pCamera == NULL)
		return E_FAIL;

	pLayer->Add_Object(pCamera);
	m_pDefaultCamLayer = pLayer;
	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Cam",pLayer));

	//-----------------Layer_DynamicCam
	Engine::CLayer* pDynamicCamLayer = Engine::CLayer::Create();
	Engine::CGameObject* pDynamicCamera = CDynamic_Camera::Create(m_pGraphicDev);
	if(pDynamicCamera == NULL)
		return E_FAIL;

	pDynamicCamLayer->Add_Object(pDynamicCamera);
	m_pDynamicCamLayer = pDynamicCamLayer;
	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_DynamicCam",pDynamicCamLayer));

	//--------------Layer_Back
	pLayer = Engine::CLayer::Create();

	//--BossMap
	pGameObject = CBossMap::Create(m_pGraphicDev);
	if(pGameObject == NULL)
		return E_FAIL;
	pLayer->Add_Object(pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Back",pLayer));


	//--------------Layer_Back
	pLayer = Engine::CLayer::Create();
	pLayer->SetisAlive(true);
	//--SkyBox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	if(pGameObject == NULL)
		return E_FAIL;
	pLayer->Add_Object(pGameObject);
	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_SkyBox",pLayer));



	
	//-------------Layer_Player
	pLayer = Engine::CLayer::Create();
	pLayer->SetisAlive(true);
	//--Player
	pGameObject = m_pPlayer = CPlayer::Create(m_pGraphicDev);
	if(pGameObject == NULL)
		return E_FAIL;
	pLayer->Add_Object(pGameObject);
	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Player",pLayer));

	Engine::CComponent* pComponent = pGameObject->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
	//((CDefault_Camera*)pCamera)->SetTarget(((Engine::CTransform*)pComponent)->GetInfomation(Engine::CTransform::POSITION));

	

	//----------------Layer_Boss
	pLayer = Engine::CLayer::Create();
	//--
	//For Boss Pattern
	Engine::CGameObject* pBoss = m_pBoss = CBoss::Create(m_pGraphicDev);
	//Engine::CGameObject* pBoss = CBossDevil::Create(m_pGraphicDev);
	if(pBoss == NULL)
		return E_FAIL;
	pLayer->Add_Object(pBoss);

	pGameObject = CBossWeapon::Create(m_pGraphicDev);
	if(pGameObject == NULL)
		return E_FAIL;
	pLayer->Add_Object(pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Boss",pLayer));


	///--------------Layer_Weapon
	pLayer = Engine::CLayer::Create();
	pLayer->SetisAlive(true);
	//--Weapon
	pGameObject = CDagger::Create(m_pGraphicDev);
	if(pGameObject == NULL)
		return E_FAIL;

	((CDagger*)pGameObject)->SetPlayerMat(((Engine::CTransform*)pComponent)->GetWorldMatrix());

	pLayer->Add_Object(pGameObject);
	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Weapon",pLayer));

	///--------------Layer_Distortion
	pLayer = Engine::CLayer::Create();
	pLayer->SetisAlive(true);
	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Distortion",pLayer));

	///--------------Layer_Effect
	pLayer = Engine::CLayer::Create();
	pLayer->SetisAlive(true);

	/*pGameObject = CPhantom_Trail::Create(m_pGraphicDev);
	pLayer->Add_Object(pGameObject);*/
	
	pGameObject = CSpirit::Create(m_pGraphicDev);
	pLayer->Add_Object(pGameObject);

	pGameObject = CChargeParticle_LightRay::Create(m_pGraphicDev,&_vec3(0.f,0.f,0.f),100);
	pLayer->Add_Object(pGameObject);


	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Effect",pLayer));

	///Layer Effect Not In Effect
	pLayer = Engine::CLayer::Create();
	pLayer->SetisAlive(false);

	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_EffectNotInEvent",pLayer));

	///--------------Layer_Trail
	pLayer = Engine::CLayer::Create();
	pLayer->SetisAlive(true);

	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Trail",pLayer));

	//----------------Layer _ EventMonster
	pLayer = Engine::CLayer::Create();
	pLayer->SetisAlive(true);

	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_EventMonster",pLayer));

	//--------------Layer - Buff

	pLayer = Engine::CLayer::Create();
	pLayer->SetisAlive(true);

	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Buff",pLayer));

	pLayer = Engine::CLayer::Create();
	pGameObject = CFog::Create(m_pGraphicDev);
	pLayer->Add_Object(pGameObject);
	pGameObject = CFog::Create(m_pGraphicDev,1);
	pLayer->Add_Object(pGameObject);
	pGameObject = CFog::Create(m_pGraphicDev,2);
	pLayer->Add_Object(pGameObject);
	pGameObject = CWaterFall::Create(m_pGraphicDev,*D3DXMatrixIdentity(&_matrix()),2);
	pLayer->Add_Object(pGameObject);
	pGameObject = CWaterFall::Create(m_pGraphicDev,*D3DXMatrixIdentity(&_matrix()),2);
	pLayer->Add_Object(pGameObject);
	pGameObject = CWaterFall::Create(m_pGraphicDev,*D3DXMatrixIdentity(&_matrix()),2);
	pLayer->Add_Object(pGameObject);

	/*for(_uint i=0;i<2;++i)
	{
		Engine::CGameObject* pGameObject = CSkyFog::Create(m_pGraphicDev,0,2);
		pLayer->Add_Object(pGameObject);
		pGameObject = CSkyFog::Create(m_pGraphicDev,1,2);
		pLayer->Add_Object(pGameObject);
	}*/
	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Environment",pLayer));


	pLayer = Engine::CLayer::Create();
	pLayer->SetisAlive(true);
	

	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Event",pLayer));




	if(m_bDragonSpawn == false)
		return S_OK;

	//-------------Layer_Dragon
	pLayer = Engine::CLayer::Create();
	//--Dragon

	pGameObject = CDragon::Create(m_pGraphicDev);
	if(pGameObject == NULL)
		return E_FAIL;
	pLayer->Add_Object(pGameObject);

	pGameObject = CVerafim::Create(m_pGraphicDev);
	if(pGameObject == NULL)
		return E_FAIL;
	pLayer->Add_Object(pGameObject);

	pGameObject = CNpcTi::Create(m_pGraphicDev);
	if(pGameObject == NULL)
		return E_FAIL;
	pLayer->Add_Object(pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Dragon",pLayer));
	

	return S_OK;
}

void CStage_Scene::StartBossEvent(void)
{
	m_isBossSpawn = true;
	m_bBossEventBlur_Ing = true;

	//ActionCam Add
	Engine::CGameObject* pActionCam = CAction_Camera::Create(m_pGraphicDev);
	Engine::CLayer* pLayer = Engine::GetLayer(L"Layer_Cam");

	m_pDefaultCam = pLayer->GetList()->front();
	m_pDefaultCam->AddRefCnt();

	pLayer->GetList()->clear();
	pLayer->Add_Object(pActionCam);

	Engine::CDynamicMesh* pBossDynamicMesh = (Engine::CDynamicMesh*)Engine::GetList(L"Layer_Boss")->front()->FindComponent(Engine::CGameObject::NONUPDATE,L"Component_Mesh_Kadan");
	CBoss* pBoss = (CBoss*)Engine::GetList(L"Layer_Boss")->front();
	pBoss->SetState(CBoss::EVENT1);
	pBossDynamicMesh->Set_AnimationSet(0,false);
}

CStage_Scene* CStage_Scene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage_Scene* pInst = new CStage_Scene(pGraphicDev);

	if(FAILED(pInst->Ready_Scene()))
	{
		Engine::Safe_Release(pInst);
		return NULL;
	}

	return pInst;
}

void CStage_Scene::Free(void)
{

	if(m_pDefaultCam != NULL)
		Engine::Safe_Release(m_pDefaultCam);

	//if(m_pBoss != NULL)
	//	Engine::Safe_Release(m_pBoss);

	list<_tchar*>::iterator iter = m_NameList.begin();
	list<_tchar*>::iterator iter_end = m_NameList.end();
	
	for(; iter!=iter_end; ++iter)
	{
		Engine::Safe_Delete_Array(*iter);
	}
	

	Engine::CScene::Free();
}

void CStage_Scene::Add_Layer_AfterLoading(const _tchar* pLayerTag,Engine::CLayer* pLayer)
{
	m_mapLayer.insert(make_pair(pLayerTag,pLayer));
}

void CStage_Scene::AddDefaultCamAfterEvent(void)
{
	Engine::CLayer* pLayer = GetLayer(L"Layer_Cam");
	Engine::Safe_Release(pLayer->GetList()->front());
	pLayer->GetList()->clear();

	DeleteLayer(L"Layer_Dragon");

	pLayer->Add_Object(m_pDefaultCam);


	Engine::CGameObject* pDevilBoss = CBossDevil::Create(m_pGraphicDev);

	pLayer = GetLayer(L"Layer_Boss");
	Engine::Safe_Release(pLayer->GetList()->front());
	pLayer->GetList()->erase(pLayer->GetList()->begin());
	pLayer->GetList()->push_front(pDevilBoss);

	((CBossWeapon*)pLayer->GetList()->back())->SetBoolOnce();
	m_pPlayer->SetBoolOnce();
	m_bBossEventBlur_Ing = false;
}


void CStage_Scene::Load_Effect(void)
{
	DWORD	dwByte = 0;

	HANDLE	hFile  = CreateFile(L"../../Data/Trail.dat", 
		GENERIC_READ, 
		0, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	while(true)
	{
		_tchar* pName = new _tchar[256];
		ReadFile(hFile,pName, sizeof(_tchar)*256, &dwByte, NULL);

		if(dwByte == 0)
		{
			Engine::Safe_Delete_Array(pName);
			break;
		}

		map<const _tchar*,vector<EFFECTDATA>>::iterator iter = find_if(m_mapEffect.begin(),m_mapEffect.end(),Engine::CTagFinder(pName));
		if(iter == m_mapEffect.end())
		{
			m_mapEffect.insert(make_pair(pName,vector<EFFECTDATA>()));
			m_NameList.push_back(pName);
		}

		_uint iVecSize = 0;
		ReadFile(hFile, &iVecSize, sizeof(_uint), &dwByte, NULL);

		EFFECTDATA tEffectData;
		for(_uint i = 0; i<iVecSize ; ++i)
		{
			ReadFile(hFile, &tEffectData, sizeof(EFFECTDATA), &dwByte, NULL);
			m_mapEffect[pName].push_back(tEffectData);
		}
	}

	CloseHandle(hFile);
}

std::vector<EFFECTDATA>* CStage_Scene::GetEffectVector(const _tchar* pAnimationTag)
{
	map<const _tchar*,vector<EFFECTDATA>>::iterator iter_found = find_if(m_mapEffect.begin(),m_mapEffect.end(),Engine::CTagFinder(pAnimationTag));
	if(iter_found == m_mapEffect.end())
		return NULL;

	return &iter_found->second;
}
