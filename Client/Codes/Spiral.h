#ifndef Spiral_h__
#define Spiral_h__

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

class CSpiral:public Engine::CGameObject
{
private:
	explicit CSpiral(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSpiral();

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
	_float m_fScale;

	_float m_fMat[4];
	_float m_fOffSetAngle;

	//For Distortion
	Engine::CTexture* m_pTextureDistortion;
	Engine::CShader* m_pShaderForRenderTarget;

public:
	static CSpiral* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);

};

#endif // Spiral_h__
