#ifndef ShaderMgr_h__
#define ShaderMgr_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Shader.h"
BEGIN(Engine)

class ENGINE_DLL CShaderMgr : public CBase
{
	DECLARE_SINGLETON(CShaderMgr)

private:
	explicit CShaderMgr(void);
	virtual ~CShaderMgr(void);

public:
	HRESULT		Ready_Shader(const TCHAR* pShaderTag, CComponent* pInstance);
	CComponent*	Clone_Shader(const TCHAR* pShaderTag);

private:
	CComponent*		Find_Shader(const TCHAR* pShaderTag);
private:
	map<const TCHAR*, CComponent*>		m_mapShader;

public:
	virtual void Free(void);
};

END
#endif // ShaderMgr_h__
