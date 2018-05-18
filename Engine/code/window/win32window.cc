#include "stdneva.h"
#include "win32window.h"

using namespace Window;
using namespace Direct3D11;

/*----------------------------------------------
*/
Win32Window::Win32Window()
	: name(""),
	description(""),
	width(0.0f),
	height(0.0f)
{
		// empty
}

/*----------------------------------------------
*/
bool Win32Window::Setup(const char* name, const char* description, float width, float height, 
	const HINSTANCE& hInstance, PWSTR pCmdLine, int nCmdShow)
{	
	this->name = name;
	this->description = description;
	this->width = width;
	this->height = height;
	this->wndClass = new WNDCLASS();

	this->wndClass->lpfnWndProc = (WNDPROC)WindowProc;
	this->wndClass->hInstance = hInstance;
	this->wndClass->lpszClassName = this->name;
	this->wndClass->hCursor = LoadCursor(NULL, IDC_ARROW);
	
	RegisterClass(this->wndClass);
	this->windowHandle = CreateWindowEx(0, this->name, this->description, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, this->width, this->height, NULL, NULL, hInstance, this
	);

	if (this->windowHandle == NULL)
	{
		return false;
	}
	
	ShowWindow(this->windowHandle, nCmdShow);
	return true;
}

/*----------------------------------------------
*/
long __stdcall Win32Window::WindowProc(HWND hwnd, unsigned int msg, unsigned int wParam, long lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0); 
		return 0;		
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

/*----------------------------------------------
*/
void Win32Window::Destroy()
{
	if (this->wndClass != nullptr)
	{
		delete this->wndClass;
		this->wndClass = 0;
	}
}


/*----------------------------------------------
*/
Win32Window::~Win32Window()
{
	this->Destroy();
}