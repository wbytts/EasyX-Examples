#include "class.h"
#include <time.h>
#include <Windows.h>
#include <sstream>
#include <string>
#include <math.h>
#include <fstream>
#include <iostream>

#define M_PI 3.14159265358979323846


bool lf::Input::mouseLeftDown = false;


void lf::GameManager::clearScence()
{
	// 清除所有的圆
	for (unsigned i = 0; i < circle.size(); i++)
		delete circle[i];
	circle.clear();

	// 清除线
	lineShape.clear();

	// 清除发射器
	delete launcher;
}


void lf::GameManager::draw()
{
	// 绘制直线
	for (unsigned i = 0; i < lineShape.size(); i++) 
	{
		lineShape[i].draw();
	}

	// 绘制发射器
	launcher->draw();

	// 绘制圆
	for (unsigned i = 0; i < circle.size(); i++) 
	{
		circle[i]->draw();
	}

	// 设置字体样式
	settextcolor(RED);					
	settextstyle(20, 0, _T("Arial"), 0, 0, 400, false, false, false, ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH);		

	// 显示时间
	setbkmode(TRANSPARENT);

	std::wstringstream ss;
	std::wstring str = _T("Time: ");
	ss << int(gameTime);
	str += ss.str();
	outtextxy(0, 0, str.c_str());
	ss.str(_T(""));

	// 显示得分
	str = _T("Score: ");
	ss << score;
	str += ss.str();
	outtextxy(0, 25, str.c_str());
	ss.str(_T(""));

	str = _T("MaxScore: ");
	ss << maxScore;
	str += ss.str();
	outtextxy(0, 50, str.c_str());
}


void lf::GameManager::update(float deltaTime)
{
	// 更新时间
	gameTime -= deltaTime;

	// 如果鼠标左键按下，发射器发射
	if (Input::mouseLeftDown)
		launcher->launch();

	// 更新球
	for (unsigned i = 0; i < circle.size(); i++) 
	{
		circle[i]->update(deltaTime);
	}

	// 更新发射器
	launcher->update(deltaTime);
}


void lf::GameManager::input()
{
	Input::mouseLeftDown = false;
	while (MouseHit()) 
	{
		MOUSEMSG msg = GetMouseMsg();

		if (msg.mkLButton)
			Input::mouseLeftDown = true;
	}
}


