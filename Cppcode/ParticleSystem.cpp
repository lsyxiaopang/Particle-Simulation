#include "particlesystem.h"
/// <summary>
/// ����ϵͳ�ĳ�ʼ��
/// </summary>
/// <param name="particle_array">�������������ӵ�����</param>
/// <param name="array_length">��������</param>
/// <param name="pwall">ǽ��ָ��</param>
/// <param name="d">��������(ͨ��ȡ��С���Ӱ뾶)</param>
ParticleSystem::ParticleSystem(Particle* particle_array, int array_length, Wall* pwall, float d) :
	particle_array(particle_array), time(0), pwall(pwall),d(d),length(array_length)
{
	if (d == 0)//��������Ϊ���а뾶��С
	{
		float min_rad = 1e5;
		for (int i = 0; i < this->length; i++)
			if (min_rad > this->particle_array[i].radius)
				min_rad = this->particle_array[i].radius;
		this->d = min_rad;
	}
}
/// <summary>
/// �������ϵͳ���������
/// </summary>
/// <returns>�������ӵ��������</returns>
float ParticleSystem::max_vec()
{
	float max = 0;
	for (int i = 0; i < this->length; i++)
		if (max < this->particle_array[i].velocity.mod())
			max = this->particle_array[i].velocity.mod();
	return max;
}
/// <summary>
/// ��������ǰ�ƶ�һ��ʱ�䲽
/// ͨ��ʹ�����񸲸ǵķ�ʽ�����ײ
/// ���Ե��ô�������������������,������ǽ�ڵ���ײ
/// </summary>
/// <param name="m">�������񻮷���</param>
/// <param name="n">�������񻮷���</param>
/// <param name="grid">�̶�����</param>
/// <returns></returns>
float ParticleSystem::run_step(int m, int n, vector<Particle*>* grid)
{
	float step = this->d / this->max_vec();
	this->time += step;//ȷ��ʱ�䲽
	for (int i = 0; i < this->length; i++)
	{
		this->particle_array[i].run_step(step);
		this->pwall->check_handle_crash(this->particle_array[i], this->time);
	}
	//ʹ���ڽ�����ķ������ֺ����ӿ��ٶ�
	//��������
	int ti, tj;
	int* ilist = new int[this->length];
	int* jlist = new int[this->length];
	for (int k = 0; k < this->length; k++)
	{
		ti=(int)((this->particle_array[k].coordinate.x - this->pwall->xmin) / (this->d * 3)+1);
		tj=(int)((this->particle_array[k].coordinate.y - this->pwall->ymin) / (this->d * 3)+1);
		ilist[k] = ti;
		jlist[k] = tj;
		grid[n * ti + tj].push_back(this->particle_array+k);
	}
	//��������,ʹ��check_vec
	for (int k = 0; k < this->length; k++)
	{
		int ti = ilist[k];
		int tj = jlist[k];
		//vector<Particle*>* this_p_m = &grid[ti*m+tj];
		Particle* tp = &particle_array[k];
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
				this->check_vec(grid + (ti + i) * n + tj + j, tp, step);
	}
	//�������
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			while(grid[i*n+j].size()!=0)
				grid[i * n + j].pop_back();
	delete [] ilist;
	delete [] jlist;
	return step;
}
/// <summary>
/// ��������������Ƿ�����ײ
/// </summary>
/// <param name="pv">���񲿷�</param>
/// <param name="tp">����</param>
/// <param name="step">ʱ�䲽(��Ч)</param>
void ParticleSystem::check_vec(vector<Particle*>* pv, Particle* tp, float step)
{
	//auto vecbeg = pv->begin();
	//auto vecend = pv->end();
	//	while (vecbeg != vecend)
	//	{			
	//		if (tp == *(vecbeg))
	//		{
	//			if (vecbeg+1==vecend)
	//			{
	//				pv->pop_back();
	//				break;
	//			}
	//			 vecbeg=pv->erase(vecbeg);
	//		}			
	//		else
	//		{
	//			Particle* np = *(vecbeg);
	//			if (tp->check_crash(*np))
	//			{
	//					
	//				Particle::handle_crash(*tp, *np, step);
	//				this->pwall->check_handle_crash(*np, step);
	//				if (vecbeg + 1 == vecend)
	//				{
	//					pv->pop_back();
	//					break;
	//				}
	//				if (vecbeg == vecend)
	//					return;
	//				vecbeg = pv->erase(vecbeg);
	//			}			
	//			else
	//			{
	//				vecbeg++;
	//			}
	//		}			
	//	}
	//	this->pwall->check_handle_crash(*tp, step);
	vector<Particle*> *newp=new vector<Particle*>();
	while (pv->size() > 0)
	{
		Particle* np = pv->back();
		pv->pop_back();
		if (np != tp)
		{
			if (tp->check_crash(*np))
			{
				Particle::handle_crash(*tp, *np, this->time);
				this->pwall->check_handle_crash(*np, this->time);
			}
			else
				newp->push_back(np);
		}

	}
	this->pwall->check_handle_crash(*tp, this->time);
	//delete pv;
	pv->swap(*newp);
	delete newp;
}
/// <summary>
/// �Զ���ǰ�ƶ�һ��ʱ��ĳ���
/// ֧��̽�����
/// </summary>
/// <param name="time">��0��ʼ��ʲôʱ��</param>
/// <param name="pf">�ص�����ָ��,���������ֱ�Ϊ����ϵͳ,�Ըı����(���ŵ��øı�ı���,����ֵ��Ϊ��),args</param>
/// <param name="args">�ص��������һ������</param>
/// <returns>ʵ��ǰ����ʱ��</returns>
float ParticleSystem::auto_run(float time, void* (*pf)(ParticleSystem*, void*, void*), void* args)
{
	int m, n;
	void* data_s = NULL;
	m = (int)((this->pwall->xmax - this->pwall->xmin) / (this->d * 3)) + 50;//���򻮷���
	n = (int)((this->pwall->ymax - this->pwall->ymin) / (this->d * 3)) + 50;//���򻮷���
	vector<Particle*>* grid = new vector<Particle*>[m * n]();
	while (this->time<time)
	{
		this->run_step(m,n,grid);
		if(pf!=NULL)
			data_s=(*pf)(this,data_s,args);
	}
	return this->time;
}
/// <summary>
/// ��ȡ��ǰʱ��
/// </summary>
/// <returns>��ǰʱ��</returns>
float ParticleSystem::get_time()
{
	return this->time;
}