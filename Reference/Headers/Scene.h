#ifndef Engine_Scene_h__
#define Engine_Scene_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene();
public:
	inline void SetisEventOn(_bool bEvent){m_isEventOn = bEvent;}
public:
	virtual HRESULT Ready_Scene();
	virtual _int Update_Scene(const _float& fTimeDelta);
	virtual void Render_Scene();

	list<CGameObject*>* GetList(const _tchar* pTagName);
	CLayer* GetLayer(const _tchar* pTagName);
	void DeleteLayer(const _tchar* pLayerTag);

protected:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	_bool m_isTimeStop;
	_bool m_isEventOn;

protected:
	map<const _tchar*, CLayer*> m_mapLayer;
	typedef map<const _tchar*, CLayer*> MAPLAYER;
	CLayer* m_pDynamicCamLayer;
	CLayer* m_pDefaultCamLayer;

protected:
	virtual void Free();
};


END

#endif // Scene_h__
