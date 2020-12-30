#ifndef _MATRIX
#define _MATRIX
#include <algorithm>
#include <cmath>
class Matrix
{
public:
	int rows, columns;//����������
	double* buffer;//���ݻ�����
	Matrix(int r, int c);
	Matrix(const Matrix& src);
	~Matrix();
	Matrix operator *(const Matrix& b) const;
	double* operator [](int index) const;//����һ��һά����
	Matrix& operator=(const Matrix& b);
	static Matrix Perspective(double l, double r, double b, double t, double n, double f);//��ȡ͸��ͶӰ����
	static Matrix RotateX(double angle);//��ȡΧ��X����ת�ľ���
	static Matrix RotateY(double angle);//��ȡΧ��Y����ת�ľ���
	static Matrix RotateZ(double angle);//��ȡΧ��Z����ת�ľ���
	static Matrix Translate(double x, double y, double z);//ƽ��
	static Matrix Scale(double x, double y, double z);//����
private:

};
#endif