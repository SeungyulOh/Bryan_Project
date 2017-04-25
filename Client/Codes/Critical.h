#ifndef Critical_h__
#define Critical_h__

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

class CCritical:public Engine::CGameObject
{
private:
	explicit CCritical(LPDIRECT3DDEVICE9 pGraphicDev,_int iCreateCnt,_vec3 vWorldCreatePos);
	virtual ~CCritical();

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
	


public:
	static CCritical* Create(LPDIRECT3DDEVICE9 pGraphicDev,_int iCreateCnt,_vec3 vWorldCreatePos);
private:
	virtual void Free(void);

};

#endif // Critical_h__
