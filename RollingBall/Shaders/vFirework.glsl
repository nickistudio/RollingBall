//
//  Copyright (c) 2013 NickiStudio. All rights reserved.
//
//  This is a vertex shader. It varies the location of each
//  particles vertex sinusoidally using the elapsed-time
//  parameter "time".
//

uniform   float time;      // elapsed time in milliseconds
varying   vec4  vertex;    // use to communicate with fragment shader
attribute vec3  velocity;  // particle relocity

void main()
{
    vertex = gl_Vertex;
    vertex.x = vertex.x + velocity.x * time * 0.001;
    vertex.y = vertex.y + velocity.y * time * 0.001 + 0.5 * -0.00000049 * time * time;
    vertex.z = vertex.z + velocity.z * time * 0.001;
    gl_Position = gl_ModelViewProjectionMatrix * vertex;
    gl_FrontColor = gl_Color;
}
