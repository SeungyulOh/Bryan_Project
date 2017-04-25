#include "AnimationCtrl.h"

using namespace Engine;

Engine::CAnimationCtrl::CAnimationCtrl(LPDIRECT3DDEVICE9 pGraphicDev)
:m_pGraphicDev(pGraphicDev)
,m_pAnimationCtrl(NULL)
,m_pAniSet(NULL)
,m_dwTrackIndex(0)
,m_NewTrackIndex(1)
,m_dTimeAcc(0.0)
,m_PreAnimationIndex(999)
,m_isOccupied(false)
{
	m_pGraphicDev->AddRef();
}

Engine::CAnimationCtrl::CAnimationCtrl(const CAnimationCtrl& rhs)
:m_pGraphicDev(rhs.m_pGraphicDev)
,m_pAniSet(NULL)
,m_dwTrackIndex(rhs.m_dwTrackIndex)
,m_NewTrackIndex(rhs.m_NewTrackIndex)
,m_dTimeAcc(rhs.m_dTimeAcc)
,m_PreAnimationIndex(rhs.m_PreAnimationIndex)
,m_isOccupied(rhs.m_isOccupied)
{
	rhs.m_pAnimationCtrl->CloneAnimationController(rhs.m_pAnimationCtrl->GetMaxNumAnimationOutputs(), rhs.m_pAnimationCtrl->GetMaxNumAnimationSets(), rhs.m_pAnimationCtrl->GetMaxNumTracks(),rhs.m_pAnimationCtrl->GetMaxNumEvents(),&m_pAnimationCtrl);
	m_pGraphicDev->AddRef();
}

Engine::CAnimationCtrl::~CAnimationCtrl()
{

}

void Engine::CAnimationCtrl::Ready_AnimationCtrl(void)
{

}

void Engine::CAnimationCtrl::Set_AnimationSet(_ulong dwIndex, _bool bflexible, _float fSpeed)
{
	if(dwIndex > m_pAnimationCtrl->GetMaxNumAnimationSets()-1)
		return;
	if(dwIndex == m_PreAnimationIndex)
		return;
	if(m_isOccupied == true)
		return;
	if(bflexible == false)
	{
		m_isOccupied = true;
	}

	m_pAnimationCtrl->GetAnimationSet(dwIndex,&m_pAniSet);
	if(NULL == m_pAniSet)
		return;

	m_NewTrackIndex = m_dwTrackIndex == 0 ? 1 : 0;
	m_pAnimationCtrl->SetTrackAnimationSet(m_NewTrackIndex,m_pAniSet);

	m_pAnimationCtrl->UnkeyAllTrackEvents(m_dwTrackIndex);
	m_pAnimationCtrl->UnkeyAllTrackEvents(m_NewTrackIndex);

	m_pAnimationCtrl->KeyTrackEnable(m_dwTrackIndex,FALSE,m_dTimeAcc + 0.2);
	m_pAnimationCtrl->KeyTrackWeight(m_dwTrackIndex,0.1f,m_dTimeAcc,0.2,D3DXTRANSITION_LINEAR);
	m_pAnimationCtrl->KeyTrackSpeed(m_dwTrackIndex,fSpeed,m_dTimeAcc,0.2,D3DXTRANSITION_LINEAR);

	m_pAnimationCtrl->SetTrackEnable(m_NewTrackIndex,TRUE);
	m_pAnimationCtrl->KeyTrackWeight(m_NewTrackIndex,0.9f,m_dTimeAcc,0.2,D3DXTRANSITION_LINEAR);
	m_pAnimationCtrl->KeyTrackSpeed(m_NewTrackIndex,fSpeed,m_dTimeAcc,0.2,D3DXTRANSITION_LINEAR);


	m_pAnimationCtrl->ResetTime();
	m_dTimeAcc = 0.0;

	m_pAnimationCtrl->SetTrackPosition(m_NewTrackIndex,0.0);

	m_dwTrackIndex = m_NewTrackIndex;
	m_PreAnimationIndex = dwIndex;
}

void Engine::CAnimationCtrl::Play_Animation(const _float& fTimeDelta)
{
	m_dTimeAcc += fTimeDelta;

	D3DXTRACK_DESC tTrackDesc;
	ZeroMemory(&tTrackDesc,sizeof(D3DXTRACK_DESC));
	m_pAnimationCtrl->GetTrackDesc(m_dwTrackIndex,&tTrackDesc);

	if(m_isOccupied == true)
	{
		 if(tTrackDesc.Position+0.4 < m_pAniSet->GetPeriod())
			m_pAnimationCtrl->AdvanceTime(fTimeDelta,NULL);
		 else
		 {
			 m_isOccupied = false;
			 //Set_AnimationSet(0,true);
		 }
	}
	else
		m_pAnimationCtrl->AdvanceTime(fTimeDelta,NULL);

}

_double Engine::CAnimationCtrl::Get_CurrentRatioFromAnimation(void)
{
	D3DXTRACK_DESC tTrackDesc;
	ZeroMemory(&tTrackDesc,sizeof(D3DXTRACK_DESC));
	m_pAnimationCtrl->GetTrackDesc(m_dwTrackIndex,&tTrackDesc);

	return (tTrackDesc.Position) / m_pAniSet->GetPeriod();
}



CAnimationCtrl* Engine::CAnimationCtrl::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAnimationCtrl* pInst = new CAnimationCtrl(pGraphicDev);
	pInst->Ready_AnimationCtrl();
	return pInst;
}

CAnimationCtrl* Engine::CAnimationCtrl::Create(const CAnimationCtrl& rhs)
{
	CAnimationCtrl* pInst = new CAnimationCtrl(rhs);
	return pInst;
}

void Engine::CAnimationCtrl::Free(void)
{
	Engine::Safe_Release(m_pAniSet);
	Engine::Safe_Release(m_pAnimationCtrl);
	Engine::Safe_Release(m_pGraphicDev);
}

void Engine::CAnimationCtrl::SetCurrentTrackPosition(_double dPosition)
{
	m_pAnimationCtrl->SetTrackPosition(m_dwTrackIndex,dPosition);
}
