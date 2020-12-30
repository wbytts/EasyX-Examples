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
	Point4 a(0, 0, 100, 1);
	Point4 b(320, 480, 100, 1);
	Point4 c(640, 0, 100, 1);
	auto ta = Persective(a, 80);
	auto tb = Persective(b, 80);
	auto tc = Persective(c, 80);
	ta.ValueArray = { 0,0 };
	tb.ValueArray = { 0.5,1 };
	tc.ValueArray = { 1,0 };
	gl.DrawTriangle(ta, tb, tc, 2, FS);
	getchar();
	return 0;
}