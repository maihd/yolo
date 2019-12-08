#include <Yolo/Graphics.h>

#include <Yolo/Math.h>
#include <Yolo/Time.h>
#include <Yolo/String.h>
#include <Yolo/Window.h>
#include <Yolo/Shader.h>

#include "./Runtime.h"
#include "./SpriteMesh.h"
#include "./DrawBuffer.h"
#include "./DrawTextBuffer.h"

namespace Graphics
{
    HGLRC glContext;
    
    float lineWidth;

    mat4 projection;

    Shader shader;
    Shader fontShader;
    Shader spriteShader;
    SpriteMesh spriteMesh;

    Handle vertexBuffer;
    Handle indexBuffer;
    Handle vertexArray;

    DrawBuffer drawBuffer;
    DrawTextBuffer drawTextBuffer;

    static String vshaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 pos;"
        "layout (location = 1) in vec2 uv;"
        "layout (location = 2) in vec4 color;"
        "out vec2 fragUV;"
        "out vec4 fragColor;"
        "uniform mat4 model;"
        "uniform mat4 projection;"
        "void main() {"
        "fragUV = uv;"
        "fragColor = color;"
        "gl_Position = projection * model * vec4(pos, 1);"
        "}";

    static String fshaderSource =
        "#version 330\n"

        "in vec2 fragUV;"
        "in vec4 fragColor;"

        "uniform vec4 color;"

        "out vec4 resultColor;"

        "void main() {"
        "resultColor = vec4(1.0);"
        "}";

    static String spriteVertexSource =
        "#version 330 core\n"

        "layout (location = 0) in vec3 pos;"
        "layout (location = 1) in vec2 texcoord;"
        //"layout (location = 2) in vec4 color;"

        "out vec2 uv;"
        //"out vec4 fragColor;"

        "uniform mat4 model;"
        "uniform mat4 projection;"

        "void main() {"
        "uv = texcoord;"
        //"fragColor = color;"
        "gl_Position = projection * model * vec4(pos, 1);"
        "}";

    static String spritePixelSource =
        "#version 330 core\n"

        "in vec2 uv;"

        "uniform sampler2D tex;"

        "out vec4 resultColor;"

        "void main() {"
        "resultColor = texture(tex, uv);"
        "}";


    static String fontVertexSource =
        "#version 330 core\n"

        "layout (location = 0) in vec3 pos;"
        "layout (location = 1) in vec2 texcoord;"

        "out vec2 uv;"

        "uniform mat4 model;"
        "uniform mat4 projection;"

        "void main() {"
        "uv = texcoord;"
        "gl_Position = projection * model * vec4(pos, 1);"
        "}";

    static String fontPixelSource =
        "#version 330 core\n"

        "in vec2 uv;"

        "uniform sampler2D tex;"

        "out vec4 resultColor;"

        "void main() {"
        "float alpha = texture(tex, uv).r;"
        "resultColor = vec4(1.0, 1.0, 1.0, alpha);"
        "}";

    void ApplyDefaultSettings(void);
    void CreateDefaultObjects(void);

    bool LoadDriver(void)
    {
        TCHAR* className = TEXT("YOLO_WINDOW_CLASS");

        HINSTANCE hInstance = GetModuleHandle(NULL);
        HWND dummyWindow = CreateWindow(className, className, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, hInstance, NULL);
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
   
    bool Init(void)
    {
        if (!Runtime::mainWindow || !Runtime::mainWindowContext)
        {
            return false;
        }

        if (!LoadDriver())
        {
            return false;
        }

        // Make sure window is initialized
        HDC hdc = Runtime::mainWindowContext;
        if (!hdc)
        {
            return false;
        }

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
            WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,

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

            WGL_SAMPLE_BUFFERS_ARB, 1,
            WGL_SAMPLES_ARB, 4,

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

        // Default settings
        ApplyDefaultSettings();
        CreateDefaultObjects();

        return true;
    }

    void Quit(void)
    {
        wglDeleteContext(glContext);
        glContext = NULL;
    }

    void ApplyDefaultSettings(void) 
    {
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE);
        //glEnable(GL_DEPTH_TEST);
        //glEnable(GL_STENCIL_TEST);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //glEnable(GL_LINE_SMOOTH);
        //glEnable(GL_POLYGON_SMOOTH);

        //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

        SetVSync(true);
        SetLineWidth(1.0f);
    }

    void CreateDefaultObjects(void)
    {
        float width = (float)Window::GetWidth();
        float height = (float)Window::GetHeight();

        projection = mat4::Ortho(0, width, 0, height, 0.0f, 100.0f);
        shader = ShaderOps::Compile(vshaderSource, fshaderSource);

        fontShader = ShaderOps::Compile(fontVertexSource, fontPixelSource);
        spriteShader = ShaderOps::Compile(spriteVertexSource, spritePixelSource);

        spriteMesh = SpriteMesh::New();
        drawBuffer = DrawBuffer::New();
        drawTextBuffer = DrawTextBuffer::New();
    }

