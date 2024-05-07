#include <list>
#include <string>
#include "offsets.h"
#include "ue.h"
#include <string.h>
#include "xorst.h"
#include "fname.h"
#include <unordered_map>
#include "impl/inc.hpp"
inline bool runpreview = true;
inline bool toggled_tb = 0;
inline bool u_has_clicked;
std::chrono::steady_clock::time_point u_tb_begin;
std::chrono::steady_clock::time_point u_tb_end;
inline 	int u_tb_time_since;

int HeldWeaponType;
using namespace mi;
flinearcolor WhiteText (255, 255, 255, 255);

float radar_position_x{ 10.0f };
float radar_position_y{ 10.0f };
float radar_size{ 250.0f };


namespace radar_style {
	bool circle = true;
	bool rectangle = false;
}
namespace radar_text_mode {
	bool outlined = true;
	bool background = false;
}
bool rect_radar = false;

float RadarDistance = { 20.f };

namespace ExternalUtils {
	inline float custom_sqrtf(float _X)
	{
		SPOOF_FUNC;
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(_X)));
	}

	static inline void custom_Memcpy(const void* dstp, const void* srcp, UINT len)
	{
		SPOOF_FUNC;
		ULONG* dst = (ULONG*)dstp;
		ULONG* src = (ULONG*)srcp;
		UINT i, tail;

		for (i = 0; i < (len / sizeof(ULONG)); i++)
			*dst++ = *src++;

		tail = len & (sizeof(ULONG) - 1);
		if (tail) {

			UCHAR* dstb = (UCHAR*)dstp;
			UCHAR* srcb = (UCHAR*)srcp;

			for (i = len - tail; i < len; i++)
				dstb[i] = srcb[i];
		}
	}



	inline float custom_sinf(float _X)
	{
		SPOOF_FUNC;
		return _mm_cvtss_f32(_mm_sin_ps(_mm_set_ss(_X)));
	}

	inline float custom_cosf(float _X)
	{
		SPOOF_FUNC;
		return _mm_cvtss_f32(_mm_cos_ps(_mm_set_ss(_X)));
	}

	inline float custom_acosf(float _X)
	{
		SPOOF_FUNC;
		return _mm_cvtss_f32(_mm_acos_ps(_mm_set_ss(_X)));
	}

	inline float custom_tanf(float _X)
	{
		SPOOF_FUNC;
		return _mm_cvtss_f32(_mm_tan_ps(_mm_set_ss(_X)));
	}

	inline float custom_atan2f(float _X, float _Y)
	{
		SPOOF_FUNC;
		return _mm_cvtss_f32(_mm_atan2_ps(_mm_set_ss(_X), _mm_set_ss(_Y)));
	}

	inline float custom_pow(float _X, float _Y)
	{
		SPOOF_FUNC;
		return _mm_cvtss_f32(_mm_pow_ps(_mm_set_ss(_X), _mm_set_ss(_Y)));
	}

	inline float custom_pow(float _X)
	{
		SPOOF_FUNC;
		return _mm_cvtss_f32(_mm_sqrt_ps(_mm_set_ss(_X)));
	}

	inline float custom_asinf(float _X)
	{
		SPOOF_FUNC;
		return _mm_cvtss_f32(_mm_asin_ps(_mm_set_ss(_X)));
	}

	float custom_fabsf(float x) {
		SPOOF_FUNC;
		__m128 x_vec = _mm_set_ss(x);
		x_vec = _mm_and_ps(x_vec, _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF)));
		return _mm_cvtss_f32(x_vec);
	}


	inline int custom_compare(const char* X, const char* Y)
	{
		SPOOF_FUNC;
		while (*X && *Y) {
			if (*X != *Y) {
				return 0;
			}
			X++;
			Y++;
		}

		return (*Y == '\0');
	}

	inline int custom_wcompare(const wchar_t* X, const wchar_t* Y)
	{
		SPOOF_FUNC;
		while (*X && *Y) {
			if (*X != *Y) {
				return 0;
			}
			X++;
			Y++;
		}

		return (*Y == L'\0');
	}

	inline const wchar_t* custom_wcsstr(const wchar_t* X, const wchar_t* Y)
	{
		SPOOF_FUNC;
		while (*X != L'\0') {
			if ((*X == *Y) && custom_wcompare(X, Y)) {
				return X;
			}
			X++;
		}
		return NULL;
	}

	inline const char* custom_strstr(const char* X, const char* Y)
	{
		SPOOF_FUNC;
		while (*X != '\0') {
			if ((*X == *Y) && custom_compare(X, Y)) {
				return X;
			}
			X++;
		}
		return NULL;
	}

	inline int custom_strlen(const char* string)
	{
		SPOOF_FUNC;
		int cnt = 0;
		if (string)
		{
			for (; *string != 0; ++string) ++cnt;
		}
		return cnt;
	}

	inline int custom_wcslen(const wchar_t* string)
	{
		SPOOF_FUNC;
		int cnt = 0;
		if (string)
		{
			for (; *string != 0; ++string) ++cnt;
		}
		return cnt;
	}
}

enum EFortWeaponType : int
{
	Rifle = 0,
	Shotgun = 1,
	Smg = 2,
	Sniper = 3
};

enum EMovementMode : uint8
{
	MOVE_None = 0,
	MOVE_Walking = 1,
	MOVE_NavWalking = 2,
	MOVE_Falling = 3,
	MOVE_Swimming = 4,
	MOVE_Flying = 5,
	MOVE_Custom = 6,
	MOVE_MAX = 7,
};

uintptr_t RootComponent(uintptr_t actor)
{
	SPOOF_FUNC;
	return driver.Read<uintptr_t>(pid, actor + offset::root_component);
}
fvector GetLocation(uintptr_t actor)
{
	SPOOF_FUNC;
	return driver.Read<fvector>(pid, RootComponent(actor) + offset::relative_location);
}

char* wchar_to_char(const wchar_t* pwchar)
{
	int currentCharIndex = 0;
	char currentChar = pwchar[currentCharIndex];

	while (currentChar != '\0')
	{
		currentCharIndex++;
		currentChar = pwchar[currentCharIndex];
	}

	const int charCount = currentCharIndex + 1;

	char* filePathC = (char*)malloc(sizeof(char) * charCount);

	for (int i = 0; i < charCount; i++)
	{
		char character = pwchar[i];

		*filePathC = character;

		filePathC += sizeof(char);

	}
	filePathC += '\0';

	filePathC -= (sizeof(char) * charCount);

	return filePathC;
}

int Length;

namespace u_loop {
	class u_fn {
	public:

		/*auto get_player_name(uintptr_t player_state) -> std::string
		{
			SPOOF_FUNC
			if ( ud::dev )
			{
				std::cout << E ( "Name Esp" ) << std::endl;
			}
			int pNameLength;
			_WORD* pNameBufferPointer;
			int i;
			char v25;
			int v26;
			int v29;

			char16_t* pNameBuffer;

				uintptr_t pNameStructure = driver.Read<uintptr_t> (pid, player_state + 0xaf0); //pNameStructure
				if ( driver.is_valid ( pNameStructure ) ) {
					pNameLength = driver.Read<int> (pid, pNameStructure + 0x10 );
					if ( pNameLength <= 0)
						return "BOT";

					pNameBuffer = new char16_t[ pNameLength ];
					uintptr_t pNameEncryptedBuffer = driver.Read<uintptr_t> (pid, pNameStructure + 0x8 );
					if ( driver.is_valid ( pNameEncryptedBuffer ) ) {
						driver.Read(pid, (PVOID) pNameEncryptedBuffer, pNameBuffer, pNameLength );

						v25 = pNameLength - 1;
						v26 = 0;
						pNameBufferPointer = ( _WORD* )pNameBuffer;

						for ( i = ( v25 ) & 3;; *pNameBufferPointer++ += i & 7 ) {
							v29 = pNameLength - 1;
							if ( ! ( _DWORD ) pNameLength )
								v29 = 0;

							if ( v26 >= v29 )
								break;

							i += 3;
							++v26;
						}

						std::u16string temp_wstring (pNameBuffer);
						delete[] pNameBuffer;
						return std::string ( temp_wstring.begin( ), temp_wstring.end( ) );
					}
				}
		}*/

		std::string decrypt_player_name(uintptr_t player_state)
		{
				int pNameLength; // rsi
			_WORD* pNameBufferPointer;
			int i; // ecx
			char v25; // al
			int v26; // er8
			int v29; // eax

			uintptr_t pNameStructure = driver.Read<uintptr_t>(pid, player_state + 0xaf0);
			pNameLength = driver.Read<int>(pid, pNameStructure + 0x10);
			if (pNameLength <= 0) return "BOT / AI";

			wchar_t* pNameBuffer = new wchar_t[pNameLength];
			uintptr_t pNameEncryptedBuffer = driver.Read<uintptr_t>(pid, pNameStructure + 0x8);
			driver.Read_Array(pid, pNameEncryptedBuffer, pNameBuffer, pNameLength * sizeof(wchar_t));

			v25 = pNameLength - 1;
			v26 = 0;
			pNameBufferPointer = (_WORD*)pNameBuffer;

			for (i = (v25) & 3; ; *pNameBufferPointer++ += i & 7)
			{
				v29 = pNameLength - 1;
				if (!(_DWORD)pNameLength)
					v29 = 0;

				if (v26 >= v29)
					break;

				i += 3;
				++v26;
			}

			std::wstring temp_wstring(pNameBuffer);
			return std::string(temp_wstring.begin(), temp_wstring.end());
		}

		std::string get_platform_name(uintptr_t player_state)
		{
			SPOOF_FUNC
				int pNameLength;

			uintptr_t pNameStructure = driver.Read<uintptr_t>(pid, player_state + 0xaf0);
			pNameLength = driver.Read<int>(pid, pNameStructure + 0x10);
			if (pNameLength <= 0) return "";

			wchar_t* pNameBuffer = new wchar_t[pNameLength];
			driver.Read_Array(pid, pNameStructure, &pNameBuffer, pNameLength * sizeof(wchar_t));
			std::wstring temp_wstring(pNameBuffer);
			return std::string(temp_wstring.begin(), temp_wstring.end());
		}


		void NoReloadThread()
		{


			if (ud::no_reload)
			{
				if (ud_cache->acknowledged_pawn)
				{
					WeaponInformation held_weapon{ };

					uint64_t player_weapon = driver.Read<uint64_t>(pid, ud_cache->acknowledged_pawn + 0xa20);

					held_weapon.reloading_weapon = driver.Read<bool>(pid, player_weapon + 0x368);

					uint64_t Mesh = driver.Read<uint64_t>(pid, ud_cache->acknowledged_pawn + offset::skeletal_mesh);

					if (held_weapon.reloading_weapon)
					{
						driver.Write<float>(pid, Mesh + 0x9F8, 999);
					}
					else
					{
						driver.Write<float>(pid, Mesh + 0x9F8, 1);
					}
				}
				

			}
		}


		void NO_recoil()
		{
			uint64_t CurrentActorWeapon = driver.Read<uint64_t>(pid, ud_cache->acknowledged_pawn + 0xA20);

			while (true)
			{
				driver.Write(pid, CurrentActorWeapon + 0x1b0c, 0);

				Sleep(1);
			}


		}


		void infFuelThread()
		{
			while (true)
			{
				uint64_t CurrentVehicle = driver.Read<uint64_t>(pid, ud_cache->acknowledged_pawn + 0x2960); //CURRENT VEHICLE
				auto FuelComponent = driver.Read<uintptr_t>(pid, (uintptr_t)CurrentVehicle + 0x1260); //FortAthenaVehicle::CachedFuelComponent
				driver.Write<float>(pid, FuelComponent + 0xf0, 2000.f);

				Sleep(1);
			}
		}

		void carfly()
		{
			while (true)
			{
				uint64_t CurrentVehicle = driver.Read<uint64_t>(pid, ud_cache->acknowledged_pawn + 0x2960); //AFortPlayerPawn::CurrentVehicle
				driver.Write<bool>(pid, CurrentVehicle + 0x813, 0);
				if (GetAsyncKeyState(VK_F4))
				{
					driver.Write<bool>(pid, CurrentVehicle + 0x812, 0); //AFortAthenaVehicle::bUseGravity
				}

				Sleep(1);
			}
		}

		//void GliderRedeployThread()
		//{
		//	if (ud::fly)
		//	{
		//		if (ud_cache->acknowledged_pawn)
		//		{
		//			if (GetAsyncKeyState_Spoofed('G'))
		//			{
		//				driver.Write<float>(pid, ud_cache->acknowledged_pawn + 0x1FB4, 0.02f); //bIsParachuteOpen Offset
		//				driver.Write<float>(pid, ud_cache->acknowledged_pawn + 0x1FB3, 0.02f); //bIsSkydiving  Offset

		//				
		//			}
		//			else
		//			{
		//			}
		//			Sleep(1);

		//		}
		//	}
		//}

