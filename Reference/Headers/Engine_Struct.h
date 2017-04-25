#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagD3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
	{
		LPD3DXMESH m_pOriMesh;
		DWORD dwBoneNum;
		LPDIRECT3DTEXTURE9* ppTexture;
		LPDIRECT3DTEXTURE9* ppNormalTex;

		D3DXMATRIX** ppCombinedMatrices;
		D3DXMATRIX*  pOffSetMatrices;
		D3DXMATRIX*  pRenderingMatrices;
	}D3DXMESHCONTAINER_DERIVED;

	typedef struct tagD3DXFrameDerived : public D3DXFRAME
	{
		D3DXMATRIX CombinedMatrix;
	}D3DXFRAME_DERIVED;

	typedef struct tagParicle
	{
		D3DXVECTOR3 vPos;
		DWORD dwColor;
		D3DXVECTOR2 vTexUV;
	}PARTICLE;

	const unsigned long D3DFVF_PARTICLE = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	typedef struct tagBrush
	{
		D3DXVECTOR4 vPos;
		D3DXVECTOR2 vTexUV;
	}BRUSH;

	const unsigned long D3DFVF_BRUSH = D3DFVF_XYZRHW | D3DFVF_TEX1;

	typedef struct tagParicleAttribute
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vVelocity;
		D3DXVECTOR3 vAcceleration;
		float fLifeTime;
		float fAge;
		D3DXCOLOR vColor;
		D3DXCOLOR vColorFade;
		bool isAlive;
		float fViewZ;
		float fAlpha;
	}PARICLEATT;

	typedef struct tagVerCubeCol
	{
		D3DXVECTOR3 vPos;
		DWORD		dwColor;
	}VTXCUBECOL;

	const unsigned long D3DFVF_VTXCUBECOL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVerCubeTex
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vTexUV;
	}VTXCUBETEX;

	const unsigned long D3DFVF_VTXCUBETEX = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);


	typedef struct tagVertexTexture
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR2 vTexUV;
	}VTXTEX;
	const unsigned long D3DFVF_VTXTEX = D3DFVF_XYZ | D3DFVF_TEX1;

	typedef struct tagInstanceData
	{
		D3DXVECTOR4 vRight;
		D3DXVECTOR4 vUp;
		D3DXVECTOR4 vLook;
		D3DXVECTOR4 vPos;
		D3DXVECTOR4 vColor;
	}INSTANCEDATA;

	typedef struct tagVertexNormalTexture
	{
		_vec3 vPos;
		_vec3 vNormal;
		_vec2 vTexUV;
	}VTXNORTEX;

	const unsigned long D3DFVF_VTXNORTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagIndex16
	{
		unsigned short _0;
		unsigned short _1;
		unsigned short _2;
	}INDEX16;

	typedef struct tagIndex32
	{
		unsigned long _0;
		unsigned long _1;
		unsigned long _2;
	}INDEX32;

	typedef struct tagBlur
	{
		_float fWidth;
		_float fHeight;
		_int iMaxCnt;
		_float fCoeff;
		_float fTotalWeight;
		_float fWeight[13];
	}BLURDATA;

}

#endif // Engine_Struct_h__
