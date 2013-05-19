//
//  Copyright (c) 2013 NickiStudio. All rights reserved.
//
//  CS6533 Rolling Ball Project
//
//  Todo: Release variables in each function. -
//

#include "main.h"

#pragma mark Common Dependences
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#pragma mark Dependences for Mac
#if MAC_OSX
#include "glew.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <unistd.h>
#endif

#pragma mark Dependences for Windows
#if WINDOWS
#define _USE_MATH_DEFINES
#include <cmath>
#include <windows.h>
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#pragma mark - Assignment 2 Settings
GLfloat mSphereRotateAngle = 5.0;
GLfloat mVRP[] = { 7,  3, -10, 1};
GLfloat mVPN[] = {-7, -3,  10, 0};
GLfloat mVUP[] = { 0,  1,   0, 0};
GLfloat mTrack[][4] = {{3, 1, 5, 1}, {-1, 1, -4, 1}, {3.5, 1, -2.5, 1}};
GLfloat mGroundVertices[][4] = {{5, 0, 8, 1}, {5, 0, -4, 1}, {-5, 0, -4, 1}, {-5, 0, 8, 1}};
GLfloat mBackgroundColor[] = {0.529, 0.807, 0.920, 0.000};

#pragma mark Assignment 3 Settings - Colors and position for shading effect
GLfloat mSpotlightPosition[]   = {-14.0,  12.0, -3.0, 1.0};
GLfloat mSpotlightDirection[]  = {  8.0, -12.0, -1.5, 0.0}; // from light position to (−6.0, 0.0, −4.5, 1.0)
GLfloat mGlobalAmbientColor[]  = {1, 1, 1, 1};
GLfloat mLightAmbientColor[]   = {0, 0, 0, 1};
GLfloat mLightDiffuseColor[]   = {1, 1, 1, 1};
GLfloat mLightSpecularColor[]  = {1, 1, 1, 1};
GLfloat mGroundColor[]         = {0, 1, 0, 1};
GLfloat mSphereColor[]         = {1.00, 0.84, 0.00, 1.00};
GLfloat mShadowColor[]         = {0.25, 0.25, 0.25, 0.65};
GLfloat mMaterialShiness       = 125.0;
GLfloat mSpotlightExponent     = 5.0;
GLfloat mSpotlightCutOffAngle  = 20.0;
GLfloat mPointlightCutOffAngle = 180.0;
GLfloat mSpotlightConstant     = 2.0;
GLfloat mSpotlightLinearAtt    = 0.01;
GLfloat mSpotlightQuadraticAtt = 0.001;
GLfloat mShadowMatrix[16] = {12, 0, 0, 0, 14, 0, 3, -1, 0, 0, 12, 0, 0, 0, 0, 12};

#pragma mark Assignment 4 Settings
GLfloat mFogColor[]              = {0.7, 0.7, 0.7, 0.5};
GLfloat mParticleStartPosition[] = {0.0, 0.1, 0.0, 0.0};
GLfloat mVerticalPlaneValue[]    = {2.5, 0.0, 0.0, 0.0};
GLfloat mSlantedPlaneValue[]     = {1.0, 1.0, 1.0, 0.0};
GLfloat mParticleSize            = 3.0;
GLfloat mFogStartValue           = 0.0;
GLfloat mFogEndValue             = 18.0;
GLfloat mFogDensity              = 0.09;
GLint   mParticleCount           = 300;

#pragma mark Variables
int mCurrentTrackId;
int mTrackCount;
int mNumberOfTrianglesInSphere;
int mCurrentSphereTextureGenerationMode = 0;
int mCurrentSphereSpaceMode = 0;
int mFpsLastTime;

GLfloat*  mViewingPoint;
GLfloat** mSphereData;
GLfloat   mSphereRadius;
GLint     mFireworkStartTime;
GLuint    mShaderProgram;
GLuint    mGroundTexture;
GLuint    mSphereTexture;
GLfloat*  mCurrentCenterPosition;
GLfloat** mTrackDirectionVectors;
GLfloat** mSphereRotationAxes;
GLfloat** mSphereNormals;
GLfloat*  mTrackLength;
GLfloat** mParticleColors;
GLfloat** mParticleVelocities;

#pragma mark Control Variables
bool mIsStarted = false;
bool mIsRolling = false;
bool mIsShadowEnabled = true;
bool mIsWireframeEnabled = false;
bool mIsLightingEnabled = true;
bool mIsSpotlightEnabled = false;
bool mIsBlendingShadowEnabled = true;
bool mIsGroundTextureEnabled = true;
bool mIsSphereTextureEnabled = true;
bool mIsFireworkEnabled = true;

int mCurrentFogMode = 0;
int mCurrentShaderMode = 1;

GLint     mCurrentTextureGenerationMode = GL_OBJECT_LINEAR;
GLfloat*  mCurrentTexturePlaneValue = mVerticalPlaneValue;
GLenum    mCurrentTexturePlaneMode = GL_OBJECT_PLANE;
GLfloat   mAcculmatedSphereMatrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

