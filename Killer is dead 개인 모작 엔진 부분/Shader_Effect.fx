matrix  g_matWorld;		// 상수 테이블 : 쉐이더에서는 내부에서 대신 사용은 가능하지만 대입이나 값 변형은 불가능하다.
matrix  g_matView;
matrix  g_matProj;

texture g_BaseTexture;

sampler BaseSampler = sampler_state		// 샘플러 : 텍스쳐의 각종 상태를 정의하는 코드(필수)
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;	
	mipfilter = linear;
};

texture	g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

struct	VS_IN
{
	float3		vPosition : POSITION;		// SYMANTIC : 멤버를 정의할 때 속성값으로 반드시 넣어줘야 하며 대문자로 써야함
	float2		vTexUV    : TEXCOORD0;
};

struct	VS_IN_POINTSPRITE
{
	float3		vPosition : POSITION;		// SYMANTIC : 멤버를 정의할 때 속성값으로 반드시 넣어줘야 하며 대문자로 써야함
	float		fPSize : PSIZE;
	float2		vTexUV    : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV    : TEXCOORD0;
	vector		vProjPos  : TEXCOORD1;
};

struct VS_OUT_POINTSPRITE
{
	vector		vPosition : POSITION;
	float		fPSize : PSIZE;
	float2		vTexUV    : TEXCOORD0;	
};

//  정점 쉐이더

VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		 matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	Out.vProjPos = Out.vPosition;

	return Out;
}

VS_OUT_POINTSPRITE		VS_MAIN_POINTSPRITE(VS_IN_POINTSPRITE In)
{
	VS_OUT_POINTSPRITE		Out = (VS_OUT_POINTSPRITE)0;

	matrix		 matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	// 윈도우사이즈 와이 * 포인트사이즈 * (1.f / (이펙트의뷰.z)^2)

	

	Out.fPSize = (600.0f * In.fPSize) * sqrt(1.f / pow((Out.vPosition.w), 2.f));
	Out.vTexUV = In.vTexUV;

	return Out;
}


struct PS_IN
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	vector		vProjPos  : TEXCOORD1;
};

struct PS_IN_POINTSPRITE
{
	vector		vPosition : POSITION;
	float		fPSize : PSIZE;
	float2		vTexUV    : TEXCOORD0;
};

struct PS_OUT
{
	vector				vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	

	float2	vNewUV = In.vProjPos.xy / In.vProjPos.w;

	vNewUV.x = vNewUV.x * 0.5f + 0.5f;
	vNewUV.y = vNewUV.y * -0.5f + 0.5f;

	vector	vDepthInfo = tex2D(DepthSampler, vNewUV);

	float	fViewZ = vDepthInfo.y * 1000.f;	

	Out.vColor.a = Out.vColor.a * saturate(fViewZ - In.vProjPos.w) * 2.f ;

	


	return Out;
}

PS_OUT PS_MAIN_POINTSPRITE(PS_IN_POINTSPRITE In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	return Out;
}


technique	Default_Device
{
	pass Normal
	{
		AlphaBlendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass PointSprite
	{
		AlphaBlendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		ZWriteEnable = false;

		PointSpriteEnable = true;

		vertexshader = compile vs_3_0 VS_MAIN_POINTSPRITE();
		pixelshader = compile ps_3_0 PS_MAIN_POINTSPRITE();
	}
};