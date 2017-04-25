float tagMyVec3::Get_Length(void)
{
	return sqrtf(x*x + y*y + z*z);
}

void tagMyVec3::Normalize(void)
{
	x /= sqrtf(x*x + y*y + z*z);
	y /= sqrtf(x*x + y*y + z*z);
	z /= sqrtf(x*x + y*y + z*z);
}

float tagMyVec3::Get_Dist(const D3DXVECTOR3* pDest)
{
	D3DXVECTOR3 vDir = *pDest - D3DXVECTOR3(x,y,z);
	return sqrtf(vDir.x*vDir.x + vDir.y*vDir.y + vDir.z*vDir.z);
}