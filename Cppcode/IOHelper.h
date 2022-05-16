#pragma once
#include<H5Cpp.h>
#include<stdint.h>
#include<iostream>
#include"particlesystem.h"
#include<string>
using namespace H5;
struct IOSettingType
{
	bool out_coordinate;//��ʱ�Ȳ�ʹ��
	bool out_velocity;//��ʱ�Ȳ�ʹ��
	Group out_group;//�����ľ��
	float time_step;//�����ʱ�䲽
};
class IOHelper
{
public:
	IOHelper(const char* file_name);
	ParticleSystem init_particle();
	static void* auto_run_func(ParticleSystem* ps, void* cstepn, void* cstep);
	void auto_run();
private:
	void* (*pf)(ParticleSystem*, void*);
	static DataSet create_write_array(Group gp,H5std_string ds, void* data, int length, PredType type);
	static void write_cv(Group gp,ParticleSystem* ps);

	float out_time;//���ʱ��(�����ļ�)
	float out_put_step;//���ʱ�䲽(�����ļ�)
	IOSettingType run_set;//����
	H5File file;//�ļ�
	ParticleSystem ps;//����ϵͳ
};

struct IOrunningType
{
	float time;
	int index;
};
