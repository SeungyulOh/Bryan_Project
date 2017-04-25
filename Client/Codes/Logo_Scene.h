#ifndef Logo_Scene_h__
#define Logo_Scene_h__

#include "Defines.h"
#include "Scene.h"
#include "Loading.h"

class CLogo_Scene : public Engine::CScene
{
private:
	explicit CLogo_Scene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogo_Scene();

public:
	virtual HRESULT Ready_Scene();
	virtual _int Update_Scene(const _float& fTimeDelta);
	virtual void Render_Scene();

private:
	CLoading* m_pLoading;

private:
	HRESULT Ready_RenderState(void);
	HRESULT Ready_Layer(void);

public:
	static CLogo_Scene* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

#endif // Logo_Scene_h__
