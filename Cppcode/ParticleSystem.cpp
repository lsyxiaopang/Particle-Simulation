#include "particlesystem.h"
/// <summary>
/// 粒子系统的初始化
/// </summary>
/// <param name="particle_array">包含了所有粒子的数组</param>
/// <param name="array_length">粒子数量</param>
/// <param name="pwall">墙的指针</param>
/// <param name="d">步长长度(通常取最小粒子半径)</param>
ParticleSystem::ParticleSystem(Particle* particle_array, int array_length, Wall* pwall, float d) :
	particle_array(particle_array), time(0), pwall(pwall),d(d),length(array_length)
{
	if (d == 0)//特征长度为所有半径最小
	{
		float min_rad = 1e5;
		for (int i = 0; i < this->length; i++)
			if (min_rad > this->particle_array[i].radius)
				min_rad = this->particle_array[i].radius;
		this->d = min_rad;
	}
}
/// <summary>
/// 获得粒子系统的最大速率
/// </summary>
/// <returns>返回粒子的最大速率</returns>
float ParticleSystem::max_vec()
{
	float max = 0;
	for (int i = 0; i < this->length; i++)
		if (max < this->particle_array[i].velocity.mod())
			max = this->particle_array[i].velocity.mod();
	return max;
}
/// <summary>
/// 让粒子向前移动一个时间步
/// 通过使用网格覆盖的方式检测碰撞
/// 可以调用处理函数处理粒子与粒子,粒子与墙壁的碰撞
/// </summary>
/// <param name="m">横向网格划分数</param>
/// <param name="n">纵向网格划分数</param>
/// <param name="grid">固定网格</param>
/// <returns></returns>
float ParticleSystem::run_step(int m, int n, vector<Particle*>* grid)
{
	float step = this->d / this->max_vec();
	this->time += step;//确定时间步
	for (int i = 0; i < this->length; i++)
	{
		this->particle_array[i].run_step(step);
		this->pwall->check_handle_crash(this->particle_array[i], this->time);
	}
	//使用邻接网格的方法划分后计算加快速度
	//建立网格
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
	//对网格检测,使用check_vec
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
	//清空网格
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			while(grid[i*n+j].size()!=0)
				grid[i * n + j].pop_back();
	delete [] ilist;
	delete [] jlist;
	return step;
}
/// <summary>
/// 检查网格内粒子是否有碰撞
/// </summary>
/// <param name="pv">网格部分</param>
/// <param name="tp">粒子</param>
/// <param name="step">时间步(无效)</param>
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
/// 自动向前移动一段时间的长度
/// 支持探针操作
/// </summary>
/// <param name="time">从0开始到什么时间</param>
/// <param name="pf">回调函数指针,三个参数分别为粒子系统,自改变变量(随着调用改变的变量,返回值即为其),args</param>
/// <param name="args">回调函数最后一个参数</param>
/// <returns>实际前进的时间</returns>
float ParticleSystem::auto_run(float time, void* (*pf)(ParticleSystem*, void*, void*), void* args)
{
	int m, n;
	void* data_s = NULL;
	m = (int)((this->pwall->xmax - this->pwall->xmin) / (this->d * 3)) + 50;//横向划分数
	n = (int)((this->pwall->ymax - this->pwall->ymin) / (this->d * 3)) + 50;//纵向划分数
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
/// 获取当前时间
/// </summary>
/// <returns>当前时间</returns>
float ParticleSystem::get_time()
{
	return this->time;
}