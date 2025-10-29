#include "rollingball.h"
#include "../components/mesh.h"
#include "../components/terrain.h"
#include <glm/gtc/matrix_transform.hpp>

RollingBall::RollingBall()
{
}

void RollingBall::Init()
{
    ballMesh = new Mesh("Assets/Models/ball.obj");
    ballMesh->Scale(0.1f);
    terrainMesh = new Terrain();//("Assets/Textures/Hund.bmp");

    camera.position = glm::vec3(0.0f, 0.0f, 4.0f);

    renderer->SetLightPos(glm::vec3(0,2,0));

    float domainStart = curve.t[curve.degree];
    float domainEnd   = curve.t[curve.controlPoints.size() - 1 + 1];
    (void)domainEnd;

    t1 = domainStart;
    t2 = domainStart + 0.001f;
}


void RollingBall::Update()
{
    static float u = curve.t[curve.degree];
    static float v = 0.0f;
    float dt = 1.0f / 60.0f;

    glm::vec3 p = curve.EvaluateBSplineSimple(u);
    glm::vec3 p_next = curve.EvaluateBSplineSimple(u + 0.001f);
    glm::vec3 tangent = glm::normalize(p_next - p);

    float a_tangent = glm::dot(gravity, tangent);

    float acceleration = a_tangent;

    acceleration -= v;

    v += acceleration * dt;
    u += v * dt;

    float u_min = curve.t[curve.degree];
    float u_max = curve.t[curve.controlPoints.size()];
    if (u > u_max) u = u_min;

    velocity = glm::vec3();
    glm::vec3 pos = curve.EvaluateBSplineSimple(u);

    pos.y = terrainMesh->GetHeightAt(pos) + 0.1f;

    ballMesh->SetPosition(pos);

    qDebug() << "u:" << u << "v:" << v << "pos:" << pos.x << pos.y << pos.z;
}
