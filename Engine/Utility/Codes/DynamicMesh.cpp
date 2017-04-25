#include "DynamicMesh.h"
#include "HierarchyLoader.h"
#include "AnimationCtrl.h"

using namespace Engine;

Engine::CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName,_matrix matFirst)
:CMesh(pGraphicDev)
,m_pLoader(NULL)
,m_pRootFrame(NULL)
{
	//D3DXMatrixIdentity(&m_matParent);
	//D3DXMatrixRotationY(&m_matParent,D3DX_PI/2.f);
}

Engine::CDynamicMesh::CDynamicMesh(const CDynamicMesh& rhs)
:CMesh(rhs)
,m_pLoader(rhs.m_pLoader)
,m_pAnimationCtrl(CAnimationCtrl::Create(*rhs.m_pAnimationCtrl))
,m_pRootFrame(rhs.m_pRootFrame)
,m_matParent(rhs.m_matParent)
,m_MeshContainerList(rhs.m_MeshContainerList)
{
	m_pLoader->AddRefCnt();

}

Engine::CDynamicMesh::~CDynamicMesh(void)
{

}

void Engine::CDynamicMesh::Ready_DynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev,const _tchar* pFilePath, const _tchar* pFileName,_matrix matFirst)
{
	m_matParent = matFirst;
	m_pLoader = CHierarchyLoader::Create(pGraphicDev,pFilePath);
	m_pAnimationCtrl = CAnimationCtrl::Create(pGraphicDev);

	_tchar szFullPath[256] = L"";
	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	//LPD3DXALLOCATEHIERARCHY
	//LPD3DXANIMATIONCONTROLLER
	if(FAILED(D3DXLoadMeshHierarchyFromX(szFullPath,D3DXMESH_MANAGED,pGraphicDev,m_pLoader,NULL,&m_pRootFrame,m_pAnimationCtrl->Get_AnimationCtrl())))
		return;

	//Calculating Combined mat
	Update_MatrixFromRootFrame((D3DXFRAME_DERIVED*)m_pRootFrame,m_matParent);

	//FillUp MatInfo On MeshContainer
	Fill_MatrixInfoOnMeshContainer((D3DXFRAME_DERIVED*)m_pRootFrame);

	


}

void Engine::CDynamicMesh::Update_MatrixFromRootFrame(D3DXFRAME_DERIVED* pRootFrame, _matrix matParent)
{
	pRootFrame->CombinedMatrix = pRootFrame->TransformationMatrix * matParent;

	if(NULL != pRootFrame->pFrameSibling)
		Update_MatrixFromRootFrame((D3DXFRAME_DERIVED*)pRootFrame->pFrameSibling,matParent);

	if(NULL != pRootFrame->pFrameFirstChild)
		Update_MatrixFromRootFrame((D3DXFRAME_DERIVED*)pRootFrame->pFrameFirstChild,pRootFrame->CombinedMatrix);
}


void Engine::CDynamicMesh::Fill_MatrixInfoOnMeshContainer(D3DXFRAME_DERIVED* pRootFrame)
{
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pRootFrame->pMeshContainer;

	if(NULL != pMeshContainer)
	{
		for(_ulong i =0; i< pMeshContainer->dwBoneNum; ++i)
		{
			const char* szBoneName = pMeshContainer->pSkinInfo->GetBoneName(i);

			D3DXFRAME_DERIVED* pFrame_Find = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame,szBoneName);
			pMeshContainer->ppCombinedMatrices[i] = &pFrame_Find->CombinedMatrix;

		}		
		//pRootFrame->CombinedMatrix * pMeshContainer->pOffSetMatrices
		m_MeshContainerList.push_back(pMeshContainer);
	}

	if(NULL != pRootFrame->pFrameSibling)
		Fill_MatrixInfoOnMeshContainer((D3DXFRAME_DERIVED*)pRootFrame->pFrameSibling);

	if(NULL != pRootFrame->pFrameFirstChild)
		Fill_MatrixInfoOnMeshContainer((D3DXFRAME_DERIVED*)pRootFrame->pFrameFirstChild);

}