#pragma mark Enums

enum TrackIdentifier {
    TRACK_0, TRACK_1, TRACK_2
};

enum SphereTextureGenerationMode {
    GEN_VERTICAL, GEN_SLANTED
};

enum SphereTextureSpaceMode {
    SPACE_EYE, SPACE_OBJECT
};

enum ShaderMode {
    SHADER_FLAT, SHADER_SMOOTH
};

enum FogMode {
    FOG_DISABLE, FOG_LINEAR, FOG_EXPONENTIAL, FOG_EXPONENTIAL_SQUARE
};

enum DrawMode {
    DRAW_NORMAL, DRAW_SHADOW
};

enum Position {
    X, Y, Z, W
};

enum Color {
    R, G, B, A
};

enum MenuEventCode {
    MENU_DEFAULT_VIEW_POINT,
    MENU_WIRE_FRAME,
    MENU_QUIT,
    MENU_SHADOW_ENABLE,
    MENU_SHADOW_DISABLE,
    MENU_SHADER_FLAT,
    MENU_SHADER_SMOOTH,
    MENU_LIGHTING_ENABLE,
    MENU_LIGHTING_DISABLE,
    MENU_SPOTLIGHT_ENABLE,
    MENU_SPOTLIGHT_DISABLE,
    MENU_LIGHTING_SPOTLIGHT,
    MENU_LIGHTING_POINTLIGHT,
    MENU_FOG_DISABLE,
    MENU_FOG_LINEAR,
    MENU_FOG_EXPONENTIAL,
    MENU_FOG_EXPONENTIAL_SQUARE,
    MENU_BLENDING_SHADOW_DISABLE,
    MENU_BLENDING_SHADOW_ENABLE,
    MENU_GROUND_TEXTURE_DISABLE,
    MENU_GROUND_TEXTURE_ENABLE,
    MENU_SPHERE_TEXTURE_ENABLE,
    MENU_SPHERE_TEXTURE_DISABLE,
    MENU_FIREWORK_ENABLE,
    MENU_FIREWORK_DISABLE
};

#pragma mark - Image Settings

#define	STRIPE_IMAGE_WIDTH  32
#define GROUND_IMAGE_WIDTH  64
#define GROUND_IMAGE_HEIGHT 64
GLubyte mStripeImage[4 * STRIPE_IMAGE_WIDTH];
GLubyte mGroundImage[GROUND_IMAGE_HEIGHT][GROUND_IMAGE_WIDTH][4];

#pragma mark - Main

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(30, 30);
    glutCreateWindow("Assignment 4");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    init();
    glutMainLoop();
}

void idle(void)
{
    // check if going to next track
    if (isGoingToNextTrack()) mCurrentTrackId = getNextTrackIdOf(mCurrentTrackId);
    
    // move 
    float offset = (mSphereRadius * mSphereRotateAngle * M_PI) / 180;
    mCurrentCenterPosition[X] = mCurrentCenterPosition[X] + mTrackDirectionVectors[mCurrentTrackId][X] * offset;
    mCurrentCenterPosition[Y] = mCurrentCenterPosition[Y] + mTrackDirectionVectors[mCurrentTrackId][Y] * offset;
    mCurrentCenterPosition[Z] = mCurrentCenterPosition[Z] + mTrackDirectionVectors[mCurrentTrackId][Z] * offset;
    
    // compute accumulated rotation matrix
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glRotatef(mSphereRotateAngle, mSphereRotationAxes[mCurrentTrackId][X], mSphereRotationAxes[mCurrentTrackId][Y], mSphereRotationAxes[mCurrentTrackId][Z]);
    glMultMatrixf(mAcculmatedSphereMatrix);
    glGetFloatv(GL_MODELVIEW_MATRIX, mAcculmatedSphereMatrix);
    glPopMatrix();
    
    // flush screen
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 'b' || key == 'B') {
        if (!mIsStarted) {
            mIsStarted = true;
            mFireworkStartTime = glutGet(GLUT_ELAPSED_TIME);
            glutIdleFunc(idle);
        }
    }
    if (key == 'v' || key == 'V') {
        mCurrentTexturePlaneValue = mVerticalPlaneValue;
        mCurrentSphereTextureGenerationMode = GEN_VERTICAL;
    }
    if (key == 's' || key == 'S') {
        mCurrentTexturePlaneValue = mSlantedPlaneValue;
        mCurrentSphereTextureGenerationMode = GEN_SLANTED;
    }
    if (key == 'o' || key == 'O') {
        mCurrentTextureGenerationMode = GL_OBJECT_LINEAR;
        mCurrentTexturePlaneMode = GL_OBJECT_PLANE;
        mCurrentSphereSpaceMode = SPACE_OBJECT;
    }
    if (key == 'e' || key == 'E') {
        mCurrentTextureGenerationMode = GL_EYE_LINEAR;
        mCurrentTexturePlaneMode = GL_EYE_PLANE;
        mCurrentSphereSpaceMode = SPACE_EYE;
    }
    if (key == 'x') mViewingPoint[X] -= 1.0;
    if (key == 'X') mViewingPoint[X] += 1.0;
    if (key == 'y') mViewingPoint[Y] -= 1.0;
    if (key == 'Y') mViewingPoint[Y] += 1.0;
    if (key == 'z') mViewingPoint[Z] -= 1.0;
    if (key == 'Z') mViewingPoint[Z] += 1.0;
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP && mIsStarted) {
        mIsRolling = !mIsRolling;
        if (mIsRolling) glutIdleFunc(idle);
        else glutIdleFunc(NULL);
    }
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(mViewingPoint[X], mViewingPoint[Y], mViewingPoint[Z], 0, 0, 0, 0, 1, 0);
    drawSpotlight();
    drawFog();
    drawSphere(DRAW_NORMAL);
    drawAxes();
    drawFireworks();
    drawGround();
    drawSphere(DRAW_SHADOW);
    fixFps();
    glFlush();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    int size = (w > h) ? h : w;
    double fovy = getFovy();
    glViewport((w - size) / 2, (h - size) / 2, size, size);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, w / h, 1, 100);
}

