
#ifndef PROJECTMCODES_SNDSYSTEM_H
#define PROJECTMCODES_SNDSYSTEM_H

enum CMN_SOUNDFX : int {
  SND_CURSOR = 0x000, 
  SND_CONFIRM = 0x001, 
  SND_CANCEL = 0x002, 
  SND_ERROR = 0x003, 
  SND_SNAPSHOT = 0x004, 
  SND_INFO_OPEN = 0x005, 
  SND_INFO_CLOSE = 0x006, 
  SND_SCROLLBAR = 0x007, 
  SND__METALLIC_WOOSH = 0x008, 
  SND__SHING = 0x009, 
  SND_COUNTDOWN = 0x00A, 
  SND_JUSTDEFENSE = 0x00B, 
  SND_SHIELDREFLECT = 0x00C, 
  SND_JUSTIMPACT = 0x00D, 
  // SND_ = 0x00E, 
  SND_HIT_MUTEKI = 0x00F, 
  SND_SOUSATU = 0x010, 
  SND_MAGNIFYING_GLASS = 0x011, 
  SND_LIFEUP = 0x012, 
  SND_CURSOR_CONFIRM = 0x013, 
  SND_CURSOR_MOVE = 0x014, 
  // SND_ = 0x015, 
  SND_AUDIENCE_SUDDENDEATH = 0x016, 
  SND_AUDIENCE_GASP_L = 0x017, 
  SND_AUDIENCE_GASP_M = 0x018, 
  SND_AUDIENCE_GASP_S = 0x019, 
  SND_AUDIENCE_CHEER_L = 0x01A, 
  SND_AUDIENCE_CHEER_M = 0x01B, 
  SND_AUDIENCE_CHEER_S = 0x01C, 
  SND_AUDIENCE_CLAP_L = 0x01D, 
  SND_AUDIENCE_CLAP_M = 0x01E, 
  SND_AUDIENCE_CLAP_S = 0x01F, 
  SND_AUDIENCE_DISAPPOINTMENT = 0x020, 
  SND_ALLSTAR_WARP = 0x021, 
  SND_MENU_PAUSE = 0x022, 
  SND_MENU_PAGE_CHANGE = 0x023, 
  SND_MENU_SWITCH = 0x024, 
  SND_MENU_COUNTER = 0x025, 
  SND_2P_ENTRY = 0x026, 
  SND_SAVE = 0x027, 
  SND_DELETE = 0x028, 
  SND_ZOOM_OUT = 0x029, 
  SND__LARGE_BLAST_ECHO = 0x02A, 
  SND_KICK_SWING_S = 0x02B, 
  SND_KICK_SWING_M = 0x02C, 
  SND_KICK_SWING_L = 0x02D, 
  SND_SWORD_SWING_S = 0x02E, 
  SND_SWORD_SWING_M = 0x02F, 
  SND_SWORD_SWING_L = 0x030, 
  SND_PUNCH_HIT_S = 0x031, 
  SND_PUNCH_HIT_M = 0x032, 
  SND_PUNCH_HIT_L = 0x033, 
  SND_KICK_HIT_S = 0x034, 
  SND_KICK_HIT_M = 0x035, 
  SND_KICK_HIT_L = 0x036, 
  SND_SWORD_HIT_S = 0x037, 
  SND_SWORD_HIT_M = 0x038, 
  SND_SWORD_HIT_L = 0x039, 
  SND_DAMAGEFLY_S = 0x03A, 
  SND_DAMAGEFLY_M = 0x03B, 
  SND_DAMAGEFLY_L = 0x03C, 
  SND_KO = 0x03D, 
  SND_FIREBALL_HIT = 0x03E, 
  SND_FIRE_S = 0x03F, 
  SND_FIRE_M = 0x040, 
  SND_FIRE_L = 0x041, 
  SND_FIRE_XL = 0x042, 
  SND_BOMB_S = 0x043, 
  SND_BOMB_M = 0x044, 
  SND_BOMB_L = 0x045, 
  SND_BOMB_XL = 0x046, 
  SND_CLIFF_CATCH = 0x047, 
  SND_CLIFF_UP = 0x048, 
  SND_STARKO = 0x049, 
  SND_DAMAGEELEC = 0x04A, 
  SND_DOWN_SOIL_L = 0x04B, 
  SND_DOWN_SOIL_M = 0x04C, 
  SND_DOWN_SOIL_S = 0x04D, 
  SND_DOWN_SOIL_SS = 0x04E, 
  SND_FURAFURA = 0x04F, 
  SND_GUARDBREAK = 0x050, 
  SND_GUARDOFF = 0x051, 
  SND_GUARDON = 0x052, 
  SND_ITEM_GET_TABEMONO = 0x053, 
  SND_ITEM_GET_DRINK = 0x054, 
  SND_ITEM_GET_PEANUT = 0x055, 
  SND_SMASH_START = 0x056, 
  SND__GRAB_WHIP = 0x057, 
  SND__GRAB_S = 0x058, 
  SND__GRAB_M = 0x059, 
  SND__GRAB_L = 0x05A, 
  SND__GRAB_XL = 0x05B, 
  SND__WOOSH_LIGHT = 0x05C, 
  SND__DEEP_WOOSH_LIGHT = 0x05D, 
  SND__WOOSH = 0x05E, 
  SND__DEEP_WOOSH = 0x05F, 
  SND__ROLL = 0x060, 
  SND__CLIMBERS_FALL = 0x061, 
  SND_MAGIC_HIT_S = 0x062, 
  SND_MAGIC_HIT_M = 0x063, 
  SND_MAGIC_HIT_L = 0x064, 
  SND_MAGIC_HIT_XL = 0x065, 
  SND_ELECTRIC_HIT_S = 0x066, 
  SND_ELECTRIC_HIT_M = 0x067, 
  SND_ELECTRIC_HIT_L = 0x068, 
  SND_ELECTRIC_HIT_XL = 0x069, 
  SND_STEP_ROCK = 0x06A, 
  SND_STEP_GRASS = 0x06B, 
  SND_STEP_SOIL = 0x06C, 
  SND_STEP_WOOD = 0x06D, 
  SND_STEP_IRON = 0x06E, 
  SND_STEP_NIBULRON = 0x06F, 
  SND_STEP_CARPET = 0x070, 
  SND_STEP_NUMENUME = 0x071, 
  SND_STEP_CREATURE = 0x072, 
  SND_STEP_ASASE = 0x073, 
  SND_STEP_SOFT = 0x074, 
  SND_STEP_TURUTURU = 0x075, 
  SND_STEP_SNOW = 0x076, 
  SND_STEP_ICE = 0x077, 
  SND_STEP_GAMEWATCH = 0x078, 
  SND_STEP_OIL = 0x079, 
  SND_STEP_DANBOURU = 0x07A, 
  SND_LANDING_ROCK = 0x07B, 
  SND_LANDING_GRASS = 0x07C, 
  SND_LANDING_SOIL = 0x07D, 
  SND_LANDING_WOOD = 0x07E, 
  SND_LANDING_IRON = 0x07F, 
  SND_LANDING_NIBULRON = 0x080, 
  SND_LANDING_CARPET = 0x081, 
  SND_LANDING_NUMENUME = 0x082, 
  SND_LANDING_CREATURE = 0x083, 
  SND_LANDING_ASASE = 0x084, 
  SND_LANDING_SOFT = 0x085, 
  SND_LANDING_TURUTURU = 0x086, 
  SND_LANDING_SNOW = 0x087, 
  SND_LANDING_ICE = 0x088, 
  SND_LANDING_GAMEWATCH = 0x089, 
  SND_LANDING_OIL = 0x08A, 
  SND_LANDING_DANBOURU = 0x08B, 
  SND__BOUNCE = 0x08C, 
  SND__METAL_ITEM_LAND = 0x08D, 
  SND__SLIP = 0x08E, 
  SND__SLIPPERY = 0x08F, 
  SND__CANNON = 0x090, 
  SND__STRONG_WINDUP = 0x091, 
  SND__FAST_AIR_PARTING = 0x092, 
  SND__STRONG_AIR_PARTING = 0x093, 
  SND_WATER_HIT_S = 0x094, 
  SND_WATER_HIT_M = 0x095, 
  SND_WATER_HIT_L = 0x096, 
  SND_WATER_HIT_XL = 0x097, 
  SND__FOOTSTOOL = 0x098, 
  SND__NONE = 0x099, 
  SND__SPLASH_LIGHT = 0x09A, 
  SND__SPLASH_LAND = 0x09B, 
  SND__SPLASH_EMERGE = 0x09C, 
  SND__BUBBLES = 0x09D, 
  SND_LANDING_WATER = 0x09E, 
  SND__GLANCING_BLOW = 0x09F, 
  SND__FIRE = 0x0A0, 
  SND__SMASH_START_KIRBY = 0x0A1, 
  SND__SHIELD_HIT = 0x0A2, 
  SND_FREEZE = 0x0A3, 
  SND_BOMB_DAMAGE_S = 0x0A4, 
  SND_BOMB_DAMAGE_M = 0x0A5, 
  SND_BOMB_DAMAGE_L = 0x0A6, 
  SND_BOMB_DAMAGE_XL = 0x0A7, 
  SND_STEP_ELECTROPLANKTON = 0x0A8, 
  SND_LANDING_ELECTROPLANKTON = 0x0A9, 
  SND_FREEZE_S = 0x0AA, 
  SND_FREEZE_M = 0x0AB, 
  SND_FREEZE_L = 0x0AC, 
  SND_FREEZE_XL = 0x0AD, 
  SND_METAL_STEP_S = 0x0AE, 
  SND_METAL_STEP_M = 0x0AF, 
  SND_METAL_STEP_L = 0x0B0, 
  SND_METAL_LANDING_S = 0x0B1, 
  SND_METAL_LANDING_M = 0x0B2, 
  SND_METAL_LANDING_L = 0x0B3, 
  SND_GET_STICKER = 0x0B4, 
  SND_GET_FIGURE = 0x0B5, 
  SND_GET_CD = 0x0B6, 
  SND_JUSTSHIELD = 0x0B7, 
  SND__BEAMPHASER = 0x0B8, 
  SND_FINAL_QUAKE = 0x0B9, 
  SND_STEP_CLOUD = 0x0BA, 
  SND_STEP_AKUUKAN = 0x0BB, 
  SND_STEP_BRICK = 0x0BC, 
  SND_LANDING_CLOUD = 0x0BD, 
  SND_LANDING_AKUUKAN = 0x0BE, 
  SND_LANDING_BRICK = 0x0BF, 
  SND_STEP_FAMICOM = 0x0C0, 
  SND_STEP_FENCE = 0x0C1, 
  SND_STEP_SAND = 0x0C2, 
  SND_LANDING_FAMICOM = 0x0C3, 
  SND_LANDING_FENCE = 0x0C4, 
  SND_LANDING_SAND = 0x0C5, 
  SND_COIN = 0x0C6, 
  SND__ELECTRIC_CONFIRM = 0x0C7,
  SND__BEAM = 0x0C8,
  SND_SWALLOW = 0x0C9,
};


