#pragma once
#include<math.h>
#include<vector>
#include"vec.h"
#include"particle.h"
using namespace std;
class Wall
{
public:
	Wall(vec2 A=vec2(0,0),vec2 C=vec2(500,500),float beg_time=10);//ͨ��ָ�����ε�A��Cʵ�ֻ���
	void check_handle_crash(Particle& p, float time);//������ܵ���ײ,(ʱ�������һЩ�������)

	float xmin, xmax, ymin, ymax;//ǽ�������
	vector<vec2> crash_velocity_vector;//������ײ(���ұ�)�����ӵ��ٶ�(���ڼ�¼��������)
private:
	float beg_time;//��ʼ��¼��ײʱ��

};

