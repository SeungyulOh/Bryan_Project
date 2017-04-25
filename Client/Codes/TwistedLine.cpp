#include "stdafx.h"
#include "TwistedLine.h"
#include "Export_Engine.h"

CTwistedLine::CTwistedLine(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)
{

}

CTwistedLine::~CTwistedLine(void)
{

}

HRESULT CTwistedLine::Ready_GameObject(void)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pStaticMeshCom = (Engine::CStaticMesh*)Engine::Clone(COM_STAGE,L"Component_Mesh_TwistedLine");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Mesh_TwistedLine",m_pStaticMeshCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Mesh");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Mesh",m_pShaderCom);


	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,_vec3(1.5f,1.5f,1.5f));
	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,_vec3(0.f,0.f,20.f));


	Engine::CTransform* pPlayerTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
	m_matPlayerWorld = pPlayerTransform->GetWorldMatrix();

	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,*(_vec3*)&m_matPlayerWorld->m[3][0]);

	m_fTimeAcc = 0.f;

	return S_OK;
}

_int CTwistedLine::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if(m_fTimeAcc > 1.f)
		m_isDestroy = true;

	_matrix matScale,matRot,matRot2,matTrans;
	D3DXMatrixScaling(&matScale,0.1f,0.1f,0.1f);
	D3DXMatrixRotationY(&matRot,-D3DX_PI/2.f);
	D3DXMatrixRotationZ(&matRot2,-m_fTimeAcc*10.f);
	D3DXMatrixTranslation(&matTrans,0.f,3.f,0.f);
	m_matLocal = matScale * matRot * matRot2 * matTrans;

	m_pTransformCom->SetWorldmat(m_matLocal * Engine::MyGetNewMatExcScale(*m_matPlayerWorld));
	
	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_NONALPHA);

	return 0;
}

void CTwistedLine::Render_GameObject(void)
{
	m_pTransformCom->SetUp_OnGraphicDev();
	m_pStaticMeshCom->SetUp_ConstantTable(m_pShaderCom->GetEffect());
	m_pStaticMeshCom->Render_SubSetsWithShader(m_pShaderCom->GetEffect(),m_pStaticMeshCom->GetSubSetCnt(),5);

}

CTwistedLine* CTwistedLine::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTwistedLine* pInst = new CTwistedLine(pGraphicDev);

	if(FAILED(pInst->Ready_GameObject()))
		return NULL;

	return pInst;
}

void CTwistedLine::Free(void)
{
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pStaticMeshCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CGameObject::Free();
}
