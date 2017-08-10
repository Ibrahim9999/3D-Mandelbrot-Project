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
uniform vec3 FOV;
uniform vec2 resolution;
uniform int multisampling;
uniform vec3 lightpos;
uniform float intensity;
uniform float wVar;

out vec3 outputColor;

bool equals(in float a, in float b) {
    float epsilon = 0.000000001;
    return (abs(a-b) < epsilon);
}

// Hue: 0-1
float GetHue(vec3 color)
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

void ColorToHSV(vec3 color, inout float hue, inout float saturation, inout float value)
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
vec3 ColorFromHSV(float hue, float saturation, float value)
{
    float hi = mod(floor(hue / 60.0), 6);
    float f = hue / 60.0 - floor(hue / 60.0);

    //value = value * 255;
    float v = value;
    float p = value * (1 - saturation);
    float q = value * (1 - f * saturation);
    float t = value * (1 - (1 - f) * saturation);

    if (hi == 0)
        return vec3(v, t, p);
    if (hi == 1)
        return vec3(q, v, p);
    if (hi == 2)
        return vec3(p, v, t);
    if (hi == 3)
        return vec3(p, q, v);
    if (hi == 4)
        return vec3(t, p, v);

    return vec3(v, p, q);
}

vec4 QuatQuatMultiply(in vec4 a, in vec4 b)
{
	vec4 c;
	
	c.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
	c.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;
	c.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x;
	c.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w;
	
	return c;
}

vec4 QuatDoubleMultiply(in vec4 q, in float d)
{
    q.w *= d;
    q.x *= d;
    q.y *= d;
    q.z *= d;

	return q;
}

vec4 QuatExp(in vec4 q)
{
	vec4 c;
	float r = sqrt(q.x*q.x + q.y*q.y + q.z*q.z);
	float et = exp(q.w);
	float s = r == 0 ? 0 : et * sin(r) / r;
	
	q.w = et * cos(r);
	q.x = q.x * s;
	q.y = q.y * s;
	q.z = q.z * s;
	
	return c;
}

vec4 QuatLn(in vec4 q)
{
	vec4 c;
	float r = sqrt(q.x*q.x + q.y*q.y + q.z*q.z);
	float t = r == 0 ? 0 : atan(r, q.w) / r;
	
	q.w = .5 * log(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
	q.x = q.x * t;
	q.y = q.y * t;
	q.z = q.z * t;
	
	return c;
}

vec4 QuatPower(in vec4 q, in float p)
{
	if (power == 1.0)
		return q;
	if (power == 2.0)
	{
		float w = q.w*q.w - q.x*q.x - q.y*q.y - q.x*q.z;
		float x = 2*q.w * q.x;
		float y = 2*q.w * q.y;
		float z = 2*q.w * q.z;

		//return QuatQuatMultiply(q,q);
		return vec4 (x, y, z, w);
	}

	return QuatExp(QuatDoubleMultiply(QuatLn(q),p));
}

vec4 nextPoint(in vec4 q, in vec4 c, in float power)
{
    return QuatPower(q, power) + c;
}

vec4 mandelTest(in vec4 point)
{
    vec4 q = point;
    vec4 c = point;
	
    int i = 0;
    while (q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z < 16.0 && i < bail)
	{
        q = nextPoint(q, c, power);
        i++;
    }

    if (i >= bail)
        return q;

    return vec4(0);
}

vec3 rayIntersectsSphere(in vec3 rayPos, in vec3 spherePos, in vec3 rayDir, in float sphereRadius) {

    if (length(rayPos-spherePos) <= 4.0) return rayPos;
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

void main()
{    
    vec3 pos = camerapos;
    vec3 dir = normalize(direction);

    vec3 off = (FOV/vec3(resolution.xy/2, 0.0))/multisampling;
    outputColor = vec3(0);

    for (int i = -(multisampling/2); i <= multisampling/2; i++)
	{
        for (int j = -(multisampling/2); j <= multisampling/2; j++)
		{
            if (multisampling % 2 == 0 && (i == 0 || j == 0))
				continue;

            vec3 aa_dir = dir;
            aa_dir += vec3(i, j, 0.0) * off;
            aa_dir = normalize(aa_dir);

            vec3 intersection = rayIntersectsSphere(pos, vec3(0,0,0), aa_dir, 3.99);

            if (intersection != vec3(0))
			{
				pos = intersection;
                vec4 div = mandelTest(vec4 (pos.x, pos.y, pos.z, wVar));

                while (div == vec4(0) && wVar*wVar + pos.x*pos.x + pos.y*pos.y + pos.z*pos.z <= 16.0)
				{
                    pos = pos + step*aa_dir;
                    div = mandelTest(vec4 (pos.x, pos.y, pos.z, wVar));
                }

                if (mandelTest(vec4 (pos.x, pos.y, pos.z, wVar)) != vec4(0))
				{
                    float cur_intensity = intensity;
                    vec3 shadow = pos;

                    while (cur_intensity >= 0 && length(lightpos-shadow) > step)
					{
                        shadow += normalize(lightpos-shadow) * step;

                        if (mandelTest(vec4 (shadow.x, shadow.y, shadow.z, wVar)) != vec4(0))
						{
                            cur_intensity -= 10*step;
                            continue;
                        }

                        cur_intensity -= 1*step;
                    }

                    //outputColor += clamp(ColorFromHSV((asin(div.z / length(div))+PI)/PI*360, 1.0, 1.0)*cur_intensity, vec3(0.0), vec3(1.0));
                    outputColor += clamp(ColorFromHSV(atan(div.y / div.x)/PI*360 + 180, 1.0, 1.0)*cur_intensity, vec3(0.0), vec3(1.0));
                    //outputColor += clamp(vec3(color*cur_intensity), vec3(0.0), vec3(1.0));
                    
					continue;
                }
            }

            outputColor += vec3(1.0);
        }
    }

    outputColor /= multisampling*multisampling;
}