#ifndef Distortion_Mgr_h__
#define Distortion_Mgr_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CRenderer;
	class CShader;
}

class CDistortion;
class CDistortion_Mgr : public Engine::CGameObject
{
private:
	explicit CDistortion_Mgr(LPDIRECT3DDEVICE9 pGraphicDev,_float fDeadTime, _float fDistortionPow);
	virtual ~CDistortion_Mgr(void);

public:
	void AddDistortion(CDistortion* pDistortion);
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();

private:
	list<CDistortion*> m_pDistortionList;
	Engine::CRenderer* m_pRendererCom;
	Engine::CShader* m_pDistortionShader;

	LPDIRECT3DTEXTURE9 m_pDistortionTex;
	LPDIRECT3DSURFACE9 m_pSurface;
	_float m_fTimeAcc;
	_float m_fDeadTime;
	_float m_fDistortionPow;

public:
	static CDistortion_Mgr* Create(LPDIRECT3DDEVICE9 pGraphicDev,_float fDeadTime = 0.2f, _float fDistortionPow = 0.05f);
private:
	virtual void Free(void);

};

#endif // Distortion_Mgr_h__
