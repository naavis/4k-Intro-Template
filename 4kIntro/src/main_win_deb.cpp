#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <GL/gl.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "intro.h"
#include "4klang.h"
#include "config.h"

//==============================================================================================

typedef struct
{
    //---------------
    HINSTANCE   hInstance;
    HDC         hDC;
    HGLRC       hRC;
    HWND        hWnd;
    //---------------
    int         full;
    //---------------
    char        wndclass[4];	// window class and title :)
    //---------------
}WININFO;

// MAX_SAMPLES gives you the number of samples for the whole song. we always produce stereo samples, so times 2 for the buffer
SAMPLE_TYPE	lpSoundBuffer[MAX_SAMPLES * 2];
HWAVEOUT	hWaveOut;

WAVEFORMATEX WaveFMT =
{
#ifdef FLOAT_32BIT	
	WAVE_FORMAT_IEEE_FLOAT,
#else
	WAVE_FORMAT_PCM,
#endif		
	2, // channels
	SAMPLE_RATE, // samples per sec
	SAMPLE_RATE * sizeof(SAMPLE_TYPE) * 2, // bytes per sec
	sizeof(SAMPLE_TYPE) * 2, // block alignment;
	sizeof(SAMPLE_TYPE) * 8, // bits per sample
	0 // extension not needed
};

WAVEHDR WaveHDR =
{
	(LPSTR)lpSoundBuffer,
	MAX_SAMPLES * sizeof(SAMPLE_TYPE) * 2,			// MAX_SAMPLES*sizeof(float)*2(stereo)
	0,
	0,
	0,
	0,
	0,
	0
};

MMTIME MMTime =
{
	TIME_SAMPLES,
	0
};

void  InitSound()
{
#ifdef USE_SOUND_THREAD
	// thx to xTr1m/blu-flame for providing a smarter and smaller way to create the thread :)
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)_4klang_render, lpSoundBuffer, 0, 0);
#else
	_4klang_render(lpSoundBuffer);
#endif
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &WaveFMT, NULL, 0, CALLBACK_NULL);
	waveOutPrepareHeader(hWaveOut, &WaveHDR, sizeof(WaveHDR));
	waveOutWrite(hWaveOut, &WaveHDR, sizeof(WaveHDR));
}



static PIXELFORMATDESCRIPTOR pfd =
    {
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    32,
    0, 0, 0, 0, 0, 0, 8, 0,
    0, 0, 0, 0, 0,  // accum
    32,             // zbuffer
    0,              // stencil!
    0,              // aux
    PFD_MAIN_PLANE,
    0, 0, 0, 0
    };

static WININFO wininfo = {  0,0,0,0,0,
							{'i','q','_',0}
                            };

//==============================================================================================

static LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// salvapantallas
	if( uMsg==WM_SYSCOMMAND && (wParam==SC_SCREENSAVE || wParam==SC_MONITORPOWER) )
		return( 0 );

	// boton x o pulsacion de escape
	if( uMsg==WM_CLOSE || uMsg==WM_DESTROY || (uMsg==WM_KEYDOWN && wParam==VK_ESCAPE) )
	{
		PostQuitMessage(0);
        return( 0 );
	}

    if( uMsg==WM_SIZE )
    {
        glViewport( 0, 0, lParam&65535, lParam>>16 );
    }

    if( uMsg==WM_CHAR || uMsg==WM_KEYDOWN)
    {
        if( wParam==VK_ESCAPE )
        {
            PostQuitMessage(0);
            return( 0 );
        }
    }

    return( DefWindowProc(hWnd,uMsg,wParam,lParam) );
}


static void window_end( WININFO *info )
{
    if( info->hRC )
    {
        wglMakeCurrent( 0, 0 );
        wglDeleteContext( info->hRC );
    }

    if( info->hDC  ) ReleaseDC( info->hWnd, info->hDC );
    if( info->hWnd ) DestroyWindow( info->hWnd );

    UnregisterClass( info->wndclass, info->hInstance );

    if( info->full )
    {
        ChangeDisplaySettings( 0, 0 );
		while( ShowCursor( 1 )<0 ); // show cursor
    }
}

