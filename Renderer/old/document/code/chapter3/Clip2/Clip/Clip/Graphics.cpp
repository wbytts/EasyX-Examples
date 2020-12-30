#include "Graphics.h"

Graphics::Graphics(int w, int h):WIDTH(w), HEIGHT(h)
{
	initgraph(WIDTH, HEIGHT);//������ͼ����
	g_pBuf = GetImageBuffer(NULL);
}

void Graphics::setPixel(int x, int y, COLORREF c)
{
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)//������������ڻ�����Χ�������
	{
		return;
	}
	y = HEIGHT - 1 - y;//��Ϊeasyx��yֵ0���������棬���Խ���Ļ���·�ת��������
	g_pBuf[y * WIDTH + x] = BGR(c);
}

Graphics::~Graphics()
{
	closegraph();// �رջ�ͼ����
}
