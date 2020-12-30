///////////////////////////////////////////////////
// �������ƣ�2D ����
// ���뻷����Visual C++ 2013��EasyX_20190219(beta)
// �������ߣ���Ʒʷ��ķ (799052200@qq.com)
// �������ڣ�2019-2-4
// ����޸ģ�2019-3-2
//
#include <graphics.h>      // ����ͼ�ο�ͷ�ļ�
#include <ctime>
#include <sstream>
#include <fstream>
#include <vector>
#include <conio.h>
// ���� MP3 ����
#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")

using namespace std;

#define	CMD_UP			1
#define	CMD_DOWN		2
#define	CMD_LEFT		4
#define	CMD_RIGHT		8
#define	sCMD_UP			16
#define	sCMD_DOWN		32
#define	sCMD_LEFT		64
#define	sCMD_RIGHT		128
#define	CMD_QUIT		256
#define PI				3.1415926

int MaxSpeed = 8;			// ����ٶ�
int FinSand = 5;			// ��ɳ�ϵ�Ħ����
int FinRoad = 1;			// ��·�ϵ�Ħ����
int FinIce = -2;			// �ڱ��ϵ�Ħ����
int SpeedAdd = 2;			// ���ٶ�
int Rota = 64;				// ת���ٶȵ� -1 �η�
int NeedR = 5;				// Ŀ��Ȧ��

int WIDE = 1280;
int HEIGHT = 960;
double EndLineForward = 0;	// �յ�Ƕ�

bool inIce;
bool inRoad;
bool inSand;
bool inWall;
bool inEndline;

IMAGE Racing;				// ������ͼ
IMAGE Toucher;				// ��ײͼ
IMAGE car1;
IMAGE car2;

IMAGE Player1;
int Px = 150;
int Py = 150;
double PForward = 0;		// ����
int Pspeed = 0;				// �ٶ�
int Ppass = 0;				// ͨ�������յ�
bool Pwrong = false;		// �Ƿ�����
bool PHadPass = false;		// �Ƿ�ͨ���յ�
bool PWaitOut = false;		// �Ƿ�ȴ�ͨ���յ�
bool Pover = false;			// �Ƿ����
clock_t Ptime = 0;
clock_t Ptime2 = 0;

IMAGE Player2;
int Cx = 170;
int Cy = 170;
double CForward = 0;
int Cspeed = 0;
int Cpass = 0;
bool Cwrong = false;
bool CHadPass = false;
bool CWaitOut = false;
bool Cover = false;
clock_t Ctime = 0;
clock_t Ctime2 = 0;

bool TwoPlayer = true;

bool isres = true;
bool chexit = false;

bool MeumMod = false;

clock_t Start = 0;
clock_t Now = 0;
clock_t MeumUsed = 0;

struct bottom				// ���׵İ�ťʵ��
{
	int ID;

	int x;
	int y;
	int wide;
	int heigh;
	RECT a;
	wstring str;

	COLORREF fillcolor;
	COLORREF linecolor;
	COLORREF textcolor;
	LOGFONT textstyle;
	UINT uFormat;

	bottom(int gID, int gx, int gy, int gw, int gh, wstring gs)
	{
		fillcolor = getfillcolor();
		linecolor = getlinecolor();
		textcolor = gettextcolor();
		gettextstyle(&textstyle);
		uFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
		ID = gID;
		x = gx;
		y = gy;
		wide = gw;
		heigh = gh;
		str = gs;
		a = { x, y, x + wide, y + heigh };
	}
};

struct page
{
	vector<bottom> botlist;

	bool MouseTouch(int left, int top, int right, int bottom, MOUSEMSG m)	// ��������ж�
	{
		for (int i1 = left; i1 < right; i1++)
		{
			for (int i2 = top; i2 < bottom; i2++)
			{
				if (m.x == i1 && m.y == i2)
				{
					return true;
				}
			}
		}
		return false;
	}

	int ShownPage()														// ��ʾ���ȴ���������Ӧ��������Ӧ��IDֵ
	{
		COLORREF fillcolor = getfillcolor();
		COLORREF linecolor = getlinecolor();
		COLORREF textcolor = gettextcolor();
		LOGFONT textstyle;
		gettextstyle(&textstyle);

		MOUSEMSG m;
		setbkmode(TRANSPARENT);

		for (unsigned int i = 0; i < botlist.size(); i++)
		{
			setfillcolor(botlist[i].fillcolor);
			setlinecolor(botlist[i].linecolor);
			settextcolor(botlist[i].textcolor);
			settextstyle(&botlist[i].textstyle);

			fillrectangle(botlist[i].x, botlist[i].y, botlist[i].x + botlist[i].wide, botlist[i].y + botlist[i].heigh);

			drawtext(botlist[i].str.c_str(), &botlist[i].a, botlist[i].uFormat);
		}
		FlushBatchDraw();

		while (true)
		{
			FlushMouseMsgBuffer();
			m = GetMouseMsg();
			if (m.mkLButton)
			{
				for (unsigned int i = 0; i < botlist.size(); i++)
				{
					if (MouseTouch(botlist[i].x, botlist[i].y, botlist[i].x + botlist[i].wide, botlist[i].y + botlist[i].heigh, m))
					{
						return botlist[i].ID;
					}
				}
			}
		}

		setfillcolor(fillcolor);
		setlinecolor(linecolor);
		settextcolor(textcolor);
		settextstyle(&textstyle);
	}
};

struct intro	// ��ͼ�Ľ�����Ϣ
{
	wstring filename;
	wstring title;
	wstring intr;
	wstring inipath;
};
vector<intro> IntroList;

class timer		// ��ʱ��
{
private:
	bool is_start = false;
	clock_t start;
public:
	bool WaitFor(clock_t s)
	{
		if (is_start)
		{
			if ((start + s) <= clock())
			{
				is_start = false;
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			start = clock();
			is_start = true;
			return false;
		}
	}
};

void init();
void gaming();
int GetCommand();
void DispatchCommand(int _cmd);
void OnLeft(bool player);		// false ��� 1��true ��� 2
void OnRight(bool player);
void OnUp(bool player);
void OnDown(bool player);
void MoveCheck(bool player);	// ��ײ�ж�
int PointTsm(int x, int y, int wide, int high);	// ��������ֵ��ת��
void Draw();
void End();
void PutImgWithout(IMAGE &obj, int px, int py, COLORREF withouter, DWORD* pbWnd, int wX, int wY, DWORD bitsub);	// ����ͼƬ������
void SetBirth();	// ��һ�ζ�ȡ
void StartWord();
void Loading();		// ���ص�ͼ
int ChooseMap();	// ѡ���ͼ
void LoadIntro(string File);
BOOL SearchFilesByWildcard(string wildcardPath);	// �����ļ����ο���https://blog.csdn.net/faithzzf/article/details/54290084
IMAGE zoomImage(IMAGE* pImg, int newWidth, int newHeight);	// ͼƬ����
void showhelp();	// ��ʾ�����ļ�
void clean();		// ��ջ�����
void restart();		// �������¿�ʼ��Ϸ
bool CanRota (bool player);//�Ƿ������ת
