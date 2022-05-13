#include"wall.h"
/// <summary>
/// ��ʼ��ǽ��
/// </summary>
/// <param name="A">���½�����</param>
/// <param name="C">���Ͻ�����</param>
/// <param name="beg_time">��ʼ��¼��ײʱ��</param>
Wall::Wall(vec2 A,vec2 C,float beg_time):
	xmin(A.x),xmax(C.x),ymin(A.y),ymax(C.y),beg_time(beg_time),crash_velocity_vector()
{
}

/// <summary>
/// ��鲢����������ǽ�ڵ���ײ
/// �����������ײ,������״̬����
/// ������ײ,������λ�������ʷ�����Ӧ�仯,�����Լ�¼
/// </summary>
/// <param name="p">����</param>
/// <param name="time">ʱ��,���ڼ�¼��ײ</param>
void Wall::check_handle_crash(Particle& p, float time)
{
	float rad = p.radius;
	float xcoor = p.coordinate.x;
	float ycoor = p.coordinate.y;
	float xmax = this->xmax-rad;
	float xmin = this->xmin+rad;
	float ymax = this->ymax-rad;
	float ymin = this->ymin+rad;//ע����ϰ뾶
	if (xcoor > xmin && xcoor<xmax && ycoor>ymin && ycoor < ymax)
		return;//û�з�����ײ
	if (xcoor<xmin)//����ǽ��ײ
	{
		p.velocity.x = -p.velocity.x;
		p.coordinate.x += 2 * (xmin - xcoor);
	}
	else if (xcoor > xmax)//����ǽ��ײ
	{
		if (time > this->beg_time)//ʱ�䳬����ʼ��¼��ʱ��
		{
			this->crash_velocity_vector.push_back(p.velocity);//��¼��ײ
		}
		p.velocity.x = -p.velocity.x;
		p.coordinate.x -= 2 * (xcoor-xmax);	
	}
	if (ycoor < ymin)//����ǽ��ײ
	{
		p.velocity.y = -p.velocity.y;
		p.coordinate.y += 2 * (ymin - ycoor);
	}
	else if (ycoor > ymax)//����ǽ��ײ
	{
		p.velocity.y = -p.velocity.y;
		p.coordinate.y -= 2 * (ycoor - ymax);
	}

	return;
}