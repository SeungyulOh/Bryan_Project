#include "stdafx.h"
#include "ExpEffect.h"
#include "Export_Engine.h"
#include "ChildEffect.h"
#include "Distortion.h"
#include "Distortion_Mgr.h"
#include "Default_Camera.h"
#include "SoundMgr.h"

const D3DVERTEXELEMENT9 g_VBDecl_Geometry[] =
{
	{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	{1, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
	{1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
	{1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3},
	{1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4},
	{1, 64, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5},
	D3DDECL_END()
};

//const D3DVERTEXELEMENT9 g_VBDecl_InstanceData[] =
//{
//	{1, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
//	{1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
//	{1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3},
//	{1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4},
//	D3DDECL_END()
//};


CExpEffect::CExpEffect(LPDIRECT3DDEVICE9 pGraphicDev, CChildEffect::EFFECTNAME eChildEffName,EFFECTNAME eEffectName, _matrix* pParentMat, _matrix pOffSetRotMat)
:Engine::CGameObject(pGraphicDev)
,m_fTimeAcc(0.f)
,m_fEmitTimeAcc(0.f)
,m_eEffectName(eEffectName)
,m_pParent(pParentMat)
,m_eEffectNameForChild(eChildEffName)
,m_OffSetRotMat(pOffSetRotMat)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
,m_pTextureCom(NULL)
,m_pBufferCom(NULL)
,m_pInstancingShader(NULL)
,m_isVisible(true)
,m_isOnce(false)
{

}

CExpEffect::~CExpEffect(void)
{

}

HRESULT CExpEffect::Ready_GameObject()
{
	m_pRendererCom = (Engine::CRenderer*)Engine::Clone(COM_STATIC,L"Component_Renderer");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Renderer",m_pRendererCom);

	m_pShaderCom= (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Default");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Default",m_pShaderCom);

	m_pInstancingShader= (Engine::CShader*)Engine::Clone(COM_STATIC,L"Component_Shader_Instancing");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Shader_Instancing",m_pInstancingShader);

	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone(COM_STATIC,L"Component_Buffer_RectTexture");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_RectTexture",m_pBufferCom);

	if(m_eEffectNameForChild == CChildEffect::SWING04DUST)
	{
		m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_KadanSwing04Dust");
		Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_KadanSwing04Dust",m_pTextureCom);
	}
	else if(m_eEffectNameForChild == CChildEffect::TELEPORT)
	{
		m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_KadanTeleport");
		Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_KadanTeleport",m_pTextureCom);
	}
	else if(m_eEffectNameForChild == CChildEffect::KADANWEAPON)
	{
		m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_KadanWeapon");
		Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_KadanWeapon",m_pTextureCom);
	}
	else if(m_eEffectNameForChild == CChildEffect::SPRINTRUN)
	{
		m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_sprintrun");
		Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_sprintrun",m_pTextureCom);
	}
	else if(m_eEffectNameForChild == CChildEffect::ATKLEFT)
	{
		m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_AtkLeft");
		Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_AtkLeft",m_pTextureCom);
	}
	else if(m_eEffectNameForChild == CChildEffect::BOLT)
	{
		m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_bolt");
		Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_bolt",m_pTextureCom);
	}
	else if(m_eEffectNameForChild == CChildEffect::FIREBALL)
	{
		m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_fireball");
		Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_fireball",m_pTextureCom);	
	}
	else if(m_eEffectNameForChild == CChildEffect::WATERFALL)
	{
		m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_WaterFall");
		Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_WaterFall",m_pTextureCom);	
	}
	else if(m_eEffectNameForChild == CChildEffect::WATERLINE)
	{
		m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_WaterLine");
		Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_WaterLine",m_pTextureCom);	
	}
	else if(m_eEffectNameForChild == CChildEffect::EXPLOSION)
	{
		m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_Explosion");
		Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_Explosion",m_pTextureCom);	
	}
	else if(m_eEffectNameForChild == CChildEffect::WAVEREADY)
	{

		m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_WaveReady");
		Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_WaveReady",m_pTextureCom);
	}
	else if(m_eEffectNameForChild == CChildEffect::WAVEHIT)
	{
		m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_WaveHit");
		Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_WaveHit",m_pTextureCom);
	}
	else if(m_eEffectNameForChild == CChildEffect::KADANEXPLOSION)
	{
		m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_KadanExplosion");
		Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_KadanExplosion",m_pTextureCom);
	}
	else if(m_eEffectNameForChild == CChildEffect::EP10)
	{
		m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_Ep10");
		Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_Ep10",m_pTextureCom);
	}
	else if(m_eEffectNameForChild == CChildEffect::EP10_HEART)
	{
		m_pTextureCom = (Engine::CTexture*)Engine::Clone(COM_STAGE,L"Component_Texture_Ep10_Heart");
		Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Texture_Ep10_Heart",m_pTextureCom);
	}

	FillEfxData();

	m_fDieTime = min(m_tEfxData.vDieTime.x,m_tEfxData.vDieTime.y) + fabs(m_tEfxData.vDieTime.y - m_tEfxData.vDieTime.x) * (rand()%11 * 0.1f);
	m_iParticleCnt = 0;

	m_iMaxCnt = 800;
	m_iSizeGeom = D3DXGetDeclVertexSize(g_VBDecl_Geometry,0);
	m_iSizeInstance = sizeof(Engine::INSTANCEDATA);
	m_pGraphicDev->CreateVertexDeclaration(g_VBDecl_Geometry,&m_pVtxDecl);
	m_pGraphicDev->CreateVertexBuffer(m_iSizeInstance * m_iMaxCnt,0 ,NULL,D3DPOOL_MANAGED,&m_pInstanceBuffer,NULL);

	return S_OK;
}

_int CExpEffect::Update_GameObject(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	m_fEmitTimeAcc += fTimeDelta;
	if(m_fTimeAcc > m_fDieTime && m_tEfxData.isLoop == false)
		m_isDestroy = true;

	if(m_isVisible == true)
		m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_ALPHA);
	
	if(m_tEfxData.fEmitDuration != 0)
	{
		if(m_fTimeAcc < m_tEfxData.fEmitDuration)
		{
			if(m_fEmitTimeAcc > m_tEfxData.fEmitInterval)
			{
				m_fEmitTimeAcc = 0.f;
				for(_int i=0; i<m_tEfxData.iParticleCount; ++i)
				{
					if(m_tEfxData.isDecrease == true)
						m_tEfxData.vStartSize.x = -20.f * m_fTimeAcc + 20.f;
					
					/*if(m_tEfxData.isDistortion == true)
					{
						_float fOffSetX = m_tEfxData.vPositionOffSet.x * _float(rand()%11) * 0.1f - m_tEfxData.vPositionOffSet.x * 0.5f;
						_float fOffSetY = m_tEfxData.vPositionOffSet.y * _float(rand()%11) * 0.1f - m_tEfxData.vPositionOffSet.y * 0.5f;
						_float fOffSetZ = m_tEfxData.vPositionOffSet.z * _float(rand()%11) * 0.1f - m_tEfxData.vPositionOffSet.z * 0.5f;

						_vec3 vNewPos = m_vFistPosForDistorion + _vec3(fOffSetX,fOffSetY,fOffSetZ);
						Engine::CGameObject* pGameObject = CDistortion::Create(m_pGraphicDev,_vec3(1.f,1.f,1.f),_vec3(0.f,0.f,0.f),vNewPos,20.f,0.05f,0,true,0.1f,m_pTextureCom->GetTexture(m_tEfxData.iImgIdx));
						Engine::GetLayer(L"Layer_Distortion")->Add_Object(pGameObject);
					}
					else
					{*/
						Engine::CGameObject* pGameObject = CChildEffect::Create(m_pGraphicDev,m_eEffectNameForChild, m_tEfxData,m_fDieTime,m_pParent,m_OffSetRotMat);
						Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
						m_ChildEffectList.push_back(pGameObject);
					//}
				}
			}
		}
		return 0;
	}

	if(m_fTimeAcc > m_tEfxData.vParticleCount[0].x * m_fDieTime && m_iParticleCnt == 0)
	{
		++m_iParticleCnt;
		_int iCreateCnt = _int(min(m_tEfxData.vParticleCount[0].y,m_tEfxData.vParticleCount[0].z) + fabs(m_tEfxData.vParticleCount[0].y - m_tEfxData.vParticleCount[0].z) * (rand()%11 * 0.1f));
		for(_int i=0; i<iCreateCnt; ++i)
		{
			Engine::CGameObject* pGameObject = CChildEffect::Create(m_pGraphicDev, m_eEffectNameForChild,m_tEfxData,m_fDieTime,m_pParent,m_OffSetRotMat);
			Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
			m_ChildEffectList.push_back(pGameObject);
		}
	}
	else if(m_fTimeAcc > m_tEfxData.vParticleCount[1].x * m_fDieTime && m_iParticleCnt == 1)
	{
		++m_iParticleCnt;
		_int iCreateCnt = _int(min(m_tEfxData.vParticleCount[1].y,m_tEfxData.vParticleCount[1].z) + fabs(m_tEfxData.vParticleCount[1].y - m_tEfxData.vParticleCount[1].z) * (rand()%11 * 0.1f));
		for(_int i=0; i<iCreateCnt; ++i)
		{
			Engine::CGameObject* pGameObject = CChildEffect::Create(m_pGraphicDev,m_eEffectNameForChild, m_tEfxData,m_fDieTime,m_pParent,m_OffSetRotMat);
			Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
			m_ChildEffectList.push_back(pGameObject);
		}
	}
	

	if(m_isVisible == true)
		m_pRendererCom->Add_RenderObject(this,Engine::CRenderer::RT_ALPHA);

	return 0;
}

_int CExpEffect::LastUpdate_GameObject(const _float& fTimeDelta)
{
	
	if(m_eEffectName == WAVEREADY0 && m_isOnce == false && m_fTimeAcc > m_fDieTime * 0.8f)
	{
		m_isOnce = true;
		Engine::CGameObject* pGameObject = NULL;
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT8,m_pParent);
		//((CExpEffect*)pGameObject)->SetLookDist(this->m_tEfxData.fLookDist);
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(this->m_tEfxData.vWorldOffSetPos);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::WAVEHIT,CExpEffect::WAVEHIT9,m_pParent);
		//((CExpEffect*)pGameObject)->SetLookDist(this->m_tEfxData.fLookDist);
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(this->m_tEfxData.vWorldOffSetPos);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		((CDefault_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(0.2f);

		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SWING04DUST,CExpEffect::SWING4DUST10, m_pParent);
		//((CExpEffect*)pGameObject)->SetLookDist(this->m_tEfxData.fLookDist);
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(this->m_tEfxData.vWorldOffSetPos);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SWING04DUST,CExpEffect::SWING4DUST9, m_pParent);
		//((CExpEffect*)pGameObject)->SetLookDist(this->m_tEfxData.fLookDist);
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(this->m_tEfxData.vWorldOffSetPos);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SWING04DUST,CExpEffect::SWING4DUST8, m_pParent);
		//((CExpEffect*)pGameObject)->SetLookDist(this->m_tEfxData.fLookDist);
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(this->m_tEfxData.vWorldOffSetPos);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);
		pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SWING04DUST,CExpEffect::SWING4DUST7, m_pParent);
		//((CExpEffect*)pGameObject)->SetLookDist(this->m_tEfxData.fLookDist);
		((CExpEffect*)pGameObject)->SetOffSetWorldPos(this->m_tEfxData.vWorldOffSetPos);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

		CSoundMgr::GetInstance()->PlaySound(L"explosion_lv3.wav",CSoundMgr::BUFF);
	}

	return 0;
}

void CExpEffect::Render_GameObject()
{
	if(m_ChildEffectList.size() == 0)
		return;

	list<Engine::CGameObject*>::iterator iter = m_ChildEffectList.begin();
	list<Engine::CGameObject*>::iterator iter_end = m_ChildEffectList.end();

	//For Instancing//////////////

	Engine::INSTANCEDATA tInstanceData[500];
	_int iIndex = 0;
	for( ; iter != iter_end ; ++iter, ++iIndex)
	{
		if(((CChildEffect*)(*iter))->GetIsLoopEnable() == false)
			continue;

		tInstanceData[iIndex].vColor = ((CChildEffect*)(*iter))->GetColor();
		memcpy(&tInstanceData[iIndex],&((CChildEffect*)(*iter))->GetWorldMatrix(),sizeof(_matrix));	
	}

	Engine::INSTANCEDATA* pData = NULL;
	m_pInstanceBuffer->Lock(0,0,(void**)&pData,0);
	memcpy(pData,tInstanceData,sizeof(Engine::INSTANCEDATA)*m_ChildEffectList.size());
	m_pInstanceBuffer->Unlock();

	m_pGraphicDev->SetVertexDeclaration(m_pVtxDecl);
	m_pGraphicDev->SetIndices(m_pBufferCom->GetIndexBuffer());

	m_pGraphicDev->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | m_ChildEffectList.size()));
	m_pGraphicDev->SetStreamSource(0,m_pBufferCom->GetVertexBuffer(),0,sizeof(Engine::VTXTEX));
	m_pGraphicDev->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1ul));
	m_pGraphicDev->SetStreamSource(1, m_pInstanceBuffer,0,m_iSizeInstance);

	m_pInstancingShader->SetMatrixOnShader(*D3DXMatrixIdentity(&_matrix()));
	m_pInstancingShader->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(m_tEfxData.iImgIdx));

	_int iPass = 0;
	if(m_tEfxData.isDestAlphaOne == true)
		iPass = 1;

	if(m_tEfxData.vUV.x == 1.f)
	{
		if(m_tEfxData.vUV.y == 0)
			m_pInstancingShader->GetEffect()->SetFloat("g_fTimeAcc",m_fTimeAcc);
		else
			m_pInstancingShader->GetEffect()->SetFloat("g_fTimeAcc",-m_fTimeAcc);
	}
	else
		m_pInstancingShader->GetEffect()->SetFloat("g_fTimeAcc",0.f);


	m_pInstancingShader->GetEffect()->Begin(NULL,0);
	m_pInstancingShader->GetEffect()->BeginPass(iPass);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);

	m_pInstancingShader->GetEffect()->EndPass();
	m_pInstancingShader->GetEffect()->End();

	m_pGraphicDev->SetStreamSourceFreq(0,1);
	m_pGraphicDev->SetStreamSourceFreq(1,1);
	
	return;

	//m_pGraphicDev->CreateVertexBuffer()

	//m_pGraphicDev->SetStreamSourceFreq(0,1);
	//m_pGraphicDev->SetStreamSource(0,m_pBufferCom,0,D3DXGetDeclVertexSize(g_VBDecl_Geometry,0));




	//////////////////// Normal Version /////////////////////////
	m_pShaderCom->SetMatrixOnShader(*D3DXMatrixIdentity(&_matrix()));
	m_pShaderCom->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(m_tEfxData.iImgIdx));

	m_pShaderCom->GetEffect()->Begin(NULL,0);
	m_pShaderCom->GetEffect()->BeginPass(5);
	for( ; iter != iter_end ; ++iter)
	{
		((CChildEffect*)(*iter))->Render_GameObject(m_pShaderCom->GetEffect());
	}
	m_pShaderCom->GetEffect()->EndPass();
	m_pShaderCom->GetEffect()->End();
}

CExpEffect* CExpEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, CChildEffect::EFFECTNAME eChildEffName,EFFECTNAME eEffectName, _matrix* pParentMat, _matrix pOffSetRotMat)
{
	CExpEffect* pInstance = new CExpEffect(pGraphicDev,eChildEffName, eEffectName, pParentMat, pOffSetRotMat);
	if(FAILED(pInstance->Ready_GameObject()))
	{
		Engine::Safe_Release(pInstance);
		return NULL;
	}
	return pInstance;
}

void CExpEffect::Free(void)
{
	if(m_eEffectName == FIREBALL1)
	{
		Engine::CGameObject* pDistortionMgr = CDistortion_Mgr::Create(m_pGraphicDev,0.4f);
		CDistortion* pDistortion = CDistortion::Create(m_pGraphicDev,_vec3(10.f,10.f,10.f),_vec3(D3DX_PI/2.f,0.f,0.f),*(_vec3*)&m_pParent->m[3][0] + _vec3(0.f,5.f,0.f),150.f,0.05f,1,false);
		((CDistortion_Mgr*)pDistortionMgr)->AddDistortion(pDistortion);
		Engine::GetLayer(L"Layer_Distortion")->Add_Object(pDistortionMgr);

		Engine::CGameObject* pGameObject = CExpEffect::Create(m_pGraphicDev, CChildEffect::SPRINTRUN,CExpEffect::SPRINTRUN7,m_pParent);
		Engine::GetLayer(L"Layer_Effect")->Add_Object(pGameObject);

		Engine::CGameObject* pPlayer = Engine::GetList(L"Layer_Player")->front();
		pPlayer->SetisCol(true);
		((CDefault_Camera*)Engine::GetList(L"Layer_Cam")->front())->ShakeCam(1.f);
	}
	Engine::CGameObject::Free();
	Engine::Safe_Release(m_pRendererCom);
	Engine::Safe_Release(m_pShaderCom);
	Engine::Safe_Release(m_pTextureCom);
	Engine::Safe_Release(m_pBufferCom);
	Engine::Safe_Release(m_pInstancingShader);
	Engine::Safe_Release(m_pVtxDecl);
	Engine::Safe_Release(m_pInstanceBuffer);

	list<Engine::CGameObject*>::iterator iter = m_ChildEffectList.begin();
	list<Engine::CGameObject*>::iterator iter_end = m_ChildEffectList.end();

	for( ; iter!=iter_end ; ++iter)
	{
		(*iter)->SetDestroy(true);
	}


}

