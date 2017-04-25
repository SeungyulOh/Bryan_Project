#ifndef RingWind_h__
#define RingWind_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CShader;
}

class CRingWind:public Engine::CGameObject
{
private:
	explicit CRingWind(LPDIRECT3DDEVICE9 pGraphicDev,_float fOffSetAngleY);
	virtual ~CRingWind();

public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject(void);

private:
	Engine::CRenderer* m_pRendererCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CRect_Texture* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CShader* m_pShader;

	_float m_fUV;
	_bool m_isOnce;
	_float m_fTimeAcc;
	_float m_fAlpha;
	_float m_fSpeed;
	_float m_fDir;

	_matrix* m_pPlayerWorldMat;
	_float m_fXdirection;
	_vec3 m_vLocalPos;
	_uint m_iRadius;
	_matrix m_matRotAfterLocal;

public:
	static CRingWind* Create(LPDIRECT3DDEVICE9 pGraphicDev,_float fOffSetAngleY);
private:
	virtual void Free(void);

};

#endif // RingWind_h__
