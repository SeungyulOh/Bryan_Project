#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Engine_Defines.h"
#include "Management.h"
#include "Com_Manager.h"
#include "Component.h"
#include "Renderer.h"
#include "Transform.h"
#include "VIBuffer.h"
#include "Rect_Texture.h"
#include "Texture.h"
#include "Terrain_Texture.h"
#include "Light.h"
#include "LightMgr.h"
#include "Material.h"
#include "Cube_Tex.h"
#include "Mesh.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "NaviMgr.h"
#include "BoxCollider.h"
#include "AnimationCtrl.h"
#include "KeyMgr.h"
#include "Shader.h"
#include "Trail_Texture.h"
#include "Target_Manager.h"
#include "Target.h"


namespace Engine
{
	//For_ManageMent
	//Getter//
	inline list<CGameObject*>* GetList(const _tchar* pTagName);
	inline CLayer* GetLayer(const _tchar* pTagName);
	inline CScene* GetCurrentScene(void);
	inline CRenderer* GetRenderer(void);
	//Setter//
	inline void Set_Renderer(CRenderer* pRenderer);
	inline void Set_EventScene(CScene* pScene);
	inline void SwapScene(void);
	inline void Set_CurrentScene(CScene* pScene);
	//General//
	inline HRESULT Ready_Management(void);
	inline _uint Update_Management(const _float fTimeDelta);
	inline void Render_Management(void);
	

	//For_Component Management
	//Getter//
	//Setter//
	//General//
	inline HRESULT Ready_ComManager(_uint iIndex);
	inline void Add_Component(_uint iIndex, const _tchar* pComTag, CComponent* pComponent);
	inline CComponent* Clone(_uint iIndex, const _tchar* pComTag);
	inline void Release_Component(_uint iIndex);
	inline CComponent* Find_Component(_uint iIndex, const _tchar* pComTag);


	//For_Light Management
	//Getter//
	//Setter//
	//General//
	inline void AddLight(LPDIRECT3DDEVICE9 pDevice ,D3DLIGHT9& tLightInfo);
	inline void LightEnable(LPDIRECT3DDEVICE9 pDevice, const _ushort& wIdx);
	inline void LightDisable(LPDIRECT3DDEVICE9 pDevice, const _ushort& wIdx);
	inline void DeleteLight(const _ushort& wIdx);
	inline void Change_LightInfo(D3DLIGHT9& tLightInfo, _uint iIndex);
	inline D3DLIGHT9 GetLightInfo(_uint iIndex);
	inline void SetisRender(_uint iIndex,_bool isRender);
	
	//For_Target Management
	inline HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const _tchar* pTargetTag, const char* pConstantName);
	inline HRESULT  Begin_MRT(const _tchar* pMRTTag);
	inline HRESULT  End_MRT(const _tchar* pMRTTag);

	//For_NaviMgr
	inline CNaviMgr* Get_NaviMgr(void);

	//For_KeyMgr
	inline CKeyMgr* Get_KeyMgr(void);


	//Release
	inline void Release_Utility();

	#include "Export_Utility.inl"
}



#endif // Export_Utility_h__
