#version 130

#define BAILOUT_RADIUS 2.0
#define ALMOST_BAIL BAILOUT_RADIUS - .0001
#define PI 3.1415926535897932384626433
#define MAX_STEPS 1000

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
uniform int orbittrap;

out vec3 outputColor;

bool equals(in float a, in float b)
{
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

bool OrbitTrap(in vec3 v, in int ot)
{
	float r = BAILOUT_RADIUS*BAILOUT_RADIUS;
	float x = v.x*v.x;
	float y = v.y*v.y;
	float z = v.z*v.z;
	
	// SPHERE
	if (ot == 0)
		return x + y + z < r;
	
	// CUBE
	if (ot == 1)
		return x < r && y < r && z < r;
	
	// W_TRAP
	if (ot == 2)
		return x < r;

	// I_TRAP
	if (ot == 2)
		return y < r;

	// J_TRAP
	if (ot == 2)
		return z < r;
}

vec3 TriplexPower(in vec3 v, in float power)
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

    if (power == 1.0)
        return v + c;
    else if (power == 2.0)
    {
        float one_zz_xx_yy = 1 - zz / xx_yy;

        x = ( xx - yy ) * one_zz_xx_yy;
        y = 2 * v.x * v.y * one_zz_xx_yy;
        z = -2 * v.z * sqrt(xx+yy);

        return vec3(x, y, z) + c;
    }

	//return TriplexPower(v, power) + TriplexPower(c, -1);
	//return TriplexPower(v, power) + vec3(c.x, -c.y, c.z);
    return TriplexPower(v, power) + c;
}

vec3 mandelTest(in vec3 point)
{
    //vec3 v = TriplexPower(point, -1);
	//vec3 v = vec3(point.x, -point.y, -point.z);
	//vec3 v = vec3(point.x, abs(point.y), abs(point.z));
	vec3 v = point;

	//Julia Set Coordinates
	//vec3 c = vec3(-.4, .6, 0);
	//vec3 c = vec3(.25, .1, 0);
	//vec3 c = vec3(-0.7269, 0.1889, 0);
	//vec3 c = vec3(-1, 0, 0);
	//vec3 c = vec3(0, -.9, 0);
	//vec3 c = vec3(0, -.8, 0);
    vec3 c = point;
	
	int i;
	for(i = 0; i < bail && OrbitTrap(v, orbittrap); i++)
        v = nextPoint(v, c, power, theta, phi);

    if (i >= bail)
        return v;

    return vec3(0);
}

float Potential(in vec3 pos)
{
	vec3 z = pos;

	for(int i = 1; i < bail; i++)
	{
 		z = TriplexPower(z, power) + pos;

		if (dot(z,z) > BAILOUT_RADIUS)
			return log(length(z)) / pow(power, float(i));
	}	

	return 0.0;	
}

float DistanceEstimator(vec3 pos)
{
	vec3 z = pos;
	float derivative = 1.0;
	float r = 0.0;
	
	for (int i = 0; i < bail; i++)
	{
		r = length(z);

		if (r < BAILOUT_RADIUS)
			break;

		z = TriplexPower(z, power);

		z += pos;
	}
	
	return .5 * log(r) * r / derivative;
}
/*
float DistanceEstimator(in vec3 p)
{
	float pot = Potential(p);

	if (pot==0.0)
		return 0.0;

	gradient = (vec3(Potential(p+xDir*EPS), Potential(p+yDir*EPS), Potential(p+zDir*EPS))-pot)/EPS;

	return (.5 / exp(pot)) * sinh(pot) / length(gradient);
}
*/
vec3 CalculateNormal(vec3 p)
{
	float e = 2e-6f;
	float n = DistanceEstimator(p);
	float dx = DistanceEstimator(p + vec3(e, 0, 0)) - n;
	float dy = DistanceEstimator(p + vec3(0, e, 0)) - n;
	float dz = DistanceEstimator(p + vec3(0, 0, e)) - n;
	vec3 grad = vec3(dx, dy, dz);

	return normalize(grad);
}

vec3 rayIntersectsSphere(in vec3 rayPos, in vec3 spherePos, in vec3 rayDir, in float sphereRadius) {

    if (length(rayPos-spherePos) <= BAILOUT_RADIUS)
		return rayPos;
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

void main() {    
    vec3 pos = camerapos;
    vec3 dir = normalize(direction);
	float distanceStep = step;

    vec3 off = (FOV/vec3(resolution.xy/2, 0.0))/multisampling;
    outputColor = vec3(0);

    for (int i = -(multisampling/2); i <= multisampling/2; i++) {
        for (int j = -(multisampling/2); j <= multisampling/2; j++) {

            int steps = 0;

            if (multisampling % 2 == 0 && (i == 0 || j == 0))
				continue;

            vec3 aa_dir = dir;
            aa_dir += vec3(i, j, 0.0) * off;
            aa_dir = normalize(aa_dir);

            vec3 intersection = rayIntersectsSphere(pos, vec3(0,0,0), aa_dir, ALMOST_BAIL);
            //outputColor = vec4((aa_dir + 1)/2,1.0);
            //outputColor = vec4(1.0);

            if (intersection != vec3(0)) {

                pos = intersection;
                vec3 div = mandelTest(pos);

                while (div == vec3(0) && pos.x*pos.x + pos.y*pos.y + pos.z*pos.z < BAILOUT_RADIUS*BAILOUT_RADIUS)
				{
					distanceStep = DistanceEstimator(pos);

					if (distanceStep < step)
						distanceStep = step;

                    pos = pos + distanceStep*aa_dir;
                    div = mandelTest(pos);
                    steps++;
                }

                if (mandelTest(pos) != vec3(0))
				{
                    float cur_intensity = intensity;
                    vec3 shadow = pos;

                    while (cur_intensity >= 0 && length(lightpos-shadow) > step)
					{
                        shadow += normalize(lightpos-shadow) * step;

                        if (mandelTest(shadow) != vec3(0))
						{
                            cur_intensity -= 10*step;
                            continue;
                        }

                        cur_intensity -= 1*step;
                    }
                    cur_intensity = clamp(cur_intensity, 0, 1);

                    outputColor += clamp(ColorFromHSV((asin(div.z / length(div))+PI)/PI*360 + 180, 1.0, 1.0)*cur_intensity, vec3(0.0), vec3(1.0));
                    //outputColor += clamp(ColorFromHSV(atan(div.y, div.x)/PI*360 + 180, 1.0, 1.0)*cur_intensity, vec3(0.0), vec3(1.0));
                    //outputColor += clamp(vec3(color*cur_intensity), vec3(0.0), vec3(1.0));
                    float fao = 1 - (float(steps) * 0.25 * step);
                    //outputColor = fao * clamp(color * cur_intensity + color*0.5, vec3(0), vec3(1));
                    
					continue;
                }
            }
            outputColor += vec3(1.0);
        }
    }

    outputColor /= multisampling*multisampling;
}
