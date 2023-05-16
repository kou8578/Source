#include "Transform.h"

USING(Engine)

Engine::CTransform::CTransform(void)
	: m_vScale(1.f, 1.f, 1.f)
	, m_vAngle(0.f, 0.f, 0.f)
{

}

Engine::CTransform::~CTransform(void)
{
	Free();
}

void CTransform::Chase_Target(const _vec3 * pTargetPos, const _float & fSpeed)
{
	_vec3	vDir = *pTargetPos - m_vInfo[INFO_POS];
	
	m_vInfo[INFO_POS] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed;

	_matrix		matScale, matRot, matTrans;

	matRot = *Compute_LookAtTarget(pTargetPos);

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, 
							m_vInfo[INFO_POS].x,
							m_vInfo[INFO_POS].y,
							m_vInfo[INFO_POS].z);

	m_matWorld = matScale * matRot * matTrans;

}

void CTransform::LookAT_Target(const _vec3 _vTargerPos, const _float& fSpeed)
{
	//_float fRadian = D3DXToDegree(180.f);
	//Set_Rotation(ROT_Y, fRadian +fAngle);


	_vec2 TargetPos2, InfoPos2, InfoLook2;
	TargetPos2.x = _vTargerPos.x;
	TargetPos2.y = _vTargerPos.z;

	InfoPos2.x = m_vInfo[INFO_POS].x;
	InfoPos2.y = m_vInfo[INFO_POS].z;

	InfoLook2.x = m_vInfo[INFO_UP].x;
	InfoLook2.y = m_vInfo[INFO_UP].y;

	_vec2		vDir2;
	vDir2 = TargetPos2 - InfoPos2;
	D3DXVec2Normalize(&vDir2, &vDir2);
	D3DXVec2Normalize(&InfoLook2, &InfoLook2);

	_float fAngle = acosf(D3DXVec2Dot(&vDir2, &InfoLook2));
	_float Degree = D3DXToDegree(fAngle);
	if (TargetPos2.x < InfoPos2.x)
		Set_Rotation(ROT_Y, -fAngle);
	else
		Set_Rotation(ROT_Y, fAngle);
//  	if (TargetPos2.x < InfoPos2.x)
//  	{
// 		//자신의 각도값보다 작으면 무조건 - 크면 +
// 		if (*(((float*)&m_vAngle) + ROT_Y) > -fAngle)
// 			Rotation(ROT_Y, -D3DXToRadian(fSpeed));
// 		else
// 			Rotation(ROT_Y, D3DXToRadian(fSpeed));
//  
//  	}
// 	else
// 	{
// 		//자신의 각도값보다 작으면 무조건 - 크면 +
// 		if (*(((float*)&m_vAngle) + ROT_Y) > fAngle)
// 			Rotation(ROT_Y, -D3DXToRadian(fSpeed));
// 		else
// 			Rotation(ROT_Y, D3DXToRadian(fSpeed));
// 	}
// 		Set_Rotation(ROT_Y, -fAngle);
// 	else
// 		Set_Rotation(ROT_Y, fAngle);

	
}

void CTransform::LookAT_Target_Quick(const _vec3 _vTargerPos)
{
	_vec2 TargetPos2, InfoPos2, InfoLook2;
	TargetPos2.x = _vTargerPos.x;
	TargetPos2.y = _vTargerPos.z;

	InfoPos2.x = m_vInfo[INFO_POS].x;
	InfoPos2.y = m_vInfo[INFO_POS].z;

	InfoLook2.x = m_vInfo[INFO_UP].x;
	InfoLook2.y = m_vInfo[INFO_UP].y;

	_vec2		vDir2;
	vDir2 = TargetPos2 - InfoPos2;
	D3DXVec2Normalize(&vDir2, &vDir2);
	D3DXVec2Normalize(&InfoLook2, &InfoLook2);

	_float fAngle = acosf(D3DXVec2Dot(&vDir2, &InfoLook2));
	_float Degree = D3DXToDegree(fAngle);
	if (TargetPos2.x < InfoPos2.x)
		Set_Rotation(ROT_Y, -fAngle);
	else
		Set_Rotation(ROT_Y, fAngle);
}

const _matrix* CTransform::Compute_LookAtTarget(const _vec3 * pTargetPos)
{
	_vec3		vDir = *pTargetPos - m_vInfo[INFO_POS];

	_vec3 vAxis = *D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDir);

	_matrix		matRot;
	_vec3		vUp;

	return D3DXMatrixRotationAxis(&matRot, &vAxis, 
		acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir),
						  D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]))));
}

HRESULT Engine::CTransform::Ready_Transform(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	for (_int i = 0; i < 4; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_float) * 3);


	return S_OK;
}

void Engine::CTransform::Update_Component(const _float& fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matNRotWorld);

	for (_int i = 0; i < INFO_END - 1; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(float) * 3);

	for (_int i = 0; i < INFO_END - 1; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	}
	for (_int i = 0; i < INFO_END; ++i)
		memcpy(&m_matNRotWorld.m[i][0], &m_vInfo[i], sizeof(float) * 3);
	_matrix		matRot[ROT_END];
	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	for (_int i = 0; i < INFO_END - 1; ++i)
	{
		for (_int j = 0; j < ROT_END; ++j)
		{
			D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRot[j]);
		}
	}

	for (_int i = 0; i < 4; ++i)
		memcpy(&m_matWorld.m[i][0], m_vInfo[i], sizeof(float) * 3);
	
}



Engine::CTransform* Engine::CTransform::Create(void)
{
	CTransform*	pInstance = new CTransform;

	if (FAILED(pInstance->Ready_Transform()))
		Engine::Safe_Delete(pInstance);

	return pInstance;
}

void Engine::CTransform::Free(void)
{

}

