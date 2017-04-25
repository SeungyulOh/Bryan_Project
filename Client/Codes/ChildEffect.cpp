#include "stdafx.h"
#include "ChildEffect.h"
#include "Export_Engine.h"
#include "Distortion.h"
#include "BossDevil.h"

CChildEffect::CChildEffect(LPDIRECT3DDEVICE9 pGraphicDev, EFFECTNAME eName, EFXDATA tEfxData, _float fDieTime, _matrix* pParentMat, _matrix pOffSetRotMat)
:Engine::CGameObject(pGraphicDev)
,m_fTimeAcc(0.f)
,m_tEfxData(tEfxData)
,m_fDieTime(fDieTime)
,m_pParentMat(pParentMat)
,m_eEffectName(eName)
,m_OffSetMat(pOffSetRotMat)
,m_fTimeAccForLoop(0.f)
,m_isAliveForLoop(true)
{
	
}

CChildEffect::~CChildEffect()
{

}

HRESULT CChildEffect::Ready_GameObject()
{

	m_pTransformCom = (Engine::CTransform*)Engine::Clone(COM_STATIC,L"Component_Transform");
	Push_CloneComponent(CGameObject::UPDATE,L"Component_Transform",m_pTransformCom);

	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone(COM_STATIC,L"Component_Buffer_RectTexture");
	Push_CloneComponent(CGameObject::NONUPDATE,L"Component_Buffer_RectTexture",m_pBufferCom);

	
	_float fRadius = 0.f;
	if(m_tEfxData.vSpreadRadius.y != 0.f)
		fRadius = min(m_tEfxData.vSpreadRadius.x,m_tEfxData.vSpreadRadius.y) + fabs(m_tEfxData.vSpreadRadius.y - m_tEfxData.vSpreadRadius.x) * (rand()%11 * 0.1f);
	else
		fRadius = m_tEfxData.vSpreadRadius.x;

	if(m_tEfxData.vSpreadRadius != 0)
	{
		_float fRandAngle = min(m_tEfxData.vSpreadAngle.x, m_tEfxData.vSpreadAngle.y) + fabs(m_tEfxData.vSpreadAngle.y - m_tEfxData.vSpreadAngle.x) * (rand()%11 * 0.1f);
		_float fRandAngleSecond = 0.f;
		if(m_tEfxData.vSpreadRadiusAngle.y == 0.f)
			fRandAngleSecond = m_tEfxData.vSpreadRadiusAngle.x;
		else
			fRandAngleSecond = min(m_tEfxData.vSpreadRadiusAngle.x, m_tEfxData.vSpreadRadiusAngle.y) + fabs(m_tEfxData.vSpreadRadiusAngle.y - m_tEfxData.vSpreadRadiusAngle.x) * (rand()%11 * 0.1f);
		
		m_vSpreadDir.z = fRadius * sinf(D3DXToRadian(fRandAngle)) * cosf(D3DXToRadian(fRandAngleSecond));
		m_vSpreadDir.x = fRadius * sinf(D3DXToRadian(fRandAngle)) * sinf(D3DXToRadian(fRandAngleSecond));
		m_vSpreadDir.y = fRadius * cosf(D3DXToRadian(fRandAngle));
		//D3DXVec3Normalize(&m_vSpreadDir,&m_vSpreadDir);
	}
	else
		m_vSpreadDir = _vec3(0.f,0.f,0.f);

	if(m_tEfxData.fLookDist != 0.f)
	{
		Engine::CTransform* pBossTransform = (Engine::CTransform*)Engine::GetList(L"Layer_Boss")->front()->FindComponent(Engine::CGameObject::UPDATE,L"Component_Transform");
		_float fAngleY = pBossTransform->GetInfomation(Engine::CTransform::ANGLE)->y;
		_matrix matRotY;
		D3DXMatrixRotationY(&matRotY,fAngleY);
		D3DXVec3TransformNormal(&m_vSpreadDir,&m_vSpreadDir,&matRotY);
	}
	
	if(m_tEfxData.fOffSetAngleY != 0.f)
	{
		_matrix matRotY;
		D3DXMatrixRotationY(&matRotY,m_tEfxData.fOffSetAngleY);
		D3DXVec3TransformNormal(&m_vSpreadDir,&m_vSpreadDir,&matRotY);
	}

	/////////////////Cylinder
	if(D3DXVec2Length(&m_tEfxData.vSpreadCylinderRadius[0]) != 0)
	{
		_float fRandRadius = min(m_tEfxData.vSpreadCylinderRadius[0].x, m_tEfxData.vSpreadCylinderRadius[0].y) + fabs(m_tEfxData.vSpreadCylinderRadius[0].y - m_tEfxData.vSpreadCylinderRadius[0].x) * (rand()%11 * 0.1f);
		_float fRandAngle = m_tEfxData.vSpreadAngle.x;
		if(m_tEfxData.vSpreadAngle.y != 0.f)
		{
			fRandAngle =  min(m_tEfxData.vSpreadAngle.x, m_tEfxData.vSpreadAngle.y) + fabs(m_tEfxData.vSpreadAngle.y - m_tEfxData.vSpreadAngle.x) * (rand()%11 * 0.1f);
		}
		m_vSpreadDir.z = fRandRadius * sinf(fRandAngle);
		m_vSpreadDir.y = m_tEfxData.vSpreadCylinderHeight[0].x;
		m_vSpreadDir.x = fRandRadius * cosf(fRandAngle);
	}

	//////////////////////////////

	m_fSpeedScale = 0.0045f;

	if(D3DXVec3Length(&m_tEfxData.vOffSetAngle) != 0)
		m_fOffSetAngleX = m_tEfxData.vOffSetAngle.x;
	else
		m_fOffSetAngleX = 0.f;

	if(m_tEfxData.vSpeed.y != 0)
		m_fSpeed = min(m_tEfxData.vSpeed.x , m_tEfxData.vSpeed.y) + fabs(m_tEfxData.vSpeed.y - m_tEfxData.vSpeed.x) * (rand()%11 * 0.1f);
	else
		m_fSpeed = m_tEfxData.vSpeed.x;

	m_fSpeed *= m_fSpeedScale;

	if(m_tEfxData.vLength.x == 0.f && m_tEfxData.vWidth.x == 0.f)
	{
		if(m_tEfxData.vStartSize.y != 0)
			m_fStartSize = min(m_tEfxData.vStartSize.y,m_tEfxData.vStartSize.x) + fabs(m_tEfxData.vStartSize.y - m_tEfxData.vStartSize.x) * (rand()%11 * 0.1f);
		else
			m_fStartSize = m_tEfxData.vStartSize.x;
		m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,Engine::_vec3(m_fStartSize,m_fStartSize,m_fStartSize));
	}
	else
	{
		if(m_tEfxData.vLengthDelta[0].x != 0 && m_tEfxData.vWidthDelta[0].x)
		{
			_float fRandX = min(m_tEfxData.vLength.x , m_tEfxData.vLength.y) + fabs(m_tEfxData.vLength.y - m_tEfxData.vLength.x) * (rand()%11 * 0.1f);
			_float fRandY = min(m_tEfxData.vWidth.x , m_tEfxData.vWidth.y) + fabs(m_tEfxData.vWidth.y - m_tEfxData.vWidth.x) * (rand()%11 * 0.1f);
			m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,Engine::_vec3(fRandX,fRandY,1.f));
		}
		else
		{
			m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,Engine::_vec3(m_tEfxData.vLength.x,m_tEfxData.vWidth.x,1.f));
		}
		
	}
	
	
	m_fStartRollZ = min(m_tEfxData.vStartRoll.x,m_tEfxData.vStartRoll.y) + fabs(m_tEfxData.vStartRoll.y - m_tEfxData.vStartRoll.x) * (rand()%11 * 0.1f);
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(m_fOffSetAngleX,0.f,D3DXToRadian(m_fStartRollZ)));

	if(m_tEfxData.isAttached == true)
		m_pTransformCom->SetOffSetRotMat(Engine::MyGetRotMatFromWorldMat(*m_pParentMat));

	m_iSpeedDeltaCnt = 0;
	m_iSizeDeltaCnt = 0;
	m_iRollDeltaCnt = 0;
	m_iAlphaDeltaCnt = 0;
	m_iCylinderRadiusCnt = 0;
	m_iCylinderHeightCnt = 0;
	m_iLengthDeltaCnt = 0;
	m_iWidthDeltaCnt = 0;

	
	if(D3DXVec3Length(&m_tEfxData.vColorMin) == 0)
		m_vColor = _vec4(D3DXCOLOR(D3DCOLOR_ARGB(255, (_int)m_tEfxData.vColorMax.x, (_int)m_tEfxData.vColorMax.y,(_int)m_tEfxData.vColorMax.z)));
	else
	{
		m_vColor.x = min(m_tEfxData.vColorMax.x,m_tEfxData.vColorMin.x) + fabs(m_tEfxData.vColorMax.x - m_tEfxData.vColorMin.x) * (rand()%11 * 0.1f);
		m_vColor.y = min(m_tEfxData.vColorMax.y,m_tEfxData.vColorMin.y) + fabs(m_tEfxData.vColorMax.y - m_tEfxData.vColorMin.y) * (rand()%11 * 0.1f);
		m_vColor.z = min(m_tEfxData.vColorMax.z,m_tEfxData.vColorMin.z) + fabs(m_tEfxData.vColorMax.z - m_tEfxData.vColorMin.z) * (rand()%11 * 0.1f);
		m_vColor.w = 1.f;
		m_vColor = _vec4(D3DXCOLOR(D3DCOLOR_ARGB(255, (_int)m_vColor.x, (_int)m_vColor.y,(_int)m_vColor.z)));
	}


	SetUpFirstPos();
	m_pTransformCom->Update_Component(0.f);


	m_vOriginScale = *m_pTransformCom->GetInfomation(Engine::CTransform::SCALE);
	m_vOriginAngle = *m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE);
	m_vOriginPos = *m_pTransformCom->GetInfomation(Engine::CTransform::POSITION);
	m_vStartDir = m_vSpreadDir;
	
	

	return S_OK;
}


