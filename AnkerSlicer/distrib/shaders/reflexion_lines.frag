/****************************************************************************
* MeshLab                                                           o o     *
* An extendible mesh processor                                    o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005, 2009                                          \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/
//
// reflexion_lines.vert: Fragment shader for rendering with reflexion lines
//
// author: Gael Guennebaud
//

varying vec3 EyePos;
varying vec3 Normal;

uniform float ScaleFactor;
uniform float Smoothing;
uniform float Shading;

float arcSinus(float x) {
	return x * (1.0 + x*x * ((1.0/6.0) + x*x * ((3.0/40.0) + (15.0/336.0)*x*x)));
}

void main (void)
{
    vec4 color;

    vec3 reflexionVector;
    vec3 n = normalize(Normal);
    vec3 viewVec = normalize(EyePos);
    reflexionVector = viewVec.xyz - 2.0*n*dot(n,viewVec.xyz);
    reflexionVector.z += 1.0;
    reflexionVector.z = 0.5/sqrt(dot(reflexionVector,reflexionVector));
    reflexionVector.xy = (reflexionVector.xy*reflexionVector.z) + 0.5;
    reflexionVector *= 2.0;
//    color = vec4(clamp(0.5 + Smoothing * sin(2.0 * 3.1428 * reflexionVector.x*ScaleFactor), 0.0, 1.0),
//			clamp(0.5 + Smoothing * sin(2.0 * 3.1428 * reflexionVector.y*ScaleFactor), 0.0, 1.0),
//			clamp(0.5 + Smoothing * sin(2.0 * 3.1428 * reflexionVector.z*ScaleFactor), 0.0, 1.0),
//			1.0);
	
	float sharpness = 1.0/arcSinus(Smoothing*fwidth(reflexionVector.x)*2.0*ScaleFactor);
    color = vec4(clamp(0.5 + sharpness * sin(2.0 * 3.1428 * reflexionVector.x*ScaleFactor), 0.0, 1.0));
    color.a = 1.0;
    gl_FragColor = vec4(vec3(sharpness), 1.0);
	gl_FragColor = clamp(dot(gl_LightSource[0].position.xyz, n)*Shading + (1.0-Shading), 0.0, 1.0) * min(color, vec4(1.0,1.0,1.0,1.0));
}
