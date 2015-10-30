#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "resource.h"
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
TCHAR* FileName(HWND hWnd, TCHAR* lpstrFile, LPCWSTR lpstrFilter, int MaxLen);

HINSTANCE g_hInst;
INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	g_hInst = hInstance;
	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, DlgProc);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static int NumImage = 0;
	static int CurImage = 0;
	static TCHAR* Paths[20];//at most 20 bitmaps.
	switch (iMsg)
	{
	case WM_INITDIALOG:
		SetTimer(hDlg, 0, 20, NULL);
		break;
	case WM_TIMER:
		if (NumImage)
		{
			SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, Paths[CurImage], SPIF_UPDATEINIFILE);
			if (++CurImage == NumImage) CurImage = 0;
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON1:
		{
			TCHAR* ImagePath = new TCHAR[MAX_PATH*10]{0};
			if (FileName(hDlg, ImagePath, L"BMP FILE(*.BMP*)\0*.BMP*\0",MAX_PATH*10))
			{
				//SINGLE?
				//Paths[NumImage] = ImagePath;
				//++NumImage;
				TCHAR* Dir = ImagePath;
				for (TCHAR* i = ImagePath;*i||*(i + 1);++i)
				{
					if (*i == L'\0')
					{
						TCHAR* NewImage = new TCHAR[MAX_PATH]{};
						wsprintf(NewImage, L"%s\\%s", ImagePath, i + 1);
						Paths[NumImage] = NewImage;
						++NumImage;
					}
				}	
				//MULTIPLE?
			}
			delete ImagePath;
			break;
		}
		case IDQUIT:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	case WM_DESTROY:
		KillTimer(hDlg, 0);
		for (auto p : Paths)
		{
			delete p;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

TCHAR* FileName(HWND hWnd, TCHAR* lpstrFile, LPCWSTR lpstrFilter, int MaxLen)
{
	TCHAR szFileTitle[MAX_PATH];
	TCHAR InitDir[MAX_PATH];

	OPENFILENAME OFN;
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hWnd;
	OFN.lpstrFilter = lpstrFilter;
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = MaxLen;
	OFN.lpstrTitle = TEXT("PLEASE SELECT FILE (*RANGE)");
	OFN.lpstrFileTitle = szFileTitle;
	OFN.nMaxFileTitle = MAX_PATH;
	OFN.Flags |= OFN_ALLOWMULTISELECT | OFN_EXPLORER;
	GetWindowsDirectory(InitDir, MAX_PATH);
	OFN.lpstrInitialDir = InitDir;

	if (GetOpenFileName(&OFN))
		return lpstrFile;
	else
	{
		TCHAR WTF[256];
		wsprintf(WTF, L"%u", CommDlgExtendedError());
		SetWindowText(hWnd, WTF);
		return 0;
	}
}