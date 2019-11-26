#include <Yolo/Graphics.h>

#include <Yolo/Math.h>
#include <Yolo/String.h>
#include <Yolo/Window.h>
#include <Yolo/Shader.h>

#include "./Runtime.h"
#include "./SpriteMesh.h"
#include "./DrawBuffer.h"

namespace Graphics
{
    SDL_GLContext glContext;
    
    mat4 projection;

    Shader shader;
    Shader spriteShader;
    SpriteMesh spriteMesh;

    Handle vertexBuffer;
    Handle indexBuffer;
    Handle vertexArray;

    DrawBuffer drawBuffer;

    void ApplyDefaultSettings(void);
    void CreateDefaultObjects(void);
   
    bool Init(void)
    {
        if (!Runtime::mainWindow)
        {
            return false;
        }

        //    WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        //    WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        //    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        //    
        //    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        //    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        //
        //    WGL_ACCUM_RED_BITS_ARB, 8,
        //    WGL_ACCUM_GREEN_BITS_ARB, 8,
        //    WGL_ACCUM_BLUE_BITS_ARB, 8,
        //    WGL_ACCUM_ALPHA_BITS_ARB, 8,
        //
        //    WGL_RED_BITS_ARB, 8,
        //    WGL_GREEN_BITS_ARB, 8,
        //    WGL_BLUE_BITS_ARB, 8,
        //    WGL_ALPHA_BITS_ARB, 8,
        //
        //    WGL_RED_SHIFT_ARB, 0,
        //    WGL_GREEN_SHIFT_ARB, 8,
        //    WGL_BLUE_SHIFT_ARB, 16,
        //    WGL_ALPHA_SHIFT_ARB, 24,
        //
        //    WGL_ACCUM_BITS_ARB, 32,
        //    WGL_COLOR_BITS_ARB, 32,
        //    WGL_DEPTH_BITS_ARB, 24,
        //    WGL_STENCIL_BITS_ARB, 8,
        //
        //    WGL_SAMPLE_BUFFERS_ARB, 1,
        //    WGL_SAMPLES_ARB, 4,
        //
        //    //WGL_ARB_multisample && settings.multisamples > 1 ? WGL_SAMPLE_BUFFERS_ARB : 0, 1,
        //    //WGL_ARB_multisample && settings.multisamples > 1 ? WGL_SAMPLES_ARB : 0, settings.multisamples,
        //
        //    0

        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

        SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 8);

        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

        glContext = SDL_GL_CreateContext(Runtime::mainWindow);
        if (!glContext)
        {
            return false;
        }

        glewExperimental = false;
        GLenum glewState = glewInit();
        if (glewState != GLEW_OK)
        {
            return false;
        }
        
        if (SDL_GL_MakeCurrent(Runtime::mainWindow, glContext) != 0)
        {
            SDL_GL_DeleteContext(glContext);
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
        SDL_GL_DeleteContext(glContext);
        glContext = NULL;
    }

    void ApplyDefaultSettings(void) 
    {
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE);
        //glEnable(GL_DEPTH_TEST);
        //glEnable(GL_STENCIL_TEST);

        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //glEnable(GL_LINE_SMOOTH);
        //glEnable(GL_POLYGON_SMOOTH);

        //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

        SetVSync(true);
    }

    void CreateDefaultObjects(void)
    {
        float width = (float)Window::GetWidth();
        float height = (float)Window::GetHeight();

        projection = mat4::Ortho(0, width, 0, height, -100.0f, 100.0f);

        string vshaderSource =
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

        string fshaderSource =
            "#version 330\n"
            
            "in vec2 fragUV;"
            "in vec4 fragColor;"

            "uniform vec4 color;"
            
            "out vec4 resultColor;"
            
            "void main() {"
            "resultColor = fragColor;"
            "}";
        shader = Shader::Compile(vshaderSource, fshaderSource);

        string spriteVertexSource =
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

        string spritePixelSource =
            "#version 330 core\n"

            "in vec2 uv;"

            "out vec4 resultColor;"

            "uniform sampler2D mainTexture;"

            "void main() {"
            //"resultColor = vec4(uv, 0, 1);"
            "resultColor = texture(mainTexture, uv);"
            "}";
        spriteShader = Shader::Compile(spriteVertexSource, spritePixelSource);

        drawBuffer = DrawBuffer::New();
        spriteMesh = SpriteMesh::New();
    }

    void Clear(void)
    {
        float width = (float)Window::GetWidth();
        float height = (float)Window::GetHeight();
        
        projection = mat4::Ortho(0, width, 0, height, -100.0f, 100.0f);

        // Set viewport
        glViewport(0, 0, Window::GetWidth(), Window::GetHeight());

        glClear(GL_COLOR_BUFFER_BIT);

        DrawBuffer::Clear(&drawBuffer);
    }

    void ClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }

    void Present(void)
    {
        DrawBuffer::UpdateBuffers(&drawBuffer);

        mat4 model = mat4::Translation(0, 0);

        glUseProgram(shader.handle);
        glUniformMatrix4fv(glGetUniformLocation(shader.handle, "projection"), 1, false, (float*)&projection);
        glUniformMatrix4fv(glGetUniformLocation(shader.handle, "model"), 1, false, (float*)&model);

        glBindVertexArray(drawBuffer.vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, drawBuffer.vertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawBuffer.indexBuffer);

        glDrawElements(GL_TRIANGLES, Array::Length(drawBuffer.indices), GL_UNSIGNED_SHORT, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glUseProgram(0);

        Window::SwapBuffer();
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

    void DrawCircle(vec2 position, float radius, vec4 color, int segments)
    {
        DrawBuffer::AddCircle(&drawBuffer, position, radius, color, segments);
    }

    void DrawRectangle(vec2 position, vec2 size, vec4 color)
    {
        DrawBuffer::AddRectangle(&drawBuffer, position, size, color);
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

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindTexture(GL_TEXTURE_2D, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);
    }
}