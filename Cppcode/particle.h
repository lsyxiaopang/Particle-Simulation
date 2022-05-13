#pragma once
#include<math.h>
#include"vec.h"
class Particle
{
public:
	Particle(vec2 coordinate=vec2(0,0), vec2 velocity=vec2(0,0),float radius=0,float weight=0);
	float get_distance(Particle& other);//获取两个粒子之间的距离
	bool check_crash(Particle& other);//检查是否发生碰撞
	static void handle_crash(Particle& A, Particle& B,float time);//处理两个粒子碰撞的情况
	void run_step(float time_step);//向前移动一个时间步

	vec2 coordinate;//粒子坐标
	vec2 velocity;//粒子速率
	float radius;//半径
	float m;//质量
	int crash_times;//粒子受到的碰撞数记录
private:

};

