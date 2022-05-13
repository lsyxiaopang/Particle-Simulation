#pragma once
/// <summary>
/// 这个库实现了对常见二维向量运算的实现
/// </summary>
/// 这个库可以初始化一个矢量(坐标),便于计算
class vec2
{
public:	
	/// <summary>
	/// Initializes a new instance of the <see cref="vec2"/> class.
	/// </summary>
	/// <param name="x">The x.</param>
	/// <param name="y">The y.</param>
	vec2(float x, float y);//初始化向量	
	float mod();//求模长	
	vec2 operator+(vec2& O);//向量和
	vec2 operator-(vec2& O);//向量差
	vec2 operator*(float O);//数乘
	float operator*(vec2& O);//点乘
	void reverse();//翻转
	void to_unit();//转换为单位矢
	static float cross_product(vec2& l, vec2& r);//叉乘(因为是二维情况,所以只会有浮点

	float x, y;
private:

};
