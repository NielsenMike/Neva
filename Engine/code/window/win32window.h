/**
* Basic win32 window.
*
* Mike Nielsen
* 2017
*/
#pragma once
#include <windows.h>
#include <renderer\direct3d11\direct3d11graphics.h>

namespace Window
{

class Win32Window
{
public:

	static Win32Window* window;
	/// static callback function for windows 
	static long __stdcall WindowProc(HWND hwnd, unsigned int msg, unsigned int wParam, long lParam);

	/// constructor
	Win32Window();
	/// destructor
	~Win32Window();

	/// Create window
	bool Setup(const char* name, const char* description, float width, float height,
		const HINSTANCE& hInstance, PWSTR pCmdLine, int nCmdShow);
	/// Destroy window
	void Destroy();

	/// get width of window
	float GetWitdh() const;
	/// get width of window
	float GetHeight() const;

	/// get windows window handle
	const HWND& GetWindowHandle();

private:
	const char* name;
	const char* description;
	float width;
	float height;

	WNDCLASS* wndClass;
	HWND windowHandle;
};


/*----------------------------------------------
*/
inline float Win32Window::GetWitdh() const
{
	return this->width;
};

/*----------------------------------------------
*/
inline float Win32Window::GetHeight() const
{
	return this->width;
}


/*----------------------------------------------
*/
inline const HWND& Win32Window::GetWindowHandle()
{
	return this->windowHandle;
};



}; // namespace Window


