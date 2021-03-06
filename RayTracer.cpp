/*==================================================================================
* COSC 363  Computer Graphics (2020)
* Department of Computer Science and Software Engineering, University of Canterbury.
*
* A basic ray tracer
* See Lab07.pdf, Lab08.pdf for details.
*===================================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include "TextureBMP.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Plane.h"
#include <GL/freeglut.h>
using namespace std;

const float WIDTH = 20.0;  
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int NUMDIV = 500;
const int MAX_STEPS = 5;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;

float red;
float green;
float blue;

vector<SceneObject*> sceneObjects;
TextureBMP texture;
TextureBMP ball;



Plane* floorPlane()
{
    Plane *plane = new Plane (glm::vec3(-50., -15., -40.),   // Point A (Near left)   FROM your position
                              glm::vec3(50., -15., -40.),    // Point B (Near right)
                              glm::vec3(50., -15., -200.),   // Point C (Far right)
                              glm::vec3(-50., -15., -200.));  // Point D (Far left)
    return plane;
}

Plane* backgroundPlane()
{
    texture = TextureBMP("galaxy.bmp");
    Plane *background = new Plane (glm::vec3(-50., -15., -200.),   // Point A (Near left)   FROM your position
                                   glm::vec3(50., -15., -200.),    // Point B (Near right)
                                   glm::vec3(50., 50., -200.),   // Point C (Far right)
                                   glm::vec3(-50., 50., -200.));  // Point D (Far left)

    Plane *backgroundLEFT = new Plane (glm::vec3(-50., -15., -40.),   // Point A (Near left)   FROM your position
                                   glm::vec3(-50., -15., -200.),    // Point B (Near right)
                                   glm::vec3(-50., 50., -200.),   // Point C (Far right)
                                   glm::vec3(-50., 50., -40.));  // Point D (Far left)

    Plane *backgroundRIGHT = new Plane (glm::vec3(50., -15., -40.),   // Point A (Near left)   FROM your position
                                   glm::vec3(50., 50., -40.),    // Point B (Near right)
                                   glm::vec3(50., 50., -200.),   // Point C (Far right)
                                   glm::vec3(50., -15., -200.));  // Point D (Far left)

    Plane *backgroundBACK = new Plane (glm::vec3(50., -15., -40.),   // Point A (Near left)   FROM your position
                                   glm::vec3(-50., -15., -40.),    // Point B (Near right)
                                   glm::vec3(-50., 50., -40.),   // Point C (Far right)
                                   glm::vec3(50., -50., -40.));  // Point D (Far left)

    return background;
}


glm::vec3 rotate(glm::vec3 vec)
{
//    vec.x = vec.x * sqrt(4)/2 - vec.y * 1/2;
//    vec.y = vec.x * 1/2 + vec.y * sqrt(4)/2;
//    vec.z = vec.z;

    return vec;
}

Plane* cubeTopFace()
{
    Plane *top = new Plane(rotate(glm::vec3(-15., -10., -75.)),
                           rotate(glm::vec3(-10., -10., -75.)),
                           rotate(glm::vec3(-10., -10., -80.)),
                           rotate(glm::vec3(-15., -10., -80.)));
    return top;
}

Plane* cubeBottomFace()
{

    Plane *bottom = new Plane(rotate(glm::vec3(-15., -15., -75.)),
                           rotate(glm::vec3(-10., -15., -75.)),
                           rotate(glm::vec3(-10., -15., -80.)),
                           rotate(glm::vec3(-15., -15., -80.)));
    return bottom;
}

Plane* cubeLeftFace()
{
    Plane *left = new Plane(rotate(glm::vec3(-15., -15., -80.)),
                           rotate(glm::vec3(-15., -10., -80.)),
                           rotate(glm::vec3(-15., -10., -75.)),
                           rotate(glm::vec3(-15., -15., -75.)));
    return left;
}

Plane* cubeRightFace()
{
    Plane *right = new Plane(rotate(glm::vec3(-10., -15., -80.)),
                           rotate(glm::vec3(-10., -10., -80.)),
                           rotate(glm::vec3(-10., -10., -75.)),
                           rotate(glm::vec3(-10., -15., -75.)));
    return right;
}

Plane* cubeBackFace()
{
    Plane *back = new Plane(rotate(glm::vec3(-15., -15., -80.)),
                           rotate(glm::vec3(-10., -15., -80.)),
                           rotate(glm::vec3(-10., -10., -80.)),
                           rotate(glm::vec3(-15., -10., -80.)));
    return back;
}

Plane* cubeFrontFace()
{
    Plane *front = new Plane(rotate(glm::vec3(-10., -15., -75.)),
                           rotate(glm::vec3(-10., -10., -75.)),
                           rotate(glm::vec3(-15., -10., -75.)),
                           rotate(glm::vec3(-15., -15., -75.)));
    return front;
}


Plane* pyramidBottomFace()
{
    Plane *bottomP = new Plane(rotate(glm::vec3(-10., -10., -75.)),
                           rotate(glm::vec3(-10., -10., -80.)),
                           rotate(glm::vec3(-15., -10., -80.)),
                           rotate(glm::vec3(-15., -10., -75.)));
    return bottomP;
}

Plane* pyramidLeftFace()
{
    Plane *leftP = new Plane(rotate(glm::vec3(-15., -10., -80.)),
                           rotate(glm::vec3(-15., -10., -75.)),
                           rotate(glm::vec3(-12.5, -4., -77.5)));
    return leftP;
}

Plane* pyramidRightFace()
{
    Plane *rightP = new Plane(rotate(glm::vec3(-10., -10., -75.)),
                           rotate(glm::vec3(-10., -10., -80.)),
                           rotate(glm::vec3(-12.5, -4., -77.5)));
    return rightP;
}

Plane* pyramidBackFace()
{
    Plane *backP = new Plane(rotate(glm::vec3(-15., -10., -80.)),
                           rotate(glm::vec3(-10., -10., -80.)),
                           rotate(glm::vec3(-12.5, -4., -77.5)));
    return backP;
}

Plane* pyramidFrontFace()
{
    Plane *frontP = new Plane(rotate(glm::vec3(-10., -10., -75.)),
                           rotate(glm::vec3(-12.5, -4., -77.5)),
                           rotate(glm::vec3(-15., -10., -75.)));
    return frontP;
}


//---The most important function in a ray tracer! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
    glm::vec3 backgroundCol(0);						//Background colour = (0,0,0)
    glm::vec3 lightPos(17, 40, -5);					//Light's position
    glm::vec3 color(0);
	SceneObject* obj;

    ray.closestPt(sceneObjects);					//Compare the ray with all objects in the scene
    if(ray.index == -1) return backgroundCol;		//no intersection
	obj = sceneObjects[ray.index];					//object on which the closest point of intersection is found


    // ####### basketball SPHERE 3
    if(ray.index == 2){
        glm::vec3 normalVec = obj->normal(ray.hit);

        float patternX = asin(normalVec.x) / M_PI + 0.5;
        float patternY = asin(normalVec.y) / M_PI + 0.5;
        color += ball.getColorAt(patternX, patternY);
    }

    // ####### Ground floor PLANE
    if (ray.index == 4)
    {
        // Generating a checkered patten on a plane
        int stripeWidth = 6;
        int iX = (int)(((ray.hit.x) + 128) / stripeWidth) % 2;
        int iZ = (int)((ray.hit.z) / stripeWidth) % 2;
        int kX = iX % 2;
        int kZ = iZ % 2;
        if ((kX && kZ) || (!kX && !kZ)) color += glm::vec3(0.54, 0.17, 0.89);
        else color += glm::vec3(0.99, 0.76, 0);
        obj->setColor(color);
    }

    // ####### Background image PLANE
    if (ray.index == 5) {
        int x1 = -50;
        int x2 = 50;
        int y1 = -15;
        int y2 = 50;
        float s = (ray.hit.x - x1) / (x2 - x1);
        float t = (ray.hit.y - y1) / (y2 - y1);
        if (s > 0 && s < 1 && t > 0 && t < 1)
        {
            color = texture.getColorAt(s, t);
            obj->setColor(color);
        }
    }


    // ####### Object's LIGHTING
    if (ray.index != 2) {
        color = obj->lighting(lightPos, -ray.dir, ray.hit);
    }



    // ####### Object SHADOWS
    glm::vec3 lightVec = lightPos - ray.hit;   // given by light position - POI of the ray NOT the shadow ray
    Ray shadowRay(ray.hit, lightVec);          // creates a shadow ray at the POI with a DIRECTION given by lightVec, which is a vector from the POS to the light source!
    shadowRay.closestPt(sceneObjects);         // FINDS the POI of the shadow ray!!

    if (shadowRay.index > -1 && shadowRay.dist < glm::length(lightVec)) {
        color = 0.2f * obj->getColor();   // where 0.2 = AMBIENT scale factor!
    }


    // Lighter shadow
    if (shadowRay.index == 1 || shadowRay.index == 3) {
        color = 0.5f * obj->getColor();
    }


    // Reflective
    if (obj->isReflective() && step < MAX_STEPS)
    {
        glm::vec3 normalVec = obj->normal(ray.hit);
        glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVec);   // 1st param = the direction of incoming ray, 2nd param = the normal vector(always pointing up from the POI)!
        Ray reflectedRay(ray.hit, reflectedDir);
        glm::vec3 reflectedColor = trace(reflectedRay, step + 1);
        color = color + ( obj->getReflectionCoeff() * reflectedColor);
    }

    // Refractive
    if (obj->isRefractive() && step < MAX_STEPS)
    {
        float eta = 1/1.005;
        glm::vec3 n = obj->normal(ray.hit);
        glm::vec3 g = glm::refract(ray.dir, n, eta);
        Ray refractedRay(ray.hit, g); // Refracted ray inside the sphere
        refractedRay.closestPt(sceneObjects);

        glm::vec3 m = obj->normal(refractedRay.hit);
        glm::vec3 h = glm::refract(g, -m, 1.0f/eta);
        Ray refractedRayOut(refractedRay.hit, h); // Refracted ray and out of the sphere
        refractedRayOut.closestPt(sceneObjects);
        glm::vec3 refractedColor = trace(refractedRayOut, step + 1);
        color = color + (eta * refractedColor);
    }


    // Transparent
    if (obj->isTransparent() && step < MAX_STEPS)
    {
        Ray rayTHROUGH(ray.hit, ray.dir);
        rayTHROUGH.closestPt(sceneObjects);

        Ray rayOUTsphere(rayTHROUGH.hit, ray.dir);
        rayOUTsphere.closestPt(sceneObjects);

        glm::vec3 translatedColor = trace(rayOUTsphere, step + 1);
        color = color + (obj->getTransparencyCoeff() * translatedColor);
    }


    // Procedural Pattern Cylinder
    if (ray.index == 17)
    {
        if ((int(ray.hit.x - ray.hit.y)) % 2 == 0) {
            obj->setColor(glm::vec3(1, 0, 0));
        } else if ((int(ray.hit.x + ray.hit.y)) % 2 == 0) {
            obj->setColor(glm::vec3(0, 1, 0));
        } else {
            obj->setColor(glm::vec3(0, 1, 0));
        }
    }

    // Procedural Pattern Cone
    if (ray.index == 18 || ray.index == 11)
    {
        if ((int(ray.hit.x - ray.hit.y)) % 2 == 0) {
            obj->setColor(glm::vec3(1, 0, 0));
        } else if ((int(ray.hit.x + ray.hit.y)) % 2 == 0) {
            obj->setColor(glm::vec3(1, 0, 0));
        } else {
            obj->setColor(glm::vec3(0.99, 0.76, 0));
        }
    }


    // Fog - Make sure you set background colour to WHITE (start of trace function)!!
//    float t = (ray.hit.z - -80.0) / (-180.0 - -80.0);
//    color = ((1 - t) * color) + (t * glm::vec3(1.0, 1.0, 1.0));
    return color;
}

glm::vec3 anti_aliasing(float red, float green, float blue, float cellX, float cellY, float xp, float yp, glm::vec3 eye) {
    for (int j = 0; j < 2; j++) {
        for (int k = 0; k < 2; k++) {
            glm::vec3 dir((xp + (j * 0.5 + 0.25) * cellX), (yp+(k * 0.5 + 0.25)*cellY), -EDIST);

            Ray primaryRay = Ray(eye, dir);

            glm::vec3 colour = trace(primaryRay, 1); // Trace the primary ray and get the colour value
            red += colour.r;
            green += colour.g;
            blue += colour.b;
        }
    }
    return glm::vec3(red, green, blue);
}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
	float xp, yp;  //grid point
	float cellX = (XMAX-XMIN)/NUMDIV;  //cell width
	float cellY = (YMAX-YMIN)/NUMDIV;  //cell height
	glm::vec3 eye(0., 0., 0.);

	glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glBegin(GL_QUADS);  //Each cell is a tiny quad.

    for(int i = 0; i < NUMDIV; i++)	//Scan every cell of the image plane
    {
        xp = XMIN + i * cellX;
        for(int j = 0; j < NUMDIV; j++)
        {
            yp = YMIN + j*cellY;
            glm::vec3 col;

            // Anti-aliasing
            red = 0; green = 0; blue = 0;
            col = anti_aliasing(red, green, blue, cellX, cellY, xp, yp, eye);
            red = col.r/4; green = col.g/4; blue = col.b/4;
            glColor3f(red, green, blue);

            // Without Anti-aliasing
//            glm::vec3 dir(xp+0.5*cellX, yp+0.5*cellY, -EDIST);
//            Ray ray = Ray(eye, dir); // defines a primary ray, with direction dir
//            col = trace(ray, 1); // TRACE the primary ray and get the colour value
//            glColor3f(col.r, col.g, col.b);

            glVertex2f(xp, yp);				//Draw each cell with its color value
            glVertex2f(xp+cellX, yp);
            glVertex2f(xp+cellX, yp+cellY);
            glVertex2f(xp, yp+cellY);
        }
    }

    glEnd();
    glFlush();
}



//---This function initializes the scene ------------------------------------------- 
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);

    glClearColor(0, 0, 0, 1);

    ball = TextureBMP ((char*)"ball.bmp");

    // Spheres
    Sphere *sphere1 = new Sphere(glm::vec3(-7.0, 0, -100.0), 10);  // BIG BOY SPHERE
    Sphere *sphere2 = new Sphere(glm::vec3(10, -8, -77.5), 3);    // REFRACTED SPHERE
    Sphere *sphere3 = new Sphere(glm::vec3(-12.5, -1.5, -77.0), 2);      // TEXTURED SPHERE
    Sphere *sphere4 = new Sphere(glm::vec3(2.0, -6.0, -77.5), 3); // TRANSPARENT SPHERE
    sphere1->setColor(glm::vec3(0.15, 0.13, 0.30));   // Set colour to blue
    sphere2->setColor(glm::vec3(0, 0, 0));   // Set colour to white
    sphere3->setColor(glm::vec3(0, 0, 0));   // Set colour to white
    sphere4->setColor(glm::vec3(0, 0.1, 0));   // Set colour to white

    sphere1->setReflectivity(true, 0.8);  // 2nd param = coefficient of reflection pr
    sphere2->setReflectivity(true, 0.1);
    sphere2->setRefractivity(true);
    sphere4->setTransparency(true, 0.8);
    sphere4->setReflectivity(true, 0.1);
    sphere3->setSpecularity(true);


    // Floor & Background Planes
    Plane *floor = floorPlane();
    Plane *background = backgroundPlane();
    floor->setSpecularity(false);
    floor->setReflectivity(true, 0.2);
    background->setSpecularity(false);


    // Cube
    Plane *topC = cubeTopFace();
    Plane *bottomC = cubeBottomFace();
    Plane *leftC = cubeLeftFace();
    Plane *rightC = cubeRightFace();
    Plane *backC = cubeBackFace();
    Plane *frontC = cubeFrontFace();
    topC->setColor(glm::vec3(1, 0, 0));
    bottomC->setColor(glm::vec3(1, 0, 0));
    leftC->setColor(glm::vec3(0.99, 0.76, 0));
    rightC->setColor(glm::vec3(0.99, 0.76, 0));
    backC->setColor(glm::vec3(1, 0, 0));
    frontC->setColor(glm::vec3(1, 0, 0));


    // Pyramid
    Plane *bottomP = pyramidBottomFace();
    Plane *leftP = pyramidLeftFace();
    Plane *rightP = pyramidRightFace();
    Plane *backP = pyramidBackFace();
    Plane *frontP = pyramidFrontFace();
    bottomP->setColor(glm::vec3(0, 0.9, 0.6));
    leftP->setColor(glm::vec3(0.4, 1, 0.6));
    rightP->setColor(glm::vec3(0.4, 1, 0.6));
    backP->setColor(glm::vec3(0.4, 1, 0.6));
    frontP->setColor(glm::vec3(0.4, 1, 0.6));


    // Cylinder
    Cylinder *cylinder = new Cylinder(glm::vec3(2, -10, -75), 0.5, 1.0);
    cylinder->setColor(glm::vec3(0, 1, 0));


    // Cone
    Cone* cone = new Cone(glm::vec3(10, -15, -77.5), 1.0, 4.0);
    cone->setColor(glm::vec3(1, 0, 0));




    sceneObjects.push_back(sphere1);       // 0          Add spheres
    sceneObjects.push_back(sphere2);       // 1
    sceneObjects.push_back(sphere3);       // 2
    sceneObjects.push_back(sphere4);       // 3

    sceneObjects.push_back(floor);         // 4          Add floor and backgroun
    sceneObjects.push_back(background);    // 5

    sceneObjects.push_back(topC);          // 6          Add Cube Faces
    sceneObjects.push_back(bottomC);       // 7
    sceneObjects.push_back(leftC);         // 8
    sceneObjects.push_back(rightC);        // 9
    sceneObjects.push_back(backC);         // 10
    sceneObjects.push_back(frontC);        // 11

    sceneObjects.push_back(bottomP);       // 12         Add Pyramid Faces
    sceneObjects.push_back(leftP);         // 13
    sceneObjects.push_back(rightP);        // 14
    sceneObjects.push_back(backP);         // 15
    sceneObjects.push_back(frontP);        // 16

    sceneObjects.push_back(cylinder);      // 17         Add Cylinder

    sceneObjects.push_back(cone);          // 18         Add Cone
}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracing");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
