#pragma once
#include <cstdint>

#ifndef IsValidPtr
#define IsValidPtr(addr) ((DWORD64)(addr) > 0x100 && (DWORD64)(addr) < 0x00007fffffffffff  && !IsBadReadPtr(addr, sizeof(PVOID)))
#endif
#pragma warning(disable:4996)

#define OFFSET_GAMERENDERER 0x143FFBE10
#define OFFSET_CLIENTGAMECONTEXT 0x143DD7948

#define pLocalPlayer GameContext::Instance()->playerManager->localPlayer
#define pRenderView GameRenderer::Instance()->renderView
#define pClientArray GameContext::Instance()->playerManager->clientArray

class ClientSoldierHealthComponent
{
public:
	char pad_0000[32]; //0x0000
	float health; //0x0020
	float m_flMaxHealth; //0x0024
	char pad_0028[32]; //0x0028
}; //Size: 0x0048
static_assert(sizeof(ClientSoldierHealthComponent) == 0x48);

class ClientSoldierPrediction
{
public:
	char pad_0000[32]; //0x0000
	DirectX::XMFLOAT3 pos; //0x0020
	char pad_002C[4]; //0x002C
	DirectX::XMFLOAT3 velocity; //0x0030
	char pad_003C[96]; //0x003C
}; //Size: 0x009C
static_assert(sizeof(ClientSoldierPrediction) == 0x9C);

class ClientAimEntity
{
public:
	char pad_0000[224]; //0x0000
	DirectX::XMFLOAT4 cachedRayCast; //0x00E0
	char pad_00F0[184]; //0x00F0
}; //Size: 0x01A8
static_assert(sizeof(ClientAimEntity) == 0x1A8);

class ClientSoldier
{
public:
	char pad_0000[712]; //0x0000
	class ClientSoldierHealthComponent* clientSolderHealthComponent; //0x02C8
	char pad_02D0[516]; //0x02D0
	float heightoffset; //0x04D4
	char pad_04D8[640]; //0x04D8
	class ClientSoldierPrediction* clientSoldierPrediction; //0x0758
	char pad_0760[760]; //0x0760
	uint8_t occluded; //0x0A58
	char pad_0A59[356]; //0x0A59
}; //Size: 0x0BBD
// static_assert(sizeof(ClientSoldier) == 0xBBD);

class NameClass
{
public:
	char name[20]; //0x0000
	char pad_0014[120]; //0x0014
}; //Size: 0x008C
static_assert(sizeof(NameClass) == 0x8C);

class ClientPlayer
{
public:
	char pad_0000[24]; //0x0000
	class NameClass* nameClass; //0x0018
	char pad_0020[56]; //0x0020
	uint32_t team; //0x0058
	char pad_005C[12]; //0x005C
	char name[20]; //0x0068
	char pad_007C[388]; //0x007C
	class AttachedControllable* attachedControllable; //0x0200
	char pad_0208[8]; //0x0208
	class ClientSoldier* controlledControllable; //0x0210
	char pad_0218[408]; //0x0218
}; //Size: 0x03B0
static_assert(sizeof(ClientPlayer) == 0x3B0);

class ClientArray
{
public:
	class ClientPlayer* clients[60]; //0x0000
	char pad_01E0[56]; //0x01E0
}; //Size: 0x0218
static_assert(sizeof(ClientArray) == 0x218);

class GameContext
{
public:
	char pad_0000[88]; //0x0000
	class PlayerManager* playerManager; //0x0058
	char pad_0060[32]; //0x0060
	static GameContext* Instance() {
		return *reinterpret_cast<GameContext**>(OFFSET_CLIENTGAMECONTEXT);
	}
}; //Size: 0x0080
static_assert(sizeof(GameContext) == 0x80);

class ClientGameContext
{
public:
	class GameContext* N000000D7; //0x0000
	char pad_0008[8]; //0x0008
}; //Size: 0x0010
static_assert(sizeof(ClientGameContext) == 0x10);

class RenderView
{
public:
	char pad_0000[800]; //0x0000
	DirectX::XMFLOAT3 cameraPos; //0x0320
	char pad_032C[260]; //0x032C
	DirectX::XMFLOAT4X4 VPMatrix; //0x0430
	char pad_0470[176]; //0x0470
}; //Size: 0x0520
static_assert(sizeof(RenderView) == 0x520);

class StaticGameRenderer
{
public:
	class GameRenderer* gameRenderer; //0x0000
	char pad_0008[64]; //0x0008
}; //Size: 0x0048
static_assert(sizeof(StaticGameRenderer) == 0x48);

class GameRenderer
{
public:
	char pad_0000[1336]; //0x0000
	class RenderView* renderView; //0x0538
	char pad_0540[424]; //0x0540
	static GameRenderer* Instance() {
		return *reinterpret_cast<GameRenderer**>(OFFSET_GAMERENDERER);
	}
}; //Size: 0x06E8
static_assert(sizeof(GameRenderer) == 0x6E8);

class PlayerManager
{
public:
	char pad_0000[1384]; //0x0000
	class ClientPlayer* localPlayer; //0x0568
	char pad_0570[504]; //0x0570
	class ClientArray* clientArray; //0x0768
	char pad_0770[112]; //0x0770
}; //Size: 0x07E0
static_assert(sizeof(PlayerManager) == 0x7E0);

class AttachedControllable
{
public:
	char pad_0000[136]; //0x0000
}; //Size: 0x0088
static_assert(sizeof(AttachedControllable) == 0x88);

class ClientSoldierWeapon
{
public:
	char pad_0000[56]; //0x0000
	class SoldierWeaponData* soldierWeaponData; //0x0038
	char pad_0040[72]; //0x0040
}; //Size: 0x0088
static_assert(sizeof(ClientSoldierWeapon) == 0x88);

class SoldierWeaponData
{
public:
	char pad_0000[152]; //0x0000
	class WeaponFiring* weaponFiring; //0x0098
	char pad_00A0[40]; //0x00A0
}; //Size: 0x00C8
static_assert(sizeof(SoldierWeaponData) == 0xC8);

class WeaponFiring
{
public:
	char pad_0000[24]; //0x0000
	class FiringFunction* firingFunction; //0x0018
	char pad_0020[104]; //0x0020
}; //Size: 0x0088
static_assert(sizeof(WeaponFiring) == 0x88);

class FiringFunction
{
public:
	char pad_0000[264]; //0x0000
	float bulletspeed; //0x0108
	char pad_010C[68]; //0x010C
	uint32_t bulletsPerShot; //0x0150
	char pad_0154[52]; //0x0154
}; //Size: 0x0188
static_assert(sizeof(FiringFunction) == 0x188);