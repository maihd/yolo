#include "RenderSystem.h"

#include "../Framework/ECSCoordinator.h"
#include "../Components/Types.h"

#include <Math/Math.h>
#include <Container/Array.h>

#include <Graphics/Window.h>
#include <Graphics/Graphics.h>

static constexpr StringView VertexShader =
    "#version 330 core\n"

    "layout(location = 0) in vec3 inPos;"
    "layout(location = 1) in vec3 inNormal;"

    "uniform mat4 uModel;"
    "uniform mat4 uView;"
    "uniform mat4 uProjection;"

    "out vec3 normal;"
    "out vec3 fragPosWorld;"

    "void main()"
    "{"
        "fragPosWorld = vec3(uModel * vec4(inPos, 1.0));"
        "normal = vec3(uModel * vec4(inNormal, 0.0));"

        "gl_Position = uProjection * uView * uModel * vec4(inPos, 1.0f);"
    "}";

static constexpr StringView FragmentShader =
    "#version 330 core\n"

    "out vec4 FragColor;"

    "in vec3 normal;"
    "in vec3 fragPosWorld;"

    "uniform vec3 uColor;"

    "vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);"

    "vec3 lightPos = vec3(-100.0f, 50.0f, 100.0f);"

    "void main()"
    "{"
        // Ambient
        "float ambientStrength = 0.5;"
        "vec3 ambient = ambientStrength * lightColor;"

        // Diffuse
        "vec3 norm = normalize(normal);"
        "vec3 lightDir = normalize(lightPos - fragPosWorld);"
        "float diff = max(dot(norm, lightDir), 0.0);"
        "vec3 diffuse = diff * lightColor;"

        "vec3 result = (ambient + diffuse) * uColor;"
        "FragColor = vec4(result, 1.0);"
    "}";

static constexpr Vector3 vertices[] = {
    Vector3{ -0.5f, -0.5f, -0.5f },
    Vector3{ 0.5f, -0.5f, -0.5f },
    Vector3{ 0.5f, 0.5f, -0.5f },
    Vector3{ 0.5f, 0.5f, -0.5f },
    Vector3{ -0.5f, 0.5f, -0.5f },
    Vector3{ -0.5f, -0.5f, -0.5f },
    Vector3{ -0.5f, -0.5f, 0.5 },
    Vector3{ 0.5f, -0.5f, 0.5 },
    Vector3{ 0.5f, 0.5f, 0.5 },
    Vector3{ 0.5f, 0.5f, 0.5 },
    Vector3{ -0.5f, 0.5f, 0.5 },
    Vector3{ -0.5f, -0.5f, 0.5 },
    Vector3{ -0.5f, 0.5f, 0.5f },
    Vector3{ -0.5f, 0.5f, -0.5f },
    Vector3{ -0.5f, -0.5f, -0.5f },
    Vector3{ -0.5f, -0.5f, -0.5f },
    Vector3{ -0.5f, -0.5f, 0.5f },
    Vector3{ -0.5f, 0.5f, 0.5f },
    Vector3{ 0.5f, 0.5f, 0.5 },
    Vector3{ 0.5f, 0.5f, -0.5 },
    Vector3{ 0.5f, -0.5f, -0.5 },
    Vector3{ 0.5f, -0.5f, -0.5 },
    Vector3{ 0.5f, -0.5f, 0.5 },
    Vector3{ 0.5f, 0.5f, 0.5 },
    Vector3{ -0.5f, -0.5f, -0.5f },
    Vector3{ 0.5f, -0.5f, -0.5f },
    Vector3{ 0.5f, -0.5f, 0.5f },
    Vector3{ 0.5f, -0.5f, 0.5f },
    Vector3{ -0.5f, -0.5f, 0.5f },
    Vector3{ -0.5f, -0.5f, -0.5f },
    Vector3{ -0.5f, 0.5f, -0.5 },
    Vector3{ 0.5f, 0.5f, -0.5 },
    Vector3{ 0.5f, 0.5f, 0.5 },
    Vector3{ 0.5f, 0.5f, 0.5 },
    Vector3{ -0.5f, 0.5f, 0.5 },
    Vector3{ -0.5f, 0.5f, -0.5 }
};

