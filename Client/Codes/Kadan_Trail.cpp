#include "stdafx.h"
#include "Kadan_Trail.h"
#include "Export_Engine.h"
#include "BossDevil.h"

CKadan_Trail::CKadan_Trail(LPDIRECT3DDEVICE9 pGraphicDev, _matrix* pUpperMat)
:Engine::CGameObject(pGraphicDev)
,m_pTransformCom(NULL)
,m_pRendererCom(NULL)
,m_pTrailBufferCom(NULL)
,m_pTextureCom(NULL)
,m_pShaderCom(NULL)
,m_isOnce(false)
,m_pUpperParentMat(pUpperMat)
,m_iMaxTrailVtxCnt(40)
{
}

CKadan_Trail::~CKadan_Trail(void)
{

}

void CKadan_Trail::Ready_Trail()
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pTrailBufferCom = (Engine::CTrail_Texture*)Engine::Clone(COM_STATIC,L"Component_Buffer_TrailTexture");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_TrailTexture",m_pTrailBufferCom);

	m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_KadanTrail");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_KadanTrail",m_pTextureCom);

	m_pShaderCom = (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Effect");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Effect",m_pShaderCom);

	CBossDevil* pBoss = dynamic_cast<CBossDevil*>(Engine::GetList(L"Layer_Boss")->front());
	if(pBoss != NULL)
	{
		m_pBossVisible = pBoss->GetVisible();
	}


	ZeroMemory(m_vTrailVector,sizeof(D3DXVECTOR4) * m_iMaxTrailVtxCnt);
	m_pShaderCom->GetEffect()->SetVectorArray("g_ThirdTrailVertex",m_vTrailVector,m_iMaxTrailVtxCnt);
}

_int CKadan_Trail::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;
	if(fTimeDelta == 0.f)
	{
		m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_ALPHA);
		return 0;
	}
	if(m_fTimeDelta != 0)
	{
		if(m_TrailList.size() < m_iMaxTrailVtxCnt)
		{
			D3DXVECTOR4 vWorldPos = D3DXVECTOR4(*(_vec3*)&m_pUpperParentMat->m[3][0]  - *(_vec3*)&m_pUpperParentMat->m[2][0] * 100.f ,1.f);
			
			m_TrailList.push_back(vWorldPos -100.f* *(_vec4*)&m_pUpperParentMat->m[2][0]);
			m_TrailList.push_back(vWorldPos);
		}
		else
		{
			m_TrailList.pop_front();
			m_TrailList.pop_front();
			D3DXVECTOR4 vWorldPos = D3DXVECTOR4(*(_vec3*)&m_pUpperParentMat->m[3][0]  - *(_vec3*)&m_pUpperParentMat->m[2][0] * 100.f ,1.f);
			
			m_TrailList.push_back(vWorldPos -100.f* *(_vec4*)&m_pUpperParentMat->m[2][0]);
			m_TrailList.push_back(vWorldPos);
		}
	}

	//MakeBegieCurve();
	
	int iIndex = 0;
	for(list<D3DXVECTOR4>::iterator iter = m_TrailList.begin();
		iter != m_TrailList.end() ; ++iter)
	{
		m_vTrailVector[iIndex] = (*iter);
		++iIndex;
	}

	
	m_pShaderCom->GetEffect()->SetVectorArray("g_ThirdTrailVertex",m_vTrailVector,m_iMaxTrailVtxCnt);
	

	if(*m_pBossVisible == true)
		m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_ALPHA);

	return 0;
}