void Engine::CDynamicMesh::Render_Mesh(void)
{
	list<D3DXMESHCONTAINER_DERIVED*>::iterator iter = m_MeshContainerList.begin();
	list<D3DXMESHCONTAINER_DERIVED*>::iterator iter_end = m_MeshContainerList.end();

	for( ; iter != iter_end ; ++iter)
	{
		for(_ulong i = 0; i < (*iter)->dwBoneNum; ++i)
			(*iter)->pRenderingMatrices[i] = (*iter)->pOffSetMatrices[i]* *(*iter)->ppCombinedMatrices[i];

		void* pSrc = NULL;
		void* pDest = NULL;

		(*iter)->m_pOriMesh->LockVertexBuffer(0,&pSrc);
		(*iter)->MeshData.pMesh->LockVertexBuffer(0,&pDest);

		(*iter)->pSkinInfo->UpdateSkinnedMesh((*iter)->pRenderingMatrices,NULL,pSrc,pDest);

		(*iter)->m_pOriMesh->UnlockVertexBuffer();
		(*iter)->MeshData.pMesh->UnlockVertexBuffer();

		m_pGraphicDev->SetMaterial(&(*iter)->pMaterials->MatD3D);
		for( _ulong i =0; i<(*iter)->NumMaterials ; ++i)
		{
			m_pGraphicDev->SetTexture(0,(*iter)->ppTexture[i]);
			(*iter)->MeshData.pMesh->DrawSubset(i);		
		}
	}
}


void Engine::CDynamicMesh::Render_MeshWithShader(LPD3DXEFFECT pEffect , _ulong dwPassIdx)
{
	D3DMATERIAL9 tMtrl;
	tMtrl = m_MeshContainerList.front()->pMaterials->MatD3D;
	if(tMtrl.Specular.r + tMtrl.Specular.g + tMtrl.Specular.b == 0.f)
		tMtrl.Specular = D3DXCOLOR(1.f,1.f,1.f,1.f);

	if(tMtrl.Ambient.r + tMtrl.Ambient.g + tMtrl.Ambient.b == 0.f)
		tMtrl.Ambient = D3DXCOLOR(0.4f,0.4f,0.4f,1.f);

	pEffect->SetVector("g_vMtrlDiffuse",(_vec4*)&tMtrl.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient",(_vec4*)&tMtrl.Ambient);
	pEffect->SetVector("g_vMtrlSpecular",(_vec4*)&tMtrl.Specular);

	list<D3DXMESHCONTAINER_DERIVED*>::iterator iter = m_MeshContainerList.begin();
	list<D3DXMESHCONTAINER_DERIVED*>::iterator iter_end = m_MeshContainerList.end();

	pEffect->Begin(0,0);
	pEffect->BeginPass(dwPassIdx);

	for( ; iter != iter_end ; ++iter)
	{
		for(_ulong i = 0; i < (*iter)->dwBoneNum; ++i)
			(*iter)->pRenderingMatrices[i] = (*iter)->pOffSetMatrices[i]* *(*iter)->ppCombinedMatrices[i];

		void* pSrc = NULL;
		void* pDest = NULL;

		(*iter)->m_pOriMesh->LockVertexBuffer(0,&pSrc);
		(*iter)->MeshData.pMesh->LockVertexBuffer(0,&pDest);

		(*iter)->pSkinInfo->UpdateSkinnedMesh((*iter)->pRenderingMatrices,NULL,pSrc,pDest);

		(*iter)->m_pOriMesh->UnlockVertexBuffer();
		(*iter)->MeshData.pMesh->UnlockVertexBuffer();

		for( _ulong i =0; i<(*iter)->NumMaterials ; ++i)
		{
			if(iter == m_MeshContainerList.end())
				break;

			pEffect->SetTexture("g_baseTexture",(*iter)->ppTexture[i]);
			pEffect->CommitChanges();
			(*iter)->MeshData.pMesh->DrawSubset(i);		
		}
	}

	pEffect->EndPass();
	pEffect->End();
}


void Engine::CDynamicMesh::Set_AnimationSet(_ulong dwIndex, _bool bflexible,  _float fSpeed)
{
	m_pAnimationCtrl->Set_AnimationSet(dwIndex, bflexible, fSpeed);
}

void Engine::CDynamicMesh::Play_Animation(const _float& fTimeDelta, _bool isUpdateCombine)
{
	m_pAnimationCtrl->Play_Animation(fTimeDelta);

	if(isUpdateCombine == true)
		Update_MatrixFromRootFrame((D3DXFRAME_DERIVED*)m_pRootFrame,m_matParent);
}



CDynamicMesh* Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName,_matrix matFirst)
{
	CDynamicMesh* pInst = new CDynamicMesh(pGraphicDev, pFilePath, pFileName,matFirst);
	pInst->Ready_DynamicMesh(pGraphicDev,pFilePath,pFileName,matFirst);
	return pInst;
}

