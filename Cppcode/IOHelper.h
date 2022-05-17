#pragma once
#include<H5Cpp.h>
#include<stdint.h>
#include<iostream>
#include"particlesystem.h"
#include<string>
using namespace H5;
struct IOSettingType
{
	bool out_coordinate;//暂时先不使用
	bool out_velocity;//暂时先不使用
	Group out_group;//输出组的句柄
	float time_step;//输出的时间步
};
class IOHelper
{
public:
	IOHelper(H5std_string file_name);
	ParticleSystem init_particle();
	static void* auto_run_func(ParticleSystem* ps, void* cstepn, void* cstep);
	void auto_run();
private:
	void* (*pf)(ParticleSystem*, void*);
	static DataSet create_write_array(Group gp,H5std_string ds, void* data, int length, PredType type);
	static void write_cv(Group gp,ParticleSystem* ps);

	float out_time;//输出时间(来自文件)
	float out_put_step;//输出时间步(来自文件)
	IOSettingType run_set;//设置
	H5File file;//文件
	ParticleSystem ps;//粒子系统
};

struct IOrunningType
{
	float time;
	int index;
};
