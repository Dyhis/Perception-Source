#include <mutex>
#include "impl/inc.hpp"
namespace ud
{
	inline bool enemy_counter = true;
	inline bool grab_defaults = true;
	inline int page = 0;
	inline fvector2d menu_pos(550, 350);
	inline bool renderframeworkmenu = false;
	inline bool hover_element = false;
	inline int aimkey;
	inline int TBkey;
	inline fvector fly_orig;
	inline bool u_aimbot = 1;
	inline bool enable_mouse_aimbot = 1;
	inline bool enable_memory_aimbot = 0;
	inline bool fly = false;
	inline bool spinbot = 0;
	std::string local_username;
	inline bool u_prediction = 0;
	inline bool sniperonlyprediction = 0;
	inline bool show_xbox_settings = 0;
	inline bool xbox_left_dpad = 0;
	inline bool xbox_right_dpad = 0;
	inline bool xbox_left_trigger = 1;
	inline bool xbox_right_trigger = 0;
	inline inline bool u_ignore_downed = 1;
	inline inline bool downed_player_found = 0;

	inline inline bool u_ignore_dead = 0;

	inline inline bool b_is_dying_lol = 1;
	inline bool draw_aim_mode = 0;
	inline bool disable_prediction = 0;
	inline float projectileSpeed;
	inline float projectileGravityScale;
	inline bool instant_revive = true;
	inline inline bool u_ignore_bots = 0;
	inline bool knocked_esp = true;
	inline bool dnbo_show_color_menu = false;
	inline bool player_states = false;
	inline inline bool u_visible_check = true;
	inline bool draw_radar = 0;
	inline bool u_enable_crosshair = 1;
	inline int crosshair_size = 2.5;
	inline bool double_pump = true;
	inline inline bool u_line = 0;
	inline int u_line_type = 0;
	inline bool airstuck = true;
	inline inline bool lamma = 0;
	inline bool color_names_by_team_index = true;
	inline bool attach_team_id = false;
	inline inline bool pickups = 0;
	inline inline bool show_floor_ammo = 0;

	inline inline bool show_floor_materials = 0;
	inline inline bool show_floor_gold = 0;
	inline inline bool u_textoutline = 1;

	inline inline bool enable_world_visuals = 0;

	inline inline bool draw_chests = 0;

	inline inline bool draw_ammobox = 0;

	inline inline bool draw_vehicles = 0;

	inline inline float lootdelay = 1;

	inline inline int loot_render_distance = 300;

	inline inline int chestrender = 100;

	inline inline int vehiclesrender = 100;

	inline inline int ammoboxrender = 100;

	inline inline int pickupsrender = 250;
	inline bool material_esp = 0;

	inline bool misc_show_fps = false;

	inline inline int lammarender = 100;

	inline inline bool UnCommon = 1;

	inline inline bool items = 1;

	inline inline bool rare = 1;

	inline inline bool purple = 1;

	inline inline bool gold = 1;

	inline inline bool mythic = 1;

	inline inline bool Common = 1;

	inline inline bool show_common = 1;
	inline inline bool show_uncommon = 1;
	inline inline bool show_rare = 1;
	inline inline bool show_epic = 1;
	inline inline bool show_legendary = 1;
	inline inline bool show_mythic = 1;

	inline inline int control_rarity = 6;

	inline float u_aimfov = 120;
	inline int u_aimfov_when_close_aim_active = 301;
	inline int u_close_aim_buffer;
	inline int u_close_aim_buffer_2;
	inline bool close_aimbot = 0;
	inline int close_aimbot_distance = 8;

	inline bool no_reload = true;
	inline bool no_recoil = true;
	inline bool infinite_fuel = true;
	inline bool ADS_when_reloading = true;
	inline bool ADS_when_jumping = true;

	inline int active_gun = 0;

	inline int u_hitbox = 0;

	inline bool u_target_line = 0;

	inline inline bool u_skeleton = 1;

	inline bool u_box_esp = 0;
	inline bool esp_auto_thick = true;
	inline bool u_enable_esp = 1;
	inline bool enable_vis_check_checkbox = 0;
	inline bool wireframe = 1;
	inline bool showniggas = 0;
	inline bool showniggas2 = 0;
	inline bool showniggas3 = 0;
	inline bool u_distance = 1;
	inline bool u_white_text_color = 1;

