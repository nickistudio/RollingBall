//
//  Copyright (c) 2013 NickiStudio. All rights reserved.
//
//  Discard the fragment if the vertex is below a certain height.
//

varying vec4 vertex;   // use to communicate with vertex shader

void main()
{
    if (vertex.y < 0.1) discard;
    gl_FragColor = gl_Color;
}
