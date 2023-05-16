// #include "Shader_Base.hpp"
// 
// float		g_fDetail = 0.f;
// float		g_fPower = 1.f;
// float		g_fHigh = 0;
// float		g_fLow = 0;
// 
// vector		g_vLightDir = (vector)0.f;
// vector		g_vCamPos;
// vector		g_vLightPos;
// 
// vector		g_vLightDiffuse;
// vector		g_vLightAmbient;
// vector		g_vLightSpecular;
// 
// vector		g_vMtrlDiffuse;
// vector		g_vMtrlAmbient;
// vector		g_vMtrlSpecular;
// 
// struct VS_IN
// {
// 	vector		vPosition : POSITION;
// 	vector		vNormal   : NORMAL;
// 	vector		vTexUV    : TEXCOORD0;
// };
// 
// struct VS_OUT
// {
// 	vector		vPosition	: POSITION;
// 	vector		vShade		: COLOR0;	//0번 랜더타겟
// 	vector		vSpecular	: COLOR1;	//1번 랜더타겟
// 	float2		vTexUV		: TEXCOORD0;
// };
// 
// VS_OUT VS_MAIN(VS_IN In)
// {
// 	VS_OUT			Out = (VS_OUT)0;
// 
// 	matrix			matWV, matWVP;
// 
// 	matWV = mul(g_matWorld, g_matView);
// 	matWVP = mul(matWV, g_matProj);
// 
// 	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
// 	
// 	vector	vWorldNormal = mul(vector(In.vNormal.xyz, 0.f), g_matWorld);
// 
// 	vector	vWorldLightDir = g_vLightDir * -1.f;
// 
// 	float	fIntensity = max(dot(normalize(vWorldLightDir), normalize(vWorldNormal)), 0.f);
// 
// 	Out.vShade = fIntensity;
// 	Out.vShade.a = 1.f;
// 	
// 
// 
// 	// 스펙큘러 연산
// 
// 	vector	vReflect = reflect(normalize(g_vLightDir), normalize(vWorldNormal));
// 	vector	vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);
// 	
// 	vector vLook = g_vCamPos - vWorldPos;
// 
// 	Out.vSpecular = pow(max(dot(normalize(vReflect), normalize(vLook)), 0.f), g_fPower);
// 	Out.vSpecular.a = 1.f;
// 
// 	Out.vTexUV = In.vTexUV;
// 
// 	return Out;
// }
// 
// struct	PS_IN 
// {
// 	//vector		vNormal : NORMAL;
// 	vector		vShade : COLOR0;	//0번 랜더 타겟
// 	vector		vSpecular : COLOR1; //1번 랜더 타겟
// 	float2		vTexUV : TEXCOORD0;
// };
// 
// struct PS_OUT
// {
// 	vector		vColor : COLOR0; //0번타겟과 1번타겟의 컬러를 합해서 내보내주고 있다.
// };
// 
// PS_OUT  PS_MAIN(PS_IN In)
// {
// 	PS_OUT			Out = (PS_OUT)0;
// 
// 	vector		vColor = tex2D(BaseSampler, In.vTexUV * g_fDetail);
// 
//   // ? = La * Ma + Ld * Md * (N . L)
// 
// 	Out.vColor = (vColor * (g_vLightDiffuse * g_vMtrlDiffuse))
// 		* (In.vShade + (g_vLightAmbient * g_vMtrlAmbient))
// 		+ (In.vSpecular * (g_vLightSpecular * g_vMtrlSpecular));
// 	//if (Out.vColor.r > g_fHigh) Out.vColor.r = 1.f;
// 	//else if (Out.vColor.r > g_fLow)	Out.vColor.r = 0.5f;
// 	//else Out.vColor.r = 0.f;
// 
// 	//if (Out.vColor.g > g_fHigh) Out.vColor.g = 1.f;
// 	//else if (Out.vColor.g > g_fLow)	Out.vColor.g = 0.5f;
// 	//else Out.vColor.g = 0.f;
// 
// 	//if (Out.vColor.b > g_fHigh) Out.vColor.b = 1.f;
// 	//else if (Out.vColor.b > g_fLow)	Out.vColor.b = 0.5f;
// 	//else Out.vColor.b = 0.f;
// 	return Out;
// }
// 
// 
// technique	Default_Device
// {
// 	pass
// 	{	
// 		vertexshader = compile vs_3_0 VS_MAIN();
// 		pixelshader = compile ps_3_0 PS_MAIN();
// 	}
// };


#include "Shader_Base.hpp"

float		g_fDetail = 0.f;

struct VS_IN
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	vector		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));

	Out.vTexUV = In.vTexUV;

	Out.vProjPos = Out.vPosition;

	return Out;
}

struct	PS_IN
{
	//vector		vNormal : NORMAL;
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos : TEXCOORD1;
};

struct PS_OUT
{
	vector		vDiffuse : COLOR0;
	vector		vNormal : COLOR1;
	vector		vDepth : COLOR2;
};

PS_OUT  PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vDiffuse = tex2D(BaseSampler, In.vTexUV);

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.0f, 0.f, 0.f);

	return Out;
}

PS_OUT		PS_MAIN_ALPHA(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vDiffuse = tex2D(BaseSampler, In.vTexUV);

	return Out;
}



technique	Default_Device
{
	pass Default
{
	zenable = true;
alphatestenable = true;
alphafunc = Greater;
alpharef = 100;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_MAIN();
}

pass alphaTesting
{
	alphatestenable = true;
alphafunc = Greater;
alpharef = 0xc0;
cullmode = none;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_MAIN_ALPHA();
}
};