#include "stdafx.h"
#include "Loading.h"
#include "Export_Engine.h"
#include "SubBackObject.h"


CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
, m_isFinish(false)
,m_pLayer(NULL)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(m_szloadingString, sizeof(_tchar) * 128);
}

CLoading::~CLoading(void)
{

}

HRESULT CLoading::Ready_Loading(LOADINGID eLoadingID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_LoadingID = eLoadingID;

	return S_OK;
}
_uint CLoading::Loading_ForDragon(void)
{
	// - DynamicMesh - Dragon //
	//D3DXMatrixIdentity(&matInput);
	Engine::CComponent*			pComponent = NULL;

	_matrix matTrans, matInput;
	D3DXMatrixRotationY(&matInput,D3DXToRadian(90.f));
	D3DXMatrixIdentity(&matTrans);

	// - DynamicMesh - Cam // 
	D3DXMatrixTranslation(&matTrans,30.f - 5.f,6.f+1.5f,110.f);
	pComponent = Engine::CDynamicMesh::Create(m_pGraphicDev,L"../bin/Resources/Animation/",L"Start_Cam.X",matInput*matTrans);
	Engine::Add_Component(COM_STAGE, L"Component_Mesh_StartCam", pComponent);

	D3DXMatrixTranslation(&matTrans,19.f,0.f,47.f);
	pComponent = Engine::CDynamicMesh::Create(m_pGraphicDev,L"../bin/Resources/Mesh/DynamicMesh/Ti/",L"Ti2.X",matInput*matTrans);
	Engine::Add_Component(COM_STAGE, L"Component_Mesh_NpcTi", pComponent);

	D3DXMatrixTranslation(&matTrans,150.f,20.f,-1485.f);
	pComponent = Engine::CDynamicMesh::Create(m_pGraphicDev,L"../bin/Resources/Mesh/DynamicMesh/Dragon/",L"Dragon2.X",matInput*matTrans);
	Engine::Add_Component(COM_STAGE, L"Component_Mesh_Dragon", pComponent);

	// - DynamicMesh - Verafim //
	D3DXMatrixTranslation(&matTrans,-635.f,5.f,50.f);
	pComponent = Engine::CDynamicMesh::Create(m_pGraphicDev,L"../bin/Resources/Mesh/DynamicMesh/Verafim/",L"Verafim2.X",matInput*matTrans);
	Engine::Add_Component(COM_STAGE, L"Component_Mesh_Verafim", pComponent);


	m_isFinish=true;

	return 0;
}

