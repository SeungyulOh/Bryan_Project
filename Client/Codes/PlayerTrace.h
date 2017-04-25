#ifndef PlayerTrace_h__
#define PlayerTrace_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CDynamicMesh;
	class CShader;
}

class CPlayerTrace : public Engine::CGameObject
{
private:
	explicit CPlayerTrace(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayerTrace(void);

public:
	virtual HRESULT Ready_GameObject(Engine::CTransform* pPlayerTransfom, Engine::CDynamicMesh* pPlayerMesh, _ulong dwAniIdx);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int LastUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

private:
	Engine::CRenderer* m_pRendererCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CDynamicMesh* m_pDynamicMeshCom;
	Engine::CShader* m_pShaderCom;

	_double m_dAniPos;
	_float m_fTimeAcc;

public:
	static CPlayerTrace* Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CTransform* pPlayerTransfom, Engine::CDynamicMesh* pPlayerMesh, _ulong dwAniIdx);
private:
	virtual void Free(void);
};

#endif // PlayerTrace_h__