static constexpr Vector3 normals[] = {
	Vector3{ 0.0f, 0.0f, -1.0f },
	Vector3{ 0.0f, 0.0f, -1.0f },
	Vector3{ 0.0f, 0.0f, -1.0f },
	Vector3{ 0.0f, 0.0f, -1.0f },
	Vector3{ 0.0f, 0.0f, -1.0f },
	Vector3{ 0.0f, 0.0f, -1.0f },
	Vector3{ 0.0f, 0.0f, 1.0f },
	Vector3{ 0.0f, 0.0f, 1.0f },
	Vector3{ 0.0f, 0.0f, 1.0f },
	Vector3{ 0.0f, 0.0f, 1.0f },
	Vector3{ 0.0f, 0.0f, 1.0f },
	Vector3{ 0.0f, 0.0f, 1.0f },
	Vector3{ -1.0f, 0.0f, 0.0f },
	Vector3{ -1.0f, 0.0f, 0.0f },
	Vector3{ -1.0f, 0.0f, 0.0f },
	Vector3{ -1.0f, 0.0f, 0.0f },
	Vector3{ -1.0f, 0.0f, 0.0f },
	Vector3{ -1.0f, 0.0f, 0.0f },
	Vector3{ 1.0f, 0.0f, 0.0f },
	Vector3{ 1.0f, 0.0f, 0.0f },
	Vector3{ 1.0f, 0.0f, 0.0f },
	Vector3{ 1.0f, 0.0f, 0.0f },
	Vector3{ 1.0f, 0.0f, 0.0f },
	Vector3{ 1.0f, 0.0f, 0.0f },
	Vector3{ 0.0f, -1.0f, 0.0f },
	Vector3{ 0.0f, -1.0f, 0.0f },
	Vector3{ 0.0f, -1.0f, 0.0f },
	Vector3{ 0.0f, -1.0f, 0.0f },
	Vector3{ 0.0f, -1.0f, 0.0f },
	Vector3{ 0.0f, -1.0f, 0.0f },
	Vector3{ 0.0f, 1.0f, 0.0f },
	Vector3{ 0.0f, 1.0f, 0.0f },
	Vector3{ 0.0f, 1.0f, 0.0f },
	Vector3{ 0.0f, 1.0f, 0.0f },
	Vector3{ 0.0f, 1.0f, 0.0f },
	Vector3{ 0.0f, 1.0f, 0.0f }
};

static void OnWindowResize(RenderSystem* renderSystem, void* eventData);

void RenderSystem::Init(ECSCoordinator* coordinator)
{
    ECSCoordinator_AddEventListener(coordinator, ECS_EVENT_RESIZED, (ECSEventHandler)OnWindowResize, this);

    Shader = CompileShader(VertexShader, FragmentShader);

    CameraEntity = ECSCoordinator_CreateEntity(coordinator);
    ECSCoordinator_AddComponent(coordinator, CameraEntity, Transform {
        Vector3 { 0.0f, 0.0f, 500.0f }
    });

    ECSCoordinator_AddComponent(coordinator, CameraEntity, Camera {
        Matrix4Perspective(45.0f, (float)WindowWidth() / WindowHeight(), 0.1f, 1000.0f)
    });

    VertexArray = MakeVertexArray();
    SetVertexData(VertexArray, vertices, sizeof(vertices), BufferUsage::StaticDraw);
    DefineAttribute(VertexArray, 0, DataType::Vector3, sizeof(Vector3), 0, false);

    SetNormalData(VertexArray, normals, sizeof(normals), BufferUsage::StaticDraw);
    DefineAttribute(VertexArray, 1, DataType::Vector3, sizeof(Vector3), 0, true);
}

void RenderSystem::Free(ECSCoordinator* coordinator)
{
    ECSCoordinator_RemoveEventListener(coordinator, ECS_EVENT_RESIZED, (ECSEventHandler)OnWindowResize, this);
}

void RenderSystem::Render(ECSCoordinator* coordinator, const float dt)
{
    Transform* cameraTransform = ECSCoordinator_GetComponent<Transform>(coordinator, CameraEntity);
    Camera* camera = ECSCoordinator_GetComponent<Camera>(coordinator, CameraEntity);

    Matrix4 viewMatrix = Matrix4Translation(-cameraTransform->Position);

    Matrix4 projectionMatrix = camera->Projection;

    for (ECSEntity entity : IterateArray(Entities))
    {
        Transform* transform = ECSCoordinator_GetComponent<Transform>(coordinator, entity);
        Renderable* renderable = ECSCoordinator_GetComponent<Renderable>(coordinator, entity);

        Matrix4 modelMatrix = Matrix4Transform(transform->Position, transform->Rotation, transform->Scale);

        SetShaderMatrix4(Shader, "uView", viewMatrix);
        SetShaderMatrix4(Shader, "uModel", viewMatrix);
        SetShaderVector3(Shader, "uColor", renderable->Color);
        SetShaderMatrix4(Shader, "uProjection", projectionMatrix);

        DrawArray(VertexArray, Shader, 36);
    }
}

void OnWindowResize(RenderSystem* renderSystem, void* eventData)
{
    //auto windowWidth = event.GetParam<unsigned int>(Events::Window::Resized::WIDTH);
    //auto windowHeight = event.GetParam<unsigned int>(Events::Window::Resized::HEIGHT);
    //
    //auto& camera = gCoordinator.GetComponent<Camera>(mCamera);
    //camera.projectionTransform = Camera::MakeProjectionTransform(45.0f, 0.1f, 1000.0f, windowWidth, windowHeight);
}
