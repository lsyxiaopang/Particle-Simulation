#pragma once
#include<math.h>
#include<vector>
#include"vec.h"
#include"particle.h"
using namespace std;
class Wall
{
public:
	Wall(vec2 A=vec2(0,0),vec2 C=vec2(500,500),float beg_time=10);//通过指定矩形的A和C实现绘制
	void check_handle_crash(Particle& p, float time);//处理可能的碰撞,(时间可以做一些别的事情)

	float xmin, xmax, ymin, ymax;//墙相关数据
	vector<vec2> crash_velocity_vector;//发生碰撞(与右壁)的粒子的速度(用于记录碰壁速率)
private:
	float beg_time;//开始记录碰撞时间

};

