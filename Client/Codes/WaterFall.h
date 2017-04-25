#ifndef WaterFall_h__
#define WaterFall_h__

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

class CWaterFall:public Engine::CGameObject
{
private:
	explicit CWaterFall(LPDIRECT3DDEVICE9 pGraphicDev, _matrix matWorld,_uint iImgIdx, _uint iPass);
	virtual ~CWaterFall();

public:
	virtual HRESULT Ready_GameObject(_float fAngleY);
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
	_matrix m_matWorld;
	_matrix m_LocalMat;
	_int m_iPassIdx;

public:
	static CWaterFall* Create(LPDIRECT3DDEVICE9 pGraphicDev, _matrix matWorld,_uint iImgIdx = 0, _uint iPass = 0, _float fAngleY = D3DX_PI/2.f);
private:
	virtual void Free(void);

};

#endif // WaterFall_h__
