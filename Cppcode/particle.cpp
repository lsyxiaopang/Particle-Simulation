#include "particle.h"
/// <summary>
/// ��ʼ��һ������
/// </summary>
/// <param name="coordinate">��������</param>
/// <param name="velocity">��ʼ����</param>
/// <param name="radius">�뾶</param>
/// <param name="weight">����</param>
Particle::Particle(vec2 coordinate, vec2 velocity, float radius, float weight) : coordinate(coordinate), velocity(velocity), radius(radius), m(weight), crash_times(0)
{
}

/// <summary>
/// �������֮��ľ���
/// </summary>
/// <param name="other">��һ������</param>
/// <returns></returns>
float Particle::get_distance(Particle &other)
{
	vec2 x = this->coordinate - other.coordinate;
	return x.mod();
}

/// <summary>
/// ����Ƿ�����ײ(��������֮��ľ���С�ڰ뾶��)
/// </summary>
/// <param name="other">��һ������</param>
/// <returns></returns>
bool Particle::check_crash(Particle &other)
{
	float dis = other.radius + this->radius;
	return dis >= this->get_distance(other);
}
/// <summary>
/// ������ײ��Ϣ,������ǵõ��������໥��ײ������,���ǿ����ƶϳ�����ʱ�䲽�����ʵλ�����ٶ�
/// ע��,�ð汾������������뾶������
/// ���ڿռ������ⳡ����(��������ײ֮�ⶼ������ֱ���˶�
/// �ں���ִ�к�,������ֵ�ᱻ�޸�
/// </summary>
/// <param name="A">һ������</param>
/// <param name="B">��һ������</param>
/// <param name="time">ʱ��(���ڶ���ײ�ļ�¼)</param>
void Particle::handle_crash(Particle &A, Particle &B, float time)
{
	//ͨ��������η��������ײʱ��
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
	root = root / (2 * a); // ���õ�����
	if (isnan(root))
		return; //��ǽ��ײʱ���ܻ����ٶ���ͬ!
	vec2 acrashcoor = (A.velocity * root) - A.coordinate;
	vec2 bcrashcoor = (B.velocity * root) - B.coordinate;
	acrashcoor.reverse();
	bcrashcoor.reverse(); //���㷢����ײ��λ��
	vec2 ndc = acrashcoor - bcrashcoor;
	ndc.to_unit(); //������ߵ�λʸ
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
	bvel = bvel + vatt; //���᷽���ٶȽ���,��ֱ�����ٶȲ���

	A.coordinate = (nav * root) + acrashcoor;
	B.coordinate = (nbv * root) + bcrashcoor;
	A.velocity = nav;
	B.velocity = nbv; //��ײ���λ��ʱ��

	if (time > 10) //��ʼ��¼��ײ��Ϣ,ֻ��¼ʱ�����10�����
	{
		A.crash_times++;
		B.crash_times++;
	}
	return;
}
/// <summary>
/// ��ǰ�ƶ�һ��ʱ�䲽
/// ���û���ⳡ,������ֱ���˶�
/// </summary>
/// <param name="time_step">��ǰǰ����ʱ�䲽</param>
void Particle::run_step(float time_step)
{
	this->coordinate = this->velocity * time_step + this->coordinate;
	//������
	this->coordinate = vec2(0, -40) * time_step * (time_step / 2) + this->coordinate;
	this->velocity = vec2(0, -40) * time_step + this->velocity;
	return;
}