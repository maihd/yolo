#include <Yolo/Shader.h>

#include <GL/glew.h>

Handle CreateGLShader(GLenum shaderType, string source)
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

Shader Shader::Load(string vertexFile, string pixelsFile)
{
    return { 0 };    
}

Shader Shader::Compile(string vertexSource, string pixelsSource)
{
    Handle vertexShader = CreateGLShader(GL_VERTEX_SHADER, vertexSource);
    Handle pixelsShader = CreateGLShader(GL_FRAGMENT_SHADER, pixelsSource);

    if (!vertexShader || !pixelsShader)
    {
        return { 0 };
    }

    Shader shader = { glCreateProgram() };

    glAttachShader(shader.handle, vertexShader);
    glAttachShader(shader.handle, pixelsShader);
    glLinkProgram(shader.handle);

    int programStatus;
    glGetProgramiv(shader.handle, GL_LINK_STATUS, &programStatus);
    if (!programStatus)
    {
        char infoLog[1024] = "";
        glGetProgramInfoLog(shader.handle, sizeof(infoLog), 0, infoLog);
        glDeleteProgram(shader.handle);
        shader.handle = 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(pixelsShader);
    return shader;
}

