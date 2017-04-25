#ifndef SkyFog_h__
#define SkyFog_h__

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

class CSkyFog:public Engine::CGameObject
{
private:
	explicit CSkyFog(LPDIRECT3DDEVICE9 pGraphicDev, _uint iImgIdx, _uint iPassIdx);
	virtual ~CSkyFog();

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
	_uint m_iPassIdx;

	_float m_fFirstAngleX;
	_float m_fFirstAngleZ;

public:
	static CSkyFog* Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iImgIdx = 0, _uint iPassIdx = 0);
private:
	virtual void Free(void);

};

#endif // SkyFog_h__