_int CChildEffect::Update_GameObject(const _float& fTimeDelta)
{
	if(m_tEfxData.isDistortion == true)
		return 0;

	if(m_eEffectName == KADANEXPLOSION)
		int a = 0;

	if(m_tEfxData.isLoop)
	{
	
		if(m_fTimeAcc > m_fDieTime && m_isAliveForLoop == true)
		{
			if(m_tEfxData.iProbabilityForLoop == 0)
			{
				m_fTimeAcc = 0.f;
				m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,m_vOriginScale);
				m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,m_vOriginAngle);
				if(m_tEfxData.vStartRoll.x != 0.f)
				{
					m_fStartRollZ = min(m_tEfxData.vStartRoll.x,m_tEfxData.vStartRoll.y) + fabs(m_tEfxData.vStartRoll.y - m_tEfxData.vStartRoll.x) * (rand()%11 * 0.1f);
					m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(m_fOffSetAngleX,0.f,D3DXToRadian(m_fStartRollZ)));
				}
				SetUpFirstPos();
				m_vSpreadDir = m_vStartDir;
				m_pTransformCom->Update_Component(0.f);
			}
			else
			{
				_uint iRand = rand()%101;
				m_fTimeAcc = 0.f;
				if(iRand < m_tEfxData.iProbabilityForLoop)
				{
					m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,m_vOriginScale);
					m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,m_vOriginAngle);
					if(m_tEfxData.vStartRoll.x != 0.f)
					{
						m_fStartRollZ = min(m_tEfxData.vStartRoll.x,m_tEfxData.vStartRoll.y) + fabs(m_tEfxData.vStartRoll.y - m_tEfxData.vStartRoll.x) * (rand()%11 * 0.1f);
						m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,_vec3(m_fOffSetAngleX,0.f,D3DXToRadian(m_fStartRollZ)));
					}
					SetUpFirstPos();
					m_vSpreadDir = m_vStartDir;
					m_pTransformCom->Update_Component(0.f);
					m_isAliveForLoop = true;
				}
				else
				{
					m_isAliveForLoop = false;
					return 0;
				}
			}
		}
		else if(m_fTimeAcc > m_fDieTime && m_isAliveForLoop == false)
			m_isAliveForLoop = true;
	}

	//ChangeSpeed
	m_fTimeAcc += fTimeDelta;
	
	if(D3DXVec2Length(&m_tEfxData.vSpeed) != 0)
		ChangePosition(fTimeDelta);
	if(D3DXVec3Length(&m_tEfxData.vSizeDelta[0]) != 0)
		ChangeSize(fTimeDelta);
	if(D3DXVec3Length(&m_tEfxData.vRollDelta[0]) != 0)
		ChangeAngle(fTimeDelta);
	if(D3DXVec2Length(&m_tEfxData.vSpreadCylinderRadius[0]) != 0)
		ChangeDirection(fTimeDelta);
	if(D3DXVec2Length(&m_tEfxData.vLengthDelta[0]) != 0)
		ChangeLength(fTimeDelta);
	if(D3DXVec2Length(&m_tEfxData.vWidthDelta[0]) != 0)
		ChangeWidth(fTimeDelta);

	ChangeAlpha(fTimeDelta);


	if(m_tEfxData.isBillBoard == true)
	{
		_matrix matView;
		m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
		D3DXMatrixInverse(&matView,0,&matView);
		m_pTransformCom->SetBillBoardMat(Engine::MyGetRotMatFromWorldMat(matView));
	}
		
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	//if(m_tEfxData.isBillBoard == true)
	//{
	//	m_pTransformCom->SetBillBoardAfterUpdate();
	//}
	
	if(m_tEfxData.isAttached == true)
	{
		m_pTransformCom->Adjust_ParentMat(&(m_OffSetMat * Engine::MyGetNewMatExcScale(*m_pParentMat)));
		//m_pTransformCom->Adjust_ParentMat(&(Engine::MyGetRotMatFromWorldMat(*m_pParentMat)));
	}
	
	//ComputeViewZ(m_pTransformCom->GetWorldMatrix());
	//m_pRendererCom->Add_RenderObject(this, Engine::CRenderer::RT_ALPHA);

	
	if(m_tEfxData.isDealing == true)
	{
		CBossDevil* pBoss = ((CBossDevil*)Engine::GetList(L"Layer_Boss")->front());
		float fDist = D3DXVec3Length(&(*(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0] - *(_vec3*)&pBoss->GetWorldMatrix().m[3][0]));
		if(fDist < 10.f)
			pBoss->SetisCol(true);
	}
	

	return 0;
}

