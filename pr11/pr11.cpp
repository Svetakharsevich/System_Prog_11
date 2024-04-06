// pr11.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "pr11.h"
#include "windows.h"
#include "commdlg.h"
#include <chrono>
#define IDC_EDIT1 2001
#define IDC_SYSMONTHCAL32
#define MAX_LOADSTRING 100
#define MSC_DAYSTATE 100
#define UDS_SETBUDDYINT 100
#define UDM_SETRANGE 100
#define IDC_LIST 100


// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    SaveAs(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Repalce(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PR11, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PR11));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW| CS_DBLCLKS;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PR11));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PR11);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   
   CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("SysMonthCal32"), nullptr, WS_CHILD | WS_VISIBLE | MSC_DAYSTATE, 0, 0, 200, 300, hWnd, (HMENU)MSC_DAYSTATE, hInstance, NULL);
   HWND Edit= CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""), ES_AUTOHSCROLL|ES_LEFT|WS_CHILD|WS_VISIBLE, 220, 10, 100, 25, hWnd,(HMENU)IDC_EDIT1,hInstance,NULL );
   HWND hListBox = CreateWindowEx(0, TEXT("ListBox"), NULL, WS_VISIBLE | WS_CHILD | LBS_STANDARD,
       220, 50, 100, 120, hWnd, NULL, hInstance, NULL);

   SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)L"Элемент 1");
   SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)L"Элемент 2");
   SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)L"Элемент 3");
   HWND hSpin=CreateWindowEx(0, TEXT("msctls_updown32"), NULL, WS_CHILD | WS_VISIBLE | UDS_SETBUDDYINT,
       350, 10, 20, 20, hWnd, NULL, hInstance, Edit);
   SendMessage(hSpin, UDM_SETRANGE, 0, MAKELPARAM(100, 0));
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
TCHAR szFindWhat[80] = TEXT("");
TCHAR szReplaceWith[80] = TEXT("");
BOOL showMasag = FALSE;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    OPENFILENAME ofn;
    FINDREPLACE ofn1;
    
    static TCHAR szFileName[MAX_PATH];
    switch (message)
    {
    case WM_CREATE:{
        SetTimer(hWnd, 1, 10000, NULL);
    }break;
    //case WM_LBUTTONDBLCLK: {
    //    MessageBox(hWnd, L" Двойной клик", L"Информация", MB_OK | MB_ICONINFORMATION);
    //}break;
/*    case WM_LBUTTONDOWN: {
        MessageBox(hWnd, L" Левая кнопка мыши нажата", L"Информация", MB_OK | MB_ICONINFORMATION);
        break;} */      
    case WM_LBUTTONUP: {
        MessageBox(hWnd, L" Левая кнопка мыши отпущена", L"Информация", MB_OK | MB_ICONINFORMATION);
    break;
    }        
    case WM_MOUSEWHEEL: {
        MessageBox(hWnd, L" Колесо мыши прокручивается", L"Информация", MB_OK | MB_ICONINFORMATION);
        break;
    }        
    case WM_SIZING: {
        MessageBox(hWnd, L"Размер окна изменился", L"Информация", MB_OK | MB_ICONINFORMATION);
        break;
    }        
    case WM_SYSCHAR: {
        MessageBox(hWnd, L"Пользователь вводит символ при удерживании клавиши ALT", L"Информация", MB_OK | MB_ICONINFORMATION);
        break;
    }       
    case WM_SYSKEYUP: {
        MessageBox(hWnd, L"Пользователь отпустил сис.клавишу", L"Информация", MB_OK | MB_ICONINFORMATION);
        break;
    }       
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {

            case ID_SAVEAS:
                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = NULL;
                ofn.lpstrFile = szFileName;
                ofn.lpstrFile[0] = '\0';
                ofn.nMaxFile = sizeof(szFileName);
                ofn.lpstrFilter = TEXT("All Files\0*.*\0");
                ofn.nFilterIndex = 1;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = NULL;
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                if (GetSaveFileName(&ofn) == TRUE) { MessageBox(hWnd, szFileName, TEXT("Выберите файл"), MB_OK); }
 /*               DialogBox(hInst, MAKEINTRESOURCE(IDD_SAVE_AS), hWnd, SaveAs);*/
                break;
            
            case ID_REPLACE: 
                ZeroMemory(&ofn1, sizeof(ofn1));
                ofn1.lStructSize = sizeof(ofn1);
                ofn1.hwndOwner = hWnd;
                ofn1.wFindWhatLen = sizeof(szFindWhat);
                ofn1.wReplaceWithLen = sizeof(szReplaceWith);
                ofn1.Flags = FR_DOWN;
                ofn1.lpstrFindWhat = szFindWhat;
                ofn1.lpstrReplaceWith = szReplaceWith;
                if (ReplaceText(&ofn1) != FALSE) {}
                break;
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_TIMER: {
        if (wParam == 1 && !showMasag) {
            showMasag = TRUE;
            KillTimer(hWnd, 1);
            MessageBox(hWnd, L"Сработал таймер посе 10 секунд", L"Информация", MB_OK|MB_ICONINFORMATION );
                
        }
    }
    break;
    case WM_DESTROY:
        if (!showMasag) {
            KillTimer(hWnd, 1);
        }
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK SaveAs(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
INT_PTR CALLBACK Replace(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

