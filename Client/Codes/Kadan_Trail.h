#ifndef Kadan_Trail_h__
#define Kadan_Trail_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CTrail_Texture;
	class CTexture;
	class CShader;
	class CDynamicMesh;
}

class CKadan_Trail : public Engine::CGameObject
{
private:
	explicit CKadan_Trail(LPDIRECT3DDEVICE9 pGraphicDev, _matrix* pUpperMat);
	virtual ~CKadan_Trail(void);
public:
	void Ready_Trail();
	_int Update_GameObject(const _float& fTimeDelta);
	void Render_GameObject();

	_vec3 GetNewCurvePoint(_vec3 vSrc,_vec3 vDest,_float fRatio);
	_vec3 GetFinalPointFromThreePoints(_vec3* pSrc, _float fRatio);
	void MakeBegieCurve(void);

private:
	Engine::CTransform* m_pTransformCom;
	Engine::CRenderer* m_pRendererCom;
	Engine::CTrail_Texture* m_pTrailBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CShader* m_pShaderCom;

	_bool m_isOnce;
	_matrix* m_pUpperParentMat;
	_float m_fTimeDelta;

	list<D3DXVECTOR4> m_TrailList;
	D3DXVECTOR4 m_vTrailVector[40];
	_vec3 m_vPreDirection;

	_uint m_iMaxTrailVtxCnt;
	_bool* m_pBossVisible;

public:
	static CKadan_Trail* Create(LPDIRECT3DDEVICE9 pGraphicDev, _matrix* pUpperMat);

private:
	virtual void Free(void);
};


#endif // Kadan_Trail_h__
