#ifndef _GRAPHICS
#define _GRAPHICS
/*
���ļ�����һ�������࣬ʹ��EasyX�ṩSetPixel��������������󣬿��������������ṩSetPixel����������GDI��D2D��SDL��ʵ��
*/
#include <graphics.h>      // ����Easyx��ͼ�ο�ͷ�ļ�
class Graphics
{
private:
	DWORD* g_pBuf;//�Դ�ָ��
public:
	int WIDTH, HEIGHT;
	Graphics(int w,int h);//��������,w,h�ֱ�Ϊ�������
	void setPixel(int x, int y, COLORREF c);//������ָ����������һ����ɫ
	void clear();
	~Graphics();
};
#endif // !_GRAPHICS