void CKadan_Trail::Render_GameObject()
{
	/*if(m_fTimeDelta != 0)
	{
		if(m_TrailList.size() < 40)
		{
			D3DXVECTOR4 vWorldPos = D3DXVECTOR4(*(_vec3*)&m_pUpperParentMat->m[3][0]  - *(_vec3*)&m_pUpperParentMat->m[2][0] * 100.f ,1.f);
			m_TrailList.push_back(vWorldPos -100.f* *(_vec4*)&m_pUpperParentMat->m[2][0]);
			m_TrailList.push_back(vWorldPos);
			
		}
		else
		{
			m_TrailList.pop_front();
			m_TrailList.pop_front();
			D3DXVECTOR4 vWorldPos = D3DXVECTOR4(*(_vec3*)&m_pUpperParentMat->m[3][0]  - *(_vec3*)&m_pUpperParentMat->m[2][0] * 100.f ,1.f);
			m_TrailList.push_back(vWorldPos -100.f* *(_vec4*)&m_pUpperParentMat->m[2][0]);
			m_TrailList.push_back(vWorldPos);
		}
	}
	MakeBegieCurve();*/

	m_pShaderCom->GetEffect()->SetMatrix("g_matWorld",m_pTransformCom->GetWorldMatrix());
	_matrix matView,matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION,&matProj);
	m_pShaderCom->GetEffect()->SetMatrix("g_matView",&matView);
	m_pShaderCom->GetEffect()->SetMatrix("g_matProj",&matProj);
	m_pShaderCom->GetEffect()->SetTexture("g_ThirdTexture",m_pTextureCom->GetTexture(9));
	m_pShaderCom->GetEffect()->SetInt("g_iVexMaxNum",m_iMaxTrailVtxCnt);

	m_pShaderCom->GetEffect()->Begin(NULL,0);
	m_pShaderCom->GetEffect()->BeginPass(4);
	
	m_pTrailBufferCom->Render_Buffer();

	m_pShaderCom->GetEffect()->EndPass();
	m_pShaderCom->GetEffect()->End();

	return;

	_vec3 vCenterPos = *(_vec3*)&m_pUpperParentMat->m[3][0];// - *(_vec3*)&m_pUpperParentMat->m[2][0] * 100.f;
	_vec3 vRight =vCenterPos +*(_vec3*)&m_pUpperParentMat->m[0][0] * 50.f;
	_vec3 vUp = vCenterPos+*(_vec3*)&m_pUpperParentMat->m[1][0] * 50.f;
	_vec3 vLook = vCenterPos+*(_vec3*)&m_pUpperParentMat->m[2][0] * 50.f;
	_vec3 vLine[2] = {vCenterPos,vRight};
	_vec3 vLine1[2] = {vCenterPos,vUp};
	_vec3 vLine2[2] = {vCenterPos,vLook};
	/*_vec3 vZero[2] = {*(_vec3*)&m_vTrailVector[0],*(_vec3*)&m_vTrailVector[0] + _vec3(0.f,1.f,0.f)};
	_vec3 vOne[2] = {*(_vec3*)&m_vTrailVector[1],*(_vec3*)&m_vTrailVector[1] + _vec3(0.f,2.f,0.f)};
	_vec3 vTwenty[2] = {*(_vec3*)&m_vTrailVector[20],*(_vec3*)&m_vTrailVector[20] + _vec3(0.f,3.f,0.f)};
	_vec3 vTwentyOne[2] = {*(_vec3*)&m_vTrailVector[21],*(_vec3*)&m_vTrailVector[21] + _vec3(0.f,4.f,0.f)};
	_vec3 vLastLine[2] = {*(_vec3*)&m_vTrailVector[38],*(_vec3*)&m_vTrailVector[39]};
	_vec3 vExpectedLastLine[2] = {vCenterPos,vCenterPos -100.f* *(_vec3*)&m_pUpperParentMat->m[2][0]};*/

	/*list<D3DXVECTOR4>::iterator iter = m_TrailList.end();
	_vec3 vLineTemp[2] = {*(_vec3*)&(*--iter),*(_vec3*)&(*--iter)};*/

	_matrix matFinal;
	matFinal = matView * matProj;

	Engine::GetLine()->Begin();
	Engine::GetLine()->DrawTransform(vLine,2,&matFinal,D3DXCOLOR(1.f,0.f,0.f,1.f));
	Engine::GetLine()->DrawTransform(vLine1,2,&matFinal,D3DXCOLOR(0.f,0.f,0.f,1.f));
	Engine::GetLine()->DrawTransform(vLine2,2,&matFinal,D3DXCOLOR(0.f,0.f,1.f,1.f));
	//Engine::GetLine()->DrawTransform(vZero,2,&matFinal,D3DXCOLOR(1.f,1.f,1.f,1.f));
	//Engine::GetLine()->DrawTransform(vOne,2,&matFinal,D3DXCOLOR(1.0f,1.f,1.f,1.f));
	//Engine::GetLine()->DrawTransform(vTwenty,2,&matFinal,D3DXCOLOR(1.0f,1.f,1.f,1.f));
	//Engine::GetLine()->DrawTransform(vTwentyOne,2,&matFinal,D3DXCOLOR(1.0f,1.f,1.f,1.f));
	////Engine::GetLine()->DrawTransform(vLastLine,2,&matFinal,D3DXCOLOR(1.f,0.f,1.f,1.f));
	//Engine::GetLine()->DrawTransform(vLineTemp,2,&matFinal,D3DXCOLOR(0.f,1.f,1.f,1.f));
	//Engine::GetLine()->DrawTransform(vExpectedLastLine,2,&matFinal,D3DXCOLOR(0.7f,0.7f,0.7f,1.f));
	Engine::GetLine()->End();

	/*TCHAR szTemp[256] = L"%d";
	for(_uint i=0;i<40;++i)
	{
		_vec3 vProjPos;
		D3DXVec3TransformCoord(&vProjPos,(_vec3*)&m_vTrailVector[i],&matFinal);
		_vec2 vViewPortPos;
		vViewPortPos.x = (vProjPos.x + 1) * 400.f;
		vViewPortPos.y = (vProjPos.y - 1) * -300.f; 
		wsprintf(szTemp,L"%d",i);
		Engine::Render_Font(L"Font_Batang",szTemp,&vViewPortPos,D3DXCOLOR(1.f,1.f,1.f,1.f));
	}*/
	
}

