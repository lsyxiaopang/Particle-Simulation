#pragma once
/// <summary>
/// �����ʵ���˶Գ�����ά���������ʵ��
/// </summary>
/// �������Գ�ʼ��һ��ʸ��(����),���ڼ���
class vec2
{
public:	
	/// <summary>
	/// Initializes a new instance of the <see cref="vec2"/> class.
	/// </summary>
	/// <param name="x">The x.</param>
	/// <param name="y">The y.</param>
	vec2(float x, float y);//��ʼ������	
	float mod();//��ģ��	
	vec2 operator+(vec2& O);//������
	vec2 operator-(vec2& O);//������
	vec2 operator*(float O);//����
	float operator*(vec2& O);//���
	void reverse();//��ת
	void to_unit();//ת��Ϊ��λʸ
	static float cross_product(vec2& l, vec2& r);//���(��Ϊ�Ƕ�ά���,����ֻ���и���

	float x, y;
private:

};
