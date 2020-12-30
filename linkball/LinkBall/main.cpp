/*
作者：洛枫
名称：小球连连看
玩法：从第一个小球出发，按鼠标左键发射射线连接第二个小球，直到重新连接到第一个小球，形成闭环游戏结束
简介：每关小球位置随机，第二关球更小更多，第三关球会移动
QQ：1101458726
Email：1101458726@qq.com
编译环境：Visual Studio 2008，EasyX_20190314(beta)
最后修改时间：2019-4-10
*/
#include"class.h"

int main()
{
	lf::GameManager game(640,480);
	game.Loop();
}