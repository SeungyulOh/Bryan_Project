#include "Layer.h"
#include "GameObject.h"

using namespace Engine;

Engine::CLayer::CLayer(void)
:m_isAliveOnEvent(false)
{

}

Engine::CLayer::~CLayer(void)
{

}


HRESULT Engine::CLayer::Ready_Layer(void)
{
	return S_OK;
}

_int Engine::CLayer::Update_Layer(const _float& fTimeDelta)
{
	if(m_ObjList.size() == 0)
		return 0;

	OBJLIST::iterator iter = m_ObjList.begin();
	OBJLIST::iterator iter_end = m_ObjList.end();

	_int iExitCode = 0;

	for( ; iter != iter_end; ++iter)
	{
		if((*iter)->GetDestroy() == true)
		{
			Engine::Safe_Release(*iter);
			iter = m_ObjList.erase(iter);
		}

		if(iter == m_ObjList.end())
			break;


		iExitCode = (*iter)->Update_GameObject(fTimeDelta);
		
		if(iExitCode & 0x80000000)
			return iExitCode;
	}

	return 0;
}


_int Engine::CLayer::LastUpdate_Layer(const _float& fTimeDelta)
{
	OBJLIST::iterator iter = m_ObjList.begin();
	OBJLIST::iterator iter_end = m_ObjList.end();

	_int iExitCode = 0;

	for( ; iter != iter_end; ++iter)
	{
		iExitCode = (*iter)->LastUpdate_GameObject(fTimeDelta);

		if(iExitCode & 0x80000000)
			return iExitCode;
	}

	return 0;
}


Engine::CLayer* Engine::CLayer::Create()
{
	CLayer* pInst = new CLayer;
	if(FAILED(pInst->Ready_Layer()))
	{
		Engine::Safe_Release(pInst);
		return NULL;
	}
	return pInst;		
}


void Engine::CLayer::Free()
{
	for_each(m_ObjList.begin(), m_ObjList.end(), CRelease_Single());
	m_ObjList.clear();
}
