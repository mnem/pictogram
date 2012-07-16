//
//  Shader.vsh
//  MobilePoly
//
//  Created by David Wagner on 01/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

attribute vec4 position;
attribute vec3 normal;

varying lowp vec4 colorVarying;
varying lowp vec2 a;
varying lowp vec2 b;

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform vec2 arrayTest[2];

void main()
{
    vec3 eyeNormal = normalize(normalMatrix * normal);
    vec3 lightPosition = vec3(0.0, 0.0, 1.0);
    vec4 diffuseColor = vec4(0.4, 0.4, 1.0, 1.0);
    
    float nDotVP = max(0.0, dot(eyeNormal, normalize(lightPosition)));
	
	a = arrayTest[0];
	b = arrayTest[1];
	
    colorVarying = diffuseColor * nDotVP;
    
    gl_Position = modelViewProjectionMatrix * position;
}