void menu(int index)
{
    switch ((MenuEventCode)index) {
        case MENU_DEFAULT_VIEW_POINT:
            mViewingPoint[X] = mVRP[X];
            mViewingPoint[Y] = mVRP[Y];
            mViewingPoint[Z] = mVRP[Z];
            break;
        case MENU_WIRE_FRAME:
            mIsWireframeEnabled = !mIsWireframeEnabled;
            break;
        case MENU_QUIT:
            exit(1);
            break;
        case MENU_SHADOW_ENABLE:
            mIsShadowEnabled = true;
            break;
        case MENU_SHADOW_DISABLE:
            mIsShadowEnabled = false;
            break;
        case MENU_SHADER_FLAT:
            mCurrentShaderMode = SHADER_FLAT;
            break;
        case MENU_SHADER_SMOOTH:
            mCurrentShaderMode = SHADER_SMOOTH;
            break;
        case MENU_LIGHTING_ENABLE:
            mIsLightingEnabled = true;
            break;
        case MENU_LIGHTING_DISABLE:
            mIsLightingEnabled = false;
            break;
        case MENU_SPOTLIGHT_ENABLE:
            mIsSpotlightEnabled = true;
            break;
        case MENU_SPOTLIGHT_DISABLE:
            mIsSpotlightEnabled = false;
            break;
        case MENU_LIGHTING_SPOTLIGHT:
            mIsSpotlightEnabled = true;
            break;
        case MENU_LIGHTING_POINTLIGHT:
            mIsSpotlightEnabled = false;
            break;
        case MENU_FOG_DISABLE:
            mCurrentFogMode = FOG_DISABLE;
            break;
        case MENU_FOG_LINEAR:
            mCurrentFogMode = FOG_LINEAR;
            break;
        case MENU_FOG_EXPONENTIAL:
            mCurrentFogMode = FOG_EXPONENTIAL;
            break;
        case MENU_FOG_EXPONENTIAL_SQUARE:
            mCurrentFogMode = FOG_EXPONENTIAL_SQUARE;
            break;
        case MENU_BLENDING_SHADOW_DISABLE:
            mIsBlendingShadowEnabled = false;
            break;
        case MENU_BLENDING_SHADOW_ENABLE:
            mIsBlendingShadowEnabled = true;
            break;
        case MENU_GROUND_TEXTURE_DISABLE:
            mIsGroundTextureEnabled = false;
            break;
        case MENU_GROUND_TEXTURE_ENABLE:
            mIsGroundTextureEnabled = true;
            break;
        case MENU_SPHERE_TEXTURE_ENABLE:
            mIsSphereTextureEnabled = true;
            break;
        case MENU_SPHERE_TEXTURE_DISABLE:
            mIsSphereTextureEnabled = false;
            break;
        case MENU_FIREWORK_ENABLE:
            mIsFireworkEnabled = true;
            mFireworkStartTime = glutGet(GLUT_ELAPSED_TIME);
            break;
        case MENU_FIREWORK_DISABLE:
            mIsFireworkEnabled = false;
            break;
        default:
            printf("Error at line %d!", __LINE__);
            break;
    }
    display();
}

#pragma mark - Initializations

