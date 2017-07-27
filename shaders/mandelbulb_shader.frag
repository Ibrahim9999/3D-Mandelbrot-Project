#version 130

in vec3 direction;

uniform float step;
uniform float power;
uniform int bail;
uniform vec3 camera;
uniform vec3 color;

out vec4 outputColor;

bool equals(in float a, in float b) {
    float epsilon = 0.000000001;
    return (abs(a-b) < epsilon);
}

vec3 nextPoint (in vec3 v, in vec3 c, in float power){

	float x = 0;
	float y = 0;
	float z = 0;
	
	float xx = v.x * v.x;
	float yy = v.y * v.y;
	float zz = v.z * v.z;
	
	if (equals(power, 1.0))
		return v;
	if (equals(power, 2.0))
	{
		x = (v.x*v.x - v.y*v.y) * (1 - (v.z*v.z) / (v.x*v.x + v.y*v.y));
		y = 2 * v.x * v.y * (1 - (v.z*v.z) / (v.x*v.x + v.y*v.y));
		z = -2 * v.z * sqrt(v.x*v.x + v.y*v.y);
		
		return vec3(x, y, z);
	}
	
    float r = sqrt( xx + yy + zz );
	float rN = pow( r, power );
	float nTheta = power * atan( v.y, v.x ) ;
	float nPhi = power * asin( v.z / r );
	
	float cosNPhi = cos( nPhi);
	
	x = cos(nTheta) * cosNPhi * rN;
	y = sin(nTheta) * cosNPhi * rN;
	z = -sin(nPhi) * rN;
	
	return vec3(rN*x + c.x, rN*y + c.y, rN*z + c.z);
}

bool mandelTest(in vec3 point) {
    vec3 v = point;
    vec3 c = point;

    for (int i = 0; i < bail; i++) {
        v = nextPoint(v, c, power);
        if (v.x*v.x + v.y*v.y + v.z*v.z > 4.0) return false;
    }
    return true;
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

void main() {    
    vec3 pos = camera;
    
    vec3 intersection = rayIntersectsSphere(pos, vec3(0,0,0), direction, 2.0);
    outputColor = vec4(0);

    if (intersection != vec3(0)) {

        pos = intersection;
        while (!mandelTest(pos) && (pos.x*pos.x + pos.y*pos.y + pos.z*pos.z <= 4.0))
            pos = pos + step*direction;
    
        outputColor = vec4(vec3(color/length(pos-camera)), 0);

    }
}