enum CMN_SOUNDFX_IDX : int {
  SNDI_CURSOR = 0x0, 
  SNDI_CONFIRM = 0x1, 
  SNDI_CANCEL = 0x2, 
  SNDI_ERROR = 0x3, 
  SNDI_SNAPSHOT = 0x4, 
  SNDI_INFO_OPEN = 0x5, 
  SNDI_INFO_CLOSE = 0x6, 
  SNDI_SCROLLBAR = 0x7, 
  SNDI__METALLIC_WOOSH = 0x8, 
  SNDI__SHING = 0x9, 
  SNDI_COUNTDOWN = 0xA, 
  SNDI_JUSTDEFENSE = 0xB, 
  SNDI_SHIELDREFLECT = 0xC, 
  SNDI_JUSTIMPACT = 0xD, 
  // ISND_ = 0x 14, 
  SNDI_HIT_MUTEKI = 0xF, 
  SNDI_SOUSATU = 0x10, 
  SNDI_MAGNIFYING_GLASS = 0x11, 
  SNDI_LIFEUP = 0x12, 
  SNDI_CURSOR_CONFIRM = 0x13, 
  SNDI_CURSOR_MOVE = 0x14, 
  // ISND_ = 0x 21, 
  SNDI_AUDIENCE_SUDDENDEATH = 0x16, 
  SNDI_AUDIENCE_GASP_L = 0x17, 
  SNDI_AUDIENCE_GASP_M = 0x18, 
  SNDI_AUDIENCE_GASP_S = 0x19, 
  SNDI_AUDIENCE_CHEER_L = 0x1A, 
  SNDI_AUDIENCE_CHEER_M = 0x1B, 
  SNDI_AUDIENCE_CHEER_S = 0x1C, 
  SNDI_AUDIENCE_CLAP_L = 0x1D, 
  SNDI_AUDIENCE_CLAP_M = 0x1E, 
  SNDI_AUDIENCE_CLAP_S = 0x1F, 
  SNDI_AUDIENCE_DISAPPOINTMENT = 0x20, 
  SNDI_ALLSTAR_WARP = 0x21, 
  SNDI_MENU_PAUSE = 0x22, 
  SNDI_MENU_PAGE_CHANGE = 0x23, 
  SNDI_MENU_SWITCH = 0x24, 
  SNDI_MENU_COUNTER = 0x25, 
  SNDI_2P_ENTRY = 0x26, 
  SNDI_SAVE = 0x27, 
  SNDI_DELETE = 0x28, 
  SNDI_ZOOM_OUT = 0x29, 

