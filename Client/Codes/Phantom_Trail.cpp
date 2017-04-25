#include "stdafx.h"
#include "Phantom_Trail.h"
#include "Export_Engine.h"
#include "TrailParticle.h"

CPhantom_Trail::CPhantom_Trail(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CDynamicMesh* pPlayerDynamicMesh)
:Engine::CGameObject(pGraphicDev)
,m_pTransformCom(NULL)
,m_pRendererCom(NULL)
,m_pTrailBufferCom(NULL)
,m_pTextureCom(NULL)
,m_pShaderCom(NULL)
,m_isOnce(false)
,m_pPlayerDynamicMesh(pPlayerDynamicMesh)
{
	m_fDestroyTime = 0.f;

	

}

CPhantom_Trail::~CPhantom_Trail(void)
{

}

void CPhantom_Trail::Ready_Trail(EFFECTDATA tData)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pTrailBufferCom = (Engine::CTrail_Texture*)Engine::Clone(COM_STATIC,L"Component_Buffer_TrailTexture");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_TrailTexture",m_pTrailBufferCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_PhantomTrail");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_PhantomTrail",m_pTextureCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Effect");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Effect",m_pShaderCom);

	m_pBoxColliderCom = (Engine::CBoxCollider*)Engine::Clone(COM_STAGE,L"Component_BoxCollider");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_BoxCollider",m_pBoxColliderCom);
	m_pBoxColliderCom->SetMinMaxDirect(_vec3(-30.f,-20.f,-40.f),_vec3(30.f,20.f,40.f));

	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,Engine::_vec3(10.f,10.f,10.f));
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,Engine::_vec3(D3DX_PI/2.f,0.f,0.f));
	m_tEffectData = tData;

	Engine::CTransform* pPlayerTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Player")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
	m_pPlayerWorld = pPlayerTransform->GetWorldMatrix();

	m_pTransformCom->SetWorldmat(m_tEffectData.matLocal * *m_pPlayerWorld);

	m_pShaderCom->GetEffect()->SetFloat("g_fSlope",m_tEffectData.fSlope);
	m_pShaderCom->GetEffect()->SetFloat("g_fScale",m_tEffectData.fScale);
	m_pShaderCom->GetEffect()->SetFloat("g_fEffectRadius",m_tEffectData.fEffectRadius);


	Engine::D3DXFRAME_DERIVED* pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(m_pPlayerDynamicMesh->GetRootFrame(),"ValveBiped_Anim_Attachment_RH");
	if(pFrameFound == NULL)
		return;
	m_RHFrameMatrix = &pFrameFound->CombinedMatrix;

	D3DXMatrixScaling(&m_matAtkRange,m_tEffectData.fAtkRange.x,m_tEffectData.fAtkRange.y,m_tEffectData.fAtkRange.z);
	ZeroMemory(m_vTrailVector,sizeof(D3DXVECTOR4)*40);

	m_vWorldPos = D3DXVECTOR4(*(_vec3*)&((*m_RHFrameMatrix * *m_pPlayerWorld).m[3][0]),1.f);

	m_pPlayerAtkState = ((CPlayer*)Engine::GetList(L"Layer_Player")->front())->GetAtkState();

	//Engine::CGameObject* pGameObject = CTrailParticle::Create(m_pGraphicDev,30,&m_vWorldPos);
	//Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

	m_fViewZ = 0.f;
}

_int CPhantom_Trail::Update_GameObject(const _float& fTimeDelta)
{
	if(m_pPlayerDynamicMesh->Get_CurrentRatioFromAnimation() < m_tEffectData.fStartRatio)
		return 0;

	m_fDestroyTime += fTimeDelta;
	if(m_fDestroyTime > m_tEffectData.fLifeTime * 2)
		m_isDestroy = true;

	m_vWorldPos = *(_vec4*)&((*m_RHFrameMatrix *m_matAtkRange* *m_pPlayerWorld).m[3][0]);
	if(m_TrailList.size() < 40)
	{
		m_TrailList.push_back(m_vWorldPos - m_tEffectData.fEffectRadius*0.5f * *(_vec4*)&(*m_RHFrameMatrix * *m_pPlayerWorld).m[0][0]);
		m_TrailList.push_back(m_vWorldPos + m_tEffectData.fEffectRadius*0.5f * *(_vec4*)&(*m_RHFrameMatrix * *m_pPlayerWorld).m[0][0]);
	}
	else
	{
		m_TrailList.pop_front();
		m_TrailList.pop_front();
		m_TrailList.push_back(m_vWorldPos - m_tEffectData.fEffectRadius*0.5f * *(_vec4*)&(*m_RHFrameMatrix * *m_pPlayerWorld).m[0][0]);
		m_TrailList.push_back(m_vWorldPos + m_tEffectData.fEffectRadius*0.5f * *(_vec4*)&(*m_RHFrameMatrix * *m_pPlayerWorld).m[0][0]);
	}

	int iIndex = 0;
	for(list<D3DXVECTOR4>::iterator iter = m_TrailList.begin();
		iter != m_TrailList.end() ; ++iter)
	{
		m_vTrailVector[iIndex] = (*iter);
		++iIndex;
	}

	m_pShaderCom->GetEffect()->SetVectorArray("g_TrailVertex",m_vTrailVector,40);

	m_pShaderCom->GetEffect()->SetFloat("g_fProceed",3.f*m_fDestroyTime);
	

	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_ALPHA);

	return 0;
}

void CPhantom_Trail::Render_GameObject()
{
	m_pShaderCom->GetEffect()->SetMatrix("g_matWorld",m_pTransformCom->GetWorldMatrix());
	_matrix matView,matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION,&matProj);
	m_pShaderCom->GetEffect()->SetMatrix("g_matView",&matView);
	m_pShaderCom->GetEffect()->SetMatrix("g_matProj",&matProj);

	m_pShaderCom->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(*m_pPlayerAtkState));

	m_pShaderCom->GetEffect()->Begin(NULL,0);
	m_pShaderCom->GetEffect()->BeginPass(2);
	for(_uint i =0; i<5; ++i)
		m_pTrailBufferCom->Render_Buffer();
	m_pShaderCom->GetEffect()->EndPass();
	m_pShaderCom->GetEffect()->End();

	_matrix matScale, matTrans;
	D3DXMatrixTranslation(&matTrans,m_TrailList.back().x,m_TrailList.back().y,m_TrailList.back().z);
	D3DXMatrixScaling(&matScale,0.1f,0.1f,0.1f);
	m_pBoxColliderCom->Render(Engine::CBoxCollider::BOX_AABB,matScale * matTrans,m_isCol,false);
	
}

CPhantom_Trail* CPhantom_Trail::Create(LPDIRECT3DDEVICE9 pGraphicDev , Engine::CDynamicMesh* pPlayerDynamicMesh, EFFECTDATA tData)
{
	CPhantom_Trail* pInst = new CPhantom_Trail(pGraphicDev,pPlayerDynamicMesh);
	pInst->Ready_Trail(tData);
	return pInst;
}

void CPhantom_Trail::Free(void)
{
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTrailBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);
	Engine::Safe_Release(m_pBoxColliderCom);


	Engine::CGameObject::Free();
}

