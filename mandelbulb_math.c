#include <math.h>

#include "mandelbulb_math.h"

Point CalculateMandelbulb (Point v, Point c, double power){
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
		
		return (Point){x, y, z};
	}
	
	double rN = pow( sqrt( xx + yy + zz ), power );
	double nPhi =  power * atan2( v.y, v.x ) ;
	double nTheta = power * acos( v.z / sqrt( xx + yy + zz ) );
	
	double sinNTheta = sin( nTheta);
	
	x = sinNTheta * cos(nPhi);
	y = sinNTheta * sin(nPhi);
	z = cos(nTheta);
	
	return (Point){rN*x + c.x, rN*y + c.y, rN*z + c.z};
}

int equals(double a, double b) {
    double epsilon = 0.000000001;
    return (fabs(a-b) < epsilon);
}
