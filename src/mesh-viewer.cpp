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
       renderer.loadShader("normals", "../shaders/normals.vs", "../shaders/normals.fs");
       renderer.loadShader("phong-vertex", "../shaders/phong-vertex.vs", "../shaders/phong-vertex.fs");
       renderer.loadShader("phong-pixel", "../shaders/phong-pixel.vs", "../shaders/phong-pixel.fs");
       renderer.loadShader("toon", "../shaders/toon.vs", "../shaders/toon.fs");
       shaders.push_back("normals");
       shaders.push_back("phong-vertex");
       shaders.push_back("phong-pixel");
       shaders.push_back("toon");
       
       modelNames = GetFilenamesInDir("../models", "ply");
       mesh.load("../models/" + modelNames[0]);

       textureNames = GetFilenamesInDir("../textures", "png");
       renderer.loadTexture(textureNames[0], "../textures/" + textureNames[0], 0);

       renderer.setUniform("material.ambient", vec3(1.0, 1.0, 1.0));
       renderer.setUniform("material.diffuse", vec3(1.0f, 1.0f, 1.0f));
       renderer.setUniform("material.specular", vec3(1.0f, 1.0f, 1.0f));

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
           if (currModel == modelNames.size()-1) {
               currModel = 0;
           }
           else {
               currModel++;
           }
           mesh.load("../models/" + modelNames[currModel]);
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
           if (currModel == 0) {
               currModel = modelNames.size() - 1;
           }
           else {
               currModel--;
           }
           mesh.load("../models/" + modelNames[currModel]);
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
       else if (key == GLFW_KEY_S) {
           if (currShader == shaders.size() - 1) {
               currShader = 0;
           }
           else {
               currShader++;
           }
       }
       else if (key == GLFW_KEY_T) {
           doTexture = !doTexture;
       }
       else if (key == GLFW_KEY_Y) {
           if (doTexture) {
               if (currTexture == textureNames.size() - 1) {
                   currTexture = 0;
               }
               else {
                   currTexture++;
               }
               renderer.loadTexture(textureNames[currTexture], "../textures/" + textureNames[currTexture], currTexture);
           }
       }
   }

   void draw() {
      renderer.beginShader(shaders[currShader]); // activates shader with given name
      
      if (doTexture) {
          renderer.texture("diffuseTexture", textureNames[currTexture]);
      }

      renderer.setUniform("eyePos", eyePos);
      renderer.setUniform("lightPos", lightPos);
      renderer.setUniform("HasUV", doTexture);

      float aspect = ((float)width()) / height();
      renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
      renderer.lookAt(eyePos, lookPos, up);
      
      renderer.rotate(rotation);
      renderer.scale(scalar);
      
      renderer.translate(translation);
      
      renderer.mesh(mesh);
      
      renderer.endShader();
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
   int currModel = 0;
   std::vector<string> textureNames;
   int currTexture = 0;
   bool doTexture = false;
   GLfloat Radius = 10;
   GLfloat Azimuth = 0;
   GLfloat Elevation = 0;
   bool leftMouseDown = false;
   bool leftShiftDown = false;
   std::vector<string> shaders;
   int currShader = 0;
   vec3 lightPos = vec3(10.0f, 10.0f, 10.0f);
};

int main(int argc, char** argv)
{
   MeshViewer viewer;
   viewer.run();
   return 0;
}

