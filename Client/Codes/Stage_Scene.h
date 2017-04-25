#ifndef Stage_Scene_h__
#define Stage_Scene_h__

#include "Defines.h"
#include "Scene.h"
#include "Loading.h"

class CBoss;
class CPlayer;
class CSnow;
class CChargeParticle_LightGlow;
class CStage_Scene : public Engine::CScene
{
private:
	explicit CStage_Scene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage_Scene();
public:
	inline _bool GetBossSpawn(void){return m_isBossSpawn;}
	inline void SetEventEndFromBossClass(void){m_bEventEndFromBossClass = true;}
	inline _bool GetBoolLine(void){return m_isRenderLine;}
	inline _bool GetDragonSpawn(void){return m_bDragonSpawn;}
	inline _bool GetisFade(void){return m_isFade;}
	inline void SetisFade(void){m_isFade = false;}
	inline void SetBossEventBlurBool(_bool isBlur){m_bBossEventBlur_Ing = isBlur;}
	inline _bool GetBossEventBlur(void){return m_bBossEventBlur_Ing;}
public:
	virtual HRESULT Ready_Scene();
	virtual _int Update_Scene(const _float& fTimeDelta);
	
	virtual void Render_Scene();

public:
	void Add_Layer_AfterLoading(const _tchar* pLayerTag, Engine::CLayer* pLayer);
	void StartBossEvent(void);
	void AddDefaultCamAfterEvent(void);
	void Load_Effect();
	

	vector<EFFECTDATA>* GetEffectVector(const _tchar* pAnimationTag);

private:
	CBoss* m_pBoss;
	CPlayer* m_pPlayer;
	CLoading* m_pLoading;
	_bool m_bDragonSpawn;
	_bool m_bEventEndFromBossClass;

	_bool m_bBossEventBlur_Ing;

	//BOSS EVENT
	_bool m_isBossSpawn;
	Engine::CGameObject* m_pDefaultCam;
	
	map<const _tchar*,vector<EFFECTDATA>> m_mapEffect;
	list<_tchar*> m_NameList;

	//Particle
	CSnow* m_pSnow;
	CChargeParticle_LightGlow* m_pFireWork;

	//Render Line
	_bool m_isRenderLine;

	_float m_fWaterPumpCoolTime;
	_matrix m_matWaterTrans;

	//////////////////////
	_bool m_isFade;
	_float m_fFadeTime;
	_float m_fTimeDelta;
private:
	HRESULT Ready_RenderState(void);
	HRESULT Ready_Layer(void);

public:
	static CStage_Scene* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

#endif // Stage_Scene_h__
