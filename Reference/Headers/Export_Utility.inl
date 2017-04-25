using namespace Engine;

//For_ManageMent
//Getter//
list<CGameObject*>* Engine::GetList(const _tchar* pTagName)
{
	return Engine::CManagement::GetInstance()->GetList(pTagName);
}
CLayer* Engine::GetLayer(const _tchar* pTagName)
{
	return Engine::CManagement::GetInstance()->GetLayer(pTagName);
}
CScene* Engine::GetCurrentScene(void)
{
	return Engine::CManagement::GetInstance()->GetCurrentScene();
}
CRenderer* Engine::GetRenderer(void)
{
	return Engine::CManagement::GetInstance()->GetRenderer();
}



//Setter//
void Engine::Set_Renderer(CRenderer* pRenderer)
{
	Engine::CManagement::GetInstance()->Set_Renderer(pRenderer);
}
void Engine::Set_CurrentScene(CScene* pScene)
{
	Engine::CManagement::GetInstance()->Set_CurrentScene(pScene);
}

void Engine::Set_EventScene(CScene* pScene)
{
	Engine::CManagement::GetInstance()->Set_EvenetScene(pScene);
}
void Engine::SwapScene(void)
{
	Engine::CManagement::GetInstance()->SwapScene();
}

//General//
HRESULT Engine::Ready_Management(void)
{
	return Engine::CManagement::GetInstance()->Ready_Management();
}
_uint Engine::Update_Management(const _float fTimeDelta)
{
	return Engine::CManagement::GetInstance()->Update_Management(fTimeDelta);
}
void Engine::Render_Management(void)
{
	return Engine::CManagement::GetInstance()->Render_Management();
}


//For_Component Management
//Getter//
//Setter//
//General//
HRESULT Engine::Ready_ComManager(_uint iIndex)
{
	return Engine::CCom_Manager::GetInstance()->Ready_ComManager(iIndex);
}

CComponent* Engine::Clone(_uint iIndex, const _tchar* pComTag)
{
	return Engine::CCom_Manager::GetInstance()->Clone(iIndex,pComTag);
}
void Engine::Add_Component(_uint iIndex, const _tchar* pComTag, CComponent* pComponent)
{
	Engine::CCom_Manager::GetInstance()->Add_Component(iIndex,pComTag,pComponent);
}
void Engine::Release_Component(_uint iIndex)
{
	Engine::CCom_Manager::GetInstance()->Release_Component(iIndex);
}
CComponent* Engine::Find_Component(_uint iIndex, const _tchar* pComTag)
{
	return Engine::CCom_Manager::GetInstance()->Find_Component(iIndex,pComTag);
}

//For_Light Management
//Getter//
//Setter//
//General//
void Engine::AddLight(LPDIRECT3DDEVICE9 pDevice ,D3DLIGHT9& tLightInfo)
{
	Engine::CLightMgr::GetInstance()->AddLight(pDevice,tLightInfo);
}
void Engine::LightEnable(LPDIRECT3DDEVICE9 pDevice, const _ushort& wIdx)
{
	Engine::CLightMgr::GetInstance()->LightEnable(pDevice,wIdx);
}
void Engine::LightDisable(LPDIRECT3DDEVICE9 pDevice, const _ushort& wIdx)
{
	Engine::CLightMgr::GetInstance()->LightDisable(pDevice,wIdx);
}
void Engine::DeleteLight(const _ushort& wIdx)
{
	Engine::CLightMgr::GetInstance()->DeleteLight(wIdx);
}
void Engine::Change_LightInfo(D3DLIGHT9& tLightInfo, _uint iIndex)
{
	Engine::CLightMgr::GetInstance()->Change_LightInfo(tLightInfo,iIndex);
}
D3DLIGHT9 Engine::GetLightInfo(_uint iIndex)
{
	return Engine::CLightMgr::GetInstance()->GetLightInfo(iIndex);
}
void Engine::SetisRender(_uint iIndex,_bool isRender)
{
	return Engine::CLightMgr::GetInstance()->SetisRender(iIndex,isRender);
}


//For_NaviMgr
CNaviMgr* Engine::Get_NaviMgr(void)
{
	return Engine::CNaviMgr::GetInstance();
}

CKeyMgr* Engine::Get_KeyMgr(void)
{
	return Engine::CKeyMgr::GetInstance();
}


//For_Target Management
HRESULT Engine::SetUp_OnShader(LPD3DXEFFECT pEffect, const _tchar* pTargetTag, const char* pConstantName)
{
	Engine::CTarget_Manager::GetInstance()->SetUp_OnShader(pEffect,pTargetTag,pConstantName);
	return S_OK;
}

HRESULT  Engine::Begin_MRT(const _tchar* pMRTTag)
{
	Engine::CTarget_Manager::GetInstance()->Begin_MRT(pMRTTag);
	return S_OK;
}
HRESULT  Engine::End_MRT(const _tchar* pMRTTag)
{
	Engine::CTarget_Manager::GetInstance()->End_MRT(pMRTTag);
	return S_OK;
}

///////////Release////////////////////
void Engine::Release_Utility()
{

	if(0 != Engine::CTarget_Manager::GetInstance()->DestroyInstance())
		MSG_BOX("CTargetManager Destroy Failed");

	if(0 != Engine::CKeyMgr::GetInstance()->DestroyInstance())
		MSG_BOX("CKeyMgr Destroy Failed");

	if(0 != Engine::CNaviMgr::GetInstance()->DestroyInstance())
		MSG_BOX("CNaviMgr Destroy Failed");

	if(0 != Engine::CLightMgr::GetInstance()->DestroyInstance())
		MSG_BOX("CLightMgr Destroy Failed");

	if(0 != Engine::CManagement::GetInstance()->DestroyInstance())
		MSG_BOX("CManagement Destroy Failed");

	if(0 != Engine::CCom_Manager::GetInstance()->DestroyInstance())
		MSG_BOX("CCom_Manager Destroy Failed");
}