#ifndef ExpEffect_h__
#define ExpEffect_h__

#include "Defines.h"
#include "GameObject.h"
#include "ChildEffect.h"

namespace Engine
{
	class CRenderer;
	class CShader;
	class CTexture;
	class CRect_Texture;
}
class CExpEffect : public Engine::CGameObject
{
public:
	enum EFFECTNAME{KADANTELE0, KADANTELE1,KADANTELE2, KADANTELE3,KADANTELE4,KADANTELE5,KADANTELE6,KADANTELE7,KADANTELE8, 
		SWING4DUST0,SWING4DUST1,SWING4DUST2,SWING4DUST3,SWING4DUST4,SWING4DUST5,SWING4DUST6,SWING4DUST7,SWING4DUST8,SWING4DUST9,SWING4DUST10,
		KADANWEAPON0,KADANWEAPON1,KADANWEAPON2,KADANWEAPON3,KADANWEAPON4,KADANWEAPON5,KADANWEAPON6,KADANWEAPON7,KADANWEAPON8,KADANWEAPON9,KADANWEAPON10,KADANWEAPON11,KADANWEAPON12,KADANWEAPON13,
		SPRINTRUN0,SPRINTRUN1,SPRINTRUN2,SPRINTRUN3,SPRINTRUN4,SPRINTRUN5,SPRINTRUN6,SPRINTRUN7,
		ATKLEFT0,ATKLEFT1,ATKLEFT2,ATKLEFT3,ATKLEFT4,ATKLEFT5,ATKLEFT6,ATKLEFT7,
		ATKRIGHT0,BOLT0,BOLT1,BOLT2,FIREBALL0,FIREBALL1,FIREBALL2,
		WATERFALL0,WATERFALL1,WATERFALL2,WATERFALL3,WATERLINE0,EXPLOSION0,EXPLOSION1,EXPLOSION2,
		WAVEREADY0,WAVEREADY1,WAVEREADY2,WAVEREADY3,WAVEREADY4,WAVEHIT0,WAVEHIT1,WAVEHIT2,WAVEHIT3,WAVEHIT4,WAVEHIT5,WAVEHIT6,WAVEHIT7,WAVEHIT8,WAVEHIT9,WAVEHIT10,WAVEHIT11,WAVEHIT12,WAVEHIT13,
		KADANEXPLOSION0,KADANEXPLOSION1,KADANEXPLOSION2,KADANEXPLOSION3,KADANEXPLOSION4,KADANEXPLOSION5,
		EP10_0,EP10_1,EP10_HEART0,EP10_HEART1,EP10_HEART2,EP10_HEART3,EP10_HEART4,EP10_HEART5,EFFECT_END};
protected:
	explicit CExpEffect(LPDIRECT3DDEVICE9 pGraphicDev, CChildEffect::EFFECTNAME eChildEffName,EFFECTNAME eEffectName, _matrix* pParentMat, _matrix pOffSetRotMat);
	virtual ~CExpEffect(void);

public:
	inline void SetVisible(_bool bVisible){m_isVisible = bVisible;}
	inline void SetFirstPosDistortion(_vec3 vPos){m_vFistPosForDistorion = vPos;}
	inline void SetFirstPositionOffSet(_vec3 vPos){m_tEfxData.vPositionOffSet = vPos;}
	void SetLookDist(_float fDist);
	void SetOffSetAngleY(_float fAngleY);
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();

public:
	void FillEfxData(void);
	void SetOffSetWorldPos(_vec3 vOffSetPos);

protected:
	Engine::CRenderer* m_pRendererCom;
	Engine::CShader* m_pShaderCom;
	Engine::CShader* m_pInstancingShader;
	Engine::CTexture* m_pTextureCom;
	Engine::CRect_Texture* m_pBufferCom;



	EFXDATA m_tEfxData;
	_float m_fTimeAcc;
	_float m_fEmitTimeAcc;
	_float m_fDieTime;

	list<Engine::CGameObject*> m_ChildEffectList;

	EFFECTNAME m_eEffectName;
	_int m_iParticleCnt;

	_matrix* m_pParent;

	CChildEffect::EFFECTNAME m_eEffectNameForChild;
	_matrix m_OffSetRotMat;


	//For Instancing
	LPDIRECT3DVERTEXBUFFER9 m_pInstanceBuffer;
	LPDIRECT3DVERTEXDECLARATION9 m_pVtxDecl;
	_bool m_isVisible;
	_int m_iMaxCnt;
	_int m_iSizeGeom;
	_int m_iSizeInstance;

	_vec3 m_vFistPosForDistorion;
	_bool m_isOnce;

public:
	static CExpEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev,CChildEffect::EFFECTNAME eChildEffName, EFFECTNAME eEffectName, _matrix* pParentMat, _matrix pOffSetRotMat = *D3DXMatrixIdentity(&_matrix()));

protected:
	virtual void Free(void);
};

#endif // ExpEffect_h__
