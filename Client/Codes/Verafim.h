#ifndef Verafim_h__
#define Verafim_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CDynamicMesh;
	class CShader;
}

class CBoss;
class CVerafim : public Engine::CGameObject
{
public:
	enum STATE {EVENT1,EVENT2,IDLEDEAD,STATE_END};
public:
	void GetState(_uint* pState);
	inline _uint GetState(){return m_eMyState;}
private:
	explicit CVerafim(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVerafim(void);

public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

	//void FirstEventCam(void);
	void SetState(STATE eState);


private:
	Engine::CRenderer* m_pRendererCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CDynamicMesh* m_pDynamicMeshCom;
	Engine::CShader* m_pShaderCom;
	CBoss* m_pBoss;
private:
	_float m_fTimeDelta;
	_uint m_eMyState;
	_bool m_isOnce;
	
	_uint m_iSoundCtrl;
	_uint m_ePreState;

public:
	static CVerafim* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

#endif // Verafim_h__