		void PlayerFlyThread()
		{

			if (ud::grab_defaults)
			{
				if (ud_cache->acknowledged_pawn)
				{
					uint64_t CharacterMovement = driver.Read<uint64_t>(pid, ud_cache->acknowledged_pawn + 0x320);  // Offsets::ACharacter::CharacterMovement
					ud::fly_orig = driver.Read<fvector>(pid, CharacterMovement + UFortMovementComp_Character::SwingLaunch);
					ud::grab_defaults = false;
				}
				
			}

			while (true)
			{
				if (ud::fly)
				{
					if (ud_cache->acknowledged_pawn)
					{
						uint64_t CharacterMovement = driver.Read<uint64_t>(pid, ud_cache->acknowledged_pawn + 0x320);  // Offsets::ACharacter::CharacterMovement
						if (CharacterMovement)
						{

							//if (ud::grab_defaults)
							//{
							//	wants_to_swing_default = driver.Read<short>(pid, CharacterMovement + UFortMovementComp_Character::bWantsToSwing);
							//    swing_attach_location_default = driver.Read<fvector>(pid, CharacterMovement + UFortMovementComp_Character::SwingAttachLocation);
							//	swing_instant_velocity_default = driver.Read<bool>(pid, CharacterMovement + UFortMovementComp_Character::bSwingInstantVelocity);

							//	ud::grab_defaults = false;

							//}

							

							driver.Write<short>(pid, CharacterMovement + UFortMovementComp_Character::bWantsToSwing, short(257));
							driver.Write<fvector>(pid, CharacterMovement + UFortMovementComp_Character::SwingAttachLocation, fvector{ 0.0, 0.0, 10000000.0 });
							driver.Write<bool>(pid, CharacterMovement + UFortMovementComp_Character::bSwingInstantVelocity, true);

							uint64_t Mesh = driver.Read<uint64_t>(pid, ud_cache->acknowledged_pawn + offset::skeletal_mesh);
							FTransform MeshTransform = driver.Read<FTransform>(pid, Mesh + offset::component_to_world);

							if (GetAsyncKeyState('W'))
							{
								double Angle;

								Angle = camera_postion.rotation.y * (M_PI / 180.0);
								double sy = sin(Angle);
								double cy = cos(Angle);

								Angle = -camera_postion.rotation.x * (M_PI / 180.0);
								double sp = sin(Angle);
								double cp = cos(Angle);

								driver.Write<fvector>(pid, CharacterMovement + UFortMovementComp_Character::SwingLaunch, fvector{ cp * cy * 800.0, cp * sy * 800.0, -sp * 800.0 * 5.0 });
							}

							if (GetAsyncKeyState('A'))
							{
								driver.Write<fvector>(pid, CharacterMovement + UFortMovementComp_Character::SwingLaunch, fvector{-150, 0, 0});
							}

							if (GetAsyncKeyState('D'))
							{
								driver.Write<fvector>(pid, CharacterMovement + UFortMovementComp_Character::SwingLaunch, fvector{ 150, 0, 0 });
							}

							if (GetAsyncKeyState(VK_SPACE))
							{
								driver.Write<fvector>(pid, CharacterMovement + UFortMovementComp_Character::SwingLaunch, fvector{ 0.0, 0.0, 3500.0 });
							}

							if (GetAsyncKeyState(VK_CONTROL))
							{
								driver.Write<fvector>(pid, CharacterMovement + UFortMovementComp_Character::SwingLaunch, fvector{ 0.0, 0.0, -3500.0 });
							}
						}
					}
				}
				else
				{
					//if (wants_to_swing_default != NULL && swing_attach_location_default.z != NULL && swing_instant_velocity_default != NULL)
					//{
					//	uint64_t CharacterMovement = driver.Read<uint64_t>(pid, ud_cache->acknowledged_pawn + 0x320);  // Offsets::ACharacter::CharacterMovement
					//	driver.Write<short>(pid, CharacterMovement + UFortMovementComp_Character::bWantsToSwing, wants_to_swing_default);
					//	driver.Write<fvector>(pid, CharacterMovement + UFortMovementComp_Character::SwingAttachLocation, swing_attach_location_default);
					//	driver.Write<bool>(pid, CharacterMovement + UFortMovementComp_Character::bSwingInstantVelocity, swing_instant_velocity_default);

					//	ud::grab_defaults = true;
					//}
					ud::grab_defaults = true;
		            uint64_t CharacterMovement = driver.Read<uint64_t>(pid, ud_cache->acknowledged_pawn + 0x320);  // Offsets::ACharacter::CharacterMovement
					driver.Write<short>(pid, CharacterMovement + UFortMovementComp_Character::bWantsToSwing, short(0));
				    driver.Write<fvector>(pid, CharacterMovement + UFortMovementComp_Character::SwingAttachLocation, fvector(0.0, 0.0, 0.0));
					driver.Write<bool>(pid, CharacterMovement + UFortMovementComp_Character::bSwingInstantVelocity, false);
					driver.Write<fvector>(pid, CharacterMovement + UFortMovementComp_Character::SwingLaunch, fvector{0, 0, 0});

					//auto nigga = driver.Read<fvector>(pid, CharacterMovement + UFortMovementComp_Character::SwingAttachLocation);
					//std::cout << "x: " << nigga.x << " y: " << nigga.y << " z: "  << nigga.z << std::endl;
				}

				Sleep(1);
			}
		}

		static auto CacheLevels() -> void {

			for (;; )
			{

				if (ud_cache->acknowledged_pawn)
				{
					if (!ud::draw_chests && !ud::lamma && !ud::pickups && !ud::draw_vehicles) continue;
					if (!ud_cache->uworld) continue;
					std::vector<item> mrxd;
					uintptr_t ItemLevels = driver.Read<uintptr_t>(pid, ud_cache->uworld + 0x170);

					for (int i = 0; i < driver.Read<DWORD>(pid, ud_cache->uworld + (0x170 + sizeof(PVOID))); ++i) {

						uintptr_t ItemLevel = driver.Read<uintptr_t>(pid, ItemLevels + (i * sizeof(uintptr_t)));

						for (int i = 0; i < driver.Read<DWORD>(pid, ItemLevel + (0xA0 + sizeof(PVOID))); ++i) {

							uintptr_t ItemsPawns = driver.Read<uintptr_t>(pid, ItemLevel + 0xA0);

							uintptr_t CurrentItemPawn = driver.Read<uintptr_t>(pid, ItemsPawns + (i * sizeof(uintptr_t))); // CurrentActor

							fvector ItemPosition = GetLocation(CurrentItemPawn);
							float ItemDist = camera_postion.location.distance(ItemPosition) / 100.f;

							int ItemIndex = driver.Read<int>(pid, CurrentItemPawn + 0x18);

							auto CurrentItemPawnName = GetNameFromFName(ItemIndex);



							if (ud::dev2)
							{
								std::cout << CurrentItemPawnName << std::endl;
								std::cout << ("fname -> ") << FNAMEPOOL_OFFSET << std::endl;
							}

							bool shouldUpdate;
							bool bIsPickup;

							if (driver.is_valid(CurrentItemPawn))
							{
								if (ud::pickups or ud::draw_vehicles or ud::lamma or ud::draw_chests)
								{
									bIsPickup = true;
									shouldUpdate = true;
								}
							}

							if (shouldUpdate)
							{
								item item{ };
								item.Actor = CurrentItemPawn;
								item.Name = CurrentItemPawnName;

								item.isPickup = bIsPickup;
								item.distance = ItemDist;

								mrxd.push_back(item);

							}
						}
					}
					item_pawns.clear();
					item_pawns = mrxd;
					std::this_thread::sleep_for(std::chrono::milliseconds(500));
				}
			}
		}


		camera_position_s get_camera()
		{
			SPOOF_FUNC

				camera_position_s camera;

			auto location_pointer = driver.Read<uintptr_t>(pid, ud_cache->uworld + 0x110);
			auto rotation_pointer = driver.Read<uintptr_t>(pid, ud_cache->uworld + 0x120);

			struct FNRot
			{
				double a; //0x0000
				char pad_0008[24]; //0x0008
				double b; //0x0020
				char pad_0028[424]; //0x0028
				double c; //0x01D0
			}fnRot;

			fnRot.a = driver.Read<double>(pid, rotation_pointer);
			fnRot.b = driver.Read<double>(pid, rotation_pointer + 0x20);
			fnRot.c = driver.Read<double>(pid, rotation_pointer + 0x1d0);

			camera.location = driver.Read<fvector>(pid, location_pointer);
			camera.rotation.x = asin(fnRot.c) * (180.0 / M_PI);
			camera.rotation.y = ((atan2(fnRot.a * -1, fnRot.b) * (180.0 / M_PI)) * -1) * -1;
			camera.fov = driver.Read<float>(pid, (uintptr_t)ud_cache->player_controller + 0x394) * 90.f;

			return camera;
		}

		inline fvector2d w2s(fvector WorldLocation)
		{
			SPOOF_FUNC

				camera_postion = get_camera();

			if (WorldLocation.x == 0)
				return fvector2d(0, 0);

			_MATRIX tempMatrix = Matrix(camera_postion.rotation);

			fvector vAxisX = fvector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
			fvector vAxisY = fvector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
			fvector vAxisZ = fvector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

			fvector vDelta = WorldLocation - camera_postion.location;
			fvector vTransformed = fvector(vDelta.dot(vAxisY), vDelta.dot(vAxisZ), vDelta.dot(vAxisX));

			if (vTransformed.z < 1.f)
				vTransformed.z = 1.f;

			return fvector2d((screen_width / 2.0f) + vTransformed.x * (((screen_width / 2.0f) / tanf(camera_postion.fov * (float)M_PI / 360.f))) / vTransformed.z, (screen_height / 2.0f) - vTransformed.y * (((screen_width / 2.0f) / tanf(camera_postion.fov * (float)M_PI / 360.f))) / vTransformed.z);
		}

		static auto is_visible(uintptr_t skeletal_mesh) -> bool {
			SPOOF_FUNC
				auto last_submit = driver.Read<float>(pid, skeletal_mesh + 0x2E8); //lastsumbittime
			auto last_render = driver.Read<float>(pid, skeletal_mesh + 0x2F0); //lastrendertimeonscreen
			return (bool)(last_render + 0.06f >= last_submit);

		}

		fvector CalculateAngle(fvector LocalPos, fvector WorldPos) {
			fvector RelativePos = WorldPos - LocalPos;
			float yaw = atan2(RelativePos.y, RelativePos.x) * 180.0f / M_PI;
			float pitch = -((acos(RelativePos.z / LocalPos.distance(WorldPos)) * 180.0f / M_PI) - 90.0f);
			return fvector(pitch, yaw, 0);
		}

		void Normalize(fvector& in)
		{
			while (in.x > 180.0f)
				in.x -= 360.0f;

			while (in.x < -180.0f)
				in.x += 360.0f;

			while (in.y > 180.0f)
				in.y -= 360.0f;

			while (in.y < -180.0f)
				in.y += 360.0f;

			in.z = 0;
		}

		float Clamp(float value, float min, float max) {
			return (value < min) ? min : ((value > max) ? max : value);
		}

		inline void RotateCamera(fvector target) {
			fvector calc = CalculateAngle(camera_postion.location, target);
			Normalize(calc);

			fvector current_rot = camera_postion.rotation;
			Normalize(current_rot);

			fvector delta = calc - current_rot;
			Normalize(delta);

			(delta.x = delta.x / (ud::u_smooth));
			(delta.y = delta.y / (ud::u_smooth));

			fvector input = fvector(Clamp(delta.x, -0.098f, 0.098f), Clamp(delta.y, -0.098f, 0.098f), 0);

			driver.Write<fvector>(pid, ud_cache->player_controller + 0x518 + 0x8, input);
			driver.Write<fvector>(pid, ud_cache->player_controller + 0x810, input);
		}

		/*static auto get_bone_3d(uintptr_t skeletal_mesh, int bone_index) -> fvector {

			SPOOF_FUNC

			auto is_cached = driver.Read<int>(pid, skeletal_mesh + offset::is_cached);

			auto bone_array = driver.Read<uintptr_t>(pid, skeletal_mesh + offset::bone_array + (is_cached * 0x10));

			auto bone = driver.Read<FTransform>(pid, bone_array + (bone_index * 0x60));

			auto component_to_world = driver.Read<FTransform>(pid, skeletal_mesh + offset::component_to_world);

			D3DMATRIX matrix = MatrixMultiplication(bone.ToMatrixWithScale(), component_to_world.ToMatrixWithScale());

			return fvector(matrix._41, matrix._42, matrix._43);
		}*/

		static auto get_bone_3d(uintptr_t skeletal_mesh, int bone_index) -> fvector
		{
			uint64_t boneArray = driver.Read<uint64_t>(pid, skeletal_mesh + 0x598 + (driver.Read<int>(pid, skeletal_mesh + 0x5e0) * 0x10));
			FTransform bone = driver.Read<FTransform>(pid, boneArray + (bone_index * 0x60));
			FTransform componentToWorld = driver.Read<FTransform>(pid, skeletal_mesh + 0x1c0);
			D3DMATRIX matrix = MatrixMultiplication(bone.ToMatrixWithScale(), componentToWorld.ToMatrixWithScale());
			return fvector(matrix._41, matrix._42, matrix._43);
		}

		auto in_screen(fvector2d screen_location) -> bool {

			if (screen_location.x > 0 && screen_location.x < screen_width && screen_location.y > 0 && screen_location.y < screen_height) return true;
			else return false;

		}

		fvector PredictLocation(fvector target, fvector targetVelocity, float projectileSpeed, float projectileGravityScale, float distance)
		{
			float horizontalTime = distance / projectileSpeed;
			float verticalTime = distance / projectileSpeed;

			target.x += targetVelocity.x * horizontalTime;
			target.y += targetVelocity.y * horizontalTime;
			target.z += targetVelocity.z * verticalTime +
				abs(-980 * projectileGravityScale) * 0.5f * (verticalTime * verticalTime);

			return target;
		}

		fvector predict_custom(fvector target, fvector velocity, float projectile_speed_scale, float projectile_gravity_scale, float distance)
		{
			int exceed_count_70 = static_cast<int>((distance - 70) / 70);
			int exceed_count_125 = static_cast<int>((distance - 125) / 125);
			int exceed_count_150 = static_cast<int>((distance - 150) / 150);

			float horizontal_time = distance / projectile_speed_scale;
			float vertical_time = distance / projectile_speed_scale;

			if (exceed_count_150 > 0)
			{
				projectile_gravity_scale -= 0.3f;
			}
			else if (exceed_count_125 > 0)
			{
				projectile_gravity_scale -= 0.4f;
			}
			else if (exceed_count_70 > 0)
			{
				projectile_gravity_scale -= 0.5f;
			}

			target.x += velocity.x * horizontal_time;
			target.y += velocity.y * horizontal_time;
			target.z += velocity.z * vertical_time + fabs(-980 * projectile_gravity_scale) * 0.5f * (vertical_time * vertical_time);

			return target;
		}
		
		void reset_angles() {
			float ViewPitchMin = -89.9999f;
			float ViewPitchMax = 89.9999f;
			float ViewYawMin = 0.0000f;
			float ViewYawMax = 359.9999f;

			uintptr_t ViewYawMinoffset = 0x244c;
			uintptr_t ViewYawMaxoffset = 0x2450;
			uintptr_t AimPitchMinoffset = 0x17a8;
			uintptr_t AimPitchMaxoffset = 0x17ac;

			uintptr_t PCameraMan = driver.Read<uintptr_t>(pid, ud_cache->player_controller + 0x348); // PlayerCameraManager 0x348(0x08)
			if (PCameraMan != 0) {
				driver.Write<float>(pid, PCameraMan + AimPitchMaxoffset, ViewPitchMax); // float ViewPitchMax;
				driver.Write<float>(pid, PCameraMan + AimPitchMinoffset, ViewPitchMin); // float ViewPitchMin; 
				driver.Write<float>(pid, PCameraMan + ViewYawMinoffset, ViewYawMin); // float ViewYawMin;
				driver.Write<float>(pid, PCameraMan + ViewYawMaxoffset, ViewYawMax);  // float ViewYawMax;
			}
		}

