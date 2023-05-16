matrix  g_matWorld;		// ��� ���̺� : ���̴������� ���ο��� ��� ����� ���������� �����̳� �� ������ �Ұ����ϴ�.
matrix  g_matView;
matrix  g_matProj;

texture g_BaseTexture;

sampler BaseSampler = sampler_state		// ���÷� : �ؽ����� ���� ���¸� �����ϴ� �ڵ�(�ʼ�)
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;					// ���� ���� �� ��� �ϴ°�? : ��� �ִ� �ȼ��� ���� �¿�, ���ڰ� ���·� ��� ���� ���Ͽ� ���� ä���ش�.
};
sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;

minfilter = linear;
magfilter = linear;					// ���� ���� �� ��� �ϴ°�? : ��� �ִ� �ȼ��� ���� �¿�, ���ڰ� ���·� ��� ���� ���Ͽ� ���� ä���ش�.
};