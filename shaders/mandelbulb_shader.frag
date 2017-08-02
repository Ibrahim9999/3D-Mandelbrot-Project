#version 130

#define ALMOST_TWO 1.9999
#define PI 3.1415926535897932384626433

in vec3 direction;

uniform float step;
uniform float power;
uniform float theta;
uniform float phi;
uniform int bail;
uniform vec3 camerapos;
uniform vec3 color;

out vec4 outputColor;

bool equals(in float a, in float b) {
    float epsilon = 0.000000001;
    return (abs(a-b) < epsilon);
}

// Hue: 0-1
float GetHue(vec4 color)
{
    float hue = 0;
    float red = color.x;
    float green = color.y;
    float blue = color.z;

    float min = min(min(red, green), blue);
    float max = max(max(red, green), blue);

    if (max == red)
    {
        hue = (green - blue) / (max - min);
    }
    else if (max == green)
    {
        hue = 2 + (blue - red) / (max - min);
    }
    else
    {
        hue = 4 + (red - green) / (max - min);
    }

    hue = hue * 60.0;
    if (hue < 0) hue = hue + 360.0;

    return hue / 360.0;
}

void ColorToHSV(vec4 color, inout float hue, inout float saturation, inout float value)
{
    float max = max(color.x, max(color.y, color.z));
    float min = min(color.x, min(color.y, color.z));

    hue = GetHue(color);
    value = max / 255.0;

    if (max == 0)
        saturation = 0;
    else
        saturation = 1 - (min / max);
}

// RGBA: 0-1
vec4 ColorFromHSV(float hue, float saturation, float value)
{
    float hi = mod(floor(hue / 60.0), 6);
    float f = hue / 60.0 - floor(hue / 60.0);

    //value = value * 255;
    float v = value;
    float p = value * (1 - saturation);
    float q = value * (1 - f * saturation);
    float t = value * (1 - (1 - f) * saturation);

    if (hi == 0)
        return vec4(v, t, p, 0);
    if (hi == 1)
        return vec4(q, v, p, 0);
    if (hi == 2)
        return vec4(p, v, t, 0);
    if (hi == 3)
        return vec4(p, q, v, 0);
    if (hi == 4)
        return vec4(t, p, v, 0);

    return vec4(v, p, q, 0);
}

vec3 TriplexPower( in vec3 v, in float power)
{
	float x = 0;
    float y = 0;
    float z = 0;
    
    float r = sqrt( v.x * v.x + v.y * v.y + v.z * v.z );
    float rN = pow( r, power );
    float nTheta = power * atan( v.y, v.x ) ;
    float nPhi = power * asin( v.z / r );

    float cosNPhi = cos( nPhi + phi);
    x = cos(nTheta + theta) * cosNPhi * rN;
    y = sin(nTheta + theta) * cosNPhi * rN;
    z = -sin(nPhi + phi) * rN;

    return vec3(rN*x, rN*y, rN*z);
}

vec3 nextPoint (in vec3 v, in vec3 c, in float power, in float theta, in float phi)
{
    float x = 0;
    float y = 0;
    float z = 0;

    float xx = v.x * v.x;
    float yy = v.y * v.y;
    float zz = v.z * v.z;
    float xx_yy = xx + yy;
    
    if (false)
        return v + c;
    else if (false)
    {
        float one_zz_xx_yy = 1 - zz / xx_yy;

        x = ( xx - yy ) * one_zz_xx_yy;
        y = 2 * v.x * v.y * one_zz_xx_yy;
        z = -2 * v.z * sqrt(xx+yy);

        return vec3(x, y, z) + c;
    }

    return TriplexPower(v, power) + c;
	//return TriplexPower(v, power) + TriplexPower(c, power);
}

vec3 rayIntersectsSphere(in vec3 rayPos, in vec3 spherePos, in vec3 rayDir, in float sphereRadius) {

    if (length(rayPos-spherePos) <= 2.0) return rayPos;
    vec3 offset = rayPos - spherePos;

    float rSquared = sphereRadius*sphereRadius;
    float odot = dot(offset, rayDir);

    if (odot > 0 || dot(offset, offset) < rSquared)
        return vec3(0); // No Collision

    vec3 a = offset - odot * rayDir; // plane perpendicular to ray through center
    float aSquared = dot(a, a);

    if (aSquared > rSquared)
        return vec3(0); // No Collision


    float h = sqrt(rSquared - aSquared);    // collision distance from plane

    vec3 collisionOffset = a - h*rayDir;
    vec3 intersection = spherePos+collisionOffset;

    return intersection;
}

vec3 mandelTest(in vec3 point) {
    vec3 v = point;
    vec3 c = point;
	
    int i = 0;
    while (v.x*v.x + v.y*v.y + v.z*v.z < 4.0 && i < bail) {
        v = nextPoint(v, c, power, theta, phi);
        i++;
    }
    if (i >= bail)
        return v;
    return vec3(0);
}

void main() {    
    vec3 pos = camerapos;
    vec3 dir = normalize(direction);

    vec3 intersection = rayIntersectsSphere(pos, vec3(0,0,0), dir, ALMOST_TWO);
    //outputColor = vec4((dir + 1)/2,1.0);
    outputColor = vec4(1.0, 1, 1, 1);

    if (intersection != vec3(0)) 
	{
        pos = intersection;
        vec3 div = mandelTest(pos);

        while (div == vec3(0) && pos.x*pos.x + pos.y*pos.y + pos.z*pos.z <= 4.0)
		{
            pos = pos + step*dir;
            div = mandelTest(pos);
        }

        if (mandelTest(pos) != vec3(0))
		{
            vec3 lightpos = vec3(1., 1., -4);
            vec3 shadow = pos;
            float intensity = 4.50;

            while (intensity >= 0 && length(lightpos-shadow) > step)
			{
                shadow += normalize(lightpos-shadow) * step;

                if (mandelTest(shadow) != vec3(0))
                    intensity -= 10*step;
                else
                    intensity -= 1*step;
            }

            outputColor = clamp(ColorFromHSV((asin(div.z / length(div))+PI)/PI*360, 1.0, 1.0)*intensity, vec4(0.0), vec4(1.0));
            //outputColor = ColorFromHSV((atan(div.y, div.x)+PI)/2/PI*360, 1.0, 1.0);
            //outputColor = ColorFromHSV(0, 1.0, 1.0);
        }
    }
}
