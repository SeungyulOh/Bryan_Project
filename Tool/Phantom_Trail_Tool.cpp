#include "stdafx.h"
#include "Phantom_Trail_Tool.h"
#include "Export_Engine.h"
#include "resource.h"
#include "MainFrm.h"
#include "EffectBack.h"

CPhantom_Trail_Tool::CPhantom_Trail_Tool(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CDynamicMesh* pPlayerDynamicMesh)
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

CPhantom_Trail_Tool::~CPhantom_Trail_Tool(void)
{

}

void CPhantom_Trail_Tool::Ready_Trail(EFFECTDATA tData)
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(0,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(0,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pTrailBufferCom = (Engine::CTrail_Texture*)Engine::Clone(0,L"Component_Buffer_TrailTexture");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_TrailTexture",m_pTrailBufferCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(0,L"Component_Texture_PhantomTrail");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_PhantomTrail",m_pTextureCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(0,L"Component_Shader_Effect");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Effect",m_pShaderCom);

	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,Engine::_vec3(10.f,10.f,10.f));
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,Engine::_vec3(D3DX_PI/2.f,0.f,0.f));



	/*_matrix matScale,matLocalScale, matTrans,matRotX,matRotX2,matRotY,matRotZ;
	D3DXMatrixScaling(&matLocalScale,10.f,10.f,10.f);
	D3DXMatrixRotationX(&matRotX,D3DX_PI/2.f);
	D3DXMatrixRotationX(&matRotX2,D3DXToRadian(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fRotX));
	D3DXMatrixRotationY(&matRotY,D3DXToRadian(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fRotY));
	D3DXMatrixRotationZ(&matRotZ,D3DXToRadian(((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fRotZ));

	D3DXMatrixTranslation(&matTrans,((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fOffSetX,((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fOffSetY,((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fOffSetZ);
	_matrix matRot = matRotX * matRotX2 * matRotY * matRotZ;
	
	m_tEffectData.matLocal = matLocalScale * matRot * matTrans;
	m_tEffectData.fLifeTime = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fLifeTime;
	m_tEffectData.fStartRatio = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fStartRatio;
	m_tEffectData.fScale = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fScale;
	m_tEffectData.fEffectRadius = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fRadius;
	m_tEffectData.fSlope = ((CMainFrame*)AfxGetMainWnd())->m_pMyForm->m_EffectTool.m_fSlope;*/

	m_tEffectData = tData;


	Engine::CTransform* pPlayerTransform = ((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pEffectBack->m_pPlayerTool->m_pTransformCom;
	m_pPlayerWorld = pPlayerTransform->GetWorldMatrix();

	m_pTransformCom->SetWorldmat(m_tEffectData.matLocal * *m_pPlayerWorld);
	//m_pTransformCom->SetWorldmat(matLocalScale* matRotX * /**m_RHFrameMatrix*/ *matScale* matNewPlayer);

	ZeroMemory(m_vTrailVector,sizeof(D3DXVECTOR4)*40);

}

_int CPhantom_Trail_Tool::Update_GameObject(const _float& fTimeDelta)
{
	return 0;	
}

void CPhantom_Trail_Tool::Render_GameObject()
{
	if(m_pPlayerDynamicMesh->Get_CurrentRatioFromAnimation() < m_tEffectData.fStartRatio)
		return;

	if(m_isOnce == false)
	{
		m_isOnce = true;

		Engine::D3DXFRAME_DERIVED* pFrameFound = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(m_pPlayerDynamicMesh->GetRootFrame(),"ValveBiped_Anim_Attachment_RH");
		if(pFrameFound == NULL)
			return;
		m_pMatPlayerRH = &pFrameFound->CombinedMatrix;

		Engine::CTransform* pPlayerTransform = ((CMainFrame*)AfxGetMainWnd())->m_pToolView->m_pEffectBack->m_pPlayerTool->m_pTransformCom;
		m_pPlayerWorld = pPlayerTransform->GetWorldMatrix();
	
	}

	m_fDestroyTime += 0.01f;
	if(m_fDestroyTime > m_tEffectData.fLifeTime)
		m_isDestroy = true;

	_matrix matScale;
	D3DXMatrixScaling(&matScale,m_tEffectData.fAtkRange.x,m_tEffectData.fAtkRange.y,m_tEffectData.fAtkRange.z);
	D3DXVECTOR4 vWorldPos = *(_vec4*)&((m_tEffectData.matLocal * *m_pMatPlayerRH *matScale* *m_pPlayerWorld).m[3][0]);
	if(m_TrailList.size() < 40)
	{
		m_TrailList.push_back(vWorldPos - m_tEffectData.fEffectRadius * *(_vec4*)&(*m_pMatPlayerRH * *m_pPlayerWorld).m[0][0]);
		m_TrailList.push_back(vWorldPos + m_tEffectData.fEffectRadius * *(_vec4*)&(*m_pMatPlayerRH * *m_pPlayerWorld).m[0][0]);
	}
	else
	{
		m_TrailList.pop_front();
		m_TrailList.pop_front();
		m_TrailList.push_back(vWorldPos - m_tEffectData.fEffectRadius * *(_vec4*)&(*m_pMatPlayerRH * *m_pPlayerWorld).m[0][0]);
		m_TrailList.push_back(vWorldPos + m_tEffectData.fEffectRadius * *(_vec4*)&(*m_pMatPlayerRH * *m_pPlayerWorld).m[0][0]);
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
	m_pShaderCom->GetEffect()->SetFloat("g_fSlope",m_tEffectData.fSlope);
	m_pShaderCom->GetEffect()->SetFloat("g_fScale",m_tEffectData.fScale);
	m_pShaderCom->GetEffect()->SetFloat("g_fEffectRadius",m_tEffectData.fEffectRadius);



	m_pShaderCom->GetEffect()->SetMatrix("g_matWorld",m_pTransformCom->GetWorldMatrix());
	_matrix matView,matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION,&matProj);
	m_pShaderCom->GetEffect()->SetMatrix("g_matView",&matView);
	m_pShaderCom->GetEffect()->SetMatrix("g_matProj",&matProj);
	m_pShaderCom->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(0));

	m_pShaderCom->GetEffect()->Begin(NULL,0);

	m_pShaderCom->GetEffect()->BeginPass(2);
	m_pTrailBufferCom->Render_Buffer();
	m_pShaderCom->GetEffect()->EndPass();

	m_pShaderCom->GetEffect()->End();
}

CPhantom_Trail_Tool* CPhantom_Trail_Tool::Create(LPDIRECT3DDEVICE9 pGraphicDev , Engine::CDynamicMesh* pPlayerDynamicMesh, EFFECTDATA tData)
{
	CPhantom_Trail_Tool* pInst = new CPhantom_Trail_Tool(pGraphicDev,pPlayerDynamicMesh);
	pInst->Ready_Trail(tData);
	return pInst;
}

void CPhantom_Trail_Tool::Free(void)
{
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTrailBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);


	Engine::CGameObject::Free();
}