  SNDI__LARGE_BLAST_ECHO = 0x2B, 

  SNDI_KICK_SWING_S = 0x35, 
  SNDI_KICK_SWING_M = 0x36, 
  SNDI_KICK_SWING_L = 0x37, 
  SNDI_SWORD_SWING_S = 0x38, 
  SNDI_SWORD_SWING_M = 0x39, 
  SNDI_SWORD_SWING_L = 0x3A, 
  SNDI_PUNCH_HIT_S = 0x3B, 
  SNDI_PUNCH_HIT_M = 0x3C, 
  SNDI_PUNCH_HIT_L = 0x3D, 
  SNDI_KICK_HIT_S = 0x3E, 
  SNDI_KICK_HIT_M = 0x3F, 
  SNDI_KICK_HIT_L = 0x40, 
  SNDI_SWORD_HIT_S = 0x41, 
  SNDI_SWORD_HIT_M = 0x42, 
  SNDI_SWORD_HIT_L = 0x43, 
  SNDI_DAMAGEFLY_S = 0x44, 
  SNDI_DAMAGEFLY_M = 0x45, 
  SNDI_DAMAGEFLY_L = 0x46, 
  SNDI_KO = 0x47, 

  // SNDI_FIREBALL_HIT = 0x48, 

  SNDI_FIRE_S = 0x4B, 
  SNDI_FIRE_M = 0x4C, 
  SNDI_FIRE_L = 0x4D, 
  SNDI_FIRE_XL = 0x4E, 
  SNDI_BOMB_S = 0x4F, 
  SNDI_BOMB_M = 0x50, 
  SNDI_BOMB_L = 0x51, 
  SNDI_BOMB_XL = 0x52, 
  SNDI_CLIFF_CATCH = 0x53, 
  SNDI_CLIFF_UP = 0x54,

