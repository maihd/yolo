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
#include "./DrawSpriteBuffer.h"

namespace Graphics
{
    static SDL_GLContext    glContext;
    
    static float            lineWidth;
    static Vector4          clearColor;

    static Matrix4          projection;

    static Shader           shader;
    static Shader           fontShader;
    static Shader           spriteShader;
    static SpriteMesh       spriteMesh;

    static DrawBuffer       drawBuffer;
    static DrawTextBuffer   drawTextBuffer;
    static DrawSpriteBuffer drawSpriteBuffer;

    constexpr String vshaderSource =
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

    constexpr String fshaderSource =
        "#version 330\n"

        "in vec2 fragUV;"
        "in vec4 fragColor;"

        "uniform vec4 color;"

        "out vec4 resultColor;"

        "void main() {"
        "resultColor = vec4(1.0);"
        "}";

    constexpr String spriteVertexSource =
        "#version 330 core\n"

        "layout (location = 0) in vec3 pos;"
        "layout (location = 1) in vec2 texcoord;"
        "layout (location = 2) in vec4 color;"

        "out vec2 uv;"
        "out vec4 fragColor;"

        "uniform mat4 model;"
        "uniform mat4 projection;"

        "void main() {"
        "uv = texcoord;"
        "fragColor = color;"
        "gl_Position = projection * model * vec4(pos, 1);"
        "}";

    constexpr String spritePixelSource =
        "#version 330 core\n"

        "in vec2 uv;"
        "in vec4 fragColor;"

        "uniform sampler2D tex;"

        "out vec4 resultColor;"

        "void main() {"
        "resultColor = texture(tex, uv) * fragColor;"
        "}";


    constexpr String fontVertexSource =
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

    constexpr String fontPixelSource =
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
   
    bool Init(void)
    {
        // Make sure window is initialized
        if (!Runtime::mainWindow)
        {
            return false;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

        SDL_GLContext context = SDL_GL_CreateContext(Runtime::mainWindow);
        if (!context)
        {
            return false;
        }

        if (SDL_GL_MakeCurrent(Runtime::mainWindow, context) != 0)
        {
            SDL_GL_DeleteContext(context);
            return false;
        }

        glewExperimental = false;
        GLenum glewState = glewInit();
        if (glewState != GLEW_OK)
        {
            SDL_GL_DeleteContext(context);
            return false;
        }

        glContext = context;

        // Default settings
        ApplyDefaultSettings();
        CreateDefaultObjects();

        return true;
    }

    void Quit(void)
    {
        SDL_GL_DeleteContext(glContext);
        glContext = nullptr;
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
        float width = (float)WindowWidth();
        float height = (float)WindowHeight();

        projection = Matrix4Ortho(0, width, 0, height, 0.0f, 100.0f);
        shader = ShaderOps::Compile(vshaderSource, fshaderSource);

        fontShader = ShaderOps::Compile(fontVertexSource, fontPixelSource);
        spriteShader = ShaderOps::Compile(spriteVertexSource, spritePixelSource);

        spriteMesh = SpriteMesh::New();
        drawBuffer = DrawBufferOps::New();
        drawTextBuffer = DrawTextBuffer::New();
        drawSpriteBuffer = DrawSpriteBufferOps::New();
    }

    void Clear(void)
    {
        // Set viewport
        glViewport(0, 0, WindowWidth(), WindowHeight());
        glClear(GL_COLOR_BUFFER_BIT);

        // Clear buffers
        DrawSpriteBufferOps::Clear(&drawSpriteBuffer);
    }

    Vector4 GetClearColor()
    {
        return clearColor;
    }

    void SetClearColor(Vector3 color)
    {
        SetClearColor(color.x, color.y, color.z);
    }

    void SetClearColor(Vector4 color)
    {
        SetClearColor(color.x, color.y, color.z, color.w);
    }

    void SetClearColor(float r, float g, float b, float a)
    {
        clearColor = Vector4{ r, g, b, a };
        glClearColor(r, g, b, a);
    }

    void Present(void)
    {
        DrawSpriteBufferOps::Draw(&drawSpriteBuffer, spriteShader, projection);

        WindowSwapBuffer();
    }

    void PresentDrawBuffer(GLenum drawMode)
    {
        DrawBufferOps::UpdateBuffers(&drawBuffer);

        Matrix4 model = Matrix4Translation(0, 0);

        glUseProgram(shader.handle);
        glUniformMatrix4fv(glGetUniformLocation(shader.handle, "projection"), 1, false, (float*)&projection);
        glUniformMatrix4fv(glGetUniformLocation(shader.handle, "model"), 1, false, (float*)&model);

        glBindVertexArray(drawBuffer.vertexArray.handle);
        glBindBuffer(GL_ARRAY_BUFFER, drawBuffer.vertexArray.vertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawBuffer.vertexArray.indexBuffer);

        GLsizei indexCount = drawBuffer.indices.count;
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
            wglSwapIntervalEXT((I32)enable);
        }
    }
    
