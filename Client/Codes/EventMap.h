#ifndef EventMap_h__
#define EventMap_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CStaticMesh;
	class CShader;

}

class CEventMap
	:public Engine::CGameObject
{
private:
	explicit CEventMap(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEventMap();
public:
	inline list<_int>* GetTargetList(void){return &m_TargetList;} 
	inline _bool GetStartChaseFlag(void){return m_isStartChase;}
	inline void SetEventEnd(void){m_isEndEvent = true;}

public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);


private:
	Engine::CRenderer* m_pRendererCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CStaticMesh* m_pStaticMeshCom;
	Engine::CShader* m_pShaderCom;

	_float m_fMagicNum1;
	_float m_fMagicNum2;

	D3DLIGHT9 m_tPrevLightInfo;

	_float m_fSelectTime;
	_float m_fTimeAcc;
	_int m_iPreRandValue;

	_float m_fFadeOutTime;
	_bool m_isEndEvent;

	list<_int> m_TargetList;
	_bool m_isStartChase;

public:
	static CEventMap* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);
};

#endif // EventMap_h__
