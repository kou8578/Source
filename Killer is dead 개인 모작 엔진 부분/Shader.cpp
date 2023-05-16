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

	/*에러가 아닌 경우 : m_pEffect는 값이 있고 m_pErrMsg = null인 경우 정상적인 파일

	경고가 인 경우 : m_pEffect는 값이 있고 m_pErrMsg = null이 아닌 경우의 파일
	에러가 난 경우 : m_pEffect는 값이 없고 m_pErrMsg = null이 아닌 경우의 파일*/

	if (FAILED(D3DXCreateEffectFromFile(m_pGraphicDev, 
										pFilePath,
										NULL, // 쉐이더 사용 시 포함할 매크로 주소
										NULL, // 쉐이더 사용 시 포함할 파일 주소
										D3DXSHADER_DEBUG,  // 쉐이더 디버깅 용의 옵션
										NULL,	// 각종 리소스 정보를 저장할 곳을 지정, null을 넣어주면 default로 장치에 저장
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

