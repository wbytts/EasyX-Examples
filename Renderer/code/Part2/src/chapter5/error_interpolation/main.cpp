#include <GraphicsDevice.h>
#include <GraphicsLibrary.h>
#include "stdio.h"
class Texture
{
private:
	COLORREF img[2][2] = {
	{RGB(255,0,0),RGB(0,255,0)},
	{RGB(0,0,255),RGB(255,123,172)}
	};
	double width = 2, heigth = 2;
public:
	COLORREF texture2D(double u, double v)
	{
		return img[(int)(v * width)][(int)(u * width)];//��Ϊu��ʾ���ᣬ�������ⶨ������Ӧ�������ά�����ݣ�����Ӧ����img[v][u]��ʾ��������
	}
};
Texture texture;

Point4 Persective(Point4& p, double n)
{
	auto pn = p.Normalize();//�Ƚ������������ά����
	return Point4(pn.X * n, pn.Y * n, pn.Z * n, pn.Z);//ִ��͸�ӱ任
}

COLORREF FS(std::vector<double>& values)
{
	return texture.texture2D(values[0], values[1]);
};
int main()
{
	GraphicsDevice gd(640, 480);
	GraphicsLibrary gl(gd);
	
	Point4 a1(0, 0, 100, 1);
	Point4 b1(415.692193816530, 0, 340, 1);
	Point4 c1(0, 480, 100, 1);
	auto ta1 = Persective(a1, 80);
	auto tb1 = Persective(b1, 80);
	auto tc1 = Persective(c1, 80);
	ta1.ValueArray = { 0,0 };
	tb1.ValueArray = { 1,0 };
	tc1.ValueArray = { 0,1 };
	gl.DrawTriangle(ta1, tb1, tc1, 2, FS);//���Ƶ�һ��������


	Point4 a2(415.692193816530, 0, 340, 1);
	Point4 b2(0, 480, 100, 1);
	Point4 c2(415.692193816530, 480, 340, 1);
	auto ta2 = Persective(a2, 80);
	auto tb2 = Persective(b2, 80);
	auto tc2 = Persective(c2, 80);
	ta2.ValueArray = { 1,0 };
	tb2.ValueArray = { 0,1 };
	tc2.ValueArray = { 1,1 };
	gl.DrawTriangle(ta2, tb2, tc2, 2, FS);//���Ƶڶ���������

	getchar();
	return 0;
}