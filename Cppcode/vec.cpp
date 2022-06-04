#include <math.h>
//�������Գ�ʼ��һ��ʸ��(����),���ڼ���
#include "vec.h"

vec2::vec2(float x, float y)
	: x(x), y(y)
{
}

float vec2::mod()
{
	float retmod;
	retmod = this->x * this->x + this->y * this->y;
	retmod = sqrt(retmod);
	if (retmod == 0)
		retmod = 1e-8;
	return retmod;
}
/// <summary>
/// T�������
/// </summary>
/// <param name="O">Ҫ��͵�ʸ��</param>
/// <returns></returns>
vec2 vec2::operator+(vec2 &O)
{
	float x = this->x + O.x;
	float y = this->y + O.y;
	return vec2(x, y);
}
/// <summary>
/// ����֮��
/// </summary>
/// <param name="O">Ҫ����ʸ��</param>
/// <returns></returns>
vec2 vec2::operator-(vec2 &O)
{
	float x = this->x - O.x;
	float y = this->y - O.y;
	return vec2(x, y);
}
/// <summary>
/// ����������
/// </summary>
/// <param name="O">���ű���</param>
/// <returns></returns>
vec2 vec2::operator*(float O)
{
	return vec2(this->x * O, this->y * O);
}
/// <summary>
/// �����ĵ��
/// </summary>
/// <param name="O">��һ��ʸ��</param>
/// <returns></returns>
float vec2::operator*(vec2 &O)
{
	float a, b;
	a = this->x * O.x;
	b = this->y * O.y;
	return a + b;
}
vec2 vec2::operator/(float O)
{
	return vec2(this->x / O, this->y / O);
}
/// <summary>
/// �����Ĳ��
/// ע��,���������Ƕ�ά����,���������һ��������(���ϻ�����)
/// </summary>
/// <param name="l">�������.</param>
/// <param name="r">�Ҳ�����.</param>
/// <returns></returns>
float vec2::cross_product(vec2 &l, vec2 &r)
{
	float a = l.x * r.y;
	float b = l.y * r.x;
	return a - b;
}
/// <summary>
/// �����������ת
/// </summary>
void vec2::reverse()
{
	this->x = -this->x;
	this->y = -this->y;
}
/// <summary>
/// ��������ת���ɵ�λ����
/// </summary>
void vec2::to_unit()
{
	float mod = this->mod();
	this->x /= mod;
	this->y /= mod;
}
