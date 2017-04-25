#ifndef CubeObj_h__
#define CubeObj_h__

#include "Engine_Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CCube_Tex;
}

class CCubeObj : public Engine::CGameObject
{
private:
	explicit CCubeObj(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCubeObj(void);

public:
	static CCubeObj* m_pSelectedCube;
public:
	Engine::VTXCUBETEX* m_pVertex;
	Engine::VTXCUBETEX* m_pVertexForColor;
	Engine::INDEX32 * m_pIndex;
	_vec3 m_vPos;


public:
	virtual HRESULT Ready_GameObject(_vec3 vPos);
	virtual void Render_GameObject(void);
	inline D3DXVECTOR3* GetPos() {return &m_vPos;}
	void UpdateWorldMatrix();


private:
	Engine::CTransform* m_pTransformCom;
	Engine::CCube_Tex* m_pCubeTexCom;
	

public:
	static CCubeObj* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
private:
	virtual void Free(void);
};

#endif // Stone_h__
