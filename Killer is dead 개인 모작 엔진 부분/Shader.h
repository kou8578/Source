#ifndef Shader_h__
#define Shader_h__

#include "Engine_Defines.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader : public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShader(const CShader& rhs);
public:
	virtual ~CShader(void);

public:
	LPD3DXEFFECT		Get_EffectHandle(void) { return m_pEffect; }

	HRESULT Ready_Shader(const TCHAR* pFilePath);

public:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	LPD3DXEFFECT					m_pEffect;
	LPD3DXBUFFER					m_pErrMsg; // ������ �߻����� �� ���ڿ� ������ ���� ������ Ȯ���� �� �ְ� ���ִ� �޸�

public:
	static	CShader*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pFilePath);
	virtual CComponent* Clone(void);
	virtual void		Free(void);
	
};

END
#endif // Shader_h__
