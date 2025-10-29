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
}


void RollingBall::Update()
{
    qDebug() << "Spline ";
    float tangen = 1;

    tangen =
        (curve.EvaluateBSplineSimple(t2).y+curve.EvaluateBSplineSimple(t1).y)/
        ((curve.EvaluateBSplineSimple(t2).z+curve.EvaluateBSplineSimple(t1).z)+
         (curve.EvaluateBSplineSimple(t2).x+curve.EvaluateBSplineSimple(t1).x));

    t1 = t2;
    t2 += 0.001;


    tangen = tan(tangen);

    glm::vec3 pos = ballMesh->GetPosition();

    if (input.Held(input.Key.SPACE))
    {
        pos = glm::vec3();
        velocity = glm::vec3();
    }

    //velocity += curve.EvaluateBSplineSimple(0);
    velocity += updateVelocity(tangen);//updateVelocity(tangen);

    pos += velocity / 60.0f;
    pos.y = terrainMesh->GetHeightAt(pos) + 0.1f;

    qDebug() << "xPos"<< pos.x;
    ballMesh->SetPosition(pos);
}