void Engine::CDynamicMesh::Free(void)
{
	Engine::CMesh::Free();

	if(m_isClone == false)
	{
		m_pLoader->DestroyFrame(m_pRootFrame);

	}
	Engine::Safe_Release(m_pLoader);
	Engine::Safe_Release(m_pAnimationCtrl);
}

CComponent* Engine::CDynamicMesh::Clone(void)
{
	return new CDynamicMesh(*this);
}

void Engine::CDynamicMesh::Compute_MinMax(_vec3* pMin, _vec3* pMax, _ulong lMeshContainerIdx)
{

	list<D3DXMESHCONTAINER_DERIVED*>::iterator iter = m_MeshContainerList.begin();
	for( _ulong i = 0 ; i < lMeshContainerIdx ; ++i)
		++iter;

	if(iter == m_MeshContainerList.end())
		return;

	//D3DXGetFVFVertexSize()

	D3DVERTEXELEMENT9 Element[MAX_FVF_DECL_SIZE];
	ZeroMemory(Element,sizeof(D3DVERTEXELEMENT9)*MAX_FVF_DECL_SIZE);
	(*iter)->MeshData.pMesh->GetDeclaration(Element);

	_short sOffSet = 1;
	for(_ulong i =0; i<MAX_FVF_DECL_SIZE; ++i)
	{
		if(Element[i].Usage == D3DDECLUSAGE_POSITION)
		{
			sOffSet = Element[i].Offset;
			break;
		}
	}

	_byte* pVertex = NULL;
	(*iter)->MeshData.pMesh->LockVertexBuffer(NULL,(void**)&pVertex);

	D3DXComputeBoundingBox((_vec3*)(pVertex + sOffSet),(*iter)->MeshData.pMesh->GetNumVertices(),D3DXGetFVFVertexSize((*iter)->MeshData.pMesh->GetFVF()),pMin,pMax);

	(*iter)->MeshData.pMesh->UnlockVertexBuffer();


	//m_pMesh->GetDeclaration(Element);
	//D3DXComputeBoundingBox()
}

bool Engine::CDynamicMesh::Get_Occupied(void)
{
	return m_pAnimationCtrl->Get_Occupied();
}

void Engine::CDynamicMesh::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	_matrix matWorld,matView,matProj;
	m_pGraphicDev->GetTransform(D3DTS_WORLD,&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION,&matProj);

	pEffect->SetMatrix("g_matWorld",&matWorld);
	pEffect->SetMatrix("g_matView",&matView);
	pEffect->SetMatrix("g_matProj",&matProj);

	D3DLIGHT9 tLightInfo;
	m_pGraphicDev->GetLight(0,&tLightInfo);

	pEffect->SetVector("g_vLightDir",&D3DXVECTOR4(tLightInfo.Direction,0.f));
	pEffect->SetVector("g_vLightDiffuse",(_vec4*)&tLightInfo.Diffuse);
	pEffect->SetVector("g_vLightAmbient",(_vec4*)&tLightInfo.Ambient);
	pEffect->SetVector("g_vLightSpecular",(_vec4*)&tLightInfo.Specular);

	//pEffect->SetFloat("g_fPower")

	D3DXMatrixInverse(&matView,0,&matView);
	pEffect->SetVector("g_vCamPos",&_vec4(*(_vec3*)&matView.m[3][0],1.f));

}

