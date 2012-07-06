//
//  Shader.fsh
//  MobilePoly
//
//  Created by David Wagner on 01/07/2012.
//  Copyright (c) 2012 Noise & Heat. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
