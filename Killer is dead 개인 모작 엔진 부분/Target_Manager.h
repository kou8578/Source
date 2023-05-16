#pragma once

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CTarget;
class ENGINE_DLL CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)
public:
	typedef struct tagTargetDesc
	{
		tagTargetDesc(_uint	_iWidth, _uint _iHeight, D3DFORMAT _Format) : iWidth(_iWidth), iHeight(_iHeight), Format(_Format) { }

		_uint		iWidth;
		_uint		iHeight;
		D3DFORMAT	Format;
	}TARGETDESC;
private:
	explicit CTarget_Manager();
	virtual ~CTarget_Manager();
public:
	HRESULT Add_Target(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pTargetTag, const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR ClearColor);
	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);
public: // For.Deferred Rendering
	HRESULT Begin_MRT(const _tchar* pMRTTag);
	HRESULT End_MRT(const _tchar* pMRTTag);
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const _tchar* pTargetTag, const char* pConstantName);
//#ifdef _DEBUG
public:
	HRESULT Ready_DebugBuffer(const _tchar* pTargetTag, const _float& fStartX, const _float& fStartY, const _float& fSizeX, const _float& fSizeY);
	HRESULT Render_DebugBuffer(const _tchar* pMRTTag);
//#endif
private:
	map<const _tchar*, CTarget*>				m_mapTargets;
	typedef map<const _tchar*, CTarget*>		MAPTARGET;

private: // 특정 태그에 타겟들을 여러개 모아둘께.
	map<const _tchar*, list<CTarget*>>			m_mapMRT;
	typedef map<const _tchar*, list<CTarget*>>	MAPMRT;
private:
	CTarget* Find_Target(const _tchar* pTargetTag);
	list<CTarget*>* Find_MRT(const _tchar* pMRTTag);
protected:
	virtual void Free();
};

END