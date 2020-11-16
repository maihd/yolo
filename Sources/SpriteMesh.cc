#include "./SpriteMesh.h"

#include "./Internal.h"

#include <assert.h>
#include <stdlib.h>
#include <Yolo/Vertex.h>

SpriteMesh SpriteMesh::New(void)
{
    SpriteMesh spriteMesh;

    glGenBuffers(1, &spriteMesh.vertexBuffer);
    glGenVertexArrays(1, &spriteMesh.vertexArray);

    glBindVertexArray(spriteMesh.vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, spriteMesh.vertexBuffer);

    VertexColor vertices[] = {
        { { -0.5f, -0.5f }, { 0.0f, 1.0f }, { 1, 1, 1, 1 } },
        { { -0.5f,  0.5f }, { 0.0f, 0.0f }, { 1, 1, 1, 1 } },
        { {  0.5f,  0.5f }, { 1.0f, 0.0f }, { 1, 1, 1, 1 } },

        { { -0.5f, -0.5f }, { 0.0f, 1.0f }, { 1, 1, 1, 1 } },
        { {  0.5f,  0.5f }, { 1.0f, 0.0f }, { 1, 1, 1, 1 } },
        { {  0.5f, -0.5f }, { 1.0f, 1.0f }, { 1, 1, 1, 1 } },
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), (const void*)offsetof(VertexColor, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexColor), (const void*)offsetof(VertexColor, uv));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexColor), (const void*)offsetof(VertexColor, color));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return spriteMesh;
}

void SpriteMesh::Free(SpriteMesh* spriteMesh)
{
    assert(spriteMesh);

    glDeleteBuffers(1, &spriteMesh->vertexBuffer);
    glDeleteVertexArrays(1, &spriteMesh->vertexArray);

    spriteMesh->vertexBuffer = 0;
    spriteMesh->vertexArray = 0;
}