void CChildEffect::Render_GameObject(LPD3DXEFFECT pEffect)
{
	if(m_tEfxData.isDistortion == true || m_isAliveForLoop == false)
		return;
	
	//m_pShader->GetEffect()->SetTexture("g_baseTexture",m_pTextureCom->GetTexture(m_tEfxData.iImgIdx));
	
	pEffect->SetFloat("g_fAlpha",m_fAlpha);
	pEffect->SetMatrix("g_matWorld",m_pTransformCom->GetWorldMatrix());
	pEffect->SetVector("g_vOffSetColor",&m_vColor);
	pEffect->CommitChanges();
	//m_pShader->GetEffect()->Begin(NULL,0);
	//m_pShader->GetEffect()->BeginPass(5);

	m_pBufferCom->Render_Buffer();

	//m_pShader->GetEffect()->EndPass();
	//m_pShader->GetEffect()->End();

	/*TCHAR szTemp[256] = L"%d";
	wsprintf(szTemp,L"Color : %d",(_int)(m_vColor.x));
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(200.f,300.f),D3DXCOLOR(1.f,1.f,1.f,1.f));

	wsprintf(szTemp,L"AlphaIdx : %d",(_int)(m_iAlphaDeltaCnt));
	Engine::Render_Font(L"Font_Batang",szTemp,&_vec2(200.f,400.f),D3DXCOLOR(1.f,1.f,1.f,1.f));*/

}

