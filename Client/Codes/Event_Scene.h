#ifndef Event_Scene_h__
#define Event_Scene_h__

#include "Defines.h"
#include "Scene.h"
#include "Loading.h"

class CEvent_Scene : public Engine::CScene
{
private:
	explicit CEvent_Scene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEvent_Scene();
public:
	virtual HRESULT Ready_Scene();
	virtual _int Update_Scene(const _float& fTimeDelta);
	virtual void Render_Scene();

private:
	HRESULT Ready_RenderState(void);
	HRESULT Ready_Layer(void);

public:
	static CEvent_Scene* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

#endif // Event_Scene_h__