void init()
{
    initInFile();
    initMenu();
    initFireworks();
    initLighting();
    initSpotlight();
    initTexture();

    mViewingPoint = new GLfloat[4];
    mViewingPoint[X] = mVRP[X];
    mViewingPoint[Y] = mVRP[Y];
    mViewingPoint[Z] = mVRP[Z];
    
    mSphereRadius = getSphereRadius();
    mSphereNormals = getSphereNormals();
    mTrackCount = sizeof(mTrack) / sizeof(mTrack[0]);
    mCurrentTrackId = TRACK_0;
    
    mCurrentCenterPosition = new GLfloat[4];
    mCurrentCenterPosition[X] = mTrack[mCurrentTrackId][X];
    mCurrentCenterPosition[Y] = mTrack[mCurrentTrackId][Y];
    mCurrentCenterPosition[Z] = mTrack[mCurrentTrackId][Z];
    
    GLfloat yAxis[] = {0, 1, 0};
    mTrackLength = new GLfloat[mTrackCount];
    mSphereRotationAxes = new GLfloat*[mTrackCount];
    mTrackDirectionVectors = new GLfloat*[mTrackCount];
    for (int i = 0; i < mTrackCount; i++) {
        int nextId = getNextTrackIdOf(i);
        mTrackLength[i] = calDistance(mTrack[i], mTrack[nextId]);
        mTrackDirectionVectors[i] = calDirection(mTrack[i], mTrack[nextId]);
        mSphereRotationAxes[i] = calCrossProduct(yAxis, mTrackDirectionVectors[i]);
    }
    
    glClearColor(mBackgroundColor[R], mBackgroundColor[G], mBackgroundColor[B], mBackgroundColor[A]);
    glEnable(GL_DEPTH_TEST);
    
}

void initInFile()
{
    char *filePath = SPHERE_PATH;

    // try to open inputted file path
    std::ifstream file(filePath);
    if (file.fail()) {
        printf("Error! cannot open file %s\n\n", filePath);
        std::getchar();
        exit(0);
    }
    
    // total number of triangles
    file >> mNumberOfTrianglesInSphere;
    printf("Total number of triangles: %d\n", mNumberOfTrianglesInSphere);
    
    // read in and store point data
    mSphereData = new GLfloat *[mNumberOfTrianglesInSphere];
    for (int i = 0; i < mNumberOfTrianglesInSphere; i++) {
        int numOfPoints = 0;
        file >> numOfPoints;
        mSphereData[i] = new GLfloat[3 * numOfPoints];
        for (int j = 0; j < 3 * numOfPoints; j++) {
            file >> mSphereData[i][j];
        }
    }
    file.close();
}

void initMenu()
{
	int shadow = glutCreateMenu(menu);
    glutAddMenuEntry("Yes", MENU_SHADOW_ENABLE);
    glutAddMenuEntry("No", MENU_SHADOW_DISABLE);
    
	int shading = glutCreateMenu(menu);
    glutAddMenuEntry("Flat", MENU_SHADER_FLAT);
    glutAddMenuEntry("Smooth", MENU_SHADER_SMOOTH);
    
	int lighting = glutCreateMenu(menu);
    glutAddMenuEntry("Yes", MENU_LIGHTING_ENABLE);
    glutAddMenuEntry("No", MENU_LIGHTING_DISABLE);
    
	int lights = glutCreateMenu(menu);
    glutAddMenuEntry("Spot Light", MENU_LIGHTING_SPOTLIGHT);
    glutAddMenuEntry("Point Light", MENU_LIGHTING_POINTLIGHT);
    
	int fog = glutCreateMenu(menu);
    glutAddMenuEntry("No Fog", MENU_FOG_DISABLE);
    glutAddMenuEntry("Linear", MENU_FOG_LINEAR);
    glutAddMenuEntry("Exponential", MENU_FOG_EXPONENTIAL);
    glutAddMenuEntry("Exponential Square", MENU_FOG_EXPONENTIAL_SQUARE);
    
	int blendingShadow = glutCreateMenu(menu);
    glutAddMenuEntry("Yes", MENU_BLENDING_SHADOW_ENABLE);
    glutAddMenuEntry("No", MENU_BLENDING_SHADOW_DISABLE);
    
	int groundTexture = glutCreateMenu(menu);
    glutAddMenuEntry("Yes", MENU_GROUND_TEXTURE_ENABLE);
    glutAddMenuEntry("No", MENU_GROUND_TEXTURE_DISABLE);
	
	int sphereTexture = glutCreateMenu(menu);
    glutAddMenuEntry("Yes", MENU_SPHERE_TEXTURE_ENABLE);
    glutAddMenuEntry("No", MENU_SPHERE_TEXTURE_DISABLE);
    
	int firework = glutCreateMenu(menu);
    glutAddMenuEntry("Yes", MENU_FIREWORK_ENABLE);
    glutAddMenuEntry("No", MENU_FIREWORK_DISABLE);
    
    glutCreateMenu(menu);
    glutAddMenuEntry("Default View Point", MENU_DEFAULT_VIEW_POINT);
    glutAddMenuEntry("Wire Frame", MENU_WIRE_FRAME);
	glutAddSubMenu("Enable Lighting", lighting);
	glutAddSubMenu("Fog Options", fog);
	glutAddSubMenu("Texture Mapping Ground", groundTexture);
	glutAddSubMenu("Texture Mapped Sphere", sphereTexture);
	glutAddSubMenu("Shadow",shadow);
	glutAddSubMenu("Blending Shadow", blendingShadow);
	glutAddSubMenu("Shading", shading);
	glutAddSubMenu("Lighting", lights);
	glutAddSubMenu("Firework", firework);
    glutAddMenuEntry("Quit", MENU_QUIT);
    glutAttachMenu(GLUT_LEFT_BUTTON);
}

