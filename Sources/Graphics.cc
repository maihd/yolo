#include <Yolo/Graphics.h>

#include <Yolo/String.h>
#include <Yolo/Window.h>

#include "./Runtime.h"

namespace Graphics
{
    bool PrepareContext(void)
    {
        string className = "DUMMY_WINDOW";

        HINSTANCE hInstance = GetModuleHandleA(NULL);

        WNDCLASSA wndClass      = {};
        wndClass.hInstance      = hInstance;
        wndClass.lpszClassName  = className;
        wndClass.lpfnWndProc    = DefWindowProcA;
        wndClass.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

        if (!RegisterClassA(&wndClass))
        {
            return false;
        }

        HWND dummyWindow = CreateWindowA(className, className, 0, 0, 0, 800, 600, NULL, NULL, hInstance, NULL);
        if (!dummyWindow)
        {
            return false;
        }

        HDC hdc = GetDC(dummyWindow);
        if (!hdc)
        {
            return false;
        }

        int format;
        PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_STEREO,    // Flags
            PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
            32,                   // Color bits of the framebuffer.
            8, 0, 8, 8, 8, 16,    // Red, Green, Blue - [bits, shift]
            8, 24,                // Alpha - [bits, shift]
            32,                   // Accum bits
            8, 8, 8, 8,           // Accum channel bits: r, g, b, a
            24,                   // Number of bits for the depthbuffer
            8,                    // Number of bits for the stencilbuffer
            0,                    // Number of Aux buffers in the framebuffer.
            PFD_MAIN_PLANE,       // Layer type
            0,                    // Is rerversed
            0, 0, 0               // Unused
        };
        if ((format = ::ChoosePixelFormat(hdc, &pfd)) == 0)
        {
            return false;
        }

        if (!::SetPixelFormat(hdc, format, &pfd))
        {
            return false;
        }

        HGLRC dummyContext = wglCreateContext(hdc);
        if (!dummyContext)
        {
            return false;
        }
        
        wglMakeCurrent(hdc, dummyContext);

        glewExperimental = false;
        GLenum glewState = glewInit();
        if (glewState != GLEW_OK)
        {
            return false;
        }

        wglDeleteContext(dummyContext);
        DestroyWindow(dummyWindow);
        return true;
    }

    HGLRC glContext;
   
    bool Init(void)
    {
        if (!PrepareContext())
        {
            return false;
        }

        // Make sure window is initialized
        HDC hdc = GetDC(Runtime::mainWindow);
        if (!hdc)
        {
            return false;
        }

        // Settings affect image result
        // It should be check
        //if (!IsSettingsValid(settings))
        //{
        //    return false;
        //}

        // Modern api require differences extension to create _context
        if (!wglewIsSupported("WGL_ARB_create_context"))
        {
            return false;
        }

        int format;
        int numFormats;
        int formatAttribs[] = {
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,

            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            //WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,

            WGL_ACCUM_RED_BITS_ARB, 8,
            WGL_ACCUM_GREEN_BITS_ARB, 8,
            WGL_ACCUM_BLUE_BITS_ARB, 8,
            WGL_ACCUM_ALPHA_BITS_ARB, 8,

            WGL_RED_BITS_ARB, 8,
            WGL_GREEN_BITS_ARB, 8,
            WGL_BLUE_BITS_ARB, 8,
            WGL_ALPHA_BITS_ARB, 8,

            WGL_RED_SHIFT_ARB, 0,
            WGL_GREEN_SHIFT_ARB, 8,
            WGL_BLUE_SHIFT_ARB, 16,
            WGL_ALPHA_SHIFT_ARB, 24,

            WGL_ACCUM_BITS_ARB, 32,
            WGL_COLOR_BITS_ARB, 32,
            WGL_DEPTH_BITS_ARB, 24,
            WGL_STENCIL_BITS_ARB, 8,

            //WGL_ARB_multisample && settings.multisamples > 1 ? WGL_SAMPLE_BUFFERS_ARB : 0, 1,
            //WGL_ARB_multisample && settings.multisamples > 1 ? WGL_SAMPLES_ARB : 0, settings.multisamples,

            0
        };
        if (!wglChoosePixelFormatARB(hdc, formatAttribs, NULL, 1, &format, (UINT*)&numFormats))
        {
            return false;
        }

        if (!::SetPixelFormat(hdc, format, NULL))
        {
            return false;
        }

        int contextAttribs[16] =
        {
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4, // Highest current supported version
            WGL_CONTEXT_MINOR_VERSION_ARB, 5, // Highest current supported version
            WGL_CONTEXT_LAYER_PLANE_ARB, 0, // main plane
            WGL_CONTEXT_FLAGS_ARB, 0, // prevent use deprecated features
            0
        };

        glContext = wglCreateContextAttribsARB(hdc, 0, contextAttribs);
        if (!glContext)
        {
            return false;
        }

        if (!wglMakeCurrent(hdc, glContext))
        {
            wglDeleteContext(glContext);
            glContext = NULL;
            return false;
        }

        // By default, vsync is enable
        //Window::SetVSyncEnabled(true);

        // Set viewport
        //Graphics::Viewport(0, 0, Window::GetWidth(), Window::GetHeight());

        // Default settings
        //ApplyDefaultSettings();
        //CreateDefaultObjects();

        return true;
    }

    void Quit(void)
    {
        wglDeleteContext(glContext);
        glContext = NULL;
    }

    void Clear(void)
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void ClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }

    void Present(void)
    {
        Window::SwapBuffer();
    }
}