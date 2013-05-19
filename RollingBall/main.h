//
//  Copyright (c) 2013 NickiStudio. All rights reserved.
//
//  CS6533 Rolling Ball Project
//

#ifndef __ROLLING_BALL_HEADER__
#define __ROLLING_BALL_HEADER__

#define WINDOWS 0
#define MAC_OSX 1

#define SPHERE_PATH          (char*) "sphere.1024"
#define VERTEX_SHADER_PATH   (char*) "Shaders/vFirework.glsl"
#define FRAGMENT_SHADER_PATH (char*) "Shaders/fFirework.glsl"

#pragma mark - Main
int  main(int argc, char **argv);
void idle(void);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void display(void);
void reshape(int w, int h);
void menu(int index);

#pragma mark - Initializations
void init();
void initInFile();
void initMenu();
void initTexture(void);
void initFireworks();
void initSpotlight();
void initLighting();

#pragma mark - Drawings
void drawGround();
void drawAxes();
void drawSphere(int mode);
void drawFireworks();
void drawShadow();
void drawFog();
void drawSpotlight();

#pragma mark - Models
int     getNextTrackIdOf(int trackId);
bool    isGoingToNextTrack();
float   getSphereRadius();
float   getFovy();
float** getSphereNormals();

#pragma mark - Utilities
float  calDistance(float* p1, float* p2);
float  calVectorLength(float x, float y, float z);
float* calDirection(float* from, float* to);
float* calCrossProduct(float* u, float* v);
float* calNormalizedVector(float* v);

#pragma mark - Shader Initializator
static char* readShaderSource(const char* shaderFile);
static unsigned int initShader(const char* vShaderFile, const char* fShaderFile);

#pragma mark - Texture Mapper
void initImages();

#pragma mark - FPS fix tool
void fixFps();

#endif