  SNDI_STARKO = 0x58, 
  SNDI_DAMAGEELEC = 0x59, 

  SNDI_DOWN_SOIL_L = 0x5C, 
  SNDI_DOWN_SOIL_M = 0x5D, 
  SNDI_DOWN_SOIL_S = 0x5E, 
  SNDI_DOWN_SOIL_SS = 0x5F, 

  SNDI_FURAFURA = 0x61, 
  SNDI_GUARDBREAK = 0x62, 
  SNDI_GUARDOFF = 0x63, 
  SNDI_GUARDON = 0x64, 
  SNDI_ITEM_GET_TABEMONO = 0x65, 
  SNDI_ITEM_GET_DRINK = 0x66, 
  SNDI_ITEM_GET_PEANUT = 0x67, 

  SNDI_SMASH_START = 0x69, 
  SNDI__GRAB_WHIP = 0x6A, 
  SNDI__GRAB_S = 0x6B, 
  SNDI__GRAB_M = 0x6C, 
  SNDI__GRAB_L = 0x6D, 
  SNDI__GRAB_XL = 0x6E, 
  SNDI__WOOSH_LIGHT = 0x6F, 
  SNDI__DEEP_WOOSH_LIGHT = 0x70, 
  SNDI__WOOSH = 0x71, 
  SNDI__DEEP_WOOSH = 0x72, 
  SNDI__ROLL = 0x73, 
  SNDI__CLIMBERS_FALL = 0x74, 
  SNDI_MAGIC_HIT_S = 0x75, 
  SNDI_MAGIC_HIT_M = 0x76, 
  SNDI_MAGIC_HIT_L = 0x77, 
  SNDI_MAGIC_HIT_XL = 0x78, 
  SNDI_ELECTRIC_HIT_S = 0x79, 
  SNDI_ELECTRIC_HIT_M = 0x7A, 
  SNDI_ELECTRIC_HIT_L = 0x7B, 
  SNDI_ELECTRIC_HIT_XL = 0x7C, 
  SNDI_STEP_ROCK = 0x7D, 
  SNDI_STEP_GRASS = 0x7E, 
  SNDI_STEP_SOIL = 0x7F, 
  SNDI_STEP_WOOD = 0x80, 
  SNDI_STEP_IRON = 0x81, 
  SNDI_STEP_NIBULRON = 0x82, 
  SNDI_STEP_CARPET = 0x83, 
  SNDI_STEP_NUMENUME = 0x84, 
  SNDI_STEP_CREATURE = 0x85, 
  SNDI_STEP_ASASE = 0x86, 
  SNDI_STEP_SOFT = 0x87, 
  SNDI_STEP_TURUTURU = 0x88, 
  SNDI_STEP_SNOW = 0x89, 
  SNDI_STEP_ICE = 0x8A, 
  SNDI_STEP_GAMEWATCH = 0x8B, 
  SNDI_STEP_OIL = 0x8C, 
  SNDI_STEP_DANBOURU = 0x8D, 
  SNDI_LANDING_ROCK = 0x8E, 
  SNDI_LANDING_GRASS = 0x8F, 
  SNDI_LANDING_SOIL = 0x90, 
  SNDI_LANDING_WOOD = 0x91, 
  SNDI_LANDING_IRON = 0x92, 
  SNDI_LANDING_NIBULRON = 0x93, 
  SNDI_LANDING_CARPET = 0x94, 
  SNDI_LANDING_NUMENUME = 0x95, 
  SNDI_LANDING_CREATURE = 0x96, 
  SNDI_LANDING_ASASE = 0x97, 
  SNDI_LANDING_SOFT = 0x98, 
  SNDI_LANDING_TURUTURU = 0x99, 
  SNDI_LANDING_SNOW = 0x9A, 
  SNDI_LANDING_ICE = 0x9B, 
  SNDI_LANDING_GAMEWATCH = 0x9C, 
  SNDI_LANDING_OIL = 0x9D, 
  SNDI_LANDING_DANBOURU = 0x9E, 
  SNDI__BOUNCE = 0x9F, 
  SNDI__METAL_ITEM_LAND = 0xA0, 