		void LevelDrawing()
		{
			SPOOF_FUNC;
			auto levelListCopy = item_pawns;

			//ImGui::PushFont(GameFont);

			for (auto entity : levelListCopy)
			{
				if (ud_cache->acknowledged_pawn && entity.Actor)
				{
					/*if (ud::draw_vehicles && strstr(entity.Name.c_str(), _xor_("Rufus_GolfCartVehicleSK_C").c_str()))
					{
						if (entity.distance <= ud::vehiclesrender)
						{
							fvector Location = GetLocation(entity.Actor);
							fvector2d Screen = w2s(Location);
							if (in_screen(Screen))
							{
								std::string BuiltString = "Golf Cart";
								ImVec2 text_size = ImGui::CalcTextSize(BuiltString.c_str());

								if (ud::u_textoutline)
								{
									DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 255), false, true, BuiltString.c_str());
								}
								else
								{
									DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 255), false, false, BuiltString.c_str());
								}
							}
						}
					}
					if (ud::draw_vehicles && strstr(entity.Name.c_str(), _xor_("Rufus_ShoppingCartVehicleSK_C").c_str()))
					{
						if (entity.distance <= ud::vehiclesrender)
						{
							fvector Location = GetLocation(entity.Actor);
							fvector2d Screen = w2s(Location);
							if (in_screen(Screen))
							{
								std::string BuiltString = "Shopping Cart";
								ImVec2 text_size = ImGui::CalcTextSize(BuiltString.c_str());

								if (ud::u_textoutline)
								{
									DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 255), false, true, BuiltString.c_str());
								}
								else
								{
									DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 255), false, false, BuiltString.c_str());
								}
							}
						}
					}

					if (ud::draw_vehicles && strstr(entity.Name.c_str(), _xor_("Rufus_FerretVehicle_C").c_str()))
					{
						if (entity.distance <= ud::vehiclesrender)
						{
							fvector Location = GetLocation(entity.Actor);
							fvector2d Screen = w2s(Location);
							if (in_screen(Screen))
							{
								std::string BuiltString = "Plane";
								ImVec2 text_size = ImGui::CalcTextSize(BuiltString.c_str());

								if (ud::u_textoutline)
								{
									DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 255), false, true, BuiltString.c_str());
								}
								else
								{
									DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 255), false, false, BuiltString.c_str());
								}
							}
						}
					}
					if (ud::draw_vehicles && strstr(entity.Name.c_str(), _xor_("Rufus_AntelopeVehicle_C").c_str()))
					{
						if (entity.distance <= ud::vehiclesrender)
						{
							fvector Location = GetLocation(entity.Actor);
							fvector2d Screen = w2s(Location);
							if (in_screen(Screen))
							{
								std::string BuiltString = "Quad Crasher";
								ImVec2 text_size = ImGui::CalcTextSize(BuiltString.c_str());

								if (ud::u_textoutline)
								{
									DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 255), false, true, BuiltString.c_str());
								}
								else
								{
									DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 255), false, false, BuiltString.c_str());
								}
							}
						}
					}*/

					if (ud::draw_vehicles && strstr(entity.Name.c_str(), _xor_("Vehicle_Hoverboard_C").c_str()))
					{
						if (entity.distance <= ud::vehiclesrender)
						{
							fvector Location = GetLocation(entity.Actor);
							fvector2d Screen = w2s(Location);
							if (in_screen(Screen))
							{
								std::string BuiltString = "Hover Board";
								framework::vec2 text_size = framework::vec2(12, 12);//ImGui::CalcTextSize(BuiltString.c_str());

								if (ud::u_textoutline)
								{
									//DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, flinearcolor(255, 255, 255, 255), false, true, BuiltString.c_str());
									m_framework->text(
										BuiltString.c_str(),
										Screen.x - (text_size.x / 2),
										Screen.y - 18,
										flinearcolor(255, 255, 255, 255),
										text_size.x,
										false
									);
								}
								else
								{
									//DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, flinearcolor(255, 255, 255, 255), false, false, BuiltString.c_str());
									m_framework->text(
										BuiltString.c_str(),
										Screen.x - (text_size.x / 2),
										Screen.y - 18,
										flinearcolor(255, 255, 255, 255),
										text_size.x,
										false
									);
								}
							}
						}
					}

					if (ud::draw_vehicles && strstr(entity.Name.c_str(), _xor_("Valet_BasicSUV_Vehicle_C").c_str()))
					{
						if (entity.distance <= ud::vehiclesrender)
						{
							fvector Location = GetLocation(entity.Actor);
							fvector2d Screen = w2s(Location);
							if (in_screen(Screen))
							{
								std::string BuiltString = "SUV CAR";
								framework::vec2 text_size = framework::vec2(12, 12);//ImGui::CalcTextSize(BuiltString.c_str());

								if (ud::u_textoutline)
								{
									//DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, flinearcolor(255, 255, 255, 255), false, true, BuiltString.c_str());
									m_framework->text(
										BuiltString.c_str(),
										Screen.x - (text_size.x / 2),
										Screen.y - 18,
										flinearcolor(255, 255, 255, 255),
										text_size.x,
										false
									);
								}
								else
								{
									//DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, flinearcolor(255, 255, 255, 255), false, false, BuiltString.c_str());
									m_framework->text(
										BuiltString.c_str(),
										Screen.x - (text_size.x / 2),
										Screen.y - 18,
										flinearcolor(255, 255, 255, 255),
										text_size.x,
										false
									);
								}
							}
						}
					}

					if (ud::draw_vehicles && strstr(entity.Name.c_str(), _xor_("Valet_SportsCar_Vehicle_C").c_str()))
					{
						if (entity.distance <= ud::vehiclesrender)
						{
							fvector Location = GetLocation(entity.Actor);
							fvector2d Screen = w2s(Location);
							if (in_screen(Screen))
							{
								std::string BuiltString = "LAMBO CAR";
								framework::vec2 text_size = framework::vec2(12, 12);//ImGui::CalcTextSize(BuiltString.c_str());

								if (ud::u_textoutline)
								{
									m_framework->text(
										BuiltString.c_str(),
										Screen.x - (text_size.x / 2),
										Screen.y - 18,
										flinearcolor(255, 255, 255, 255),
										text_size.x,
										false
									);
									//DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, flinearcolor(255, 255, 255, 255), false, true, BuiltString.c_str());
								}
								else
								{
									m_framework->text(
										BuiltString.c_str(),
										Screen.x - (text_size.x / 2),
										Screen.y - 18,
										flinearcolor(255, 255, 255, 255),
										text_size.x,
										false
									);
									//DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, flinearcolor(255, 255, 255, 255), false, false, BuiltString.c_str());
								}
							}
						}
					}

					if (ud::draw_vehicles && strstr(entity.Name.c_str(), _xor_("Motorcycle_DirtBike_Vehicle_C").c_str()))
					{
						if (entity.distance <= ud::vehiclesrender)
						{
							fvector Location = GetLocation(entity.Actor);
							fvector2d Screen = w2s(Location);
							if (in_screen(Screen))
							{
								std::string BuiltString = "Dirtbike";
								framework::vec2 text_size = framework::vec2(12, 12);//ImGui::CalcTextSize(BuiltString.c_str());

								if (ud::u_textoutline)
								{
									m_framework->text(
										BuiltString.c_str(),
										Screen.x - (text_size.x / 2),
										Screen.y - 18,
										flinearcolor(255, 255, 255, 255),
										text_size.x,
										false
									);
									//DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, flinearcolor(255, 255, 255, 255), false, true, BuiltString.c_str());
								}
								else
								{
									m_framework->text(
										BuiltString.c_str(),
										Screen.x - (text_size.x / 2),
										Screen.y - 18,
										flinearcolor(255, 255, 255, 255),
										text_size.x,
										false
									);
									//DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, flinearcolor(255, 255, 255, 255), false, false, BuiltString.c_str());
								}
							}
						}
					}

					if (ud::lamma && strstr(entity.Name.c_str(), _xor_("AthenaSupplyDrop_Llama_C").c_str()))
					{
						if (entity.distance <= ud::lammarender)
						{
							fvector Location = GetLocation(entity.Actor);
							fvector2d Screen = w2s(Location);
							if (in_screen(Screen))
							{
								std::string BuiltString = "llama";

								framework::vec2 text_size = framework::vec2(12, 12);//ImGui::CalcTextSize(BuiltString.c_str());

								if (ud::u_textoutline)
								{
									m_framework->text(
										BuiltString.c_str(),
										Screen.x - (text_size.x / 2),
										Screen.y - 18,
										flinearcolor(255, 255, 255, 255),
										text_size.x,
										false
									);
									//DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, flinearcolor(255, 255, 0, 255), false, true, BuiltString.c_str());
								}
								else
								{
									m_framework->text(
										BuiltString.c_str(),
										Screen.x - (text_size.x / 2),
										Screen.y - 18,
										flinearcolor(255, 255, 255, 255),
										text_size.x,
										false
									);
									//DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, flinearcolor(255, 255, 0, 255), false, false, BuiltString.c_str());
								}
							}
						}
					}

					if (ud::pickups && strstr(entity.Name.c_str(), _xor_("FortPickupAthena").c_str()) || strstr(entity.Name.c_str(), _xor_("Fort_Pickup_Creative_C").c_str()))
					{

						//std::cout << "Item name: " << entity.Name.c_str() << std::endl;

						if (entity.distance <= ud::pickupsrender)
						{
							auto definition = driver.Read<uint64_t>(pid, entity.Actor + 0x350 + 0x18);
							if (driver.is_valid(definition))
							{
								BYTE tier = driver.Read<BYTE>(pid, definition + 0x12B);

								flinearcolor Color, RGBAColor;
								fvector Location = GetLocation(entity.Actor);

								fvector2d PickupPosition = w2s(Location);


								uint64_t ftext_ptr = driver.Read<uint64_t>(pid, definition + 0x30);

								if (driver.is_valid(ftext_ptr)) {
									uint64_t ftext_data = driver.Read<uint64_t>(pid, ftext_ptr + 0x28);
									int ftext_length = driver.Read<int>(pid, ftext_ptr + 0x30);
									if (ftext_length > 0 && ftext_length < 50) {
										wchar_t* ftext_buf = new wchar_t[ftext_length];
										driver.Read(pid, (PVOID)ftext_data, ftext_buf, ftext_length * sizeof(wchar_t));
										wchar_t* WeaponName = ftext_buf;
										delete[] ftext_buf;

										std::string Text = wchar_to_char(WeaponName);

										if (!ud::show_floor_ammo && Text.find("Ammo") != std::string::npos) {
											continue;
										}

										if (!ud::show_floor_materials && Text.find("Wood") != std::string::npos) {
											continue;
										}

										if (!ud::show_floor_materials && Text.find("Stone") != std::string::npos) {
											continue;
										}

										if (!ud::show_floor_materials && Text.find("Metal") != std::string::npos) {
											continue;
										}

										if (!ud::show_floor_gold && Text.find("Bars") != std::string::npos) {
											continue;
										}


										std::string wtf2 = Text + " [" + std::to_string((int)entity.distance) + ("m]");

										if (tier == 2 && (ud::Common))
										{
											Color = flinearcolor(0, 204, 34, 255);
										}
										else if ((tier == 3) && (ud::rare))
										{
											Color = flinearcolor(0, 112, 221, 255);
										}
										else if ((tier == 4) && (ud::purple))
										{
											Color = flinearcolor(163, 53, 238, 255);
										}
										else if ((tier == 5) && (ud::gold))
										{
											Color = flinearcolor(255, 128, 0, 255);
										}
										else if ((tier == 6) && (ud::mythic))
										{
											Color = flinearcolor(254, 202, 33, 255);
										}

										else if ((tier == 0) || (tier == 1) && (ud::UnCommon))
										{
											Color = flinearcolor(170, 165, 169, 255);
										}
										else if (tier != 0 || tier != 1, tier != 02, tier != 3, tier != 4, tier != 5, tier != 6)
										{
											Color = flinearcolor(255, 255, 255, 255);
										}

										framework::vec2 text_size = framework::vec2(12, 12);//ImGui::CalcTextSize(wtf2.c_str());

										if (ud::u_textoutline)
										{
											m_framework->text(
												wtf2.c_str(),
												PickupPosition.x - (text_size.x / 2),
												PickupPosition.y - 18,
												flinearcolor(255, 255, 255, 255),
												text_size.x,
												false
											);
											//DrawString(13, PickupPosition.x - (text_size.x / 2), PickupPosition.y - 18, Color, false, true, wtf2.c_str());
										}
										else
										{
											m_framework->text(
												wtf2.c_str(),
												PickupPosition.x - (text_size.x / 2),
												PickupPosition.y - 18,
												flinearcolor(255, 255, 255, 255),
												text_size.x,
												false
											);
											//DrawString(13, PickupPosition.x - (text_size.x / 2), PickupPosition.y - 18, Color, false, false, wtf2.c_str());
										}

									}
								}
							}
						}
					}



					//if (ud::draw_chests && strstr(entity.Name.c_str(), _xor_("Tiered_Chest_Athena_C").c_str()))
					//{
					//	
					//	//if (entity.distance <= ud::chestrender)
					//	if (true)
					//	{
					//		auto alr_searched = driver.Read<uintptr_t>(pid, entity.Actor + 0xD92);
					//		fvector Location = GetLocation(entity.Actor);
					//		fvector2d Screen = w2s(Location);
					//		if (in_screen(Screen))
					//		{

					//			if (!alr_searched)
					//			{
					//				std::string BuiltString = "Chest";
					//				ImVec2 text_size = ImGui::CalcTextSize(BuiltString.c_str());

					//				if (ud::u_textoutline)
					//				{
					//					DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 0), false, true, BuiltString.c_str());
					//				}
					//				else
					//				{
					//					DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 0), false, false, BuiltString.c_str());
					//				}
					//			}

					//		}
					//	}
					//}

					if (ud::draw_ammobox && strstr(entity.Name.c_str(), _xor_("Tiered_Ammo_Athena_C").c_str()))
					{

						if (entity.distance <= ud::ammoboxrender)
						{
							auto alr_searched = driver.Read<uintptr_t>(pid, entity.Actor + 0xD92);
							fvector Location = GetLocation(entity.Actor);
							fvector2d Screen = w2s(Location);
							if (in_screen(Screen))
							{

								if (!alr_searched)
								{
									std::string BuiltString = "Ammo Box";
									framework::vec2 text_size = framework::vec2(12, 12);//ImGui::CalcTextSize(BuiltString.c_str());

									if (ud::u_textoutline)
									{
										m_framework->text(
											BuiltString.c_str(),
											Screen.x - (text_size.x / 2),
											Screen.y - 18,
											flinearcolor(255, 255, 255, 255),
											text_size.x,
											false
										);
										//DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, flinearcolor(255, 255, 0, 255), false, true, BuiltString.c_str());
									}
									else
									{
										m_framework->text(
											BuiltString.c_str(),
											Screen.x - (text_size.x / 2),
											Screen.y - 18,
											flinearcolor(255, 255, 255, 255),
											text_size.x,
											false
										);
										//DrawString(13, Screen.x - (text_size.x / 2), Screen.y - 18, flinearcolor(255, 255, 0, 255), false, false, BuiltString.c_str());
									}
								}

							}
						}
					}

				}
			}
		}

