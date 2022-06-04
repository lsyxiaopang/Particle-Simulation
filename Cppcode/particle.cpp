#include "particle.h"
/// <summary>
/// 初始化一个粒子
/// </summary>
/// <param name="coordinate">粒子坐标</param>
/// <param name="velocity">初始速率</param>
/// <param name="radius">半径</param>
/// <param name="weight">质量</param>
Particle::Particle(vec2 coordinate, vec2 velocity, float radius, float weight) : coordinate(coordinate), velocity(velocity), radius(radius), m(weight), crash_times(0)
{
}

/// <summary>
/// 获得粒子之间的距离
/// </summary>
/// <param name="other">另一个粒子</param>
/// <returns></returns>
float Particle::get_distance(Particle &other)
{
	vec2 x = this->coordinate - other.coordinate;
	return x.mod();
}

/// <summary>
/// 检查是否发生碰撞(两个粒子之间的距离小于半径和)
/// </summary>
/// <param name="other">另一个粒子</param>
/// <returns></returns>
bool Particle::check_crash(Particle &other)
{
	float dis = other.radius + this->radius;
	return dis >= this->get_distance(other);
}
/// <summary>
/// 处理碰撞信息,如果我们得到了两个相互碰撞的粒子,我们可以推断出其在时间步后的真实位置与速度
/// 注意,该版本不考虑质量与半径的区别
/// 且在空间中无外场存在(即除了碰撞之外都是匀速直线运动
/// 在函数执行后,参数的值会被修改
/// </summary>
/// <param name="A">一个粒子</param>
/// <param name="B">另一个粒子</param>
/// <param name="time">时间(用于对碰撞的记录)</param>
void Particle::handle_crash(Particle &A, Particle &B, float time)
{
	//通过构造二次方程求解碰撞时间
	float dis = A.radius + B.radius;
	vec2 delvel = A.velocity - B.velocity;
	vec2 delcoor = A.coordinate - B.coordinate;
	float a = delvel.mod();
	a = a * a;
	float c = delcoor.mod();
	c = c * c;
	c -= dis * dis;
	float b1, b2, b;
	b1 = -2 * delvel.x * delcoor.x;
	b2 = -2 * delvel.y * delcoor.y;
	b = b1 + b2;
	float root = (-b + (float)sqrt(b * b - 4 * a * c));
	root = root / (2 * a); // 求解得到方程
	if (isnan(root))
		return; //外墙碰撞时可能会有速度相同!
	vec2 acrashcoor = (A.velocity * root) - A.coordinate;
	vec2 bcrashcoor = (B.velocity * root) - B.coordinate;
	acrashcoor.reverse();
	bcrashcoor.reverse(); //计算发生碰撞的位置
	vec2 ndc = acrashcoor - bcrashcoor;
	ndc.to_unit(); //获得轴线的位矢
	//!
	vec2 avel = A.velocity;
	vec2 bvel = B.velocity;
	float m1 = A.m;
	float m2 = B.m;
	vec2 vatt = ndc * (avel * ndc);
	vec2 vbtt = ndc * (bvel * ndc);
	vec2 aven = avel - vatt;
	vec2 bven = bvel - vbtt;
	vec2 vm = (vatt * m1 + vbtt * m2) / (m1 + m2);
	vec2 rvmat = vatt - vm;
	vec2 rvmbt = vbtt - vm;
	vec2 navt = vatt - rvmat * 2;
	vec2 nbvt = vbtt - rvmbt * 2;
	vec2 nav = navt + aven;
	vec2 nbv = nbvt + bven;
	//!
	avel = avel - vatt;
	avel = avel + vbtt;
	bvel = bvel - vbtt;
	bvel = bvel + vatt; //沿轴方向速度交换,垂直于轴速度不变

	A.coordinate = (nav * root) + acrashcoor;
	B.coordinate = (nbv * root) + bcrashcoor;
	A.velocity = nav;
	B.velocity = nbv; //碰撞后的位置时间

	if (time > 10) //开始记录碰撞信息,只记录时间大于10的情况
	{
		A.crash_times++;
		B.crash_times++;
	}
	return;
}
/// <summary>
/// 向前移动一个时间步
/// 如果没有外场,则匀速直线运动
/// </summary>
/// <param name="time_step">向前前进的时间步</param>
void Particle::run_step(float time_step)
{
	this->coordinate = this->velocity * time_step + this->coordinate;
	//势能项
	this->coordinate = vec2(0, -40) * time_step * (time_step / 2) + this->coordinate;
	this->velocity = vec2(0, -40) * time_step + this->velocity;
	return;
}