  SNDI__SLIP = 0xA3, 
  SNDI__SLIPPERY = 0xA4, 

  SNDI__CANNON = 0xA6, 
  SNDI__STRONG_WINDUP = 0xA7, 
  SNDI__FAST_AIR_PARTING = 0xA8, 
  SNDI__STRONG_AIR_PARTING = 0xA9, 
  SNDI_WATER_HIT_S = 0xAA, 
  SNDI_WATER_HIT_M = 0xAB, 
  SNDI_WATER_HIT_L = 0xAC, 
  SNDI_WATER_HIT_XL = 0xAD, 
  SNDI__FOOTSTOOL = 0xAE, 
  // SNDI__NONE = 0xAF, 

  SNDI__SPLASH_LIGHT = 0xB4, 
  SNDI__SPLASH_LAND = 0xB5, 
  SNDI__SPLASH_EMERGE = 0xB6, 

  SNDI__BUBBLES = 0xB9, 
  SNDI_LANDING_WATER = 0xBA,

  SNDI__GLANCING_BLOW = 0xBD, 
  SNDI__FIRE = 0xBE, 
  SNDI__SMASH_START_KIRBY = 0xBF, 

  SNDI__SHIELD_HIT = 0xC1, 

  SNDI_FREEZE = 0xC3, 