CKadan_Trail* CKadan_Trail::Create(LPDIRECT3DDEVICE9 pGraphicDev,_matrix* pUpperMat)
{
	CKadan_Trail* pInst = new CKadan_Trail(pGraphicDev,pUpperMat);
	pInst->Ready_Trail();
	return pInst;
}

void CKadan_Trail::Free(void)
{
	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pTrailBufferCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pShaderCom);

	Engine::CGameObject::Free();
}

_vec3 CKadan_Trail::GetNewCurvePoint(_vec3 vSrc,_vec3 vDest,_float fRatio)
{
	_vec3 vNewDir = vDest - vSrc;
	_float fDist = D3DXVec3Length(&vNewDir);
	D3DXVec3Normalize(&vNewDir,&vNewDir);
	return vSrc + vNewDir * fDist * fRatio;
}

void CKadan_Trail::MakeBegieCurve(void)
{
	_uint iTotalPath = 1;
	_uint iSamplingCnt = 20;
	if(m_TrailList.size() < iSamplingCnt * iTotalPath)
		return;
	for(_uint k=0; k < iTotalPath ; ++ k)
	{
		_vec3 vLeftSrcPoint[10];
		_vec3 vRightSrcPoint[10];
		_uint iTrailsize = m_TrailList.size();
		list<D3DXVECTOR4>::iterator iter = m_TrailList.end();
		for(_uint i=0; i<k*(iSamplingCnt-2) ; ++i)
		{
			--iter;
		}
		--iter;
		
		for( _uint i = 0; i < iSamplingCnt ; ++i, --iter)
		{
			if(i % 2 == 0)
				vRightSrcPoint[i/2] = *(_vec3*)&(*iter);
			else
				vLeftSrcPoint[i/2] = *(_vec3*)&(*iter);

			if(i == iSamplingCnt-1)
				break;
		}
	
		//three point pair cnt
		_uint iOffSet = (iSamplingCnt - 6) / 2 + 1;
		for(_uint j=0; j<iOffSet ; ++j)
		{
			for(_uint i=0; i<40/iOffSet; i = i+2)
			{
				_uint iIndex = (iOffSet-1) * (40/iOffSet);
				m_vTrailVector[iIndex + (i)] = D3DXVECTOR4(GetFinalPointFromThreePoints(&vLeftSrcPoint[j],(1.f / (40/(iOffSet*2))) * (i/2)),1.f);
				m_vTrailVector[iIndex + (i+1)] = D3DXVECTOR4(GetFinalPointFromThreePoints(&vRightSrcPoint[j],(1.f / (40/(iOffSet*2))) * (i/2)),1.f);
			}
		}
	}
}

_vec3 CKadan_Trail::GetFinalPointFromThreePoints(_vec3* pSrc, _float fRatio)
{
	_vec3 vLeftFirstPerOnce = GetNewCurvePoint(pSrc[0],pSrc[1],fRatio);
	_vec3 vLeftSecondPerOnce = GetNewCurvePoint(pSrc[1],pSrc[2],fRatio);
	//_vec3 vLeftThirdPerOnce = GetNewCurvePoint(pSrc[2],pSrc[3],fRatio);

	_vec3 vLeftFirstPerTwice = GetNewCurvePoint(vLeftFirstPerOnce, vLeftSecondPerOnce,fRatio);
	//_vec3 vLeftSecondPerTwice = GetNewCurvePoint(vLeftSecondPerOnce,vLeftThirdPerOnce,fRatio);

	//_vec3 vLeftFinalPoint = GetNewCurvePoint(vLeftFirstPerTwice,vLeftSecondPerTwice,fRatio);
	return vLeftFirstPerTwice;
}