	inline bool u_username = 1;

	inline bool u_head = 0;

	inline int u_head_size = 15;

	inline bool u_snapline = 0;

	inline bool u_weapon_cfg = 0;

	inline bool dev = 0;
	inline bool dev2 = 0;
	inline bool develop_mode_weapons = 1;

	flinearcolor u_color_Visible(255, 205, 50, 255);
	flinearcolor u_color_invisible(255, 255, 0, 0);
	flinearcolor u_color_dbno(255, 255, 0, 255);

	static float u_color[3] = { 1.00f, 1.00f, 1.00f };

	static int u_color_fov[3] = { 1.00f, 1.00f, 1.00f };

	float u_render_distance = 300;

	inline bool u_view_angle = 0;

	inline bool u_weapon_esp = 1;
	inline bool platform_esp = 0;
	inline bool u_teamid = 0;
	inline bool showwasd = 1;

	inline int u_box_type = 0;
	inline int u_skeleton_type = 0;
	inline bool u_skeleton_vis_colors = 0;

	inline bool u_outline = 0;

	inline float u_box_thick = 2;

	inline int u_line_thick = 2;

	inline int u_view_angle_thick = 1;

	inline float u_skeleton_thick = 3;

	inline float u_aim_max_dist = 275;

	inline float u_smooth = 1;
	inline flinearcolor fov_color(255, 255, 255, 255);
	//inline float* fov_color;
	inline bool u_render_fov = 1;
	inline bool u_render_fov_filled = 0;
	inline float filled_fov_opacity = 50;
	inline bool active_color_picker_fov = 0;
	inline bool active_color_picker_vis_check = 0;

	inline bool u_weapon_rarity = 1;
	inline bool show_reloads = 1;
	inline bool show_targetting_enemy = 0;
	inline bool show_weapon_esp_ammo_count = 0;

	inline bool u_render_crosshair = 0;

	inline bool u_render_menu = 0;

	inline bool u_ignore_keybind = 0;

	inline int u_aim_key;

	inline bool shotgun_only = 0;
	inline bool u_triggerbot = 0;
	inline bool hold_key_for_tb = 1;
	inline bool toggle_key_for_tb = 0;
	inline bool u_auto_aim_when_tb = 0;
	inline bool autofire = 1;
	inline bool u_shotguns_only = 0;
	inline float u_maximum_distance = 10;
	inline float u_custom_delay = 100;

	inline bool u_streamproof;

	inline bool in_lobby;

	inline inline bool u_debug = 0;

	inline bool u_render_uncommon = 1;
	inline bool u_render_common = 1;
	inline bool u_render_rare = 1;
	inline bool u_render_epic = 1;
	inline bool u_render_legendary = 1;
	inline bool u_render_mythic = 1;
}

namespace playerstats
{
	inline bool CrouchingActoVar = 0;
	inline bool bIsSliding = 0;
	inline bool bIsJumping = 0;
	inline bool playerkills = 0;
}

namespace rifle
{
	inline bool aimbot = 1;
	inline bool ignore_downed = 1;
	inline bool ignore_bots = 0;
	inline bool visible_check = 1;
	inline bool target_line = 1;
	inline float aim_fov = 75;
	inline float smooth = 7;
	inline int hitbox = 2;
	inline int aimkey;
	inline int aimkeyTB;
}

namespace shotgun
{
	inline bool aimbot = 1;
	inline bool ignore_downed = 1;
	inline bool ignore_bots = 0;
	inline bool visible_check = 1;
	inline bool target_line = 1;
	inline float aim_fov = 100;
	inline float smooth = 3;
	inline int hitbox = 0;
	inline int aimkey;
	inline int aimkeyTB;
}

namespace smg
{
	inline bool aimbot = 1;
	inline bool ignore_downed = 1;
	inline bool ignore_bots = 0;
	inline bool visible_check = 1;
	inline bool target_line = 1;
	inline float aim_fov = 65;
	inline float smooth = 6;
	inline int hitbox = 2;
	inline int aimkey;
	inline int aimkeyTB;
}

namespace sniper
{
	inline bool aimbot = 1;
	inline bool ignore_downed = 1;
	inline bool ignore_bots = 0;
	inline bool visible_check = 1;
	inline bool target_line = 1;
	inline float aim_fov = 150;
	inline float smooth = 3;
	inline int hitbox = 0;
	inline int aimkey;
	inline int aimkeyTB;
}

