#include "stdafx.h"
#include "BossMap.h"
#include "Export_Engine.h"

CBossMap::CBossMap(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
,m_pRendererCom(NULL)
,m_pTransformCom(NULL)
,m_pStaticMeshCom(NULL)
,m_pShaderCom(NULL)
{
	m_fMagicNum1 = 0.f;
	m_fMagicNum2 = 0.f;
}

CBossMap::~CBossMap()
{

}

HRESULT CBossMap::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

//	m_pStaticMeshCom = Engine::CStaticMesh::Create(m_pGraphicDev,L"../bin/Resources/Mesh/StaticMesh/Map/",L"Map.X");
	m_pStaticMeshCom = (Engine::CStaticMesh*)Engine::Clone(COM_STAGE,L"Component_Mesh_BossMap");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Mesh_BossMap",m_pStaticMeshCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Mesh");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Mesh",m_pShaderCom);

	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,_vec3(0.1f,0.1f,0.1f));
	return S_OK;
}

_int CBossMap::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	
	//m_pMaterialCom->Change_Material(Engine::CMaterial::DIFFUSE,&D3DXCOLOR(1.f,0.f,0.f,1.f));
	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_NONALPHA);
	return 0;

}

void CBossMap::Render_GameObject(void)
{

	m_pTransformCom->SetUp_OnGraphicDev();
	
	m_pStaticMeshCom->SetUp_ConstantTable(m_pShaderCom->GetEffect());
	m_pShaderCom->GetEffect()->SetMatrix("g_matLightView",m_pRendererCom->GetLightView());
	m_pShaderCom->GetEffect()->SetMatrix("g_matLightProj",m_pRendererCom->GetLightProj());
	//m_pShaderCom->GetEffect()->SetTexture("g_shadowTexture",m_pRendererCom->GetTargetTexture(Engine::CRenderer::FOR_SHADOW));

	Engine::SetUp_OnShader(m_pShaderCom->GetEffect(),L"Target_Shadow","g_shadowTexture");

	if(GetAsyncKeyState('3'))
	{
		m_fMagicNum1 += 0.01f;
		m_pShaderCom->GetEffect()->SetFloat("g_fMagicNum1",m_fMagicNum1);
	}
	if(GetAsyncKeyState('4'))
	{
		m_fMagicNum1 -= 0.01f;
		m_pShaderCom->GetEffect()->SetFloat("g_fMagicNum1",m_fMagicNum1);
	}

	if(GetAsyncKeyState('5'))
	{
		m_fMagicNum2 += 0.001f;
		m_pShaderCom->GetEffect()->SetFloat("g_fMagicNum2",m_fMagicNum2);
	}
	if(GetAsyncKeyState('6'))
	{
		m_fMagicNum2 -= 0.001f;
		m_pShaderCom->GetEffect()->SetFloat("g_fMagicNum2",m_fMagicNum2);
	}

	m_pStaticMeshCom->Render_SubSetsWithShader(m_pShaderCom->GetEffect(),m_pStaticMeshCom->GetSubSetCnt(),6);
	//m_pStaticMeshCom->Render_SubSetsWithHWInstancing(m_pShaderCom->GetEffect(),m_pStaticMeshCom->GetSubSetCnt(),*D3DXMatrixIdentity(&_matrix()),6);

	/*TCHAR szTemp[256] = L"%d";
	wsprintf(szTemp,L"Magic1 : %d",_uint(m_fMagicNum1 * 10000.f));
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(200.f,250.f),D3DXCOLOR(1.f,1.f,1.f,1.f));

	wsprintf(szTemp,L"Magic2: %d",_uint(m_fMagicNum2 * 10000.f));
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(200.f,270.f),D3DXCOLOR(1.f,1.f,1.f,1.f));*/

}


CBossMap* CBossMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossMap* pInst = new CBossMap(pGraphicDev);
	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CBossMap::Free(void)
{
	Engine::CGameObject::Free();

	Engine::Safe_Release(m_pShaderCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pStaticMeshCom);
}

