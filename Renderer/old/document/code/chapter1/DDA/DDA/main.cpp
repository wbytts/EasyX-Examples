#include "Graphics.h"
#include "Point2.h"
#include <stdio.h>
void DDA(Graphics& gp,Point2& A, Point2& B)
{
	double k = (A.Y - B.Y) / (A.X - B.X);//���A.X-B.X==0�Ļ���ʹ��int������⣬�ᱨ��0�쳣������double��inf(infinite)��nan(not a number)������1/0=inf,1/inf=0,1/inf=nan��inf�����κ���Ч���֣�-infС���κ���Ч���֣�����������Բ��ܷ�ĸΪ0�����
	Point2 tmp;
	if (k > 1 || k < -1)//����Y����
	{
		if (A.Y > B.Y)////ʹA��yС��B��y
		{
			tmp = A;
			A = B;
			B = tmp;
		}
		int y = (int)A.Y;
		double x = A.X;
		for (; y <= B.Y; y++)
		{
			gp.setPixel((int)x, y, WHITE);
			x += 1 / k;
		}
	}
	else//����X����
	{
		if (A.X > B.X)////ʹA��xС��B��x
		{
			tmp = A;
			A = B;
			B = tmp;
		}
		int x = (int)A.X;
		double y = A.Y;
		for (; x <= B.X; x++)
		{
			gp.setPixel(x, (int)y, WHITE);
			y += k;
		}
	}
}

int main()
{
	Graphics gp(640,480);//����һ�������������ṩ�˻��㺯��
	Point2 ps[2] = { {-10,400},{10,0} };//����������
	DDA(gp,ps[0], ps[1]);//ִ��DDA�㷨
	getchar();//�ȴ�������˳�
}