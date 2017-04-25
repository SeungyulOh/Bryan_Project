#include "stdafx.h"
#include "MainApp.h"
#include "Export_Engine.h"
#include "Logo_Scene.h"
#include <time.h>
#include "SoundMgr.h"

CMainApp::CMainApp()
:m_pGraphicDev(NULL)
,m_fTimeAcc(0.f)
,m_dwFpsCnt(0)
{
	ZeroMemory(m_szFps,sizeof(_tchar)*128);
	srand(unsigned(time(NULL)));
}

CMainApp::~CMainApp()
{
	
}

HRESULT CMainApp::Ready_Component(void)
{
	Engine::Ready_ComManager(COM_END);

	Engine::CComponent* pComponent = Engine::CRenderer::Create(m_pGraphicDev);
	Engine::Add_Component(COM_STATIC,L"Component_Renderer",pComponent);
	Engine::Set_Renderer((Engine::CRenderer*)pComponent);

	//Transform
	pComponent = Engine::CTransform::Create(m_pGraphicDev);
	Engine::Add_Component(COM_STATIC,L"Component_Transform",pComponent);

	//Buffer
	pComponent = Engine::CRect_Texture::Create(m_pGraphicDev);
	Engine::Add_Component(COM_STATIC,L"Component_Buffer_RectTexture",pComponent);

	//TrailBuffer
	pComponent = Engine::CTrail_Texture::Create(m_pGraphicDev);
	Engine::Add_Component(COM_STATIC,L"Component_Buffer_TrailTexture",pComponent);

	//// For.CubeTex Buffer
	pComponent = Engine::CCube_Tex::Create(m_pGraphicDev);
	Engine::Add_Component(COM_STATIC, L"Component_Buffer_CubeTex", pComponent);

	////For. Material Loading
	pComponent = Engine::CMaterial::Create(m_pGraphicDev, D3DXCOLOR(1.f,1.f,1.f,1.f), D3DXCOLOR(1.f,1.f,1.f,1.f), D3DXCOLOR(1.f,1.f,1.f,1.f), D3DXCOLOR(0.f,0.f,0.f,0.f),1.f);
	Engine::Add_Component(COM_STATIC, L"Component_Material", pComponent);

	//Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev,L"../bin/Resources/Texture/Logo/Logo.png",Engine::CTexture::TYPE_NORMAL);
	Engine::Add_Component(COM_LOGO,L"Component_Texture_Logo",pComponent);

	//Shader
	pComponent = Engine::CShader::Create(m_pGraphicDev,L"../Codes/Shader_Default.hpp");
	Engine::Add_Component(COM_STATIC,L"Component_Shader_Default",pComponent);

	
	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/efx_ripple1_normal%d.tga", Engine::CTexture::TYPE_NORMAL,3);
	Engine::Add_Component(COM_STATIC, L"Component_Texture_efx_ripple1_normal", pComponent);

	CSoundMgr::GetInstance()->Initialize();

	return S_OK;
}

HRESULT CMainApp::Ready_MainApp()
{
	if(FAILED(Ready_DefaultSetting()))
		return E_FAIL;

	ShowCursor(false);

	CSoundMgr::GetInstance()->PlayBGM(L"bgm_ep10_kadan_prev.wav");

	return S_OK;
}

_int CMainApp::Update_MainApp(const _float& fTimeDelta)
{

	m_fTimeAcc += fTimeDelta;
	
	Engine::SetUp_InputState();
	Engine::Get_KeyMgr()->KeyCheck();

	return Engine::Update_Management(fTimeDelta);

}

void CMainApp::Render_MainApp()
{
	++m_dwFpsCnt;
	if(m_fTimeAcc >= 1.f)
	{
		wsprintf(m_szFps,L"FPS : %d",m_dwFpsCnt);
		m_fTimeAcc = 0.f;
		m_dwFpsCnt = 0;
	}

	Engine::Render_Begin(D3DXCOLOR(0.f,0.f,1.f,1.f));

	Engine::Render_Management();
	Engine::Render_Font(L"Font_Batang",m_szFps,&_vec2(0.f,0.f),D3DXCOLOR(1.f,1.f,1.f,1.f));

	Engine::Render_End();
}

HRESULT CMainApp::Ready_DefaultSetting(void)
{
	//Graphic Dev
	if(FAILED(Engine::Ready_GraphicDev(g_hWnd,Engine::CGraphicDev::MODE_FULL,WINCX,WINCY)))
		MSG_BOX("Ready_GraphicDev Failed");

	m_pGraphicDev = Engine::Get_GraphicDev();
	m_pGraphicDev->AddRef();
	
	if(FAILED(Engine::Ready_InputDevice(g_hInst,g_hWnd)))
		MSG_BOX("Ready_InputDev Failed");

	//Font Init
	if(FAILED(Engine::Ready_Font(m_pGraphicDev,L"Font_Batang",L"Batang",10,15,FW_HEAVY)))
		MSG_BOX("Ready_Font Failed");

	//SCene Init
	if(FAILED(Engine::Ready_Management()))
		MSG_BOX("Ready_Management Failed");

	//Component setting
	if(FAILED(Ready_Component()))
		MSG_BOX("Ready_Component Failed");

	Engine::CScene* pScene = CLogo_Scene::Create(m_pGraphicDev);
	if(pScene != NULL)
		Engine::Set_CurrentScene(pScene);

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pMine = new CMainApp;
	pMine->Ready_MainApp();
	return pMine;
}

void CMainApp::Free()
{
	CSoundMgr::GetInstance()->StopSoundAll();
	CSoundMgr::GetInstance()->DestroyInstance();

	Engine::Safe_Release(m_pGraphicDev);

	Engine::Release_Utility();

	Engine::Release_System();
	

	
}
