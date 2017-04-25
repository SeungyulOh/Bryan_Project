#include "StaticMesh.h"
#include "Shader.h"

using namespace Engine;

const D3DVERTEXELEMENT9 g_VBDecl_Geometry[] =
{
	{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
	{1, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
	{1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3},
	{1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4},
	{1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5},
	{1, 64, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 6},
	D3DDECL_END()
};

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev)
:CMesh(pGraphicDev)
{
	
}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
:CMesh(rhs)
,m_pAdjacency(rhs.m_pAdjacency)
,m_pSubset(rhs.m_pSubset)
,m_pMesh(rhs.m_pMesh)
,m_pOriMesh(rhs.m_pMesh)
,m_dwSubSetCnt(rhs.m_dwSubSetCnt)
,m_pMtrl(rhs.m_pMtrl)
,m_ppTexture(rhs.m_ppTexture)
{
	m_pMesh->AddRef();
	m_pAdjacency->AddRef();
	m_pSubset->AddRef();
	m_pOriMesh->AddRef();

	for(_ulong i = 0; i<m_dwSubSetCnt ; ++i)
		m_ppTexture[i]->AddRef();
}

Engine::CStaticMesh::~CStaticMesh(void)
{
	
}

Engine::CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* szPath, const _tchar* szFileName)
{
	CStaticMesh* pInst = new CStaticMesh(pGraphicDev);

	if(FAILED(pInst->Ready_Component(szPath,szFileName)))
		return NULL;

	return pInst;
}

Engine::CComponent* Engine::CStaticMesh::Clone(void)
{
	return new CStaticMesh(*this);
}


HRESULT Engine::CStaticMesh::Ready_Component(const _tchar* szPath, const _tchar* szFileName)
{
	_tchar szFullPath[256] = L"";
	lstrcpy(szFullPath,szPath);
	lstrcat(szFullPath,szFileName);

	_ulong	dwSubSetCnt = 0;

	if(FAILED(D3DXLoadMeshFromX(szFullPath,D3DXMESH_MANAGED,m_pGraphicDev,&m_pAdjacency,&m_pSubset,NULL,&dwSubSetCnt,&m_pOriMesh)))
		return E_FAIL;

	_ulong dwFVF = m_pOriMesh->GetFVF();

	if(dwFVF & D3DFVF_NORMAL)
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(),dwFVF,m_pGraphicDev,&m_pMesh);
	}
	else
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(),dwFVF | D3DFVF_NORMAL,m_pGraphicDev,&m_pMesh);

		D3DXComputeNormals(m_pMesh,(_ulong*)m_pAdjacency->GetBufferPointer());
	}


	m_dwSubSetCnt = dwSubSetCnt == 0 ? 1 : dwSubSetCnt;

	LPD3DXMATERIAL pSubSet = (LPD3DXMATERIAL)m_pSubset->GetBufferPointer();
	m_pMtrl = new D3DMATERIAL9[m_dwSubSetCnt];
	m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwSubSetCnt];

	if(0 != dwSubSetCnt)
	{
		for(_ulong i = 0; i<m_dwSubSetCnt ; ++i)
		{
			m_pMtrl[i] = pSubSet[i].MatD3D;
			m_pMtrl[i].Ambient = D3DXCOLOR(0.4f,0.4f,0.4f,1.f);
			m_pMtrl[i].Specular = D3DXCOLOR(1.f,1.f,1.f,1.f);

			_tchar szTexPath[256] = L"";
			_tchar szTexName[128] = L"";
			MultiByteToWideChar(CP_ACP,NULL, pSubSet[i].pTextureFilename,strlen(pSubSet[i].pTextureFilename),szTexName,sizeof(szTexName));
			lstrcpy(szTexPath, szPath);
			lstrcat(szTexPath, szTexName);

			if(FAILED(D3DXCreateTextureFromFile(m_pGraphicDev,szTexPath,&m_ppTexture[i])))
				MSG_BOX("Create Texture Failed");
		}
	}
	else
	{
		m_pMtrl[0].Diffuse = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		m_pMtrl[0].Specular = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		m_pMtrl[0].Emissive = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		m_pMtrl[0].Ambient = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		m_pMtrl[0].Power = 1.0f;

		m_ppTexture[0] = NULL;		
	}

	D3DVERTEXELEMENT9 tElement[MAX_FVF_DECL_SIZE];
	m_pMesh->GetDeclaration(tElement);


	return S_OK;
}

void Engine::CStaticMesh::Render_SubSets(_ulong lStage)
{
	for(_ulong i =0; i<m_dwSubSetCnt ; ++i)
	{
		m_pGraphicDev->SetMaterial(&m_pMtrl[i]);
		m_pGraphicDev->SetTexture(lStage,m_ppTexture[i]);
		m_pMesh->DrawSubset(i);	
	}
}


void Engine::CStaticMesh::Render_SubSetsWithShader(LPD3DXEFFECT pEffect,_ulong dwSubSetCnt, _ulong dwPassIdx)
{
	pEffect->SetVector("g_vMtrlDiffuse",(_vec4*)&m_pMtrl[0].Diffuse);
	pEffect->SetVector("g_vMtrlAmbient",(_vec4*)&m_pMtrl[0].Ambient);
	pEffect->SetVector("g_vMtrlSpecular",(_vec4*)&m_pMtrl[0].Specular);

	pEffect->Begin(NULL,0);
	pEffect->BeginPass(dwPassIdx);

	for(_ulong i =0;i<dwSubSetCnt; ++i)
	{
		//pEffect->SetFloat("g_fPower",20.f);
		pEffect->SetTexture("g_baseTexture",m_ppTexture[i]);
		pEffect->CommitChanges();

		m_pMesh->DrawSubset(i);
	}

	pEffect->EndPass();
	pEffect->End();
	
}


