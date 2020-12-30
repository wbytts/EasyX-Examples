#include "Graphics.h"
#include "Point2.h"
#include <stdio.h>
#include <list>//Ϊ�˷��㣬ʹ����STL��list�������Ͳ����Լ���д�����ˣ�������Ҫ�ǽ�ͼ��ѧ���������ݽṹ
#include <deque>
#include "Vector3.h"
#include <chrono>
#pragma warning(disable:4996)
struct Edge//�߽ṹ
{
	double X;
	double DX;
	double YMAX;
	Edge(double x, double dx, double ymax) :X(x), DX(dx), YMAX(ymax)
	{}
};
COLORREF picture[2][2] = {//����һ��2*2�Ķ�ֵͼƬ
	{RGB(255,0,0),RGB(255,255,255)},
	{RGB(0,255,0),RGB(0,0,255)}
};
//��ȡͼƬ��ɫ
COLORREF getPixel(int x, int y)
{
	return picture[x][y];
}
//��uv����ת����xy����
void UV2XY(double u, double v, double& x, double& y, double w, double h)
{
	x = u * w;
	y = v * h;
	x = min(max(x, 0), w);//��x������[0,w]����
	y = min(max(y, 0), h);//��y������[0,h]����
}
//��������룬��std::list����ʱ����
bool SortEdge(Edge const& E1, Edge const& E2)//���߱����򣬰�X�����������Xһ��������dx��������
{
	if (E1.X != E2.X)
	{
		return E1.X < E2.X;
	}
	else
	{
		return E1.DX < E2.DX;
	}
}
//ʹ�ö������亯���򵥵ĸ�����һ�£������е�count������3
void FillTriangle(Graphics& gp, Point2* ps, double* coordinate)
{
	Vector3 ab(ps[1].X - ps[0].X, ps[1].Y - ps[0].Y, 0.0);//ps[0]->ps[1]
	Vector3 bc(ps[2].X - ps[1].X, ps[2].Y - ps[1].Y, 0.0);//ps[1]->ps[2]
	Vector3 ca(ps[0].X - ps[2].X, ps[0].Y - ps[2].Y, 0.0);//ps[2]->ps[0]
	Vector3 ac(ps[2].X - ps[0].X, ps[2].Y - ps[0].Y, 0.0);//ps[2]->ps[0]
	double square = ab.X * ac.Y - ab.Y * ac.X;//�õ����������������2��
	if (square == 0)//���������Ϊ0�򲻽��к���Ļ��ƴ���
	{
		return;
	}


	std::list<Edge>* NET = new std::list<Edge>[gp.HEIGHT];//����NET
	int YMIN = (int)ps[0].Y;
	int YMAX = (int)ps[0].Y;
	//��NET���������
	for (int i = 0; i < 3; i++)//��ÿ���߶��ᱻ�ж����Σ�ѡ��YֵС�ĵ���Ϊ��㣬Yֵ��ĵ���Ϊ�յ㣬ƽ����ɨ���ߵı߱�����,������Ϊ��������ɨ����ƽ�еıߣ�����Ҳ�������dx�������������
	{
		double x, dx, ymax;
		if (ps[i].Y > ps[(i + 1) % 3].Y)//��pi+1��Ϊ���,pi��Ϊ�յ�
		{
			x = ps[(i + 1) % 3].X;
			dx = (ps[(i + 1) % 3].X - ps[i].X) / (ps[(i + 1) % 3].Y - ps[i].Y);
			ymax = ps[i].Y;
			NET[(int)ps[(i + 1) % 3].Y].push_back(Edge(x, dx, ymax));
		}
		else if (ps[i].Y < ps[(i + 1) % 3].Y)//��pi��Ϊ��㣬pi+1��Ϊ�յ�
		{
			x = ps[i].X;
			dx = (ps[(i + 1) % 3].X - ps[i].X) / (ps[(i + 1) % 3].Y - ps[i].Y);
			ymax = ps[(i + 1) % 3].Y;
			NET[(int)ps[i].Y].push_back(Edge(x, dx, ymax));
		}
		else//ƽ����ɨ���ߵı�
		{
		}
		YMIN = (int)min(ps[i].Y, YMIN);//��¼����ε���СYֵ
		YMAX = (int)max(ps[i].Y, YMAX);//��¼����ε����Yֵ
	}
	std::list<Edge> AEL;
	for (int y = YMIN; y < YMAX; y++)//��Զ���θ��������ÿ��ɨ���ߴ���
	{
		if (!NET[y].empty())//�����ǰɨ���߶�Ӧ��NET��Ϊ��
		{
			AEL.splice(AEL.end(), NET[y]);//������ӵ�AEL��
			AEL.sort(SortEdge);//����������
		}
		std::list<Edge>::iterator edgeStar, edgeEnd;//�ɶ�ȡ���Ľ���
		int counterOfedge = 0;//ȡ������ļ�����������ɶ�ȡ������
		for (std::list<Edge>::iterator it = AEL.begin(); it != AEL.end();)//����AEL�е�ÿ������
		{
			if ((int)it->YMAX <= y)
			{
				it = AEL.erase(it);//��ǰɨ�����Ѿ�����it�����ߵ�Ymax,��it��ɾ������Ϊִ��erase���Զ���ָ�������ƶ�һ��Ԫ�أ���������Ͳ���ִ��it++��
			}
			else
			{
				if (counterOfedge == 0)//��ǰȡ������һ�Խ����е�ǰһ��
				{
					edgeStar = it;
					counterOfedge++;
				}
				else//��ǰȡ����һ�Խ�����еĺ�һ�����Ѿ���ɶ���
				{
					edgeEnd = it;
					counterOfedge = 0;
					for (int x = (int)edgeStar->X; x < edgeEnd->X; x++)//������Խ�����ɵ��߶�
					{
						double WeightA, WeightB, WeightC;
						Vector3 bp(x - ps[1].X, y - ps[1].Y, 0.0);
						Vector3 ap(x - ps[0].X, y - ps[0].Y, 0.0);
						Vector3 cp(x - ps[2].X, y - ps[2].Y, 0.0);
						WeightA = (bc.X * bp.Y - bc.Y * bp.X) / square;
						WeightB = (ca.X * cp.Y - ca.Y * cp.X) / square;
						WeightC = (ab.X * ap.Y - ab.Y * ap.X) / square;//�õ����������Ȩֵ

						double u, v;//�����uv
						u = WeightA * coordinate[0] + WeightB * coordinate[2] + WeightC * coordinate[4];//ʹ�����Բ�ֵ���㵱ǰ�������ص�uֵ
						v = WeightA * coordinate[1] + WeightB * coordinate[3] + WeightC * coordinate[5];//ʹ�����Բ�ֵ���㵱ǰ�������ص�vֵ
						double px, py;//ͼƬ��x,y
						UV2XY(u, v, px, py, 2, 2);//��uvת����xy����
						COLORREF c = getPixel((int)px, (int)py);
						gp.setPixel(x, y, c);//��������
					}
					//����Խ����xֵ����dx
					edgeStar->X += edgeStar->DX;
					edgeEnd->X += edgeEnd->DX;
				}
				it++;
			}
		}
	}
	delete[] NET;
}

int main()
{
	Point2 Polygon[] = { {0,0},{400,0},{400,400} };
	double coordinate[] = {//��������
		0,0,1,0,1,1
	};
	Graphics gp(640, 480);//������ͼ����
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	FillTriangle(gp, Polygon, coordinate);//����������
	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	double useTime = double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;//����ʱ��
	char msg[256];//����������ӡ��Ϣ�õĻ�����
	sprintf(msg, "Debug�汾�����������ֵ���ƺ�ʱ:%lf ����\n", useTime * 1000);
	OutputDebugStringA(msg);//�������������֡����ʱ����
	getchar();
}