CChildEffect* CChildEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, EFFECTNAME eName, EFXDATA tEfxData, _float fDieTime, _matrix* pParentMat, _matrix pOffSetRotMat)
{
	CChildEffect* pInst = new CChildEffect(pGraphicDev,eName,tEfxData,fDieTime,pParentMat,pOffSetRotMat);
	if(FAILED(pInst->Ready_GameObject()))
	{
		Engine::Safe_Release(pInst);
		return NULL;
	}
	return pInst;
}

void CChildEffect::Free(void)
{
	Engine::CGameObject::Free();

	Engine::Safe_Release(m_pTransformCom);
	Engine::Safe_Release(m_pBufferCom);


}

_int CChildEffect::LastUpdate_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CChildEffect::ChangePosition(const _float& fTimeDelta)
{
	if(D3DXVec3Length(&m_tEfxData.vSpeedDelta[0]) == 0)
	{
		_vec3 vNewPos = *(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0] + m_vSpreadDir * (m_fSpeed) * fTimeDelta;
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vNewPos);
		return;
	}

	for(_int i=0; i<9; ++i)
	{
		if(m_fTimeAcc > m_tEfxData.vSpeedDelta[i].x * m_fDieTime && m_fTimeAcc < m_tEfxData.vSpeedDelta[i+1].x * m_fDieTime)
		{
			m_iSpeedDeltaCnt = i;
			break;
		}
	}
	if(D3DXVec2Length(&m_tEfxData.vGravity) != 0)
	{
		if(m_eEffectName == KADANEXPLOSION)
			int a = 0;

		_float fRand = min(m_tEfxData.vGravity.x,m_tEfxData.vGravity.y) + fabs(m_tEfxData.vGravity.y - m_tEfxData.vGravity.x) * (rand()%11 * 0.1f);
		m_vSpreadDir -= _vec3(0.f,fRand,0.f) * fTimeDelta;
		//D3DXVec3Normalize(&m_vSpreadDir,&m_vSpreadDir);
	}
		

	_float fSpeedDelta;
	fSpeedDelta = ((m_tEfxData.vSpeedDelta[m_iSpeedDeltaCnt+1].y - m_tEfxData.vSpeedDelta[m_iSpeedDeltaCnt].y) / (m_tEfxData.vSpeedDelta[m_iSpeedDeltaCnt+1].x * m_fDieTime - m_tEfxData.vSpeedDelta[m_iSpeedDeltaCnt].x * m_fDieTime)) * (m_fTimeAcc - m_tEfxData.vSpeedDelta[m_iSpeedDeltaCnt].x * m_fDieTime) + m_tEfxData.vSpeedDelta[m_iSpeedDeltaCnt].y;  
	fSpeedDelta *= m_fSpeedScale;
	_vec3 vNewPos = *(_vec3*)&m_pTransformCom->GetWorldMatrix()->m[3][0] + m_vSpreadDir * (m_fSpeed + fSpeedDelta) * fTimeDelta;
	m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,vNewPos);
}

