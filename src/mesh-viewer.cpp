//--------------------------------------------------
// Author:
// Date:
// Description: Loads PLY files in ASCII format
//--------------------------------------------------

#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"
#include "plymesh.h"
#include "osutils.h"

using namespace std;
using namespace glm;
using namespace agl;

class MeshViewer : public Window {
public:
   MeshViewer() : Window() {
   }

   void setup() {
       modelNames = GetFilenamesInDir("../models", "ply");
       mesh.load("..\\models\\" + modelNames[0]);
       vec3 mins = mesh.minBounds();
       vec3 maxs = mesh.maxBounds();
       float scale = glm::min(glm::min(10 / (maxs.x - mins.x), 10 / (maxs.y - mins.y)), 10 / (maxs.z - mins.z));
       scalar = vec3(scale, scale, scale);
       translation = vec3(-(mins.x+(maxs.x-mins.x)/2), -(mins.y+(maxs.y-mins.y)/2), -(mins.z+(maxs.z-mins.z)/2));
       rotation = vec3(0, 0, 0);
   }

   void repositionCamera(GLfloat r, GLfloat az, GLfloat el) {
       eyePos.x = r * cos(az) * cos(el);
       eyePos.y = r * sin(el);
       eyePos.z = r * sin(az) * cos(el);
   }

   void mouseMotion(int x, int y, int dx, int dy) {
       if (leftMouseDown && leftShiftDown) {
           if (dx > 0 && Radius <= 1) {
               return;
           }
           else if (dx < 0 && Radius >= 10) {
               return;
           }
           else if (dx < 0) {
               Radius += 0.1;
           }
           else if (dx > 0) {
               Radius -= 0.1;
           }
           repositionCamera(Radius, Azimuth, Elevation);
       }
       else if (leftMouseDown) {
           Azimuth += dx * M_PI / 180;
           if (Elevation + dy * M_PI / 180 >= -M_PI / 2 && Elevation + dy * M_PI / 180 <= M_PI / 2) {
               Elevation += dy * M_PI / 180;
           }
           repositionCamera(Radius, Azimuth, Elevation);
       }
   }

   void mouseDown(int button, int mods) {
       if (button == GLFW_MOUSE_BUTTON_LEFT) {
           leftMouseDown = true;
       }
   }

   void mouseUp(int button, int mods) {
       if (button == GLFW_MOUSE_BUTTON_LEFT) {
           leftMouseDown = false;
       }
   }

   void scroll(float dx, float dy) {
       if (dy > 0 && Radius == 1) {
           return;
       }
       else if (dy < 0 && Radius == 10) {
           return;
       }
       else if (dy < 0) {
           Radius++;
       }
       else if (dy > 0) {
           Radius--;
       }
       repositionCamera(Radius, Azimuth, Elevation);
   }

   void keyUp(int key, int mods) {
       if (key == GLFW_KEY_LEFT_SHIFT) {
           leftShiftDown = false;
       }
   }

   void keyDown(int key, int mods) {
       if (key == GLFW_KEY_N) {
           mesh.clear();
           mesh = PLYMesh();
           if (currFile == modelNames.size()-1) {
               currFile = 0;
           }
           else {
               currFile--;
           }
           mesh.load("..\\models\\" + modelNames[currFile]);
           vec3 mins = mesh.minBounds();
           vec3 maxs = mesh.maxBounds();
           float scale = glm::min(glm::min(10 / (maxs.x - mins.x), 10 / (maxs.y - mins.y)), 10 / (maxs.z - mins.z));
           scalar = vec3(scale, scale, scale);
           translation = vec3(-(mins.x + (maxs.x - mins.x) / 2), -(mins.y + (maxs.y - mins.y) / 2), -(mins.z + (maxs.z - mins.z) / 2));
           rotation = vec3(0, 0, 0);
           Radius = 10;
           Azimuth = 0;
           Elevation = 0;
       }
       else if (key == GLFW_KEY_P) {
           mesh.clear();
           mesh = PLYMesh();
           if (currFile == 0) {
               currFile = modelNames.size() - 1;
           }
           else {
               currFile--;
           }
           mesh.load("..\\models\\" + modelNames[currFile]);
           vec3 mins = mesh.minBounds();
           vec3 maxs = mesh.maxBounds();
           float scale = glm::min(glm::min(10 / (maxs.x - mins.x), 10 / (maxs.y - mins.y)), 10 / (maxs.z - mins.z));
           scalar = vec3(scale, scale, scale);
           translation = vec3(-(mins.x + (maxs.x - mins.x) / 2), -(mins.y + (maxs.y - mins.y) / 2), -(mins.z + (maxs.z - mins.z) / 2));
           rotation = vec3(0, 0, 0);
           eyePos = vec3(10, 0, 0);
           Radius = 10;
           Azimuth = 0;
           Elevation = 0;
       }
       else if (key == GLFW_KEY_LEFT_SHIFT) {
           leftShiftDown = true;
       }
   }

   void draw() {
      float aspect = ((float)width()) / height();
      renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
      renderer.lookAt(eyePos, lookPos, up);
      renderer.rotate(rotation);
      renderer.scale(scalar);
      renderer.translate(translation);
      renderer.mesh(mesh);
   }

protected:
   PLYMesh mesh;
   vec3 eyePos = vec3(10, 0, 0);
   vec3 lookPos = vec3(0, 0, 0);
   vec3 up = vec3(0, 1, 0);
   vec3 rotation;
   vec3 translation;
   vec3 scalar;
   std::vector<string> modelNames;
   int currFile = 0;
   GLfloat Radius = 10;
   GLfloat Azimuth = 0;
   GLfloat Elevation = 0;
   bool leftMouseDown = false;
   bool leftShiftDown = false;
};

int main(int argc, char** argv)
{
   MeshViewer viewer;
   viewer.run();
   return 0;
}

