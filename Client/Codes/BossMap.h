#ifndef BossMap_h__
#define BossMap_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CStaticMesh;
	class CShader;

}

class CBossMap
	:public Engine::CGameObject
{
private:
	explicit CBossMap(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossMap();

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

public:
	static CBossMap* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);
};

#endif // BossMap_h__
