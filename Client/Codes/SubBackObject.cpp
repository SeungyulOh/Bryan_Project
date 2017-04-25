#include "stdafx.h"
#include "SubBackObject.h"
#include "Export_Engine.h"
#include "WaterFall.h"
#include "ExpEffect.h"
#include "ChildEffect.h"

CSubBackObject::CSubBackObject(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
,m_pShader(NULL)
,m_fOffSet(0.f)
,m_isOnce(false)
{
	
}

CSubBackObject::~CSubBackObject(void)
{

}

HRESULT CSubBackObject::Ready_GameObject(const _tchar* pNameTag,_vec3 vFirstPos)
{
	pName = pNameTag;

	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pStaticMeshCom = (Engine::CStaticMesh*)Engine::Clone(COM_STAGE,pName);
	Push_CloneComponent(CGameObject::NONUPDATE,pName,m_pStaticMeshCom);

	m_pShader = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Mesh");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Mesh",m_pShader);

	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,_vec3(1.f,1.f,1.f));
	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vFirstPos);
	m_pTransformCom->Update_Component(0.f);
	
	return S_OK;
}

_int CSubBackObject::Update_GameObject(const _float& fTimeDelta)
{
	if(!lstrcmp(pName,L"floating_island02") || !lstrcmp(pName,L"floating_island03"))// || !lstrcmp(pName,L"floating_island03"))
	{
		if(m_isOnce == false)
		{
			_float fNewY = m_pTransformCom->GetInfomation(Engine::CTransform::POSITION)->y - 100.f;
			m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,_vec3(m_pTransformCom->GetInfomation(Engine::CTransform::POSITION)->x,fNewY,m_pTransformCom->GetInfomation(Engine::CTransform::POSITION)->z));

			Engine::CGameObject* pGameObject = NULL;
			pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WATERFALL,CExpEffect::WATERFALL2,m_pTransformCom->GetWorldMatrix());
			Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		}
		m_isOnce = true;
	}


	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_NONALPHA);
	return 0;
}

void CSubBackObject::Render_GameObject(void)
{

	m_fOffSet += 0.01f;
	m_pTransformCom->SetUp_OnGraphicDev();
	m_pStaticMeshCom->SetUp_ConstantTable(m_pShader->GetEffect());

	if(!lstrcmp(pName,L"elk_waterfall01"))
	{
		m_pShader->GetEffect()->SetFloat("g_OffSet",m_fOffSet);
		m_pStaticMeshCom->Render_SubSetsWithShader(m_pShader->GetEffect(),1,Engine::CShader::WATER);
	}
	else if(!lstrcmp(pName,L"cs_prop_dragon"))
	{
		return;
	}
	else if(!lstrcmp(pName,L"branchtree01") || !lstrcmp(pName,L"branchtree02") || !lstrcmp(pName,L"branchtree03"))
	{
		m_pStaticMeshCom->Render_SubSetsWithShader(m_pShader->GetEffect(),m_pStaticMeshCom->GetSubSetCnt(),Engine::CShader::ALPHATEST);
	}
	else
	{
		m_pStaticMeshCom->Render_SubSetsWithShader(m_pShader->GetEffect(),m_pStaticMeshCom->GetSubSetCnt());
	}

}



CSubBackObject* CSubBackObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pNameTag,_vec3 vFirstPos)
{
	CSubBackObject* pInst = new CSubBackObject(pGraphicDev);

	if(FAILED(pInst->Ready_GameObject(pNameTag,vFirstPos)))
		return NULL;

	return pInst;
}

void CSubBackObject::Free(void)
{
	Engine::Safe_Delete_Array(pName);

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pStaticMeshCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pShader);

	Engine::CGameObject::Free();
}

