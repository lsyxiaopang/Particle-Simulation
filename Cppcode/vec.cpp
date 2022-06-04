#include <math.h>
//这个库可以初始化一个矢量(坐标),便于计算
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
/// T向量求和
/// </summary>
/// <param name="O">要求和的矢量</param>
/// <returns></returns>
vec2 vec2::operator+(vec2 &O)
{
	float x = this->x + O.x;
	float y = this->y + O.y;
	return vec2(x, y);
}
/// <summary>
/// 向量之差
/// </summary>
/// <param name="O">要求差的矢量</param>
/// <returns></returns>
vec2 vec2::operator-(vec2 &O)
{
	float x = this->x - O.x;
	float y = this->y - O.y;
	return vec2(x, y);
}
/// <summary>
/// 向量的数乘
/// </summary>
/// <param name="O">缩放比例</param>
/// <returns></returns>
vec2 vec2::operator*(float O)
{
	return vec2(this->x * O, this->y * O);
}
/// <summary>
/// 向量的点乘
/// </summary>
/// <param name="O">另一个矢量</param>
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
/// 向量的叉乘
/// 注意,由于这里是二维向量,故最后结果是一个浮点数(向上或向下)
/// </summary>
/// <param name="l">左操作数.</param>
/// <param name="r">右操作数.</param>
/// <returns></returns>
float vec2::cross_product(vec2 &l, vec2 &r)
{
	float a = l.x * r.y;
	float b = l.y * r.x;
	return a - b;
}
/// <summary>
/// 将这个向量翻转
/// </summary>
void vec2::reverse()
{
	this->x = -this->x;
	this->y = -this->y;
}
/// <summary>
/// 将该向量转换成单位向量
/// </summary>
void vec2::to_unit()
{
	float mod = this->mod();
	this->x /= mod;
	this->y /= mod;
}