namespace Preview
{
	float head_bone_x = 0;
	float head_bone_y = 94;

	float neck_bone_x = 4;
	float neck_bone_y = 114;

	float chest_bone_x = 2;
	float chest_bone_y = 122;

	float left_shoulder_bone_x = -24;
	float left_shoulder_bone_y = 124;

	float left_elbow_bone_x = -37;
	float left_elbow_bone_y = 153;

	float left_hand_bone_x = -51;
	float left_hand_bone_y = 196;

	float right_hand_bone_x = 47;
	float right_hand_bone_y = 190;

	float right_elbow_bone_x = 43;
	float right_elbow_bone_y = 167;

	float right_shoulder_bone_x = 20;
	float right_shoulder_bone_y = 124;

	float pelvis_bone_x = -6;
	float pelvis_bone_y = 184;

	float left_thigh_bone_x = -22;
	float left_thigh_bone_y = 186;

	float right_thigh_bone_x = 12;
	float right_thigh_bone_y = 186;

	float right_knee_bone_x = 25;
	float right_knee_bone_y = 253;

	float left_knee_bone_x = -37;
	float left_knee_bone_y = 245;

	float right_foot_bone_x = 31;
	float right_foot_bone_y = 310;

	float left_foot_bone_x = -47;
	float left_foot_bone_y = 308;

	float right_footfinger_bone_x = 37;
	float right_footfinger_bone_y = 324;

	float left_footfinger_bone_x = -57;
	float left_footfinger_bone_y = 322;
}