void initTexture(void)
{
    initImages();
    
    // Ground
    glGenTextures(1, &mGroundTexture);
    glBindTexture(GL_TEXTURE_2D, mGroundTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GROUND_IMAGE_WIDTH, GROUND_IMAGE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, mGroundImage);
    
    // Sphere
    glGenTextures(1, &mSphereTexture);
    glBindTexture(GL_TEXTURE_1D, mSphereTexture);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, STRIPE_IMAGE_WIDTH, 0, GL_RGBA, GL_UNSIGNED_BYTE, mStripeImage);
}

void initFireworks()
{
    mFireworkStartTime = 0;
    mParticleColors = new GLfloat*[mParticleCount];
    mParticleVelocities = new GLfloat*[mParticleCount];
    
    // init particles
    for (int i = 0; i < mParticleCount; i++) {
        mParticleColors[i] = new GLfloat[3];
        mParticleColors[i][R] = (rand() % 256) / 256.0;
        mParticleColors[i][G] = (rand() % 256) / 256.0;
        mParticleColors[i][B] = (rand() % 256) / 256.0;
        mParticleVelocities[i] = new GLfloat[3];
        mParticleVelocities[i][X] = 2.0 * ((rand() % 256) / 256.0 - 0.5);
        mParticleVelocities[i][Y] = 1.2 * 2.0 * ((rand() % 256) / 256.0);
        mParticleVelocities[i][Z] = 2.0 * ((rand() % 256) / 256.0 - 0.5);
    }
    // use glew to speed up the drawings
	int status = glewInit();
	if (status != GLEW_OK) {
		printf("Error: glewInit failed: %s\n", (char*) glewGetErrorString(status));
        std::getchar();
		exit(1);
	}
    // init shader
    mShaderProgram = initShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
}

void initSpotlight()
{
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_POSITION, mSpotlightPosition);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, mSpotlightDirection);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, mLightDiffuseColor);
    glLightfv(GL_LIGHT1, GL_SPECULAR, mLightSpecularColor);
    glLightfv(GL_LIGHT1, GL_AMBIENT, mLightAmbientColor);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, mSpotlightExponent);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, mSpotlightConstant);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, mSpotlightLinearAtt);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, mSpotlightQuadraticAtt);
}

void initLighting()
{
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, mLightAmbientColor);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, mLightDiffuseColor);
    glLightfv(GL_LIGHT0, GL_SPECULAR, mLightSpecularColor);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, mGlobalAmbientColor);
}

#pragma mark - Drawings

void drawSpotlight()
{
    if (mIsSpotlightEnabled) glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, mSpotlightCutOffAngle);
    else glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, mPointlightCutOffAngle);
}

void drawGround()
{
    // set up
    glPushMatrix();
    glDepthMask(GL_FALSE);
    
    if (mIsLightingEnabled) {
        glEnable(GL_LIGHTING);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mGroundColor);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mLightAmbientColor);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mLightSpecularColor);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mMaterialShiness);
    }
    if (mIsGroundTextureEnabled) {
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, mGroundTexture);
    }
    glColor3fv(mGroundColor);
    glBegin(GL_POLYGON);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0);      glVertex3fv(mGroundVertices[0]);
    glTexCoord2f(0, 1.25);   glVertex3fv(mGroundVertices[1]);
    glTexCoord2f(1.5, 1.25); glVertex3fv(mGroundVertices[2]);
    glTexCoord2f(1.5, 0);    glVertex3fv(mGroundVertices[3]);
    glEnd();
    
    // clean
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mLightDiffuseColor);
	glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDepthMask(GL_TRUE);
    glPopMatrix();
}

void drawAxes()
{
    glDisable(GL_LIGHTING); // setup
    glColor3f(1.0, 0.0, 0.0); // x axis in red
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(100, 0, 0);
    glEnd();
    glColor3f(1.0, 0.0, 1.0); // y axis in magenta
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 100, 0);
    glEnd();
    glColor3f(0.0, 0.0, 1.0); // z axis in blue
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 100);
    glEnd();
    glEnable(GL_LIGHTING); // clean up
}