_uint CLoading::Loading_ForStage(void)
{
	// For.Textures Loading ============================================================================================================================================
	lstrcpy(m_szloadingString, L"Textures Loading...");
	Engine::CComponent*			pComponent = NULL;

	//// For. SkyBox Texture
	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/SkyBox/SkyBox2.dds", Engine::CTexture::TYPE_CUBE,1);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_SkyBox", pComponent);

	//For Particle
	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/light_glow01.tga", Engine::CTexture::TYPE_NORMAL);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_Particle_LightGlow1", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/glow/glow%d.tga", Engine::CTexture::TYPE_NORMAL,17);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_Particle_Glow", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/light_ray.tga", Engine::CTexture::TYPE_NORMAL);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_Particle_light_ray", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/yellowflare_white_glow.tga", Engine::CTexture::TYPE_NORMAL);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_Particle_yellowflare_white_glow", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/particle_trail.tga", Engine::CTexture::TYPE_NORMAL);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_Particle_trail", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/atk03/parti_Spray%d.tga", Engine::CTexture::TYPE_NORMAL,3);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_Particle_Spray_A", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/atk03/dust_2_white.tga", Engine::CTexture::TYPE_NORMAL);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_Particle_dust_2_white", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/atk03/shape_fire_04.tga", Engine::CTexture::TYPE_NORMAL);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_Particle_shape_fire_04", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/atk03/lightning_ani.tga", Engine::CTexture::TYPE_NORMAL);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_Particle_lightning_ani", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/atk03/ring_outer_wind06.tga", Engine::CTexture::TYPE_NORMAL);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_Particle_ring_outer_wind06", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/atk03/evy_icelight_small%d.tga", Engine::CTexture::TYPE_NORMAL,2);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_Particle_evy_icelight_small", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/atk03/blueglow5.tga", Engine::CTexture::TYPE_NORMAL);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_Particle_blueglow5", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/atk03/simpleaura0%d.tga", Engine::CTexture::TYPE_NORMAL,3);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_simpleaura", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/atk03/shape_fire_09.tga", Engine::CTexture::TYPE_NORMAL);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_shape_fire_09", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/atk03/evy_icelight_07_gray_blue.tga", Engine::CTexture::TYPE_NORMAL);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_icelight_07_gray_blue", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/atk03/flare%d.tga", Engine::CTexture::TYPE_NORMAL,7);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_flare", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/atk04/blade_01.tga", Engine::CTexture::TYPE_NORMAL);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_Blade1", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/bomb0%d.tga", Engine::CTexture::TYPE_NORMAL,4);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_bomb", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/shape_fire_13.tga", Engine::CTexture::TYPE_NORMAL);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_shape_fire_13", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/magic_fire%d.tga", Engine::CTexture::TYPE_NORMAL,3);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_magic_fire", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/Buff%d.tga", Engine::CTexture::TYPE_NORMAL,3);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_Buff", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/dust_party_0%d.tga", Engine::CTexture::TYPE_NORMAL,3);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_DustParty", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/evy_icelight_07_gray_red.tga", Engine::CTexture::TYPE_NORMAL,1);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_evy_icelight_gray_red", pComponent);
	
	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/ring_disk.tga", Engine::CTexture::TYPE_NORMAL,1);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_ring_disk", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/ring_outer_wind0%d.tga", Engine::CTexture::TYPE_NORMAL,2);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_ring_outer_wind", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/wind_blade%d.tga", Engine::CTexture::TYPE_NORMAL,2);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_windblade", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Particle/critical_ehen01.tga", Engine::CTexture::TYPE_NORMAL,1);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_critical_ehen", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Hit%d.tga", Engine::CTexture::TYPE_NORMAL,11);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_Hit", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/hp_low_screen.tga", Engine::CTexture::TYPE_NORMAL,1);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_RedScreen", pComponent);

	//For Kadan Skill
	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Skill/kadan_teleport/kadan_teleport%d.tga", Engine::CTexture::TYPE_NORMAL,8);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_KadanTeleport", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Skill/kadan_swing04dust/swing04dust%d.tga", Engine::CTexture::TYPE_NORMAL,7);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_KadanSwing04Dust", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Trail/kadan_trail%d.tga", Engine::CTexture::TYPE_NORMAL,11);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_KadanTrail", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Skill/Kadan_Weapon/Weapon%d.tga", Engine::CTexture::TYPE_NORMAL,14);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_KadanWeapon", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Skill/sprint_run/sprint%d.tga", Engine::CTexture::TYPE_NORMAL,2);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_sprintrun", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Skill/Kadan_AtkLeft/AtkLeft%d.tga", Engine::CTexture::TYPE_NORMAL,5);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_AtkLeft", pComponent);


	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Skill/bolt/bolt%d.tga", Engine::CTexture::TYPE_NORMAL,3);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_bolt", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Skill/bolt/fireball%d.tga", Engine::CTexture::TYPE_NORMAL,2);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_fireball", pComponent);


	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Mesh/StaticMesh/Object/whirlpoolcloud0%d.tga", Engine::CTexture::TYPE_NORMAL,4);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_WhirlpoolCloud", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Water/waterfall0%d.tga", Engine::CTexture::TYPE_NORMAL,9);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_WaterFall", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Fog/Fog%d.tga", Engine::CTexture::TYPE_NORMAL,3);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_SkyFog", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Water/water_line0%d.tga", Engine::CTexture::TYPE_NORMAL,1);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_WaterLine", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Explosion/Explosion%d.tga", Engine::CTexture::TYPE_NORMAL,5);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_Explosion", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/SwordWave/waveready%d.tga", Engine::CTexture::TYPE_NORMAL,4);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_WaveReady", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/SwordWave/wavehit%d.tga", Engine::CTexture::TYPE_NORMAL,13);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_WaveHit", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Skill/Kadan_Explosion/Explosion%d.tga", Engine::CTexture::TYPE_NORMAL,6);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_KadanExplosion", pComponent);


	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/ep10/ep10_%d.tga", Engine::CTexture::TYPE_NORMAL,2);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_Ep10", pComponent);

	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/ep10/heart%d.tga", Engine::CTexture::TYPE_NORMAL,6);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_Ep10_Heart", pComponent);






	//For normal
	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Mesh/DynamicMesh/KadanHuman/kadan_face_n.tga", Engine::CTexture::TYPE_NORMAL,1);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_KadanFaceNormal", pComponent);

	////For.Trail
	pComponent = Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resources/Texture/Trail/trail0%d.tga", Engine::CTexture::TYPE_NORMAL,3);
	Engine::Add_Component(COM_STAGE, L"Component_Texture_PhantomTrail", pComponent);
	
	//// For.Collider Loading
	pComponent = Engine::CBoxCollider::Create(m_pGraphicDev);
	Engine::Add_Component(COM_STAGE, L"Component_BoxCollider",pComponent);

	lstrcpy(m_szloadingString, L"Shader Loading...");
	//Shader_water
	pComponent = Engine::CShader::Create(m_pGraphicDev,L"../Codes/Shader_Water.hpp");
	Engine::Add_Component(COM_STATIC,L"Component_Shader_Water",pComponent);

	//Shader_Mesh
	pComponent = Engine::CShader::Create(m_pGraphicDev,L"../Codes/Shader_Mesh.hpp");
	Engine::Add_Component(COM_STATIC,L"Component_Shader_Mesh",pComponent);

	//Shader_Effect
	pComponent = Engine::CShader::Create(m_pGraphicDev,L"../Codes/Shader_Effect.hpp");
	Engine::Add_Component(COM_STATIC,L"Component_Shader_Effect",pComponent);

	//Shader_Mesh_Phong
	pComponent = Engine::CShader::Create(m_pGraphicDev,L"../Codes/Shader_Mesh_Phong.hpp");
	Engine::Add_Component(COM_STATIC,L"Component_Shader_Mesh_Phong",pComponent);

	//Shader_Particle
	pComponent = Engine::CShader::Create(m_pGraphicDev,L"../Codes/Shader_Particle.hpp");
	Engine::Add_Component(COM_STATIC,L"Component_Shader_Particle",pComponent);

	//Shader_RenderTarget
	pComponent = Engine::CShader::Create(m_pGraphicDev,L"../Codes/Shader_RenderTarget.hpp");
	Engine::Add_Component(COM_STATIC,L"Component_Shader_RenderTarget",pComponent);

	//Shader_Skybox
	pComponent = Engine::CShader::Create(m_pGraphicDev,L"../Codes/Shader_Skybox.hpp");
	Engine::Add_Component(COM_STATIC,L"Component_Shader_Skybox",pComponent);

	//Shader_Shadow
	pComponent = Engine::CShader::Create(m_pGraphicDev,L"../Codes/Shader_Shadow.hpp");
	Engine::Add_Component(COM_STATIC,L"Component_Shader_Shadow",pComponent);
	
	//Shader_Instancing
	pComponent = Engine::CShader::Create(m_pGraphicDev,L"../Codes/Shader_HWInstancing.hpp");
	Engine::Add_Component(COM_STATIC,L"Component_Shader_Instancing",pComponent);



	// For.Meshes Loading
	
	_matrix matInput;
	
	D3DXMatrixRotationY(&matInput,D3DX_PI/2.f);
	lstrcpy(m_szloadingString, L"Hagie Meshes Loading...");
	pComponent = Engine::CDynamicMesh::Create(m_pGraphicDev,L"../bin/Resources/Mesh/DynamicMesh/Hagie/",L"Hagie_v9.X",matInput);
	Engine::Add_Component(COM_STAGE, L"Component_Mesh_Hagie", pComponent);

	lstrcpy(m_szloadingString, L"Dagger Meshes Loading...");
	pComponent = Engine::CDynamicMesh::Create(m_pGraphicDev,L"../bin/Resources/Mesh/DynamicMesh/Dagger/",L"Dagger.X",matInput);
	Engine::Add_Component(COM_STAGE, L"Component_Mesh_Dagger", pComponent);

	// - DynamicMesh - Kadan //
	lstrcpy(m_szloadingString, L"Kadan Meshes Loading...");
	pComponent = Engine::CDynamicMesh::Create(m_pGraphicDev,L"../bin/Resources/Mesh/DynamicMesh/KadanHuman/",L"CadanEvent2.X",matInput);
	Engine::Add_Component(COM_STAGE, L"Component_Mesh_Kadan", pComponent);
	lstrcpy(m_szloadingString, L"Loading Actor...");
	Loading_ForDragon();
	// Stage - bosskadan -> boss, Stage - m_isDragonSpawn false -> true

	//Kadan-Devil
	lstrcpy(m_szloadingString, L"KadanDevil Meshes Loading...");
	pComponent = Engine::CDynamicMesh::Create(m_pGraphicDev,L"../bin/Resources/Mesh/DynamicMesh/Kadan/",L"Kadan2.X",matInput);
	Engine::Add_Component(COM_STAGE, L"Component_Mesh_KadanDevil", pComponent);

	lstrcpy(m_szloadingString, L"KadanRage Meshes Loading...");
	pComponent = Engine::CDynamicMesh::Create(m_pGraphicDev,L"../bin/Resources/Mesh/DynamicMesh/Kadan/",L"KadanRage.X",matInput);
	Engine::Add_Component(COM_STAGE, L"Component_Mesh_KadanRage", pComponent);

	lstrcpy(m_szloadingString, L"Reaper Meshes Loading...");
	pComponent = Engine::CDynamicMesh::Create(m_pGraphicDev,L"../bin/Resources/Mesh/DynamicMesh/Reaper/",L"Reaper.X",matInput);
	Engine::Add_Component(COM_STAGE, L"Component_Mesh_Reaper", pComponent);

	//// - StaticMesh - BossWeapon // 
	pComponent = Engine::CStaticMesh::Create(m_pGraphicDev,L"../bin/Resources/Mesh/StaticMesh/BossWeapon/",L"BossWeapon.X");
	Engine::Add_Component(COM_STAGE, L"Component_Mesh_BossWeapon", pComponent);

	pComponent = Engine::CStaticMesh::Create(m_pGraphicDev,L"../bin/Resources/Mesh/StaticMesh/BossWeapon/",L"BossWeaponMad.X");
	Engine::Add_Component(COM_STAGE, L"Component_Mesh_BossWeaponMad", pComponent);

	lstrcpy(m_szloadingString, L"BossMap Meshes Loading...");
	// - StaticMesh - Map // 
	pComponent = Engine::CStaticMesh::Create(m_pGraphicDev,L"../bin/Resources/Mesh/StaticMesh/Map/",L"KadanMap4.X");
	Engine::Add_Component(COM_STAGE, L"Component_Mesh_BossMap", pComponent);

	pComponent = Engine::CStaticMesh::Create(m_pGraphicDev,L"../bin/Resources/Mesh/StaticMesh/Map/",L"EventMap2.X");
	Engine::Add_Component(COM_STAGE, L"Component_Mesh_EventMap", pComponent);
	
	lstrcpy(m_szloadingString, L"Loading Navi...");
	LoadNavi();
	lstrcpy(m_szloadingString, L"Loading SubObject...");
	LoadSubObject();
	
	m_isFinish = true;

	lstrcpy(m_szloadingString, L"Loading Complete...");

	return 0;
}

