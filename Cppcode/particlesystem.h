#pragma once
#include<math.h>
#include<vector>
#include"particle.h"
#include"wall.h"

using namespace std;
class ParticleSystem
{
public:
	ParticleSystem(Particle* particle_array, int array_length, Wall* wall,float d=0);//初始化一个粒子系统
	float run_step(int m, int n, vector<Particle*>* grid);//向前前进一个时间步,返回步长
	float auto_run(float time, void* (*pf)(ParticleSystem*, void*, void*) = NULL, void* args = NULL);//自动运算并调用探针函数,args为向最后一个传递的参数
	float get_time();
	Particle* particle_array;//粒子
	int length;//粒子数
	float d;//行走长度
	Wall* pwall;//墙壁的指针
private:
	float time;	//时间
	float max_vec();//获得最大速率
	void check_vec(vector<Particle*>* pv,Particle* tp, float step);
};

