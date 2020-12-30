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
	// ������е�Բ
	for (unsigned i = 0; i < circle.size(); i++)
		delete circle[i];
	circle.clear();

	// �����
	lineShape.clear();

	// ���������
	delete launcher;
}


void lf::GameManager::draw()
{
	// ����ֱ��
	for (unsigned i = 0; i < lineShape.size(); i++) 
	{
		lineShape[i].draw();
	}

	// ���Ʒ�����
	launcher->draw();

	// ����Բ
	for (unsigned i = 0; i < circle.size(); i++) 
	{
		circle[i]->draw();
	}

	// ����������ʽ
	settextcolor(RED);					
	settextstyle(20, 0, _T("Arial"), 0, 0, 400, false, false, false, ANSI_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH);		

	// ��ʾʱ��
	setbkmode(TRANSPARENT);

	std::wstringstream ss;
	std::wstring str = _T("Time: ");
	ss << int(gameTime);
	str += ss.str();
	outtextxy(0, 0, str.c_str());
	ss.str(_T(""));

	// ��ʾ�÷�
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
	// ����ʱ��
	gameTime -= deltaTime;

	// ������������£�����������
	if (Input::mouseLeftDown)
		launcher->launch();

	// ������
	for (unsigned i = 0; i < circle.size(); i++) 
	{
		circle[i]->update(deltaTime);
	}

	// ���·�����
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
	// ����������ʽ
	settextcolor(BLUE);
	settextstyle(50, 0, _T("Arial"), 0, 0, 400, false, false, false, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH);

	// ���Ƶȴ���Ϣ
	setbkmode(OPAQUE);
	RECT r = { 0,0,windowWidth,windowHeight };
	std::wstringstream ss;
	std::wstring str = _T("��ϲ���أ�");
	for (int i = 3; i > 0; i--)
	{
		ss << i;
		str += ss.str();
		str += _T(" ���ʼ��һ��");
		drawtext(str.c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		str = _T("��ϲ���أ�");
		ss.str(_T(""));
		FlushBatchDraw();
		Sleep(1000);
	}
}


void lf::GameManager::startGame()
{
	// ��ʼ����Ϸ����
	score = 0;
	gameTime = 0;

	// ����������ʽ
	settextcolor(BLUE);
	settextstyle(50, 0, _T("Arial"), 0, 0, 400, false, false, false, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH);

	// ���Ƶȴ���Ϣ
	RECT r = { 0, 0, windowWidth, windowHeight };
	std::wstringstream ss;
	std::wstring str;
	for (int i = 3; i > 0; i--)
	{
		cleardevice();
		ss << i;
		str += ss.str();
		str += _T(" ���ʼ��Ϸ");
		drawtext(str.c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		str = _T("");
		ss.str(_T(""));
		FlushBatchDraw();
		Sleep(1000);
	}

	// ��������Ϣ
	FlushMouseMsgBuffer();
}


void lf::GameManager::Scence1()
{
	// �Ѵ��ڷֳ� 3 * 2 �飬ÿ�����λ������һ�� 1 / 2 ��С��Բ
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
	// �Ѵ��ڷֳ� 4 * 3 �飬ÿ�����λ������һ�� 1 / 4 ��С��Բ
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

	// �����������ڵ�һ��Բ��
	launcher = new Launcher(circle[0], this);
}


void lf::GameManager::Scence3()
{
	// �Ѵ��ڷֳ� 3 * 2 �飬ÿ�����λ������һ�� 1 / 3 ��С��Բ
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
			(this->*scence[i])();	// ��ʼ������

			while (gameLoop && gameTime >= 0) 
			{
				start = clock();	// ��¼��ǰ֡��ʼʱ��
				cleardevice();
				input();
				update(deltaTime);
				draw();
				FlushBatchDraw();
				end = clock();		// ��ǰ֡����ʱ��

				if (end - start > 33) 
				{
					deltaTime = (end - start) / 1000.0f;
					//deltaTime = 33/1000.0f;
				}
				// ��ʱ
				else 
				{
					Sleep(33 - end + start);
					deltaTime = 33 / 1000.0f;
				}
			}
			clearScence();			// �����ǰ����

			// ������߼�¼
			std::wstringstream ss;
			ss << maxScore;
			WritePrivateProfileString(_T("LinkBall"), _T("MaxScore"), ss.str().c_str(), _T(".\\data.ini"));

			// ʱ���þ�
			if (gameTime <= 0)
				break;
			if (i != scence.size() - 1)
				wait();
		}
		int selection = MessageBox(GetHWnd(), _T("�Ƿ������Ϸ��"), _T("��Ϸ������ "), MB_OKCANCEL| MB_ICONQUESTION);

		if (selection == 2)	break;	// �˳���Ϸ
	}
}


