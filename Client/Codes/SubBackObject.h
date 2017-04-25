#ifndef SubBackObject_h__
#define SubBackObject_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CStaticMesh;
	class CShader;
}

class CSubBackObject : public Engine::CGameObject
{
private:
	explicit CSubBackObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSubBackObject(void);

public:
	inline Engine::CTransform* GetTransform(){return m_pTransformCom;}
	inline const _tchar* GetName(){return pName;}

public:
	virtual HRESULT Ready_GameObject(const _tchar* pNameTag,_vec3 vFirstPos);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);
	

private:
	const _tchar* pName;
	Engine::CTransform* m_pTransformCom;
	Engine::CStaticMesh* m_pStaticMeshCom;
	Engine::CRenderer* m_pRendererCom;
	Engine::CShader* m_pShader;
	_float	m_fOffSet;
	_bool m_isOnce;

public:
	static CSubBackObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pNameTag, _vec3 vFirstPos);
private:
	virtual void Free(void);
};

#endif // SubBackObject_h__