void drawSphere(int mode)
{
    glPushMatrix();
    glDisable(GL_LIGHTING);
    
    if (mode == DRAW_NORMAL) {
        glColor3fv(mSphereColor);
    }
    else if (mode == DRAW_SHADOW) {
        if (mViewingPoint[Y] < 0 || !mIsShadowEnabled) {
            glPopMatrix();
            return;
        }
        glColor4fv(mShadowColor);
        glMultMatrixf(mShadowMatrix);
        glDisable(GL_LIGHTING);
        glDepthMask(GL_FALSE);
        if (mIsBlendingShadowEnabled) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

    }
    else {
        printf("Sphere drawing error at line %d", __LINE__);
        glPopMatrix();
        return;
    }
    glTranslatef(mCurrentCenterPosition[X], mCurrentCenterPosition[Y], mCurrentCenterPosition[Z]);
    glMultMatrixf(mAcculmatedSphereMatrix);
    glLineWidth(1);
    
    if (mIsLightingEnabled && mode != DRAW_SHADOW) {
        glEnable(GL_LIGHTING);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mSphereColor);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mSphereColor);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mLightAmbientColor);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mMaterialShiness);
    }
    
    for (int i = 0; i < mNumberOfTrianglesInSphere; i++) {
        if (mIsWireframeEnabled) {
            glDisable(GL_LIGHTING);
            glBegin(GL_LINE_LOOP);
        }
        else {
            if (mIsSphereTextureEnabled && mode != DRAW_SHADOW) {
                glPushMatrix();
                glLoadIdentity();
                glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, mCurrentTextureGenerationMode);
                glTexGenfv(GL_S, mCurrentTexturePlaneMode, mCurrentTexturePlaneValue);
                glEnable(GL_TEXTURE_1D);
                glEnable(GL_TEXTURE_GEN_S);
                glPopMatrix();
            }
            glBegin(GL_TRIANGLES);
        }
        if (mCurrentShaderMode == SHADER_FLAT || mode == DRAW_SHADOW) {
            glNormal3fv(mSphereNormals[i]);
            glVertex3f(mSphereData[i][0], mSphereData[i][1], mSphereData[i][2]);
            glVertex3f(mSphereData[i][3], mSphereData[i][4], mSphereData[i][5]);
            glVertex3f(mSphereData[i][6], mSphereData[i][7], mSphereData[i][8]);
        }
        else if (mCurrentShaderMode == SHADER_SMOOTH){
            glNormal3f(mSphereData[i][0], mSphereData[i][1], mSphereData[i][2]);
            glVertex3f(mSphereData[i][0], mSphereData[i][1], mSphereData[i][2]);
            glNormal3f(mSphereData[i][3], mSphereData[i][4], mSphereData[i][5]);
            glVertex3f(mSphereData[i][3], mSphereData[i][4], mSphereData[i][5]);
            glNormal3f(mSphereData[i][6], mSphereData[i][7], mSphereData[i][8]);
            glVertex3f(mSphereData[i][6], mSphereData[i][7], mSphereData[i][8]);
        }
        else {
            printf("Error at line %d", __LINE__);
        }
        glEnd();
    }
    // clean
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_1D);
    glDisable(GL_TEXTURE_GEN_S);
    glPopMatrix();
}

void drawFireworks()
{
    if (!mIsFireworkEnabled || !mIsStarted) return;
    glPointSize(3);
    
	glUseProgram(mShaderProgram);
	GLint shaderTimeId = glGetUniformLocation(mShaderProgram, "time");
	GLint shaderVelocityId = glGetAttribLocation(mShaderProgram, "velocity");
    
    GLfloat currentime = (glutGet(GLUT_ELAPSED_TIME) - mFireworkStartTime) % 10000;
	glUniform1f(shaderTimeId, currentime);
    
	for(int i=0; i<mParticleCount; i++)
	{
		glVertexAttrib3fv(shaderVelocityId, mParticleVelocities[i]);
		glBegin(GL_POINTS);
		glColor3fv(mParticleColors[i]);
		glVertex3fv(mParticleStartPosition);
		glEnd();
	}
    glUseProgram(0);
    glPopMatrix();
}

void drawFog()
{
    switch ((FogMode)mCurrentFogMode) {
        case FOG_DISABLE: 
            glDisable(GL_FOG);
            break;
        case FOG_LINEAR:
            glEnable(GL_FOG);
            glFogi(GL_FOG_MODE, GL_LINEAR);
            glFogfv(GL_FOG_COLOR, mFogColor);
            glFogf(GL_FOG_START, mFogStartValue);
            glFogf(GL_FOG_END, mFogEndValue);
            break;
        case FOG_EXPONENTIAL: 
            glEnable(GL_FOG);
            glFogi(GL_FOG_MODE, GL_EXP);
            glFogfv(GL_FOG_COLOR, mFogColor);
            glFogf(GL_FOG_START, mFogStartValue);
            glFogf(GL_FOG_END, mFogEndValue);
            glFogf(GL_FOG_DENSITY, mFogDensity);
            break;
        case FOG_EXPONENTIAL_SQUARE:
            glEnable(GL_FOG);
            glFogi(GL_FOG_MODE, GL_EXP2);
            glFogfv(GL_FOG_COLOR, mFogColor);
            glFogf(GL_FOG_START, mFogStartValue);
            glFogf(GL_FOG_END, mFogEndValue);
            glFogf(GL_FOG_DENSITY, mFogDensity);
            break;
    }
}

