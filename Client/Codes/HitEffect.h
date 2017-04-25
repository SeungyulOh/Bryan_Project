#ifndef HitEffect_h__
#define HitEffect_h__

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

class CHitEffect:public Engine::CGameObject
{
private:
	explicit CHitEffect(LPDIRECT3DDEVICE9 pGraphicDev,_int iCreateCnt,_vec3 vWorldCreatePos, _int iImgCnt,_float fScaleOffSet);
	virtual ~CHitEffect();

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
	_matrix* m_pPlayerMat;
	_matrix m_LocalMat;
	_int m_iCreateCnt;
	_vec3 m_vWorldCreatePos;
	_float m_fScaleOffset;
	


public:
	static CHitEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev,_int iCreateCnt,_vec3 vWorldCreatePos, _int iImgCnt = 0 , _float fScaleOffSet = 3.f);
private:
	virtual void Free(void);

};

#endif // HitEffect_h__
