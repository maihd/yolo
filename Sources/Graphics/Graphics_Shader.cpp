#include <GL/glew.h>
#include <Graphics/Graphics.h>

static Handle CreateGLShader(GLenum shaderType, const char* source)
{
    GLuint shader = glCreateShader(shaderType);
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

Shader LoadShader(StringView vertexFile, StringView pixelsFile)
{
    return Shader{ 0 };
}

Shader CompileShader(StringView vertexSource, StringView pixelsSource)
{
    Handle vertexShader = CreateGLShader(GL_VERTEX_SHADER, vertexSource.Buffer);
    Handle pixelsShader = CreateGLShader(GL_FRAGMENT_SHADER, pixelsSource.Buffer);

    if (!vertexShader || !pixelsShader)
    {   
        glDeleteShader((GLuint)vertexShader);
        glDeleteShader((GLuint)pixelsShader);
        return Shader{ 0 };
    }

    GLuint program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, pixelsShader);
    glLinkProgram(program);

    int programStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &programStatus);
    if (!programStatus)
    {
        char infoLog[1024] = "";
        glGetProgramInfoLog(program, sizeof(infoLog), 0, infoLog);

        glDeleteProgram(program);
        program = 0;
    }

    glDeleteShader((GLuint)vertexShader);
    glDeleteShader((GLuint)pixelsShader);
    return Shader{ program };
}

void FreeShader(Shader* shader)
{
    glDeleteProgram((GLuint)shader->Handle);
    shader->Handle = 0;
}

void SetShaderVector3(Shader shader, StringView name, Vector3 data)
{
    glUniform3f(glGetUniformLocation(shader.Handle, name.Buffer), data.x, data.y, data.z);
}

void SetShaderMatrix4(Shader shader, StringView name, Matrix4 data)
{
    glUniformMatrix4fv(glGetUniformLocation(shader.Handle, name.Buffer), 1, false, (float*)&data);
}