void lf::GameManager::wait()
{
	// 设置字体样式
	settextcolor(BLUE);
	settextstyle(50, 0, _T("Arial"), 0, 0, 400, false, false, false, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH);

	// 绘制等待信息
	setbkmode(OPAQUE);
	RECT r = { 0,0,windowWidth,windowHeight };
	std::wstringstream ss;
	std::wstring str = _T("恭喜过关，");
	for (int i = 3; i > 0; i--)
	{
		ss << i;
		str += ss.str();
		str += _T(" 秒后开始下一关");
		drawtext(str.c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		str = _T("恭喜过关，");
		ss.str(_T(""));
		FlushBatchDraw();
		Sleep(1000);
	}
}


void lf::GameManager::startGame()
{
	// 初始化游戏数据
	score = 0;
	gameTime = 0;

	// 设置字体样式
	settextcolor(BLUE);
	settextstyle(50, 0, _T("Arial"), 0, 0, 400, false, false, false, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH);

	// 绘制等待信息
	RECT r = { 0, 0, windowWidth, windowHeight };
	std::wstringstream ss;
	std::wstring str;
	for (int i = 3; i > 0; i--)
	{
		cleardevice();
		ss << i;
		str += ss.str();
		str += _T(" 秒后开始游戏");
		drawtext(str.c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		str = _T("");
		ss.str(_T(""));
		FlushBatchDraw();
		Sleep(1000);
	}

	// 清空鼠标消息
	FlushMouseMsgBuffer();
}


void lf::GameManager::Scence1()
{
	// 把窗口分成 3 * 2 块，每块随机位置生成一个 1 / 2 大小的圆
	gameTime += 30;
	for (int i = 0; i < 6; i++) 
	{
		Position pos;

		int offsetX = rand() % windowWidth / 3 / 4 + windowWidth / 3 / 4;
		int offsetY = rand() % windowHeight / 2 / 4 + windowHeight / 2 / 4;

		pos.setX(float((i % 3) * windowWidth / 3 + offsetX));
		pos.setY(float(i / 3 * windowHeight / 2 + offsetY));

		circle.push_back(new Circle(pos, windowWidth / 3 / 4, i + 1, this));
	}

	launcher = new Launcher(circle[0], this);
}

void lf::GameManager::Scence2()
{
	// 把窗口分成 4 * 3 块，每块随机位置生成一个 1 / 4 大小的圆
	gameTime += 30;
	for (int i = 0; i < 12; i++) 
	{
		Position pos;

		int offsetX = rand() % windowWidth / 4 / 8 + windowWidth / 4 / 8;
		int offsetY = rand() % windowHeight / 3 / 8 + windowHeight / 3 / 8;

		pos.setX(float((i % 4) * windowWidth / 4 + offsetX));
		pos.setY(float(i / 4 * windowHeight / 3 + offsetY));

		circle.push_back(new Circle(pos, windowWidth / 4 / 8, i + 1, this));
	}

	// 发射器附着在第一个圆上
	launcher = new Launcher(circle[0], this);
}


void lf::GameManager::Scence3()
{
	// 把窗口分成 3 * 2 块，每块随机位置生成一个 1 / 3 大小的圆
	gameTime += 30;
	for (int i = 0; i < 6; i++) 
	{
		Position pos;

		int offsetX = rand() % windowWidth / 3 / 6 + windowWidth / 3 / 6;
		int offsetY = rand() % windowHeight / 2 / 6 + windowHeight / 2 / 6;

		pos.setX(float((i % 3) * windowWidth / 3 + offsetX));
		pos.setY(float(i / 3 * windowHeight / 2 + offsetY));

		circle.push_back(new MoveCircle(pos, windowWidth / 3 / 6, i + 1, this));
	}

	launcher = new Launcher(circle[0], this);
}


void lf::GameManager::Loop()
{
	clock_t start, end;
	while (1) 
	{
		startGame();
		for (unsigned i = 0; i < scence.size(); i++)
		{
			gameLoop = true;
			(this->*scence[i])();	// 初始化场景

			while (gameLoop && gameTime >= 0) 
			{
				start = clock();	// 记录当前帧开始时间
				cleardevice();
				input();
				update(deltaTime);
				draw();
				FlushBatchDraw();
				end = clock();		// 当前帧结束时间

				if (end - start > 33) 
				{
					deltaTime = (end - start) / 1000.0f;
					//deltaTime = 33/1000.0f;
				}
				// 延时
				else 
				{
					Sleep(33 - end + start);
					deltaTime = 33 / 1000.0f;
				}
			}
			clearScence();			// 清除当前场景

			// 保存最高纪录
			std::wstringstream ss;
			ss << maxScore;
			WritePrivateProfileString(_T("LinkBall"), _T("MaxScore"), ss.str().c_str(), _T(".\\data.ini"));

			// 时间用尽
			if (gameTime <= 0)
				break;
			if (i != scence.size() - 1)
				wait();
		}
		int selection = MessageBox(GetHWnd(), _T("是否继续游戏？"), _T("游戏结束： "), MB_OKCANCEL| MB_ICONQUESTION);

		if (selection == 2)	break;	// 退出游戏
	}
}


lf::GameManager::GameManager(int width, int height)
{

	// 加入关卡
	scence.push_back(&GameManager::Scence1);
	scence.push_back(&GameManager::Scence2);
	scence.push_back(&GameManager::Scence3);

	// 设置宽高，并初始化窗口
	initgraph(width, height);
	windowWidth = width;
	windowHeight = height;

	// 设置背景颜色，开始批量绘图
	setbkcolor(WHITE);
	BeginBatchDraw();
	
	

	srand((unsigned int)time(NULL));	// 初始化随机数种子

	// 初始化变量
	gameTime = 0;						// 游戏剩余时间
	deltaTime = 0;						// 每帧之间的间隔时间
	score = 0;							// 得分
	bool gameLoop = false;				// 游戏循环
	maxScore = GetPrivateProfileInt(_T("LinkBall"), _T("MaxScore"), 0, _T(".\\data.ini"));	// 最高分
	
}


lf::GameManager::~GameManager()
{
	// 保存最高纪录
	std::wstringstream ss;
	ss << maxScore;
	WritePrivateProfileString(_T("LinkBall"), _T("MaxScore"), ss.str().c_str(), _T(".\\data.ini"));

	closegraph();
}


lf::Launcher::Launcher(Circle *circle, GameManager *gameManager)
{
	lastCircle = circle;
	this->circle = circle;
	this->gameManager = gameManager;
	rayLength = circle->getR() * 1.5f;
	rayRotation = 0;
	isLaunch = false;
}


void lf::Launcher::setPosition(Circle *circle, bool isNew)
{
	// 如果是新的位置那么更新上一个圆的引用
	if (isNew)
	{
		lastCircle = this->circle;
		rayRotation = 0;				// 重置旋转角度
		this->circle = circle;
	}

	rayLength = circle->getR() * 1.5f;	// 重置射线长度
	isLaunch = false;
}


void lf::Launcher::update(float deltaTime)
{
	// 如果发射射线，增加射线长度，判断是否与圆发生碰撞或者超出边界
	if (isLaunch) 
	{
		rayLength += 200 * deltaTime;	// 每秒增长200像素

		// 计算射线坐标
		Position pos;
		float rx = circle->getPosition().getX();
		float ry = circle->getPosition().getY();
		double beta = (360 - rayRotation) * M_PI / 180;

		float x = circle->getPosition().getX();
		float y = circle->getPosition().getY() - rayLength;
		
		pos.setX(float((x - rx) * cos(beta) + (y - ry) * sin(beta) + rx));
		pos.setY(float((y - ry) * cos(beta) - (x - rx) * sin(beta) + ry));

		// 射线超出屏幕
		if (pos.getX() > gameManager->getWidth() || pos.getX() < 0 ||
			pos.getY() > gameManager->getHeight() || pos.getY() < 0) 
		{
			
			setPosition(circle, false);
			return;
		}

		std::vector<Circle*> &v = gameManager->getCircle();
		for (unsigned i = 0; i < v.size(); i++)
		{
			// 判断是否与圆发生碰撞
			if ((pos.getX() - v[i]->getPosition().getX()) * (pos.getX() - v[i]->getPosition().getX()) +
				(pos.getY() - v[i]->getPosition().getY()) * (pos.getY() - v[i]->getPosition().getY()) <
				v[i]->getR() * v[i]->getR()) 
			{
				// 碰撞的圆已连接
				if (v[i]->getLinked())
				{				
					setPosition(circle, false);
					return;
				}

				// 与上次链接的圆一样，重置发射器
				if (v[i] == lastCircle) 
				{	
					setPosition(circle, false);
					return;
				}

				// 圆的编号为 1 游戏结束
				if (v[i]->getNumber() == 1) 
				{
					
					gameManager->addScore(10);
					gameManager->setGameLoop(false);
					return;
				}

				// 以上两个条件都不满足，说明碰撞到的是未连接的圆
				gameManager->addScore(10);
				v[i]->setLinked(true);
				gameManager->addLine(Line(circle->getPosition(), pos));
				setPosition(v[i]);
				return;
			}
		}
	}
	else
	{
		// 没有发射射线，三角形进行旋转
		rayRotation += 360 * deltaTime * 1.0f / 2;	// 2s 转一圈

		if (rayRotation > 360)
			rayRotation -= 360;
	}
}


void lf::Launcher::draw()
{
	setfillcolor(GREEN);
	Position p1, p2, p3;
	float rx = circle->getPosition().getX();
	float ry = circle->getPosition().getY();
	double beta = (360 - rayRotation) * M_PI / 180;
	float x, y;

	if(isLaunch)
	{
		// 计算射线位置
		x = circle->getPosition().getX();
		y = circle->getPosition().getY() - rayLength;
		p1.setX(float((x - rx) * cos(beta) + (y - ry) * sin(beta) + rx));
		p1.setY(float((y - ry) * cos(beta) - (x - rx) * sin(beta) + ry));

		// 绘制射线
		setlinecolor(BLUE);
		line(int(circle->getPosition().getX()), int(circle->getPosition().getY()), int(p1.getX()), int(p1.getY()));
	}

	// 计算三角形三个点的坐标
	x = circle->getPosition().getX()- circle->getR() + 5;
	y = circle->getPosition().getY();
	p1.setX(float((x - rx) * cos(beta) + (y - ry) * sin(beta) + rx));
	p1.setY(float((y - ry) * cos(beta) - (x - rx) * sin(beta) + ry));
	
	x = circle->getPosition().getX() + circle->getR() - 5;
	y = circle->getPosition().getY();
	p2.setX(float((x - rx) * cos(beta) + (y - ry) * sin(beta) + rx));
	p2.setY(float((y - ry) * cos(beta) - (x - rx) * sin(beta) + ry));
	
	x = circle->getPosition().getX();
	y = circle->getPosition().getY()- circle->getR() * 1.5f;
	p3.setX(float((x - rx) * cos(beta) + (y - ry) * sin(beta) + rx));
	p3.setY(float((y - ry) * cos(beta) - (x - rx) * sin(beta) + ry));
	
	// 绘制三角形
	POINT pts[] = { {(long)p1.getX(), (long)p1.getY()},
					{(long)p2.getX(), (long)p2.getY()}, 
					{(long)p3.getX(), (long)p3.getY()} };
	solidpolygon(pts, 3);
}


lf::Circle::Circle(const Position &position, int r, int number, GameManager *gameManager)
{
	this->gameManager = gameManager;
	this->position = position;
	this->r = r;
	this->number = number;
	linked = false;
}


lf::MoveCircle::MoveCircle(const Position &position, int r, int number, GameManager *gameManager) 
	:Circle(position, r, number, gameManager)
{
	initPos = position;
	direction = rand() % 4 + 1;	// 随机一个移动方向
}


void lf::MoveCircle::update(float deltaTime)
{
	// 如果已经被链接或者为起始球不移动
	if (linked || number == 1) 
		return;

	float dist = r * deltaTime;	// 两秒移动一个圆的位置
	Position target(position);

	// 更新位置,距离起始位置太远或者超出边界换相反方向移动
	switch (direction)
	{
		case 1:
			target.setY(target.getY() - dist);
			if (abs(target.getY() - initPos.getY()) > r * 2 || target.getY() < r || target.getY() >
				gameManager->getHeight() - r)
			{
				direction = 2;
			}
			else 
			{
				position = target;
			}
			break;
		case 2:
			target.setY(target.getY() + dist);
			if (abs(target.getY() - initPos.getY()) > r * 2 || target.getY() < r || target.getY()> 
				gameManager->getHeight() - r) 
			{
				direction = 1;
			}
			else 
			{
				position = target;
			}
			break;
		case 3:
			target.setX(target.getX()-dist);
			if (abs(target.getX() - initPos.getX()) > r * 2 || target.getX() < r || target.getX() > 
				gameManager->getWidth() - r) {
				direction = 4;
			}
			else {
				position = target;
			}
			break;
		case 4:
			target.setX(target.getX() + dist);
			if (abs(target.getX() - initPos.getX()) > r * 2 || target.getX() < r || target.getX() > 
				gameManager->getWidth() - r)
			{
				direction = 3;
			}
			else
			{
				position = target;
			}
			break;
	}
}