#pragma mark - Models

float getSphereRadius()
{
    float y_max = -10000, y_min = 10000;
    for (int i = 0; i < mNumberOfTrianglesInSphere; i++) {
        {
            GLfloat y = mSphereData[i][1];
            y_max = (y > y_max) ? y : y_max;
            y_min = (y < y_min) ? y : y_min;
        }
        {
            GLfloat y = mSphereData[i][4];
            y_max = (y > y_max) ? y : y_max;
            y_min = (y < y_min) ? y : y_min;
        }
        {
            GLfloat y = mSphereData[i][7];
            y_max = (y > y_max) ? y : y_max;
            y_min = (y < y_min) ? y : y_min;
        }
    }
    return (y_max - y_min) / 2;
}

float** getSphereNormals()
{
    float** normals = new float*[mNumberOfTrianglesInSphere];
    for (int a = 0; a < mNumberOfTrianglesInSphere; a++) {
        float* p1 = new float[3];
        p1[X] = mSphereData[a][0];
        p1[Y] = mSphereData[a][1];
        p1[Z] = mSphereData[a][2];
        float* p2 = new float[3];
        p2[X] = mSphereData[a][3];
        p2[Y] = mSphereData[a][4];
        p2[Z] = mSphereData[a][5];
        float* p3 = new float[3];
        p3[X] = mSphereData[a][6];
        p3[Y] = mSphereData[a][7];
        p3[Z] = mSphereData[a][8];
        float* v1 = calDirection(p1, p2);
        float* v2 = calDirection(p2, p3);
        float* n = calCrossProduct(v1, v2);
        normals[a] = calNormalizedVector(n);
    }
    return normals;
}

float getFovy()
{
    float sphereCenter[] = {0, mSphereRadius, 2};
    float distanceFromViewpoint = calDistance(mViewingPoint, sphereCenter);
    float size = calVectorLength(5.0, mSphereRadius, 6.0); // size of the whole scene
    float angleInRadian = 2.0 * atan2(size, distanceFromViewpoint);
    float angleInDegree = (180.0 * angleInRadian) / M_PI;
    return angleInDegree;
}

int getNextTrackIdOf(int trackId)
{
    return (trackId + 1) % mTrackCount;
}

bool isGoingToNextTrack()
{
    float distance = calDistance(mTrack[mCurrentTrackId], mCurrentCenterPosition);
    return distance > mTrackLength[mCurrentTrackId];
}

#pragma mark - Utilities

float calDistance(float* p1, float* p2)
{
    float dx = p1[X] - p2[X];
    float dy = p1[Y] - p2[Y];
    float dz = p1[Z] - p2[Z];
    return calVectorLength(dx, dy, dz);
}

float* calDirection(float* from, float* to)
{
    float* v = new float[3];
    v[X] = to[X] - from[X];
    v[Y] = to[Y] - from[Y];
    v[Z] = to[Z] - from[Z];
    float d = calVectorLength(v[X], v[Y], v[Z]);
    v[X] = v[X] / d;
    v[Y] = v[Y] / d;
    v[Z] = v[Z] / d;
    return v;
}

float* calCrossProduct(float* u, float* v)
{
    float* n = new float[3];
    n[X] = u[Y] * v[Z] - u[Z] * v[Y];
    n[Y] = u[Z] * v[X] - u[X] * v[Z];
    n[Z] = u[X] * v[Y] - u[Y] * v[X];
    return n;
}

float* calNormalizedVector(float* v)
{
    float d = calVectorLength(v[X], v[Y], v[Z]);
    v[X] = v[X] / d;
    v[Y] = v[Y] / d;
    v[Z] = v[Z] / d;
    return v;
}

float calVectorLength(float x, float y, float z)
{
    return sqrt(x * x + y * y + z * z);
}

#pragma mark - Shader Initializator

static char* readShaderSource(const char* shaderFile)
{
    FILE* fp = fopen(shaderFile, "rb");
    char* buf;
    long size;
    
    if(fp==NULL) return NULL;
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    buf = (char*) malloc((size+1) * sizeof(char));
    fread(buf, 1, size, fp);
    buf[size] = '\0'; /* terminate the string with NULL */
    fclose(fp);
    return buf;
}

