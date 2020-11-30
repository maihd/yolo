#include <Yolo/Graphics.h>

#include <GL/glew.h>

static Handle CreateGLShader(GLenum shaderType, const char* source)
{
    Handle shader = glCreateShader(shaderType);
    if (!shader)
    {
        return 0;
    }

    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);

    int status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        char infoLog[1024] = "";
        glGetShaderInfoLog(shader, sizeof(infoLog), 0, infoLog);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

Shader LoadShader(const char* vertexFile, const char* pixelsFile)
{
    return Shader{ 0 };
}

Shader CompileShader(const char* vertexSource, const char* pixelsSource)
{
    Handle vertexShader = CreateGLShader(GL_VERTEX_SHADER, vertexSource);
    Handle pixelsShader = CreateGLShader(GL_FRAGMENT_SHADER, pixelsSource);

    if (!vertexShader || !pixelsShader)
    {
        return Shader{ 0 };
    }

    Shader shader = { glCreateProgram() };

    glAttachShader(shader.Handle, vertexShader);
    glAttachShader(shader.Handle, pixelsShader);
    glLinkProgram(shader.Handle);

    int programStatus;
    glGetProgramiv(shader.Handle, GL_LINK_STATUS, &programStatus);
    if (!programStatus)
    {
        char infoLog[1024] = "";
        glGetProgramInfoLog(shader.Handle, sizeof(infoLog), 0, infoLog);
        glDeleteProgram(shader.Handle);
        shader.Handle = 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(pixelsShader);
    return shader;
}

void FreeShader(Shader* shader)
{
    glDeleteProgram(shader->Handle);
    shader->Handle = 0;
}
