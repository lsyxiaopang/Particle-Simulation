#include "IOHelper.h"
/// <summary>
/// �½�һ��I/O������
/// </summary>
/// <param name="file_name">HDF5�ļ���</param>
IOHelper::IOHelper(const char* file_name= "D:\\python\\Maxwell\\final\\CPPRewrite\\x64\\Release\\test1.h5") :
	file(file_name, H5F_ACC_RDWR), ps(NULL, 0, NULL, 0)
{
}
/// <summary>
/// ͨ����ʼ��HDF5�ļ�ʵ�ֶ�����ϵͳ�ĳ�ʼ����߽���������
/// </summary>
/// <returns>����ϵͳ</returns>
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
	ingr.openAttribute("d").read(PredType::INTEL_F32, &d);//��ȡ��������
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
/// �Զ���������
/// ����auto_run�б��ص�
/// </summary>
/// <param name="ps">����ϵͳ.</param>
/// <param name="cstepn">�Ըı��������</param>
/// <param name="cstep">args(�������HDF5�ļ���Ϣ)</param>
/// <returns>�Ըı����</returns>
void* IOHelper::auto_run_func(ParticleSystem* ps, void* cstepn, void* cstep)
{
	if (cstepn == NULL)//��һ�ε���
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

		DataSet xset = IOHelper::create_write_array(thisg, "x", x, count,PredType::INTEL_F32);//x����
		DataSet yset = IOHelper::create_write_array(thisg, "y", y, count, PredType::INTEL_F32);//y����
		DataSet vxset = IOHelper::create_write_array(thisg, "vx", vx, count, PredType::INTEL_F32);//x�����ٶ�
		DataSet vyset = IOHelper::create_write_array(thisg, "vy", vy, count, PredType::INTEL_F32);//y�����ٶ�
		DataSet pcset = IOHelper::create_write_array(thisg, "particle crash times", pca, count, PredType::INTEL_I32);//������ײ��
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
/// A������ϵͳ����Ԥ����ʽ��ʼ����
/// </summary>
void IOHelper::auto_run()
{
	//void* (*pf)(ParticleSystem*, void*)=
	this->ps.auto_run(this->out_time,IOHelper::auto_run_func,&this->run_set);
}
/// <summary>
/// �ڲ�����
/// </summary>
/// <returns>д������ݼ�</returns>
DataSet IOHelper::create_write_array(Group gp, H5std_string dsname, void* data, int length, PredType type)
{
	hsize_t s[1] = { length };
	DataSpace sp(1, s);
	DataSet ds = gp.createDataSet(dsname, type, sp);
	ds.write(data, type, sp);
	return ds;
}
/// <summary>
/// ר������д����ײ��Ϣ
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