#ifndef Engine_Com_Manager_h__
#define Engine_Com_Manager_h__

#include "Engine_Defines.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCom_Manager : public CBase
{
	DECLARE_SINGLETON(CCom_Manager)
private:
	explicit CCom_Manager(void);
	virtual ~CCom_Manager(void);

public:
	HRESULT Ready_ComManager(_uint iSize);
	void Add_Component(_uint iIndex, const _tchar* pComTag, CComponent* pComponent);
	void Release_Component(_uint iIndex);

private:
	map<const _tchar*, CComponent*>* m_pMapComponent;
	typedef map<const _tchar*, CComponent*> MAPCOMPONENT;

public:
	CComponent* Find_Component(_uint iIndex, const _tchar* pComTag);
private:
	_uint m_iContainerSize;

public:
	CComponent* Clone(_uint iIndex, const _tchar* pComTag);
private:
	virtual void Free(void);

};


END

#endif // Engine_Com_Manager_h__