void CExpEffect::FillEfxData(void)
{
	ZeroMemory(&m_tEfxData,sizeof(EFXDATA));

	switch(m_eEffectName)
	{
	case KADANTELE0:
		m_tEfxData.iImgIdx = m_eEffectName;
		m_tEfxData.fEmitInterval = 0.02f;
		m_tEfxData.fEmitDuration = 0.1f;
		m_tEfxData.iParticleCount = 14;
		m_tEfxData.vColorMin = _vec3(0.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.3f,0.4f);
		m_tEfxData.vSpeed = _vec2(210.f,180.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.f,20.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,1.f,0.f);
		m_tEfxData.vSpreadAngle = _vec2(-45.f,45.f);
		m_tEfxData.vSpreadRadius = _vec2(10.f,35.f); 
		m_tEfxData.vSpreadRadiusAngle = _vec2(360.f,0.f);
		m_tEfxData.vStartSize = _vec2(3.5f,5.5f); 
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,0.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,-2.f,0.f);
		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,10.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.5f,250.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.f,0.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.f,0.f,0.001f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f,160.f,0.f);
		m_tEfxData.vMyOffSetPos = _vec3(0.f,5.f,0.f);
		m_tEfxData.isBillBoard = true;
		break;

	case KADANTELE1:
		m_tEfxData.iImgIdx = m_eEffectName;
		m_tEfxData.vParticleCount[0] = _vec3(0.1f, 7.f,10.f);
		m_tEfxData.vParticleCount[1] = _vec3(0.5f, 10.f,7.f);
		m_tEfxData.vColorMin = _vec3(0.f,0.f,0.f);
		m_tEfxData.vColorMax = _vec3(0.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.5f,0.7f);
		m_tEfxData.vSpeed = _vec2(100.f,0.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.f,180.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,100.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);
		m_tEfxData.vSpreadAngle = _vec2(70.f,-70.f);
		m_tEfxData.vSpreadRadius = _vec2(20.f,0.f); 
		m_tEfxData.vSpreadRadiusAngle = _vec2(30.f,0.f);
		//m_tEfxData.vStartSize = _vec2(3.f,6.f); 
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,5.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,10.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,12.f,0.f);
		m_tEfxData.vAlphaDelta[0] = _vec3(0.3f,200.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.7f,100.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.f,0.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-60.f,60.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.f,0.f,0.001f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f,100.f,0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.f,150.f,0.f);
		m_tEfxData.vMyOffSetPos = _vec3(0.f,5.f,0.f);
		m_tEfxData.isBillBoard = true;
		break;

	case KADANTELE2:
		m_tEfxData.iImgIdx = m_eEffectName;
		m_tEfxData.vPositionOffSet = _vec3(0.f,0.f,1.f);
		m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		m_tEfxData.fEmitInterval = 0.01f;
		m_tEfxData.fEmitDuration = 0.15f;
		m_tEfxData.iParticleCount = 30;
		m_tEfxData.vColorMin = _vec3(0.f,0.f,0.f);
		m_tEfxData.vColorMax = _vec3(39.f,35.f,27.f);
		m_tEfxData.vDieTime = _vec2(0.5f,1.f);
		m_tEfxData.vSpeed = _vec2(0.f,120.f);
	
	
		m_tEfxData.vSpreadAngle = _vec2(100.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(1.f,20.f); 
		m_tEfxData.vSpreadRadiusAngle = _vec2(360.f,0.f);
		m_tEfxData.vStartSize = _vec2(3.f,6.f); 
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,5.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,10.f,0.f);
		m_tEfxData.vAlphaDelta[0] = _vec3(0.3f,250.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.7f,200.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.f,0.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.f,0.f,0.001f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f,80.f,0.f);
		m_tEfxData.vMyOffSetPos = _vec3(0.f,1.f,0.f);
		break;

	case KADANTELE3:
		m_tEfxData.iImgIdx = m_eEffectName;
		m_tEfxData.fEmitInterval = 0.05f;
		m_tEfxData.fEmitDuration = 0.1f;
		m_tEfxData.iParticleCount = 10;
		m_tEfxData.vColorMin = _vec3(0.f,0.f,0.f);
		m_tEfxData.vColorMax = _vec3(0.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(1.f,2.f);
		m_tEfxData.vSpeed = _vec2(50.f,70.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.f,10.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(160.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(20.f,0.f); 
		m_tEfxData.vSpreadRadiusAngle = _vec2(0.f,0.f);
		m_tEfxData.vStartSize = _vec2(3.f,10.f); 
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,5.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,8.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,12.f,0.f);
		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,200.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.6f,150.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.f,1.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-60.f,60.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.f,0.f,0.001f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f,50.f,0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.f,70.f,0.f);
		m_tEfxData.vMyOffSetPos = _vec3(0.f,5.f,0.f);
		m_tEfxData.isBillBoard = true;
		break;

	case KADANTELE4:
		m_tEfxData.iImgIdx = m_eEffectName;
		m_tEfxData.fEmitInterval = 0.01f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 3;
		m_tEfxData.vColorMin = _vec3(0.f,0.f,0.f);
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(0.3f,0.5f);
		m_tEfxData.vSpeed = _vec2(50.f,70.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.f,10.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		/*m_tEfxData.vSpreadAngle = _vec2(160.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(20.f,0.f); 
		m_tEfxData.vSpreadRadiusAngle = _vec2(0.f,0.f);
		m_tEfxData.vStartSize = _vec2(3.f,10.f); */
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,3.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,8.f,0.f);
		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,50.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.2f,250.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(0.7f,80.f,0.f);
		m_tEfxData.vAlphaDelta[3] = _vec3(1.f,10.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-45.f,45.f);
	/*	m_tEfxData.vRollDelta[0] = _vec3(0.f,0.f,0.001f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f,50.f,0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.f,70.f,0.f);*/
		m_tEfxData.vSpreadCylinderRadius[0] = _vec2(0.f,40.f);
		m_tEfxData.vSpreadCylinderRadius[1] = _vec2(0.3f,30.f);
		m_tEfxData.vSpreadCylinderRadius[2] = _vec2(0.6f,10.f);
		m_tEfxData.vSpreadCylinderRadius[3] = _vec2(0.8f,10.f);
		m_tEfxData.vSpreadCylinderRadius[4] = _vec2(1.f,35.f);
		m_tEfxData.vSpreadCylinderHeight[0] = _vec2(0.f,5.f);
		m_tEfxData.vSpreadCylinderHeight[1] = _vec2(0.5f,30.f);
		m_tEfxData.vSpreadCylinderHeight[2] = _vec2(1.f,80.f);
		m_tEfxData.vMyOffSetPos = _vec3(0.f,5.f,0.f);
		m_tEfxData.isBillBoard = true;
		break;

	case KADANTELE5:
		m_tEfxData.isDistortion = true;
		m_tEfxData.iImgIdx = m_eEffectName;
		m_tEfxData.fEmitInterval = 0.02f;
		m_tEfxData.fEmitDuration = 1.0f;
		m_tEfxData.iParticleCount = 1;
		m_tEfxData.vColorMin = _vec3(0.f,0.f,0.f);
		m_tEfxData.vColorMax = _vec3(0.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.2f,0.4f);
		m_tEfxData.vSpeed = _vec2(1.f,0.f);
		//m_tEfxData.vSpreadAngle = _vec2(160.f,0.f);
		//m_tEfxData.vSpreadRadius = _vec2(20.f,0.f); 
		//m_tEfxData.vSpreadRadiusAngle = _vec2(0.f,0.f);
		m_tEfxData.vStartSize = _vec2(6.f,8.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,1.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.2f,50.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(0.7f,-20.f,0.f);
		m_tEfxData.vAlphaDelta[0] = _vec3(0.2f,250.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.6f,200.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.f,0.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
	/*	m_tEfxData.vRollDelta[0] = _vec3(0.f,0.f,0.001f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f,50.f,0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.f,70.f,0.f);*/
		m_tEfxData.vMyOffSetPos = _vec3(0.f,5.f,0.f);
		m_tEfxData.isBillBoard = true;
		break;
	case KADANTELE6:
		m_tEfxData.iImgIdx = m_eEffectName;
		//m_tEfxData.vPositionOffSet = _vec3(0.f,0.f,45.f);
		m_tEfxData.fEmitInterval = 0.01f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 1;
		m_tEfxData.vColorMin = _vec3(0.f,0.f,0.f);
		m_tEfxData.vColorMax = _vec3(0.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.4f,0.8f);
		m_tEfxData.vSpeed = _vec2(15.f,35.f);

		m_tEfxData.vSpreadAngle = _vec2(60.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(1.f,15.f); 
		m_tEfxData.vSpreadRadiusAngle = _vec2(0.f,0.f);
		m_tEfxData.vStartSize = _vec2(12.f,6.f); 
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,0.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,7.f,0.f);
		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,50.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.2f,250.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(0.7f,80.f,0.f);
		m_tEfxData.vAlphaDelta[3] = _vec3(1.f,10.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-45.f,45.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.f,0.f,0.001f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f,35.f,0.f);
		m_tEfxData.vSpreadCylinderRadius[0] = _vec2(0.f,15.f);
		m_tEfxData.vSpreadCylinderRadius[1] = _vec2(0.3f,80.f);
		m_tEfxData.vSpreadCylinderRadius[2] = _vec2(1.f,40.f);
		m_tEfxData.vSpreadCylinderHeight[0] = _vec2(0.f,10.f);
		m_tEfxData.vSpreadCylinderHeight[1] = _vec2(0.5f,25.f);
		m_tEfxData.vSpreadCylinderHeight[2] = _vec2(1.f,45.f);
		m_tEfxData.vMyOffSetPos = _vec3(0.f,5.f,0.f);
		m_tEfxData.isBillBoard = true;
		break;
	
	case KADANTELE7:
		m_tEfxData.iImgIdx = m_eEffectName;
		//m_tEfxData.vPositionOffSet = _vec3(0.f,0.f,45.f);
		m_tEfxData.fEmitInterval = 0.02f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 2;
		m_tEfxData.vColorMin = _vec3(0.f,0.f,0.f);
		m_tEfxData.vColorMax = _vec3(10.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.5f,0.7f);
		m_tEfxData.vSpeed = _vec2(1.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(50.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(1.f,15.f); 
		m_tEfxData.vSpreadRadiusAngle = _vec2(0.f,0.f);
		m_tEfxData.vStartSize = _vec2(15.f,7.f); 
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,0.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,13.f,0.f);
		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,0.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.1f,90.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(0.8f,60.f,0.f);
		m_tEfxData.vAlphaDelta[3] = _vec3(1.f,0.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.f,0.f,0.001f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f,110.f,0.f);
		m_tEfxData.vMyOffSetPos = _vec3(0.f,5.f,0.f);
		m_tEfxData.isBillBoard = true;
		break;

	case KADANTELE8:
		m_tEfxData.iImgIdx = KADANTELE7;
		//m_tEfxData.vPositionOffSet = _vec3(0.f,0.f,45.f);
		m_tEfxData.fEmitInterval = 0.1f;
		m_tEfxData.fEmitDuration = 1.5f;
		m_tEfxData.iParticleCount = 2;
		m_tEfxData.vColorMin = _vec3(0.f,0.f,0.f);
		m_tEfxData.vColorMax = _vec3(10.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(2.5f,3.0f);
		m_tEfxData.vSpeed = _vec2(1.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(50.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(1.f,15.f); 
		m_tEfxData.vSpreadRadiusAngle = _vec2(0.f,0.f);
		m_tEfxData.vStartSize = _vec2(1.f,0.f); 
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,3.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,10.f,0.f);
		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,0.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.1f,200.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(0.4f,100.f,0.f);
		m_tEfxData.vAlphaDelta[3] = _vec3(1.f,-50.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.f,0.f,0.001f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f,110.f,0.f);
		m_tEfxData.vMyOffSetPos = _vec3(0.f,2.f,0.f);
		m_tEfxData.isBillBoard = false;
		m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		m_tEfxData.isDecrease = true;
		break;

	case SWING4DUST0:
		m_tEfxData.iImgIdx = SWING4DUST0 - SWING4DUST0;
		//m_tEfxData.vPositionOffSet = _vec3(110.f,0.f,-20.f);
		m_tEfxData.vPositionOffSet = _vec3(0.f,5.f,0.f);
		m_tEfxData.fEmitInterval = 0.03f;
		m_tEfxData.fEmitDuration = 0.15f;
		m_tEfxData.iParticleCount = 7;
		m_tEfxData.vColorMin = _vec3(0.f,0.f,0.f);
		m_tEfxData.vColorMax = _vec3(30.f,30.f,30.f);
		m_tEfxData.vDieTime = _vec2(0.3f,0.5f);
		m_tEfxData.vSpeed = _vec2(135.f,145.f);

		m_tEfxData.vSpreadAngle = _vec2(150.f,-150.f);
		m_tEfxData.vSpreadRadius = _vec2(10.f,20.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(0.f,0.1f);
		//m_tEfxData.vSpreadCylinderRadius[0] = _vec2(30.f,45.f);
		/*m_tEfxData.vStartSize = _vec2(1.f,0.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,3.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,10.f,0.f);*/
		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,150.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.5f,250.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.f,-150.f,0.f);
		/*m_tEfxData.vStartRoll = _vec2(-360.f,360.f);*/
		m_tEfxData.isBillBoard = true;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,2.f,0.f);
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		m_tEfxData.vLength = _vec2(0.3f,0.26f);
		m_tEfxData.vWidth = _vec2(0.7f,0.5f);
		m_tEfxData.vLengthDelta[0] = _vec2(0.f,0.1f);
		m_tEfxData.vLengthDelta[1] = _vec2(1.f,1.0f);
		m_tEfxData.vWidthDelta[0] = _vec2(0.f,0.1f);
		m_tEfxData.vWidthDelta[1] = _vec2(1.f,4.5f);
		m_tEfxData.fLookDist = 120.f;
		break;

	case SWING4DUST1:
		m_tEfxData.iImgIdx = SWING4DUST1 - SWING4DUST0;
		//m_tEfxData.vPositionOffSet = _vec3(110.f,5.f,-20.f);
		m_tEfxData.vPositionOffSet = _vec3(10.f,5.f,10.f);
		m_tEfxData.fEmitInterval = 0.03f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 6;
		m_tEfxData.vColorMin = _vec3(0.f,0.f,0.f);
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(1.4f,1.7f);
		m_tEfxData.vSpeed = _vec2(170.f,155.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(1.f,-20.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(14.f,-14.f);
		m_tEfxData.vSpreadCylinderRadius[0] = _vec2(3.f,15.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(360.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,40.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.2f,200.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.f,-100.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.isBillBoard = true;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,2.f,0.f);

		m_tEfxData.vLength = _vec2(0.22f , 0.25f);
		m_tEfxData.vWidth = _vec2(1.5f,4.5f);
		m_tEfxData.vLengthDelta[0] = _vec2(0.f,0.1f);
		m_tEfxData.vLengthDelta[1] = _vec2(1.f,3.0f);
		m_tEfxData.fLookDist = 120.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		break;

	case SWING4DUST2:
		m_tEfxData.iImgIdx = SWING4DUST2 - SWING4DUST0;
		m_tEfxData.vPositionOffSet = _vec3(15.f,5.f,15.f);
		m_tEfxData.fEmitInterval = 0.03f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 3;
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(1.7f,2.2f);
		m_tEfxData.vSpeed = _vec2(70.f,55.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(1.f,-20.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(14.f,-14.f);
		m_tEfxData.vSpreadCylinderRadius[0] = _vec2(13.f,35.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(360.f,0.f);
		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.4f,0.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.f,-10.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f,20.1f,0.f);
		m_tEfxData.isBillBoard = true;

		m_tEfxData.vStartSize = _vec2(2.f,2.8f); 
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,10.f,0.f);

		m_tEfxData.vMyOffSetPos = _vec3(0.f,2.f,0.f);
		m_tEfxData.fLookDist = 120.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		break;
	case SWING4DUST3:
		m_tEfxData.iImgIdx = SWING4DUST3 - SWING4DUST0;
		m_tEfxData.vPositionOffSet = _vec3(15.f,0.f,15.f);
		m_tEfxData.fEmitInterval = 0.015f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 1;
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(1.f,1.5f);

		m_tEfxData.vSpreadCylinderRadius[0] = _vec2(13.f,35.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(360.f,0.f);
		m_tEfxData.vSpreadAngle = _vec2(-30.f,30.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.4f,230.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.f,-10.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f,30.1f,0.f);
		m_tEfxData.isBillBoard = true;
		m_tEfxData.vSpeed = _vec2(1.f,0.f);

		m_tEfxData.vStartSize = _vec2(1.f,2.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,2.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.05f,5.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,6.5f,0.f);

		m_tEfxData.vMyOffSetPos = _vec3(0.f,2.f,0.f);
		m_tEfxData.fLookDist = 120.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		break;

	case SWING4DUST4:
		m_tEfxData.iImgIdx = SWING4DUST4 - SWING4DUST0;
		m_tEfxData.vPositionOffSet = _vec3(10.f,5.f,10.f);
		m_tEfxData.fEmitInterval = 0.006f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 1;
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(0.3f,0.6f);

		m_tEfxData.vSpeed = _vec2(240.f,180.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,130.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(1.0f,80.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,150.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.3f,150.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.f,-10.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f,160.1f,0.f);
		m_tEfxData.isBillBoard = true;

		m_tEfxData.vStartSize = _vec2(0.5f,1.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.3f,0.3f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,0.3f,0.f);

		m_tEfxData.vMyOffSetPos = _vec3(0.f,2.f,0.f);
		m_tEfxData.vSpreadAngle = _vec2(130.f,-130.f);
		m_tEfxData.vSpreadRadius = _vec2(10.f,0.f);
		m_tEfxData.fLookDist = 120.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		break;

	case SWING4DUST5:
		m_tEfxData.iImgIdx = SWING4DUST5 - SWING4DUST0;
		m_tEfxData.vPositionOffSet = _vec3(10.f,5.f,10.f);
		m_tEfxData.fEmitInterval = 0.005f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 1;
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(0.4f,0.8f);

		m_tEfxData.vSpeed = _vec2(240.f,180.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,100.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(1.0f,50.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,150.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.3f,150.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.f,-10.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f,160.1f,0.f);
		m_tEfxData.isBillBoard = true;

		m_tEfxData.vStartSize = _vec2(0.05f,0.2f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.3f,0.3f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,0.3f,0.f);

		m_tEfxData.vMyOffSetPos = _vec3(0.f,2.f,0.f);
		m_tEfxData.vSpreadAngle = _vec2(130.f,-130.f);
		m_tEfxData.vSpreadRadius = _vec2(10.f,0.f);
		m_tEfxData.fLookDist = 120.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		break;

	case SWING4DUST6:
		m_tEfxData.iImgIdx = SWING4DUST6 - SWING4DUST0;
		m_tEfxData.vPositionOffSet = _vec3(10.f,5.f,10.f);
		m_tEfxData.fEmitInterval = 0.01f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 1;
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(0.8f,1.3f);

		m_tEfxData.vSpeed = _vec2(140.f,50.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,100.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(1.0f,50.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,150.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.3f,150.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.f,-10.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f,160.1f,0.f);
		m_tEfxData.isBillBoard = true;

		m_tEfxData.vStartSize = _vec2(0.05f,0.2f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.3f,0.3f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,0.3f,0.f);

		m_tEfxData.vMyOffSetPos = _vec3(0.f,2.f,0.f);
		m_tEfxData.vSpreadAngle = _vec2(30.f,-30.f);
		m_tEfxData.vSpreadRadius = _vec2(10.f,0.f);
		m_tEfxData.fLookDist = 120.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		break;

	case SWING4DUST7:
		m_tEfxData.iImgIdx = SWING4DUST2 - SWING4DUST0;
		m_tEfxData.vPositionOffSet = _vec3(15.f,5.f,15.f);
		m_tEfxData.fEmitInterval = 0.03f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 3;
		m_tEfxData.vColorMax = _vec3(30.f,30.f,30.f);
		m_tEfxData.vDieTime = _vec2(1.7f,2.2f);
		m_tEfxData.vSpeed = _vec2(70.f,55.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(1.f,-20.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(14.f,-14.f);
		m_tEfxData.vSpreadCylinderRadius[0] = _vec2(13.f,35.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(360.f,0.f);
		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.4f,0.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.f,-10.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f,20.1f,0.f);
		m_tEfxData.isBillBoard = true;

		m_tEfxData.vStartSize = _vec2(3.f,6.8f); 
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,10.f,0.f);

		m_tEfxData.vMyOffSetPos = _vec3(0.f,2.f,0.f);
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		break;
	case SWING4DUST8:
		m_tEfxData.iImgIdx = SWING4DUST3 - SWING4DUST0;
		m_tEfxData.vPositionOffSet = _vec3(15.f,0.f,15.f);
		m_tEfxData.fEmitInterval = 0.015f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 3;
		m_tEfxData.vColorMax = _vec3(30.f,30.f,30.f);
		m_tEfxData.vDieTime = _vec2(1.f,1.5f);

		m_tEfxData.vSpreadCylinderRadius[0] = _vec2(13.f,35.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(360.f,0.f);
		m_tEfxData.vSpreadAngle = _vec2(-30.f,30.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.4f,230.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.f,-10.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f,30.1f,0.f);
		m_tEfxData.isBillBoard = true;
		m_tEfxData.vSpeed = _vec2(1.f,0.f);

		m_tEfxData.vStartSize = _vec2(3.f,6.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,2.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.05f,5.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,6.5f,0.f);

		m_tEfxData.vMyOffSetPos = _vec3(0.f,2.f,0.f);
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		break;

	case SWING4DUST9:
		m_tEfxData.iImgIdx = SWING4DUST4 - SWING4DUST0;
		m_tEfxData.vPositionOffSet = _vec3(10.f,5.f,10.f);
		m_tEfxData.fEmitInterval = 0.006f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 3;
		m_tEfxData.vColorMax = _vec3(30.f,30.f,30.f);
		m_tEfxData.vDieTime = _vec2(0.3f,0.6f);

		m_tEfxData.vSpeed = _vec2(240.f,180.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,130.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(1.0f,80.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,150.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.3f,150.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.f,-10.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f,160.1f,0.f);
		m_tEfxData.isBillBoard = true;

		m_tEfxData.vStartSize = _vec2(2.5f,5.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.3f,0.3f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,0.3f,0.f);

		m_tEfxData.vMyOffSetPos = _vec3(0.f,2.f,0.f);
		m_tEfxData.vSpreadAngle = _vec2(130.f,-130.f);
		m_tEfxData.vSpreadRadius = _vec2(10.f,0.f);
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		break;

	case SWING4DUST10:
		m_tEfxData.iImgIdx = SWING4DUST5 - SWING4DUST0;
		m_tEfxData.vPositionOffSet = _vec3(10.f,5.f,10.f);
		m_tEfxData.fEmitInterval = 0.005f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 3;
		m_tEfxData.vColorMax = _vec3(30.f,30.f,30.f);
		m_tEfxData.vDieTime = _vec2(0.4f,0.8f);

		m_tEfxData.vSpeed = _vec2(240.f,180.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,100.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(1.0f,50.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,150.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.3f,150.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.f,-10.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f,160.1f,0.f);
		m_tEfxData.isBillBoard = true;

		m_tEfxData.vStartSize = _vec2(2.f,5.2f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.3f,0.3f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,0.3f,0.f);

		m_tEfxData.vMyOffSetPos = _vec3(0.f,2.f,0.f);
		m_tEfxData.vSpreadAngle = _vec2(130.f,-130.f);
		m_tEfxData.vSpreadRadius = _vec2(10.f,0.f);
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		break;

	case KADANWEAPON0:
		m_tEfxData.iImgIdx = KADANWEAPON0 - KADANWEAPON0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,5.f,1.f);
		m_tEfxData.fEmitInterval = 0.03f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 20;
		//m_tEfxData.vColorMin = _vec3(255,240,171);
		m_tEfxData.vColorMax = _vec3(255.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.5f,0.0f);

		m_tEfxData.vSpeed = _vec2(-50.f,-70.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,-10.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(1.0f,-5.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.1f,-255.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(0.2f,255.f,0.f);
		m_tEfxData.vAlphaDelta[3] = _vec3(0.6f,-200.f,0.f);
		m_tEfxData.vAlphaDelta[4] = _vec3(1.0f,-255.f,0.f);
		/*m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f,160.1f,0.f);*/
		m_tEfxData.isBillBoard = true;

		/*m_tEfxData.vStartSize = _vec2(0.05f,0.2f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.3f,0.3f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,0.3f,0.f);*/

		m_tEfxData.vMyOffSetPos = _vec3(0.f,2.f,0.f);
		m_tEfxData.vSpreadAngle = _vec2(2.f,-2.f);
		m_tEfxData.vSpreadRadius = _vec2(5.f,15.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(10.f,0.f);

		m_tEfxData.vLength = _vec2(0.7f,1.0f);
		m_tEfxData.vWidth = _vec2(5.f,7.f);
		m_tEfxData.isLoop = true;
		
		//m_tEfxData.fLookDist = 120.f;
		m_tEfxData.vOffSetAngle = _vec3(D3DX_PI,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		break;

	case KADANWEAPON1:
		m_tEfxData.iImgIdx = KADANWEAPON1 - KADANWEAPON0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,5.f,1.f);
		m_tEfxData.fEmitInterval = 0.03f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 1;
		//m_tEfxData.vColorMin = _vec3(255,240,171);
		m_tEfxData.vColorMax = _vec3(0.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.5f,0.75f);

		m_tEfxData.vSpeed = _vec2(-150.f,-120.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,-10.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,-5.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.1f,-255.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(0.2f,255.f,0.f);
		m_tEfxData.vAlphaDelta[3] = _vec3(0.6f,-200.f,0.f);
		m_tEfxData.vAlphaDelta[4] = _vec3(1.0f,-255.f,0.f);
		/*m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f,160.1f,0.f);*/
		m_tEfxData.isBillBoard = true;

		/*m_tEfxData.vStartSize = _vec2(0.05f,0.2f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.3f,0.3f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,0.3f,0.f);*/

		m_tEfxData.vMyOffSetPos = _vec3(0.f,2.f,0.f);
		m_tEfxData.vSpreadAngle = _vec2(1.f,-1.f);
		m_tEfxData.vSpreadRadius = _vec2(5.f,15.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(10.f,0.f);

		m_tEfxData.vLength = _vec2(0.7f,1.f);
		m_tEfxData.vWidth = _vec2(10.f,20.f);
		m_tEfxData.vWidthDelta[0] = _vec2(0.01f,0.f);
		m_tEfxData.vWidthDelta[1] = _vec2(1.f,-5.f);
		m_tEfxData.isLoop = true;
		//m_tEfxData.fLookDist = 120.f;
		m_tEfxData.vOffSetAngle = _vec3(D3DX_PI,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		break;

	case KADANWEAPON2:

		m_tEfxData.iImgIdx = KADANWEAPON2 - KADANWEAPON0;
		m_tEfxData.vPositionOffSet = _vec3(1.5f,1.5f,1.5f);
		m_tEfxData.fEmitInterval = 0.25f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 15;
		m_tEfxData.vColorMax = _vec3(0.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.1f,0.45f);
		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.1f,-255.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(0.2f,255.f,0.f);
		m_tEfxData.vAlphaDelta[3] = _vec3(0.6f,-200.f,0.f);
		m_tEfxData.vAlphaDelta[4] = _vec3(1.0f,-255.f,0.f);
		m_tEfxData.isBillBoard = true;
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vSpreadAngle = _vec2(1.f,-1.f);
		m_tEfxData.vSpreadRadius = _vec2(0.f,5.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(0.f,0.f);
		m_tEfxData.vLength = _vec2(2.0f,2.5f);
		m_tEfxData.vWidth = _vec2(3.0f,4.5f);
		m_tEfxData.vWidthDelta[0] = _vec2(0.01f,0.f);
		m_tEfxData.vWidthDelta[1] = _vec2(1.f,-2.f);
		m_tEfxData.vLookDistRand = _vec2(-0.1f, -100.f);
		m_tEfxData.isLoop = true;
		
		break;

	case KADANWEAPON3:
		m_tEfxData.iImgIdx = KADANWEAPON3 - KADANWEAPON0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,5.f,1.f);
		m_tEfxData.fEmitInterval = 0.05f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 1;
		//m_tEfxData.vColorMin = _vec3(255,240,171);
		m_tEfxData.vColorMax = _vec3(0.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.5f,0.75f);

		m_tEfxData.vSpeed = _vec2(-160.f,-120.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,-10.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,-5.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.1f,-255.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(0.2f,255.f,0.f);
		m_tEfxData.vAlphaDelta[3] = _vec3(0.6f,-200.f,0.f);
		m_tEfxData.vAlphaDelta[4] = _vec3(1.0f,-255.f,0.f);
		/*m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f,160.1f,0.f);*/
		m_tEfxData.isBillBoard = true;

		/*m_tEfxData.vStartSize = _vec2(0.05f,0.2f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.3f,0.3f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,0.3f,0.f);*/

		m_tEfxData.vMyOffSetPos = _vec3(0.f,2.f,0.f);
		m_tEfxData.vSpreadAngle = _vec2(1.f,-1.f);
		m_tEfxData.vSpreadRadius = _vec2(1.f,5.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(0.f,0.f);

		m_tEfxData.vLength = _vec2(0.7f,1.f);
		m_tEfxData.vWidth = _vec2(10.f,20.f);
		m_tEfxData.vWidthDelta[0] = _vec2(0.01f,0.f);
		m_tEfxData.vWidthDelta[1] = _vec2(1.f,-6.f);
		m_tEfxData.isLoop = true;
		//m_tEfxData.fLookDist = 120.f;
		m_tEfxData.vOffSetAngle = _vec3(D3DX_PI,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		break;

	case KADANWEAPON4:
		m_tEfxData.iImgIdx = KADANWEAPON4 - KADANWEAPON0;
		m_tEfxData.vPositionOffSet = _vec3(1.5f,1.5f,1.5f);
		m_tEfxData.fEmitInterval = 0.25f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 15;
		//m_tEfxData.vColorMin = _vec3(255,240,171);
		m_tEfxData.vColorMin = _vec3(50.1f,0.f,0.f);
		m_tEfxData.vColorMax = _vec3(130.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.1f,0.45f);

		//m_tEfxData.vSpeed = _vec2(-150.f,-120.f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,-10.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,-5.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.1f,-255.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(0.2f,255.f,0.f);
		m_tEfxData.vAlphaDelta[3] = _vec3(0.6f,-200.f,0.f);
		m_tEfxData.vAlphaDelta[4] = _vec3(1.0f,-255.f,0.f);
		/*m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f,160.1f,0.f);*/
		m_tEfxData.isBillBoard = true;

		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vSpreadAngle = _vec2(1.f,-1.f);
		m_tEfxData.vSpreadRadius = _vec2(0.f,5.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(0.f,0.f);

		m_tEfxData.vLength = _vec2(2.0f,2.5f);
		m_tEfxData.vWidth = _vec2(3.0f,4.5f);
		m_tEfxData.vWidthDelta[0] = _vec2(0.01f,0.f);
		m_tEfxData.vWidthDelta[1] = _vec2(1.f,-2.f);
		m_tEfxData.vLookDistRand = _vec2(-0.1f, -100.f);
		//m_tEfxData.isAttached = true;
		m_tEfxData.isLoop = true;
		//m_tEfxData.fLookDist = 120.f;
		//m_tEfxData.vStartRoll = _vec2(-120.f,120.f);
		//m_tEfxData.isDecrease = true;
		break;

	case KADANWEAPON7:
		m_tEfxData.iImgIdx = KADANWEAPON7 - KADANWEAPON0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,5.f,1.f);
		m_tEfxData.fEmitInterval = 0.03f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 1;
		//m_tEfxData.vColorMin = _vec3(255,240,171);
		m_tEfxData.vColorMax = _vec3(70.f,10.f,10.f);
		m_tEfxData.vDieTime = _vec2(0.5f,0.75f);

		m_tEfxData.vSpeed = _vec2(-250.f,-220.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,-10.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,-5.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.1f,-255.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(0.2f,255.f,0.f);
		m_tEfxData.vAlphaDelta[3] = _vec3(0.6f,-200.f,0.f);
		m_tEfxData.vAlphaDelta[4] = _vec3(1.0f,-255.f,0.f);
		/*m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f,160.1f,0.f);*/
		m_tEfxData.isBillBoard = true;

		/*m_tEfxData.vStartSize = _vec2(0.05f,0.2f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.f,0.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.3f,0.3f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,0.3f,0.f);*/

		m_tEfxData.vMyOffSetPos = _vec3(0.f,2.f,0.f);
		m_tEfxData.vSpreadAngle = _vec2(1.f,-1.f);
		m_tEfxData.vSpreadRadius = _vec2(0.f,5.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(0.f,0.f);

		m_tEfxData.vLength = _vec2(0.7f,1.f);
		m_tEfxData.vWidth = _vec2(10.f,20.f);
		m_tEfxData.vWidthDelta[0] = _vec2(0.01f,0.f);
		m_tEfxData.vWidthDelta[1] = _vec2(1.f,-6.f);
		m_tEfxData.isLoop = true;
		//m_tEfxData.fLookDist = 120.f;
		m_tEfxData.vOffSetAngle = _vec3(D3DX_PI,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		break;

	case KADANWEAPON12:
		m_tEfxData.iImgIdx = KADANWEAPON12 - KADANWEAPON0;
		//m_tEfxData.vPositionOffSet = _vec3(1.f,5.f,1.f);
		m_tEfxData.fEmitInterval = 0.01f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 20;
		//m_tEfxData.vColorMin = _vec3(255,240,171);
		m_tEfxData.vColorMax = _vec3(71.f,10.f,10.f);
		m_tEfxData.vDieTime = _vec2(0.4f,1.f);
		m_tEfxData.vSpeed = _vec2(1.5f,-1.5f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.3f,100.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(0.6f,255.f,0.f);
		m_tEfxData.vAlphaDelta[3] = _vec3(1.0f,100.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
	
		m_tEfxData.isBillBoard = true;
		m_tEfxData.vStartSize = _vec2(1.f,4.f);
	
		m_tEfxData.vMyOffSetPos = _vec3(0.f,0.f,0.f);
		m_tEfxData.vSpreadAngle = _vec2(1.f,-1.f);
		m_tEfxData.vSpreadRadius = _vec2(10.f,130.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(0.f,0.f);
		m_tEfxData.vLookDistRand = _vec2(-0.1f, -100.f);
		m_tEfxData.isLoop = true;
		//m_tEfxData.fLookDist = 120.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		break;

	case KADANWEAPON13:
		m_tEfxData.iImgIdx = KADANWEAPON13 - KADANWEAPON0;
		//m_tEfxData.vPositionOffSet = _vec3(5.f,5.f,5.f);
		m_tEfxData.fEmitInterval = 0.01f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 20;
		//m_tEfxData.vColorMin = _vec3(255,240,171);
		m_tEfxData.vColorMax = _vec3(71.f,10.f,10.f);
		m_tEfxData.vDieTime = _vec2(0.2f,1.5f);
		m_tEfxData.vSpeed = _vec2(1.5f,-1.5f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.f,100.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.1f,255.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(0.5f,255.f,0.f);
		m_tEfxData.vAlphaDelta[3] = _vec3(1.0f,-200.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);

		m_tEfxData.isBillBoard = true;

		m_tEfxData.vStartSize = _vec2(1.f,4.f);

		m_tEfxData.vMyOffSetPos = _vec3(0.f,0.f,0.f);
		m_tEfxData.vSpreadAngle = _vec2(1.f,-1.f);
		m_tEfxData.vSpreadRadius = _vec2(10.f,130.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(0.f,0.f);
		m_tEfxData.vLookDistRand = _vec2(-0.1f, -100.f);
		m_tEfxData.isLoop = true;
		

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;


	case SPRINTRUN0:
		m_tEfxData.iImgIdx = SPRINTRUN0 - SPRINTRUN0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.05f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 10;
		m_tEfxData.vColorMax = _vec3(155.f,155.f,155.f);
		m_tEfxData.vDieTime = _vec2(0.5f,1.0f);
		m_tEfxData.vSpeed = _vec2(10.f,0.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,60.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,70.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,10.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(30.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(20.f,0.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(30.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.3f,200.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.7f,50.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-100.f,0.f);
		//m_tEfxData.vStartRoll = _vec2(-360.f,360.f);

		m_tEfxData.isBillBoard = true;

		m_tEfxData.vLength = _vec2(1.7f,1.5f);
		m_tEfxData.vWidth = _vec2(4.0f,0.f);

		m_tEfxData.fLookDist = 10.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case SPRINTRUN1:
		m_tEfxData.iImgIdx = SPRINTRUN1 - SPRINTRUN0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.05f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 10;
		m_tEfxData.vColorMax = _vec3(60.f,60.f,62.f);
		m_tEfxData.vDieTime = _vec2(0.5f,1.f);
		m_tEfxData.vSpeed = _vec2(10.f,40.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,60.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,30.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,1.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(60.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(20.f,0.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(30.f,0.f);

		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,3.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,4.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.3f,250.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.7f,150.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,0.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-60.f,60.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 50.f,0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 100.f,0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.0f, 150.f,0.f);

		m_tEfxData.isBillBoard = true;
		m_tEfxData.fLookDist = 10.f;
		




		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case ATKLEFT0:
		m_tEfxData.iImgIdx = ATKLEFT0 - ATKLEFT0;
		m_tEfxData.vPositionOffSet = _vec3(5.f,0.f,5.f);
		m_tEfxData.fEmitInterval = 0.01f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 7;
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(0.3f,0.5f);
		m_tEfxData.vSpeed = _vec2(50.f,0.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,60.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,70.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,70.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(50.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(1.f,15.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(30.f,0.f);

		/*m_tEfxData.vSizeDelta[0] = _vec3(0.1f,2.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,3.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,5.f,0.f);*/

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,250.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.7f,150.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-100.f,0.f);

		m_tEfxData.vLength = _vec2(2.7f,3.5f);
		m_tEfxData.vWidth = _vec2(4.0f,0.f);

		/*m_tEfxData.vStartRoll = _vec2(-60.f,60.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 100.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);*/

		m_tEfxData.isBillBoard = true;
		m_tEfxData.fLookDist = -150.f;
		m_tEfxData.isStartedFromLand = true;
		//m_tEfxData.fLookDist = 10.f;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

		case ATKLEFT1:
		m_tEfxData.iImgIdx = ATKLEFT1 - ATKLEFT0;
		m_tEfxData.vPositionOffSet = _vec3(5.f,0.f,5.f);
		m_tEfxData.fEmitInterval = 0.01f;
		m_tEfxData.fEmitDuration = 0.4f;
		m_tEfxData.iParticleCount = 2;
		m_tEfxData.vColorMax = _vec3(39.f,35.f,27.f);
		m_tEfxData.vDieTime = _vec2(0.3f,0.4f);
		m_tEfxData.vSpeed = _vec2(50.f,80.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,80.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,30.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,1.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(40.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(20.f,0.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(30.f,0.f);

		m_tEfxData.vStartSize = _vec2(1.f,2.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,1.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,3.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,250.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.7f,150.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-100.f,0.f);

		/*m_tEfxData.vLength = _vec2(1.7f,1.5f);
		m_tEfxData.vWidth = _vec2(10.0f,0.f);*/

		m_tEfxData.vStartRoll = _vec2(-60.f,60.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 50.f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 100.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.0f, 150.f, 0.f);

		m_tEfxData.isBillBoard = true;
		m_tEfxData.fLookDist = -150.f;
		m_tEfxData.isStartedFromLand = true;
		//m_tEfxData.fLookDist = 10.f;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

		case ATKLEFT2:
		m_tEfxData.iImgIdx = ATKLEFT2 - ATKLEFT0;
		m_tEfxData.vPositionOffSet = _vec3(10.f,0.f,10.f);
		m_tEfxData.fEmitInterval = 0.01f;
		m_tEfxData.fEmitDuration = 0.1f;
		m_tEfxData.iParticleCount = 15;
		m_tEfxData.vGravity = _vec2(30.f,60.f);
		m_tEfxData.vColorMax = _vec3(239.f,235.f,227.f);
		m_tEfxData.vDieTime = _vec2(0.6f,0.9f);
		m_tEfxData.vSpeed = _vec2(350.f,480.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,80.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,30.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,1.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(30.f,-30.f);
		m_tEfxData.vSpreadRadius = _vec2(10.f,40.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(-90.f,90.f);

		m_tEfxData.vStartSize = _vec2(0.5f,0.7f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.3f,250.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.7f,50.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-100.f,0.f);

		/*m_tEfxData.vLength = _vec2(1.7f,1.5f);
		m_tEfxData.vWidth = _vec2(10.0f,0.f);*/

		m_tEfxData.vStartRoll = _vec2(-60.f,60.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 50.f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 100.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.0f, 150.f, 0.f);

		m_tEfxData.isBillBoard = true;
		m_tEfxData.fLookDist = -150.f;
		m_tEfxData.isStartedFromLand = true;
		//m_tEfxData.fLookDist = 10.f;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

		case ATKLEFT3:
		m_tEfxData.iImgIdx = ATKLEFT3 - ATKLEFT0;
		m_tEfxData.vPositionOffSet = _vec3(10.f,0.f,10.f);
		m_tEfxData.fEmitInterval = 0.01f;
		m_tEfxData.fEmitDuration = 0.1f;
		m_tEfxData.iParticleCount = 10;
		m_tEfxData.vColorMax = _vec3(239.f,235.f,227.f);
		m_tEfxData.vDieTime = _vec2(0.6f,0.9f);
		m_tEfxData.vSpeed = _vec2(50.f,80.f);
		m_tEfxData.vGravity = _vec2(30.f,60.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,80.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,30.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,1.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(40.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(5.f,20.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(90.f,0.f);

		m_tEfxData.vStartSize = _vec2(2.f,1.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.3f,250.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.7f,150.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-100.f,0.f);

		/*m_tEfxData.vLength = _vec2(1.7f,1.5f);
		m_tEfxData.vWidth = _vec2(10.0f,0.f);*/

		m_tEfxData.vStartRoll = _vec2(-60.f,60.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 50.f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 100.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.0f, 150.f, 0.f);

		m_tEfxData.isBillBoard = true;
		m_tEfxData.fLookDist = -150.f;
		m_tEfxData.isStartedFromLand = true;
		//m_tEfxData.fLookDist = 10.f;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

		case ATKLEFT4:
		m_tEfxData.iImgIdx = ATKLEFT0 - ATKLEFT0;
		m_tEfxData.vPositionOffSet = _vec3(10.f,0.f,10.f);
		m_tEfxData.fEmitInterval = 0.01f;
		m_tEfxData.fEmitDuration = 0.07f;
		m_tEfxData.iParticleCount = 10;
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(1.4f,1.8f);
		m_tEfxData.vSpeed = _vec2(10.f,30.f);

		m_tEfxData.vSpreadAngle = _vec2(50.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(5.f,55.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(350.f,0.f);

		m_tEfxData.vStartSize = _vec2(2.f,1.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,5.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,10.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,11.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.3f,250.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.7f,100.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-100.f,0.f);

		/*m_tEfxData.vLength = _vec2(1.7f,1.5f);
		m_tEfxData.vWidth = _vec2(10.0f,0.f);*/

		m_tEfxData.vStartRoll = _vec2(-60.f,60.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 50.f, 0.f);

		m_tEfxData.isBillBoard = true;
		m_tEfxData.fLookDist = -150.f;
		m_tEfxData.isStartedFromLand = true;
		//m_tEfxData.fLookDist = 10.f;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

		case ATKLEFT5:
		m_tEfxData.iImgIdx = ATKLEFT4 - ATKLEFT0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,0.f,1.f);
		m_tEfxData.fEmitInterval = 0.01f;
		m_tEfxData.fEmitDuration = 0.2f;
		m_tEfxData.iParticleCount = 15;
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(0.5f,1.0f);
		m_tEfxData.vSpeed = _vec2(150.f,320.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f, 100.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f, 50.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(0.6f, 20.f,0.f);
		m_tEfxData.vSpeedDelta[3] = _vec3(0.1f, 10.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(35.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(5.f,0.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(0.f,0.f);

		m_tEfxData.vStartSize = _vec2(2.f,5.f);
		

		m_tEfxData.vAlphaDelta[0] = _vec3(0.3f,250.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.7f,200.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-0.f,0.f);

		m_tEfxData.vLength = _vec2(1.2f,1.25f);
		m_tEfxData.vWidth = _vec2(1.5f,2.6f);
	/*	m_tEfxData.vLengthDelta[0] = _vec3(0.1f,0.1f,0.f);
		m_tEfxData.vLengthDelta[1] = _vec3(1.f,-0.16f,0.f);*/

		m_tEfxData.isBillBoard = true;
		m_tEfxData.isDestAlphaOne = true;
		m_tEfxData.fLookDist = -150.f;
		m_tEfxData.isStartedFromLand = true;
		//m_tEfxData.fLookDist = 10.f;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

		case ATKLEFT6:
		m_tEfxData.iImgIdx = ATKLEFT4 - ATKLEFT0;
		m_tEfxData.vPositionOffSet = _vec3(5.f,0.f,5.f);
		m_tEfxData.fEmitInterval = 0.1f;
		m_tEfxData.fEmitDuration = 0.25f;
		m_tEfxData.iParticleCount = 10;
		m_tEfxData.vGravity = _vec2(10.f,100.f);
		
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(0.7f,1.3f);
		m_tEfxData.vSpeed = _vec2(350.f,450.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f, 50.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f, 10.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(0.6f, 10.f,0.f);
		m_tEfxData.vSpeedDelta[3] = _vec3(0.1f, 10.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(30.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(15.f,30.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(30.f,0.f);

		m_tEfxData.vStartSize = _vec2(1.f,4.f);
		

		m_tEfxData.vAlphaDelta[0] = _vec3(0.3f,250.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.7f,200.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-0.f,0.f);

		m_tEfxData.vLength = _vec2(1.2f,0.45f);
		m_tEfxData.vWidth = _vec2(1.5f,0.6f);
	/*	m_tEfxData.vLengthDelta[0] = _vec3(0.1f,0.1f,0.f);
		m_tEfxData.vLengthDelta[1] = _vec3(1.f,-0.16f,0.f);*/

		m_tEfxData.isBillBoard = true;
		m_tEfxData.isDestAlphaOne = true;
		m_tEfxData.fLookDist = -150.f;
		m_tEfxData.isStartedFromLand = true;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

		case ATKLEFT7:
		m_tEfxData.iImgIdx = ATKLEFT4 - ATKLEFT0;
		m_tEfxData.vPositionOffSet = _vec3(3.f,0.f,3.f);
		m_tEfxData.fEmitInterval = 0.01f;
		m_tEfxData.fEmitDuration = 0.2f;
		m_tEfxData.iParticleCount = 10;
		m_tEfxData.vColorMax = _vec3(246.f,226.f,203.f);
		m_tEfxData.vDieTime = _vec2(0.5f,0.8f);
		m_tEfxData.vSpeed = _vec2(10.f,50.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f, 30.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f, 10.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(0.1f, 0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(20.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(1.f,0.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(30.f,0.f);

		m_tEfxData.vStartSize = _vec2(0.3f,1.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.01f,1.7f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.1f,2.f,0.f);	
		m_tEfxData.vSizeDelta[2] = _vec3(0.2f,5.f,0.f);
		m_tEfxData.vSizeDelta[3] = _vec3(0.5f,2.f,0.f);
		m_tEfxData.vSizeDelta[4] = _vec3(1.f,1.f,0.f);
		

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,200.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.7f,100.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-50.f,0.f);

		m_tEfxData.vStartRoll = _vec2(-60.f,60.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f,100.f,0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f,200.f,0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.f,300.f,0.f);

		/*m_tEfxData.vLength = _vec2(0.2f,0.25f);
		m_tEfxData.vWidth = _vec2(1.5f,0.6f);*/
	/*	m_tEfxData.vLengthDelta[0] = _vec3(0.1f,0.1f,0.f);
		m_tEfxData.vLengthDelta[1] = _vec3(1.f,-0.16f,0.f);*/

		m_tEfxData.isBillBoard = true;
		m_tEfxData.isDestAlphaOne = true;
		m_tEfxData.fLookDist = -150.f;
		m_tEfxData.isStartedFromLand = true;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case ATKRIGHT0:
		m_tEfxData.iImgIdx = ATKLEFT4 - ATKLEFT0;
		m_tEfxData.vPositionOffSet = _vec3(5.f,0.f,5.f);
		m_tEfxData.fEmitInterval = 0.1f;
		m_tEfxData.fEmitDuration = 0.25f;
		m_tEfxData.iParticleCount = 10;
		m_tEfxData.vGravity = _vec2(10.f,100.f);
		
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(0.7f,1.3f);
		m_tEfxData.vSpeed = _vec2(350.f,450.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f, 50.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f, 10.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(0.6f, 10.f,0.f);
		m_tEfxData.vSpeedDelta[3] = _vec3(0.1f, 10.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(-60.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(15.f,30.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(30.f,0.f);

		m_tEfxData.vStartSize = _vec2(1.f,4.f);
		

		m_tEfxData.vAlphaDelta[0] = _vec3(0.3f,250.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.7f,200.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-0.f,0.f);

		m_tEfxData.vLength = _vec2(1.2f,0.45f);
		m_tEfxData.vWidth = _vec2(1.5f,0.6f);
	/*	m_tEfxData.vLengthDelta[0] = _vec3(0.1f,0.1f,0.f);
		m_tEfxData.vLengthDelta[1] = _vec3(1.f,-0.16f,0.f);*/

		m_tEfxData.isBillBoard = true;
		m_tEfxData.isDestAlphaOne = true;
		m_tEfxData.fLookDist = -150.f;
		m_tEfxData.isStartedFromLand = true;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case BOLT0:
		m_tEfxData.iImgIdx = BOLT0 - BOLT0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.1f;
		m_tEfxData.fEmitDuration = 5.5f;
		m_tEfxData.iParticleCount = 2;
		//m_tEfxData.vGravity = _vec2(10.f,100.f);
		
		m_tEfxData.vColorMax = _vec3(70.f,15.f,20.f);
		m_tEfxData.vDieTime = _vec2(7.5f,7.6f);
		m_tEfxData.vSpeed = _vec2(0.f,0.f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f, 50.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f, 10.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(0.6f, 10.f,0.f);
		//m_tEfxData.vSpeedDelta[3] = _vec3(0.1f, 10.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(0.1f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(1.f,0.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(1.f,0.f);

		m_tEfxData.vStartSize = _vec2(1.f,2.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,0.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.4f,3.5f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,4.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.8f,-600.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-100.f,0.f);

		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		//m_tEfxData.vLength = _vec2(1.2f,0.45f);
		//m_tEfxData.vWidth = _vec2(1.5f,0.6f);
	/*	m_tEfxData.vLengthDelta[0] = _vec3(0.1f,0.1f,0.f);
		m_tEfxData.vLengthDelta[1] = _vec3(1.f,-0.16f,0.f);*/

		m_tEfxData.isBillBoard = true;
		//m_tEfxData.isDestAlphaOne = true;
		//m_tEfxData.fLookDist = -150.f;
		//m_tEfxData.isStartedFromLand = true;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case BOLT1:
		m_tEfxData.iImgIdx = BOLT1 - BOLT0;
		m_tEfxData.vPositionOffSet = _vec3(2.f,2.f,2.f);
		m_tEfxData.fEmitInterval = 0.1f;
		m_tEfxData.fEmitDuration = 5.5f;
		m_tEfxData.iParticleCount = 2;
		//m_tEfxData.vGravity = _vec2(10.f,100.f);
		
		m_tEfxData.vColorMax = _vec3(0.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(7.5f,7.6f);
		m_tEfxData.vSpeed = _vec2(0.f,0.f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f, 50.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f, 10.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(0.6f, 10.f,0.f);
		//m_tEfxData.vSpeedDelta[3] = _vec3(0.1f, 10.f,0.f);
		m_tEfxData.vSpreadAngle = _vec2(-180.f,+180.f);
		m_tEfxData.vSpreadRadius = _vec2(10.f,-10.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(10.f,180.f);

		m_tEfxData.vStartSize = _vec2(0.5f,1.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,0.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.4f,3.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,4.5f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.8f,-600.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-100.f,0.f);

		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		//m_tEfxData.vLength = _vec2(1.2f,0.45f);
		//m_tEfxData.vWidth = _vec2(1.5f,0.6f);
	/*	m_tEfxData.vLengthDelta[0] = _vec3(0.1f,0.1f,0.f);
		m_tEfxData.vLengthDelta[1] = _vec3(1.f,-0.16f,0.f);*/

		m_tEfxData.isBillBoard = true;
		//m_tEfxData.isDestAlphaOne = true;
		//m_tEfxData.fLookDist = -150.f;
		//m_tEfxData.isStartedFromLand = true;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case BOLT2:
		m_tEfxData.iImgIdx = BOLT2 - BOLT0;
		//m_tEfxData.vPositionOffSet = _vec3(0.f,2.f,2.f);
		m_tEfxData.fEmitInterval = 0.01f;
		m_tEfxData.fEmitDuration = 0.06f;
		m_tEfxData.iParticleCount = 3;
		//m_tEfxData.vGravity = _vec2(10.f,100.f);
		
		m_tEfxData.vColorMax = _vec3(0.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.2f,0.4f);
		m_tEfxData.vSpeed = _vec2(1.f,0.f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f, 50.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f, 10.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(0.6f, 10.f,0.f);
		//m_tEfxData.vSpeedDelta[3] = _vec3(0.1f, 10.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(20.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(10.f,0.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(40.f,0.f);

		m_tEfxData.vStartSize = _vec2(3.f,5.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,0.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.4f,7.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,9.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.8f,200.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-100.f,0.f);

		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f,0.1f,0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f,5.f,0.f);
		//m_tEfxData.vLength = _vec2(1.2f,0.45f);
		//m_tEfxData.vWidth = _vec2(1.5f,0.6f);
	/*	m_tEfxData.vLengthDelta[0] = _vec3(0.1f,0.1f,0.f);
		m_tEfxData.vLengthDelta[1] = _vec3(1.f,-0.16f,0.f);*/

		m_tEfxData.isBillBoard = true;
		//m_tEfxData.isDestAlphaOne = true;
		//m_tEfxData.fLookDist = -150.f;
		//m_tEfxData.isStartedFromLand = true;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case FIREBALL0:
		m_tEfxData.iImgIdx = FIREBALL0 - FIREBALL0;
		//m_tEfxData.vPositionOffSet = _vec3(0.f,2.f,2.f);
		m_tEfxData.fEmitInterval = 0.02f;
		m_tEfxData.fEmitDuration = 2.f;
		m_tEfxData.iParticleCount = 5;
		//m_tEfxData.vGravity = _vec2(10.f,100.f);
		
		m_tEfxData.vColorMax = _vec3(30.f,30.f,30.f);
		m_tEfxData.vDieTime = _vec2(1.5f,2.0f);
		m_tEfxData.vSpeed = _vec2(100.f,150.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f, 30.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f, 10.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f, 0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(-180.f,180.f);
		m_tEfxData.vSpreadRadius = _vec2(5.f,10.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(1.f,180.f);

		m_tEfxData.vStartSize = _vec2(0.4f,0.6f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,5.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.2f,0.5f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(0.3f,5.5f,0.f);
		m_tEfxData.vSizeDelta[3] = _vec3(0.5f,0.5f,0.f);
		m_tEfxData.vSizeDelta[4] = _vec3(1.f,1.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,200.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.7f,-500.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-500.f,0.f);

		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f,50.f,0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f,100.f,0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.f,150.f,0.f);
		m_tEfxData.vMyOffSetPos = _vec3(0.f,5.f,0.f);
		//m_tEfxData.vLength = _vec2(1.2f,0.45f);
		//m_tEfxData.vWidth = _vec2(1.5f,0.6f);
	/*	m_tEfxData.vLengthDelta[0] = _vec3(0.1f,0.1f,0.f);
		m_tEfxData.vLengthDelta[1] = _vec3(1.f,-0.16f,0.f);*/

		m_tEfxData.isBillBoard = true;
		//m_tEfxData.isDestAlphaOne = true;
		//m_tEfxData.fLookDist = -150.f;
		//m_tEfxData.isStartedFromLand = true;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

		case FIREBALL1:
		m_tEfxData.iImgIdx = FIREBALL1 - FIREBALL0;
		//m_tEfxData.vPositionOffSet = _vec3(0.f,2.f,2.f);
		m_tEfxData.fEmitInterval = 0.1f;
		m_tEfxData.fEmitDuration = 2.f;
		m_tEfxData.iParticleCount = 5;
		//m_tEfxData.vGravity = _vec2(10.f,100.f);
		
		m_tEfxData.vColorMax = _vec3(120.f,40.f,30.f);
		m_tEfxData.vDieTime = _vec2(1.4f,1.5f);
		m_tEfxData.vSpeed = _vec2(0.f,0.f);

		//m_tEfxData.vSpreadAngle = _vec2(360.f,0.f);
		//m_tEfxData.vSpreadRadius = _vec2(1.f,0.f);
		//m_tEfxData.vSpreadRadiusAngle = _vec2(0.f,0.f);

		m_tEfxData.vStartSize = _vec2(2.f,4.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,5.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.2f,2.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(0.4f,5.f,0.f);
		m_tEfxData.vSizeDelta[3] = _vec3(0.6f,2.f,0.f);
		m_tEfxData.vSizeDelta[4] = _vec3(0.8f,5.f,0.f);
		m_tEfxData.vSizeDelta[5] = _vec3(1.f,2.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.2f,155.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.5f,-155.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,0.f,0.f);

		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f,0.1f,0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f,100.f,0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.f,150.f,0.f);
		m_tEfxData.vMyOffSetPos = _vec3(0.f,5.f,0.f);
		//m_tEfxData.vLength = _vec2(1.2f,0.45f);
		//m_tEfxData.vWidth = _vec2(1.5f,0.6f);
	/*	m_tEfxData.vLengthDelta[0] = _vec3(0.1f,0.1f,0.f);
		m_tEfxData.vLengthDelta[1] = _vec3(1.f,-0.16f,0.f);*/

		m_tEfxData.isBillBoard = true;
		m_tEfxData.isDestAlphaOne = true;
		//m_tEfxData.fLookDist = -150.f;
		//m_tEfxData.isStartedFromLand = true;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case FIREBALL2:
		m_tEfxData.iImgIdx = FIREBALL0 - FIREBALL0;
		//m_tEfxData.vPositionOffSet = _vec3(0.f,2.f,2.f);
		m_tEfxData.fEmitInterval = 0.02f;
		m_tEfxData.fEmitDuration = 1.f;
		m_tEfxData.iParticleCount = 5;
		//m_tEfxData.vGravity = _vec2(10.f,100.f);
		
		m_tEfxData.vColorMax = _vec3(255.f,80.f,60.f);
		m_tEfxData.vDieTime = _vec2(0.2f,0.3f);
		m_tEfxData.vSpeed = _vec2(50.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(360.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(1.f,0.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(0.f,0.f);

		m_tEfxData.vStartSize = _vec2(2.f,4.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,5.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.2f,2.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(0.4f,5.f,0.f);
		m_tEfxData.vSizeDelta[3] = _vec3(0.6f,2.f,0.f);
		m_tEfxData.vSizeDelta[4] = _vec3(0.8f,5.f,0.f);
		m_tEfxData.vSizeDelta[5] = _vec3(1.f,2.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.3f,0.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(0.5f,255.f,0.f);
		m_tEfxData.vAlphaDelta[3] = _vec3(0.7f,100.f,0.f);
		m_tEfxData.vAlphaDelta[4] = _vec3(1.0f,-100.f,0.f);
		//m_tEfxData.isDistortion = true;

	/*	m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f,0.1f,0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f,100.f,0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.f,150.f,0.f);*/
		m_tEfxData.vMyOffSetPos = _vec3(0.f,5.f,0.f);
		//m_tEfxData.vLength = _vec2(1.2f,0.45f);
		//m_tEfxData.vWidth = _vec2(1.5f,0.6f);
	/*	m_tEfxData.vLengthDelta[0] = _vec3(0.1f,0.1f,0.f);
		m_tEfxData.vLengthDelta[1] = _vec3(1.f,-0.16f,0.f);*/

		m_tEfxData.isBillBoard = true;
		//m_tEfxData.isDestAlphaOne = true;
		//m_tEfxData.fLookDist = -150.f;
		//m_tEfxData.isStartedFromLand = true;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case SPRINTRUN2:
		m_tEfxData.iImgIdx = SPRINTRUN0 - SPRINTRUN0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.05f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 10;
		m_tEfxData.vColorMax = _vec3(200.f,200.f,200.f);
		m_tEfxData.vDieTime = _vec2(1.f,2.f);
		m_tEfxData.vSpeed = _vec2(5.f,15.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,17.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(70.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(20.f,0.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(50.f,0.f);

		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,2.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,3.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,5.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,200.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.5f,50.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-100.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-60.f,60.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 100.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);

		m_tEfxData.isBillBoard = true;
		m_tEfxData.fLookDist = 10.f;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case SPRINTRUN3:
		m_tEfxData.iImgIdx = SPRINTRUN0 - SPRINTRUN0;
		m_tEfxData.vPositionOffSet = _vec3(5.f,1.f,5.f);
		m_tEfxData.fEmitInterval = 0.15f;
		m_tEfxData.fEmitDuration = 3.f;
		m_tEfxData.iParticleCount = 10;
		m_tEfxData.vGravity = _vec2(40.f,70.f);
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(3.f,3.f);
		m_tEfxData.vSpeed = _vec2(70.f,150.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,70.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,30.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,10.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(90.f,270.f);
		m_tEfxData.vSpreadRadius = _vec2(35.f,70.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(10.f,0.f);

		m_tEfxData.vStartSize = _vec2(5.f,10.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,10.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,20.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,30.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,200.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.5f,0.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-100.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-60.f,60.f);
		m_tEfxData.isDestAlphaOne = true;
		m_tEfxData.isLoop = true;
		m_tEfxData.vMyOffSetPos = _vec3(-100.f,-95.f,0.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 10.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.0f, 25.f, 0.f);

		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = 10.f;
		break;

	case SPRINTRUN4:
		m_tEfxData.iImgIdx = SPRINTRUN0 - SPRINTRUN0;
		m_tEfxData.vPositionOffSet = _vec3(0.f,0.f,0.f);
		m_tEfxData.fEmitInterval = 0.15f;
		m_tEfxData.fEmitDuration = 3.f;
		m_tEfxData.iParticleCount = 5;
		//m_tEfxData.vGravity = _vec2(40.f,70.f);
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(3.f,3.f);
		m_tEfxData.vSpeed = _vec2(150.f,180.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,30.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,20.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,10.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(90.f,270.f);
		m_tEfxData.vSpreadRadius = _vec2(50.f,90.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(10.f,0.f);

		m_tEfxData.vStartSize = _vec2(15.f,20.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,30.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,50.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,80.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,200.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.5f,20.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-100.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-60.f,60.f);
		m_tEfxData.isDestAlphaOne = true;
		m_tEfxData.isLoop = true;
		m_tEfxData.vMyOffSetPos = _vec3(-150.f,-305.f,0.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 10.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.0f, 25.f, 0.f);

		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = 10.f;
		break;

	case SPRINTRUN5:
		m_tEfxData.iImgIdx = SPRINTRUN0 - SPRINTRUN0;
		m_tEfxData.vPositionOffSet = _vec3(0.f,0.f,0.f);
		m_tEfxData.fEmitInterval = 0.02f;
		m_tEfxData.fEmitDuration = 0.6f;
		m_tEfxData.iParticleCount = 10;
		m_tEfxData.vGravity = _vec2(100.f,150.f);
		m_tEfxData.vColorMin = _vec3(20.f,20.f,20.f);
		m_tEfxData.vColorMax = _vec3(40.f,20.f,20.f);
		m_tEfxData.vDieTime = _vec2(3.f,3.f);
		m_tEfxData.vSpeed = _vec2(150.f,180.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,30.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,20.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,10.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(8.f,-8.f);
		m_tEfxData.vSpreadRadius = _vec2(80.f,100.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(-360.f,360.f);

		m_tEfxData.vStartSize = _vec2(0.5f,1.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,3.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,20.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,100.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,200.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.5f,20.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-100.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-60.f,60.f);
		m_tEfxData.isDestAlphaOne = false;
		m_tEfxData.isLoop = false;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,0.f,0.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 10.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.0f, 25.f, 0.f);

		m_tEfxData.isBillBoard = true;
		break;

	case WATERFALL0:
		m_tEfxData.iImgIdx = 2;
		m_tEfxData.vPositionOffSet = _vec3(0.f,0.f,0.f);
		m_tEfxData.fEmitInterval = 0.5f;
		m_tEfxData.fEmitDuration = 1.0f;
		m_tEfxData.iParticleCount = 1;
		//m_tEfxData.vGravity = _vec2(50.f,100.f);
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(100.f,0.f);
		/*m_tEfxData.vSpeed = _vec2(100.f,150.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,70.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,30.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,10.f,0.f);*/

		/*m_tEfxData.vSpreadAngle = _vec2(70.f,-70.f);
		m_tEfxData.vSpreadRadius = _vec2(50.f,0.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(20.f,-20.f);*/

		/*m_tEfxData.vStartSize = _vec2(50.f,100.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,10.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,20.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,30.f,0.f);*/
		m_tEfxData.vLength = _vec2(40.f,0.f);
		m_tEfxData.vWidth = _vec2(210.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,255.f,0.f);
		m_tEfxData.vStartRoll = _vec2(0.f,0.f);
		m_tEfxData.isDestAlphaOne = true;
		m_tEfxData.isLoop = true;
		m_tEfxData.vMyOffSetPos = _vec3(-70.f,-190.f,0.f);
	/*	m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 10.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.0f, 25.f, 0.f);*/

		m_tEfxData.isBillBoard = true;
		m_tEfxData.vUV = _vec2(1.f,0.f);
		//m_tEfxData.fLookDist = 10.f;
		break;

	case WATERFALL1:
		m_tEfxData.iImgIdx = 8;
		m_tEfxData.vPositionOffSet = _vec3(0.f,0.f,0.f);
		m_tEfxData.fEmitInterval = 0.02f;
		m_tEfxData.fEmitDuration = 0.6f;
		m_tEfxData.iParticleCount = 10;
		m_tEfxData.vGravity = _vec2(100.f,150.f);
		m_tEfxData.vColorMin = _vec3(255.f,255.f,255.f);
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(3.f,3.f);
		m_tEfxData.vSpeed = _vec2(150.f,180.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,30.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,20.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,10.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(8.f,-8.f);
		m_tEfxData.vSpreadRadius = _vec2(80.f,100.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(-360.f,360.f);

		m_tEfxData.vStartSize = _vec2(0.5f,1.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,3.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,20.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,100.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,200.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.5f,20.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-100.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-60.f,60.f);
		m_tEfxData.isDestAlphaOne = true;
		m_tEfxData.isLoop = false;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,0.f,0.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 10.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.0f, 25.f, 0.f);

		m_tEfxData.isBillBoard = true;
		break;

	case WATERFALL2:
		m_tEfxData.iImgIdx = 4;
		m_tEfxData.vPositionOffSet = _vec3(0.f,0.f,0.f);
		m_tEfxData.fEmitInterval = 0.6f;
		m_tEfxData.fEmitDuration = 1.0f;
		m_tEfxData.iParticleCount = 6;
		//m_tEfxData.vGravity = _vec2(50.f,100.f);
		m_tEfxData.vColorMax = _vec3(0.f,128.f,255.f);
		m_tEfxData.vDieTime = _vec2(100.f,0.f);
		/*m_tEfxData.vSpeed = _vec2(100.f,150.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,70.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,30.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,10.f,0.f);*/

		/*m_tEfxData.vSpreadAngle = _vec2(70.f,-70.f);
		m_tEfxData.vSpreadRadius = _vec2(50.f,0.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(20.f,-20.f);*/

		/*m_tEfxData.vStartSize = _vec2(50.f,100.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,10.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,20.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,30.f,0.f);*/
		m_tEfxData.vLength = _vec2(40.f,0.f);
		m_tEfxData.vWidth = _vec2(500.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,255.f,0.f);
		m_tEfxData.vStartRoll = _vec2(0.f,0.f);
		m_tEfxData.isDestAlphaOne = true;
		m_tEfxData.isLoop = true;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,300.f,0.f);
	/*	m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 10.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.0f, 25.f, 0.f);*/

		m_tEfxData.isBillBoard = true;
		m_tEfxData.vUV = _vec2(1.f,1.f);
		//m_tEfxData.fLookDist = 10.f;
		break;

	case WATERFALL3:
		m_tEfxData.iImgIdx = 8;
		m_tEfxData.vPositionOffSet = _vec3(0.f,0.f,0.f);
		m_tEfxData.fEmitInterval = 0.02f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 10;
		m_tEfxData.vGravity = _vec2(-30.f,-40.f);
		m_tEfxData.vColorMin = _vec3(255.f,255.f,255.f);
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(1.5f,1.5f);
		m_tEfxData.vSpeed = _vec2(150.f,180.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,30.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,20.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,10.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(90.1f,90.2f);
		m_tEfxData.vSpreadRadius = _vec2(50.f,60.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(-5.1f,5.1f);

		m_tEfxData.vStartSize = _vec2(0.5f,1.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,3.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,20.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,100.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,200.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.5f,20.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-100.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-60.f,60.f);
		m_tEfxData.isDestAlphaOne = true;
		m_tEfxData.isLoop = false;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,-3.f,0.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 10.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.0f, 25.f, 0.f);
		m_tEfxData.isDealing = true;

		//m_tEfxData.isAttached = true;
		m_tEfxData.isBillBoard = true;
		break;

	case WATERLINE0:
		m_tEfxData.iImgIdx = WATERLINE0 - WATERLINE0;
		m_tEfxData.vPositionOffSet = _vec3(0.f,0.f,0.f);
		m_tEfxData.fEmitInterval = 0.1f;
		m_tEfxData.fEmitDuration = 1.0f;
		m_tEfxData.iParticleCount = 1;
		m_tEfxData.vGravity = _vec2(10.f,30.f);
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(0.5f,0.7f);
		m_tEfxData.vSpeed = _vec2(50.f,70.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,30.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,20.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,10.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(30.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(10.f,0.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(10.f,0.f);

		m_tEfxData.vStartSize = _vec2(1.f,5.f);
		/*m_tEfxData.vSizeDelta[0] = _vec3(0.1f,10.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,20.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,30.f,0.f);*/
		//m_tEfxData.vLength = _vec2(40.f,0.f);
		//m_tEfxData.vWidth = _vec2(200.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,255.f,0.f);
		m_tEfxData.vStartRoll = _vec2(0.f,0.f);
		m_tEfxData.isDestAlphaOne = false;
		//m_tEfxData.isLoop = true;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,10.f,0.f);
	/*	m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 10.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.0f, 25.f, 0.f);*/

		m_tEfxData.isBillBoard = true;
		//m_tEfxData.isUVFlow = true;
		//m_tEfxData.fLookDist = 10.f;
		break;
	case EXPLOSION0:
		m_tEfxData.iImgIdx = EXPLOSION0 - EXPLOSION0;
		m_tEfxData.vPositionOffSet = _vec3(0.f,0.f,0.f);
		m_tEfxData.fEmitInterval = 0.05f;
		m_tEfxData.fEmitDuration = 0.12f;
		m_tEfxData.iParticleCount = 3;
		m_tEfxData.vGravity = _vec2(10.f,30.f);
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(0.2f,0.3f);
		m_tEfxData.vSpeed = _vec2(50.f,70.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,30.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,20.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,10.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(30.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(10.f,0.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(10.f,0.f);

		m_tEfxData.vStartSize = _vec2(0.5f,1.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,3.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,7.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,10.f,0.f);
		//m_tEfxData.vLength = _vec2(40.f,0.f);
		//m_tEfxData.vWidth = _vec2(200.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,0.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.isDestAlphaOne = true;
		//m_tEfxData.isLoop = true;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,2.5f,0.f);
	/*	m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 10.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.0f, 25.f, 0.f);*/

		m_tEfxData.isBillBoard = true;
		//m_tEfxData.isUVFlow = true;
		//m_tEfxData.fLookDist = 10.f;
		break;
	case EXPLOSION1:
		m_tEfxData.iImgIdx = EXPLOSION1 - EXPLOSION0;
		m_tEfxData.vPositionOffSet = _vec3(0.f,0.f,0.f);
		m_tEfxData.fEmitInterval = 0.05f;
		m_tEfxData.fEmitDuration = 0.12f;
		m_tEfxData.iParticleCount = 3;
		m_tEfxData.vGravity = _vec2(10.f,30.f);
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(0.2f,0.3f);
		m_tEfxData.vSpeed = _vec2(50.f,70.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,30.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,20.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,10.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(30.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(10.f,0.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(10.f,0.f);

		m_tEfxData.vStartSize = _vec2(0.5f,1.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,3.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,7.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,15.f,0.f);
		//m_tEfxData.vLength = _vec2(40.f,0.f);
		//m_tEfxData.vWidth = _vec2(200.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,0.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.isDestAlphaOne = true;
		//m_tEfxData.isLoop = true;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,2.5f,0.f);
	/*	m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 10.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.0f, 25.f, 0.f);*/

		m_tEfxData.isBillBoard = true;
		//m_tEfxData.isUVFlow = true;
		//m_tEfxData.fLookDist = 10.f;
		break;

	case EXPLOSION2:
		m_tEfxData.iImgIdx = EXPLOSION2 - EXPLOSION0;
		m_tEfxData.vPositionOffSet = _vec3(0.f,0.f,0.f);
		m_tEfxData.fEmitInterval = 0.05f;
		m_tEfxData.fEmitDuration = 0.12f;
		m_tEfxData.iParticleCount = 3;
		m_tEfxData.vGravity = _vec2(10.f,30.f);
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(0.2f,0.3f);
		m_tEfxData.vSpeed = _vec2(50.f,70.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,30.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,20.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,10.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(30.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(10.f,0.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(10.f,0.f);

		m_tEfxData.vStartSize = _vec2(0.5f,1.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,3.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,10.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,20.f,0.f);
		//m_tEfxData.vLength = _vec2(40.f,0.f);
		//m_tEfxData.vWidth = _vec2(200.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,0.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.isDestAlphaOne = true;
		//m_tEfxData.isLoop = true;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,2.5f,0.f);
	/*	m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 10.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.0f, 25.f, 0.f);*/

		m_tEfxData.isBillBoard = true;
		//m_tEfxData.isUVFlow = true;
		//m_tEfxData.fLookDist = 10.f;
		break;

	case SPRINTRUN6:
		m_tEfxData.iImgIdx = SPRINTRUN0 - SPRINTRUN0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.01f;
		m_tEfxData.fEmitDuration = 2.f;
		m_tEfxData.iParticleCount = 2;
		m_tEfxData.vColorMax = _vec3(200.f,200.f,200.f);
		m_tEfxData.vDieTime = _vec2(3.f,4.f);
		m_tEfxData.vSpeed = _vec2(5.f,15.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,17.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(70.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(20.f,0.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(50.f,0.f);

		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,5.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,10.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,15.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,200.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.5f,-500.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-1000.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-60.f,60.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 100.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);

		m_tEfxData.isBillBoard = true;
		m_tEfxData.fLookDist = -100.f;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case WAVEREADY0:
		m_tEfxData.iImgIdx = WAVEREADY0 - WAVEREADY0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.04f;
		m_tEfxData.fEmitDuration = 0.8f;
		m_tEfxData.iParticleCount = 1;
		m_tEfxData.vColorMax = _vec3(0.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(1.5f,1.5f);
		m_tEfxData.vSpeed = _vec2(60.f,0.f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,17.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(0.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(6.1f,10.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(50.f,0.f);

		m_tEfxData.vStartSize = _vec2(5.f,15.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,0.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,10.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,0.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.3f,450.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-500.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		//m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		//m_tEfxData.vRollDelta[1] = _vec3(0.5f, 100.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);

		m_tEfxData.vMyOffSetPos = _vec3(0.f,3.f,0.f);
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = 150.f;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case WAVEREADY1:
		m_tEfxData.iImgIdx = WAVEREADY1 - WAVEREADY0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.07f;
		m_tEfxData.fEmitDuration = 0.8f;
		m_tEfxData.iParticleCount = 1;
		m_tEfxData.vColorMax = _vec3(251.f,21.f,28.f);
		m_tEfxData.vDieTime = _vec2(1.5f,1.5f);
		m_tEfxData.vSpeed = _vec2(1.f,0.f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,17.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(0.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(5.1f,25.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(50.f,0.f);

		m_tEfxData.vStartSize = _vec2(17.f,24.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,0.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,3.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,0.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.3f,450.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-500.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f, 8.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = true;
		m_tEfxData.isBillBoard = false;
		//m_tEfxData.fLookDist = 150.f;

		//m_tEfxData.fLookDist = 1.f;
		m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case WAVEREADY2:
		m_tEfxData.iImgIdx = WAVEREADY0 - WAVEREADY0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.03f;
		m_tEfxData.fEmitDuration = 1.f;
		m_tEfxData.iParticleCount = 1;
		m_tEfxData.vGravity = _vec2(35.f,35.5f);
		m_tEfxData.vColorMax = _vec3(35.f,11.f,10.f);
		m_tEfxData.vDieTime = _vec2(1.5f,1.5f);
		m_tEfxData.vSpeed = _vec2(7.f,15.f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,17.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(5.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(25.1f,40.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(-360.f,360.f);

		m_tEfxData.vStartSize = _vec2(7.f,10.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,0.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,-3.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,0.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.3f,450.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-500.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f, 20.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = false;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = 150.f;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case WAVEREADY3:
		m_tEfxData.iImgIdx = WAVEREADY2 - WAVEREADY0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.02f;
		m_tEfxData.fEmitDuration = 0.3f;
		m_tEfxData.iParticleCount = 2;
		//m_tEfxData.vGravity = _vec2(35.f,35.5f);
		m_tEfxData.vColorMax = _vec3(0.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(1.5f,1.5f);
		m_tEfxData.vSpeed = _vec2(1.f,1.1f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,17.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(1.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(1.0f,1.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(-360.f,360.f);

		m_tEfxData.vStartSize = _vec2(11.f,16.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,2.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,11.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,0.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.3f,500.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-500.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f, 20.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = false;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = 150.f;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case WAVEREADY4:
		m_tEfxData.iImgIdx = 3;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.1f;
		m_tEfxData.fEmitDuration = 0.2f;
		m_tEfxData.iParticleCount = 1;
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(1.5f,1.5f);
		m_tEfxData.vSpeed = _vec2(1.f,0.f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,17.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(0.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(5.1f,25.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(50.f,0.f);

		m_tEfxData.vStartSize = _vec2(14.f,19.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.1f,0.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,3.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,0.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.3f,450.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-500.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f, 8.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = true;
		m_tEfxData.isBillBoard = false;
		//m_tEfxData.fLookDist = 150.f;

		//m_tEfxData.fLookDist = 1.f;
		m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case WAVEHIT0:
		m_tEfxData.iImgIdx = WAVEHIT0 - WAVEHIT0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.02f;
		m_tEfxData.fEmitDuration = 0.2f;
		m_tEfxData.iParticleCount = 1;
		//m_tEfxData.vGravity = _vec2(35.f,35.5f);
		m_tEfxData.vColorMax = _vec3(0.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.1f,0.2f);
		m_tEfxData.vSpeed = _vec2(2.f,2.1f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,17.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(30.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(20.0f,1.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(30.f,0.f);

		m_tEfxData.vStartSize = _vec2(1.f,18.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.01f,2.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.12f,37.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,60.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,250.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,-300.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f, 40.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = false;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = -100.f;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,3.f,0.f);

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case WAVEHIT1:
		m_tEfxData.iImgIdx = WAVEHIT1 - WAVEHIT0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.03f;
		m_tEfxData.fEmitDuration = 0.2f;
		m_tEfxData.iParticleCount = 1;
		//m_tEfxData.vGravity = _vec2(35.f,35.5f);
		m_tEfxData.vColorMax = _vec3(0.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.3f,0.5f);
		m_tEfxData.vSpeed = _vec2(-2.f,0.1f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,17.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(0.1f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(20.0f,170.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(30.f,0.f);

		m_tEfxData.vStartSize = _vec2(18.f,31.f);
		/*m_tEfxData.vSizeDelta[0] = _vec3(0.01f,2.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.12f,37.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,60.f,0.f);*/

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,250.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,-300.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		//m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		//m_tEfxData.vRollDelta[1] = _vec3(1.f, 40.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = false;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = -100.f;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,3.f,0.f);

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case WAVEHIT2:
		m_tEfxData.iImgIdx = WAVEHIT2 - WAVEHIT0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.02f;
		m_tEfxData.fEmitDuration = 0.1f;
		m_tEfxData.iParticleCount = 1;
		//m_tEfxData.vGravity = _vec2(35.f,35.5f);
		m_tEfxData.vColorMax = _vec3(30.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.3f,0.5f);
		m_tEfxData.vSpeed = _vec2(-2.f,0.1f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,17.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(0.1f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(10.0f,130.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(10.f,0.f);

		m_tEfxData.vStartSize = _vec2(8.f,21.f);
		/*m_tEfxData.vSizeDelta[0] = _vec3(0.01f,2.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.12f,37.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,60.f,0.f);*/

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,180.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,-300.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		//m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		//m_tEfxData.vRollDelta[1] = _vec3(1.f, 40.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = false;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = -100.f;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,3.f,0.f);

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case WAVEHIT3:
		m_tEfxData.iImgIdx = 7;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.1f;
		m_tEfxData.fEmitDuration = 0.2f;
		m_tEfxData.iParticleCount = 1;
		//m_tEfxData.vGravity = _vec2(35.f,35.5f);
		m_tEfxData.vColorMax = _vec3(255.f,80.f,70.f);
		m_tEfxData.vDieTime = _vec2(0.1f,0.15f);
		m_tEfxData.vSpeed = _vec2(1.f,1.f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,17.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(0.1f,0.1f);
		m_tEfxData.vSpreadRadius = _vec2(0.1f,0.1f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(0.1f,0.1f);

		//m_tEfxData.vStartSize = _vec2(8.f,21.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.01f,25.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.2f,129.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(0.3f,91.f,0.f);
		m_tEfxData.vSizeDelta[3] = _vec3(0.4f,1.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,250.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,-300.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vMyOffSetPos = _vec3(0.f,3.f,0.f);


		//m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		//m_tEfxData.vRollDelta[1] = _vec3(1.f, 40.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = true;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = -100.f;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case WAVEHIT4:
		m_tEfxData.iImgIdx = 8;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.02f;
		m_tEfxData.fEmitDuration = 0.1f;
		m_tEfxData.iParticleCount = 1;
		//m_tEfxData.vGravity = _vec2(35.f,35.5f);
		m_tEfxData.vColorMax = _vec3(255.f,55.f,20.f);
		m_tEfxData.vDieTime = _vec2(0.1f,0.2f);
		m_tEfxData.vSpeed = _vec2(0.1f,0.1f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,17.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(0.1f,0.1f);
		m_tEfxData.vSpreadRadius = _vec2(0.1f,0.1f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(0.1f,0.1f);

		m_tEfxData.vStartSize = _vec2(3.f,25.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.01f,0.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.1f,51.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,250.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,-300.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vMyOffSetPos = _vec3(0.f,3.f,0.f);


		//m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		//m_tEfxData.vRollDelta[1] = _vec3(1.f, 40.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = true;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = -100.f;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

		case WAVEHIT5:
		m_tEfxData.iImgIdx = WAVEHIT0 - WAVEHIT0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.02f;
		m_tEfxData.fEmitDuration = 0.2f;
		m_tEfxData.iParticleCount = 1;
		//m_tEfxData.vGravity = _vec2(35.f,35.5f);
		m_tEfxData.vColorMax = _vec3(0.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.1f,0.2f);
		m_tEfxData.vSpeed = _vec2(2.f,2.1f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,17.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(30.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(20.0f,1.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(30.f,0.f);

		m_tEfxData.vStartSize = _vec2(1.f,8.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.01f,2.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.12f,17.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,30.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,250.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,-300.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f, 40.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = false;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = -100.f;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,3.f,0.f);

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case WAVEHIT6:
		m_tEfxData.iImgIdx = WAVEHIT1 - WAVEHIT0;
		m_tEfxData.vPositionOffSet = _vec3(2.f,2.f,2.f);
		m_tEfxData.fEmitInterval = 0.01f;
		m_tEfxData.fEmitDuration = 0.15f;
		m_tEfxData.iParticleCount = 10;
		//m_tEfxData.vGravity = _vec2(35.f,35.5f);
		m_tEfxData.vColorMax = _vec3(0.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.3f,0.5f);
		m_tEfxData.vSpeed = _vec2(-2.f,0.1f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,17.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(0.1f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(20.0f,170.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(30.f,0.f);

		m_tEfxData.vStartSize = _vec2(10.f,20.f);
		/*m_tEfxData.vSizeDelta[0] = _vec3(0.01f,2.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.12f,37.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,60.f,0.f);*/

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,250.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,-300.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		//m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		//m_tEfxData.vRollDelta[1] = _vec3(1.f, 40.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = false;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = -100.f;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,3.f,0.f);
		m_tEfxData.isLoop = true;
		m_tEfxData.iProbabilityForLoop = 20;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case WAVEHIT7:
		m_tEfxData.iImgIdx = WAVEHIT2 - WAVEHIT0;
		m_tEfxData.vPositionOffSet = _vec3(2.f,2.f,2.f);
		m_tEfxData.fEmitInterval = 0.01f;
		m_tEfxData.fEmitDuration = 0.1f;
		m_tEfxData.iParticleCount = 10;
		//m_tEfxData.vGravity = _vec2(35.f,35.5f);
		m_tEfxData.vColorMax = _vec3(30.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.3f,0.5f);
		m_tEfxData.vSpeed = _vec2(-2.f,0.1f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,17.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(0.1f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(10.0f,130.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(10.f,0.f);

		m_tEfxData.vStartSize = _vec2(6.f,13.f);
		/*m_tEfxData.vSizeDelta[0] = _vec3(0.01f,2.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.12f,37.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,60.f,0.f);*/

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,180.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,-300.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		//m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		//m_tEfxData.vRollDelta[1] = _vec3(1.f, 40.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = false;
		m_tEfxData.isBillBoard = true;
		m_tEfxData.isLoop = true;
		m_tEfxData.iProbabilityForLoop = 20;
		//m_tEfxData.fLookDist = -100.f;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,3.f,0.f);

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case WAVEHIT8: //blunt
		m_tEfxData.iImgIdx = 10;
		m_tEfxData.vPositionOffSet = _vec3(5.f,5.f,5.f);
		m_tEfxData.fEmitInterval = 0.01f;
		m_tEfxData.fEmitDuration = 0.1f;
		m_tEfxData.iParticleCount = 5;
		//m_tEfxData.vGravity = _vec2(35.f,35.5f);
		m_tEfxData.vColorMin = _vec3(70.f,50.f,60.f);
		m_tEfxData.vColorMax = _vec3(200.f,50.f,60.f);
		m_tEfxData.vDieTime = _vec2(0.4f,0.4f);
		m_tEfxData.vSpeed = _vec2(50.f,100.1f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,50.1f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(1.f,65.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(0.1f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(10.0f,30.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(360.f,0.f);

		//m_tEfxData.vStartSize = _vec2(6.f,13.f);
		/*m_tEfxData.vSizeDelta[0] = _vec3(0.01f,2.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.12f,37.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,60.f,0.f);*/
		m_tEfxData.vLength = _vec2(15.f,21.f);
		m_tEfxData.vWidth = _vec2(5.f,7.f);
		m_tEfxData.vWidthDelta[0] = _vec2(0.1f,10.f);
		m_tEfxData.vWidthDelta[1] = _vec2(1.0f,50.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,180.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,-300.f,0.f);
		//m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		//m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		//m_tEfxData.vRollDelta[1] = _vec3(1.f, 40.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = false;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.isLoop = true;
		//m_tEfxData.iProbabilityForLoop = 20;
		//m_tEfxData.fLookDist = -100.f;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,0.f,0.f);
		//m_tEfxData.fLookDist = 150.f;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case WAVEHIT9: //blunt
		m_tEfxData.iImgIdx = 11;
		m_tEfxData.vPositionOffSet = _vec3(5.f,5.f,5.f);
		m_tEfxData.fEmitInterval = 0.01f;
		m_tEfxData.fEmitDuration = 0.1f;
		m_tEfxData.iParticleCount = 20;
		//m_tEfxData.vGravity = _vec2(35.f,35.5f);
		m_tEfxData.vColorMin = _vec3(10.f,10.f,10.f);
		m_tEfxData.vColorMax = _vec3(20.f,10.f,10.f);
		m_tEfxData.vDieTime = _vec2(0.4f,0.4f);
		m_tEfxData.vSpeed = _vec2(50.f,100.1f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,50.1f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(1.f,65.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(50.1f,-50.f);
		m_tEfxData.vSpreadRadius = _vec2(10.0f,30.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(-180.f,180.f);

		//m_tEfxData.vStartSize = _vec2(6.f,13.f);
		/*m_tEfxData.vSizeDelta[0] = _vec3(0.01f,2.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.12f,37.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,60.f,0.f);*/
		m_tEfxData.vLength = _vec2(15.f,21.f);
		m_tEfxData.vWidth = _vec2(5.f,7.f);
		m_tEfxData.vWidthDelta[0] = _vec2(0.1f,10.f);
		m_tEfxData.vWidthDelta[1] = _vec2(1.0f,50.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,180.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,-300.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-60.f,60.f);
		//m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		//m_tEfxData.vRollDelta[1] = _vec3(1.f, 40.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = false;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.isLoop = true;
		//m_tEfxData.iProbabilityForLoop = 20;
		//m_tEfxData.fLookDist = -100.f;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,0.f,0.f);
		//m_tEfxData.fLookDist = 150.f;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case WAVEHIT10:
		m_tEfxData.iImgIdx = WAVEHIT0 - WAVEHIT0;
		m_tEfxData.vPositionOffSet = _vec3(5.f,5.f,5.f);
		m_tEfxData.fEmitInterval = 0.02f;
		m_tEfxData.fEmitDuration = 0.1f;
		m_tEfxData.iParticleCount = 2;
		//m_tEfxData.vGravity = _vec2(35.f,35.5f);
		m_tEfxData.vColorMax = _vec3(0.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.5f,0.8f);
		m_tEfxData.vSpeed = _vec2(2.f,2.1f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,17.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(30.f,-30.f);
		m_tEfxData.vSpreadRadius = _vec2(20.0f,1.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(30.f,0.f);

		m_tEfxData.vStartSize = _vec2(1.f,18.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.01f,2.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.12f,107.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,150.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,250.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,-300.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		//m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		//m_tEfxData.vRollDelta[1] = _vec3(1.f, 40.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = false;
		//m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = -100.f;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,3.f,0.f);

		//m_tEfxData.fLookDist = 1.f;
		m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case WAVEHIT11: // player snatch
		m_tEfxData.iImgIdx = WAVEHIT0 - WAVEHIT0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,2.f,1.f);
		m_tEfxData.fEmitInterval = 0.05f;
		m_tEfxData.fEmitDuration = 1.f;
		m_tEfxData.iParticleCount = 2;
		//m_tEfxData.vGravity = _vec2(35.f,35.5f);
		m_tEfxData.vColorMax = _vec3(0.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(1.5f,2.f);
		m_tEfxData.vSpeed = _vec2(2.f,2.1f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,17.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(30.f,-30.f);
		m_tEfxData.vSpreadRadius = _vec2(20.0f,25.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(30.f,-30.f);

		m_tEfxData.vStartSize = _vec2(1.f,3.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.01f,2.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.12f,7.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,12.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,250.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,-700.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f, 360.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = false;
		//m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = -100.f;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,3.f,0.f);

		//m_tEfxData.fLookDist = 1.f;
		m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

		case WAVEHIT12://blunt
		m_tEfxData.iImgIdx = 12;
		m_tEfxData.vPositionOffSet = _vec3(5.f,5.f,5.f);
		m_tEfxData.fEmitInterval = 0.05f;
		m_tEfxData.fEmitDuration = 0.15f;
		m_tEfxData.iParticleCount = 1;
		//m_tEfxData.vGravity = _vec2(35.f,35.5f);
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(0.8f,0.8f);
		m_tEfxData.vSpeed = _vec2(50.f,100.1f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,50.1f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(1.f,65.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(0.1f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(10.0f,30.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(360.f,0.f);

		//m_tEfxData.vStartSize = _vec2(6.f,13.f);
		/*m_tEfxData.vSizeDelta[0] = _vec3(0.01f,2.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.12f,37.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,60.f,0.f);*/
		m_tEfxData.vLength = _vec2(8.f,10.f);
		m_tEfxData.vWidth = _vec2(1.f,2.f);
		m_tEfxData.vWidthDelta[0] = _vec2(0.1f,8.f);
		m_tEfxData.vWidthDelta[1] = _vec2(1.0f,30.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,180.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,-300.f,0.f);
		//m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		//m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		//m_tEfxData.vRollDelta[1] = _vec3(1.f, 40.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = true;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.isLoop = true;
		//m_tEfxData.iProbabilityForLoop = 20;
		//m_tEfxData.fLookDist = -100.f;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,0.f,0.f);
		m_tEfxData.fLookDist = 50.f;
		m_tEfxData.isDealing = true;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case WAVEHIT13: //blunt
		m_tEfxData.iImgIdx = 12;
		m_tEfxData.vPositionOffSet = _vec3(5.f,5.f,5.f);
		m_tEfxData.fEmitInterval = 0.05f;
		m_tEfxData.fEmitDuration = 0.2f;
		m_tEfxData.iParticleCount = 1;
		//m_tEfxData.vGravity = _vec2(35.f,35.5f);
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(0.8f,0.8f);
		m_tEfxData.vSpeed = _vec2(50.f,100.1f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,50.1f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(1.f,65.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(50.1f,-50.f);
		m_tEfxData.vSpreadRadius = _vec2(10.0f,30.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(-180.f,180.f);

		//m_tEfxData.vStartSize = _vec2(6.f,13.f);
		/*m_tEfxData.vSizeDelta[0] = _vec3(0.01f,2.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.12f,37.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,60.f,0.f);*/
		m_tEfxData.vLength = _vec2(8.f,10.f);
		m_tEfxData.vWidth = _vec2(1.f,2.f);
		m_tEfxData.vWidthDelta[0] = _vec2(0.1f,8.f);
		m_tEfxData.vWidthDelta[1] = _vec2(1.0f,30.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,180.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,-300.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-5.f,5.f);
		//m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		//m_tEfxData.vRollDelta[1] = _vec3(1.f, 40.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = true;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.isLoop = true;
		//m_tEfxData.iProbabilityForLoop = 20;
		//m_tEfxData.fLookDist = -100.f;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,0.f,0.f);
		m_tEfxData.fLookDist = 50.f;

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case KADANEXPLOSION0:
		m_tEfxData.iImgIdx = KADANEXPLOSION0 - KADANEXPLOSION0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.2f;
		m_tEfxData.fEmitDuration = 1.f;
		m_tEfxData.iParticleCount = 1;
		//m_tEfxData.vGravity = _vec2(35.f,35.5f);
		m_tEfxData.vColorMax = _vec3(122.f,42.f,42.f);
		m_tEfxData.vDieTime = _vec2(0.2f,0.3f);
		m_tEfxData.vSpeed = _vec2(1.f,1.1f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,17.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(0.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(0.0f,0.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(0.f,0.f);

		m_tEfxData.vStartSize = _vec2(3.f,15.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.01f,7.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,100.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,0.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.4f,400.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-300.f,0.f);
		//m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		//m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		//m_tEfxData.vRollDelta[1] = _vec3(1.f, 40.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = false;
		//m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = -100.f;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,3.f,0.f);

		//m_tEfxData.fLookDist = 1.f;
		m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case KADANEXPLOSION1:
		m_tEfxData.iImgIdx = KADANEXPLOSION1 - KADANEXPLOSION0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.02f;
		m_tEfxData.fEmitDuration = 0.1f;
		m_tEfxData.iParticleCount = 2;
		//m_tEfxData.vGravity = _vec2(35.f,35.5f);
		m_tEfxData.vColorMax = _vec3(20.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.2f,0.3f);
		m_tEfxData.vSpeed = _vec2(120.f,280.1f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,17.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(0.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(30.f,70.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(-360.f,360.f);

		m_tEfxData.vStartSize = _vec2(7.f,20.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.01f,3.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,11.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,0.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.4f,400.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-300.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f, 10.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = false;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = -100.f;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,3.f,0.f);

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case KADANEXPLOSION2:
		m_tEfxData.iImgIdx = KADANEXPLOSION2 - KADANEXPLOSION0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.02f;
		m_tEfxData.fEmitDuration = 0.2f;
		m_tEfxData.iParticleCount = 3;
		m_tEfxData.vGravity = _vec2(85.f,100.5f);
		m_tEfxData.vColorMax = _vec3(20.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.8f,1.3f);
		m_tEfxData.vSpeed = _vec2(50.f,60.1f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,17.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(30.f,-30.f);
		m_tEfxData.vSpreadRadius = _vec2(40.f,60.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(-360.f,360.f);

		m_tEfxData.vStartSize = _vec2(7.f,10.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.01f,3.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,11.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,0.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.4f,400.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-300.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f, 10.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = false;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = -100.f;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,3.f,0.f);

		//m_tEfxData.fLookDist = 1.f;
		//m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case KADANEXPLOSION3:
		m_tEfxData.iImgIdx = KADANEXPLOSION3 - KADANEXPLOSION0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.02f;
		m_tEfxData.fEmitDuration = 0.1f;
		m_tEfxData.iParticleCount = 3;
		//m_tEfxData.vGravity = _vec2(85.f,100.5f);
		m_tEfxData.vColorMax = _vec3(0.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.6f,0.8f);
		m_tEfxData.vSpeed = _vec2(1.f,1.f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,1.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,1.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(1.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(1.f,0.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(-360.f,360.f);

		m_tEfxData.vStartSize = _vec2(11.f,23.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.01f,2.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,61.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,250.f,0.f);
		//m_tEfxData.vAlphaDelta[1] = _vec3(0.4f,400.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,-400.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f, 40.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = false;
		//m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = -100.f;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,3.f,0.f);

		//m_tEfxData.fLookDist = 1.f;
		m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case KADANEXPLOSION4:
		m_tEfxData.iImgIdx = KADANEXPLOSION4 - KADANEXPLOSION0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.08f;
		m_tEfxData.fEmitDuration = 1.f;
		m_tEfxData.iParticleCount = 2;
		//m_tEfxData.vGravity = _vec2(85.f,100.5f);
		m_tEfxData.vColorMax = _vec3(30.f,10.f,10.f);
		m_tEfxData.vDieTime = _vec2(0.6f,0.9f);
		m_tEfxData.vSpeed = _vec2(2.f,0.f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,1.f,0.f);
		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,1.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(1.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(1.f,0.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(-360.f,360.f);

		m_tEfxData.vStartSize = _vec2(2.f,4.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.01f,1.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,51.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,0.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.4f,400.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-400.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 5.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f, 20.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = false;
		m_tEfxData.isBillBoard = false;
		//m_tEfxData.fLookDist = -100.f;
		//m_tEfxData.vMyOffSetPos = _vec3(0.f,3.f,0.f);

		//m_tEfxData.fLookDist = 1.f;
		m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case KADANEXPLOSION5:
		m_tEfxData.iImgIdx = KADANEXPLOSION5 - KADANEXPLOSION0;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.1f;
		m_tEfxData.fEmitDuration = 1.f;
		m_tEfxData.iParticleCount = 1;
		//m_tEfxData.vGravity = _vec2(85.f,100.5f);
		m_tEfxData.vColorMax = _vec3(40.f,20.f,15.f);
		m_tEfxData.vDieTime = _vec2(1.7f,1.3f);
		m_tEfxData.vSpeed = _vec2(1.f,0.f);
		//m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,1.f,0.f);

		//m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,1.f,0.f);
		//m_tEfxData.vSpeedDelta[2] = _vec3(1.f,0.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(1.f,0.f);
		m_tEfxData.vSpreadRadius = _vec2(1.f,0.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(-360.f,360.f);

		m_tEfxData.vStartSize = _vec2(8.f,12.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.01f,1.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,75.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,0.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.4f,200.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-400.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		//m_tEfxData.vRollDelta[0] = _vec3(0.1f, 5.1f , 0.f);
		//m_tEfxData.vRollDelta[1] = _vec3(1.f, 20.f, 0.f);
		//m_tEfxData.vRollDelta[2] = _vec3(1.0f, 250.f, 0.f);
		m_tEfxData.isDestAlphaOne = false;
		m_tEfxData.isBillBoard = false;
		//m_tEfxData.fLookDist = -100.f;
		//m_tEfxData.vMyOffSetPos = _vec3(0.f,3.f,0.f);

		//m_tEfxData.fLookDist = 1.f;
		m_tEfxData.vOffSetAngle = _vec3(D3DX_PI/2.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;


	case EP10_0:
		m_tEfxData.iImgIdx = 1;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.02f;
		m_tEfxData.fEmitDuration = 3.3f;
		m_tEfxData.iParticleCount = 1;
		m_tEfxData.vGravity = _vec2(3.f,4.f);
		m_tEfxData.vColorMax = _vec3(30.f,0.f,0.f);
		m_tEfxData.vDieTime = _vec2(0.6f,0.8f);
		m_tEfxData.vSpeed = _vec2(1.f,0.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,1.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,1.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,1.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(-360.f,360.f);
		m_tEfxData.vSpreadRadius = _vec2(0.1f,5.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(-360.f,360.f);

		m_tEfxData.vStartSize = _vec2(3.f,1.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.01f,-1.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,1.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,-1.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.5f,0.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-400.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 50.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f, 80.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.0f, 100.f, 0.f);
		m_tEfxData.isDestAlphaOne = true;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = -100.f;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,5.f,0.f);

		//m_tEfxData.fLookDist = 1.f;
		m_tEfxData.vOffSetAngle = _vec3(0.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;
		break;

	case EP10_1:
		m_tEfxData.iImgIdx = 1;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.02f;
		m_tEfxData.fEmitDuration = 1.f;
		m_tEfxData.iParticleCount = 4;
		m_tEfxData.vGravity = _vec2(100.f,110.f);
		m_tEfxData.vColorMax = _vec3(255.f,255.f,255.f);
		m_tEfxData.vDieTime = _vec2(0.1f,0.2f);
		m_tEfxData.vSpeed = _vec2(170.f,200.f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,1.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(0.3f,10.f,0.f);
		m_tEfxData.vSpeedDelta[2] = _vec3(1.f,20.f,0.f);

		/*m_tEfxData.vSpreadAngle = _vec2(-360.f,360.f);
		m_tEfxData.vSpreadRadius = _vec2(0.1f,5.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(-360.f,360.f);*/

		/*m_tEfxData.vStartSize = _vec2(3.f,1.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.01f,-1.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,1.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(1.f,-1.f,0.f);*/

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,255.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.5f,0.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-400.f,0.f);
		/*m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 50.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f, 80.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.0f, 100.f, 0.f);*/
		m_tEfxData.isDestAlphaOne = true;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = -100.f;
		m_tEfxData.vMyOffSetPos = _vec3(0.f,5.f,0.f);
		//m_tEfxData.isLoop = true;

		//m_tEfxData.fLookDist = 1.f;
		m_tEfxData.vOffSetAngle = _vec3(0.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;

		m_tEfxData.vWidth = _vec2(16.f,25.f);
		m_tEfxData.vWidthDelta[0] = _vec2(0.1f,0.1f);
		m_tEfxData.vWidthDelta[1] = _vec2(1.f,-11.f);
		m_tEfxData.vLength = _vec2(1.1f,1.2f);
		m_tEfxData.vLengthDelta[0] = _vec2(0.1f,0.1f);
		m_tEfxData.vLengthDelta[1] = _vec2(1.f,0.4f);
		break;

	case EP10_HEART0:
		m_tEfxData.iImgIdx = 2;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.1f;
		m_tEfxData.fEmitDuration = 1.f;
		m_tEfxData.iParticleCount = 16;
		m_tEfxData.vGravity = _vec2(7.f,7.f);
		m_tEfxData.vColorMax = _vec3(30.f,5.f,5.f);
		m_tEfxData.vDieTime = _vec2(0.6f,1.f);
		m_tEfxData.vSpeed = _vec2(0.1f,1.1f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,1.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(1.f,2.f,0.f);

		/*m_tEfxData.vSpreadAngle = _vec2(-360.f,360.f);
		m_tEfxData.vSpreadRadius = _vec2(0.1f,5.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(-360.f,360.f);*/

		m_tEfxData.vStartSize = _vec2(1.f,3.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.01f,0.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,3.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,100.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.2f,300.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-400.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f, 10.f, 0.f);
		m_tEfxData.isDestAlphaOne = true;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = -100.f;
		//m_tEfxData.isLoop = true;

		//m_tEfxData.fLookDist = 1.f;
		m_tEfxData.vOffSetAngle = _vec3(0.f,0.f,0.f);
		m_tEfxData.isLoop = true;
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;

		/*m_tEfxData.vWidth = _vec2(16.f,25.f);
		m_tEfxData.vWidthDelta[0] = _vec2(0.1f,0.1f);
		m_tEfxData.vWidthDelta[1] = _vec2(1.f,-11.f);
		m_tEfxData.vLength = _vec2(1.1f,1.2f);
		m_tEfxData.vLengthDelta[0] = _vec2(0.1f,0.1f);
		m_tEfxData.vLengthDelta[1] = _vec2(1.f,0.4f);*/
		break;

	case EP10_HEART1:
		m_tEfxData.iImgIdx = 3;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.1f;
		m_tEfxData.fEmitDuration = 1.f;
		m_tEfxData.iParticleCount = 4;
		m_tEfxData.vGravity = _vec2(7.f,7.f);
		m_tEfxData.vColorMax = _vec3(30.f,5.f,5.f);
		m_tEfxData.vDieTime = _vec2(0.6f,1.f);
		m_tEfxData.vSpeed = _vec2(0.1f,1.1f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,1.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(1.f,2.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(120.f,-120.f);
		m_tEfxData.vSpreadRadius = _vec2(0.1f,1.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(-360.f,360.f);

		m_tEfxData.vStartSize = _vec2(1.f,3.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.01f,0.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,3.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,100.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.2f,300.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-400.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f, 10.f, 0.f);
		m_tEfxData.isDestAlphaOne = true;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = -100.f;
		//m_tEfxData.isLoop = true;

		//m_tEfxData.fLookDist = 1.f;
		m_tEfxData.vOffSetAngle = _vec3(0.f,0.f,0.f);
		m_tEfxData.isLoop = true;
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;

		/*m_tEfxData.vWidth = _vec2(16.f,25.f);
		m_tEfxData.vWidthDelta[0] = _vec2(0.1f,0.1f);
		m_tEfxData.vWidthDelta[1] = _vec2(1.f,-11.f);
		m_tEfxData.vLength = _vec2(1.1f,1.2f);
		m_tEfxData.vLengthDelta[0] = _vec2(0.1f,0.1f);
		m_tEfxData.vLengthDelta[1] = _vec2(1.f,0.4f);*/
		break;


	case EP10_HEART2:
		m_tEfxData.iImgIdx = 4;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.1f;
		m_tEfxData.fEmitDuration = 1.f;
		m_tEfxData.iParticleCount = 4;
		//m_tEfxData.vGravity = _vec2(7.f,7.f);
		m_tEfxData.vColorMax = _vec3(60.f,5.f,5.f);
		m_tEfxData.vDieTime = _vec2(0.6f,0.8f);
		m_tEfxData.vSpeed = _vec2(0.1f,1.1f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,1.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(1.f,2.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(360.f,-360.f);
		m_tEfxData.vSpreadRadius = _vec2(0.1f,3.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(-360.f,360.f);

		m_tEfxData.vStartSize = _vec2(1.f,2.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.01f,-1.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,1.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(0.5f,-1.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,100.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.2f,300.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-400.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 30.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 50.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.f, 60.f, 0.f);
		m_tEfxData.isDestAlphaOne = false;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = -100.f;
		//m_tEfxData.isLoop = true;

		//m_tEfxData.fLookDist = 1.f;
		m_tEfxData.vOffSetAngle = _vec3(0.f,0.f,0.f);
		m_tEfxData.isLoop = true;
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;

		/*m_tEfxData.vWidth = _vec2(16.f,25.f);
		m_tEfxData.vWidthDelta[0] = _vec2(0.1f,0.1f);
		m_tEfxData.vWidthDelta[1] = _vec2(1.f,-11.f);
		m_tEfxData.vLength = _vec2(1.1f,1.2f);
		m_tEfxData.vLengthDelta[0] = _vec2(0.1f,0.1f);
		m_tEfxData.vLengthDelta[1] = _vec2(1.f,0.4f);*/
		break;

	case EP10_HEART3:
		m_tEfxData.iImgIdx = 5;
		m_tEfxData.vPositionOffSet = _vec3(1.f,1.f,1.f);
		m_tEfxData.fEmitInterval = 0.02f;
		m_tEfxData.fEmitDuration = 1.f;
		m_tEfxData.iParticleCount = 2;
		//m_tEfxData.vGravity = _vec2(7.f,7.f);
		m_tEfxData.vColorMax = _vec3(60.f,5.f,5.f);
		m_tEfxData.vDieTime = _vec2(0.6f,0.8f);
		m_tEfxData.vSpeed = _vec2(0.1f,1.1f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,1.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(1.f,2.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(360.f,-360.f);
		m_tEfxData.vSpreadRadius = _vec2(0.1f,3.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(-360.f,360.f);

		m_tEfxData.vStartSize = _vec2(2.f,4.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.01f,-1.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,1.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(0.5f,-1.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,200.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,-400.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 30.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 50.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.f, 60.f, 0.f);
		m_tEfxData.isDestAlphaOne = false;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = -100.f;
		m_tEfxData.isLoop = true;
		//m_tEfxData.isLoop = true;

		//m_tEfxData.fLookDist = 1.f;
		m_tEfxData.vOffSetAngle = _vec3(0.f,0.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;

		/*m_tEfxData.vWidth = _vec2(16.f,25.f);
		m_tEfxData.vWidthDelta[0] = _vec2(0.1f,0.1f);
		m_tEfxData.vWidthDelta[1] = _vec2(1.f,-11.f);
		m_tEfxData.vLength = _vec2(1.1f,1.2f);
		m_tEfxData.vLengthDelta[0] = _vec2(0.1f,0.1f);
		m_tEfxData.vLengthDelta[1] = _vec2(1.f,0.4f);*/
		break;

	case EP10_HEART4:
		m_tEfxData.iImgIdx = 2;
		m_tEfxData.vPositionOffSet = _vec3(3.f,3.f,3.f);
		m_tEfxData.fEmitInterval = 0.1f;
		m_tEfxData.fEmitDuration = 2.5f;
		m_tEfxData.iParticleCount = 2;
		m_tEfxData.vGravity = _vec2(7.f,7.f);
		m_tEfxData.vColorMax = _vec3(30.f,5.f,5.f);
		m_tEfxData.vDieTime = _vec2(3.4f,3.6f);
		m_tEfxData.vSpeed = _vec2(30.1f,40.1f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,1.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(1.f,2.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(-70.f,70.f);
		m_tEfxData.vSpreadRadius = _vec2(11.f,30.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(-360.f,360.f);

		m_tEfxData.vStartSize = _vec2(1.f,3.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.01f,10.1f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(1.f,13.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,100.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(0.2f,300.f,0.f);
		m_tEfxData.vAlphaDelta[2] = _vec3(1.0f,-400.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 0.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(1.f, 10.f, 0.f);
		m_tEfxData.isDestAlphaOne = true;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = -100.f;
		//m_tEfxData.isLoop = true;

		//m_tEfxData.fLookDist = 1.f;
		m_tEfxData.vOffSetAngle = _vec3(0.f,8.f,0.f);
		m_tEfxData.isLoop = false;
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;

		/*m_tEfxData.vWidth = _vec2(16.f,25.f);
		m_tEfxData.vWidthDelta[0] = _vec2(0.1f,0.1f);
		m_tEfxData.vWidthDelta[1] = _vec2(1.f,-11.f);
		m_tEfxData.vLength = _vec2(1.1f,1.2f);
		m_tEfxData.vLengthDelta[0] = _vec2(0.1f,0.1f);
		m_tEfxData.vLengthDelta[1] = _vec2(1.f,0.4f);*/
		break;

		case EP10_HEART5:
		m_tEfxData.iImgIdx = 5;
		m_tEfxData.vPositionOffSet = _vec3(3.f,3.f,3.f);
		m_tEfxData.fEmitInterval = 0.1f;
		m_tEfxData.fEmitDuration = 2.5f;
		m_tEfxData.iParticleCount = 1;
		//m_tEfxData.vGravity = _vec2(7.f,7.f);
		m_tEfxData.vColorMax = _vec3(60.f,5.f,5.f);
		m_tEfxData.vDieTime = _vec2(3.4f,3.6f);
		m_tEfxData.vSpeed = _vec2(30.1f,40.1f);
		m_tEfxData.vSpeedDelta[0] = _vec3(0.1f,1.f,0.f);
		m_tEfxData.vSpeedDelta[1] = _vec3(1.f,2.f,0.f);

		m_tEfxData.vSpreadAngle = _vec2(-35.f,35.f);
		m_tEfxData.vSpreadRadius = _vec2(11.f,20.f);
		m_tEfxData.vSpreadRadiusAngle = _vec2(-360.f,360.f);

		m_tEfxData.vStartSize = _vec2(2.f,4.f);
		m_tEfxData.vSizeDelta[0] = _vec3(0.01f,-1.f,0.f);
		m_tEfxData.vSizeDelta[1] = _vec3(0.5f,6.f,0.f);
		m_tEfxData.vSizeDelta[2] = _vec3(0.5f,10.f,0.f);

		m_tEfxData.vAlphaDelta[0] = _vec3(0.1f,200.f,0.f);
		m_tEfxData.vAlphaDelta[1] = _vec3(1.0f,-400.f,0.f);
		m_tEfxData.vStartRoll = _vec2(-360.f,360.f);
		m_tEfxData.vRollDelta[0] = _vec3(0.1f, 30.1f , 0.f);
		m_tEfxData.vRollDelta[1] = _vec3(0.5f, 50.f, 0.f);
		m_tEfxData.vRollDelta[2] = _vec3(1.f, 60.f, 0.f);
		m_tEfxData.isDestAlphaOne = false;
		m_tEfxData.isBillBoard = true;
		//m_tEfxData.fLookDist = -100.f;
		m_tEfxData.isLoop = false;
		//m_tEfxData.isLoop = true;

		//m_tEfxData.fLookDist = 1.f;
		m_tEfxData.vOffSetAngle = _vec3(0.f,8.f,0.f);
		//m_tEfxData.isDecrease = true;
		//m_tEfxData.isAttached = true;

		/*m_tEfxData.vWidth = _vec2(16.f,25.f);
		m_tEfxData.vWidthDelta[0] = _vec2(0.1f,0.1f);
		m_tEfxData.vWidthDelta[1] = _vec2(1.f,-11.f);
		m_tEfxData.vLength = _vec2(1.1f,1.2f);
		m_tEfxData.vLengthDelta[0] = _vec2(0.1f,0.1f);
		m_tEfxData.vLengthDelta[1] = _vec2(1.f,0.4f);*/
		break;
	}

	
}

void CExpEffect::SetLookDist(_float fDist)
{
	m_tEfxData.fLookDist = fDist;
}

void CExpEffect::SetOffSetWorldPos(_vec3 vOffSetPos)
{
	m_tEfxData.vWorldOffSetPos = vOffSetPos;
}

void CExpEffect::SetOffSetAngleY(_float fAngleY)
{
	m_tEfxData.fOffSetAngleY = fAngleY;
}

