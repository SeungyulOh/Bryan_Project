#include "stdafx.h"
#include "Event_Scene.h"
#include "Export_Engine.h"
#include "Default_Camera.h"
#include "Dynamic_Camera.h"
#include "EventMap.h"
#include "SkyBox.h"
#include "Player.h"
#include "BossDevil.h"
#include "BossWeapon.h"
#include "Dagger.h"
#include "Spirit.h"
#include "ChargeParticle_LightRay.h"

CEvent_Scene::CEvent_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CScene(pGraphicDev)
{

}

CEvent_Scene::~CEvent_Scene()
{

}

HRESULT CEvent_Scene::Ready_Scene()
{
	if(FAILED(Ready_RenderState()))
		return E_FAIL;

	if(FAILED(Ready_Layer()))
		return E_FAIL;

	return S_OK;
}

_int CEvent_Scene::Update_Scene(const _float& fTimeDelta)
{
	
	Engine::CScene::Update_Scene(fTimeDelta);

	if(Engine::Get_KeyMgr()->KeyPressed(KEY_ESCAPE))
		m_isTimeStop = !m_isTimeStop;

	return 0;
}

void CEvent_Scene::Render_Scene()
{

}

HRESULT CEvent_Scene::Ready_RenderState(void)
{
	

	D3DLIGHT9 tLightInfo;
	ZeroMemory(&tLightInfo,sizeof(D3DLIGHT9));
	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	D3DXCOLOR color = D3DXCOLOR(1.f,1.f,1.f,1.f);
	tLightInfo.Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.f);
	//tLightInfo.Diffuse = D3DXCOLOR(0.1f,0.1f,0.1f,1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.3f,0.3f,0.3f,1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f,1.f,1.f,1.f);
	tLightInfo.Direction = _vec3(0.f,-1.f,-2.f);
	
	Engine::AddLight(m_pGraphicDev,tLightInfo);
	Engine::LightEnable(m_pGraphicDev,0);


	//Á¡±¤¿ø
	/*ZeroMemory(&tLightInfo,sizeof(D3DLIGHT9));
	tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Diffuse = D3DXCOLOR(1.f,0.f,0.f,1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.3f,0.3f,0.3f,1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f,1.f,1.f,1.f);
	tLightInfo.Attenuation0 = 5;
	tLightInfo.Range = 10.f;
	tLightInfo.Position = _vec3(0.f,50.f,0.f);

	Engine::AddLight(m_pGraphicDev,tLightInfo);*/
	//Engine::LightDisable(m_pGraphicDev,1);
	


	return S_OK;
}

HRESULT CEvent_Scene::Ready_Layer(void)
{

	Engine::CLayer* pLayer = NULL;
	Engine::CGameObject* pGameObject = NULL;
	//---------------Layer_Cam
	
	pLayer = Engine::CLayer::Create();
	
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
	pGameObject = CEventMap::Create(m_pGraphicDev);
	if(pGameObject == NULL)
		return E_FAIL;
	pLayer->Add_Object(pGameObject);

	//--SkyBox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	if(pGameObject == NULL)
		return E_FAIL;
	pLayer->Add_Object(pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Back",pLayer));


	
	//-------------Layer_Player
	pLayer = Engine::CLayer::Create();
	//--Player
	pGameObject = CPlayer::Create(m_pGraphicDev);
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
	//Engine::CGameObject* pBoss = m_pBoss = CBoss::Create(m_pGraphicDev);
	Engine::CGameObject* pBoss = CBossDevil::Create(m_pGraphicDev);
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
	//--Weapon
	pGameObject = CDagger::Create(m_pGraphicDev);
	if(pGameObject == NULL)
		return E_FAIL;

	((CDagger*)pGameObject)->SetPlayerMat(((Engine::CTransform*)pComponent)->GetWorldMatrix());

	pLayer->Add_Object(pGameObject);
	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Weapon",pLayer));

	///--------------Layer_Distortion
	pLayer = Engine::CLayer::Create();
	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Distortion",pLayer));

	///--------------Layer_Effect
	pLayer = Engine::CLayer::Create();

	/*pGameObject = CPhantom_Trail::Create(m_pGraphicDev);
	pLayer->Add_Object(pGameObject);*/
	
	pGameObject = CSpirit::Create(m_pGraphicDev);
	pLayer->Add_Object(pGameObject);

	pGameObject = CChargeParticle_LightRay::Create(m_pGraphicDev,&_vec3(0.f,0.f,0.f),100);
	pLayer->Add_Object(pGameObject);


	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Effect",pLayer));

	///--------------Layer_Trail
	pLayer = Engine::CLayer::Create();

	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Trail",pLayer));

	pLayer = Engine::CLayer::Create();

	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Buff",pLayer));

	return S_OK;
}

CEvent_Scene* CEvent_Scene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEvent_Scene* pInst = new CEvent_Scene(pGraphicDev);

	if(FAILED(pInst->Ready_Scene()))
	{
		Engine::Safe_Release(pInst);
		return NULL;
	}

	return pInst;
}

void CEvent_Scene::Free(void)
{
	Engine::CScene::Free();
}