    bool IsWireframe(void)
    {
        I32 value;
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

    Matrix4 GetProjection(void)
    {
        return projection;
    }

    void SetProjection(Matrix4 projection)
    {
        Graphics::projection = projection;
    }

    void DrawCircle(DrawMode mode, Vector2 position, float radius, Vector4 color, I32 segments)
    {
        DrawBufferOps::Clear(&drawBuffer);

        if (mode == DrawMode::Line)
        {
            DrawBufferOps::AddCircleLines(&drawBuffer, position, radius, color, segments);
            PresentDrawBuffer(GL_LINE_STRIP);
        }
        else
        {
            DrawBufferOps::AddCircle(&drawBuffer, position, radius, color, segments);
            PresentDrawBuffer(GL_TRIANGLES);
        }
    }

    void DrawRectangle(DrawMode mode, Vector2 position, Vector2 size, Vector4 color)
    {
        DrawBufferOps::Clear(&drawBuffer);
        if (mode == DrawMode::Line)
        {
            DrawBufferOps::AddRectangleLines(&drawBuffer, position, size, color);
            PresentDrawBuffer(GL_LINE_STRIP);
        }
        else
        {
            DrawBufferOps::AddRectangle(&drawBuffer, position, size, color);
            PresentDrawBuffer(GL_TRIANGLES);
        }
    }

    void DrawTexture(Texture texture, Vector2 position, float rotation, Vector2 scale, Vector4 color, Vector2 pivot)
    {
#if 0
        Matrix4 model = mul(Matrix4::Translation(position), Matrix4::Scalation(size));
        
        I32 projectionLocation = glGetUniformLocation(shader.handle, "projection");
        I32 modelLocation = glGetUniformLocation(shader.handle, "model");

        glUseProgram(spriteShader.handle);
        glUniformMatrix4fv(projectionLocation, 1, false, (float*)&projection);
        glUniformMatrix4fv(modelLocation, 1, false, (float*)&model);

        glBindVertexArray(spriteMesh.vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, spriteMesh.vertexArray);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.handle);

        //I32 textureLocation = glGetUniformLocation(spriteShader.handle, "image");
        //glUniform1i(textureLocation, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindTexture(GL_TEXTURE_2D, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);
#endif
        DrawSpriteBufferOps::AddTexture(&drawSpriteBuffer, texture, position, rotation, scale, color, pivot);
    }

#undef DrawText
    void DrawText(String text, Font font, Vector2 position)
    {
        DrawTextBuffer::AddText(&drawTextBuffer, text, font);
        DrawTextBuffer::UpdateBuffers(&drawTextBuffer);

        Matrix4 model = mul(Matrix4Translation(position), Matrix4Scalation(1.0f, -1.0f));

        glUseProgram(fontShader.handle);

        I32 projectionLocation = glGetUniformLocation(shader.handle, "projection");
        I32 modelLocation = glGetUniformLocation(shader.handle, "model");
        glUniformMatrix4fv(projectionLocation, 1, false, (float*)&projection);
        glUniformMatrix4fv(modelLocation, 1, false, (float*)&model);

        glBindVertexArray(drawTextBuffer.vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, drawTextBuffer.vertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawTextBuffer.indexBuffer);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, font.texture.handle);

        I32 indexCount = drawTextBuffer.indices.count;
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, 0);

        glBindTexture(GL_TEXTURE_2D, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glUseProgram(0);

        DrawTextBuffer::Clear(&drawTextBuffer);
    }

    void DrawFramerate(Font font, Vector2 position)
    {
        char buffer[1024];
        float framerate = GetFramerate();
        String text = StringOps::Format(buffer, sizeof(buffer), "FPS: %.2f", framerate);
        DrawText(text, font, position);
    }
}