lf::GameManager::GameManager(int width, int height)
{

	// ����ؿ�
	scence.push_back(&GameManager::Scence1);
	scence.push_back(&GameManager::Scence2);
	scence.push_back(&GameManager::Scence3);

	// ���ÿ�ߣ�����ʼ������
	initgraph(width, height);
	windowWidth = width;
	windowHeight = height;

	// ���ñ�����ɫ����ʼ������ͼ
	setbkcolor(WHITE);
	BeginBatchDraw();
	
	

	srand((unsigned int)time(NULL));	// ��ʼ�����������

	// ��ʼ������
	gameTime = 0;						// ��Ϸʣ��ʱ��
	deltaTime = 0;						// ÿ֮֡��ļ��ʱ��
	score = 0;							// �÷�
	bool gameLoop = false;				// ��Ϸѭ��
	maxScore = GetPrivateProfileInt(_T("LinkBall"), _T("MaxScore"), 0, _T(".\\data.ini"));	// ��߷�
	
}


lf::GameManager::~GameManager()
{
	// ������߼�¼
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
	// ������µ�λ����ô������һ��Բ������
	if (isNew)
	{
		lastCircle = this->circle;
		rayRotation = 0;				// ������ת�Ƕ�
		this->circle = circle;
	}

	rayLength = circle->getR() * 1.5f;	// �������߳���
	isLaunch = false;
}


void lf::Launcher::update(float deltaTime)
{
	// ����������ߣ��������߳��ȣ��ж��Ƿ���Բ������ײ���߳����߽�
	if (isLaunch) 
	{
		rayLength += 200 * deltaTime;	// ÿ������200����

		// ������������
		Position pos;
		float rx = circle->getPosition().getX();
		float ry = circle->getPosition().getY();
		double beta = (360 - rayRotation) * M_PI / 180;

		float x = circle->getPosition().getX();
		float y = circle->getPosition().getY() - rayLength;
		
		pos.setX(float((x - rx) * cos(beta) + (y - ry) * sin(beta) + rx));
		pos.setY(float((y - ry) * cos(beta) - (x - rx) * sin(beta) + ry));

		// ���߳�����Ļ
		if (pos.getX() > gameManager->getWidth() || pos.getX() < 0 ||
			pos.getY() > gameManager->getHeight() || pos.getY() < 0) 
		{
			
			setPosition(circle, false);
			return;
		}

		std::vector<Circle*> &v = gameManager->getCircle();
		for (unsigned i = 0; i < v.size(); i++)
		{
			// �ж��Ƿ���Բ������ײ
			if ((pos.getX() - v[i]->getPosition().getX()) * (pos.getX() - v[i]->getPosition().getX()) +
				(pos.getY() - v[i]->getPosition().getY()) * (pos.getY() - v[i]->getPosition().getY()) <
				v[i]->getR() * v[i]->getR()) 
			{
				// ��ײ��Բ������
				if (v[i]->getLinked())
				{				
					setPosition(circle, false);
					return;
				}

				// ���ϴ����ӵ�Բһ�������÷�����
				if (v[i] == lastCircle) 
				{	
					setPosition(circle, false);
					return;
				}

				// Բ�ı��Ϊ 1 ��Ϸ����
				if (v[i]->getNumber() == 1) 
				{
					
					gameManager->addScore(10);
					gameManager->setGameLoop(false);
					return;
				}

				// �������������������㣬˵����ײ������δ���ӵ�Բ
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
		// û�з������ߣ������ν�����ת
		rayRotation += 360 * deltaTime * 1.0f / 2;	// 2s תһȦ

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
		// ��������λ��
		x = circle->getPosition().getX();
		y = circle->getPosition().getY() - rayLength;
		p1.setX(float((x - rx) * cos(beta) + (y - ry) * sin(beta) + rx));
		p1.setY(float((y - ry) * cos(beta) - (x - rx) * sin(beta) + ry));

		// ��������
		setlinecolor(BLUE);
		line(int(circle->getPosition().getX()), int(circle->getPosition().getY()), int(p1.getX()), int(p1.getY()));
	}

	// ���������������������
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
	
	// ����������
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
	direction = rand() % 4 + 1;	// ���һ���ƶ�����
}


void lf::MoveCircle::update(float deltaTime)
{
	// ����Ѿ������ӻ���Ϊ��ʼ���ƶ�
	if (linked || number == 1) 
		return;

	float dist = r * deltaTime;	// �����ƶ�һ��Բ��λ��
	Position target(position);

	// ����λ��,������ʼλ��̫Զ���߳����߽绻�෴�����ƶ�
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
