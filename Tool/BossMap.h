#ifndef Map_h__
#define Map_h__

#include "Engine_Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CStaticMesh;
}

class CBossMap : public Engine::CGameObject
{
private:
	explicit CBossMap(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossMap(void);

public:
	virtual HRESULT Ready_GameObject(void);
	virtual void Render_GameObject(void);


private:
	Engine::CTransform* m_pTransformCom;
	Engine::CStaticMesh* m_pStaticMeshCom;

public:
	static CBossMap* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

#endif // Stone_h__
