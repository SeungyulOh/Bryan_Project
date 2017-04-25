_vec3 Engine::MyGetCamPosition(LPDIRECT3DDEVICE9 pGraphicDev, VECTYPE eType)
{
	_matrix matView;
	_vec3 vCamPos;

	pGraphicDev->GetTransform(D3DTS_VIEW,&matView);
	D3DXMatrixInverse(&matView,0,&matView);
	memcpy(&vCamPos,&matView.m[eType][0],sizeof(_float)*3);
	return vCamPos;
}

_vec3 Engine::MyGetVec3FromMat(VECTYPE eType,_matrix InputMat)
{
	_vec3 vResult;
	memcpy(&vResult,&InputMat.m[eType][0],sizeof(_float)*3);
	return vResult;
}


_matrix Engine::MyGetNewMatExcScale(_matrix matIn)
{
	_matrix matResult, matScale;
	D3DXMatrixIdentity(&matResult);

	D3DXMatrixScaling(&matScale,D3DXVec3Length((_vec3*)&matIn.m[0][0]),D3DXVec3Length((_vec3*)&matIn.m[1][0]),D3DXVec3Length((_vec3*)&matIn.m[2][0]));
	D3DXMatrixInverse(&matScale,0,&matScale);
	matResult = matScale * matIn;
	return matResult;
}

_matrix MyGetRotMatFromWorldMat(_matrix matWorld)
{
	_matrix matResult, matScale;
	ZeroMemory(&matWorld.m[3][0],sizeof(float)*3);

	D3DXMatrixScaling(&matScale,D3DXVec3Length((_vec3*)&matWorld.m[0][0]),D3DXVec3Length((_vec3*)&matWorld.m[1][0]),D3DXVec3Length((_vec3*)&matWorld.m[2][0]));
	D3DXMatrixInverse(&matScale,0,&matScale);
	matResult = matScale * matWorld;
	return matResult;
}

_matrix MyGetTransMatFromWorldMat(_matrix matWorld)
{
	_matrix matResult;
	D3DXMatrixIdentity(&matResult);
	matResult._41 = matWorld._41;
	matResult._42 = matWorld._42;
	matResult._43 = matWorld._43;
	return matResult;
}

_matrix MyGetNewMatExcRot(_matrix matIn)
{
	_matrix matResult,matScale,matTrans;

	D3DXMatrixScaling(&matScale,D3DXVec3Length((_vec3*)&matIn.m[0][0]),D3DXVec3Length((_vec3*)&matIn.m[1][0]),D3DXVec3Length((_vec3*)&matIn.m[2][0]));
	D3DXMatrixTranslation(&matTrans,matIn._41,matIn._42,matIn._43);

	matResult = matScale * matTrans;
	return matResult;
}