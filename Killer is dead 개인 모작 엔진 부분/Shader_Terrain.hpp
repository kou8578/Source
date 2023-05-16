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
	vector		vNormal   : NORMAL; //명암표현을위한 노말백터
	vector		vTexUV    : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition	: POSITION;
	vector		vShade		: COLOR0;	//명암
	vector		vSpecular	: COLOR1; //물체가 반짝이는것
	float2		vTexUV		: TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	

	//들어온 노멀백터(법선백터)는 현재 로컬좌표이므로 월드좌표로 바꿔줘야한다.
	vector	vWorldNormal = mul(vector(In.vNormal.xyz, 0.f), g_matWorld);
	//조명의 방향또한 마찬가지
	vector	vWorldLightDir = g_vLightDir * -1.f;

	//조명의 방향과 노멀백터를 정규화하여 내적을한후, 0보다 큰값만을 빛의 세기로 사용한다.
	float	fIntensity = max(dot(normalize(vWorldLightDir), normalize(vWorldNormal)), 0.f);

	Out.vShade = fIntensity;
	Out.vShade.a = 1.f;

	// 스펙큘러 연산
	// 정반사광을구하는 과정
	vector	vReflect = reflect(normalize(g_vLightDir), normalize(vWorldNormal));
	vector	vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);
	
	vector vLook = g_vCamPos - vWorldPos;
	//max까지 연산후 pow함수로 g_fPower을 제곱연산하여 빛을 감쇠시켜준다.
	Out.vSpecular = pow(max(dot(normalize(vReflect), normalize(vLook)), 0.f), g_fPower);
	//알파값은 감쇠연산을 하지않기위해 초기화시켜준다.
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
	//퐁쉐이딩공식
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