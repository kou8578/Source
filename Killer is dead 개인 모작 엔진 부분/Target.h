#pragma once

#include "Engine_Defines.h"
#include "Base.h"

// 용도 : 렌더타겟 하나를 대표하는 객체.
// 렌더타겟을 보유(생성)하고 있는 객체다.

// 


BEGIN(Engine)

class CTarget final : public CBase
{
private:
	explicit CTarget(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CTarget() = default;
public:
	HRESULT Ready_Target(const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR ClearColor);
public:
	HRESULT Clear_Target();
	HRESULT SetUp_OnGraphicDev(const _uint& iIndex);
	HRESULT SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName);
	HRESULT Release_OnGraphicDev(const _uint& iIndex);
//#ifdef _DEBUG
public:
	HRESULT Ready_DebugBuffer(const _float& fStartX, const _float& fStartY, const _float& fSizeX, const _float& fSizeY);
	HRESULT Render_DebugBuffer();
//#endif
private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = nullptr;
	LPDIRECT3DTEXTURE9			m_pTarget_Texture = nullptr;
	LPDIRECT3DSURFACE9			m_pTarget_Surface = nullptr;
	LPDIRECT3DSURFACE9			m_pOld_Surface = nullptr;
	D3DXCOLOR					m_Clear_Color;
//#ifdef _DEBUG
private:
	LPDIRECT3DVERTEXBUFFER9		m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9		m_pIB = nullptr;
//#endif

public:
	static CTarget* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _uint& iWidht, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR ClearColor);
	virtual void Free();
};

END