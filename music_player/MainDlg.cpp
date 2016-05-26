#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include "resource.h"
#include "MainDlg.h"
#include<mmsystem.h>
#include <commctrl.h>

/*
Template designed by RuPeng.com. Please visit http://www.rupeng.com for more information
如鹏网（http://www.rupeng.com）大学生计算机学习社区，提供大量免费视频学习教程，提供个性化一对一学习指导
*/
BOOL WINAPI Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        HANDLE_MSG(hWnd, WM_INITDIALOG, Main_OnInitDialog);
        HANDLE_MSG(hWnd, WM_COMMAND, Main_OnCommand);
		HANDLE_MSG(hWnd,WM_HSCROLL, Main_OnHScroll);
		HANDLE_MSG(hWnd,WM_CLOSE, Main_OnClose);
    }

    return FALSE;
}
void CALLBACK MyTimerProc(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime)
{
	SYSTEMTIME stLocal;
	GetLocalTime(&stLocal);
	TCHAR strTime[256];
	wsprintf(strTime,"%i年%i月%i日  %02i:%02i:%02i",stLocal.wYear,stLocal.wMonth,stLocal.wDay,stLocal.wHour,stLocal.wMinute,stLocal.wSecond);
	SetDlgItemText(hwnd,IDC_TIMER,strTime);
	TCHAR str[256];
    HWND Combo1 = GetDlgItem(hwnd,IDC_LIST1);
    int lcon = ListBox_GetCurSel(Combo1);
    ListBox_GetText(Combo1,lcon,str);
    HWND hs=GetDlgItem(hwnd,IDC_SLIDER1);				   
    int voice=SendMessage(hs,TBM_GETPOS,0,0);
    char vol[256];
    wsprintf(vol,"setaudio %s volume to %d",str,voice);
    mciSendString(vol,"",0,NULL);
}
void CALLBACK progressBar(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTime)
{
	TCHAR str[256],str1[256],str2[256],pp[256];
	GetDlgItemText(hwnd,IDC_EDIT2,str,sizeof(str));
	wsprintf(str2,"status %s position",str);
	mciSendString(str2,pp,sizeof(pp)/sizeof(TCHAR),NULL);
	int musictime=atoi(pp)/1000;
	int tsecond,tminute;
	tminute = musictime/60;
	tsecond = musictime-tminute*60;
	wsprintf(str1,"%02d:%02d",tminute,tsecond);
	SetDlgItemText(hwnd,IDC_CURTIME,str1);
	SendDlgItemMessage(hwnd,IDC_SLIDER2,TBM_SETPOS,true,(LPARAM)musictime);
}
BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
//	SetTimer(hwnd,0,1000,progressBar);
	SetTimer(hwnd,0,1000,MyTimerProc);
//	mciSendString("play D:\\KwDownload\\song\\开头提示音.mp3","",0,NULL);
	HINSTANCE hInst=GetModuleHandle(NULL);
    HICON hIcon=LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON1));
    SendMessage(hwnd,WM_SETICON,ICON_BIG,(LPARAM)hIcon);
	HWND hs=GetDlgItem(hwnd,IDC_SLIDER1);
	SendMessage(hs,TBM_SETRANGE,(WPARAM)1,(LPARAM)MAKELONG(0,1000));
	SendDlgItemMessage(hwnd,IDC_SLIDER1,TBM_SETPOS,true,(LPARAM)500);
    return TRUE;
}

