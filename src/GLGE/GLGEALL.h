/**
 * @file GLGEALL.h
 * @author DM8AT
 * @brief include all main cores from GLGE
 * @version 0.1
 * @date 2023-03-27
 * 
 * This header-only file includes all GLGE-cores(yes, the math file is a core)
 * 
 * Current cores:
 * - GLGE.h:
 *   the main core of the library, needed for both: 3D and 2D
 * - GLGE2Dcore.h:
 *   the 2D core of the library. It is used for 2D drawing, compleate with a 2D camera and a 2D Object called: Object2D
 * - GLGE3Dcore.h:
 *   the 3D core of the library. It does all the complecated 3D graphic things for you, like camera functions, projection, translation, ...
 * - GLGEMaterialCore.h:
 *   It is used to give color to 3D objects and input more than one texture to an shader
 * - GLGEShaderCore.h:
 *   This file contains a shader object to create custom shaders. A shader is needed for both 3D and 2D to be bound to an object
 * - GLGEMath.h:
 *   This file is the only file here independend form GLGE.h. It contains mathematical functions, like clamp and lerp, but also noise(SmoothNoise or InterpolatedNoise)
 * 
 * @copyright Copyright DM8AT 2023. All rights reserved. This project is released under the MIT license. 
 * 
 */

//include the core of the library
#include "GLGE.h"
//include the 2D part for drawing
#include "GLGE2Dcore.h"
//include the 3D part for drawing
#include "GLGE3Dcore.h"
//include the materials
#include "GLGEMaterialCore.h"
//include the shaders
#include "GLGEShaderCore.h"
//include math
#include "GLGEMath.h"
//include the window management system
#include "GLGEWindow.h"
//include textures
#include "GLGETexture.h"
//includet compute shaders
#include "GLGEComputeShader.h"
//include partickles
#include "GLGEParticles.h"
//include Meta-Objects
#include "GLGEMetaObject.h"