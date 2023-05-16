
vector		g_vLightPos;
float		g_fRange;

vector		g_vLightDir;
vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlDiffuse = (vector)1.f;
vector		g_vMtrlAmbient = (vector)1.f;
vector		g_vMtrlSpecular = (vector)1.f;
float		g_fPower = 10.f;

vector		g_vCamPosition;

matrix		g_matViewInv;
matrix		g_matProjInv;

texture		g_NormalTexture;

sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;
};

texture		g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};


struct PS_IN
{
	vector	vPosition : POSITION;
	float2	vTexUV : TEXCOORD;
};

struct PS_OUT
{
	vector	vShade : COLOR0;
	vector	vSpecular : COLOR1;
};

PS_OUT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector	vNormalInfo = tex2D(NormalSampler, In.vTexUV);
	vector	vDepthInfo = tex2D(DepthSampler, In.vTexUV);

	vector	vNormal = vector(vNormalInfo.xyz * 2.f - 1.f, 0.f);

	Out.vShade = (saturate(max(dot(normalize(g_vLightDir) * -1.f, normalize(vNormal)), 0.f)
		+ (g_vLightAmbient * g_vMtrlAmbient)))
		* (g_vLightDiffuse * g_vMtrlDiffuse);
	Out.vShade *= Out.vShade;
	Out.vShade.a = 1.f;
	// 빛의 반사벡터 * 시선벡터.

	vector	vReflect = reflect(normalize(g_vLightDir), normalize(vNormal));

	vector	vPosition;

	float	fViewZ = (vDepthInfo.y * 1000.f);

	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepthInfo.r * fViewZ;
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);


	vector	vLook = vPosition - g_vCamPosition;

	Out.vSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), g_fPower) * (g_vLightSpecular * g_vMtrlSpecular);
	Out.vSpecular.a = 0.f;
	return Out;
}

PS_OUT PS_MAIN_POINT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector	vNormalInfo = tex2D(NormalSampler, In.vTexUV);
	vector	vDepthInfo = tex2D(DepthSampler, In.vTexUV);

	vector	vNormal = vector(vNormalInfo.xyz * 2.f - 1.f, 0.f);

	vector	vPosition;

	float	fViewZ = (vDepthInfo.y * 1000.f);

	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepthInfo.r * fViewZ;
	vPosition.w = fViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);

	vector	vLightDir = vPosition - g_vLightPos;

	float	fDistance = length(vLightDir);

	float	fAtt = max(g_fRange - fDistance, 0.f) / g_fRange;


	Out.vShade = (saturate(max(dot(normalize(vLightDir) * -1.f, normalize(vNormal)), 0.f)
		+ (g_vLightAmbient * g_vMtrlAmbient)))
		* (g_vLightDiffuse * g_vMtrlDiffuse) * fAtt;


	////그림자의 값을 3단계로 나누어야한다
	////0,0.5,1
	//////일정값 이하는 아예 0으로
	//////일정값 이상은 아예 높게
	//////-1,0,1 저장
	//vector HighContrast = sign(Out.vShade - 0.5f);
	//HighContrast += 1; //0,1,2,가 됨
	//Out.vShade = saturate(HighContrast * 0.5f);
	//Out.vShade = smoothstep(0.04f, 0.06f, Out.vShade);
	Out.vShade.a = 1.f;
	// 빛의 반사벡터 * 시선벡터.

	vector	vReflect = reflect(normalize(vLightDir), normalize(vNormal));

	vector	vLook = vPosition - g_vCamPosition;

	Out.vSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), g_fPower) * (g_vLightSpecular * g_vMtrlSpecular) * fAtt;
	//Out.vSpecular = smoothstep(0.4f, 0.8f, Out.vSpecular);
	Out.vSpecular.a = 0.f;

	return Out;
}



technique	Default_Device
{
	pass Directional
	{
		ZWriteEnable = false;

		AlphaBlendEnable = true;

		SrcBlend = one;
		DestBlend = one;

		//SrcColor * one + DestColor *  one);

		VertexShader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN_DIRECTIONAL();
	}

	pass Point
	{
		ZWriteEnable = false;

		AlphaBlendEnable = true;

		SrcBlend = one;
		DestBlend = one;

		VertexShader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN_POINT();
	}
}