void CChildEffect::ChangeSize(const _float& fTimeDelta)
{
	for(_int i=0; i<9; ++i)
	{
		if(m_fTimeAcc > m_tEfxData.vSizeDelta[i].x * m_fDieTime && m_fTimeAcc < m_tEfxData.vSizeDelta[i+1].x * m_fDieTime)
		{
			m_iSizeDeltaCnt = i;
			break;
		}
	}

	_float fSizeDelta;
	fSizeDelta = ((m_tEfxData.vSizeDelta[m_iSizeDeltaCnt+1].y - m_tEfxData.vSizeDelta[m_iSizeDeltaCnt].y) / (m_tEfxData.vSizeDelta[m_iSizeDeltaCnt+1].x * m_fDieTime - m_tEfxData.vSizeDelta[m_iSizeDeltaCnt].x * m_fDieTime)) * (m_fTimeAcc - m_tEfxData.vSizeDelta[m_iSizeDeltaCnt].x * m_fDieTime) + m_tEfxData.vSizeDelta[m_iSizeDeltaCnt].y;  
	_vec3 vNewScale = _vec3(m_fStartSize + fSizeDelta, m_fStartSize + fSizeDelta, m_fStartSize + fSizeDelta);
	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,vNewScale);
}

void CChildEffect::ChangeAngle(const _float& fTimeDelta)
{
	for(_int i=0; i<9; ++i)
	{
		if(m_fTimeAcc > m_tEfxData.vRollDelta[i].x * m_fDieTime && m_fTimeAcc < m_tEfxData.vRollDelta[i+1].x * m_fDieTime)
		{
			m_iRollDeltaCnt = i;
			break;
		}
	}

	_float fAngleDelta;
	fAngleDelta = ((m_tEfxData.vRollDelta[m_iRollDeltaCnt+1].y - m_tEfxData.vRollDelta[m_iRollDeltaCnt].y) / (m_tEfxData.vRollDelta[m_iRollDeltaCnt+1].x * m_fDieTime - m_tEfxData.vRollDelta[m_iRollDeltaCnt].x * m_fDieTime)) * (m_fTimeAcc - m_tEfxData.vRollDelta[m_iRollDeltaCnt].x * m_fDieTime) + m_tEfxData.vRollDelta[m_iRollDeltaCnt].y;  
	_vec3 vNewAngle = _vec3(m_fOffSetAngleX,0.f,D3DXToRadian(m_fStartRollZ)) + _vec3(0.f,0.f,D3DXToRadian(fAngleDelta));
	m_pTransformCom->SetInfomation(Engine::CTransform::ANGLE,vNewAngle);
}	