    void Clear(void)
    {
        float width = (float)Window::GetWidth();
        float height = (float)Window::GetHeight();
        
        projection = mat4::Ortho(0, width, 0, height, 0.0f, 100.0f);

        // Set viewport
        glViewport(0, 0, Window::GetWidth(), Window::GetHeight());
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

    void PresentDrawBuffer(GLenum drawMode)
    {
        DrawBuffer::UpdateBuffers(&drawBuffer);

        mat4 model = mat4::Translation(0, 0);

        glUseProgram(shader.handle);
        glUniformMatrix4fv(glGetUniformLocation(shader.handle, "projection"), 1, false, (float*)&projection);
        glUniformMatrix4fv(glGetUniformLocation(shader.handle, "model"), 1, false, (float*)&model);

        glBindVertexArray(drawBuffer.vertexArray.handle);
        glBindBuffer(GL_ARRAY_BUFFER, drawBuffer.vertexArray.vertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawBuffer.vertexArray.indexBuffer);

        GLsizei indexCount = Array::Length(drawBuffer.indices);
        glDrawElements(drawMode, indexCount, GL_UNSIGNED_SHORT, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glUseProgram(0);
    }

    bool IsVSync(void)
    {
        if (wglGetSwapIntervalEXT)
        {
            return wglGetSwapIntervalEXT() != 0;
        }
        else
        {
            return false;
        }
    }

    void SetVSync(bool enable)
    {
        if (wglSwapIntervalEXT)
        {
            wglSwapIntervalEXT((int)enable);
        }
    }
    
    bool IsWireframe(void)
    {
        int value;
        glGetIntegerv(GL_POLYGON_MODE, &value);

        return value == GL_LINE;
    }

    void SetWireframe(bool enable)
    {
        glPolygonMode(GL_FRONT_AND_BACK, enable ? GL_LINE : GL_FILL);
    }

    float GetLineWidth(void)
    {
        return lineWidth;
    }

    void SetLineWidth(float width)
    {
        glLineWidth(width);
        lineWidth = width;
    }

    void DrawCircle(DrawMode mode, vec2 position, float radius, vec4 color, int segments)
    {
        DrawBuffer::Clear(&drawBuffer);

        if (mode == DrawMode::Line)
        {
            DrawBuffer::AddCircleLines(&drawBuffer, position, radius, color, segments);
            PresentDrawBuffer(GL_LINE_STRIP);
        }
        else
        {
            DrawBuffer::AddCircle(&drawBuffer, position, radius, color, segments);
            PresentDrawBuffer(GL_TRIANGLES);
        }
    }

    void DrawRectangle(DrawMode mode, vec2 position, vec2 size, vec4 color)
    {
        DrawBuffer::Clear(&drawBuffer);
        if (mode == DrawMode::Line)
        {
            DrawBuffer::AddRectangleLines(&drawBuffer, position, size, color);
            PresentDrawBuffer(GL_LINE_STRIP);
        }
        else
        {
            DrawBuffer::AddRectangle(&drawBuffer, position, size, color);
            PresentDrawBuffer(GL_TRIANGLES);
        }
    }

    void DrawTexture(Texture texture, vec2 position, vec2 size)
    {
        mat4 model = mul(mat4::Translation(position), mat4::Scalation(size));
        
        int projectionLocation = glGetUniformLocation(shader.handle, "projection");
        int modelLocation = glGetUniformLocation(shader.handle, "model");

        glUseProgram(spriteShader.handle);
        glUniformMatrix4fv(projectionLocation, 1, false, (float*)&projection);
        glUniformMatrix4fv(modelLocation, 1, false, (float*)&model);

        glBindVertexArray(spriteMesh.vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, spriteMesh.vertexArray);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.handle);

        //int textureLocation = glGetUniformLocation(spriteShader.handle, "image");
        //glUniform1i(textureLocation, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindTexture(GL_TEXTURE_2D, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);
    }

#undef DrawText
    void DrawText(String text, Font font, vec2 position)
    {
        DrawTextBuffer::AddText(&drawTextBuffer, text, font);
        DrawTextBuffer::UpdateBuffers(&drawTextBuffer);

        mat4 model = mul(mat4::Translation(position), mat4::Scalation(1.0f, -1.0f));

        glUseProgram(fontShader.handle);

        int projectionLocation = glGetUniformLocation(shader.handle, "projection");
        int modelLocation = glGetUniformLocation(shader.handle, "model");
        glUniformMatrix4fv(projectionLocation, 1, false, (float*)&projection);
        glUniformMatrix4fv(modelLocation, 1, false, (float*)&model);

        glBindVertexArray(drawTextBuffer.vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, drawTextBuffer.vertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawTextBuffer.indexBuffer);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, font.texture.handle);

        int indexCount = Array::Length(drawTextBuffer.indices);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, 0);

        glBindTexture(GL_TEXTURE_2D, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glUseProgram(0);

        DrawTextBuffer::Clear(&drawTextBuffer);
    }

    void DrawFramerate(Font font, vec2 position)
    {
        char buffer[1024];
        float framerate = Time::GetFramerate();
        String text = StringOps::Format(buffer, sizeof(buffer), "FPS: %.2f", framerate);
        DrawText(text, font, position);
    }
}

