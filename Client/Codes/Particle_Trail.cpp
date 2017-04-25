#include "stdafx.h"
#include "Particle_Trail.h"
#include "Export_Engine.h"

CParticle_Trail::CParticle_Trail(LPDIRECT3DDEVICE9 pGraphicDev, _matrix* pUpperMat,_vec3* pParentPos, _float fLifeTime)
:Engine::CGameObject(pGraphicDev)
,m_pTransformCom(NULL)
,m_pRendererCom(NULL)
,m_pTrailBufferCom(NULL)
,m_pTextureCom(NULL)
,m_pShaderCom(NULL)
,m_isOnce(false)
,m_fLifeTime(fLifeTime)
,m_pParentWorldPos(pParentPos)
,m_pUpperParentMat(pUpperMat)
{
	m_fDestroyTime = 0.f;
}

CParticle_Trail::~CParticle_Trail(void)
{

}

void CParticle_Trail::Ready_Trail()
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pTrailBufferCom = (Engine::CTrail_Texture*)Engine::Clone(COM_STATIC,L"Component_Buffer_TrailTexture");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_TrailTexture",m_pTrailBufferCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_Particle_trail");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_Particle_trail",m_pTextureCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Effect");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Effect",m_pShaderCom);


	ZeroMemory(m_vTrailVector,sizeof(D3DXVECTOR4) * 40);
	m_pShaderCom->GetEffect()->SetVectorArray("g_SecondTrailVertex",m_vTrailVector,40);
	m_fViewZ = 0.f;
}

_int CParticle_Trail::Update_GameObject(const _float& fTimeDelta)
{
	m_fDestroyTime += fTimeDelta;
	if(m_fDestroyTime > m_fLifeTime)
		m_isDestroy = true;

	//D3DXVec3TransformCoord(&_vec3(),m_pParentWorldPos,m_pUpperParentMat)
	D3DXVECTOR4 vWorldPos = D3DXVECTOR4(*D3DXVec3TransformCoord(&_vec3(),m_pParentWorldPos,m_pUpperParentMat),1.f);
	if(m_TrailList.size() < 40)
	{
		m_TrailList.push_back(vWorldPos -0.4f* *(_vec4*)&m_pUpperParentMat->m[0][0]);
		m_TrailList.push_back(vWorldPos +0.4f* *(_vec4*)&m_pUpperParentMat->m[0][0]);
	}
	else
	{
		m_TrailList.pop_front();
		m_TrailList.pop_front();
		m_TrailList.push_back(vWorldPos -0.4f* *(_vec4*)&m_pUpperParentMat->m[0][0]);
		m_TrailList.push_back(vWorldPos +0.4f* *(_vec4*)&m_pUpperParentMat->m[0][0]);
	}

	int iIndex = 0;
	for(list<D3DXVECTOR4>::iterator iter = m_TrailList.begin();
		iter != m_TrailList.end() ; ++iter)
	{
		m_vTrailVector[iIndex] = (*iter);
		++iIndex;
	}

	m_pShaderCom->GetEffect()->SetVectorArray("g_SecondTrailVertex",m_vTrailVector,40);

	m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_ALPHA);

	return 0;
}

void CParticle_Trail::Render_GameObject()
{
	m_pShaderCom->GetEffect()->SetMatrix("g_matWorld",m_pTransformCom->GetWorldMatrix());
	_matrix matView,matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION,&matProj);
	m_pShaderCom->GetEffect()->SetMatrix("g_matView",&matView);
	m_pShaderCom->GetEffect()->SetMatrix("g_matProj",&matProj);
	m_pShaderCom->GetEffect()->SetTexture("g_SecondTexture",m_pTextureCom->GetTexture(0));

	m_pShaderCom->GetEffect()->Begin(NULL,0);
	m_pShaderCom->GetEffect()->BeginPass(3);
	
	m_pTrailBufferCom->Render_Buffer();

	m_pShaderCom->GetEffect()->EndPass();
	m_pShaderCom->GetEffect()->End();
}

CParticle_Trail* CParticle_Trail::Create(LPDIRECT3DDEVICE9 pGraphicDev,_matrix* pUpperMat,_vec3* pParentPos, _float fLifeTime)
{
	CParticle_Trail* pInst = new CParticle_Trail(pGraphicDev,pUpperMat,pParentPos,fLifeTime);
	pInst->Ready_Trail();
	return pInst;
}

void CParticle_Trail::Free(void)
{
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTrailBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CGameObject::Free();
}

