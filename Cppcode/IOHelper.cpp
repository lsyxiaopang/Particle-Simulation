#include "IOHelper.h"
/// <summary>
/// 新建一个I/O处理器
/// </summary>
/// <param name="file_name">HDF5文件名</param>
IOHelper::IOHelper(H5std_string file_name= "D:\\python\\Maxwell\\final\\CPPRewrite\\x64\\Release\\test1.h5") :
	file(file_name, H5F_ACC_RDWR), ps(NULL, 0, NULL, 0)
{
}
/// <summary>
/// 通过初始化HDF5文件实现对粒子系统的初始化与边界条件设置
/// </summary>
/// <returns>粒子系统</returns>
ParticleSystem IOHelper::init_particle()
{
	H5std_string group_name("input_data");
	int count;
	float out_put_step;
	float time_out;
	float d;
	Group ingr = this->file.openGroup(group_name);
	ingr.openAttribute("count").read(PredType::INTEL_I32, &count);
	ingr.openAttribute("out_put_step").read(PredType::INTEL_F32, &out_put_step);
	ingr.openAttribute("time_out").read(PredType::INTEL_F32, &time_out);
	ingr.openAttribute("d").read(PredType::INTEL_F32, &d);//读取各个数据
	Particle* pa = new Particle[count];
	float* xa = new float[count];
	float* ya = new float[count];
	float* vxa = new float[count];
	float* vya = new float[count];
	float* ra = new float[count];
	float* ma = new float[count];
	ingr.openDataSet("x").read(xa, PredType::ALPHA_F32);
	ingr.openDataSet("y").read(ya, PredType::ALPHA_F32);
	ingr.openDataSet("vx").read(vxa, PredType::ALPHA_F32);
	ingr.openDataSet("vy").read(vya, PredType::ALPHA_F32);
	ingr.openDataSet("r").read(ra, PredType::ALPHA_F32);
	ingr.openDataSet("m").read(ma, PredType::ALPHA_F32);
	//cout << count;
	for (int i = 0; i < count; i++)
	{
		(pa + i)->coordinate = vec2(xa[i], ya[i]);
		(pa + i)->velocity = vec2(vxa[i], vya[i]);
		(pa + i)->radius = ra[i];
		(pa + i)->m = ma[i];
	}
	//
	ParticleSystem p(pa, count, new Wall(), d);
	this->ps = p;
	this->out_time = time_out;
	this->out_put_step = out_put_step;
	this->run_set.time_step = out_put_step;
	Group outgr = this->file.createGroup("output_data");
	this->run_set.out_group = outgr;
	return p;
}
/// <summary>
/// 自动处理函数
/// 会在auto_run中被回调
/// </summary>
/// <param name="ps">粒子系统.</param>
/// <param name="cstepn">自改变变量类型</param>
/// <param name="cstep">args(保存关于HDF5文件信息)</param>
/// <returns>自改变变量</returns>
void* IOHelper::auto_run_func(ParticleSystem* ps, void* cstepn, void* cstep)
{
	if (cstepn == NULL)//第一次调用
		cstepn = new IOrunningType();
	IOrunningType* rt = (IOrunningType*)cstepn;
	IOSettingType* st = (IOSettingType*)cstep;
	float time_now = ps->get_time();
	if (time_now > rt->time)
	{
		std::cout << time_now << "\n";
		H5std_string grname=std::to_string(rt->index);
		Group thisg=st->out_group.createGroup(grname);
		int count = ps->length;
		float* x = new float[count];
		float* y = new float[count];
		float* vx = new float[count];
		float* vy = new float[count];
		float* pca = new float[count];
		for (int i = 0; i < count; i++)
		{
			*(x + i) = (ps->particle_array + i)->coordinate.x;
			*(y + i) = (ps->particle_array + i)->coordinate.y;
			*(vx + i) = (ps->particle_array + i)->velocity.x;
			*(vy + i) = (ps->particle_array + i)->velocity.y;
			*(pca + i) = (ps->particle_array + i)->crash_times;
		}

		DataSet xset = IOHelper::create_write_array(thisg, "x", x, count,PredType::INTEL_F32);//x数据
		DataSet yset = IOHelper::create_write_array(thisg, "y", y, count, PredType::INTEL_F32);//y数据
		DataSet vxset = IOHelper::create_write_array(thisg, "vx", vx, count, PredType::INTEL_F32);//x方向速度
		DataSet vyset = IOHelper::create_write_array(thisg, "vy", vy, count, PredType::INTEL_F32);//y方向速度
		DataSet pcset = IOHelper::create_write_array(thisg, "particle crash times", pca, count, PredType::INTEL_I32);//粒子碰撞数
		hsize_t atsize[1] = { 1 };
		DataSpace aspace(1, atsize);
		Attribute ta=thisg.createAttribute("time", PredType::INTEL_F32,aspace);			
		ta.write(PredType::INTEL_F32, &time_now);		
		IOHelper::write_cv(thisg,ps);
		delete []x;
		delete []y;
		delete []vx;
		delete []vy;
		delete[]pca;
		rt->index++;
		rt->time += st->time_step;

	}
	return cstepn;
}
/// <summary>
/// A让粒子系统按照预定形式开始运行
/// </summary>
void IOHelper::auto_run()
{
	//void* (*pf)(ParticleSystem*, void*)=
	this->ps.auto_run(this->out_time,IOHelper::auto_run_func,&this->run_set);
}
/// <summary>
/// 内部函数
/// </summary>
/// <returns>写入的数据集</returns>
DataSet IOHelper::create_write_array(Group gp, H5std_string dsname, void* data, int length, PredType type)
{
	hsize_t s[1] = { length };
	DataSpace sp(1, s);
	DataSet ds = gp.createDataSet(dsname, type, sp);
	ds.write(data, type, sp);
	return ds;
}
/// <summary>
/// 专门用于写入碰撞信息
/// </summary>
void IOHelper::write_cv(Group gp, ParticleSystem* ps)
{
	int crash_times = ps->pwall->crash_velocity_vector.size();
	float* avx = new float[crash_times];
	float* avy = new float[crash_times];
	for (int ind=0;ind<crash_times;ind++)
	{
		avx[ind] = ps->pwall->crash_velocity_vector[ind].x;
		avy[ind] = ps->pwall->crash_velocity_vector[ind].y;
	}
	hsize_t h[1] = { 1 };
	DataSpace dsp(1, h);
	Attribute ca = gp.createAttribute("crash times", PredType::INTEL_I32, dsp);
	ca.write(PredType::INTEL_I32, &crash_times);
	create_write_array(gp, "cvx", avx, crash_times, PredType::INTEL_F32);
	create_write_array(gp, "cvy", avy, crash_times, PredType::INTEL_F32);
	delete []avx;
	delete[]avy;
	return;
}