//
// Created by Ilir on 2021-12-16.
//

#ifndef PROJECTMCODES_EFFECTITEMHANDLER_H
#define PROJECTMCODES_EFFECTITEMHANDLER_H

#include "Brawl/IT/BaseItem.h"
#include "Brawl/IT/itManager.h"
#include "EXIPacket.h"
#include "Assembly.h"
#include "Utilities.h"


void checkItemSpawnPokemonOrAssist();

EXIStatus effectItemSpawn(int itemId, u16 amount);
EXIStatus effectItemPreloadPokemon(int itemId, u16 amount);
EXIStatus effectItemPreloadAssist(int itemId, u16 amount);
EXIStatus effectItemAttachGooey(int numPlayers, u16 targetPlayer, u16 amount);


#endif //PROJECTMCODES_EFFECTITEMHANDLER_H