static unsigned int initShader(const char* vShaderFile, const char* fShaderFile)
{
    GLint status;
    GLchar *vSource, *fSource;
    GLuint vShader, fShader; // for vertex and fragment shader handles
    GLuint program;          // for program handle
    GLchar *ebuffer; /* buffer for error messages */
    GLsizei elength; /* length of error message */
    
    /* read shader files */
    vSource = readShaderSource(vShaderFile);
    if(vSource==NULL)
    {
        printf("Failed to read vertex shader %s\n", vShaderFile);
        std::getchar();
        exit(EXIT_FAILURE);
    }
    else printf("Successfully read vertex shader %s\n", vShaderFile);
    
    fSource = readShaderSource(fShaderFile);
    if(fSource==NULL)
    {
        printf("Failed to read fragment shader %s\n", fShaderFile);
        std::getchar();
        exit(EXIT_FAILURE);
    }
    else printf("Successfully read fragment shader %s\n", fShaderFile);
    
    /* create program and shader objects */
    vShader = glCreateShader(GL_VERTEX_SHADER);
    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    program = glCreateProgram();
    
    /* attach shaders to the program object */
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    
    /* read shaders */
    glShaderSource(vShader, 1, (const GLchar**) &vSource, NULL);
    glShaderSource(fShader, 1, (const GLchar**) &fSource, NULL);
    
    /* compile vertex shader */
    glCompileShader(vShader);
    
    /* error check */
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &status);
    if(status==GL_FALSE)
    {
        printf("Failed to compile vertex shader %s\n", vShaderFile);
        glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &elength);
        ebuffer = (GLchar *) malloc(elength*sizeof(char));
        glGetShaderInfoLog(vShader, elength, NULL, ebuffer);
        printf("%s\n", ebuffer); free(ebuffer);
        std::getchar();
        exit(EXIT_FAILURE);
    }
    else printf("Successfully compiled vertex shader %s\n", vShaderFile);
    
    /* compile fragment shader */
    glCompileShader(fShader);
    
    /* error check */
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &status);
    if(status==GL_FALSE)
    {
        printf("Failed to compile fragment shader %s\n", fShaderFile);
        glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &elength);
        ebuffer = (GLchar *) malloc(elength*sizeof(char));
        glGetShaderInfoLog(fShader, elength, NULL, ebuffer);
        printf("%s\n", ebuffer); free(ebuffer);
        std::getchar();
        exit(EXIT_FAILURE);
    }
    else printf("Successfully compiled fragment shader %s\n", fShaderFile);
    
    /* link and error check */
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status==GL_FALSE)
    {
        printf("Failed to link program object\n");
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &elength);
        ebuffer = (GLchar *) malloc(elength*sizeof(char));
        glGetProgramInfoLog(program, elength, NULL, ebuffer);
        printf("%s\n", ebuffer); free(ebuffer);
        std::getchar();
        exit(EXIT_FAILURE);
    }
    else printf("Successfully linked program object\n\n");
    
    /*--- Return the created program handle ---*/
    return program; 
}

#pragma mark - Texture Mapper

void initImages()
{
    int i, j, c;
    
    /* --- Generate checkerboard image to the image array ---*/
    for (i = 0; i < GROUND_IMAGE_HEIGHT; i++)
        for (j = 0; j < GROUND_IMAGE_WIDTH; j++)
        {
            c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0));
            
            if (c == 1) /* white */
            {
                c = 255;
                mGroundImage[i][j][0] = (GLubyte) c;
                mGroundImage[i][j][1] = (GLubyte) c;
                mGroundImage[i][j][2] = (GLubyte) c;
            }
            else  /* green */
            {
                mGroundImage[i][j][0] = (GLubyte) 0;
                mGroundImage[i][j][1] = (GLubyte) 150;
                mGroundImage[i][j][2] = (GLubyte) 0;
            }
            
            mGroundImage[i][j][3] = (GLubyte) 255;
        }
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    /*--- Generate 1D stripe image to array stripeImage[] ---*/
    for (j = 0; j < STRIPE_IMAGE_WIDTH; j++) {
        /* When j <= 4, the color is (255, 0, 0),   i.e., red stripe/line.
         When j > 4,  the color is (255, 255, 0), i.e., yellow remaining texture
         */
        mStripeImage[4*j] = (GLubyte)    255;
        mStripeImage[4*j+1] = (GLubyte) ((j>4) ? 255 : 0);
        mStripeImage[4*j+2] = (GLubyte) 0;
        mStripeImage[4*j+3] = (GLubyte) 255;
    }
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    /*----------- End 1D stripe image ----------------*/
    
    /*--- texture mapping set-up is to be done in
     init() (set up texture objects)
     and in 
     display() (specify actual mapping and 
     assign texture coordinates, etc.)
     ---*/
}

#pragma mark - FPS fix tool

void fixFps()
{
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int timeInterval = currentTime - mFpsLastTime;
    if(timeInterval < 17) {
        unsigned int timeSleep = 17 - timeInterval;
#if WINDOWS
        Sleep(timeSleep);
#else
        usleep(timeSleep*1000);
#endif
    }
    mFpsLastTime = glutGet(GLUT_ELAPSED_TIME);
}