#include <Yolo/Math.h>
#include <Yolo/Time.h>
#include <Yolo/String.h>
#include <Yolo/Window.h>
#include <Yolo/Graphics.h>

#include "./Internal.h"
#include "./SpriteMesh.h"
#include "./DrawBuffer.h"
#include "./DrawTextBuffer.h"
#include "./DrawSpriteBuffer.h"

#include "./Imgui/imgui_impl_sdl.h"
#include "./Imgui/imgui_impl_opengl3.h"

namespace Graphics
{
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

    constexpr StringView vshaderSource =
        "#version 330 core\n"

        "layout (location = 0) in vec3 pos;"
        "layout (location = 1) in vec4 color;"
        "out vec4 fragColor;"

        "uniform mat4 model;"
        "uniform mat4 projection;"

        "void main() {"
        "fragColor = color;"
        "gl_Position = projection * model * vec4(pos, 1);"
        "}";

    constexpr StringView fshaderSource =
        "#version 330 core\n"

        "in vec2 fragUV;"
        "in vec4 fragColor;"

        "out vec4 resultColor;"

        "void main() {"
        "resultColor = fragColor;"
        "}";

    constexpr StringView spriteVertexSource =
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

    constexpr StringView spritePixelSource =
        "#version 330 core\n"

        "in vec2 uv;"
        "in vec4 fragColor;"

        "uniform sampler2D tex;"

        "out vec4 resultColor;"

        "void main() {"
        "resultColor = texture(tex, uv) * fragColor;"
        "}";


    constexpr StringView fontVertexSource =
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

    constexpr StringView fontPixelSource =
        "#version 330 core\n"

        "in vec2 uv;"

        "uniform sampler2D tex;"

        "out vec4 resultColor;"

        "void main() {"
        "float alpha = texture(tex, uv).r;"
        "resultColor = vec4(1.0, 1.0, 1.0, alpha);"
        "}";

    void ApplyDefaultSettings(void) 
    {
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //glEnable(GL_LINE_SMOOTH);
        //glEnable(GL_POLYGON_SMOOTH);

        //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        SetVSync(true);
        SetLineWidth(1.0f);
    }

    void CreateDefaultObjects(void)
    {
        float width = (float)WindowWidth();
        float height = (float)WindowHeight();

        projection = Matrix4Ortho(0, width, 0, height, 0.0f, 100.0f);
        shader = CompileShader(vshaderSource, fshaderSource);

        fontShader = CompileShader(fontVertexSource, fontPixelSource);
        spriteShader = CompileShader(spriteVertexSource, spritePixelSource);

        spriteMesh = SpriteMesh::New();
        drawBuffer = DrawBufferNew();
        drawTextBuffer = DrawTextBuffer::New();
        drawSpriteBuffer = DrawSpriteBufferOps::New();
    }

    void Clear(void)
    {
        // Set viewport
        int windowWidth = WindowWidth();
        int windowHeight = WindowHeight();
        glViewport(0, 0, windowWidth, windowHeight);

        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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

    void NewFrame(void)
    {
        Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(Runtime.MainWindow);
        ImGui::NewFrame();
    }

    void Present(void)
    {
        DrawSpriteBufferOps::Draw(&drawSpriteBuffer, spriteShader, projection);

        ImGui::Render();
        ImGuiIO& io = ImGui::GetIO();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }

        WindowSwapBuffer();
    }

    void PresentDrawBuffer(GLenum drawMode)
    {
        DrawBufferUpdateBuffers(&drawBuffer);

        Matrix4 model = Matrix4Translation(0, 0);

        glUseProgram(shader.Handle);
        glUniformMatrix4fv(glGetUniformLocation(shader.Handle, "projection"), 1, false, (float*)&projection);
        glUniformMatrix4fv(glGetUniformLocation(shader.Handle, "model"), 1, false, (float*)&model);

        glBindVertexArray(drawBuffer.VertexArray.Handle);
        glBindBuffer(GL_ARRAY_BUFFER, drawBuffer.VertexArray.VertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawBuffer.VertexArray.IndexBuffer);

        GLsizei indexCount = drawBuffer.Indices.Count;
        glDrawElements(drawMode, indexCount, GL_UNSIGNED_SHORT, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glUseProgram(0);
    }

    bool IsVSync(void)
    {
        return SDL_GL_GetSwapInterval();
    }

    void SetVSync(bool enable)
    {
        SDL_GL_SetSwapInterval(enable);
    }
    
    bool IsWireframe(void)
    {
        I32 value;
        glGetIntegerv(GL_POLYGON_MODE, &value);

        return value == GL_LINE;
    }

    void SetWireframe(bool enable)
    {
        //glPolygonMode(GL_FRONT_AND_BACK, enable ? GL_LINE : GL_FILL);
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
}

void DrawCircle(DrawMode mode, Vector2 position, float radius, Vector4 color, I32 segments)
{
    DrawBufferClear(&Graphics::drawBuffer);

    if (mode == DrawMode::Line)
    {
        DrawBufferAddCircleLines(&Graphics::drawBuffer, position, radius, color, segments);
        Graphics::PresentDrawBuffer(GL_LINE_STRIP);
    }
    else
    {
        DrawBufferAddCircle(&Graphics::drawBuffer, position, radius, color, segments);
        Graphics::PresentDrawBuffer(GL_TRIANGLES);
    }
}

void DrawRectangle(DrawMode mode, Vector2 position, Vector2 size, Vector4 color)
{
    DrawBufferClear(&Graphics::drawBuffer);
    if (mode == DrawMode::Line)
    {
        DrawBufferAddRectangleLines(&Graphics::drawBuffer, position, size, color);
        Graphics::PresentDrawBuffer(GL_LINE_STRIP);
    }
    else
    {
        DrawBufferAddRectangle(&Graphics::drawBuffer, position, size, color);
        Graphics::PresentDrawBuffer(GL_TRIANGLES);
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
    DrawSpriteBufferOps::AddTexture(&Graphics::drawSpriteBuffer, texture, position, rotation, scale, color, pivot);
}

#undef DrawText
void DrawText(StringView text, Font font, Vector2 position)
{
    DrawTextBuffer::AddText(&Graphics::drawTextBuffer, text.Buffer, font);
    DrawTextBuffer::UpdateBuffers(&Graphics::drawTextBuffer);

    Matrix4 model = mul(Matrix4Translation(position), Matrix4Scalation(1.0f, -1.0f));

    glUseProgram(Graphics::fontShader.Handle);

    I32 projectionLocation = glGetUniformLocation(Graphics::fontShader.Handle, "projection");
    I32 modelLocation = glGetUniformLocation(Graphics::fontShader.Handle, "model");
    glUniformMatrix4fv(projectionLocation, 1, false, (float*)&Graphics::projection);
    glUniformMatrix4fv(modelLocation, 1, false, (float*)&model);

    glBindVertexArray(Graphics::drawTextBuffer.vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, Graphics::drawTextBuffer.vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Graphics::drawTextBuffer.indexBuffer);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font.Texture.Handle);

    I32 indexCount = Graphics::drawTextBuffer.indices.Count;
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUseProgram(0);

    DrawTextBuffer::Clear(&Graphics::drawTextBuffer);
}

void DrawFramerate(Font font, Vector2 position)
{
    //DrawRectangle(DrawMode::Fill, position, Vector2{ 100.0f, 50.0f }, Vector4{ 0, 0, 0, 0.2f });

    char buffer[1024];
    float framerate = GetFramerate();
    String text = StringFormat(buffer, sizeof(buffer), "FPS: %.2f", framerate);
    DrawText(text, font, position);
}
