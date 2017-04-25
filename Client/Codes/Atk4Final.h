#ifndef Atk4Final_h__
#define Atk4Final_h__

#include "Defines.h"
#include "ParticleSystem.h"

namespace Engine
{
	class CRenderer;
	class CTexture;
	class CShader;
	class CTransform;
}

class CAtk4Final : public Engine::CParticleSystem
{
private:
	explicit CAtk4Final(LPDIRECT3DDEVICE9 pGraphicDev,_int iNumParticles);
	virtual ~CAtk4Final();

public:
	void ResetParticle(Engine::PARICLEATT* pAttribute,_uint iIndex);
	virtual HRESULT Ready_GameObject(); 
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	inline _vec3* GetOriginVector(void){return &m_vOrigin;} 
	void CalculateParticlePos(Engine::PARICLEATT* pAttribute);

private:
	Engine::CRenderer* m_pRendererCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CShader* m_pShaderCom;
	Engine::CTransform* m_pParentTransform;
	_matrix* m_matPlayerRH;

	_bool m_isOnce;
	_matrix m_matLocal;
	_float m_fTimeAcc;
	_float m_fTimeDelta;
	_float m_fAlpha;
	_uint m_iRadius;
	_float m_fXdirection;


public:
	static CAtk4Final* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iNumParticles);

private:
	virtual void Free();
};


#endif // Atk4Final_h__
