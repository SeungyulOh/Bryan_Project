#ifndef NpcTi_h__
#define NpcTi_h__

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
class CNpcTi : public Engine::CGameObject
{
public:
	enum STATE {EVENT2,EVENT3,EVENT5,EVENT7,STATE_END};
public:
	void GetState(_uint* pState);
	inline _uint GetState(){return m_eMyState;}
private:
	explicit CNpcTi(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNpcTi(void);

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

public:
	static CNpcTi* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

#endif // NpcTi_h__
