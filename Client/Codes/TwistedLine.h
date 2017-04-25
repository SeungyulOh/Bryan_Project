#ifndef TwistedLine_h__
#define TwistedLine_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CStaticMesh;
	class CBoxCollider;
	class CShader;
}

class CTwistedLine : public Engine::CGameObject
{
private:
	explicit CTwistedLine(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTwistedLine(void);

public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

	void KeyInput(const _float& fTimeDelta);


private:
	Engine::CRenderer* m_pRendererCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CStaticMesh* m_pStaticMeshCom;
	Engine::CShader* m_pShaderCom;

	_matrix* m_matPlayerWorld;
	_matrix m_matLocal;

	_float m_fTimeAcc;


public:
	static CTwistedLine* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

#endif // TwistedLine_h__
