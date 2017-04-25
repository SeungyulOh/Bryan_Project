#ifndef Engine_AnimationCtrl_h__
#define Engine_AnimationCtrl_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CAnimationCtrl : public CBase
{
private:
	explicit CAnimationCtrl(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAnimationCtrl(const CAnimationCtrl& rhs);
	virtual ~CAnimationCtrl();

public:
	inline LPD3DXANIMATIONCONTROLLER* Get_AnimationCtrl(void){return &m_pAnimationCtrl;}
	inline void SetOccupied(void){m_isOccupied = false;}
	inline _ulong GetCurrentTrack(void){return m_dwTrackIndex;}
	inline _ulong GetNextTrack(void){return m_NewTrackIndex;}
	inline LPD3DXANIMATIONSET GetCurrentAniSet(void){return m_pAniSet;}
public:
	void Ready_AnimationCtrl(void);
	void Set_AnimationSet(_ulong dwIndex, _bool bflexible , _float fSpeed);
	void Play_Animation(const _float& fTimeDelta);
	inline bool Get_Occupied(void) {return m_isOccupied;}

	_double Get_CurrentRatioFromAnimation(void);
	void SetCurrentTrackPosition(_double dPosition);

private:
	LPD3DXANIMATIONCONTROLLER m_pAnimationCtrl;
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	LPD3DXANIMATIONSET m_pAniSet;
	_ulong m_dwTrackIndex;
	_ulong m_NewTrackIndex;
	_double m_dTimeAcc;
	_ulong m_PreAnimationIndex;

	//
	_bool m_isOccupied;

public:
	static CAnimationCtrl* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CAnimationCtrl* Create(const CAnimationCtrl& rhs);
private:
	virtual void Free(void);
};


END


#endif // AnimationCtrl_h__
