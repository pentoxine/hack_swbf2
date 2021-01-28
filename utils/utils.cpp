#include "includes.h"

namespace utils {
	bool WorldToScreen(DirectX::XMFLOAT3& vPosition)
	{
		DirectX::XMFLOAT4X4 ViewProj = pRenderView->VPMatrix;
		float mX = (globals::wRect.right - globals::wRect.left) / 2;
		float mY = (globals::wRect.bottom - globals::wRect.top) / 2;

		float w =
			ViewProj(0, 3) * vPosition.x +
			ViewProj(1, 3) * vPosition.y +
			ViewProj(2, 3) * vPosition.z +
			ViewProj(3, 3);

		if (w < 0.65f)
		{
			vPosition.z = w;
			return false;
		}

		float x =
			ViewProj(0, 0) * vPosition.x +
			ViewProj(1, 0) * vPosition.y +
			ViewProj(2, 0) * vPosition.z +
			ViewProj(3, 0);

		float y =
			ViewProj(0, 1) * vPosition.x +
			ViewProj(1, 1) * vPosition.y +
			ViewProj(2, 1) * vPosition.z +
			ViewProj(3, 1);

		vPosition.x = (mX + mX * x / w);
		vPosition.y = (mY - mY * y / w);
		vPosition.z = w;
		return true;
	}
	float DistanceVec3(DirectX::XMFLOAT3 vec1, DirectX::XMFLOAT3 vec2)
	{
		float x_d = (vec2.x - vec1.x);
		float y_d = (vec2.y - vec1.y);
		float z_d = (vec2.z - vec1.z);
		return sqrt((x_d * x_d) + (y_d * y_d) + (z_d * z_d));
	}
}