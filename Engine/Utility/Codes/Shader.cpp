#include "Shader.h"

using namespace Engine;

Engine::CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
:m_pGraphicDev(pGraphicDev)
,m_pEffect(NULL)
,m_pErrBuffer(NULL)
{
	m_pGraphicDev->AddRef();
}

Engine::CShader::CShader(const CShader& rhs)
:m_pGraphicDev(rhs.m_pGraphicDev)
,m_pEffect(rhs.m_pEffect)
,m_pErrBuffer(rhs.m_pErrBuffer)
{
	m_pGraphicDev->AddRef();

	if(NULL != m_pEffect)
		m_pEffect->AddRef();

	if(NULL != m_pErrBuffer)
		m_pErrBuffer->AddRef();
}

Engine::CShader::~CShader(void)
{

}

HRESULT Engine::CShader::Ready_Shader(const _tchar* szShaderFilePath)
{
	if(FAILED(D3DXCreateEffectFromFile(m_pGraphicDev,szShaderFilePath,NULL,NULL,D3DXSHADER_DEBUG,NULL,&m_pEffect,&m_pErrBuffer)))
	{
		MessageBoxA(NULL,(char*)m_pErrBuffer->GetBufferPointer(),NULL,MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

CShader* Engine::CShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* szShaderFilePath)
{
	CShader* pInstance = new CShader(pGraphicDev);
	if(FAILED(pInstance->Ready_Shader(szShaderFilePath)))
	{
		Engine::Safe_Release(pInstance);
		return NULL;
	}
	return pInstance;
		
}

CComponent* Engine::CShader::Clone(void)
{
	return new CShader(*this);
}

void Engine::CShader::Free(void)
{
	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pErrBuffer);
	Engine::Safe_Release(m_pEffect);
	

	Engine::CComponent::Free();
}

void Engine::CShader::SetMatrixOnShader()
{
	_matrix matWorld,matView,matProj;
	m_pGraphicDev->GetTransform(D3DTS_WORLD,&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION,&matProj);

	m_pEffect->SetMatrix("g_matWorld",&matWorld);
	m_pEffect->SetMatrix("g_matView",&matView);
	m_pEffect->SetMatrix("g_matProj",&matProj);
}

void Engine::CShader::SetMatrixOnShader(_matrix matWorld)
{
	_matrix matView,matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION,&matProj);

	m_pEffect->SetMatrix("g_matWorld",&matWorld);
	m_pEffect->SetMatrix("g_matView",&matView);
	m_pEffect->SetMatrix("g_matProj",&matProj);
}

