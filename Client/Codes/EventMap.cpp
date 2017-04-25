#include "stdafx.h"
#include "EventMap.h"
#include "Export_Engine.h"
#include "Reaper.h"

CEventMap::CEventMap(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTransformCom(NULL)
,m_pStaticMeshCom(NULL)
,m_pShaderCom(NULL)
,m_fSelectTime(0.f)
,m_iPreRandValue(0)
,m_fTimeAcc(0.f)
,m_fFadeOutTime(0.f)
,m_isEndEvent(false)
,m_isStartChase(false)
{
	m_fMagicNum1 = 0.f;
	m_fMagicNum2 = 0.f;
}

CEventMap::~CEventMap()
{

}

HRESULT CEventMap::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

//	m_pStaticMeshCom = Engine::CStaticMesh::Create(m_pGraphicDev,L"../bin/Resources/Mesh/StaticMesh/Map/",L"Map.X");
	m_pStaticMeshCom = (Engine::CStaticMesh*)Engine::Clone(COM_STAGE,L"Component_Mesh_EventMap");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Mesh_EventMap",m_pStaticMeshCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Mesh");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Mesh",m_pShaderCom);

	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,_vec3(1.f,1.f,1.f));
	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,_vec3(0.f,-2.6f,0.f));


	//ChangeLight;

	m_tPrevLightInfo = Engine::GetLightInfo(0);

	D3DLIGHT9 tNewLight;
	ZeroMemory(&tNewLight,sizeof(D3DLIGHT9));
	tNewLight.Type = D3DLIGHT_DIRECTIONAL;
	D3DXCOLOR color = D3DXCOLOR(1.f,1.f,1.f,1.f);
	tNewLight.Diffuse = D3DXCOLOR(0.2f,0.2f,0.2f,1.f);
	tNewLight.Ambient = D3DXCOLOR(0.1f,0.1f,0.1f,1.f);
	tNewLight.Specular = D3DXCOLOR(0.2f,0.2f,0.2f,1.f);
	tNewLight.Direction = _vec3(0.f,-1.f,-2.f);
	Engine::Change_LightInfo(tNewLight,0);



	// For Red Reaper
	D3DLIGHT9 tLightInfo;
	ZeroMemory(&tLightInfo,sizeof(D3DLIGHT9));
	tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Diffuse = D3DXCOLOR(1.f,0.f,0.f,1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.f,0.f,0.f,1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f,0.f,0.f,1.f);
	tLightInfo.Range = 30.f;
	tLightInfo.Position = _vec3(0.f,50.f,0.f);
	Engine::AddLight(m_pGraphicDev,tLightInfo);

	// For Green Reaper
	ZeroMemory(&tLightInfo,sizeof(D3DLIGHT9));
	tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Diffuse = D3DXCOLOR(0.f,1.f,0.f,1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.f,0.f,0.f,1.f);
	tLightInfo.Specular = D3DXCOLOR(0.f,1.f,0.f,1.f);
	tLightInfo.Range = 30.f;
	tLightInfo.Position = _vec3(0.f,50.f,0.f);
	Engine::AddLight(m_pGraphicDev,tLightInfo);

	// For Blue Reaper
	ZeroMemory(&tLightInfo,sizeof(D3DLIGHT9));
	tLightInfo.Type = D3DLIGHT_POINT;
	tLightInfo.Diffuse = D3DXCOLOR(0.f,0.f,1.f,1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.f,0.f,0.f,1.f);
	tLightInfo.Specular = D3DXCOLOR(0.f,0.f,1.f,1.f);
	tLightInfo.Range = 30.f;
	tLightInfo.Position = _vec3(0.f,50.f,0.f);
	Engine::AddLight(m_pGraphicDev,tLightInfo);

	Engine::SetisRender(2,false);
	Engine::SetisRender(3,false);


	return S_OK;
}

