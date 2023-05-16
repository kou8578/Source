#include "Shader.h"

USING(Engine)


Engine::CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pEffect(nullptr)
{
}

Engine::CShader::CShader(const CShader& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_pEffect(rhs.m_pEffect)
{
	m_pEffect->AddRef();
}

Engine::CShader::~CShader(void)
{
}

HRESULT Engine::CShader::Ready_Shader(const TCHAR* pFilePath)
{

	/*������ �ƴ� ��� : m_pEffect�� ���� �ְ� m_pErrMsg = null�� ��� �������� ����

	��� �� ��� : m_pEffect�� ���� �ְ� m_pErrMsg = null�� �ƴ� ����� ����
	������ �� ��� : m_pEffect�� ���� ���� m_pErrMsg = null�� �ƴ� ����� ����*/

	if (FAILED(D3DXCreateEffectFromFile(m_pGraphicDev, 
										pFilePath,
										NULL, // ���̴� ��� �� ������ ��ũ�� �ּ�
										NULL, // ���̴� ��� �� ������ ���� �ּ�
										D3DXSHADER_DEBUG,  // ���̴� ����� ���� �ɼ�
										NULL,	// ���� ���ҽ� ������ ������ ���� ����, null�� �־��ָ� default�� ��ġ�� ����
										&m_pEffect, 
										&m_pErrMsg)))
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "shader_Error", MB_OK);
		return E_FAIL;
	}
	else if(nullptr != m_pErrMsg)	
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "shader_warning", MB_OK);
	}


	return S_OK;
}

Engine::CShader* Engine::CShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pFilePath)
{
	CShader*	pInstance = new CShader(pGraphicDev);

	if (FAILED(pInstance->Ready_Shader(pFilePath)))
		Safe_Delete(pInstance);
	
	return pInstance;
}

Engine::CComponent* Engine::CShader::Clone(void)
{
	return new CShader(*this);
}

void Engine::CShader::Free(void)
{
	Safe_Release(m_pEffect);
	Safe_Release(m_pGraphicDev);
}

