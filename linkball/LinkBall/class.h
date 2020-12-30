#pragma once

#include <easyx.h>
#include <vector>
#include <iostream>

namespace lf
{
	class Input 
	{
	public:
		static bool mouseLeftDown;
	};


	class Position 
	{
	private:
		float x, y;

	public:
		Position(float x, float y) 
		{
			this->x = x;
			this->y = y;
		}

		Position() 
		{
			x = 0;
			y = 0;
		}

		Position(const Position&pos) 
		{
			x = pos.x;
			y = pos.y;
		}

		Position& operator=(const Position&pos)
		{
			x = pos.x;
			y = pos.y;
			return *this;
		}

		void setX(float x) 
		{
			this->x = x;
		}

		void setY(float y)
		{
			this->y = y;
		}

		float getX()
		{
			return x;
		}

		float getY()
		{
			return y;
		}
	};


	class Circle 
	{
	protected:
		class GameManager *gameManager;
		Position position;	// λ��
		bool linked;		// �Ƿ��Ѿ�����
		int number;			// ���
		int r;				// �뾶

	public:
		Circle(const Position &position, int r, int number, class GameManager *gameManager);

		virtual void update(float deltaTime) {}

		virtual void draw()
		{
			setfillcolor(YELLOW);
			solidcircle((int)position.getX(), (int)position.getY(), r);
		}

		Position getPosition()
		{
			return position;
		}

		bool getLinked() 
		{
			return linked;
		}

		void setLinked(bool boolean) 
		{
			linked = boolean;
		}

		int getNumber()
		{
			return number;
		}

		int getR() 
		{
			return r;
		}
	};


	class MoveCircle : public Circle
	{
	private:
		int direction;		// �������ң�1 2 3 4
		Position initPos;	// ��ʼλ��

	public:
		MoveCircle(const Position &position, int r, int number, class GameManager *gameManager);
		virtual void update(float deltaTime);
	};


	class Line
	{
	protected:
		Position start, end;	// ֱ�ߵ������յ�

	public:
		Line(const Position&start, const Position& end) 
		{
			this->start = start;
			this->end = end;
		}

		void draw() 
		{
			setlinecolor(BLUE);
			line((int)start.getX(), (int)start.getY(), (int)end.getX(), (int)end.getY());
		}
	};


	// ������
	class Launcher 
	{
	protected:
		class GameManager *gameManager;
		float rayLength;	// ���ߵĳ���
		float rayRotation;	// ���߽Ƕ�
		Circle *circle;		// ��ǰλ�ڵ�Բ
		Circle *lastCircle;	// ��һ��λ�ڵ�Բ
		bool isLaunch;		// �Ƿ���

	public:
		Launcher(Circle *circle, class GameManager *gameManager);
		void setPosition(Circle *circle, bool isNew = true);
		void update(float deltaTime);
		void draw();

		// ��������
		void launch() 
		{
			isLaunch = true;
		}
		
	};


	class GameManager 
	{
	private:
		std::vector<Circle*> circle;				// ���ڴ洢��Ļ�����е�Բ
		std::vector<Line> lineShape;				// ���ڴ洢��Ļ�����е�ֱ��

		std::vector<void(GameManager::*)()> scence;	// ��Ϸ����
		Launcher *launcher;//������

		float gameTime;					// ��Ϸʣ��ʱ��
		float deltaTime;				// ÿ֮֡��ļ��ʱ��
		int score;						// �÷�
		int maxScore;					// ��ߵ÷�
		int windowWidth, windowHeight;	// ��Ļ���
		bool gameLoop;					// ��Ϸѭ��

		void clearScence();				// �����ǰ����
		void draw();
		void update(float deltaTime);
		void input();

		void wait();					// ���غ�ȴ�
		void startGame();				// ��ʼ��Ϸ

	public:
		std::vector<Circle*>& getCircle() 
		{
			return circle;
		}

		int getWidth() 
		{
			return windowWidth;
		}

		int getHeight()
		{
			return windowHeight;
		}

		void setGameLoop(bool boolean) 
		{
			gameLoop = boolean;
		}

		void addScore(int num)
		{
			score += num;
			if (score > maxScore)
				maxScore = score;
		}

		void addLine(const Line& l) 
		{
			lineShape.push_back(l);
		}

		void Scence1();	// ��һ��
		void Scence2();	// �ڶ���
		void Scence3();	// ������

		void Loop();	// ��Ϸ����
		GameManager(int width,int height);
		~GameManager();
	};
}
