#include<Winsock2.h>
#include<iostream>
#include<graphics.h>
#include<conio.h>
#include<stdlib.h>
#include<vector>

#pragma comment(lib,"WS2_32.LIB")

using namespace std;
#define SERV_PORT 8086
#define MAXLISTENQ 5 


struct my_data
{
	char name[10];
	int id;
	vector<int> _data;
};

struct in_frame
{
	char name[10];
	int data;
};

my_data data[MAXLISTENQ] = { { "none", 0}, { "none", 1} ,{"none", 2}, {"none", 3}, {"none", 4} };

bool initSocket()//加载相应的dll
{
	WORD version;
	version = MAKEWORD(2, 2);
	WSAData wsadata;
	int err = WSAStartup(version, &wsadata);
	if (err != 0)
	{
		cout << "dll error" << endl;
		return false;
	}
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2)
	{
		cout << "dll version error" << endl;
		WSACleanup();
		return false;
	}
	return true;
}

#define WITH 640
#define HEIGHT 480

COLORREF my_color[MAXLISTENQ] = { RED, BLUE, GREEN, BROWN, LIGHTMAGENTA };
#define BETWEEN 20
#define RECT_WITH 104
#define RECT_HEIGH 104

void ui_update(my_data data);
void ui_init();
int getfreeid();
void CharToTchar(const char * _char, TCHAR * tchar);
void freeData(int id);
void childProcess(void *arg)
{
	int *connectfd = (int*)arg;
	int id = getfreeid();
	if (id < 0)
	{
		closesocket(*connectfd);
		delete connectfd;
		return;
	}
	while (1)
	{
		in_frame tempframe;
		int n = recv(*connectfd, (char*)&tempframe, sizeof(tempframe), 0);
		if (n <= 0)
		{
			closesocket(*connectfd);
			delete connectfd;
			freeData(id);
			ui_update(data[id]);
			return;
		}
		else
		{
			if (data[id]._data.size() < RECT_WITH)
			{
				memcpy(data[id].name, tempframe.name, strlen(tempframe.name)+1);
				int temp_data = ntohl(tempframe.data);
				data[id]._data.push_back(temp_data);
			}
			else
			{
				memcpy(data[id].name, tempframe.name, strlen(tempframe.name)+1);
				int temp_data = ntohl(tempframe.data);
				data[id]._data.erase(data[id]._data.begin());
				data[id]._data.push_back(temp_data);
			}
			ui_update(data[id]);
		}
	}
}

int main()
{
	ui_init();
	if (initSocket() == false)
	{
		return -1;
	}
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in serverAddr, clientAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERV_PORT);
	serverAddr.sin_addr.s_addr = htonl(ADDR_ANY);
	bind(listenfd, (sockaddr *)&serverAddr, sizeof(serverAddr));
	listen(listenfd, MAXLISTENQ);
	int connectfd;
	int clientAddrlen = sizeof(clientAddr);
	while (1)
	{
		connectfd = accept(listenfd, (sockaddr *)&(clientAddr), &clientAddrlen);
		if (connectfd > 0)
		{
			int* arg = new int;
			*arg = connectfd;
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)childProcess, (LPVOID)arg, 0, NULL);
		}
	}
}


#define top  (2 * BETWEEN + RECT_HEIGH + 33 + 16)
#define bottom (HEIGHT - BETWEEN - 30) 
void line_update(int id)
{
	setbkcolor(BLACK);
	int begin = BETWEEN + id*(RECT_WITH + BETWEEN)+2;
	clearrectangle(begin, top, begin + RECT_WITH-1, bottom);
	if (data[id]._data.size() > 1)
	{
		for (int i = 1; i < data[id]._data.size(); i++)
		{
			int y0 = bottom - data[id]._data[i - 1] * (bottom - top) / 100;
			int y1 = bottom - data[id]._data[i] * (bottom - top) / 100;
			setlinecolor(my_color[id]);
			line(begin + i - 1, y0, begin + i, y1);
		}
	}
}

void ui_init()
{
	initgraph(WITH, HEIGHT);
	setlinecolor(BLACK);
	settextstyle(16, 0, _T("宋体"));
	for (int i = 0; i < MAXLISTENQ; i++)
	{
		setfillcolor(my_color[i]);
		fillrectangle(BETWEEN + i*(RECT_WITH + BETWEEN), BETWEEN, (BETWEEN + RECT_WITH)*(i + 1), BETWEEN + RECT_HEIGH);
		setbkcolor(my_color[i]);
		outtextxy(BETWEEN + i*(RECT_WITH + BETWEEN)+10, BETWEEN*2, _T("name:"));
		outtextxy(BETWEEN + i*(RECT_WITH + BETWEEN)+10, BETWEEN*2+50, _T("usage:"));
	}
	setbkcolor(TRANSPARENT);
	setlinecolor(WHITE);
	rectangle(BETWEEN, 2 * BETWEEN + RECT_HEIGH, WITH - BETWEEN+2, HEIGHT - BETWEEN);
	outtextxy(BETWEEN + 3, 2 * BETWEEN + RECT_HEIGH + 3, _T("rate"));
	outtextxy(BETWEEN + 1, 2 * BETWEEN + RECT_HEIGH + 33, _T("-100%"));
	outtextxy(BETWEEN + 1, HEIGHT - BETWEEN - 30, _T("-0%"));
	outtextxy(WITH - BETWEEN - 40, HEIGHT - BETWEEN - 30, _T("time"));
	for (int i = 0; i < MAXLISTENQ; i++)
	{
		ui_update(data[i]);
	}
}

void ui_update(my_data data)
{
	if (strcmp(data.name, "none") == 0)
	{
		setbkcolor(my_color[data.id]);
		outtextxy(BETWEEN + data.id*(RECT_WITH + BETWEEN) + 60, BETWEEN * 2, _T("none"));
		outtextxy(BETWEEN + data.id*(RECT_WITH + BETWEEN) + 60, BETWEEN * 2 + 50, _T("--  "));
		line_update(data.id);
	}
	else
	{
		setbkcolor(my_color[data.id]);
		outtextxy(BETWEEN + data.id*(RECT_WITH + BETWEEN) + 60, BETWEEN * 2, _T("     "));
		outtextxy(BETWEEN + data.id*(RECT_WITH + BETWEEN) + 60, BETWEEN * 2 + 50, _T("   %"));
		TCHAR temp[6];
		CharToTchar(data.name,temp);
		outtextxy(BETWEEN + data.id*(RECT_WITH + BETWEEN) + 60, BETWEEN * 2, temp);
		char temp2[10];
		_itoa_s(data._data[data._data.size()-1], temp2,10,10);
		CharToTchar(temp2, temp);
		outtextxy(BETWEEN + data.id*(RECT_WITH + BETWEEN) + 60, BETWEEN * 2 + 50,temp);
		line_update(data.id);
	}
}
int getfreeid()
{
	for (int i = 0; i < MAXLISTENQ; i++)
	{
		if (strcmp(data[i].name, "none") == 0)
		{
			return data[i].id;
		}
	}
	return -1;
}

void freeData(int id)
{
	char *str = "none";
	memcpy(data[id].name, str, 5);
	data[id]._data.clear();
}

void CharToTchar(const char * _char, TCHAR * tchar)
{
#ifdef UNICODE  
	MultiByteToWideChar(CP_ACP, 0, _char, -1, tchar, 6);
#else  
	strcpy(Name, strUsr);
#endif 
}