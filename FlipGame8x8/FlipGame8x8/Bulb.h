#pragma once
#ifndef BULB_H
#define BULB_H

#include <EasyX.h>

class LED8
{
private:
	static int _WndWidth;	// ���ڿ��
	static int _WndHeight;	// ���ڸ߶�

	static HWND _Hwnd;		// ���
	static DWORD* _Buff;	// �Դ�
	static HDC _Hdc;		// HDC

	static IMAGE BackgroundImage;
	static IMAGE BrightImage;
	static IMAGE DarkImage;

	static IMAGE ActiveBackgroundImage;
	static IMAGE BrightImagePiece[64];
	static IMAGE DarkImagePiece[64];
public:
	LED8() = delete;

	// ��������,������ʹ�ã����⺯������

	static HWND const& Hwnd;	// ���
	static DWORD* const& Buff;	// �Դ�
	static HDC const& Hdc;		// HDC

	static int const& WndWidth;
	static int const& WndHeight;

	// ��ֱ�����ñ���
	static int BulbWidth;	// ���ݿ��
	static int BulbHeight;	// ���ݸ߶�
	static int GapWidth;	// ��϶���
	static int GapHeight;	// ��϶�߶�

	static DWORD64 Board;	// ����

	static const IMAGE* SplitImage(const IMAGE&, IMAGE*, size_t, size_t, size_t = 0, size_t = 0, bool = true);

	static const IMAGE& SetBright(COLORREF);
	static const IMAGE& SetDark(COLORREF);
	static const IMAGE& SetBackground(COLORREF);

	static const IMAGE& SetBright(const IMAGE&);
	static const IMAGE& SetDark(const IMAGE&);
	static const IMAGE& SetBackground(const IMAGE&);

	static int SetBulbSize(int, int);	// ���õ��ݴ�С
	static int GetBulbSize(int&, int&);	// ��õ��ݴ�С
	static int SetGapSize(int, int);	// ���ü�϶��С
	static int GetGapSize(int&, int&);	// ��ü�϶��С

	static int ResizeWndSize(int, int);	// �������ڴ�С
	static int AutoWndSize();			// ����Ӧ���ڴ�С

	static HWND Create(DWORD64 = 0);
	static int Destory();
	static DWORD64 Write(DWORD64);
	static DWORD64 Write(int, int);
	static DWORD64 Read(DWORD64&);
	static int Update(bool = true);
};

#endif // !LED_H