namespace indexcolor
{
	flinearcolor color1(255, 0, 0, 0); // Red
	flinearcolor color2(255, 0, 0, 85); // Dark Red
	flinearcolor color3(255, 0, 0, 170); // Maroon
	flinearcolor color4(255, 0, 0, 255); // Deep Maroon
	flinearcolor color5(255, 0, 85, 0); // Dark Green
	flinearcolor color6(255, 0, 85, 85); // Forest Green
	flinearcolor color7(255, 0, 85, 170); // Dark Cyan
	flinearcolor color8(255, 0, 85, 255); // Teal
	flinearcolor color9(255, 0, 170, 0); // Green
	flinearcolor color10(255, 0, 170, 85); // Light Teal
	flinearcolor color11(255, 0, 170, 170); // Aqua
	flinearcolor color12(255, 0, 170, 255); // Sky Blue
	flinearcolor color13(255, 0, 255, 0); // Lime Green
	flinearcolor color14(255, 0, 255, 85); // Spring Green
	flinearcolor color15(255, 0, 255, 170); // Turquoise
	flinearcolor color16(255, 0, 255, 255); // Cyan
	flinearcolor color17(255, 85, 0, 0); // Dark Maroon
	flinearcolor color18(255, 85, 0, 85); // Brown
	flinearcolor color19(255, 85, 0, 170); // Deep Pink
	flinearcolor color20(255, 85, 0, 255); // Fuchsia
	flinearcolor color21(255, 85, 85, 0); // Olive
	flinearcolor color22(255, 85, 85, 85); // Dark Olive
	flinearcolor color23(255, 85, 85, 170); // Olive Drab
	flinearcolor color24(255, 85, 85, 255); // Light Purple
	flinearcolor color25(255, 85, 170, 0); // Goldenrod
	flinearcolor color26(255, 85, 170, 85); // Tan
	flinearcolor color27(255, 85, 170, 170); // Peach
	flinearcolor color28(255, 85, 170, 255); // Salmon
	flinearcolor color29(255, 85, 255, 0); // Chartreuse
	flinearcolor color30(255, 85, 255, 85); // Light Chartreuse
	flinearcolor color31(255, 85, 255, 170); // Spring Chartreuse
	flinearcolor color32(255, 85, 255, 255); // Light Cyan
	flinearcolor color33(255, 170, 0, 0); // Dark Pink
	flinearcolor color34(255, 170, 0, 85); // Rosy Brown
	flinearcolor color35(255, 170, 0, 170); // Purple
	flinearcolor color36(255, 170, 0, 255); // Light Purple
	flinearcolor color37(255, 170, 85, 0); // Dark Khaki
	flinearcolor color38(255, 170, 85, 85); // Khaki
	flinearcolor color39(255, 170, 85, 170); // Light Salmon
	flinearcolor color40(255, 170, 85, 255); // Light Pink
	flinearcolor color41(255, 170, 170, 0); // Yellow
	flinearcolor color42(255, 170, 170, 85); // Light Yellow
	flinearcolor color43(255, 170, 170, 170); // Light Grey
	flinearcolor color44(255, 170, 170, 255); // White Smoke
	flinearcolor color45(255, 170, 255, 0); // Beige
	flinearcolor color46(255, 170, 255, 85); // Light Beige
	flinearcolor color47(255, 170, 255, 170); // Seashell
	flinearcolor color48(255, 170, 255, 255); // Snow
	flinearcolor color49(255, 255, 0, 0); // Red
	flinearcolor color50(255, 255, 0, 85); // Light Red
	flinearcolor color51(255, 255, 0, 170); // Pink
	flinearcolor color52(255, 255, 0, 255); // Light Pink
	flinearcolor color53(255, 255, 85, 0); // Dark Orange
	flinearcolor color54(255, 255, 85, 85); // Orange
	flinearcolor color55(255, 255, 85, 170); // Light Orange
	flinearcolor color56(255, 255, 85, 255); // Peach
	flinearcolor color57(255, 255, 170, 0); // Dark Yellow
	flinearcolor color58(255, 255, 170, 85); // Gold
	flinearcolor color59(255, 255, 170, 170); // Light Gold
	flinearcolor color60(255, 255, 170, 255); // Lemon
	flinearcolor color61(255, 255, 255, 0); // Yellow
	flinearcolor color62(255, 255, 255, 85); // Light Yellow
	flinearcolor color63(255, 255, 255, 170); // Cream
	flinearcolor color64(255, 255, 255, 255); // White
	flinearcolor color65(255, 0, 0, 0); // Black
	flinearcolor color66(255, 85, 85, 85); // Dark Grey
	flinearcolor color67(255, 170, 170, 170); // Grey
	flinearcolor color68(255, 210, 210, 210); // Light Grey
	flinearcolor color69(255, 85, 0, 0); // Dark Red
	flinearcolor color70(255, 170, 0, 0); // Medium Red
	flinearcolor color71(255, 255, 0, 0); // Light Red
	flinearcolor color72(255, 0, 85, 0); // Dark Green
	flinearcolor color73(255, 0, 170, 0); // Medium Green
	flinearcolor color74(255, 0, 255, 0); // Light Green
	flinearcolor color75(255, 0, 0, 85); // Dark Blue
	flinearcolor color76(255, 0, 0, 170); // Medium Blue
	flinearcolor color77(255, 0, 0, 255); // Light Blue
	flinearcolor color78(255, 85, 0, 85); // Dark Purple
	flinearcolor color79(255, 170, 0, 170); // Medium Purple
	flinearcolor color80(255, 255, 0, 255); // Light Purple
	flinearcolor color81(255, 85, 85, 0); // Dark Yellow
	flinearcolor color82(255, 170, 170, 0); // Medium Yellow
	flinearcolor color83(255, 255, 255, 0); // Light Yellow
	flinearcolor color84(255, 0, 85, 85); // Dark Cyan
	flinearcolor color85(255, 0, 170, 170); // Medium Cyan
	flinearcolor color86(255, 0, 255, 255); // Light Cyan
	flinearcolor color87(255, 85, 0, 85); // Dark Magenta
	flinearcolor color88(255, 170, 0, 170); // Medium Magenta
	flinearcolor color89(255, 255, 0, 255); // Light Magenta
	flinearcolor color90(255, 85, 0, 0); // Dark Orange
	flinearcolor color91(255, 170, 85, 0); // Medium Orange
	flinearcolor color92(255, 255, 170, 0); // Light Orange
	flinearcolor color93(255, 0, 85, 0); // Dark Chartreuse
	flinearcolor color94(255, 0, 170, 85); // Medium Chartreuse
	flinearcolor color95(255, 0, 255, 170); // Light Chartreuse
	flinearcolor color96(255, 85, 0, 85); // Dark Rose
	flinearcolor color97(255, 170, 0, 170); // Medium Rose
	flinearcolor color98(255, 255, 0, 255); // Light Rose
	flinearcolor color99(255, 85, 85, 0); // Dark Mint
	flinearcolor color100(255, 170, 170, 0); // Medium Mint
}
