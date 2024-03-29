#ifndef PP_ACTIONS
#define PP_ACTIONS

#include "stddef.h"
#include <Brawl/FT/Fighter.h>

#pragma region
#define ACTION_WAIT 0X0
#define ACTION_WALK 0X1
#define ACTION_WALKBRAKE 0X2
#define ACTION_DASH 0X3
#define ACTION_RUN 0X4
#define ACTION_RUNBRAKE 0X5
#define ACTION_TURN1 0X6
#define ACTION_TURN2 0X7
#define ACTION_TURNRUN 0X8
#define ACTION_TURNRUNBRAKE 0X9
#define ACTION_JUMPSQUAT 0XA
#define ACTION_JUMP 0XB
#define ACTION_JUMPAERIAL 0XC
#define ACTION_MULTIJUMP 0XD
#define ACTION_FALL 0XE
#define ACTION_FALLAERIAL 0XF
#define ACTION_FALLSPECIAL 0X10
#define ACTION_SQUAT 0X11
#define ACTION_SQUATWAIT 0X12
#define ACTION_SQUATF 0X13
#define ACTION_SQUATB 0X14
#define ACTION_SQUATRV 0X15
#define ACTION_LANDINGHEAVY 0X16
#define ACTION_LANDINGLIGHT 0X17
#define ACTION_LANDINGLAGAERIALATTACK 0X18
#define ACTION_LANDINGFALLSPECIAL 0X19
#define ACTION_GUARDON 0X1A
#define ACTION_GUARD 0X1B
#define ACTION_GUARDOFF 0X1C
#define ACTION_GUARDDAMAGE 0X1D
#define ACTION_ESCAPEN 0X1E
#define ACTION_ESCAPEF 0X1F
#define ACTION_ESCAPEB 0X20
#define ACTION_ESCAPEAIR 0X21
#define ACTION_REBOUND 0X22
#define ACTION_REBOUND2 0X23
#define ACTION_JAB 0X24
#define ACTION_RAPIDJAB 0X25
#define ACTION_ATTACKDASH 0X26
#define ACTION_ATTACKS3 0X27
#define ACTION_ATTACKHI3 0X28
#define ACTION_ATTACKLW3 0X29
#define ACTION_ATTACKS4START 0X2A
#define ACTION_ATTACKS4HOLD 0X2B
#define ACTION_ATTACKS4S 0X2C
#define ACTION_ATTACKLW4START 0X2D
#define ACTION_ATTACKLW4HOLD 0X2E
#define ACTION_ATTACKLW4 0X2F
#define ACTION_ATTACKHI4START 0X30
#define ACTION_ATTACKHI4HOLD 0X31
#define ACTION_ATTACKHI4 0X32
#define ACTION_AERIALATTACK 0X33
#define ACTION_CATCH 0X34
#define ACTION_CATCHPULL 0X35
#define ACTION_CATCHDASH 0X36
#define ACTION_CATCHWAIT1F 0X37
#define ACTION_CATCHTURN 0X38
#define ACTION_CATCHWAIT 0X39
#define ACTION_CATCHATTACK 0X3A
#define ACTION_CATCHCUT 0X3B
#define ACTION_THROW 0X3C
#define ACTION_CAPTUREWAIT 0X3D
#define ACTION_CAPTUREWAITHI 0X3E
#define ACTION_CAPTUREDAMAGE 0X3F
#define ACTION_CAPTURECUT 0X40
#define ACTION_CAPTUREJUMP 0X41
#define ACTION_THROWN 0X42
#define ACTION_GROUNDHIT 0X43
#define ACTION_AIRHITLAND 0X44
#define ACTION_HITSTUN 0X45
#define ACTION_HITGROUND 0X46
#define ACTION_HITWALL 0X47
#define ACTION_HITCEILING 0X48
#define ACTION_DAMAGEFALL 0X49
#define ACTION_HITGROUNDTUMBLE 0X4A
#define ACTION_HITWALLTUMBLE 0X4B
#define ACTION_HITCEILINGTUMBLE 0X4C
#define ACTION_DOWNWAIT 0X4D
#define ACTION_DOWNEAT 0X4F
#define ACTION_DOWNFOWARD 0X50
#define ACTION_DOWNSTAND 0X51
#define ACTION_DOWNFORWARD 0X52
#define ACTION_DOWNATTACK 0X53
#define ACTION_DOWNBOUND 0X54
#define ACTION_DOWNDAMAGE 0X55
#define ACTION_SHIELDBREAK 0X56
#define ACTION_SHIELDBREAKBOUND 0X57
#define ACTION_SHIELDBREAKBOUND_2 0X58
#define ACTION_FURAFURASTART 0X59
#define ACTION_FURAFURA 0X5A
#define ACTION_FURAFURAEND 0X5B
#define ACTION_FURASLEEPSTART 0X5C
#define ACTION_FURASLEEPLOOP 0X5D
#define ACTION_FURASLEEPEND 0X5E
#define ACTION_DEKUNUTSTUN 0X5F
#define ACTION_PASSIVE 0X60
#define ACTION_PASSIVESTAND 0X61
#define ACTION_PASSIVEWALL 0X62
#define ACTION_PASSIVEWALLJUMP 0X63
#define ACTION_PASSIVECEIL 0X64
#define ACTION_RUNINTOWALL 0X65
#define ACTION_HITHEADONWALL 0X66
#define ACTION_WALLJUMP 0X67
#define ACTION_WALLCLINGSTART 0X68
#define ACTION_WALLCLINGWAIT 0X69
#define ACTION_WALLCLINGEND 0X6A
#define ACTION_WALLCLINGJUMP 0X6C
#define ACTION_FOOTSTOOL 0X6D
#define ACTION_GETFOOTSTOOLED 0X6E
#define ACTION_FOOTSTOOLSTAND 0X6F
#define ACTION_GETFOOTSTOOLEDAIRSTART 0X70
#define ACTION_GETFOOTSTOOLED2 0X71
#define ACTION_PASS 0X72
#define ACTION_CLIFFCATCHSTART 0X73
#define ACTION_CLIFFCATCH 0X74
#define ACTION_CLIFFWAIT 0X75
#define ACTION_CLIFFATTACK 0X76
#define ACTION_CLIFFCLIMB 0X77
#define ACTION_CLIFFESCAPE 0X78
#define ACTION_CLIFFJUMPSTART 0X79
#define ACTION_CLIFFJUMPEND 0X7A
#define ACTION_OTTOTTO 0X7C
#define ACTION_OTTOTTOWAIT 0X7D
#define ACTION_EDGESLIP 0X7E
#define ACTION_FIRETETHER 0X7F
#define ACTION_TETHERAIMING 0X80
#define ACTION_TETHERGRABBINGSTAGE 0X81
#define ACTION_TETHERRISE 0X82
#define ACTION_GLIDESTART 0X84
#define ACTION_GLIDEDIRECTION 0X85
#define ACTION_GLIDELANDING 0X86
#define ACTION_GLIDEATTACK 0X87
#define ACTION_GLIDEEND 0X88
#define ACTION_SLIP 0X89
#define ACTION_SLIPDOWN 0X8A
#define ACTION_SLIPTURN 0X8B
#define ACTION_SLIPDASH 0X8C
#define ACTION_SLIPWAIT 0X8D
#define ACTION_SLIPSTAND 0X8E
#define ACTION_SLIPATTACK 0X8F
#define ACTION_SLIPESCAPEF 0X90
#define ACTION_SLIPESCAPEB 0X91
#define ACTION_LADDERCLIMB 0X92
#define ACTION_LADDERWAIT 0X94
#define ACTION_GRABITEM 0X96
#define ACTION_GRABHEAVYITEM 0X97
#define ACTION_THROWITEM 0X9B
#define ACTION_THROWITEMRUN 0X9C
#define ACTION_THROWHEAVYITEM 0X9D
#define ACTION_NEUTRALSWINGITEM 0X9E
#define ACTION_TILTSWINGITEM 0X9F
#define ACTION_SMASHSWINGITEMWINDUP 0XA0
#define ACTION_SMASHSWINGITEM 0XA1
#define ACTION_SMASHSWINGITEMCHARGE 0XA2
#define ACTION_DASHSWINGITEM 0XA3
#define ACTION_ITEMSCREW 0XAF
#define ACTION_ITEMSCREW_2 0XB0
#define ACTION_ITEMSCREWFALL 0XB1
#define ACTION_ITEMASSIST 0XB2
#define ACTION_SWIMFALL 0XB3
#define ACTION_SWIMRISE 0XB4
#define ACTION_SWIMDAMAGE 0XB5
#define ACTION_SWIM 0XB6
#define ACTION_SWIMF 0XB7
#define ACTION_SWIMEND 0XB8
#define ACTION_SWIMTURN 0XB9
#define ACTION_SWIMJUMPSQUAT 0XBA
#define ACTION_SWIMDROWN 0XBB
#define ACTION_SWIMDROWNOUT 0XBC
#define ACTION_DEATH 0XBD
#define ACTION_RESPAWN 0XBE
#define ACTION_SPRINGJUMP 0XC4
#define ACTION_EATENBYFISHONSUMMIT 0XC6
#define ACTION_CAPTURECHOMP 0XCC
#define ACTION_CAPTUREFALCONDIVE 0XCF
#define ACTION_BEINGINHALED 0XD0
#define ACTION_CAPTUREINHALE 0XD1
#define ACTION_CAPTUREMONKEYFLIP 0XD6
#define ACTION_DARKDIVEGRABBED 0XE6
#define ACTION_FLAMECHOKEGROUNDGRABBED 0XE7
#define ACTION_FLAMECHOKEAIRGRABBED 0XE8
#define ACTION_FLAMECHOKEAIRPLUMMETINGGRABBED 0XE9
#define ACTION_FLAMECHOKESCRAMBLE 0XEA
#define ACTION_EGGIMMINENT 0XEB
#define ACTION_CAPTUREEGG 0XEC
#define ACTION_CAPTUREFLYINGSLAM 0XEE
#define ACTION_CAPTUREMASTERHAND 0XFA
#define ACTION_UNLOADED 0X10B
#define ACTION_APPEAL 0X10C
#define ACTION_APPEALSMASH 0X10D
#define ACTION_ENTRANCE 0X10E
#define ACTION_VICTORY 0X10F
#define ACTION_LOSS 0X110
#define ACTION_SPECIALN 0X112
#define ACTION_SPECIALS 0X113
#define ACTION_SPECIALHI 0X114
#define ACTION_SPECIALLW 0X115
#define ACTION_FINAL 0X116
#define ACTION_FUWAFUWA 0x11E
#pragma endregion

bool isEATBitExclusion(CHAR_ID charKind, int actionId);
bool isAttackingAction(u16 action);
bool isDefinitelyActionable(u16 action);
const char* actionName(u16 action);

#endif