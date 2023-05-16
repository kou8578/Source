matrix  g_matWorld;		// 상수 테이블 : 쉐이더에서는 내부에서 대신 사용은 가능하지만 대입이나 값 변형은 불가능하다.
matrix  g_matView;
matrix  g_matProj;

texture g_BaseTexture; 

sampler BaseSampler = sampler_state		// 샘플러 : 텍스쳐의 각종 상태를 정의하는 코드(필수)
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;					// 선형 보간 시 어떻게 하는가? : 비어 있는 픽셀의 위에 좌우, 십자가 형태로 평균 값을 구하여 값을 채워준다.
};

struct	VS_IN
{								
	vector		vPosition : POSITION;		// SYMANTIC : 멤버를 정의할 때 속성값으로 반드시 넣어줘야 하며 대문자로 써야함
	float2		vTexUV    : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
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
	//Out.vNormal  = mul(vector(In.vNormal.xyz, 0.f), matWVP);

	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN		// 픽셀 세이더 구조체에서는 POSITION 시멘틱을 절대 사용 할 수 없다.
{
	float2				vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector				vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);			// 2차원 텍스처에서 uv좌표에 해당하는 색을 추출하는 함수, 리턴 타입이 벡터 타입

	Out.vColor.a = 0.5f;
	
	return Out;
}

technique	Default_Device
{
	pass 
	{
		AlphaBlendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}
};