  SNDI_BOMB_DAMAGE_S = 0xC6, 
  SNDI_BOMB_DAMAGE_M = 0xC7, 
  SNDI_BOMB_DAMAGE_L = 0xC8, 
  SNDI_BOMB_DAMAGE_XL = 0xC9, 
  SNDI_STEP_ELECTROPLANKTON = 0xCA, 
  SNDI_LANDING_ELECTROPLANKTON = 0xCB, 
  SNDI_FREEZE_S = 0xCC, 
  SNDI_FREEZE_M = 0xCD, 
  SNDI_FREEZE_L = 0xCE, 
  SNDI_FREEZE_XL = 0xCF, 
  SNDI_METAL_STEP_S = 0xD0, 
  SNDI_METAL_STEP_M = 0xD1, 
  SNDI_METAL_STEP_L = 0xD2, 
  SNDI_METAL_LANDING_S = 0xD3, 
  SNDI_METAL_LANDING_M = 0xD4, 
  SNDI_METAL_LANDING_L = 0xD5, 
  SNDI_GET_STICKER = 0xD6, 
  SNDI_GET_FIGURE = 0xD7, 
  SNDI_GET_CD = 0xD8, 
  SNDI_JUSTSHIELD = 0xD9, 
  SNDI__BEAMPHASER = 0xDA, 
  SNDI_FINAL_QUAKE = 0xDB, 
  SNDI_STEP_CLOUD = 0xDC, 
  SNDI_STEP_AKUUKAN = 0xDD, 
  SNDI_STEP_BRICK = 0xDE, 
  SNDI_LANDING_CLOUD = 0xDF, 
  SNDI_LANDING_AKUUKAN = 0xE0, 
  SNDI_LANDING_BRICK = 0xE1, 
  SNDI_STEP_FAMICOM = 0xE2, 
  SNDI_STEP_FENCE = 0xE3, 
  SNDI_STEP_SAND = 0xE4, 
  SNDI_LANDING_FAMICOM = 0xE5, 
  SNDI_LANDING_FENCE = 0xE6, 
  SNDI_LANDING_SAND = 0xE7, 
  SNDI_COIN = 0xE8, 
  SNDI__ELECTRIC_CONFIRM = 0xE9,
  SNDI__BEAM = 0xEA,

  SNDI_SWALLOW = 0xEC,
};

struct sndSystem {
  void playSE(int effectID);
};

#define SOUND_SYSTEM (((sndSystem* (*)()) 0x80683b48)())
#define _playSE_sndSystem ((void (*)(sndSystem * self, int id, int unk, int unk2, int unk3, int unk4)) 0x800742b0)

#endif