		fvector2d head;
		fvector2d root;
		fvector2d pelvis;
		fvector2d left_shoulder;
		fvector2d left_elbow;
		fvector2d left_hand;
		fvector2d right_shoulder;
		fvector2d neck;
		fvector2d left_hip;
		fvector2d right_hip;

		fvector2d right_elbow;
		fvector2d right_hand;
		fvector2d left_knee;
		fvector2d left_foot;
		fvector2d left_toes;
		fvector2d right_knee;
		fvector2d right_foot;
		fvector2d right_toes;


		auto cache_entities() -> void {

			temporary_entity_list.clear();

			ud_cache->uworld = driver.Read<uintptr_t>(pid, virtualaddy + offset::uworld);
			ud_cache->game_instance = driver.Read<uintptr_t>(pid, ud_cache->uworld + offset::game_instance);
			ud_cache->local_player = driver.Read<uintptr_t>(pid, driver.Read<uintptr_t>(pid, ud_cache->game_instance + offset::local_player));
			ud_cache->player_controller = driver.Read<uintptr_t>(pid, ud_cache->local_player + offset::player_controller);
			ud_cache->acknowledged_pawn = driver.Read<uintptr_t>(pid, ud_cache->player_controller + offset::acknowledged_pawn);
			ud_cache->skeletal_mesh = driver.Read<uintptr_t>(pid, ud_cache->acknowledged_pawn + offset::skeletal_mesh);
			ud_cache->player_state = driver.Read<uintptr_t>(pid, ud_cache->acknowledged_pawn + offset::player_state);
			ud_cache->root_component = driver.Read<uintptr_t>(pid, ud_cache->acknowledged_pawn + offset::root_component);
			ud_cache->relative_location = driver.Read<fvector>(pid, ud_cache->root_component + offset::relative_location);
			ud_cache->team_index = driver.Read<int>(pid, ud_cache->player_state + offset::team_index);

			if (ud::dev)
			{
				std::cout << E(" Uworld -> ") << ud_cache->uworld << std::endl;
				std::cout << E(" game_instance -> ") << ud_cache->game_instance << std::endl;
				std::cout << E(" local_player -> ") << ud_cache->local_player << std::endl;
				std::cout << E(" player_controller -> ") << ud_cache->player_controller << std::endl;
				std::cout << E(" acknowledged_pawn -> ") << ud_cache->acknowledged_pawn << std::endl;
				std::cout << E(" skeletal_mesh -> ") << ud_cache->skeletal_mesh << std::endl;
				std::cout << E(" player_state -> ") << ud_cache->player_state << std::endl;
				std::cout << E(" root_component -> ") << ud_cache->root_component << std::endl;
				std::cout << E(" relative_location.X -> ") << ud_cache->relative_location.x << std::endl;
				std::cout << E(" relative_location.Y -> ") << ud_cache->relative_location.y << std::endl;
				std::cout << E(" relative_location.Z -> ") << ud_cache->relative_location.z << std::endl;
				std::cout << E(" team_index -> ") << ud_cache->team_index << std::endl;
			}

			if (!ud_cache->acknowledged_pawn)
			{
				ud::in_lobby = true;
			}
			else
			{
				ud::in_lobby = false;
			}

			if (ud_cache->acknowledged_pawn) {

				if (ud::u_weapon_cfg)
				{
					uint64_t player_weapon = driver.Read<uint64_t>(pid, ud_cache->acknowledged_pawn + 0xa20); // 	struct AFortWeapon* CurrentWeapon;

					if (driver.is_valid(player_weapon)) {

						uint64_t weapon_data = driver.Read<uint64_t>(pid, player_weapon + 0x4d0);	//struct UFortWeaponItemDefinition* WeaponData;

						if (driver.is_valid(weapon_data)) {

							uint64_t ftext_ptr = driver.Read<uint64_t>(pid, weapon_data + 0x38);

							if (driver.is_valid(ftext_ptr)) {
								uint64_t ftext_data = driver.Read<uint64_t>(pid, ftext_ptr + 0x28);
								int ftext_length = driver.Read<int>(pid, ftext_ptr + 0x30);
								if (ftext_length > 0 && ftext_length < 50) {
									wchar_t* ftext_buf = new wchar_t[ftext_length];
									driver.Read(pid, (PVOID)ftext_data, ftext_buf, ftext_length * sizeof(wchar_t));
									wchar_t* WeaponName = ftext_buf;
									delete[] ftext_buf;

									if (wcsstr(WeaponName, E(L"Assault Rifle")) || wcsstr(WeaponName, E(L"Havoc Suppressed Assault Rifle")) || wcsstr(WeaponName, E(L"Red-Eye Assault Rifle"))
										|| wcsstr(WeaponName, E(L"Suppressed Assault Rifle")) || wcsstr(WeaponName, E(L"Striker Assault Rifle")) || wcsstr(WeaponName, E(L"Burst Assault Rifle"))
										|| wcsstr(WeaponName, E(L"Ranger Assault Rifle")) || wcsstr(WeaponName, E(L"Flapjack Rifle")) || wcsstr(WeaponName, E(L"Heavy Assault Rifle"))
										|| wcsstr(WeaponName, E(L"MK-Seven Assault Rifle")) || wcsstr(WeaponName, E(L"MK-Alpha Assault Rifle")) || wcsstr(WeaponName, E(L"Enforcer AR"))
										|| wcsstr(WeaponName, E(L"Nemesis AR")) || wcsstr(WeaponName, E(L"Hammer Assault Rifle")) || wcsstr(WeaponName, E(L"Sideways Rifle")) || wcsstr(WeaponName, E(L"Striker AR"))) {
										HeldWeaponType = EFortWeaponType::Rifle;
										ud::u_aimbot = rifle::aimbot;
										//ud::u_visible_check = rifle::visible_check;
										ud::u_aimfov = rifle::aim_fov;
										ud::u_smooth = rifle::smooth;
										ud::u_hitbox = rifle::hitbox;
										// render shit
										//aimkey = rifle::aimkey;
										if (ud::develop_mode_weapons)
										{
											std::cout << E("RIFLE HELD") << std::endl;
										}
									}
									if (wcsstr(WeaponName, E(L"Shotgun")) || wcsstr(WeaponName, E(L"Hammer Pump Shotgun")) || wcsstr(WeaponName, E(L"Frenzy Auto Shotgun")) || wcsstr(WeaponName, E(L"Burst Assault Rifle"))) {

										HeldWeaponType = EFortWeaponType::Shotgun;
										ud::u_aimbot = shotgun::aimbot;
										//ud::u_visible_check = shotgun::visible_check;
										ud::u_aimfov = shotgun::aim_fov;
										ud::u_smooth = shotgun::smooth;
										ud::u_hitbox = shotgun::hitbox;
										//aimkey = shotgun::aimkey;
										if (ud::develop_mode_weapons)
										{
											std::cout << E("SHOTGUN HELD") << std::endl;
										}
									}
									if (wcsstr(WeaponName, E(L"Smg")) || wcsstr(WeaponName, E(L"Submachine Gun")) || wcsstr(WeaponName, E(L"Thunder Burst SMG")) || wcsstr(WeaponName, E(L"Ranger Pistol")) || wcsstr(WeaponName, E(L"Hyper SMG"))) {
										HeldWeaponType = EFortWeaponType::Smg;
										ud::u_aimbot = smg::aimbot;
										//ud::u_visible_check = smg::visible_check;
										ud::u_aimfov = smg::aim_fov;
										ud::u_smooth = smg::smooth;
										ud::u_hitbox = smg::hitbox;
										//aimkey = smg::aimkey;
										if (ud::develop_mode_weapons)
										{
											std::cout << E("SMG HELD") << std::endl;
										}
									}
									if (wcsstr(WeaponName, E(L"Hunting Rifle")) || wcsstr(WeaponName, E(L"Heavy Sniper Rifle")) || wcsstr(WeaponName, E(L"Suppressed Sniper Rifle"))
										|| wcsstr(WeaponName, E(L"Storm Scout")) || wcsstr(WeaponName, E(L"Bolt-Action Sniper Rifle")) || wcsstr(WeaponName, E(L"Automatic Sniper Rifle"))
										|| wcsstr(WeaponName, E(L"Reaper Sniper Rifle")) || wcsstr(WeaponName, E(L"Thermal DMR")) || wcsstr(WeaponName, E(L"Hunter Bolt-Action Sniper"))
										|| wcsstr(WeaponName, E(L"Crossbow")) || wcsstr(WeaponName, E(L"Mechanical Bow"))) {
										HeldWeaponType = EFortWeaponType::Sniper;
										ud::u_aimbot = sniper::aimbot;
										//ud::u_visible_check = sniper::visible_check;
										ud::u_aimfov = sniper::aim_fov;
										ud::u_smooth = sniper::smooth;
										ud::u_hitbox = sniper::hitbox;
										//aimkey = sniper::aimkey;
										if (ud::develop_mode_weapons)
										{
											std::cout << E("SNIPER HELD") << std::endl;
										}
									}
								}
							}
						}
					}
				}
			}

			ud_cache->game_state = driver.Read<uintptr_t>(pid, ud_cache->uworld + offset::game_state);
			ud_cache->player_array = driver.Read<uintptr_t>(pid, ud_cache->game_state + offset::player_array);
			ud_cache->player_array_size = driver.Read<int>(pid, ud_cache->game_state + (offset::player_array + sizeof(uintptr_t)));

			for (int i = 0; i < ud_cache->player_array_size; ++i) {
				auto player_state = driver.Read<uintptr_t>(pid, ud_cache->player_array + (i * sizeof(uintptr_t)));
				auto current_actor = driver.Read<uintptr_t>(pid, player_state + offset::pawn_private);

				auto root_component = driver.Read<uintptr_t>(pid, current_actor + offset::root_component);

				auto relative_location = driver.Read<fvector>(pid, root_component + offset::relative_location);


				fvector2d screen_location;
				screen_location = w2s(relative_location);
				if (!in_screen(screen_location)) continue;

				auto skeletalmesh = driver.Read<uintptr_t>(pid, current_actor + offset::skeletal_mesh);

				auto is_despawning = (driver.Read<char>(pid, current_actor + 0x758) >> 3);

				auto team_index = driver.Read<int>(pid, player_state + offset::team_index);

				auto pawn_private = driver.Read<uintptr_t>(pid, player_state + 0x308);

				if (ud::b_is_dying_lol) {
					bool bIsDyingasd = (driver.Read<BYTE>(pid, pawn_private + 0x758) & 0x10);
					if (bIsDyingasd)
						continue;
				}
				if (ud::u_ignore_dead) {
					bool isDBNO = (driver.Read<BYTE>(pid, pawn_private + 0x93a) >> 4) & 1;
					if (isDBNO)
						continue;
				}

				if (ud_cache->team_index == team_index && ud_cache->acknowledged_pawn) continue;

				entity cached_actors{ };
				cached_actors.entity = current_actor;
				cached_actors.skeletal_mesh = skeletalmesh;
				cached_actors.root_component = root_component;
				cached_actors.relative_location = relative_location;
				cached_actors.player_state = player_state;
				cached_actors.team_index = team_index;



				temporary_entity_list.push_back(cached_actors);
			}

			entity_list.clear();
			entity_list = temporary_entity_list;
			std::this_thread::sleep_for(std::chrono::milliseconds(150));

		}

		fvector2d head_box;
		fvector2d root_box;
		fvector2d root_box1;