void CChildEffect::ChangeAlpha(const _float& fTimeDelta)
{
	for(_int i=0; i<9; ++i)
	{
		if(m_fTimeAcc > m_tEfxData.vAlphaDelta[i].x * m_fDieTime && m_fTimeAcc < m_tEfxData.vAlphaDelta[i+1].x * m_fDieTime)
		{
			m_iAlphaDeltaCnt = i;
			break;
		}
	}

	m_fAlpha = ((m_tEfxData.vAlphaDelta[m_iAlphaDeltaCnt+1].y - m_tEfxData.vAlphaDelta[m_iAlphaDeltaCnt].y) / (m_tEfxData.vAlphaDelta[m_iAlphaDeltaCnt+1].x * m_fDieTime - m_tEfxData.vAlphaDelta[m_iAlphaDeltaCnt].x * m_fDieTime)) * (m_fTimeAcc - m_tEfxData.vAlphaDelta[m_iAlphaDeltaCnt].x * m_fDieTime) + m_tEfxData.vAlphaDelta[m_iAlphaDeltaCnt].y;  
	m_fAlpha /= 255.f;
}

void CChildEffect::ChangeDirection(const _float& fTimeDelta)
{
	for(_int i=0; i<9; ++i)
	{
		if(m_fTimeAcc > m_tEfxData.vSpreadCylinderRadius[i].x * m_fDieTime && m_fTimeAcc < m_tEfxData.vSpreadCylinderRadius[i+1].x * m_fDieTime)
		{
			m_iCylinderRadiusCnt = i;
			break;
		}
	}
	for(_int i=0; i<9; ++i)
	{
		if(m_fTimeAcc > m_tEfxData.vSpreadCylinderHeight[i].x * m_fDieTime && m_fTimeAcc < m_tEfxData.vSpreadCylinderHeight[i+1].x * m_fDieTime)
		{
			m_iCylinderHeightCnt = i;
			break;
		}
	}

	_float fCylinderRadius;
	fCylinderRadius = ((m_tEfxData.vSpreadCylinderRadius[m_iCylinderRadiusCnt+1].y - m_tEfxData.vSpreadCylinderRadius[m_iCylinderRadiusCnt].y) / (m_tEfxData.vSpreadCylinderRadius[m_iCylinderRadiusCnt+1].x * m_fDieTime - m_tEfxData.vSpreadCylinderRadius[m_iCylinderRadiusCnt].x * m_fDieTime)) * (m_fTimeAcc - m_tEfxData.vSpreadCylinderRadius[m_iCylinderRadiusCnt].x * m_fDieTime) + m_tEfxData.vSpreadCylinderRadius[m_iCylinderRadiusCnt].y;  
	_float fCylinderHeight;
	fCylinderHeight = ((m_tEfxData.vSpreadCylinderHeight[m_iCylinderHeightCnt+1].y - m_tEfxData.vSpreadCylinderHeight[m_iCylinderHeightCnt].y) / (m_tEfxData.vSpreadCylinderHeight[m_iCylinderHeightCnt+1].x * m_fDieTime - m_tEfxData.vSpreadCylinderHeight[m_iCylinderHeightCnt].x * m_fDieTime)) * (m_fTimeAcc - m_tEfxData.vSpreadCylinderHeight[m_iCylinderHeightCnt].x * m_fDieTime) + m_tEfxData.vSpreadCylinderHeight[m_iCylinderHeightCnt].y; 

	m_vSpreadDir.y = fCylinderHeight;
	m_vSpreadDir.z = fCylinderRadius * sinf(m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->z);
	m_vSpreadDir.x = fCylinderRadius * cosf(m_pTransformCom->GetInfomation(Engine::CTransform::ANGLE)->z);
	D3DXVec3Normalize(&m_vSpreadDir,&m_vSpreadDir);
}

