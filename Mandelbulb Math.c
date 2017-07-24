

struct Point
{
	double X, double Y, double Z
};

Point CalculateMandelbulb (Point v, Point c, double power){
	double x = 0;
	double y = 0;
	double z = 0;
	
	double xx = v.X * v.X;
	double yy = v.Y * v.Y;
	double zz = v.Z * v.Z;
	
	if (power.Equals(1))
		return v;
	if (power.Equals(2))
	{
		x = -(2*v.X*v.Y * ( xx + yy - zz )) / (( xx + yy ) * ( xx + yy + +v.Z*v.Z));
		y = -x;
		z = (v.X*v.X-v.Y*v.Y) / (v.X*v.X-v.Y*v.Y);
		
		return new Point(x, y, z);
	}
	
	double rN = Math.pow( Math.sqrt( xx + yy + zz ), power );
	double nPhi =  power * Math.atan2( v.Y, v.X ) ;
	double nTheta = power * arccos( v.Z / Math.sqrt( xx + yy + zz ) );
	
	double sinNTheta = Math.sin( nTheta);
	
	x = sinNTheta * Math.cos(nPhi);
	y = sinNTheta * Math.sin(nPhi);
	z = Math.cos(nTheta);
	
	return new Point(x, y, z);
}
