#ifndef MeshObj_h__
#define MeshObj_h__

#include "Engine_Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CStaticMesh;
}

class CMeshObj : public Engine::CGameObject
{
private:
	explicit CMeshObj(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMeshObj(void);

public:
	inline Engine::CTransform* GetTransform(){return m_pTransformCom;}
public:
	_tchar szName[256];
	inline _tchar* GetName(){return szName;}

public:
	virtual HRESULT Ready_GameObject(const _tchar* pNameTag,_vec3 vFirstPos);
	virtual void Render_GameObject(void);


private:
	Engine::CTransform* m_pTransformCom;
	Engine::CStaticMesh* m_pStaticMeshCom;

public:
	static CMeshObj* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pNameTag, _vec3 vFirstPos);
private:
	virtual void Free(void);
};

#endif // MeshObj_h__
