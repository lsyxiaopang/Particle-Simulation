#pragma once
#include<math.h>
#include"vec.h"
class Particle
{
public:
	Particle(vec2 coordinate=vec2(0,0), vec2 velocity=vec2(0,0),float radius=0,float weight=0);
	float get_distance(Particle& other);//��ȡ��������֮��ľ���
	bool check_crash(Particle& other);//����Ƿ�����ײ
	static void handle_crash(Particle& A, Particle& B,float time);//��������������ײ�����
	void run_step(float time_step);//��ǰ�ƶ�һ��ʱ�䲽

	vec2 coordinate;//��������
	vec2 velocity;//��������
	float radius;//�뾶
	float m;//����
	int crash_times;//�����ܵ�����ײ����¼
private:

};

