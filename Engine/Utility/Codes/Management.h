#ifndef Management_h__
#define Management_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Scene.h"
#include "Renderer.h"

BEGIN(Engine)

class CLayer;
class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement();
	virtual ~CManagement();

public:
	void Set_CurrentScene(CScene* pScene);
	void Set_EvenetScene(CScene* pScene);
	void SwapScene(void);
	void Set_Renderer(CRenderer* pRenderer);
	list<CGameObject*>* GetList(const _tchar* pTagName);
	CLayer* GetLayer(const _tchar* pTagName);
	CScene* GetCurrentScene(void);
	inline CRenderer* GetRenderer(void){return m_pRenderer;}
	

	HRESULT Ready_Management(void);
	_int Update_Management(const _float fTimeDelta);
	void Render_Management(void);
	
	

private:
	CScene*  m_pCurrentScene;
	CScene*	 m_pEventScene;
	CRenderer* m_pRenderer;

private:
	virtual void Free(void);
};

END

#endif // Management_h__
