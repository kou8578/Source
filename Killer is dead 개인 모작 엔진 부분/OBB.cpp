#include "OBB.h"
#include "Collider.h"
USING(Engine)

Engine::COBB::COBB(void)
{

}

Engine::COBB::~COBB(void)
{
	Free();
}




_bool COBB::CheckOBB(OBB* _A_OBB, OBB* _B_OBB)
{
	double c[3][3];
	double absC[3][3];
	double d[3];

	double r0, r1, r;
	int i;

	const double cutoff = 0.999999;
	bool existsParallelPair = false;

	D3DXVECTOR3 diff = _A_OBB->vCenterPos - _B_OBB->vCenterPos;



	for (i = 0; i < 3; ++i)
	{
		c[0][i] = D3DXVec3Dot(&_A_OBB->vNormalLine[0], &_B_OBB->vNormalLine[i]);
		absC[0][i] = abs(c[0][i]);
		if (absC[0][i] > cutoff)
			existsParallelPair = true;
	}
	d[0] = D3DXVec3Dot(&diff, &_A_OBB->vNormalLine[0]);
	r = abs(d[0]);
	r0 = _A_OBB->fNormalLine_Len[0];
	r1 = _B_OBB->fNormalLine_Len[0] * absC[0][0] + _B_OBB->fNormalLine_Len[1] * absC[0][1] + _B_OBB->fNormalLine_Len[2] * absC[0][2];

	if (r > r0 + r1)
		return FALSE;



	for (i = 0; i < 3; ++i)
	{
		c[1][i] = D3DXVec3Dot(&_A_OBB->vNormalLine[1], &_B_OBB->vNormalLine[i]);
		absC[1][i] = abs(c[1][i]);
		if (absC[1][i] > cutoff)
			existsParallelPair = true;
	}
	d[1] = D3DXVec3Dot(&diff, &_A_OBB->vNormalLine[1]);
	r = abs(d[1]);
	r0 = _A_OBB->fNormalLine_Len[1];
	r1 = _B_OBB->fNormalLine_Len[0] * absC[1][0] + _B_OBB->fNormalLine_Len[1] * absC[1][1] + _B_OBB->fNormalLine_Len[2] * absC[1][2];

	if (r > r0 + r1)
		return FALSE;



	for (i = 0; i < 3; ++i)
	{
		c[2][i] = D3DXVec3Dot(&_A_OBB->vNormalLine[2], &_B_OBB->vNormalLine[i]);
		absC[2][i] = abs(c[2][i]);
		if (absC[2][i] > cutoff)
			existsParallelPair = true;
	}
	d[2] = D3DXVec3Dot(&diff, &_A_OBB->vNormalLine[2]);
	r = abs(d[2]);
	r0 = _A_OBB->fNormalLine_Len[2];
	r1 = _B_OBB->fNormalLine_Len[0] * absC[2][0] + _B_OBB->fNormalLine_Len[1] * absC[2][1] + _B_OBB->fNormalLine_Len[2] * absC[2][2];

	if (r > r0 + r1)
		return FALSE;



	r = abs(D3DXVec3Dot(&diff, &_B_OBB->vNormalLine[0]));
	r0 = _A_OBB->fNormalLine_Len[0] * absC[0][0] + _A_OBB->fNormalLine_Len[1] * absC[1][0] + _A_OBB->fNormalLine_Len[2] * absC[2][0];
	r1 = _B_OBB->fNormalLine_Len[0];

	if (r > r0 + r1)
		return FALSE;



	r = abs(D3DXVec3Dot(&diff, &_B_OBB->vNormalLine[1]));
	r0 = _A_OBB->fNormalLine_Len[0] * absC[0][1] + _A_OBB->fNormalLine_Len[1] * absC[1][1] + _A_OBB->fNormalLine_Len[2] * absC[2][1];
	r1 = _B_OBB->fNormalLine_Len[1];

	if (r > r0 + r1)
		return FALSE;



	r = abs(D3DXVec3Dot(&diff, &_B_OBB->vNormalLine[2]));
	r0 = _A_OBB->fNormalLine_Len[0] * absC[0][2] + _A_OBB->fNormalLine_Len[1] * absC[1][2] + _A_OBB->fNormalLine_Len[2] * absC[2][2];
	r1 = _B_OBB->fNormalLine_Len[2];

	if (r > r0 + r1)
		return FALSE;



	if (existsParallelPair == true)
		return TRUE;



	r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
	r0 = _A_OBB->fNormalLine_Len[1] * absC[2][0] + _A_OBB->fNormalLine_Len[2] * absC[1][0];
	r1 = _B_OBB->fNormalLine_Len[1] * absC[0][2] + _B_OBB->fNormalLine_Len[2] * absC[0][1];
	if (r > r0 + r1)
		return FALSE;



	r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
	r0 = _A_OBB->fNormalLine_Len[1] * absC[2][1] + _A_OBB->fNormalLine_Len[2] * absC[1][1];
	r1 = _B_OBB->fNormalLine_Len[0] * absC[0][2] + _B_OBB->fNormalLine_Len[2] * absC[0][0];
	if (r > r0 + r1)
		return FALSE;



	r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
	r0 = _A_OBB->fNormalLine_Len[1] * absC[2][2] + _A_OBB->fNormalLine_Len[2] * absC[1][2];
	r1 = _B_OBB->fNormalLine_Len[0] * absC[0][1] + _B_OBB->fNormalLine_Len[1] * absC[0][0];
	if (r > r0 + r1)
		return FALSE;



	r = abs(d[0] * c[2][0] - d[2] * c[0][0]);
	r0 = _A_OBB->fNormalLine_Len[0] * absC[2][0] + _A_OBB->fNormalLine_Len[2] * absC[0][0];
	r1 = _B_OBB->fNormalLine_Len[1] * absC[1][2] + _B_OBB->fNormalLine_Len[2] * absC[1][1];
	if (r > r0 + r1)
		return FALSE;



	r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
	r0 = _A_OBB->fNormalLine_Len[0] * absC[2][1] + _A_OBB->fNormalLine_Len[2] * absC[0][1];
	r1 = _B_OBB->fNormalLine_Len[0] * absC[1][2] + _B_OBB->fNormalLine_Len[2] * absC[1][0];
	if (r > r0 + r1)
		return FALSE;



	r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
	r0 = _A_OBB->fNormalLine_Len[0] * absC[2][2] + _A_OBB->fNormalLine_Len[2] * absC[0][2];
	r1 = _B_OBB->fNormalLine_Len[0] * absC[1][1] + _B_OBB->fNormalLine_Len[1] * absC[1][0];
	if (r > r0 + r1)
		return FALSE;



	r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
	r0 = _A_OBB->fNormalLine_Len[0] * absC[1][0] + _A_OBB->fNormalLine_Len[1] * absC[0][0];
	r1 = _B_OBB->fNormalLine_Len[1] * absC[2][2] + _B_OBB->fNormalLine_Len[2] * absC[2][1];
	if (r > r0 + r1)
		return FALSE;



	r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
	r0 = _A_OBB->fNormalLine_Len[0] * absC[1][1] + _A_OBB->fNormalLine_Len[1] * absC[0][1];
	r1 = _B_OBB->fNormalLine_Len[0] * absC[2][2] + _B_OBB->fNormalLine_Len[2] * absC[2][0];
	if (r > r0 + r1)
		return FALSE;



	r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
	r0 = _A_OBB->fNormalLine_Len[0] * absC[1][2] + _A_OBB->fNormalLine_Len[1] * absC[0][2];
	r1 = _B_OBB->fNormalLine_Len[0] * absC[2][1] + _B_OBB->fNormalLine_Len[1] * absC[2][0];
	if (r > r0 + r1)
		return FALSE;



	return TRUE;


}




Engine::COBB* Engine::COBB::Create(void)
{
	COBB*	pInstance = new COBB;
	return pInstance;
}

void Engine::COBB::Free(void)
{

}

