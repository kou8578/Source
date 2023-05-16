#include "ShaderMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CShaderMgr)
Engine::CShaderMgr::CShaderMgr(void)
{

}

CShaderMgr::~CShaderMgr(void)
{
	Free();
}

HRESULT CShaderMgr::Ready_Shader(const TCHAR * pShaderTag, CComponent * pInstance)
{
	CComponent*		pShader = Find_Shader(pShaderTag);

	if (nullptr != pShader)
		return E_FAIL;

	m_mapShader.emplace(pShaderTag, pInstance);

	return S_OK;
}

CComponent * CShaderMgr::Clone_Shader(const TCHAR * pShaderTag)
{
	CComponent*	pShader = Find_Shader(pShaderTag);
	if (nullptr == pShader)
		return nullptr;
	return pShader->Clone();
}

CComponent * CShaderMgr::Find_Shader(const TCHAR * pShaderTag)
{
	auto	iter = find_if(m_mapShader.begin(), m_mapShader.end(), CTag_Finder(pShaderTag));

	if (iter == m_mapShader.end())
		return nullptr;

	return iter->second;
}

void CShaderMgr::Free(void)
{
	for (auto& iter : m_mapShader)
		Safe_Delete(iter.second);
	m_mapShader.clear();
}