void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{

    switch(id)
    {
        case IDC_BUTTON2:
		{
			OPENFILENAME ofn;
			char szFile[MAX_PATH];
			ZeroMemory(&ofn,sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFile = szFile;
			ofn.lpstrFile[0] = TEXT('\0');
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = TEXT("ALL\0*.*\0Text\0*.txt\0C++\0*.cpp\0mp3\0*.mp3\0");
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.hwndOwner = hwnd;
			ofn.Flags = OFN_EXPLORER|OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
			if(GetOpenFileName(&ofn))
			{
				char songname[256];
				wsprintf(songname,"play %s repeat",szFile);
				mciSendString(songname,"",0,NULL);
				SetDlgItemText(hwnd,IDC_EDIT2,szFile);
			}
		}
        break;
		case IDC_BUTTON3:
		{
			OPENFILENAME ofn;
			char szFile[MAX_PATH];
			ZeroMemory(&ofn,sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFile = szFile;
			ofn.lpstrFile[0] = TEXT('\0');
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = TEXT("mp3\0*.mp3\0AVI\0*.avi\0");
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.hwndOwner = hwnd;
			ofn.Flags = OFN_EXPLORER|OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT;
			if(GetOpenFileName(&ofn))
			{
				TCHAR songname[256];
				HWND Combo1 = GetDlgItem(hwnd,IDC_LIST1);
				int count = ListBox_GetCount(Combo1),loc,i=0;
				for(loc=0;loc<count;loc++)
				{
					ListBox_GetText(Combo1,loc,songname);
					if(strcmp(songname,szFile)==0){
						MessageBox(NULL,TEXT("此选项已经存在，请重新选择！"),TEXT("       重试"),MB_OK|MB_ICONEXCLAMATION);
						break;}
					i++;
				}
				if(i==count)
					ListBox_InsertString(Combo1,-1,szFile);
			}
		}
		break;
		case IDC_BUTTON4:
		{
			TCHAR EDIT[256],STOPSTR[256];
			GetDlgItemText(hwnd,IDC_EDIT2,EDIT,sizeof(EDIT));
			wsprintf(STOPSTR,"stop %s",EDIT);
			mciSendString(STOPSTR,"",0,NULL);
			TCHAR str1[256];
			HWND Combo1 = GetDlgItem(hwnd,IDC_LIST1);
			int cixu = ListBox_GetCurSel(Combo1);
			if(cixu==-1){
				MessageBox(NULL,TEXT("未选择播放歌曲"),TEXT("       重试"),MB_OK|MB_ICONEXCLAMATION);break;}
			ListBox_GetText(Combo1,cixu,str1);
			TCHAR songname[256];
			wsprintf(songname,"play %s repeat",str1);
			mciSendString(songname,"",0,NULL);
			SetDlgItemText(hwnd,IDC_EDIT2,str1);
			TCHAR leth[256],str2[256],str3[256];
			wsprintf(str2,"status %s length",str1);			
			mciSendString(str2,leth,sizeof(leth)/sizeof(char),NULL);
			int tminute,tsecond;
			int lll = atoi(leth);
			int totalsecond = atoi(leth)/1000;
			tminute = totalsecond/60;
			tsecond = totalsecond-tminute*60;
			wsprintf(str3,"%02d:%02d",tminute,tsecond);
			SetDlgItemText(hwnd,IDC_TIME,str3);
			SendDlgItemMessage(hwnd,IDC_SLIDER2,TBM_SETRANGEMAX,false,(LPARAM)(totalsecond-1));
			SendDlgItemMessage(hwnd,IDC_SLIDER2,TBM_SETRANGEMIN,false,(LPARAM)0);
			SetTimer(hwnd,1,1000,progressBar);			
		}
		break;
		case IDC_BUTTON1:
		{
			TCHAR str1[256];
			HWND Combo1 = GetDlgItem(hwnd,IDC_LIST1);
			int cixu = ListBox_GetCurSel(Combo1);
			if(cixu==-1){
				MessageBox(NULL,TEXT("未选择播放歌曲"),TEXT("       重试"),MB_OK|MB_ICONEXCLAMATION);break;}
			GetDlgItemText(hwnd,IDC_EDIT2,str1,sizeof(str1));
			TCHAR songname[256];
			wsprintf(songname,"pause %s",str1);
			mciSendString(songname,"",0,NULL);	
		}
		break;
		case IDC_BUTTON5:
		{
			
			HWND Combo1 = GetDlgItem(hwnd,IDC_LIST1);
			int cixu = ListBox_GetCurSel(Combo1);
			if(cixu==-1){
				MessageBox(NULL,TEXT("未选择歌曲"),TEXT("       重试"),MB_OK|MB_ICONEXCLAMATION);break;}			
			ListBox_DeleteString(Combo1,cixu);
		}
		break;
		case IDC_BUTTON6:
		{
			TCHAR str1[256];
			HWND Combo1 = GetDlgItem(hwnd,IDC_LIST1);
			int cixu = ListBox_GetCurSel(Combo1);
			if(cixu==-1){
				MessageBox(NULL,TEXT("未选择播放歌曲"),TEXT("       重试"),MB_OK|MB_ICONEXCLAMATION);break;}
			GetDlgItemText(hwnd,IDC_EDIT2,str1,sizeof(str1));
			TCHAR songname[256];
			wsprintf(songname,"play %s repeat",str1);
			mciSendString(songname,"",0,NULL);
			SetDlgItemText(hwnd,IDC_EDIT2,str1);
		}
		break;
		case IDC_BUTTON7:
		{
			TCHAR EDI[256],STOPS[256];
			HWND Combo1 = GetDlgItem(hwnd,IDC_LIST1);
			int cixu = ListBox_GetCurSel(Combo1);
			if(cixu==-1){
				MessageBox(NULL,TEXT("未选择歌曲"),TEXT("       重试"),MB_OK|MB_ICONEXCLAMATION);break;}
			GetDlgItemText(hwnd,IDC_EDIT2,EDI,sizeof(EDI));
			wsprintf(STOPS,"stop %s",EDI);
			mciSendString(STOPS,"",0,NULL);
			
			TCHAR str[256],str1[256],str2[256],str3[256];
			GetDlgItemText(hwnd,IDC_EDIT2,str,sizeof(str));
			int location;
			int count = ListBox_GetCount(Combo1);
			for(location=0;location<count;location++)
			{
				ListBox_GetText(Combo1,location,str2);
				if(strcmp(str,str2)==0)
				{
					if(location==0)
						location=count;
					location = location-1;
					ListBox_GetText(Combo1,location,str3);
					wsprintf(str1,"play %s repeat",str3);
					mciSendString(str1,"",0,NULL);
					SetDlgItemText(hwnd,IDC_EDIT2,str3);
					ListBox_SetCurSel(Combo1,location);
					break;
				}
			}
			TCHAR leth[256],str4[256],str5[256];
			wsprintf(str4,"status %s length",str3);			
			mciSendString(str4,leth,sizeof(leth)/sizeof(char),NULL);
			int tminute,tsecond;
			int lll = atoi(leth);
			int totalsecond = atoi(leth)/1000;
			tminute = totalsecond/60;
			tsecond = totalsecond-tminute*60;
			wsprintf(str5,"%02d:%02d",tminute,tsecond);
			SetDlgItemText(hwnd,IDC_TIME,str5);
			SendDlgItemMessage(hwnd,IDC_SLIDER2,TBM_SETRANGEMAX,false,(LPARAM)(totalsecond-1));
			SendDlgItemMessage(hwnd,IDC_SLIDER2,TBM_SETRANGEMIN,false,(LPARAM)0);
		}
		break;
		case IDC_BUTTON8:
		{	
			TCHAR EDIT[256],STOPSTR[256];
			HWND Combo1 = GetDlgItem(hwnd,IDC_LIST1);
			int cixu = ListBox_GetCurSel(Combo1);
			if(cixu==-1){
				MessageBox(NULL,TEXT("未选择播放歌曲"),TEXT("       重试"),MB_OK|MB_ICONEXCLAMATION);break;}
			GetDlgItemText(hwnd,IDC_EDIT2,EDIT,sizeof(EDIT));
			wsprintf(STOPSTR,"stop %s",EDIT);
			mciSendString(STOPSTR,"",0,NULL);
			
			TCHAR str[256],str1[256],str2[256],str3[256];
			GetDlgItemText(hwnd,IDC_EDIT2,str,sizeof(str));
			int location;
			int count = ListBox_GetCount(Combo1);
			for(location=0;location<count;location++)
			{
				ListBox_GetText(Combo1,location,str2);
				if(strcmp(str,str2)==0)
				{
					location++;
					if(location==count)
						location=0;
					ListBox_GetText(Combo1,location,str3);
					wsprintf(str1,"play %s repeat",str3);
					mciSendString(str1,"",0,NULL);
					SetDlgItemText(hwnd,IDC_EDIT2,str3);
					ListBox_SetCurSel(Combo1,location);
					break;
				}
			}
			TCHAR leth[256],str4[256],str5[256];
			wsprintf(str4,"status %s length",str3);			
			mciSendString(str4,leth,sizeof(leth)/sizeof(char),NULL);
			int tminute,tsecond;
			int totalsecond = atoi(leth)/1000;
			tminute = totalsecond/60;
			tsecond = totalsecond-tminute*60;
			wsprintf(str5,"%02d:%02d",tminute,tsecond);
			SetDlgItemText(hwnd,IDC_TIME,str5);
			SendDlgItemMessage(hwnd,IDC_SLIDER2,TBM_SETRANGEMAX,false,(LPARAM)(totalsecond-1));
			SendDlgItemMessage(hwnd,IDC_SLIDER2,TBM_SETRANGEMIN,false,(LPARAM)0);
		}
		case IDC_BUTTON9:
		{
			int loc;
			HWND Combo1 = GetDlgItem(hwnd,IDC_LIST1);
			int cixu = ListBox_GetCurSel(Combo1);
			if(cixu==-1){
				MessageBox(NULL,TEXT("无歌曲可保存"),TEXT("       重试"),MB_OK|MB_ICONEXCLAMATION);break;}
			OPENFILENAME ofn;
			char szFile[MAX_PATH];
			ZeroMemory(&ofn,sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFile = szFile;
			ofn.lpstrFile[0] = TEXT('\0');
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = TEXT("mp3\0*.mp3\0AVI\0*.avi\0text\0*.txt\0");
			ofn.nFilterIndex = 3;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.hwndOwner = hwnd;
			ofn.Flags = OFN_EXPLORER|OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT;
			if(GetSaveFileName(&ofn))
			{
				
				
				FILE *fp;
				TCHAR EDI[256],str[256],str1[256];
				wsprintf(str1,"%s.txt",szFile);
				int count = ListBox_GetCount(Combo1);
				fp = fopen(str1,"w");
				for(loc=0;loc<count;loc++)
				{
					ListBox_GetText(Combo1,loc,EDI);
					wsprintf(str,"%s\n",EDI);								
					fputs(str,fp);				
				}
				fclose(fp);
			}
			
		}
		break;
		case IDC_GETLIST:
		{
			OPENFILENAME ofn;
			char szFile[MAX_PATH];
			ZeroMemory(&ofn,sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFile = szFile;
			ofn.lpstrFile[0] = TEXT('\0');
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = TEXT("mp3\0*.mp3\0AVI\0*.avi\0text\0*.txt\0");
			ofn.nFilterIndex = 3;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.hwndOwner = hwnd;
			ofn.Flags = OFN_EXPLORER|OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT;
			if(GetOpenFileName(&ofn))
			{
				FILE *fp;
				HWND ListBox = GetDlgItem(hwnd,IDC_LIST1);
				TCHAR songname[256];
				fp = fopen(szFile,"a+");
				int size=100,loc;
				while(fgets(songname,sizeof(songname),fp)!=0)
				{
					for(loc = 0;loc<size;loc++)
					{
						if(songname[loc]=='\n')
							songname[loc]='\0';
					}
					ListBox_InsertString(ListBox,-1,songname);		
				}
				fclose(fp);
			}
			
		}
		break;
		case IDC_LIST1:
		{
			TCHAR buf[256];
			if(LBN_DBLCLK==codeNotify)
			{
				TCHAR EDIT[256],STOPSTR[256],PLAYSTR[256];
				HWND Combo1 = GetDlgItem(hwnd,IDC_LIST1);				
				GetDlgItemText(hwnd,IDC_EDIT2,EDIT,sizeof(EDIT));
				wsprintf(STOPSTR,"stop %s",EDIT);
				mciSendString(STOPSTR,"",0,NULL);
				int index = SendDlgItemMessage(hwnd,IDC_LIST1,LB_GETCURSEL,0,0);
				SetTimer(hwnd,2,1000,progressBar);
				SendDlgItemMessage(hwnd,IDC_LIST1,LB_GETTEXT,index,(LPARAM)buf);
				wsprintf(PLAYSTR,"play %s repeat",buf);
				mciSendString(PLAYSTR,"",0,NULL);
				SetDlgItemText(hwnd,IDC_EDIT2,buf);//				ListBox_SetCurSel(Combo1,index);
				TCHAR leth[256],str4[256],str5[256];
				wsprintf(str4,"status %s length",buf);			
				mciSendString(str4,leth,sizeof(leth)/sizeof(char),NULL);
				int tminute,tsecond;
				int totalsecond = atoi(leth)/1000;
				tminute = totalsecond/60;
				tsecond = totalsecond-tminute*60;
				wsprintf(str5,"%02d:%02d",tminute,tsecond);
				SetDlgItemText(hwnd,IDC_TIME,str5);
				SendDlgItemMessage(hwnd,IDC_SLIDER2,TBM_SETRANGEMAX,false,(LPARAM)(totalsecond-1));
				SendDlgItemMessage(hwnd,IDC_SLIDER2,TBM_SETRANGEMIN,false,(LPARAM)0);
								
			}						
		}
		break;
		case IDC_MOVIE:
		{
			TCHAR buf[256],buf1[256],buf2[256];
			TCHAR EDIT[256],STOPSTR[256],PLAYSTR[256];
			GetDlgItemText(hwnd,IDC_EDIT2,EDIT,sizeof(EDIT));
			wsprintf(STOPSTR,"stop %s",EDIT);
			mciSendString(STOPSTR,"",0,NULL);
			HWND movie = GetDlgItem(hwnd,IDC_LIST1);
			int cur = SendDlgItemMessage(hwnd,IDC_LIST1,LB_GETCURSEL,false,0);
			ListBox_GetText(movie,cur,buf);
			wsprintf(buf1,"open %s type MPEGVideo",buf);
			wsprintf(buf2,"play %s",buf);
			mciSendString(buf1,0,0,NULL);
			mciSendString(buf2,0,0,NULL);
			SetDlgItemText(hwnd,IDC_EDIT2,buf);								
		}
        default:
		break;
    }
}
void Main_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
   switch(GetDlgCtrlID(hwndCtl))
   {    //声音控制
	   case IDC_SLIDER1:
		   {
			   if(TB_ENDTRACK==code)
			   {
				   TCHAR str[256];
				   GetDlgItemText(hwnd,IDC_EDIT2,str,sizeof(str));
				   HWND hs=GetDlgItem(hwnd,IDC_SLIDER1);				   
				   int voice=SendMessage(hs,TBM_GETPOS,0,0);
				   char vol[256];
				   wsprintf(vol,"setaudio %s volume to %d",str,voice);
				   mciSendString(vol,"",0,NULL);
			   } 
		  }
		   break;
	   case IDC_SLIDER2:
		  {	   
			    char cmd[256];
				if(TB_ENDTRACK==code)
				{
					TCHAR str[256];
				    HWND Combo1 = GetDlgItem(hwnd,IDC_LIST1);
				    int lcon = ListBox_GetCurSel(Combo1);
				    ListBox_GetText(Combo1,lcon,str);
				    int p=SendMessage(hwndCtl,TBM_GETPOS,0,0);//获得滑动条的当前位置
				    wsprintf(cmd,"play %s from %d repeat",str,p*1000);
			        mciSendString(cmd,"",0,NULL);
				    mciSendString(cmd,"",0,NULL);
				}
		  }
		  break;					  
		  default:
		  break;	   		   
   }
}
void Main_OnClose(HWND hwnd)
{
    EndDialog(hwnd, 0);
}
