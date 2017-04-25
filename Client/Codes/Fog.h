#ifndef Fog_h__
#define Fog_h__

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

class CFog:public Engine::CGameObject
{
private:
	explicit CFog(LPDIRECT3DDEVICE9 pGraphicDev, _uint iImgIdx);
	virtual ~CFog();

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

	_uint m_iImgIdx;

public:
	static CFog* Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iImgIdx = 0);
private:
	virtual void Free(void);

};

#endif // Fog_h__
