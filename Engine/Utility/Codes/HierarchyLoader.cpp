#include "HierarchyLoader.h"

using namespace Engine;

Engine::CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* szPath)
:m_dwRefCnt(0)
,m_pGraphicDev(pGraphicDev)
,m_szPath(szPath)
{
	m_pGraphicDev->AddRef();
}

Engine::CHierarchyLoader::~CHierarchyLoader(void)
{

}


void Engine::CHierarchyLoader::Ready_HierarchyLoader(void)
{

}

STDMETHODIMP Engine::CHierarchyLoader::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	D3DXFRAME_DERIVED* pNewFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pNewFrame,sizeof(D3DXFRAME_DERIVED));

	if(NULL != Name)
		Allocate_Name(&pNewFrame->Name,Name);

	pNewFrame->CombinedMatrix = *D3DXMatrixIdentity(&pNewFrame->TransformationMatrix);

	*ppNewFrame = pNewFrame;
	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::CreateMeshContainer(THIS_ LPCSTR Name, CONST D3DXMESHDATA *pMeshData, CONST D3DXMATERIAL *pMaterials, CONST D3DXEFFECTINSTANCE *pEffectInstances, DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED* pNewMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pNewMeshContainer,sizeof(D3DXMESHCONTAINER_DERIVED));

	if(NULL != Name)
		Allocate_Name(&pNewMeshContainer->Name,Name);

	pNewMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	LPD3DXMESH pMesh = pMeshData->pMesh;

	if(NULL == pMesh)
		return S_OK;

	_ulong dwFvF = pMesh->GetFVF();
	_ulong dwPolygonCnt = pMesh->GetNumFaces();

	if(!(dwFvF & D3DFVF_NORMAL))
	{
		pMesh->CloneMeshFVF(pMesh->GetOptions(),dwFvF | D3DFVF_NORMAL,m_pGraphicDev,&pNewMeshContainer->MeshData.pMesh);
		D3DXComputeNormals(pNewMeshContainer->MeshData.pMesh,pNewMeshContainer->pAdjacency);
	}
	else
	{
		pMesh->CloneMeshFVF(pMesh->GetOptions(),dwFvF,m_pGraphicDev,&pNewMeshContainer->MeshData.pMesh);
	}

	

	pNewMeshContainer->pAdjacency = new _ulong[dwPolygonCnt * 3];
	memcpy(pNewMeshContainer->pAdjacency,pAdjacency,sizeof(_ulong)*dwPolygonCnt*3);

	pNewMeshContainer->NumMaterials = NumMaterials == 0 ? 1 : NumMaterials;

	pNewMeshContainer->pMaterials = new D3DXMATERIAL[pNewMeshContainer->NumMaterials];
	

	if(0 != pNewMeshContainer->NumMaterials)
	{
		memcpy(pNewMeshContainer->pMaterials,pMaterials,sizeof(D3DXMATERIAL) * pNewMeshContainer->NumMaterials);

		/*char szTemp[256] = "";
		strcpy(szTemp,pMaterials[0].pTextureFilename);
		char* pCompareName = strtok(szTemp,"_");
		if(!strcmp(pCompareName,"kadan"))
		{
			pNewMeshContainer->ppTexture = new LPDIRECT3DTEXTURE9[pNewMeshContainer->NumMaterials*2];
			for(_ulong i = 0; i<2*pNewMeshContainer->NumMaterials ; i = i+2)
			{
				_tchar szPath[256] = L"";
				_tchar szFileName[128] = L"";
				lstrcpy(szPath,m_szPath);

				char szDevilName[256] = "";
				strcpy(szDevilName,pMaterials[i/2].pTextureFilename);
				strtok(szDevilName,".");
				strcat(szDevilName,"_devil.tga");
				_tchar szNewDevilName[256] = L"";
				_tchar szDevilPath[256] = L"";
				lstrcpy(szDevilPath,m_szPath);

				MultiByteToWideChar(CP_ACP, NULL, pMaterials[i/2].pTextureFilename,strlen(pMaterials[i/2].pTextureFilename),szFileName,sizeof(szFileName));
				MultiByteToWideChar(CP_ACP, NULL, szDevilName,strlen(szDevilName),szNewDevilName,sizeof(szNewDevilName));
				lstrcat(szPath,szFileName);
				lstrcat(szDevilPath,szNewDevilName);

				D3DXCreateTextureFromFile(m_pGraphicDev,szPath,&pNewMeshContainer->ppTexture[i]);
				D3DXCreateTextureFromFile(m_pGraphicDev,szDevilPath,&pNewMeshContainer->ppTexture[i+1]);
			}

		}
		else*/
		{
			pNewMeshContainer->ppTexture = new LPDIRECT3DTEXTURE9[pNewMeshContainer->NumMaterials];
			pNewMeshContainer->ppNormalTex = new LPDIRECT3DTEXTURE9[pNewMeshContainer->NumMaterials];
			for(_ulong i = 0; i<pNewMeshContainer->NumMaterials ; ++i)
			{
				_tchar szPath[256] = L"";
				_tchar szFileName[128] = L"";
				_tchar szNormalTexPath[128] = L"";
				lstrcpy(szPath,m_szPath);
				lstrcpy(szNormalTexPath,m_szPath);

				char szNormalName[128];
				strcpy(szNormalName,pMaterials[i].pTextureFilename);
				strtok(szNormalName,".");
				strcat(szNormalName,"_n.tga");

				MultiByteToWideChar(CP_ACP, NULL, pMaterials[i].pTextureFilename,strlen(pMaterials[i].pTextureFilename),szFileName,sizeof(szFileName));
				lstrcat(szPath,szFileName);

				_tchar szNormalFileName[128] = L"";
				MultiByteToWideChar(CP_ACP, NULL, szNormalName,strlen(szNormalName),szNormalFileName,sizeof(szNormalFileName));
				lstrcat(szNormalTexPath,szNormalFileName);
						
				D3DXCreateTextureFromFile(m_pGraphicDev,szPath,&pNewMeshContainer->ppTexture[i]);


				if(FAILED(D3DXCreateTextureFromFile(m_pGraphicDev,szNormalTexPath,&pNewMeshContainer->ppNormalTex[i])))
				{
					pNewMeshContainer->ppNormalTex[i] = NULL;
					continue;
				}
			}
		}
	}
	else
	{
		pNewMeshContainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Power = 1.0f;

		pNewMeshContainer->ppTexture[0] = NULL;	
	}

	if(NULL != pSkinInfo)
	{
		pMeshData->pMesh->CloneMeshFVF(pMeshData->pMesh->GetOptions(),pMeshData->pMesh->GetFVF(),m_pGraphicDev,&pNewMeshContainer->m_pOriMesh);

		pNewMeshContainer->pSkinInfo = pSkinInfo;
		pNewMeshContainer->pSkinInfo->AddRef();


		pNewMeshContainer->dwBoneNum = pNewMeshContainer->pSkinInfo->GetNumBones();

		pNewMeshContainer->ppCombinedMatrices = new _matrix*[pNewMeshContainer->dwBoneNum];
		pNewMeshContainer->pOffSetMatrices = new _matrix[pNewMeshContainer->dwBoneNum];
		pNewMeshContainer->pRenderingMatrices = new _matrix[pNewMeshContainer->dwBoneNum];

		for(_ulong i =0; i<pNewMeshContainer->dwBoneNum; ++i)
			pNewMeshContainer->pOffSetMatrices[i] = *pNewMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);

	}
	


	*ppNewMeshContainer = pNewMeshContainer;

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	Engine::Safe_Delete_Array(pFrameToFree->Name);

	if(NULL != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	if(NULL != pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	if(NULL != pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	Engine::Safe_Delete(pFrameToFree);

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;
	

	Engine::Safe_Delete_Array(pMeshContainer->Name);
	Engine::Safe_Release(pMeshContainer->MeshData.pMesh);
	Engine::Safe_Delete_Array(pMeshContainer->pAdjacency);

	for(_ulong i = 0; i< pMeshContainer->NumMaterials; ++i)
	{
		pMeshContainer->ppTexture[i]->Release();
		
		if(pMeshContainer->ppNormalTex[i] != NULL)
			pMeshContainer->ppNormalTex[i]->Release();
	}

	Engine::Safe_Delete_Array(pMeshContainer->pMaterials);
	Engine::Safe_Delete_Array(pMeshContainer->ppTexture);
	Engine::Safe_Delete_Array(pMeshContainer->ppNormalTex);

	Engine::Safe_Release(pMeshContainer->pSkinInfo);

	Engine::Safe_Delete_Array(pMeshContainer->ppCombinedMatrices);
	Engine::Safe_Delete_Array(pMeshContainer->pOffSetMatrices);
	Engine::Safe_Delete_Array(pMeshContainer->pRenderingMatrices);
	Engine::Safe_Release(pMeshContainer->m_pOriMesh);


	Engine::Safe_Delete(pMeshContainer);

	return S_OK;
}

CHierarchyLoader* Engine::CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* szPath)
{
	CHierarchyLoader* pInst = new CHierarchyLoader(pGraphicDev,szPath);
	pInst->Ready_HierarchyLoader();
	return pInst;
}

_ulong Engine::CHierarchyLoader::Release(void)
{
	if(0 == m_dwRefCnt)
	{
		Engine::Safe_Release(m_pGraphicDev);
		delete this;
		
		
		return m_dwRefCnt;
	}
	return m_dwRefCnt--;
}

void Engine::CHierarchyLoader::Allocate_Name(char** ppDestName, const char* pSrcName)
{
	_ulong uLength = strlen(pSrcName);

	*ppDestName = new char[uLength + 1];
	strcpy(*ppDestName,pSrcName);
}

