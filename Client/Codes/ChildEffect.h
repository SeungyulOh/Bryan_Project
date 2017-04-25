#ifndef ChildEffect_h__
#define ChildEffect_h__

#include "GameObject.h"
#include "Defines.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;

	
}

class CChildEffect : public Engine::CGameObject
{
public:
	enum EFFECTNAME {TELEPORT,SWING04DUST,KADANWEAPON,SPRINTRUN,ATKLEFT,BOLT,FIREBALL,WATERFALL,WATERLINE,EXPLOSION,
		WAVEREADY,WAVEHIT,KADANEXPLOSION,EP10,EP10_HEART,EFFECT_END};

private:
	explicit CChildEffect(LPDIRECT3DDEVICE9 pGraphicDev, EFFECTNAME eName, EFXDATA tEfxData, _float fDieTime, _matrix* pParentMat, _matrix pOffSetRotMat);
	virtual ~CChildEffect();
public:
	_matrix GetWorldMatrix();
	D3DXVECTOR4 GetColor();
	inline _bool GetIsLoopEnable(){return m_isAliveForLoop;}
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(LPD3DXEFFECT pEffect);

public:
	void ChangePosition(const _float& fTimeDelta);
	void ChangeSize(const _float& fTimeDelta);
	void ChangeAngle(const _float& fTimeDelta);
	void ChangeAlpha(const _float& fTimeDelta);
	void ChangeDirection(const _float& fTimeDelta);
	void ChangeLength(const _float& fTimeDelta);
	void ChangeWidth(const _float& fTimeDelta);

	////
	void SetUpFirstPos(void);

protected:
	Engine::CTransform* m_pTransformCom;
	Engine::CRect_Texture* m_pBufferCom;

private:
	EFXDATA m_tEfxData;

	_float m_fTimeAcc;
	_float m_fDieTime;
	_vec3 m_vSpreadDir;
	_float m_fSpeed;
	_float m_fStartSize;
	_float m_fStartRollZ;
	_float m_fAlpha;
	_float m_fSpeedScale;
	_vec3 m_vStartPos;
	_vec4 m_vColor;
	_float m_fOffSetAngleX;

	_int m_iSpeedDeltaCnt;
	_int m_iSizeDeltaCnt;
	_int m_iRollDeltaCnt;
	_int m_iAlphaDeltaCnt;
	_int m_iCylinderRadiusCnt;
	_int m_iCylinderHeightCnt;
	_int m_iLengthDeltaCnt;
	_int m_iWidthDeltaCnt;

	_matrix* m_pParentMat;
	EFFECTNAME m_eEffectName;
	_matrix m_OffSetMat;

	_vec3 m_vOriginScale;
	_vec3 m_vOriginAngle;
	_vec3 m_vOriginPos;
	_vec3 m_vStartDir;
	_float m_fTimeAccForLoop;
	_bool m_isAliveForLoop;


public:
	static CChildEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev, EFFECTNAME eName,EFXDATA tEfxData , _float fDieTime, _matrix* pParentMat, _matrix pOffSetRotMat);

protected:
	virtual void Free(void);

};

#endif // ChildEffect_h__
