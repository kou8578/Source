#include "Shader_Base.hpp"

float		g_fDetail = 0.f;
float		g_fPower = 1.f;

vector		g_vLightDir = (vector)0.f;
vector		g_vCamPos;

vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlDiffuse;
vector		g_vMtrlAmbient;
vector		g_vMtrlSpecular;

struct VS_IN
{
	vector		vPosition : POSITION;
	vector		vNormal   : NORMAL; //���ǥ�������� �븻����
	vector		vTexUV    : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition	: POSITION;
	vector		vShade		: COLOR0;	//���
	vector		vSpecular	: COLOR1; //��ü�� ��¦�̴°�
	float2		vTexUV		: TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	

	//���� ��ֹ���(��������)�� ���� ������ǥ�̹Ƿ� ������ǥ�� �ٲ�����Ѵ�.
	vector	vWorldNormal = mul(vector(In.vNormal.xyz, 0.f), g_matWorld);
	//������ ������� ��������
	vector	vWorldLightDir = g_vLightDir * -1.f;

	//������ ����� ��ֹ��͸� ����ȭ�Ͽ� ����������, 0���� ū������ ���� ����� ����Ѵ�.
	float	fIntensity = max(dot(normalize(vWorldLightDir), normalize(vWorldNormal)), 0.f);

	Out.vShade = fIntensity;
	Out.vShade.a = 1.f;

	// ����ŧ�� ����
	// ���ݻ籤�����ϴ� ����
	vector	vReflect = reflect(normalize(g_vLightDir), normalize(vWorldNormal));
	vector	vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);
	
	vector vLook = g_vCamPos - vWorldPos;
	//max���� ������ pow�Լ��� g_fPower�� ���������Ͽ� ���� ��������ش�.
	Out.vSpecular = pow(max(dot(normalize(vReflect), normalize(vLook)), 0.f), g_fPower);
	//���İ��� ���迬���� �����ʱ����� �ʱ�ȭ�����ش�.
	Out.vSpecular.a = 1.f;

	Out.vTexUV = In.vTexUV;

	return Out;
}

struct	PS_IN 
{
	//vector		vNormal : NORMAL;
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT  PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vColor = tex2D(BaseSampler, In.vTexUV * g_fDetail);

  // ? = La * Ma + Ld * Md * (N . L)
	//�����̵�����
	Out.vColor = (vColor * (g_vLightDiffuse * g_vMtrlDiffuse)) 
		* (In.vShade + ( g_vLightAmbient * g_vMtrlAmbient)) 
		+(In.vSpecular * (g_vLightSpecular * g_vMtrlSpecular)) ;

	return Out;
}


technique	Default_Device
{
	pass
	{	
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}
};