_int CEventMap::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if(m_fTimeAcc > 10.f)
		m_fSelectTime += fTimeDelta;

	if(Engine::Get_KeyMgr()->KeyDown(KEY_L)) //Cheat
		m_isEndEvent = true;

	if(m_isEndEvent == true)
	{
		m_fFadeOutTime += fTimeDelta;

		_float fFade = 1 - m_fFadeOutTime*0.5f;
		m_pRendererCom->SetFadeIn(fFade);

		if(m_fFadeOutTime > 3.f)
		{
			list<CGameObject*>::iterator iter = Engine::GetList(L"Layer_EventMonster")->begin();
			list<CGameObject*>::iterator iter_end = Engine::GetList(L"Layer_EventMonster")->end();
			for( ; iter != iter_end ; ++iter)
				(*iter)->SetDestroy(true);

			m_pRendererCom->SetisFadeIn(false);
			m_isDestroy = true;
			return 0;
		}
	}
	
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	if(Engine::GetList(L"Layer_EventMonster")->size() == 0)
		m_isEndEvent = true;


	_uint iTargetSize = 6;
	if(m_fSelectTime > 2.f && m_TargetList.size() < iTargetSize && m_isStartChase == false)
	{
		list<CGameObject*>::iterator iter_green = Engine::GetList(L"Layer_EventMonster")->begin();
		list<CGameObject*>::iterator iter_blue = Engine::GetList(L"Layer_EventMonster")->begin();
		++iter_blue;
		list<CGameObject*>::iterator iter_red = Engine::GetList(L"Layer_EventMonster")->end();
		--iter_red;
	
		if(CReaper::GetCurrentOne() == NULL)
		{
			_int iRand = rand()%3;
			if(iRand == 0)
				CReaper::SelectOnce(((CReaper*)(*iter_green)));
			else if(iRand == 1)
				CReaper::SelectOnce(((CReaper*)(*iter_blue)));
			else if(iRand == 2)
				CReaper::SelectOnce(((CReaper*)(*iter_red)));
		}

	
		if(CReaper::GetCurrentOne() == *iter_green) // G , B , R
		{
			_int iRand = rand()%2;
			if(iRand == 0)
				CReaper::SelectOnce(((CReaper*)(*iter_blue)));
			else
				CReaper::SelectOnce(((CReaper*)(*iter_red)));
		}
		else if(CReaper::GetCurrentOne() == *iter_blue) // G , B , R
		{
			_int iRand = rand()%2;
			if(iRand == 0)
				CReaper::SelectOnce(((CReaper*)(*iter_green)));
			else
				CReaper::SelectOnce(((CReaper*)(*iter_red)));
		}
		else if(CReaper::GetCurrentOne() == *iter_red) // G , B , R
		{
			_int iRand = rand()%2;
			if(iRand == 0)
				CReaper::SelectOnce(((CReaper*)(*iter_green)));
			else
				CReaper::SelectOnce(((CReaper*)(*iter_blue)));
		}

		if(CReaper::GetCurrentOne() == *iter_red)
			m_TargetList.push_back(0);
		else if(CReaper::GetCurrentOne() == *iter_green)
			m_TargetList.push_back(1);
		else if(CReaper::GetCurrentOne() == *iter_blue)
			m_TargetList.push_back(2);

		m_fSelectTime = 0.f;		
	}

	if(m_TargetList.size() >= iTargetSize && m_fSelectTime > 2.f && m_isStartChase == false)
	{
		list<CGameObject*>::iterator iter_green = Engine::GetList(L"Layer_EventMonster")->begin();
		list<CGameObject*>::iterator iter_blue = Engine::GetList(L"Layer_EventMonster")->begin();
		++iter_blue;
		list<CGameObject*>::iterator iter_red = Engine::GetList(L"Layer_EventMonster")->end();
		--iter_red;

		CReaper::SelectOnce(NULL);
		((CReaper*)*iter_green)->SetMakeMoving(true);
		((CReaper*)*iter_blue)->SetMakeMoving(true);
		((CReaper*)*iter_red)->SetMakeMoving(true);

		m_isStartChase = true;
	}

	//m_pMaterialCom->Change_Material(Engine::CMaterial::DIFFUSE,&D3DXCOLOR(1.f,0.f,0.f,1.f));
	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_NONALPHA);
	return 0;

}

void CEventMap::Render_GameObject(void)
{
	m_pTransformCom->SetUp_OnGraphicDev();
	
	m_pStaticMeshCom->SetUp_ConstantTable(m_pShaderCom->GetEffect());
	m_pShaderCom->GetEffect()->SetMatrix("g_matLightView",m_pRendererCom->GetLightView());
	m_pShaderCom->GetEffect()->SetMatrix("g_matLightProj",m_pRendererCom->GetLightProj());
	//m_pShaderCom->GetEffect()->SetTexture("g_shadowTexture",m_pRendererCom->GetTargetTexture(Engine::CRenderer::FOR_SHADOW));

	Engine::SetUp_OnShader(m_pShaderCom->GetEffect(),L"Target_Shadow","g_shadowTexture");


	m_pStaticMeshCom->Render_SubSetsWithShader(m_pShaderCom->GetEffect(),m_pStaticMeshCom->GetSubSetCnt(),6);
	//m_pStaticMeshCom->Render_SubSetsWithHWInstancing(m_pShaderCom->GetEffect(),m_pStaticMeshCom->GetSubSetCnt(),*D3DXMatrixIdentity(&_matrix()),6);

	/*TCHAR szTemp[256] = L"%d";
	wsprintf(szTemp,L"Size : %d",(_uint)m_TargetList.size());
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(1000.f,290.f),D3DXCOLOR(1.f,1.f,1.f,1.f));*/

}


CEventMap* CEventMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEventMap* pInst = new CEventMap(pGraphicDev);
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CEventMap::Free(void)
{
	Engine::CGameObject::Free();

	Engine::Safe_Release(m_pShaderCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pStaticMeshCom);

	Engine::Change_LightInfo(m_tPrevLightInfo,0);
	Engine::GetCurrentScene()->SetisEventOn(false);

	Engine::SetisRender(2,true);
	Engine::SetisRender(3,true);
	Engine::SetisRender(4,false);
	Engine::SetisRender(5,false);
	Engine::SetisRender(6,false);
}

