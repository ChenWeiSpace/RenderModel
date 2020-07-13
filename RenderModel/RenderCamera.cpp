#include "RenderCamera.h"

RenderCamera::RenderCamera()
{
	D3DXMatrixIdentity(&m_World);
	D3DXMatrixIdentity(&m_Projection);

	D3DXMatrixIdentity(&m_Projection);
	D3DXMatrixIdentity(&m_ViewProjection);
	updateMatrix();
}


RenderCamera::~RenderCamera()
{
}

void RenderCamera::setCameraParam(int width, int height, float zn /*= 0.01f*/, float zf /*= 100.0f*/)
{
	if ( width!= m_width || height != m_height || m_zn != zn || m_zf != zf)
	{
		m_width = width;
		m_height = height;
		m_zn = zn;
		m_zf = zf;
		updateProjectMatrix();
		updateViewProjectMatrix();
	}
}

void RenderCamera::setWorldPosition(D3DXVECTOR3 pos)
{
	if (memcmp(&m_worldPos, &pos, sizeof(XMVECTOR))!=0)
	{
		m_worldPos = pos;
		updateMatrix();
		updateViewProjectMatrix();
	}
}

void RenderCamera::setRotation(EulerAngle eularAngle)
{
	if (m_rotation != eularAngle)
	{
		m_rotation = eularAngle;
		updateMatrix();
		updateViewProjectMatrix();
	}
}

void RenderCamera::updateMatrix()
{
	static  const 	D3DXVECTOR3 s_xAxis{ 1,0,0 };
	static  const 	D3DXVECTOR3 s_yAxis{ 0,1,0 };
	static  const 	D3DXVECTOR3 s_zAxis{ 0,0,1 };

	D3DXMATRIX mCameraRot;
	D3DXMatrixRotationYawPitchRoll(&mCameraRot, D3DXToRadian(m_rotation.heading), D3DXToRadian(m_rotation.pitch), D3DXToRadian(m_rotation.roll));

	D3DXVECTOR3 vWorldUp, vWorldAhead;
	D3DXVECTOR3 vLocalUp = D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 vLocalAhead = D3DXVECTOR3(0, 0, 1);

	D3DXVec3TransformCoord(&vWorldUp, &vLocalUp, &mCameraRot);
	D3DXVec3TransformCoord(&vWorldAhead, &vLocalAhead, &mCameraRot);

	D3DXVec3TransformNormal(&m_xVector, &s_xAxis, &mCameraRot);
	D3DXVec3TransformNormal(&m_yVector, &s_yAxis, &mCameraRot);
	D3DXVec3TransformNormal(&m_zVector, &s_zAxis, &mCameraRot);

	//vWorldUp += m_worldPos;
	vWorldAhead += m_worldPos;
	D3DXMatrixLookAtLH(&m_View, &m_worldPos, &vWorldAhead, &vWorldUp);

	D3DXMatrixInverse(&m_World, NULL, &m_View);
}

void RenderCamera::updateProjectMatrix()
{
	D3DXMatrixPerspectiveFovLH(&m_Projection, D3DX_PI / 4, (float)m_width/ (float)m_height, m_zn, m_zf);
}

void RenderCamera::updateViewProjectMatrix()
{
	D3DXMatrixMultiply(&m_ViewProjection, &m_View, &m_Projection);
}