static int window_init( WININFO *info )
{
	unsigned int	PixelFormat;
    DWORD			dwExStyle, dwStyle;
    DEVMODE			dmScreenSettings;
    RECT			rec;

    WNDCLASS		wc;

    ZeroMemory( &wc, sizeof(WNDCLASS) );
    wc.style         = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = info->hInstance;
    wc.lpszClassName = info->wndclass;
    wc.hbrBackground =(HBRUSH)CreateSolidBrush(0x00102030);
	
    if( !RegisterClass(&wc) )
        return( 0 );

    if( info->full )
    {
        dmScreenSettings.dmSize       = sizeof(DEVMODE);
        dmScreenSettings.dmFields     = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmPelsWidth  = XRES;
        dmScreenSettings.dmPelsHeight = YRES;

        if( ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
            return( 0 );

        dwExStyle = WS_EX_APPWINDOW;
        dwStyle   = WS_VISIBLE | WS_POPUP;

		while( ShowCursor( 0 )>=0 );	// hide cursor
    }
    else
    {
        dwExStyle = 0;
        dwStyle   = WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_OVERLAPPED;
		dwStyle   = WS_VISIBLE | WS_OVERLAPPEDWINDOW|WS_POPUP;

    }

    rec.left   = 0;
    rec.top    = 0;
    rec.right  = XRES;
    rec.bottom = YRES;

    AdjustWindowRect( &rec, dwStyle, 0 );

    info->hWnd = CreateWindowEx( dwExStyle, wc.lpszClassName, "Intro", dwStyle,
                               (GetSystemMetrics(SM_CXSCREEN)-rec.right+rec.left)>>1,
                               (GetSystemMetrics(SM_CYSCREEN)-rec.bottom+rec.top)>>1,
                               rec.right-rec.left, rec.bottom-rec.top, 0, 0, info->hInstance, 0 );

    if( !info->hWnd )
        return( 0 );

    if( !(info->hDC=GetDC(info->hWnd)) )
        return( 0 );

    if( !(PixelFormat=ChoosePixelFormat(info->hDC,&pfd)) )
        return( 0 );

    if( !SetPixelFormat(info->hDC,PixelFormat,&pfd) )
        return( 0 );

    if( !(info->hRC=wglCreateContext(info->hDC)) )
        return( 0 );

    if( !wglMakeCurrent(info->hDC,info->hRC) )
        return( 0 );

    return( 1 );
}


//==============================================================================================

int WINAPI WinMain( HINSTANCE instance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    MSG         msg;
    int         done=0;
    WININFO     *info = &wininfo;

    info->hInstance = GetModuleHandle( 0 );

  //if( MessageBox( 0, "fullscreen?", info->wndclass, MB_YESNO|MB_ICONQUESTION)==IDYES ) info->full++;

    if( !window_init(info) )
    {
        window_end( info );
        MessageBox( 0, "window_init()!","error",MB_OK|MB_ICONEXCLAMATION );
        return( 0 );
	}

    if( !intro_init() )
    {
        window_end( info );
        MessageBox( 0, "intro_init()!","error",MB_OK|MB_ICONEXCLAMATION );
        return( 0 );
	}

#ifndef SOUND_DISABLED
	InitSound();
#endif

    long to = timeGetTime();
    while( !done )
    {
		long t = timeGetTime() - to;
#ifndef SOUND_DISABLED
		waveOutGetPosition(hWaveOut, &MMTime, sizeof(MMTIME));
#endif

        while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE) )
        {
            if( msg.message==WM_QUIT ) done=1;
		    TranslateMessage( &msg );

            DispatchMessage( &msg );
        }

        intro_do(t);

        SwapBuffers( info->hDC );
        Sleep( 1 ); // give other processes some chance to do something

		if (MMTime.u.sample >= MAX_SAMPLES)
			done = 1;
    }

    sndPlaySound( 0, 0 );
    window_end( info );

    return( 0 );
}



