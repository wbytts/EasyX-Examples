#include "Graphics.h"
#include "Point2.h"
#include "Point3.h"
#include "Point4.h"
#include <stdio.h>
#include <list>//Ϊ�˷��㣬ʹ����STL��list�������Ͳ����Լ���д�����ˣ�������Ҫ�ǽ�ͼ��ѧ���������ݽṹ
#include <deque>
#include "Vector3.h"
#include <chrono>
#pragma warning(disable:4996)
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
struct Edge//�߽ṹ
{
	double X;
	double DX;
	double YMAX;

	double ReciprocateOfOmega;//1 / ���ڱ�����ʼֵ
	double UDivOmega;
	double VDivOmega;
	double DReciprocateOfOmega; //��¼��ǰ��������1/���ڲ�ͬɨ�������������
	double DUDivOmega;
	double DVDivOmega;
	Edge(double x, double dx, double ymax, double reciprocateOfOmega, double uDivOmega, double vDivOmega, double dReciprocateOfOmega, double dUDivOmega, double dVDivOmega)
		:X(x), DX(dx), YMAX(ymax), ReciprocateOfOmega(reciprocateOfOmega), UDivOmega(uDivOmega), VDivOmega(vDivOmega), DReciprocateOfOmega(dReciprocateOfOmega), DUDivOmega(dUDivOmega), DVDivOmega(dVDivOmega)
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
//ʹ�ö������亯���򵥵ĸ�����һ�£������е�count������3
void FillTriangle(Graphics& gp, Point4* Triangle, double* attribute)
{
	Point4 ps[3];
	for (int i = 0; i < 3; i++)
	{
		//ps[i].X = (Triangle[i].X + 1) / 2 * (gp.WIDTH - 1);//ʵ���к�ȡֵ��ΧΪ[0,Height-1]
		//ps[i].Y = (Triangle[i].Y + 1) / 2 * (gp.HEIGHT - 1);//ʵ���к�ȡֵ��ΧΪ[0,Height-1]
		ps[i].X = (Triangle[i].X + 1) / 2 * (gp.WIDTH);//ʵ���к�ȡֵ��ΧΪ[0,Height-1]
		ps[i].Y = (Triangle[i].Y + 1) / 2 * (gp.HEIGHT);//ʵ���к�ȡֵ��ΧΪ[0,Height-1]
		ps[i].W = Triangle[i].W;
	}
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
		double ReciprocateOfOmega, UDivOmega, VDivOmega, DReciprocateOfOmega, DUDivOmega, DVDivOmega;// 1/�ء�u/�ء�v/���ڱ��ϵ���ʼֵ�͸ñ��ڲ�ͬɨ�������������
		if (ps[i].Y > ps[(i + 1) % 3].Y)//��pi+1��Ϊ���,pi��Ϊ�յ�
		{
			x = ps[(i + 1) % 3].X;
			dx = (ps[(i + 1) % 3].X - ps[i].X) / (ps[(i + 1) % 3].Y - ps[i].Y);
			ymax = ps[i].Y;

			ReciprocateOfOmega = 1 / ps[(i + 1) % 3].W;
			UDivOmega = attribute[((i + 1) % 3) * 2] / ps[(i + 1) % 3].W;
			VDivOmega = attribute[((i + 1) % 3) * 2 + 1] / ps[(i + 1) % 3].W;

			DReciprocateOfOmega = (1 / ps[(i + 1) % 3].W - 1 / ps[i].W) / (ps[(i + 1) % 3].Y - ps[i].Y);
			DUDivOmega = (attribute[((i + 1) % 3) * 2] / ps[(i + 1) % 3].W - attribute[i * 2] / ps[i].W) / (ps[(i + 1) % 3].Y - ps[i].Y);
			DVDivOmega = (attribute[((i + 1) % 3) * 2 + 1] / ps[(i + 1) % 3].W - attribute[i * 2 + 1] / ps[i].W) / (ps[(i + 1) % 3].Y - ps[i].Y);

			NET[(int)ps[(i + 1) % 3].Y].push_back(Edge(x, dx, ymax, ReciprocateOfOmega, UDivOmega, VDivOmega, DReciprocateOfOmega, DUDivOmega, DVDivOmega));
		}
		else if (ps[i].Y < ps[(i + 1) % 3].Y)//��pi��Ϊ��㣬pi+1��Ϊ�յ�
		{
			x = ps[i].X;
			dx = (ps[(i + 1) % 3].X - ps[i].X) / (ps[(i + 1) % 3].Y - ps[i].Y);
			ymax = ps[(i + 1) % 3].Y;

			ReciprocateOfOmega = 1 / ps[i].W;
			UDivOmega = attribute[i * 2] / ps[i].W;
			VDivOmega = attribute[i * 2 + 1] / ps[i].W;

			DReciprocateOfOmega = (1 / ps[(i + 1) % 3].W - 1 / ps[i].W) / (ps[(i + 1) % 3].Y - ps[i].Y);
			DUDivOmega = (attribute[((i + 1) % 3) * 2] / ps[(i + 1) % 3].W - attribute[i * 2] / ps[i].W) / (ps[(i + 1) % 3].Y - ps[i].Y);
			DVDivOmega = (attribute[((i + 1) % 3) * 2 + 1] / ps[(i + 1) % 3].W - attribute[i * 2 + 1] / ps[i].W) / (ps[(i + 1) % 3].Y - ps[i].Y);

			NET[(int)ps[i].Y].push_back(Edge(x, dx, ymax, ReciprocateOfOmega, UDivOmega, VDivOmega, DReciprocateOfOmega, DUDivOmega, DVDivOmega));
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

					/*
					Ϊ�˷����ҿ�����(��ʵ���Լ���������)�����ڰ�������д�ĺ������һ��
					s:start ��ʼ
					e:end ����
					r:reciprocal ����
					d:divide ����
					o:omega ��
					u:����U
					v:����V
					*/
					double sudo = edgeStar->UDivOmega;//ȡ�õ�ǰɨ��������X,1/�ء�u/�ء�v/�س�ʼֵ�ͽ���ֵ
					double eudo = edgeEnd->UDivOmega;
					double svdo = edgeStar->VDivOmega;
					double evdo = edgeEnd->VDivOmega;
					double sro = edgeStar->ReciprocateOfOmega;
					double ero = edgeEnd->ReciprocateOfOmega;

					double sx = edgeStar->X;
					double ex = edgeEnd->X;

					double dxro, dxudo, dxvdo;
					dxro = (ero - sro) / (ex - sx);//��������1/�ء�u/�ء�v/����ɨ�������������
					dxudo = (eudo - sudo) / (ex - sx);
					dxvdo = (evdo - svdo) / (ex - sx);

					int x = sx;//��ʼ����ǰ���ص�x,u,v����ֵ
					double ro = sro;
					double udo = sudo;
					double vdo = svdo;

					for (; x < ex; x++)//������Խ�����ɵ��߶�
					{
						double px, py;//ͼƬ��x,y
						double omega = 1 / ro;
						double u = omega * udo;
						double v = omega * vdo;
						UV2XY(u, v, px, py, 2, 2);//��uvת����xy����
						COLORREF c = getPixel((int)px, (int)py);
						gp.setPixel(x, y, c);//��������
						ro += dxro;
						udo += dxudo;
						vdo += dxvdo;
					}
					//����Խ����xֵ����dx
					edgeStar->X += edgeStar->DX;
					edgeEnd->X += edgeEnd->DX;

					edgeStar->ReciprocateOfOmega += edgeStar->DReciprocateOfOmega;
					edgeEnd->ReciprocateOfOmega += edgeEnd->DReciprocateOfOmega;

					edgeStar->UDivOmega += edgeStar->DUDivOmega;
					edgeEnd->UDivOmega += edgeEnd->DUDivOmega;

					edgeStar->VDivOmega += edgeStar->DVDivOmega;
					edgeEnd->VDivOmega += edgeEnd->DVDivOmega;
				}
				it++;
			}
		}
	}
	delete[] NET;
}
//ͶӰ����,�ֱ𴫵�һ�����nearƽ�棬�������ͶӰ�㲢�����������һ�����������
void Projection(Point3& p, double l, double r, double b, double t, double n, double f, Point4& result)
{
	result.X = (2 * n * p.X) / (r - l) - p.Z * (r + l) / (r - l);
	result.Y = (2 * n * p.Y) / (t - b) - p.Z * (t + b) / (t - b);
	result.Z = (f + n) / (f - n) * p.Z + 2 * f * n / (n - f);
	result.W = p.Z;
}
//��ֱ�߽��вü�,����-1��ʾƽ���ܾ�������0��ʾƽ�����ܣ�����1��ʾ��A����вü�������2��ʾ��B����вü��������������result��t��ʾ�ü��������tֵ
//���в���flag��0��5�ֱ��ʾ�߽�Ϊleft��right��bottom��top��near��far
int ClipLine(int flag, Point4& A, Point4& B, Point4& result, double& t)
{
	double aClipCondition = 0;
	double bClipCondition = 0;
	switch (flag)
	{
	case 0:
		aClipCondition = A.W + A.X;
		bClipCondition = B.W + B.X;
		break;
	case 1:
		aClipCondition = A.W - A.X;
		bClipCondition = B.W - B.X;
		break;
	case 2:
		aClipCondition = A.W + A.Y;
		bClipCondition = B.W + B.Y;
		break;
	case 3:
		aClipCondition = A.W - A.Y;
		bClipCondition = B.W - B.Y;
		break;
	case 4:
		aClipCondition = A.W + A.Z;
		bClipCondition = B.W + B.Z;
		break;
	case 5:
		aClipCondition = A.W - A.Z;
		bClipCondition = B.W - B.Z;
		break;
	default:
		return 0;
	}
	if (aClipCondition < 0 && bClipCondition < 0)//ƽ���ܾ�
	{
		return -1;
	}
	if (aClipCondition >= 0 && bClipCondition >= 0)//ƽ������
	{
		return 0;
	}
	t = aClipCondition / (aClipCondition - bClipCondition);//���㽻���tֵ
	result.X = A.X + (B.X - A.X) * t;//���㽻��
	result.Y = A.Y + (B.Y - A.Y) * t;
	result.Z = A.Z + (B.Z - A.Z) * t;
	result.W = A.W + (B.W - A.W) * t;
	if (aClipCondition < 0)
	{
		return 1;
	}
	else
	{
		return 2;
	}
}
void ClipTriangleAndDraw(Point4 ps[3], double* attribute, Graphics& gp)//�ü����һ���������
{
	std::deque<Point4> tmp;//�Ѷ��㸴�Ƶ�һ����ʱ������
	std::deque<double> tmp_a;//�Ѷ������Ը��Ƶ�һ����ʱ������
	for (int i = 0; i < 3; i++)
	{
		tmp.push_back(ps[i]);
		tmp_a.push_back(attribute[i * 2]);//��������ֵ
		tmp_a.push_back(attribute[i * 2 + 1]);
	}
	//����ʹ�������߽������ü�
	for (int clipflag = 0; clipflag < 6; clipflag++)
	{
		std::deque<Point4> points;
		std::deque<double> attributes;
		size_t size = tmp.size();
		//���βü�size����
		for (int i = 0; i < size; i++)
		{
			Point4 result;
			double t;
			int ret = ClipLine(clipflag, tmp[i], tmp[((size_t)i + 1) % size], result, t);//���߶ν��вü�
			if (ret == 0)//ƽ������
			{
				points.push_back(tmp[((size_t)i + 1) % size]);//��Ŷ�������
				attributes.push_back(tmp_a[((size_t)i + 1) % size * 2]);//��Ŷ�������
				attributes.push_back(tmp_a[((size_t)i + 1) % size * 2 + 1]);
			}
			else if (ret == 1)//��㱻�ü�
			{
				points.push_back(result);//��Ŷ�������
				attributes.push_back(tmp_a[i * 2] + (tmp_a[((size_t)i + 1) % size * 2] - tmp_a[i * 2]) * t);//��Ŷ�������
				attributes.push_back(tmp_a[i * 2 + 1] + (tmp_a[((size_t)i + 1) % size * 2 + 1] - tmp_a[i * 2 + 1]) * t);
				points.push_back(tmp[((size_t)i + 1) % size]);//��Ŷ�������
				attributes.push_back(tmp_a[((size_t)i + 1) % size * 2]);//��Ŷ�������
				attributes.push_back(tmp_a[((size_t)i + 1) % size * 2 + 1]);
			}
			else if (ret == 2)//���㱻�ü�
			{
				points.push_back(result);//��Ŷ�������
				attributes.push_back(tmp_a[i * 2] + (tmp_a[((size_t)i + 1) % size * 2] - tmp_a[i * 2]) * t);//��Ŷ�������
				attributes.push_back(tmp_a[i * 2 + 1] + (tmp_a[((size_t)i + 1) % size * 2 + 1] - tmp_a[i * 2 + 1]) * t);
			}
		}
		tmp.clear();
		tmp.assign(points.begin(), points.end());//�ѱ��βü��Ķ���ν�����������߽�Ĳü�
		tmp_a.clear();
		tmp_a.assign(attributes.begin(), attributes.end());
	}
	//���tmp��tmp_a���ǲü��Ľ��
	for (int i = 1; i < tmp.size() - 1; i++)//���ü�֮��Ķ����ϸ�ֳɶ��������
	{
		Point4 Triangle[3];
		double attributes[6];
		Triangle[0].X = tmp[0].X / tmp[0].W;//ִ��͸�ӳ���
		Triangle[0].Y = tmp[0].Y / tmp[0].W;
		Triangle[0].W = tmp[0].W;
		Triangle[1].X = tmp[i].X / tmp[i].W;
		Triangle[1].Y = tmp[i].Y / tmp[i].W;
		Triangle[1].W = tmp[i].W;
		Triangle[2].X = tmp[i + 1].X / tmp[i + 1].W;
		Triangle[2].Y = tmp[i + 1].Y / tmp[i + 1].W;
		Triangle[2].W = tmp[i + 1].W;

		attributes[0] = tmp_a[0];//�������ֵ
		attributes[1] = tmp_a[0 + 1];

		attributes[2] = tmp_a[i * 2];
		attributes[3] = tmp_a[i * 2 + 1];

		attributes[4] = tmp_a[(i + 1) * 2];
		attributes[5] = tmp_a[(i + 1) * 2 + 1];

		FillTriangle(gp, Triangle, attributes);
	}
}
int main()
{
	Graphics gp(640, 480);//������ͼ����
	double coordinate1[] = {//��������1
			0,0,0,1,1,0
	};
	double coordinate2[] = {//��������2
			1,1,1,0,0,1
	};

	Point3 ps3[] = { {-639.90252489721047,-480,88.830459715149615},{-639.90252489721047,480,88.830459715149615},{639.90252489721047,-480,111.16954028485038} };//����ps1�Ƶ�(0,0,100)����(0,0,1)������Ϊ��ת����ת��һ���Ƕ�
	Point3 ps4[] = { {639.90252489721047,480,111.16954028485038},{639.90252489721047,-480,111.16954028485038},{-639.90252489721047,480,88.830459715149615} };//����ps2�Ƶ�(0,0,100)����(0,0,1)������Ϊ��ת����ת��һ���Ƕ�
	Point4 result3[3];
	Point4 result4[3];
	for (int i = 0; i < 3; i++)//ִ��͸�ӳ˷�
	{
		Projection(ps3[i], -320, 320, -240, 240, 25, 200, result3[i]);
		Projection(ps4[i], -320, 320, -240, 240, 25, 200, result4[i]);
	}
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	ClipTriangleAndDraw(result3, coordinate1, gp);
	ClipTriangleAndDraw(result4, coordinate2, gp);
	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	double useTime = double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;//����ʱ��
	char msg[256];//����������ӡ��Ϣ�õĻ�����
	sprintf(msg, "release�汾��˫���Բ�ֵ���ƺ�ʱ:%lf ����\n", useTime * 1000);
	OutputDebugStringA(msg);//�������������֡����ʱ����
	getchar();
}