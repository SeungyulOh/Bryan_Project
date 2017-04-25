#include "stdafx.h"
#include "Logo_Scene.h"
#include "Export_Engine.h"
#include "LogoBack.h"
#include "Stage_Scene.h"
#include "Event_Scene.h"

CLogo_Scene::CLogo_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CScene(pGraphicDev)
,m_pLoading(NULL)
{

}

CLogo_Scene::~CLogo_Scene()
{

}

HRESULT CLogo_Scene::Ready_Scene()
{
	if(FAILED(Ready_RenderState()))
		return E_FAIL;

	if(FAILED(Ready_Layer()))
		return E_FAIL;	

	m_pLoading = CLoading::Create(m_pGraphicDev,CLoading::LOADING_STAGE);

	return S_OK;
}

_int CLogo_Scene::Update_Scene(const _float& fTimeDelta)
{
	if(true == m_pLoading->Get_Finish())
	{
		if(GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			Engine::CScene* pStage = CStage_Scene::Create(m_pGraphicDev);
			((CStage_Scene*)pStage)->Add_Layer_AfterLoading(L"Layer_SubObject",m_pLoading->Export_LoadedLayer());
			Engine::Set_CurrentScene(pStage);


			return 0;
		}
	}

	Engine::CScene::Update_Scene(fTimeDelta);

	return 0;
}

void CLogo_Scene::Render_Scene()
{
	Engine::Render_Font(L"Font_Batang",m_pLoading->Get_String(),&_vec2(300.f,0.f),D3DXCOLOR(1.f,1.f,0.f,1.f));
}

HRESULT CLogo_Scene::Ready_RenderState(void)
{
	//Engine::Set_RenderState(D3DRS_ZENABLE,FALSE);
	//Engine::Set_RenderState(D3DRS_ZWRITEENABLE,FALSE);
	//Engine::Set_RenderState(D3DRS_LIGHTING,FALSE);

	return S_OK;
}

HRESULT CLogo_Scene::Ready_Layer(void)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();

	Engine::CGameObject* pGameObject = CLogoBack::Create(m_pGraphicDev);
	if(pGameObject == NULL)
		return E_FAIL;

	pLayer->Add_Object(pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Back",pLayer));
	

	return S_OK;
}

CLogo_Scene* CLogo_Scene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo_Scene* pInst = new CLogo_Scene(pGraphicDev);

	if(FAILED(pInst->Ready_Scene()))
	{
		Engine::Safe_Release(pInst);
		return NULL;
	}

	return pInst;
}

void CLogo_Scene::Free(void)
{

	Engine::Safe_Release(m_pLoading);

	Engine::Release_Component(COM_LOGO);
	Engine::CScene::Free();
}