void Engine::CDynamicMesh::Render_MeshOnlyForKadan(LPD3DXEFFECT pEffect, _ulong dwPassIdx /*= 0*/)
{
	list<D3DXMESHCONTAINER_DERIVED*>::iterator iter = m_MeshContainerList.begin();
	list<D3DXMESHCONTAINER_DERIVED*>::iterator iter_end = m_MeshContainerList.end();

	pEffect->Begin(0,0);
	pEffect->BeginPass(dwPassIdx);

	for( ; iter != iter_end ; ++iter)
	{
		for(_ulong i = 0; i < (*iter)->dwBoneNum; ++i)
			(*iter)->pRenderingMatrices[i] = (*iter)->pOffSetMatrices[i]* *(*iter)->ppCombinedMatrices[i];

		void* pSrc = NULL;
		void* pDest = NULL;

		(*iter)->m_pOriMesh->LockVertexBuffer(0,&pSrc);
		(*iter)->MeshData.pMesh->LockVertexBuffer(0,&pDest);

		(*iter)->pSkinInfo->UpdateSkinnedMesh((*iter)->pRenderingMatrices,NULL,pSrc,pDest);

		(*iter)->m_pOriMesh->UnlockVertexBuffer();
		(*iter)->MeshData.pMesh->UnlockVertexBuffer();

		D3DMATERIAL9 tMtrl;
		tMtrl = (*iter)->pMaterials->MatD3D;
		if(tMtrl.Specular.r + tMtrl.Specular.g + tMtrl.Specular.b == 0.f)
			tMtrl.Specular = D3DXCOLOR(1.f,1.f,1.f,1.f);

		if(tMtrl.Ambient.r + tMtrl.Ambient.g + tMtrl.Ambient.b == 0.f)
			tMtrl.Ambient = D3DXCOLOR(0.4f,0.4f,0.4f,1.f);


		pEffect->SetVector("g_vMtrlDiffuse",(_vec4*)&tMtrl.Diffuse);
		pEffect->SetVector("g_vMtrlAmbient",(_vec4*)&tMtrl.Ambient);
		pEffect->SetVector("g_vMtrlSpecular",(_vec4*)&tMtrl.Specular);
		//pEffect->SetFloat("g_fPower",(*iter)->pMaterials->MatD3D.Power);

		for( _ulong i =0; i<(*iter)->NumMaterials ; ++i)
		{
			if(iter == m_MeshContainerList.end())
				break;

			pEffect->SetTexture("g_baseTexture",(*iter)->ppTexture[i*2+1]);
			pEffect->CommitChanges();
			(*iter)->MeshData.pMesh->DrawSubset(i);		
		}
	}

	pEffect->EndPass();
	pEffect->End();
}

_double Engine::CDynamicMesh::Get_CurrentRatioFromAnimation()
{
	return m_pAnimationCtrl->Get_CurrentRatioFromAnimation();
}

void Engine::CDynamicMesh::SetCurrentTrackPosition(double _dPosition)
{
	m_pAnimationCtrl->SetCurrentTrackPosition(_dPosition);
}

_matrix Engine::CDynamicMesh::CalculateCombineMat(D3DXFRAME_DERIVED* pMatIn)
{
	_matrix matResult;
	D3DXMatrixIdentity(&matResult);

	if(m_pRootFrame->pFrameFirstChild != pMatIn)
		return matResult;

	matResult = m_pRootFrame->TransformationMatrix * m_matParent * pMatIn->TransformationMatrix;
	return matResult;
	
}