void CLoading::LoadNavi(void)
{
	
	Engine::Get_NaviMgr()->Initialize(m_pGraphicDev,100);


	DWORD	dwByte = 0;

	HANDLE	hFile  = CreateFile(L"../../Data/Navi.dat", 
		GENERIC_READ, 
		0, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	D3DXVECTOR3 vLoad[3];
	while(true)
	{
		ReadFile(hFile, vLoad, sizeof(D3DXVECTOR3)*3, &dwByte, NULL);

		if(dwByte == 0)
			break;

		for(_uint i=0; i<3; ++i)
		{
			vLoad[i].y += 5.f;
			vLoad[i] *= 0.1f;
		}

		D3DXVECTOR3 vDir1 = vLoad[1] - vLoad[0];
		D3DXVECTOR3 vDir2 = vLoad[2] - vLoad[1];

		D3DXVec3Normalize(&vDir1,&vDir1);
		D3DXVec3Normalize(&vDir2,&vDir2);

		D3DXVECTOR3 vNormal;
		D3DXVec3Cross(&vNormal,&vDir1,&vDir2);
		float fCos = D3DXVec3Dot(&vNormal, &D3DXVECTOR3(0.f,1.f,0.f));

		if(fCos >= 0.f)
			Engine::Get_NaviMgr()->AddNaviCell(&vLoad[0],&vLoad[1],&vLoad[2],false);
		else
			Engine::Get_NaviMgr()->AddNaviCell(&vLoad[0],&vLoad[2],&vLoad[1],false);

	}


	CloseHandle(hFile);

	Engine::Get_NaviMgr()->SetNeighbor();


}

void CLoading::LoadSubObject(void)
{
	DWORD	dwByte = 0;

	HANDLE	hFile  = CreateFile(L"../../Data/test3.dat", 
		GENERIC_READ, 
		0, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	m_pLayer = Engine::CLayer::Create();

	SaveMeshObj tLoadFile;
	while(true)
	{
		ReadFile(hFile, &tLoadFile, sizeof(SaveMeshObj), &dwByte, NULL);

		if(dwByte == 0)
			break;

		_tchar* pName = new _tchar[lstrlen(tLoadFile.szName)+1];
		lstrcpy(pName,tLoadFile.szName);

		_tchar szFullFileName[256] = L"";
		lstrcpy(szFullFileName,pName);
		lstrcat(szFullFileName,L".X");

		Engine::CComponent* pComponent_Found = Engine::Find_Component(COM_STAGE,pName);
		if(pComponent_Found == NULL)
		{
			Engine::CComponent* pComponent = Engine::CStaticMesh::Create(m_pGraphicDev,L"../bin/Resources/Mesh/StaticMesh/Object/",szFullFileName);
			Engine::Add_Component(COM_STAGE, pName, pComponent);
		}

		tLoadFile.vScale *= 0.1f;
		tLoadFile.vPos *= 0.1f;

		CSubBackObject* pObj = CSubBackObject::Create(m_pGraphicDev,pName,tLoadFile.vPos);
		pObj->GetTransform()->SetInfomation(Engine::CTransform::SCALE,tLoadFile.vScale);
		pObj->GetTransform()->SetInfomation(Engine::CTransform::ANGLE,tLoadFile.vAngle);
		pObj->GetTransform()->SetInfomation(Engine::CTransform::POSITION,tLoadFile.vPos);
		m_pLayer->Add_Object(pObj);


	}
	CloseHandle(hFile);
}



unsigned int APIENTRY CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint		iFlag = 0;

	EnterCriticalSection(pLoading->Get_CriticalSection());

	switch(pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;
	case LOADING_DRAGON:
		iFlag = pLoading->Loading_ForDragon();
		break;
	case LOADING_BOSS:
		//pLoading->Loading_ForBoss();
		break;
	}	

	LeaveCriticalSection(pLoading->Get_CriticalSection());

	return iFlag;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoadingID)
{
	CLoading*	pInstance = new CLoading(pGraphicDev);

	if(FAILED(pInstance->Ready_Loading(eLoadingID)))
	{
		MSG_BOX("CLoading Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoading::Free(void)
{
	if(m_LoadingID != LOADING_DRAGON)
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);	
		DeleteCriticalSection(&m_Crt);
	}
	
	::Safe_Release(m_pGraphicDev);
}

Engine::CLayer* CLoading::Export_LoadedLayer(void)
{
	return m_pLayer;
}


