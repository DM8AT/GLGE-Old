/**
 * @file ParticleTest.cpp
 * @author DM8AT
 * @brief Implement an example to showcase the capability of GLGE to render compute shader based particles
 * @version 0.1
 * @date 2024-07-03
 * 
 * @copyright Copyright DM8AT 2024. All rights reserved. This project is released under the MIT license. 
 * 
 */

#include "ParticleTest.hpp"
#include "GLGE/GLGEALL.h"
#include <iostream>

ParticleSystem* particles;
Camera* partCam;
Object* planet;

void partDraw()
{
    //draw all the particles
    particles->draw();
    //draw the planet
    planet->draw();
}

void partTick()
{
    //simple first person controller
    //rotation
    vec2 delta = (glgeGetMouse().pos - vec2(0.5)) * -0.05;
    partCam->rotate(delta.x * glgeGetDeltaTime(),delta.y * glgeGetDeltaTime(),0);
    glgeWarpPointer(vec2(0));
    glgeSetCursor(GLGE_CURSOR_STYLE_NONE);
    //movement
    float speed = 0.03 * glgeGetDeltaTime();
    if (glgeGetKeys().w) {partCam->move(-speed,0,0);}
    if (glgeGetKeys().s) {partCam->move(speed,0,0);}
    partCam->rotate(glgeToRadians(90));
    if (glgeGetKeys().a) {partCam->move(-speed,0,0);}
    if (glgeGetKeys().d) {partCam->move(speed,0,0);}
    partCam->rotate(-glgeToRadians(90));
    if (glgeGetKeys().space) {partCam->move(0,speed,0);}
    if (glgeGetKeys().leftShift) {partCam->move(0,-speed,0);}

    //make it possible to enter fullscreen
    if (glgePressedKeys.f11) {glgeToggleFullscreen();glgeWarpPointer(vec2(0));}

    //rotate all the particles around the emitter
    particles->setRot(particles->getRot() + vec3(0,glgeToRadians(0.0003 * glgeGetDeltaTime()),0));

    //update the camera
    partCam->update();
    partCam->recalculateProjection();
    //update the particles
    particles->update();
    //update the planet
    planet->update();

    //update the window title
    glgeSetWindowTitle((std::string("Particle Example: " + std::to_string(glgeGetCurrentFPS()) + " FPS")).c_str());
}

void runParticleExample()
{
    //initalise GLGE for 3D-Projects
    glgeInit();
    glgeCreateWindow("Particle Example: init...", 1000,1000);
    glgeInit3DCore();

    //create the camera
    partCam = new Camera(90.f, 0.1f, 3000.f, Transform(vec3(-231,45,346),vec3(-135,-10,0),vec3(0)));
    //bind the camera
    glgeBindCamera(partCam);

    //set the sampeling to closest for now
    glgeSetInterpolationMode(GLGE_NEAREST);

    //create the base mesh for the particles
    Mesh* mesh = new Mesh(GLGE_PRESET_CUBE, vec4(-1), 0);
    //create a new particle system with the basemesh
    particles = new ParticleSystem(mesh, 500000);
    //create a new material for the particle system
    particles->setMaterial(new Material("assets/cubeTexture.png"), 0.3);
    //create a compute shader as the controll shader
    particles->setControllShader(new ComputeShader("src/ParticleControllShader.cs"));
    //create a shader for the particles to use
    particles->setShader(new Shader(GLGE_DEFAULT_3D_PARTICLE_VERTEX_SHADER, GLGE_DEFAULT_3D_FRAGMENT));

    //switch back to linear sampeling
    glgeSetInterpolationMode(GLGE_LINEAR);

    //create the planet using a preset sphere
    planet = new Object(new Mesh(GLGE_PRESET_SPHERE, vec4(-1), 0), Transform(vec3(0),vec3(0), vec3(100)));
    //create a material for the planet
    planet->setMaterial(new Material(vec4(0.48, 0.26, 0.11,1), 0.5));

    //add a simple sky light
    glgeAddGlobalLighSource(new Light(vec3(0,0,0), glgeAngleToDir(vec3(45,-90,0)), GLGE_LIGHT_SOURCE_TYPE_DIRECTIONAL, vec3(1), 0.f, 10.f));

    //add simple post processing functions
    glgeSetPostProsessingShader("src/Shaders/glgeFinalise.fs");
    glgeSetPostProsessingShader("src/Shaders/FxAA.fs");

    //set the draw function
    glgeBindDisplayFunc(partDraw);
    //set the tick function
    glgeBindMainFunc(partTick);

    //position the mouse in the middle of the window
    glgeWarpPointer(vec2(0));
    //start GLGE
    glgeRunMainLoop();
}