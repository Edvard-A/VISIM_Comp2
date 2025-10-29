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
    qDebug() << "Spline ";
    /*float tangen = 1;

    tangen =
        (curve.EvaluateBSplineSimple(t2).y+curve.EvaluateBSplineSimple(t1).y)/
        ((curve.EvaluateBSplineSimple(t2).z+curve.EvaluateBSplineSimple(t1).z)+
         (curve.EvaluateBSplineSimple(t2).x+curve.EvaluateBSplineSimple(t1).x));

    t1 = t2;
    t2 += 0.001;


    tangen = tan(tangen);
*/
    glm::vec3 pos = ballMesh->GetPosition();

    t1 = t2;
    t2 += 0.001f;
    float domainStart = curve.t[curve.degree];
    float domainEnd   = curve.t[curve.controlPoints.size() - 1 + 1];
    if (t2 > domainEnd) t2 = domainStart;

    glm::vec3 p1 = curve.EvaluateBSplineSimple(t1);
    glm::vec3 p2 = curve.EvaluateBSplineSimple(t2);
    glm::vec3 tangent = p2 - p1;
    if (glm::length(tangent) > 1e-6f)
        tangent = glm::normalize(tangent);



    /*if (input.Held(input.Key.SPACE))
    {*/
        pos = glm::vec3();
        velocity = glm::vec3();
    //}

    //velocity += curve.EvaluateBSplineSimple(0);
    velocity += updateVelocity(t1);//updateVelocity(tangen);

    pos += velocity / 60.0f;
    pos.y = terrainMesh->GetHeightAt(pos) + 0.1f;

    qDebug() << "xPos"<< pos.x;
    ballMesh->SetPosition(pos);
}