void CChildEffect::ChangeLength(const _float& fTimeDelta)
{
	for(_int i=0; i<9; ++i)
	{
		if(m_fTimeAcc > m_tEfxData.vLengthDelta[i].x * m_fDieTime && m_fTimeAcc < m_tEfxData.vLengthDelta[i+1].x * m_fDieTime)
		{
			m_iLengthDeltaCnt = i;
			break;
		}
	}
	_float fLengthDelta = ((m_tEfxData.vLengthDelta[m_iLengthDeltaCnt+1].y - m_tEfxData.vLengthDelta[m_iLengthDeltaCnt].y) / (m_tEfxData.vLengthDelta[m_iLengthDeltaCnt+1].x * m_fDieTime - m_tEfxData.vLengthDelta[m_iLengthDeltaCnt].x * m_fDieTime)) * (m_fTimeAcc - m_tEfxData.vLengthDelta[m_iLengthDeltaCnt].x * m_fDieTime) + m_tEfxData.vLengthDelta[m_iLengthDeltaCnt].y;
	_vec3 vNewScale = *m_pTransformCom->GetInfomation(Engine::CTransform::SCALE);
	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,vNewScale + _vec3(fLengthDelta,0.f,0.f));
	
}

void CChildEffect::ChangeWidth(const _float& fTimeDelta)
{
	for(_int i=0; i<9; ++i)
	{
		if(m_fTimeAcc > m_tEfxData.vWidthDelta[i].x * m_fDieTime && m_fTimeAcc < m_tEfxData.vWidthDelta[i+1].x * m_fDieTime)
		{
			m_iWidthDeltaCnt = i;
			break;
		}
	}
	_float fWidthDelta = ((m_tEfxData.vWidthDelta[m_iWidthDeltaCnt+1].y - m_tEfxData.vWidthDelta[m_iWidthDeltaCnt].y) / (m_tEfxData.vWidthDelta[m_iWidthDeltaCnt+1].x * m_fDieTime - m_tEfxData.vWidthDelta[m_iWidthDeltaCnt].x * m_fDieTime)) * (m_fTimeAcc - m_tEfxData.vWidthDelta[m_iWidthDeltaCnt].x * m_fDieTime) + m_tEfxData.vWidthDelta[m_iWidthDeltaCnt].y;
	_vec3 vNewScale = *m_pTransformCom->GetInfomation(Engine::CTransform::SCALE);
	m_pTransformCom->SetInfomation(Engine::CTransform::SCALE,vNewScale + _vec3(0.f,fWidthDelta,0.f));
}

