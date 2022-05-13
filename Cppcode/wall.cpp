#include"wall.h"
/// <summary>
/// 初始化墙壁
/// </summary>
/// <param name="A">左下角坐标</param>
/// <param name="C">右上角坐标</param>
/// <param name="beg_time">开始记录碰撞时间</param>
Wall::Wall(vec2 A,vec2 C,float beg_time):
	xmin(A.x),xmax(C.x),ymin(A.y),ymax(C.y),beg_time(beg_time),crash_velocity_vector()
{
}

/// <summary>
/// 检查并处理粒子与墙壁的碰撞
/// 如果不发生碰撞,则粒子状态不变
/// 发生碰撞,则粒子位置与速率发生相应变化,并加以记录
/// </summary>
/// <param name="p">粒子</param>
/// <param name="time">时间,用于记录碰撞</param>
void Wall::check_handle_crash(Particle& p, float time)
{
	float rad = p.radius;
	float xcoor = p.coordinate.x;
	float ycoor = p.coordinate.y;
	float xmax = this->xmax-rad;
	float xmin = this->xmin+rad;
	float ymax = this->ymax-rad;
	float ymin = this->ymin+rad;//注意加上半径
	if (xcoor > xmin && xcoor<xmax && ycoor>ymin && ycoor < ymax)
		return;//没有发生碰撞
	if (xcoor<xmin)//与左墙碰撞
	{
		p.velocity.x = -p.velocity.x;
		p.coordinate.x += 2 * (xmin - xcoor);
	}
	else if (xcoor > xmax)//与右墙碰撞
	{
		if (time > this->beg_time)//时间超过开始记录的时间
		{
			this->crash_velocity_vector.push_back(p.velocity);//记录碰撞
		}
		p.velocity.x = -p.velocity.x;
		p.coordinate.x -= 2 * (xcoor-xmax);	
	}
	if (ycoor < ymin)//与下墙碰撞
	{
		p.velocity.y = -p.velocity.y;
		p.coordinate.y += 2 * (ymin - ycoor);
	}
	else if (ycoor > ymax)//与上墙碰撞
	{
		p.velocity.y = -p.velocity.y;
		p.coordinate.y -= 2 * (ycoor - ymax);
	}

	return;
}