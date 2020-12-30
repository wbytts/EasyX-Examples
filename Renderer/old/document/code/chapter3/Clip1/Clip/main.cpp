#include "Graphics.h"
#include "Point2.h"
#include <stdio.h>
#include <list>//Ϊ�˷��㣬ʹ����STL��list�������Ͳ����Լ���д�����ˣ�������Ҫ�ǽ�ͼ��ѧ���������ݽṹ
#include <deque>
struct Edge//�߽ṹ
{
	double X;
	double DX;
	double YMAX;
	Edge(double x, double dx, double ymax) :X(x), DX(dx), YMAX(ymax)
	{}
};
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
void FillPolygon(Graphics& gp, Point2* ps, int count)
{
	if (count < 3)//������������3���޷�Χ��һ�������
	{
		return;
	}
	std::list<Edge>* NET = new std::list<Edge>[gp.HEIGHT];//����NET
	int YMIN = (int)ps[0].Y;
	int YMAX = (int)ps[0].Y;
	//��NET���������
	for (int i = 0; i < count; i++)//��ÿ���߶��ᱻ�ж����Σ�ѡ��YֵС�ĵ���Ϊ��㣬Yֵ��ĵ���Ϊ�յ㣬ƽ����ɨ���ߵı߱�����,������Ϊ��������ɨ����ƽ�еıߣ�����Ҳ�������dx�������������
	{
		double x, dx, ymax;
		if (ps[i].Y > ps[(i + 1) % count].Y)//��pi+1��Ϊ���,pi��Ϊ�յ�
		{
			x = ps[(i + 1) % count].X;
			dx = (ps[(i + 1) % count].X - ps[i].X) / (ps[(i + 1) % count].Y - ps[i].Y);
			ymax = ps[i].Y;
			NET[(int)ps[(i + 1) % count].Y].push_back(Edge(x, dx, ymax));
		}
		else if (ps[i].Y < ps[(i + 1) % count].Y)//��pi��Ϊ��㣬pi+1��Ϊ�յ�
		{
			x = ps[i].X;
			dx = (ps[(i + 1) % count].X - ps[i].X) / (ps[(i + 1) % count].Y - ps[i].Y);
			ymax = ps[(i + 1) % count].Y;
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
						gp.setPixel(x, y, WHITE);
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
/*
ʹ��ֱ�߲ü�һ��ֱ��
����һ��͹�������˵�������еĶ��㶼������һ���ߵ�ͬһ�࣬���Ե�ʹ�øö���ε�ĳһ������Ϊ�ü��߽磬��ѡ�����ⲻ������������Ķ���r����Ϊ�ü���������
�����ü�����p��r����ֱ�ߵ�ͬһ��ʱ˵��p�ᱻ�ü���
����������ü���Line,���޸Ĵ������line�����꣬�޸�A��ʱ����1,�޸�B��ʱ����2��ƽ������ʱ����0��ƽ���ܾ�����-1
*/
int clipLL(Point2& Boundary0,Point2 &Boundary1, Point2& Reference, Point2 *line)//clipWindowΪPoint2[2]�Ĳü�Boundary,ReferenceΪ���ڲü�ֱ������ĵ㣬�������ü�����,lineΪPoint2[2]���ü���ֱ��
{
	double A1, B1, C1;//�߽��һ��ʽ���̲���,ֱ������ʽ����һ��ʽ,A=y2-y1 B=x2-x1 C=x1(y2-y1)+y1(x2-x1)
	A1 = Boundary1.Y - Boundary0.Y;
	B1 = Boundary0.X - Boundary1.X;
	C1 = Boundary1.X * Boundary0.Y - Boundary0.X * Boundary1.Y;

	bool clipA = false;//���ü��߶ε�a���Ƿ񱻲ü����
	bool clipB = false;//���ü��߶ε�b���Ƿ񱻲ü����
	if ((A1 * Reference.X + B1 * Reference.Y + C1) * (A1 * line[0].X + B1 * line[0].Y + C1) < 0)//���ü��߶ε�a���Ƿ񱻲ü�
	{
		clipA = true;
	}
	if ((A1 * Reference.X + B1 * Reference.Y + C1) * (A1 * line[1].X + B1 * line[1].Y + C1) < 0)//���ü��߶ε�b���Ƿ񱻲ü�
	{
		clipB = true;
	}
	if (!clipA && !clipB)//ƽ������
	{
		return 0;
	}
	else if (clipA && clipB)//ƽ���ܾ�
	{
		return -1;
	}
	double A2, B2, C2;//���ü��߶ε�һ��ʽ���̲���
	A2 = line[1].Y - line[0].Y;
	B2 = line[0].X - line[1].X;
	C2 = line[1].X * line[0].Y - line[0].X * line[1].Y;
	//�������
	double x = (C2 * B1 - C1 * B2) / (A1 * B2 - A2 * B1);
	double y = (C1 * A2 - C2 * A1) / (A1 * B2 - A2 * B1);
	if (clipA)
	{
		line[0].X = x;
		line[0].Y = y;
		return 1;
	}
	else
	{
		line[1].X = x;
		line[1].Y = y;
		return 2;
	}
}
/*
ʹ��һ��͹�������Ϊ�ü�����
*/
std::deque<Point2> clip(Point2* clipWindow, int countOfwindow, Point2* Polygon, int countOfpolygon)//countOfwindow���ü����ڵĶ���������countOfpolygon������εĶ�������
{
	std::deque<Point2> tmp;
	for (int i=0;i< countOfpolygon;i++)
	{
		tmp.push_back(Polygon[i]);
	}
	for (int j=0;j<countOfwindow;j++)
	{
		std::deque<Point2> points;
		size_t size = tmp.size();
		//tmp��ŵ��Ǳ��βü��Ķ���ζ��㼯��
		for (size_t i = 0; i < size; i++)//ʹ�òü����ڵ�һ������Ϊ�ü��߽�Զ���ε�ÿ���߽��вü�
		{
			Point2 line[] = { tmp[i], tmp[(i + 1) % size] };//����һ�����ü����߶�
			int ret = clipLL(clipWindow[j], clipWindow[(j+1)% countOfwindow], clipWindow[(j + 2) % countOfwindow], line);//ѡȡ���ڵ�һ������Ϊ�߽磬����һ������Ϊ���յ�
			if (ret == 0)//ƽ������
			{
				points.push_back(line[1]);
			}
			else if (ret == 1)//���
			{
				points.push_back(line[0]);
				points.push_back(line[1]);
			}
			else if (ret == 2)//����
			{
				points.push_back(line[1]);
			}
		}
		tmp.clear();
		tmp.assign(points.begin(), points.end());//�ѱ��βü��Ķ���ν�����������߽�Ĳü�
	}
	//���tmp���ǲü��Ľ��
	return tmp;
}
int main()
{
	Point2 Polygon[] = { {0,0},{100,0},{100,100},{0,100} };//���ü������
	Point2 clipWindow[] = { {0,0},{90,0},{50,90} };//�ü�����
	auto tmp=clip(clipWindow, 3, Polygon, 4);//�ü�
	int count = (int)tmp.size();
	Point2* result = new Point2[count];//���ü�������Ƶ�����
	for (int i = 0; i < count; i++)
	{
		result[i] = tmp[i];
	}
	Graphics gp(640, 480);//������ͼ����
	FillPolygon(gp, result, count);//���ƶ����
	delete[] result;
	getchar();
}