void Engine::CStaticMesh::Render_SubSetsWithHWInstancing(LPD3DXEFFECT pEffect, _ulong dwSubSetCnt, _matrix matWorld, _ulong dwPassIdx /*= 0*/)
{
	LPDIRECT3DVERTEXDECLARATION9 pVtxDecl;
	m_pGraphicDev->CreateVertexDeclaration(g_VBDecl_Geometry,&pVtxDecl);

	_int iSizeGeom = D3DXGetDeclVertexSize(g_VBDecl_Geometry,0);
	_int iSizeInstance = sizeof(Engine::INSTANCEDATA);
	LPDIRECT3DVERTEXBUFFER9 pInstanceBuffer;
	m_pGraphicDev->CreateVertexBuffer(iSizeInstance * m_pMesh->GetNumFaces(),0 ,NULL,D3DPOOL_MANAGED,&pInstanceBuffer,NULL);
	Engine::INSTANCEDATA tInstanceData;
	tInstanceData.vColor = _vec4(1.f,1.f,1.f,1.f);
	tInstanceData.vRight = _vec4(1.f,0.f,0.f,1.f);
	tInstanceData.vUp = _vec4(0.f,1.f,0.f,1.f);
	tInstanceData.vLook = _vec4(0.f,0.f,1.f,1.f);
	tInstanceData.vPos = _vec4(0.f,0.f,0.f,1.f);

	Engine::INSTANCEDATA* pData = NULL;
	pInstanceBuffer->Lock(0,0,(void**)&pData,0);
	for(_uint i=0; i<m_pMesh->GetNumFaces(); ++i)
		memcpy(&pData[i],&tInstanceData,sizeof(Engine::INSTANCEDATA));	
	pInstanceBuffer->Unlock();

	m_pGraphicDev->SetVertexDeclaration(pVtxDecl);
	LPDIRECT3DINDEXBUFFER9 pIndexBuffer;
	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer;
	m_pMesh->GetIndexBuffer(&pIndexBuffer);
	m_pMesh->GetVertexBuffer(&pVertexBuffer);
	m_pGraphicDev->SetIndices(pIndexBuffer);

	m_pGraphicDev->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | m_pMesh->GetNumFaces()));
	m_pGraphicDev->SetStreamSource(0,pVertexBuffer,0,m_pMesh->GetNumBytesPerVertex());
	m_pGraphicDev->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1ul));
	m_pGraphicDev->SetStreamSource(1, pInstanceBuffer,0,iSizeInstance);

	pEffect->SetVector("g_vMtrlDiffuse",(_vec4*)&m_pMtrl[0].Diffuse);
	pEffect->SetVector("g_vMtrlAmbient",(_vec4*)&m_pMtrl[0].Ambient);
	pEffect->SetVector("g_vMtrlSpecular",(_vec4*)&m_pMtrl[0].Specular);

	pEffect->Begin(NULL,0);
	pEffect->BeginPass(dwPassIdx);

	for(_ulong i =0;i<dwSubSetCnt; ++i)
	{
		//pEffect->SetFloat("g_fPower",20.f);
		pEffect->SetTexture("g_baseTexture",m_ppTexture[i]);
		pEffect->CommitChanges();

		//m_pMesh->DrawSubset(i);
		m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);
	}

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetStreamSourceFreq(0,1);
	m_pGraphicDev->SetStreamSourceFreq(1,1);

	pInstanceBuffer->Release();
	pIndexBuffer->Release();
	pVertexBuffer->Release();
	pVtxDecl->Release();
}



void Engine::CStaticMesh::Free(void)
{
	Engine::CMesh::Free();

	Engine::Safe_Release(m_pMesh);
	Engine::Safe_Release(m_pAdjacency);
	Engine::Safe_Release(m_pSubset);
	Engine::Safe_Release(m_pOriMesh);

	for(_ulong i = 0; i<m_dwSubSetCnt ; ++i)
		m_ppTexture[i]->Release();

	if(m_isClone == false)
	{
		Engine::Safe_Delete_Array(m_pMtrl);
		Engine::Safe_Delete_Array(m_ppTexture);
	}
}

void Engine::CStaticMesh::Compute_MinMax(_vec3* pMin, _vec3* pMax)
{
	//D3DXGetFVFVertexSize()

	D3DVERTEXELEMENT9 Element[MAX_FVF_DECL_SIZE];
	ZeroMemory(Element,sizeof(D3DVERTEXELEMENT9)*MAX_FVF_DECL_SIZE);
	m_pMesh->GetDeclaration(Element);


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
	m_pMesh->LockVertexBuffer(NULL,(void**)&pVertex);

	D3DXComputeBoundingBox((_vec3*)(pVertex + sOffSet),m_pMesh->GetNumVertices(),D3DXGetFVFVertexSize(m_pMesh->GetFVF()),pMin,pMax);

	m_pMesh->UnlockVertexBuffer();


}

void Engine::CStaticMesh::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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
