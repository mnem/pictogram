//
//  test_01.fsh
//
//  Created by David Wagner on 01/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

varying lowp vec4 colorVarying;
varying lowp vec2 a;
varying lowp vec2 b;

void main()
{
    gl_FragColor = colorVarying + (vec4(a, b));
}
