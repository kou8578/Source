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
sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;

minfilter = linear;
magfilter = linear;					// 선형 보간 시 어떻게 하는가? : 비어 있는 픽셀의 위에 좌우, 십자가 형태로 평균 값을 구하여 값을 채워준다.
};