#include "stdafx.h"
#include "CubeObj.h"
#include "Export_Engine.h"

CCubeObj* CCubeObj::m_pSelectedCube;
CCubeObj::CCubeObj(LPDIRECT3DDEVICE9 pGraphicDev)
:Engine::CGameObject(pGraphicDev)

{

}

CCubeObj::~CCubeObj(void)
{

}

HRESULT CCubeObj::Ready_GameObject(_vec3 vPos)
{
	m_vPos = vPos;

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(0,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pCubeTexCom = (Engine::CCube_Tex*)Engine::Clone(0,L"Component_Buffer_CubeTex");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_CubeTex",m_pCubeTexCom);

	m_pVertex = new Engine::VTXCUBETEX[8];
	m_pVertexForColor = new Engine::VTXCUBETEX[8];
	m_pIndex = new Engine::INDEX32[12];
	m_pCubeTexCom->GetIdxInfo(m_pIndex);
	m_pCubeTexCom->GetVtxInfo(m_pVertex);

	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&matWorld,m_vPos.x,m_vPos.y,m_vPos.z);
	for(int i=0;i<8;++i)
	{
		D3DXVec3TransformCoord(&m_pVertex[i].vPos,&m_pVertex[i].vPos,&matWorld);
	}

	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vPos);
	m_pTransformCom->Update_Component(0.f);

	return S_OK;
}


void CCubeObj::Render_GameObject(void)
{
	m_pGraphicDev->SetTexture(0,NULL);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	if(m_pSelectedCube == this)
	{
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
		m_pTransformCom->SetUp_OnGraphicDev();
		m_pCubeTexCom->Render_Buffer();
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
	}
	else
	{
		m_pTransformCom->SetUp_OnGraphicDev();
		m_pCubeTexCom->Render_Buffer();
	}

	
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

}

void CCubeObj::UpdateWorldMatrix()
{
	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,m_vPos);
	m_pTransformCom->Update_Component(0.f);
}

CCubeObj* CCubeObj::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CCubeObj* pInst = new CCubeObj(pGraphicDev);

	if(FAILED(pInst->Ready_GameObject(vPos)))
		return NULL;

	return pInst;
}

void CCubeObj::Free(void)
{
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pCubeTexCom);

	delete []m_pVertex;
	delete []m_pIndex;
	delete []m_pVertexForColor;

	Engine::CGameObject::Free();
}
