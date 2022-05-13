#pragma once
#include<math.h>
#include<vector>
#include"particle.h"
#include"wall.h"

using namespace std;
class ParticleSystem
{
public:
	ParticleSystem(Particle* particle_array, int array_length, Wall* wall,float d=0);//��ʼ��һ������ϵͳ
	float run_step(int m, int n, vector<Particle*>* grid);//��ǰǰ��һ��ʱ�䲽,���ز���
	float auto_run(float time, void* (*pf)(ParticleSystem*, void*, void*) = NULL, void* args = NULL);//�Զ����㲢����̽�뺯��,argsΪ�����һ�����ݵĲ���
	float get_time();
	Particle* particle_array;//����
	int length;//������
	float d;//���߳���
	Wall* pwall;//ǽ�ڵ�ָ��
private:
	float time;	//ʱ��
	float max_vec();//����������
	void check_vec(vector<Particle*>* pv,Particle* tp, float step);
};