void CChildEffect::SetUpFirstPos(void)
{
	if(D3DXVec3Length(&m_tEfxData.vPositionOffSet) != 0)
	{
		_float fOffSetX = m_tEfxData.vPositionOffSet.x * _float(rand()%11) * 0.1f - m_tEfxData.vPositionOffSet.x * 0.5f;
		_float fOffSetY = m_tEfxData.vPositionOffSet.y * _float(rand()%11) * 0.1f - m_tEfxData.vPositionOffSet.y * 0.5f;
		_float fOffSetZ = m_tEfxData.vPositionOffSet.z * _float(rand()%11) * 0.1f - m_tEfxData.vPositionOffSet.z * 0.5f;

		//if(m_tEfxData.isAttached == false)
		{
			_float fDist = 0.f;
			if(m_tEfxData.fLookDist != 0.f)
				fDist = m_tEfxData.fLookDist;
			else if(D3DXVec2Length(&m_tEfxData.vLookDistRand) != 0)
				fDist = min(m_tEfxData.vLookDistRand.y,m_tEfxData.vLookDistRand.x) + fabs(m_tEfxData.vLookDistRand.y - m_tEfxData.vLookDistRand.x) * (rand()%11 * 0.1f);

			m_vStartPos = *(_vec3*)&m_pParentMat->m[3][0] + m_tEfxData.vMyOffSetPos + _vec3(fOffSetX,fOffSetY,fOffSetZ) + *(_vec3*)&m_pParentMat->m[2][0] * fDist;
		}
		//else
		//m_vStartPos =m_tEfxData.vMyOffSetPos + _vec3(fOffSetX,fOffSetY,fOffSetZ);

		if(m_tEfxData.isStartedFromLand == true)
			m_vStartPos.y = 0.5f;

		m_vStartPos += m_tEfxData.vWorldOffSetPos;
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,m_vStartPos);
	}
	else
	{
		_float fDist = 0.f;
		if(m_tEfxData.fLookDist != 0.f)
			fDist = m_tEfxData.fLookDist;
		else if(D3DXVec2Length(&m_tEfxData.vLookDistRand) != 0)
			fDist = min(m_tEfxData.vLookDistRand.y,m_tEfxData.vLookDistRand.x) + fabs(m_tEfxData.vLookDistRand.y - m_tEfxData.vLookDistRand.x) * (rand()%11 * 0.1f);

		m_vStartPos = m_tEfxData.vMyOffSetPos;
		//if(m_tEfxData.isAttached == false)
		m_vStartPos += *(_vec3*)&m_pParentMat->m[3][0]  + *(_vec3*)&m_pParentMat->m[2][0] * fDist;


		if(m_tEfxData.isStartedFromLand == true)
			m_vStartPos.y = 0.5f;

		m_vStartPos += m_tEfxData.vWorldOffSetPos;
		m_pTransformCom->SetInfomation(Engine::CTransform::POSITION,m_vStartPos);
	}
}

_matrix CChildEffect::GetWorldMatrix()
{
	return *m_pTransformCom->GetWorldMatrix();
}

D3DXVECTOR4 CChildEffect::GetColor()
{
	_vec4 vNewColor = m_vColor;
	vNewColor.w = m_fAlpha;
	return vNewColor;
}
