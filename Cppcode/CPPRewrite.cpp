// CPPRewrite.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<stdio.h>
#include<fstream>
#include"IOHelper.h"
#include"vec.h"
#include"particle.h"
#include"particlesystem.h"

void probe_func(ParticleSystem* ps)
{
    printf("%0.4f\n", ps->get_time());
}

int main()
{
    //Particle a(vec2(50,50),vec2(-100,10),1,1);
    //Particle b(vec2(60, 60), vec2(-1, -1), 1, 1);
    //Particle* plist = new Particle[40000];
    //for (int i = 0; i < 200; i++)
    //{
    //    for (int j = 0; j < 200; j++)
    //    {
    //        Particle* thp = plist + i*200+j;
    //        thp->coordinate = vec2(5 + 0.5 * i, 1 + 0.5 * j);
    //        thp->velocity = vec2(100,-100);
    //        thp->radius = 0.2;
    //    }
    //}

    //Wall sw(vec2(0, 0), vec2(500, 500));
    //ParticleSystem ps(plist, 40000, &sw, 0.2);
    //ps.auto_run(100,*probe_func);
    //std::ofstream out1("out1.txt");
    //std::ofstream out2("out2.txt");
    //for (int i = 0; i < 2000; i++)
    //{
    //    out1 << plist[i].velocity.mod() << " ";
    //    out2 << plist[i].coordinate.x << " " << plist[i].coordinate.y<<"\n";
    //}
    //    
    //a.run_step(5);
    //Wall wall(vec2(0,0),vec2(500,500));
    //wall.check_handle_crash(a, 5);
    IOHelper ios("test1.hdf5");//读入文件
    ios.init_particle();//初始化系统
    ios.auto_run();//输出
    return 0;
}


