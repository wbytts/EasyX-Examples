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
		Position position;	// 位置
		bool linked;		// 是否已经连接
		int number;			// 编号
		int r;				// 半径

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
		int direction;		// 上下左右：1 2 3 4
		Position initPos;	// 初始位置

	public:
		MoveCircle(const Position &position, int r, int number, class GameManager *gameManager);
		virtual void update(float deltaTime);
	};


	class Line
	{
	protected:
		Position start, end;	// 直线的起点和终点

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


	// 发射器
	class Launcher 
	{
	protected:
		class GameManager *gameManager;
		float rayLength;	// 射线的长度
		float rayRotation;	// 射线角度
		Circle *circle;		// 当前位于的圆
		Circle *lastCircle;	// 上一次位于的圆
		bool isLaunch;		// 是否发射

	public:
		Launcher(Circle *circle, class GameManager *gameManager);
		void setPosition(Circle *circle, bool isNew = true);
		void update(float deltaTime);
		void draw();

		// 发射射线
		void launch() 
		{
			isLaunch = true;
		}
		
	};


	class GameManager 
	{
	private:
		std::vector<Circle*> circle;				// 用于存储屏幕上所有的圆
		std::vector<Line> lineShape;				// 用于存储屏幕上所有的直线

		std::vector<void(GameManager::*)()> scence;	// 游戏场景
		Launcher *launcher;//发射器

		float gameTime;					// 游戏剩余时间
		float deltaTime;				// 每帧之间的间隔时间
		int score;						// 得分
		int maxScore;					// 最高得分
		int windowWidth, windowHeight;	// 屏幕宽高
		bool gameLoop;					// 游戏循环

		void clearScence();				// 清除当前场景
		void draw();
		void update(float deltaTime);
		void input();

		void wait();					// 过关后等待
		void startGame();				// 开始游戏

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

		void Scence1();	// 第一关
		void Scence2();	// 第二关
		void Scence3();	// 第三关

		void Loop();	// 游戏启动
		GameManager(int width,int height);
		~GameManager();
	};
}
