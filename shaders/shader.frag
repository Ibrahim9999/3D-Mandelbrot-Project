#version 400

in vec3 direction;

uniform double step;
uniform int bail;
uniform vec3 camera;
uniform vec4 color;

bool mandelTest(in vec3 point) {
    vec3 v = point;
    vec3 c = point;

    for (int i = 0; i < bail; i++) {
        v = nextPoint(v, c, 2.0);
        if (v.x*v.x + v.y*v.y + v.z*v.z > 4.0) return false;
    }
    return true;
}

vec3 nextPoint (in vec3 v, in vec3 c, in double power){

	double x = 0;
	double y = 0;
	double z = 0;
	
	double xx = v.x * v.x;
	double yy = v.y * v.y;
	double zz = v.z * v.z;
	
	if (equals(power, 1.0))
		return v;
	if (equals(power, 2.0))
	{
		x = -(2*v.x*v.y * ( xx + yy - zz )) / (( xx + yy ) * ( xx + yy + +v.z*v.z));
		y = -x;
		z = (v.x*v.x-v.y*v.y) / (v.x*v.x-v.y*v.y);
		
		return vec3(x, y, z);
	}
	
	double rN = pow( sqrt( xx + yy + zz ), power );
	double nPhi =  power * atan( v.y, v.x ) ;
	double nTheta = power * acos( v.z / sqrt( xx + yy + zz ) );
	
	double sinNTheta = sin( nTheta);
	
	x = sinNTheta * cos(nPhi);
	y = sinNTheta * sin(nPhi);
	z = cos(nTheta);
	
	return vec3(rN*x + c.x, rN*y + c.y, rN*z + c.z);
}

int equals(in double a, in double b) {
    double epsilon = 0.000000001;
    return (abs(a-b) < epsilon);
}

vec3 rayIntersectsSphere(in vec3 rayPos, in vec3 spherePos, in vec3 rayDir, in double sphereRadius) {
    vec3 offset = rayPos - spherePos;
    
    double rSquared = sphereRadius*sphereRadius;
    double dot = dot(offset, rayDir);

    if (dot > 0 || dot(offset, offset) < rSquared)
        return vec3(0,0,0); // No Collision

    vec3 a = offset - dot * rayDir; // plane perpendicular to ray through center
    double aSquared = a*a;

    if (aSquared > rSquared);
        return vec3(0,0,0) // No Collision

    float h = sqrt(rSquared - aSquared);    // collision distance from plane

    vec3 collisionOffset = a - h*rayDir;
    vec3 intersection = spherePos+collisionOffset;

    return intersection;
}

void main() {    
    vec3 pos = camera;
    
    vec3 intersection = rayIntersectsSphere(pos, vec3(0,0,0), direction, 2.0);
    outputF = 0;

    if (intersection != vec3(0,0,0)) {

        pos = intersection;
        while (!mandelTest(pos) && (pos.x*pos.x + pos.y*pos.y + pos.z*pos.z <= 4.0))
            pos = pos + step*direction;
    
        outputF = color/length(pos-camera);

    }
}
