#include "spoofer.h"
#include "SkCrypt.h"
#include "sdk.h"
#include <d3d9.h>
#include "driver.hpp"
#include "globals.h"
#include "imports.h"

inline int center_x;
inline int center_y;

class entity {
public:
	uintptr_t
		entity,
		skeletal_mesh,
		root_component,
		player_state;
	int
		team_index;
	fvector
		relative_location;
	float
		lastrendertime;
	bool
		is_visible;
};

std::vector<entity> entity_list;
std::vector<entity> temporary_entity_list;

class level_entity {
public:
	uintptr_t
		entity,
		root_component;
	std::string
		name;
};

std::vector<level_entity> level_list;
std::vector<level_entity> temporary_level_list;


namespace u_cached {
class u_cache
{
	public:

		uintptr_t
			uworld,
			game_instance,
			game_state,
			local_player,
			player_controller,
			acknowledged_pawn,
			skeletal_mesh,
			player_state,
			root_component,
			player_array;
		int
			team_index,
			player_array_size;
		fvector
			relative_location,
			velocity;
	};
} static u_cached::u_cache* ud_cache = new u_cached::u_cache();

//class 
//	item_pawns1
//{
//	uintptr_t
//		Actor;
//	std::string
//		Name;
//	bool
//		isVehicle,
//		isChest,
//		isPickup,
//		isAmmoBox;
//	float
//		distance;
//}item_pawns1;

class item {
public:
	uintptr_t
		Actor;

	std::string
		Name,
		isVehicle,
		isChest,
		isPickup,
		isAmmoBox;
	float
		distance;
};
std::vector<item> item_pawns;


typedef struct _LootEntity {
	std::string GNames;
	uintptr_t ACurrentItem;
}LootEntity;

static std::vector<LootEntity> LootentityList;



