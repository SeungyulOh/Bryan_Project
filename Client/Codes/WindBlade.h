#ifndef WindBlade_h__
#define WindBlade_h__

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

class CWindBlade:public Engine::CGameObject
{
private:
	explicit CWindBlade(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vWorldCreatePos , _float fEffectDir, _int iCloneCnt);
	virtual ~CWindBlade();

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

	_vec3 m_vWorldFirstPos;
	_float m_fEffectDir;

	_int m_iCloneCnt;
	


public:
	static CWindBlade* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vWorldCreatePos, _float fEffectDir , _int iCloneCnt);
private:
	virtual void Free(void);

};

#endif // WindBlade_h__