		auto actor_loop() -> void {
			float target_dist = FLT_MAX;
			uintptr_t target_entity = 0;

			char fpsinfo[256];
			//m_framework->text(fpsinfo, 15, 15, flinearcolor(255, 255, 255, 255), 12, false);
			if (ud::u_debug)
			{
				char gworld_pointer_debug[256];
				sprintf_s(gworld_pointer_debug, ("Gworld -> %d"), ud_cache->uworld);
				m_framework->text(gworld_pointer_debug, 15, 30, flinearcolor(255, 255, 255, 255), 12, false);

				char gameinstance_debug[256];
				sprintf_s(gameinstance_debug, ("Game Instance -> %d"), ud_cache->game_instance);
				m_framework->text(gameinstance_debug, 15, 45, flinearcolor(255, 255, 255, 255), 12, false);

				char localplayer_debug[256];
				sprintf_s(localplayer_debug, ("Local Player -> %d"), ud_cache->local_player);
				m_framework->text(localplayer_debug, 15, 60, flinearcolor(255, 255, 255, 255), 12, false);

				char playerstate_debug[256];
				sprintf_s(playerstate_debug, ("Player State -> %d"), ud_cache->player_state);
				m_framework->text(playerstate_debug, 15, 75, flinearcolor(255, 255, 255, 255), 12, false);

				char rootcomp_debug[256];
				sprintf_s(rootcomp_debug, ("Root Component -> %d"), ud_cache->root_component);
				m_framework->text(rootcomp_debug, 15, 90, flinearcolor(255, 255, 255, 255), 12, false);

				char teamindex_debug[256];
				sprintf_s(teamindex_debug, ("Team Index -> %d"), ud_cache->team_index);
				m_framework->text(teamindex_debug, 15, 105, flinearcolor(255, 255, 255, 255), 12, false);

				char skeletalmesh_debug[256];
				sprintf_s(skeletalmesh_debug, ("Mesh -> %d"), ud_cache->skeletal_mesh);
				m_framework->text(skeletalmesh_debug, 15, 120, flinearcolor(255, 255, 255, 255), 12, false);

				char relalocationx_debug[256];
				sprintf_s(relalocationx_debug, ("Relative Location.X -> %d"), ud_cache->relative_location.x);
				m_framework->text(relalocationx_debug, 15, 135, flinearcolor(255, 255, 255, 255), 12, false);

				char relalocationy_debug[256];
				sprintf_s(relalocationy_debug, ("Relative Location.Y -> %d"), ud_cache->relative_location.y);
				m_framework->text(relalocationy_debug, 15, 150, flinearcolor(255, 255, 255, 255), 12, false);

				char prcoess_id_debug[256];
				sprintf_s(prcoess_id_debug, ("Process ID  -> %d"), pid);
				m_framework->text(prcoess_id_debug, 15, 180, flinearcolor(255, 255, 255, 255), 12, false);

				char baseaddy_debug[256];
				sprintf_s(baseaddy_debug, ("Base Address  -> %d"), virtualaddy);
				m_framework->text(baseaddy_debug, 15, 195, flinearcolor(255, 255, 255, 255), 12, false);
				//sprintf_s(text_ammo_count2, ("[Levels: %d]"), leveltest);
				//sprintf_s(text_ammo_count3, ("[Ammo: %d]"), global->WeaponInfo.ammo_count);i wo
			}

			const float centerWidth = screen_width / 2;
			const float centerHeight = screen_height / 2;

			for (auto& cached : entity_list) {
				auto root_bone = get_bone_3d(cached.skeletal_mesh, EBoneIndex::Root);
				root = w2s(root_bone);

				root_box = w2s(fvector(root_bone.x, root_bone.y, root_bone.z + 30));

				root_box1 = w2s(fvector(root_bone.x, root_bone.y, root_bone.z - 15));

				auto head_bone = get_bone_3d(cached.skeletal_mesh, EBoneIndex::Head);
				head = w2s(head_bone);
				head_box = w2s(fvector(head_bone.x, head_bone.y, head_bone.z + 15));

				float box_height = abs(head.y - root_box1.y);
				float box_width = box_height * 0.50f;
				if (ud_cache->acknowledged_pawn)
				{
					if (!ud_cache->relative_location.x)
					{
						//std::cout << "in car\n";
					}
				}
				float distance = ud_cache->relative_location.distance(root_bone) / 100;

				

				//if (distance > ud::u_render_distance && ud_cache->acknowledged_pawn) continue;

				if (ud::u_aimbot) {

					auto dx = head.x - (screen_width / 2);
					auto dy = head.y - (screen_height / 2);
					auto dist = sqrtf(dx * dx + dy * dy);

					if (ud::u_visible_check) {
						if (is_visible(cached.skeletal_mesh)) {
							if (dist < ud::u_aimfov && dist < target_dist) {

								target_dist = dist;
								target_entity = cached.entity;
							}
						}
					}
					else {
						if (dist < ud::u_aimfov && dist < target_dist) {

							target_dist = dist;
							target_entity = cached.entity;
						}
					}
				}

				flinearcolor vis;
				flinearcolor invis;
				flinearcolor normal;

				vis = flinearcolor( ud::u_color_Visible.r, ud::u_color_Visible.g, ud::u_color_Visible.b, 1.0f );
				invis = flinearcolor( ud::u_color_invisible.r, ud::u_color_invisible.g, ud::u_color_invisible.b, 1.0f );
				normal = flinearcolor(1.0f, ud::u_color[0], ud::u_color[1], ud::u_color[2]);
				flinearcolor name_index(255, 255, 255, 255);

				flinearcolor visibleColor;
				if (ud::u_visible_check)
				{
					visibleColor = is_visible(cached.skeletal_mesh) ? flinearcolor(ud::u_color_Visible) : flinearcolor(ud::u_color_invisible);
				}
				else
				{
					visibleColor = flinearcolor(normal);
				}

				if (ud::knocked_esp)
				{
					auto pawn_private = driver.Read<uintptr_t>(pid, cached.player_state + 0x308);
					bool isDBNO = (driver.Read<BYTE>(pid, pawn_private + 0x93a) >> 4) & 1;

					if (isDBNO)
					{
						visibleColor = flinearcolor(ud::u_color_dbno);
					}
				}

				if (ud::color_names_by_team_index)
				{
					switch (cached.team_index) {
					case 1:   name_index = indexcolor::color1;   break;
					case 2:   name_index = indexcolor::color2;   break;
					case 3:   name_index = indexcolor::color3;   break;
					case 4:   name_index = indexcolor::color4;   break;
					case 5:   name_index = indexcolor::color5;   break;
					case 6:   name_index = indexcolor::color6;   break;
					case 7:   name_index = indexcolor::color7;   break;
					case 8:   name_index = indexcolor::color8;   break;
					case 9:   name_index = indexcolor::color9;   break;
					case 10:  name_index = indexcolor::color10;  break;
					case 11:  name_index = indexcolor::color11;  break;
					case 12:  name_index = indexcolor::color12;  break;
					case 13:  name_index = indexcolor::color13;  break;
					case 14:  name_index = indexcolor::color14;  break;
					case 15:  name_index = indexcolor::color15;  break;
					case 16:  name_index = indexcolor::color16;  break;
					case 17:  name_index = indexcolor::color17;  break;
					case 18:  name_index = indexcolor::color18;  break;
					case 19:  name_index = indexcolor::color19;  break;
					case 20:  name_index = indexcolor::color20;  break;
					case 21:  name_index = indexcolor::color21;  break;
					case 22:  name_index = indexcolor::color22;  break;
					case 23:  name_index = indexcolor::color23;  break;
					case 24:  name_index = indexcolor::color24;  break;
					case 25:  name_index = indexcolor::color25;  break;
					case 26:  name_index = indexcolor::color26;  break;
					case 27:  name_index = indexcolor::color27;  break;
					case 28:  name_index = indexcolor::color28;  break;
					case 29:  name_index = indexcolor::color29;  break;
					case 30:  name_index = indexcolor::color30;  break;
					case 31:  name_index = indexcolor::color31;  break;
					case 32:  name_index = indexcolor::color32;  break;
					case 33:  name_index = indexcolor::color33;  break;
					case 34:  name_index = indexcolor::color34;  break;
					case 35:  name_index = indexcolor::color35;  break;
					case 36:  name_index = indexcolor::color36;  break;
					case 37:  name_index = indexcolor::color37;  break;
					case 38:  name_index = indexcolor::color38;  break;
					case 39:  name_index = indexcolor::color39;  break;
					case 40:  name_index = indexcolor::color40;  break;
					case 41:  name_index = indexcolor::color41;  break;
					case 42:  name_index = indexcolor::color42;  break;
					case 43:  name_index = indexcolor::color43;  break;
					case 44:  name_index = indexcolor::color44;  break;
					case 45:  name_index = indexcolor::color45;  break;
					case 46:  name_index = indexcolor::color46;  break;
					case 47:  name_index = indexcolor::color47;  break;
					case 48:  name_index = indexcolor::color48;  break;
					case 49:  name_index = indexcolor::color49;  break;
					case 50:  name_index = indexcolor::color50;  break;
					case 51:  name_index = indexcolor::color51;  break;
					case 52:  name_index = indexcolor::color52;  break;
					case 53:  name_index = indexcolor::color53;  break;
					case 54:  name_index = indexcolor::color54;  break;
					case 55:  name_index = indexcolor::color55;  break;
					case 56:  name_index = indexcolor::color56;  break;
					case 57:  name_index = indexcolor::color57;  break;
					case 58:  name_index = indexcolor::color58;  break;
					case 59:  name_index = indexcolor::color59;  break;
					case 60:  name_index = indexcolor::color60;  break;
					case 61:  name_index = indexcolor::color61;  break;
					case 62:  name_index = indexcolor::color62;  break;
					case 63:  name_index = indexcolor::color63;  break;
					case 64:  name_index = indexcolor::color64;  break;
					case 65:  name_index = indexcolor::color65;  break;
					case 66:  name_index = indexcolor::color66;  break;
					case 67:  name_index = indexcolor::color67;  break;
					case 68:  name_index = indexcolor::color68;  break;
					case 69:  name_index = indexcolor::color69;  break;
					case 70:  name_index = indexcolor::color70;  break;
					case 71:  name_index = indexcolor::color71;  break;
					case 72:  name_index = indexcolor::color72;  break;
					case 73:  name_index = indexcolor::color73;  break;
					case 74:  name_index = indexcolor::color74;  break;
					case 75:  name_index = indexcolor::color75;  break;
					case 76:  name_index = indexcolor::color76;  break;
					case 77:  name_index = indexcolor::color77;  break;
					case 78:  name_index = indexcolor::color78;  break;
					case 79:  name_index = indexcolor::color79;  break;
					case 80:  name_index = indexcolor::color80;  break;
					case 81:  name_index = indexcolor::color81;  break;
					case 82:  name_index = indexcolor::color82;  break;
					case 83:  name_index = indexcolor::color83;  break;
					case 84:  name_index = indexcolor::color84;  break;
					case 85:  name_index = indexcolor::color85;  break;
					case 86:  name_index = indexcolor::color86;  break;
					case 87:  name_index = indexcolor::color87;  break;
					case 88:  name_index = indexcolor::color88;  break;
					case 89:  name_index = indexcolor::color89;  break;
					case 90:  name_index = indexcolor::color90;  break;
					case 91:  name_index = indexcolor::color91;  break;
					case 92:  name_index = indexcolor::color92;  break;
					case 93:  name_index = indexcolor::color93;  break;
					case 94:  name_index = indexcolor::color94;  break;
					case 95:  name_index = indexcolor::color95;  break;
					case 96:  name_index = indexcolor::color96;  break;
					case 97:  name_index = indexcolor::color97;  break;
					case 98:  name_index = indexcolor::color98;  break;
					case 99:  name_index = indexcolor::color99;  break;
					case 100: name_index = indexcolor::color100; break;
					default:  name_index = flinearcolor(255, 255, 255, 255); break;
					}
				}



				if (ud::u_ignore_downed)
				{
					auto pawn_private = driver.Read<uintptr_t>(pid, cached.player_state + 0x308);
					bool isDBNO = (driver.Read<BYTE>(pid, pawn_private + 0x93a) >> 4) & 1;

					if (isDBNO)
					{
						ud::downed_player_found = 1;
					}
				}

				if (ud::esp_auto_thick)
						{
							if (distance <= 2)
							{
								ud::u_skeleton_thick = 7.5;
								ud::u_box_thick = 7.5;
							}
							else if (distance <= 5)
							{
								ud::u_skeleton_thick = 6.5;
								ud::u_box_thick = 6.5;
							}
							else if (distance <= 8)
							{
								ud::u_skeleton_thick = 5.8;
								ud::u_box_thick = 5.8;
							}
							else if (distance <= 10)
							{
								ud::u_skeleton_thick = 5.2;
								ud::u_box_thick = 5.2;
							}
							else if (distance <= 15)
							{
								ud::u_skeleton_thick = 4.3;
								ud::u_box_thick = 4.3;
							}
							else if (distance <= 20)
							{
								ud::u_skeleton_thick = 3.2;
								ud::u_box_thick = 3.2;
							}
							else if (distance <= 30)
							{
								ud::u_skeleton_thick = 2.5;
								ud::u_box_thick = 2.5;
							}
							else if (distance <= 50)
							{
								ud::u_skeleton_thick = 2;
								ud::u_box_thick = 2;
							}
							else if (distance <= 90)
							{
								ud::u_skeleton_thick = 1.3;
								ud::u_box_thick = 1.3;
							}

							else if (distance >= 90)
							{
								ud::u_skeleton_thick = 1;
								ud::u_box_thick = 1;
							}
						}

				


				if (ud::u_enable_esp && ud::u_line)
				{
					if (ud::u_line_type == 0) {
						//draw_list->AddLine(ImVec2(screen_width / 2, 0), ImVec2(head_box.x, head_box.y), visibleColor, ud::u_line_thick);
						m_framework->line(screen_width / 2, 0, head_box.x, head_box.y, visibleColor, ud::u_line_thick);
					}

					if (ud::u_line_type == 1) {
						//draw_list->AddLine(ImVec2(screen_width / 2, screen_height / 2), ImVec2(head_box.x, head_box.y), visibleColor, ud::u_line_thick);
						m_framework->line(screen_width / 2, screen_height / 2, head_box.x, head_box.y, visibleColor, ud::u_line_thick);
					}

					if (ud::u_line_type == 2) {
						//draw_list->AddLine(ImVec2(screen_width / 2, screen_height - 5), ImVec2(head_box.x, head_box.y), visibleColor, ud::u_line_thick);
						m_framework->line(screen_width / 2, screen_height - 5, head_box.x, head_box.y, visibleColor, ud::u_line_thick);
					}
				}

				

				if (ud::u_box_esp && ud::u_enable_esp) {

					if (ud::u_box_type == 0)
					{

						if (ud::u_outline)
						{
							m_framework->line(head_box.x - (box_width / 2), head_box.y, root.x + (box_width / 2), head_box.y, flinearcolor(0, 0, 0, 255), ud::u_box_thick + 2);
							m_framework->line(head_box.x - (box_width / 2), head_box.y, head_box.x - (box_width / 2), root.y, flinearcolor(0, 0, 0, 255), ud::u_box_thick + 2);
							m_framework->line(head_box.x - (box_width / 2), root.y, root.x + (box_width / 2), root.y, flinearcolor(0, 0, 0, 255), ud::u_box_thick + 2);
							m_framework->line(root.x + (box_width / 2), head_box.y, root.x + (box_width / 2), root.y, flinearcolor(0, 0, 0, 255), ud::u_box_thick + 2);

						}

						m_framework->line(head_box.x - (box_width / 2), head_box.y, root.x + (box_width / 2), head_box.y, visibleColor, ud::u_box_thick);
						m_framework->line(head_box.x - (box_width / 2), head_box.y, head_box.x - (box_width / 2), root.y, visibleColor, ud::u_box_thick);
						m_framework->line(head_box.x - (box_width / 2), root.y, root.x + (box_width / 2), root.y, visibleColor, ud::u_box_thick);
						m_framework->line(root.x + (box_width / 2), head_box.y, root.x + (box_width / 2), root.y, visibleColor, ud::u_box_thick);
						//DrawBox(head_box.x - (CornerWidth / 2), head_box.y, CornerWidth, CornerHeight, visibleColor, ud::u_box_thick);
					}
					else if (ud::u_box_type == 1)
					{
						if (ud::u_outline)
						{
							m_framework->rect(head_box.x - (box_width / 2), head_box.y, head_box.x - (box_width / 2) + box_width, head_box.y + box_height, flinearcolor(0, 0, 0, 255), ud::u_box_thick + 2.0);
						}
						// 50% opacity
						m_framework->filled_rect(head_box.x - (box_width / 2), head_box.y, box_width, box_height, flinearcolor(visibleColor.r, visibleColor.g, visibleColor.b, 0.5f), false, 0.0f, 0.0f);
					}
					else if (ud::u_box_type == 2)
					{
						if (ud::u_outline)
						{
							float outlineThickness = ud::u_box_thick + 2.0;
							m_framework->line(root.x - (box_width / 2), head_box.y, root.x - (box_width / 2) + box_width, head_box.y, flinearcolor(0, 0, 0, 255), outlineThickness);
							m_framework->line(root.x - (box_width / 2), head_box.y + box_height, root.x - (box_width / 2) + box_width, head_box.y + box_height, flinearcolor(0, 0, 0, 255), outlineThickness);
							m_framework->line(root.x - (box_width / 2), head_box.y, root.x - (box_width / 2), head_box.y + box_height, flinearcolor(0, 0, 0, 255), outlineThickness);
							m_framework->line(root.x - (box_width / 2) + box_width, head_box.y, root.x - (box_width / 2) + box_width, head_box.y + box_height, flinearcolor(0, 0, 0, 255), outlineThickness);
						}

						m_framework->rect(root.x - (box_width / 2), head_box.y, box_width, box_height, visibleColor, ud::u_box_thick);
					}

				}

				if (ud::u_skeleton && ud::u_enable_esp) {
					head = w2s(get_bone_3d(cached.skeletal_mesh, 68));
					neck = w2s(get_bone_3d(cached.skeletal_mesh, EBoneIndex::Neck));
					left_shoulder = w2s(get_bone_3d(cached.skeletal_mesh, EBoneIndex::LShoulder));
					left_elbow = w2s(get_bone_3d(cached.skeletal_mesh, EBoneIndex::LElbow));
					left_hand = w2s(get_bone_3d(cached.skeletal_mesh, EBoneIndex::LHand));
					right_shoulder = w2s(get_bone_3d(cached.skeletal_mesh, EBoneIndex::RShoulder));
					right_elbow = w2s(get_bone_3d(cached.skeletal_mesh, EBoneIndex::RElbow));
					right_hand = w2s(get_bone_3d(cached.skeletal_mesh, EBoneIndex::RHand));
					pelvis = w2s(get_bone_3d(cached.skeletal_mesh, EBoneIndex::Pelvis));
					left_hip = w2s(get_bone_3d(cached.skeletal_mesh, 71));
					left_knee = w2s(get_bone_3d(cached.skeletal_mesh, EBoneIndex::LKnee));
					left_foot = w2s(get_bone_3d(cached.skeletal_mesh, EBoneIndex::LFoot));
					right_hip = w2s(get_bone_3d(cached.skeletal_mesh, 78));
					right_knee = w2s(get_bone_3d(cached.skeletal_mesh, EBoneIndex::RKnee));
					right_foot = w2s(get_bone_3d(cached.skeletal_mesh, EBoneIndex::RFoot));
					right_toes = w2s(get_bone_3d(cached.skeletal_mesh, EBoneIndex::RToes));
					left_toes = w2s(get_bone_3d(cached.skeletal_mesh, EBoneIndex::LToes));

					if (ud::u_skeleton_type == 0)
					{
						if (ud::u_outline)
						{
							m_framework->line(left_shoulder.x, left_shoulder.y, right_shoulder.x, right_shoulder.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
							m_framework->line(left_shoulder.x, left_shoulder.y, left_elbow.x, left_elbow.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
							m_framework->line(left_elbow.x, left_elbow.y, left_hand.x, left_hand.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
							m_framework->line(right_shoulder.x, right_shoulder.y, right_elbow.x, right_elbow.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
							m_framework->line(right_elbow.x, right_elbow.y, right_hand.x, right_hand.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
							m_framework->line(neck.x, neck.y, pelvis.x, pelvis.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
							m_framework->line(pelvis.x, pelvis.y, left_hip.x, left_hip.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
							m_framework->line(left_hip.x, left_hip.y, left_knee.x, left_knee.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
							m_framework->line(left_knee.x, left_knee.y, left_foot.x, left_foot.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
							m_framework->line(pelvis.x, pelvis.y, right_hip.x, right_hip.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
							m_framework->line(right_hip.x, right_hip.y, right_knee.x, right_knee.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
							m_framework->line(right_knee.x, right_knee.y, right_foot.x, right_foot.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
						}
						
						m_framework->line(left_shoulder.x, left_shoulder.y, right_shoulder.x, right_shoulder.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(left_shoulder.x, left_shoulder.y, left_elbow.x, left_elbow.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(left_elbow.x, left_elbow.y, left_hand.x, left_hand.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(right_shoulder.x, right_shoulder.y, right_elbow.x, right_elbow.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(right_elbow.x, right_elbow.y, right_hand.x, right_hand.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(neck.x, neck.y, pelvis.x, pelvis.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(neck.x, neck.y, head.x, head.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(pelvis.x, pelvis.y, left_hip.x, left_hip.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(left_hip.x, left_hip.y, left_knee.x, left_knee.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(left_knee.x, left_knee.y, left_foot.x, left_foot.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(left_foot.x, left_foot.y, left_toes.x, left_toes.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(pelvis.x, pelvis.y, right_hip.x, right_hip.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(right_hip.x, right_hip.y, right_knee.x, right_knee.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(right_knee.x, right_knee.y, right_foot.x, right_foot.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(right_foot.x, right_foot.y, right_toes.x, right_toes.y, visibleColor, ud::u_skeleton_thick);
					}


					if (ud::u_skeleton_type == 1)
					{
						if (ud::u_outline)
						{
							m_framework->line(neck.x, neck.y, pelvis.x, pelvis.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
							m_framework->line(neck.x, neck.y, neck.x, neck.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
							m_framework->line(neck.x, neck.y, left_elbow.x, left_elbow.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
							m_framework->line(left_elbow.x, left_elbow.y, left_hand.x, left_hand.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
							m_framework->line(neck.x, neck.y, right_elbow.x, right_elbow.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
							m_framework->line(right_elbow.x, right_elbow.y, right_hand.x, right_hand.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
							m_framework->line(pelvis.x, pelvis.y, left_knee.x, left_knee.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
							m_framework->line(left_knee.x, left_knee.y, left_foot.x, left_foot.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
							m_framework->line(pelvis.x, pelvis.y, right_knee.x, right_knee.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
							m_framework->line(right_knee.x, right_knee.y, right_foot.x, right_foot.y, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2.5);
						}
						m_framework->line(neck.x, neck.y, pelvis.x, pelvis.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(neck.x, neck.y, neck.x, neck.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(neck.x, neck.y, left_elbow.x, left_elbow.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(left_elbow.x, left_elbow.y, left_hand.x, left_hand.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(neck.x, neck.y, right_elbow.x, right_elbow.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(right_elbow.x, right_elbow.y, right_hand.x, right_hand.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(pelvis.x, pelvis.y, left_knee.x, left_knee.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(left_knee.x, left_knee.y, left_foot.x, left_foot.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(pelvis.x, pelvis.y, right_knee.x, right_knee.y, visibleColor, ud::u_skeleton_thick);
						m_framework->line(right_knee.x, right_knee.y, right_foot.x, right_foot.y, visibleColor, ud::u_skeleton_thick);
					}

				}
				
				if (ud::enemy_counter)
				{
					if (true)
					{
						int player_array = ud_cache->player_array_size;
						int visible = is_visible(cached.entity);
						std::string player_num = std::to_string(player_array);
						std::string visible_num = std::to_string(visible);

						m_framework->text("Enemies Around: " + player_num + " | Visible: " + visible_num, screen_width / 2, 22, flinearcolor(255, 255, 255, 255), 16, true, true);
					}
				}

				if (ud::u_weapon_esp && ud::u_enable_esp)
				{
					if (ud_cache->acknowledged_pawn)
					{

						WeaponInformation held_weapon{ };

						uint64_t player_weapon = driver.Read<uint64_t>(pid, cached.entity + 0xa20);

						if (driver.is_valid(player_weapon)) {
							held_weapon.ammo_count = driver.Read<int32_t>(pid, player_weapon + 0xe54);
							uint64_t weapon_data = driver.Read<uint64_t>(pid, player_weapon + 0x4f0);

							if (driver.is_valid(weapon_data)) {
								held_weapon.tier = driver.Read<BYTE>(pid, weapon_data + 0x13b);
								held_weapon.reloading_weapon = driver.Read<bool>(pid, player_weapon + 0x388);
								uint64_t ftext_ptr = driver.Read<uint64_t>(pid, weapon_data + 0x38);

								if (driver.is_valid(ftext_ptr)) {
									uint64_t ftext_data = driver.Read<uint64_t>(pid, ftext_ptr + 0x28);
									int ftext_length = driver.Read<int>(pid, ftext_ptr + 0x30);
									if (ftext_length > 0 && ftext_length < 50) {
										wchar_t* ftext_buf = new wchar_t[ftext_length];

										driver.Read(pid, (PVOID)ftext_data, ftext_buf, ftext_length * sizeof(wchar_t));
										std::wstring wstr_buf(ftext_buf);
										held_weapon.weapon_name = std::string(wstr_buf.begin(), wstr_buf.end());
										delete[] ftext_buf;
									}
								}

							}
							

							WeaponInfo = held_weapon;

							std::string final = WeaponInfo.weapon_name;

							if (ud::show_weapon_esp_ammo_count)
							{
								if (!(strstr(WeaponInfo.weapon_name.c_str(), E("Pickaxe")) != nullptr))
									final = WeaponInfo.weapon_name + std::to_string(held_weapon.ammo_count) + " rounds";
							}
							if (ud::show_reloads)
							{
								if (held_weapon.reloading_weapon)
								{
									final = "RELOADING";
								}
							}

							framework::vec2 TextSize = framework::vec2(16, 16);//ImGui::CalcTextSize(final.c_str());

							framework::vec2 text_size = framework::vec2(16, 16);//ImGui::CalcTextSize(final.c_str());
							int someMultiplier = 3;
							int xOffset = static_cast<int>(final.length() * someMultiplier);
							int add;
							if (ud::u_distance)
							{
								add = 25;
							}
							else
							{
								add = 5;
							}

							//if (held_weapon.reloading_weapon)
							//{
							//	DrawString(16, (root.x) - (TextSize.x / 2), (root.y + add), ImColor(255, 0, 0), false, true, final.c_str());
							//	return;
							//}

							if (ud::u_weapon_rarity)
							{
								if (WeaponInfo.tier == 0)
								{
									if (strstr(WeaponInfo.weapon_name.c_str(), E("Pickaxe")) != nullptr)
									{
										m_framework->text(
											final.c_str(),
											root.x,
											root.y + 50,
											flinearcolor(255, 255, 255, 255),
											TextSize.x,
											true,
											true
										);
									}
									else
									{
										std::string fina1l = ("Building Plan");
										m_framework->text(
											fina1l.c_str(),
											//root.x -  42,
											root.x,
											root.y + 50,
											flinearcolor(255, 255, 255, 255),
											TextSize.x,
											true,
											true
										);
									}
								}
								if (WeaponInfo.tier == 1)
								{
									m_framework->text(
										final.c_str(),
										root.x,
										root.y + 50,
										flinearcolor(255, 170, 165, 169),
										TextSize.x,
										true,
										true
									);
								}

								if (WeaponInfo.tier == 2)
								{
									m_framework->text(
										final.c_str(),
										root.x,
										root.y + 50,
										flinearcolor(255, 0, 204, 34),
										TextSize.x,
										true,
										true
									);
								}
								if (WeaponInfo.tier == 3)
								{
									m_framework->text(
										final.c_str(),
										root.x,
										root.y + 50,
										flinearcolor(255, 0, 112, 221),
										TextSize.x,
										true,
										true
									);
								}
								if (WeaponInfo.tier == 4)
								{
									m_framework->text(
										final.c_str(),
										root.x,
										root.y + 50,
										flinearcolor(255, 163, 53, 238),
										TextSize.x,
										true,
										true
									);
								}
								if (WeaponInfo.tier == 5)
								{
									m_framework->text(
										final.c_str(),
										root.x,
										root.y + 50,
										flinearcolor(255, 255, 128, 0),
										TextSize.x,
										true,
										true
									);
								}
								if (WeaponInfo.tier == 6)
								{
									m_framework->text(
										final.c_str(),
										root.x,
										root.y + 50,
										flinearcolor(255, 254, 202, 33),
										TextSize.x,
										true,
										true
									);
								}
							}
							else
							{
								if (WeaponInfo.tier == 0)
								{
									if (strstr(WeaponInfo.weapon_name.c_str(), E("Pickaxe")) != nullptr)
									{
										m_framework->text(
											final.c_str(),
											root.x,
											root.y + 50,
											visibleColor,
											TextSize.x,
											true,
											true
										);
									}
									else
									{
										std::string fina1l = ("Building Plan");
										framework::vec2 TextSiz1e = framework::vec2(12, 12); // Adjust the size accordingly
										m_framework->text(
											fina1l.c_str(),
											root.x,
											root.y + 50,
											visibleColor,
											TextSize.x,
											true,
											true
										);
									}
								}
								else
								{
									m_framework->text(
										final.c_str(),
										root.x,
										root.y + 50,
										visibleColor,
										TextSize.x,
										true,
										true
									);
								}
							}
						}

					}
				}


				if (ud::u_view_angle && ud::u_enable_esp) {
					fvector angle = driver.Read<fvector>(pid, cached.root_component + offset::relative_rotation);
					angle.x = (angle.x) * 3.14159265 / 180;
					angle.y = (angle.y) * 3.14159265 / 180;
					angle.z = (angle.z) * 3.14159265 / 180;

					float
						sinYaw = sin(angle.y),
						cosYaw = cos(angle.y),
						sinPitch = sin(angle.x),
						cosPitch = cos(angle.x);

					fvector direction;
					direction.x = -cosPitch * cosYaw;
					direction.y = -cosPitch * sinYaw;
					direction.z = -sinPitch;

					fvector
						trail(direction.x * -100.f, direction.y * -100.f, direction.z * -100.f),
						headPosition = get_bone_3d(cached.skeletal_mesh, EBoneIndex::Head),
						trailEnd = headPosition + direction * -100.f;
					fvector2d screenTrail;
					screenTrail = w2s(trailEnd);

					m_framework->line(head.x, head.y, screenTrail.x, screenTrail.y, visibleColor, ud::u_view_angle_thick);
				}

				if (ud::u_username && ud::u_enable_esp)
				{
					if (ud_cache->acknowledged_pawn)
					{
						std::string username_str = decrypt_player_name(cached.player_state);
						std::string final;
						if (ud::attach_team_id)
						{
							std::string team_id_str = std::to_string(cached.team_index);

							final = username_str + " [" + team_id_str + "]";
						}
						else
						{
							final = username_str;
						}

						if (runpreview)
						{
							std::string local_user_buffer = decrypt_player_name(ud_cache->player_state);
							ud::local_username = local_user_buffer;
							runpreview = false;
						}

						framework::vec2 text_size = framework::vec2(14, 14);//ImGui::CalcTextSize(username_str.c_str());
						int someMultiplier = 3;
						int xOffset = static_cast<int>(username_str.length() * someMultiplier);

						if (ud::color_names_by_team_index)
						{
							m_framework->text(
								final.c_str(),
								head_box.x,
								head_box.y - 10,
								name_index,
								16,
								true,
								true
							);
						}
					}
					else
					{
						if (runpreview)
						{
							ud::local_username = "player name";
						}
					}
				}


				if (ud::u_enable_esp && playerstats::playerkills) {

					if (ud_cache->acknowledged_pawn)
					{

					}

				}


				if (ud::u_enable_esp && ud::wireframe) {

					if (ud_cache->acknowledged_pawn)
					{
						//auto PawnMaterials = driver.Read<std::vector<uintptr_t>>(pid, ud_cache->player_state + 0x5C40);
						//for (int i = 0; i < PawnMaterials.size(); i++)
						//{
						//	auto MaterialInstance = PawnMaterials[i];

						//	auto MaterialInterface = driver.Read<uintptr_t>(pid, MaterialInstance + 0x110);

						//	auto Material = driver.Read<uintptr_t>(pid, MaterialInstance + 0x18);

						//	driver.WriteBit(pid, Material + 0x1b8, 6); //Wireframe is the 6th index
						//}
					}

				}

				
				

				if (ud::platform_esp && ud::u_enable_esp) {
					if (ud_cache->acknowledged_pawn)
					{
						

						WCHAR buffer[0x30];

						driver.Read_Array(pid, driver.Read<uintptr_t>(pid, ud_cache->player_state + 0x438), buffer, sizeof(buffer));

						std::wstring ws = buffer;

						std::string PlatformName = std::string(ws.begin(), ws.end());

						flinearcolor PlatformColor;
						framework::vec2 DistanceTextSize2 = framework::vec2(12, 12);//ImGui::CalcTextSize(PlatformName.c_str());
						framework::vec2 text_size = framework::vec2(12, 12);//ImGui::CalcTextSize(PlatformName.c_str());

						if (strstr(PlatformName.c_str(), ("WIN")))
						{
							PlatformColor = flinearcolor(255, 255, 255, 255);

							m_framework->text(
								("[Platform: PC]"),
								head_box.x - (text_size.x / 2),
								head_box.y,
								PlatformColor,
								16,
								false
							);
						}
						else if (strstr(PlatformName.c_str(), ("PSN")) || strstr(PlatformName.c_str(), ("PS5")))
						{
							PlatformColor = flinearcolor(0, 108, 199, 255);

							m_framework->text(
								("[Platform: PSN]"),
								head_box.x - (text_size.x / 2),
								head_box.y,
								PlatformColor,
								16,
								false
							);
						}
						else if (strstr(PlatformName.c_str(), ("XBL")) || strstr(PlatformName.c_str(), ("XSX")))
						{
							PlatformColor = flinearcolor(18, 120, 16, 255);

							m_framework->text(
								("[Platform: XBOX]"),
								head_box.x - (text_size.x / 2),
								head_box.y,
								PlatformColor,
								16,
								false
							);
						}
						else if (strstr(PlatformName.c_str(), ("SWT")))
						{
							PlatformColor = flinearcolor(223, 0, 17, 255);

							m_framework->text(
								("[Platform: SWITCH]"),
								head_box.x - (text_size.x / 2),
								head_box.y,
								PlatformColor,
								16,
								false
							);
						}
						else if (strstr(PlatformName.c_str(), ("AND")) || strstr(PlatformName.c_str(), ("IOS")))
						{
							PlatformColor = flinearcolor(255, 255, 255, 255);

							m_framework->text(
								("[Platform: MOBILE]"),
								head_box.x - (text_size.x / 2),
								head_box.y,
								PlatformColor,
								16,
								false
							);
						}
						else
						{
							PlatformColor = flinearcolor(255, 255, 255, 255);

							m_framework->text(
								PlatformName.c_str(),
								head_box.x - (text_size.x / 2),
								head_box.y,
								PlatformColor,
								16,
								false
							);
						}

					

					}
				}


				if (ud::u_enable_esp && ud::u_head)
				{
					if (!ud_cache->acknowledged_pawn)
					{
						fvector2d head_2d = w2s(fvector(head_bone.x, head_bone.y, head_bone.z + 20));

						fvector delta = head_bone - camera_postion.location;
						float distance = delta.length();

						const float constant_circle_size = ud::u_head_size;

						float circle_radius = constant_circle_size * (screen_height / (2.0f * distance * tanf(camera_postion.fov * (float)M_PI / 360.f)));

						float y_offset = +97.0f;
						head_2d.y += y_offset;

						int segments = 50;
						float thickness = 2.0f;

						if (ud::u_outline)
						{
							m_framework->circle(head_2d.x, head_2d.y, circle_radius, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2, false);
						}
						m_framework->circle(head_2d.x, head_2d.y, circle_radius, visibleColor, ud::u_skeleton_thick, false);
					}
					else
					{
						fvector2d head_2d = w2s(fvector(head_bone.x, head_bone.y, head_bone.z));

						fvector delta = head_bone - camera_postion.location;
						float distance = delta.length();

						const float constant_circle_size = ud::u_head_size;

						float circle_radius = constant_circle_size * (screen_height / (2.0f * distance * tanf(camera_postion.fov * (float)M_PI / 360.f)));

						float y_offset = +12.0f;

						int segments = 50;
						float thickness = 2.0f;

						if (ud::u_outline)
						{
							m_framework->circle(head_2d.x, head_2d.y, circle_radius, flinearcolor(0, 0, 0, 255), ud::u_skeleton_thick + 2, false);
						}
						m_framework->circle(head_2d.x, head_2d.y, circle_radius, visibleColor, ud::u_skeleton_thick, false);
					}

				}
				if (ud::u_distance && ud::u_enable_esp) {
					std::string distance_str;
					if (distance < 300)
					{
						distance_str = std::to_string((int)distance) + (std::string)("m");
					}
					else
					{
						if (ud_cache->acknowledged_pawn)
						{
							distance_str = "in car";
						}
						else
						{
							distance_str = std::to_string((int)distance) + (std::string)("m");
						}
					}
					
					framework::vec2 text_size = framework::vec2(12, 12);//ImGui::CalcTextSize(distance_str.c_str());
					if (ud::u_white_text_color)
					{
						m_framework->text(distance_str.c_str(), root.x - 18, root.y + 5, flinearcolor(255, 255, 255, 255), 16, false, true);
						
					}
					else
					{
						m_framework->text(distance_str.c_str(), root.x - 18, root.y + 5, visibleColor, 16, false, true);
						
					}

					if (ud::close_aimbot)
					{
						if (ud::u_aimfov > 300 && !distance < ud::close_aimbot_distance)
						{
							ud::u_aimfov = ud::u_close_aim_buffer_2;
						}
						if (ud::u_aimfov < 301)
						{
							ud::u_close_aim_buffer = ud::u_aimfov;
							ud::u_close_aim_buffer_2 = ud::u_close_aim_buffer;
						}
						if (distance < ud::close_aimbot_distance)
						{
							auto closest_mesh = driver.Read<uint64_t>(pid, target_entity + offset::skeletal_mesh);
							fvector hitbox = get_bone_3d(closest_mesh, bones::HumanChest);
							fvector2d hitbox_screen = w2s(hitbox);
							if ((hitbox.x != 0 || hitbox.y != 0) && (get_cross_distance(hitbox.x, hitbox.y, screen_width / 2, screen_height / 2) <= ud::u_aimfov))
							{
								m_framework->text("RETARD NIGGA NEAR", root.x - (text_size.x / 2) - 130, head.y - 110, flinearcolor(255, 15, 15, 255), 40, false);
								ud::u_aimfov = ud::u_aimfov_when_close_aim_active;
							}
							else
							{
								ud::u_aimfov = ud::u_close_aim_buffer;
							}
						}

					}

				}

				

				//if (ud::infinite_fuel)
				//{
				//	uint64_t CurrentVehicle = driver.Read<float>(pid, ud_cache->acknowledged_pawn + 0x28B8);
				//	auto FuelComponent = driver.Read<uintptr_t>(pid, (uintptr_t)CurrentVehicle + 0x1230); //FortAthenaVehicle::CachedFuelComponent 0x1230

				//	driver.Write<float>(pid, FuelComponent + 0xf0, 20000.f);
				//}

				//if (ud::no_recoil)
				//{
				//	driver.Write<float>(pid, ud_cache->player_controller + 0x64, -1);
				//}

				//if (ud::ADS_when_jumping)
				//{

				//	driver.Write<bool>(pid, ud_cache->acknowledged_pawn + 0xD80, true); // AFortWeapon::bAllowTargeting -> 0xD80
				//}
				//else
				//{

				//	driver.Write<bool>(pid, ud_cache->acknowledged_pawn + 0xD80, false);
				//}

				//if (ud::ADS_when_reloading)
				//{
				//	if (ud_cache->acknowledged_pawn)
				//	{
				//		driver.Write<bool>(pid, ud_cache->acknowledged_pawn + 0xaff, true); // UFortWeaponItemDefinition::bAllowTargetingDuringReload
				//	}
				//	
				//}
				//else
				//{

				//	driver.Write<bool>(pid, ud_cache->acknowledged_pawn + 0xaff, false);
				//}
				

				if (ud::u_triggerbot) {

					auto closest_mesh_tb = driver.Read<std::uint64_t>(pid, target_entity + offset::skeletal_mesh);
					fvector hitbox_tb;
					fvector2d hitbox_screen_tb;

					if (ud::shotgun_only)
					{
						uint64_t player_weapon = driver.Read<uint64_t>(pid, ud_cache->acknowledged_pawn + 0xa20); // 	struct AFortWeapon* CurrentWeapon;

						if (driver.is_valid(player_weapon)) {

							uint64_t weapon_data = driver.Read<uint64_t>(pid, player_weapon + 0x4f0);	//struct UFortWeaponItemDefinition* WeaponData;

							if (driver.is_valid(weapon_data)) {

								uint64_t ftext_ptr = driver.Read<uint64_t>(pid, weapon_data + 0x38);

								if (driver.is_valid(ftext_ptr)) {
									uint64_t ftext_data = driver.Read<uint64_t>(pid, ftext_ptr + 0x28);
									int ftext_length = driver.Read<int>(pid, ftext_ptr + 0x30);
									if (ftext_length > 0 && ftext_length < 50) {
										wchar_t* ftext_buf = new wchar_t[ftext_length];
										driver.Read(pid, (PVOID)ftext_data, ftext_buf, ftext_length * sizeof(wchar_t));
										wchar_t* WeaponName = ftext_buf;
										delete[] ftext_buf;

										if (wcsstr(WeaponName, E(L"Shotgun")))
										{
											if (ud::hold_key_for_tb)
											{
												if (GetAsyncKeyState_Spoofed(ud::TBkey))
												{
													if (ud::u_auto_aim_when_tb)
													{
														hitbox_tb = get_bone_3d(closest_mesh_tb, EBoneIndex::Head);
														hitbox_screen_tb = w2s(get_bone_3d(closest_mesh_tb, EBoneIndex::Head));
														if (hitbox_tb.x != 0 || hitbox_tb.y != 0 && (get_cross_distance(hitbox_tb.x, hitbox_tb.y, screen_width / 2, screen_height / 2) <= ud::u_aimfov))
															u_mouse->move_mouse(hitbox_screen_tb);
													}

													m_framework->text("Triggerbot Active", screen_width / 2, 40, flinearcolor(127, 0, 255, 255), 20, false);
													if (distance < ud::u_maximum_distance) {

														auto targeted_fort_pawn = driver.Read<std::uintptr_t>(pid, ud_cache->player_controller + 0x1890); // AFortPlayerController	TargetedFortPawn	AFortPawn*
														auto target_state = driver.Read<std::uintptr_t>(pid, targeted_fort_pawn + offset::player_state);
														auto target_team = driver.Read<int>(pid, target_state + offset::team_index);
														if ((ud_cache->acknowledged_pawn) and (targeted_fort_pawn != 0) and (target_team != ud_cache->team_index)) {
															if (u_has_clicked) {
																u_tb_begin = std::chrono::steady_clock::now();
																u_has_clicked = 0;
															}
															u_tb_end = std::chrono::steady_clock::now();
															u_tb_time_since = std::chrono::duration_cast<std::chrono::milliseconds>(u_tb_end - u_tb_begin).count();
															if (u_tb_time_since >= ud::u_custom_delay) {
																u_mouse->left_down();
																u_mouse->left_up();
																u_has_clicked = 1;
															}
														}
													}
												}
											}

											if (ud::toggle_key_for_tb)
											{
												if (GetAsyncKeyState_Spoofed(ud::TBkey))
												{
													toggled_tb = !toggled_tb;
												}

												if (toggled_tb)
												{
													if (ud::u_auto_aim_when_tb)
													{
														hitbox_tb = get_bone_3d(closest_mesh_tb, EBoneIndex::Head);
														hitbox_screen_tb = w2s(get_bone_3d(closest_mesh_tb, EBoneIndex::Head));
														if (hitbox_tb.x != 0 || hitbox_tb.y != 0 && (get_cross_distance(hitbox_tb.x, hitbox_tb.y, screen_width / 2, screen_height / 2) <= ud::u_aimfov))
															u_mouse->move_mouse(hitbox_screen_tb);
													}

													m_framework->text("Triggerbot Active", screen_width / 2, 40, flinearcolor(127, 0, 255, 255), 20, false);
													if (distance < ud::u_maximum_distance) {

														auto targeted_fort_pawn = driver.Read<std::uintptr_t>(pid, ud_cache->player_controller + 0x1890); // AFortPlayerController	TargetedFortPawn	AFortPawn*
														auto target_state = driver.Read<std::uintptr_t>(pid, targeted_fort_pawn + offset::player_state);
														auto target_team = driver.Read<int>(pid, target_state + offset::team_index);
														if ((ud_cache->acknowledged_pawn) and (targeted_fort_pawn != 0) and (target_team != ud_cache->team_index)) {
															if (u_has_clicked) {
																u_tb_begin = std::chrono::steady_clock::now();
																u_has_clicked = 0;
															}
															u_tb_end = std::chrono::steady_clock::now();
															u_tb_time_since = std::chrono::duration_cast<std::chrono::milliseconds>(u_tb_end - u_tb_begin).count();
															if (u_tb_time_since >= ud::u_custom_delay) {
																u_mouse->left_down();
																u_mouse->left_up();
																u_has_clicked = 1;
															}
														}
													}
												}
											}
										}
										else
										{
											continue;
										}
									}
								}
							}
						}
					}

					if (ud::hold_key_for_tb)
					{
						if (GetAsyncKeyState_Spoofed(ud::TBkey))
						{
							if (ud::u_auto_aim_when_tb)
							{
								hitbox_tb = get_bone_3d(closest_mesh_tb, EBoneIndex::Head);
								hitbox_screen_tb = w2s(get_bone_3d(closest_mesh_tb, EBoneIndex::Head));
								if (hitbox_tb.x != 0 || hitbox_tb.y != 0 && (get_cross_distance(hitbox_tb.x, hitbox_tb.y, screen_width / 2, screen_height / 2) <= ud::u_aimfov))
									u_mouse->move_mouse(hitbox_screen_tb);
							}

							m_framework->text("Triggerbot Active", screen_width / 2, 40, flinearcolor(127, 0, 255, 255), 20, false);
							if (distance < ud::u_maximum_distance) {

								auto targeted_fort_pawn = driver.Read<std::uintptr_t>(pid, ud_cache->player_controller + 0x1890); // AFortPlayerController	TargetedFortPawn	AFortPawn*
								auto target_state = driver.Read<std::uintptr_t>(pid, targeted_fort_pawn + offset::player_state);
								auto target_team = driver.Read<int>(pid, target_state + offset::team_index);
								if ((ud_cache->acknowledged_pawn) and (targeted_fort_pawn != 0) and (target_team != ud_cache->team_index)) {
									if (u_has_clicked) {
										u_tb_begin = std::chrono::steady_clock::now();
										u_has_clicked = 0;
									}
									u_tb_end = std::chrono::steady_clock::now();
									u_tb_time_since = std::chrono::duration_cast<std::chrono::milliseconds>(u_tb_end - u_tb_begin).count();
									if (u_tb_time_since >= ud::u_custom_delay) {
										u_mouse->left_down();
										u_mouse->left_up();
										u_has_clicked = 1;
									}
								}
							}
						}
					}

					if (ud::toggle_key_for_tb)
					{
						if (GetAsyncKeyState_Spoofed(ud::TBkey))
						{
							toggled_tb = !toggled_tb;
						}

						if (toggled_tb)
						{
							if (ud::u_auto_aim_when_tb)
							{
								hitbox_tb = get_bone_3d(closest_mesh_tb, EBoneIndex::Head);
								hitbox_screen_tb = w2s(get_bone_3d(closest_mesh_tb, EBoneIndex::Head));
								if (hitbox_tb.x != 0 || hitbox_tb.y != 0 && (get_cross_distance(hitbox_tb.x, hitbox_tb.y, screen_width / 2, screen_height / 2) <= ud::u_aimfov))
									u_mouse->move_mouse(hitbox_screen_tb);
							}

							m_framework->text("Triggerbot Active", screen_width / 2, 40, flinearcolor(127, 0, 255, 255), 20, false);
							if (distance < ud::u_maximum_distance) {

								auto targeted_fort_pawn = driver.Read<std::uintptr_t>(pid, ud_cache->player_controller + 0x1890); // AFortPlayerController	TargetedFortPawn	AFortPawn*
								auto target_state = driver.Read<std::uintptr_t>(pid, targeted_fort_pawn + offset::player_state);
								auto target_team = driver.Read<int>(pid, target_state + offset::team_index);
								if ((ud_cache->acknowledged_pawn) and (targeted_fort_pawn != 0) and (target_team != ud_cache->team_index)) {
									if (u_has_clicked) {
										u_tb_begin = std::chrono::steady_clock::now();
										u_has_clicked = 0;
									}
									u_tb_end = std::chrono::steady_clock::now();
									u_tb_time_since = std::chrono::duration_cast<std::chrono::milliseconds>(u_tb_end - u_tb_begin).count();
									if (u_tb_time_since >= ud::u_custom_delay) {
										u_mouse->left_down();
										u_mouse->left_up();
										u_has_clicked = 1;
									}
								}
							}
						}
					}
				}
			}

			/*if (ud::u_prediction) {

				if (target_entity && ud::u_aimbot) {

					auto closest_mesh = driver.Read<uint64_t>(pid, target_entity + offset::mesh);
					auto CurrentWeapon = driver.Read<uint64_t>(pid, ud_cache->acknowledged_pawn + 0xa20);
					auto ProjectileSpeed = 6000.f;//driver.Read<float>(pid, CurrentWeapon + 0x174);

					std::cout << "ProjectileSpeed -> " << ProjectileSpeed << std::endl;

					fvector hitbox = get_bone_3d(closest_mesh, bones::HumanHead);
					fvector2d hitbox_screen = w2s(hitbox);


					if (hitbox.x != 0 || hitbox.y != 0 && (get_cross_distance(hitbox.x, hitbox.y, screen_width / 2, screen_height / 2) <= ud::u_aimfov))
					{
						{
							float distance = ud_cache->relative_location.distance(hitbox);
							std::cout << "Distance -> " << distance << std::endl;
							uint64_t CurrentActorRootComponent = driver.Read<uint64_t>(pid, closest_mesh + 0x198);
							fvector Velocity = driver.Read<fvector>(pid, CurrentActorRootComponent + 0x168);

							fvector CalculatedPosition = hitbox;
							if (ProjectileSpeed) //shitty check to see if its a projectile weapon
							{
								float TimeToTarget = distance / fabs(ProjectileSpeed);
								std::cout << "TimeToTarget -> " << TimeToTarget << std::endl;
								float BulletDrop = (fabs(-336) / 250) * TimeToTarget * TimeToTarget; //-336 is the gravity for most weapons
								std::cout << "Bullet Drop -> " << BulletDrop << std::endl;
								CalculatedPosition + fvector(TimeToTarget * Velocity.x, TimeToTarget * Velocity.y, BulletDrop * 120 + TimeToTarget * Velocity.z);
							}

							if (GetAsyncKeyState_Spoofed(ud::u_aim_key))
								u_mouse->move_mouse(w2s(CalculatedPosition));
						}
					}

				}
			}*/

			auto CurrentWeapon = driver.Read<uint64_t>(pid, ud_cache->acknowledged_pawn + 0xa20);

			uint64_t player_weapon = driver.Read<uint64_t>(pid, ud_cache->acknowledged_pawn + 0xa20); // 	struct AFortWeapon* CurrentWeapon;

			if (driver.is_valid(player_weapon)) {

				uint64_t weapon_data = driver.Read<uint64_t>(pid, player_weapon + 0x4f0);	//struct UFortWeaponItemDefinition* WeaponData;

				if (driver.is_valid(weapon_data)) {

					uint64_t ftext_ptr = driver.Read<uint64_t>(pid, weapon_data + 0x38);

					if (driver.is_valid(ftext_ptr)) {
						uint64_t ftext_data = driver.Read<uint64_t>(pid, ftext_ptr + 0x28);
						int ftext_length = driver.Read<int>(pid, ftext_ptr + 0x30);
						if (ftext_length > 0 && ftext_length < 50) {
							wchar_t* ftext_buf = new wchar_t[ftext_length];
							driver.Read(pid, (PVOID)ftext_data, ftext_buf, ftext_length * sizeof(wchar_t));
							wchar_t* WeaponName = ftext_buf;
							delete[] ftext_buf;

							if (wcsstr(WeaponName, E(L"Dragon's Breath Sniper")) || wcsstr(WeaponName, E(L"Storm Scout")) || wcsstr(WeaponName, E(L"Storm Scout Sniper Rifle"))
								|| wcsstr(WeaponName, E(L"Hunting Rifle")) || wcsstr(WeaponName, E(L"Explosive Repeater Rifle")) || wcsstr(WeaponName, E(L"Bolt-Action Sniper Rifle"))
								|| wcsstr(WeaponName, E(L"Suppressed Sniper Rifle")) || wcsstr(WeaponName, E(L"Lever Action Rifle")) || wcsstr(WeaponName, E(L"Sniper"))) {

								ud::projectileSpeed = 30000.f;
								ud::projectileGravityScale = 0.12f;
								ud::u_prediction = true;
							}
							else if (wcsstr(WeaponName, E(L"Heavy Sniper Rifle")) || wcsstr(WeaponName, E(L"Hunter Bolt-Action Sniper"))) {
								ud::projectileSpeed = 45000.f;
								ud::projectileGravityScale = 0.12f;
								ud::u_prediction = true;
							}
							else if (wcsstr(WeaponName, E(L"Cobra DMR")) || wcsstr(WeaponName, E(L"DMR")) || wcsstr(WeaponName, E(L"Thermal DMR"))) {
								ud::projectileSpeed = 53000.f;
								ud::projectileGravityScale = 0.15f;
								ud::u_prediction = true;
							}
							else if (wcsstr(WeaponName, E(L"Automatic Sniper Rifle"))) {
								ud::projectileSpeed = 50000.f;
								ud::projectileGravityScale = 0.12f;
								ud::u_prediction = true;
							}
							else if (wcsstr(WeaponName, E(L"Reaper Sniper Rifle")))
							{
								ud::projectileSpeed = 62000;
								ud::projectileGravityScale = 4.3;
								ud::u_prediction = true;
							}
							else if (wcsstr(WeaponName, E(L"Nemesis AR")) && !ud::sniperonlyprediction)
							{
								ud::projectileSpeed = 80000;
								ud::projectileGravityScale = 3.8;
								ud::u_prediction = true;
							}
							else if (wcsstr(WeaponName, E(L"Striker AR")) && !ud::sniperonlyprediction)
							{
								ud::projectileSpeed = 80000;
								ud::projectileGravityScale = 3.8;
								ud::u_prediction = true;
							}
							else if (wcsstr(WeaponName, E(L"SMG")) && !ud::sniperonlyprediction)
							{
								ud::projectileSpeed = 70000;
								ud::projectileGravityScale = 3;
								ud::u_prediction = true;
							}
							else if (wcsstr(WeaponName, E(L"Pistol")) && !ud::sniperonlyprediction)
							{
								ud::projectileSpeed = 64000;
								ud::projectileGravityScale = 2;
								ud::u_prediction = true;
							}
							else if (wcsstr(WeaponName, E(L"AR")) && !ud::sniperonlyprediction)
							{
								ud::projectileSpeed = 80000;
								ud::projectileGravityScale = 3.8;
								ud::u_prediction = true;
							}
							else {
								ud::projectileSpeed = 0;
								ud::projectileGravityScale = 0;
								ud::u_prediction = false;
							}
							int bone;
							if (ud::projectileSpeed && !ud::disable_prediction) {
								auto closest_mesh = driver.Read<uint64_t>(pid, target_entity + offset::skeletal_mesh);

								fvector hitbox = get_bone_3d(closest_mesh, bones::HumanHead);

								float Distance = ud_cache->relative_location.distance(hitbox);

								auto test = driver.Read<uintptr_t>(pid, target_entity + offset::root_component);
								fvector Velocity = driver.Read<fvector>(pid, test + 0x168);

								hitbox = PredictLocation(hitbox, Velocity, ud::projectileSpeed, ud::projectileGravityScale, Distance);

								fvector2d hitbox_screen = w2s(hitbox);

								//std::cout << "projectile speed: " << ud::projectileSpeed << std::endl;
								//std::cout << "projectile gravity scale: " << ud::projectileGravityScale << std::endl;
								//std::wcout << L"projectile name: " << WeaponName << std::endl;

								if (hitbox.x != 0 || hitbox.y != 0 && (get_cross_distance(hitbox.x, hitbox.y, screen_width / 2, screen_height / 2) <= ud::u_aimfov))
								{
									if (GetAsyncKeyState_Spoofed(ud::aimkey))
										if (ud::enable_mouse_aimbot)
										{
											u_mouse->move_mouse(hitbox_screen);
										}
									if (GetAsyncKeyState_Spoofed(ud::aimkey))
										if (ud::enable_memory_aimbot)
										{
											RotateCamera(hitbox);
										}
									//std::cout << "proj aim" << std::endl;
								}

							}
						}
					}
				}
			}

			//if (true)
			//{
			//	if (true)
			//	{
			//		fvector hitbox;
			//		fvector2d hitbox_screen;

			//		auto closest_mesh = driver.Read<std::uint64_t>(pid, target_entity + offset::skeletal_mesh);
			//		hitbox_screen = w2s(get_bone_3d(closest_mesh, EBoneIndex::Head));
			//		hitbox = get_bone_3d(closest_mesh, EBoneIndex::Head);
			//		// Class FortniteGame.FortWeaponRanged
			//		uint64_t CurrentActorWeapon = driver.Read<uint64_t>(pid, ud_cache->acknowledged_pawn + 0xA20);
			//		uintptr_t CurrentActorPM = driver.Read<uintptr_t>(pid, ud_cache->player_controller + 0x348);
			//		uintptr_t ViewYawMin = 0x244c;
			//		uintptr_t ViewYawMax = 0x2450;
			//		uintptr_t AimPitchMin = 0x17a8;
			//		uintptr_t AimPitchMax = 0x17ac;

			//		if (hitbox.x != 0 || hitbox.y != 0)
			//		{
			//			if (CurrentActorWeapon && CurrentActorPM)
			//			{
			//				if (GetAsyncKeyState_Spoofed(VK_CAPITAL))
			//				{
			//					fvector NewRotation = CalculateAngle(camera_postion.location, hitbox);

			//					static float OrginalPitchMin = driver.Read<float>(pid, CurrentActorPM + ViewYawMin);
			//					static float OrginalPitchMax = driver.Read<float>(pid, CurrentActorPM + ViewYawMax);

			//					driver.Write<float>(pid, CurrentActorWeapon + AimPitchMin, NewRotation.x);
			//					driver.Write<float>(pid, CurrentActorWeapon + AimPitchMax, NewRotation.x);

			//					driver.Write<float>(pid, CurrentActorPM + ViewYawMin, NewRotation.y);
			//					driver.Write<float>(pid, CurrentActorPM + ViewYawMax, NewRotation.y);

			//					Sleep(5);

			//					driver.Write<float>(pid, CurrentActorPM + ViewYawMin, OrginalPitchMin);
			//					driver.Write<float>(pid, CurrentActorPM + ViewYawMax, OrginalPitchMax);
			//					
			//				}
			//				else
			//				{
			//					reset_angles();
			//				}
			//			}
			//		}
			//	}
			//}

			if (!ud::u_prediction)
			{
				if (target_entity && ud::u_aimbot) {

					auto closest_mesh = driver.Read<std::uint64_t>(pid, target_entity + offset::skeletal_mesh);

					fvector hitbox;
					fvector2d hitbox_screen;

					switch (ud::u_hitbox) {
					case 0:
						hitbox_screen = w2s(get_bone_3d(closest_mesh, EBoneIndex::Head));
						hitbox = get_bone_3d(closest_mesh, EBoneIndex::Head);
						break;
					case 1:
						hitbox_screen = w2s(get_bone_3d(closest_mesh, EBoneIndex::Neck));
						hitbox = get_bone_3d(closest_mesh, EBoneIndex::Neck);
						break;
					case 2:
						hitbox_screen = w2s(get_bone_3d(closest_mesh, EBoneIndex::Chest));
						hitbox = get_bone_3d(closest_mesh, EBoneIndex::Chest);
						break;
					case 3:
						hitbox_screen = w2s(get_bone_3d(closest_mesh, EBoneIndex::Pelvis));
						hitbox = get_bone_3d(closest_mesh, EBoneIndex::Pelvis);
						break;
					}

					if (hitbox.x != 0 || hitbox.y != 0 && (get_cross_distance(hitbox.x, hitbox.y, screen_width / 2, screen_height / 2) <= ud::u_aimfov))
					{
						if (ud::u_aimbot && ud::u_target_line)
							m_framework->line(centerWidth, centerHeight, hitbox_screen.x, hitbox_screen.y, flinearcolor(255, 0, 0, 255), 2);

						if (GetAsyncKeyState_Spoofed(ud::aimkey))
							if (ud::enable_mouse_aimbot)
							{
								u_mouse->move_mouse(hitbox_screen);
							}
						//std::cout << "normal aim" << std::endl;

						//if (Controller::IsPressingRightTrigger())
						//	RotateCamera(hitbox);
					}

				}
			}
			else {
				target_dist = FLT_MAX;
				target_entity = NULL;
			}
		}

	};
} static u_loop::u_fn* u_main = new u_loop::u_fn();