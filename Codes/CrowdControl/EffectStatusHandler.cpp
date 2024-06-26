//
// Created by Ilir on 2021-12-18.
//

#include "EffectStatusHandler.h"

#define setSwap ((void (*)(ftEntryManager entryManager, bool setEffect, entryID targetPlayer1, entryID targetPlayer2, bool unk3, int duration)) 0x80824090)

// TODO: Check if player is KO'ed / effect can be applied
// TODO: Versions where permanently on unless taken off?

//// Credit: Kapedani
EXIStatus effectStatusGiveMetal(u16 numPlayers, u16 targetPlayer, bool setEffect, double health){

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players metal
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            getFighter(targetPlayer)->setMetal(health, setEffect, 0xffffffff);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        getFighter(targetPlayer)->setMetal(health, setEffect, 0xffffffff);
    }

    return RESULT_EFFECT_SUCCESS;
}

//// Credit: Kapedani
EXIStatus effectStatusGiveCurry(u16 numPlayers, u16 targetPlayer, bool setEffect){

    // TODO: Effect goes away on hit, maybe should investigate why

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players curry
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            getFighter(targetPlayer)->setCurry(setEffect, 0xffffffff);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        getFighter(targetPlayer)->setCurry(setEffect, 0xffffffff);
    }

    return RESULT_EFFECT_SUCCESS;
}

//// Credit: Kapedani
EXIStatus effectStatusGiveHammer(u16 numPlayers, u16 targetPlayer, bool setEffect){
    // TODO: Have a duration and stop at duration

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players hammer
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            getFighter(targetPlayer)->setHammer(setEffect, 0);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        getFighter(targetPlayer)->setHammer(setEffect, 0);
    }

    return RESULT_EFFECT_SUCCESS;
}

//// Credit: Kapedani
EXIStatus effectStatusGiveSuperStar(u16 numPlayers, u16 targetPlayer, bool setEffect){

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        getFighter(targetPlayer)->setSuperStar(setEffect, 0xffffffff, 0);
    }

    return RESULT_EFFECT_SUCCESS;
}

//// Credit: Kapedani
EXIStatus effectStatusGiveFlower(u16 numPlayers, u16 targetPlayer, bool setEffect, double rate, double size){

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players flowers
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            getFighter(targetPlayer)->setFlower(rate, setEffect, 0, size, 0);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        getFighter(targetPlayer)->setFlower(rate, setEffect, 0, size, 0);
    }

    return RESULT_EFFECT_SUCCESS;
}

//// Credit: Kapedani
EXIStatus effectStatusGiveHeart(u16 numPlayers, u16 targetPlayer, u16 givingPlayer, bool setEffect){

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (givingPlayer >= numPlayers) {
        givingPlayer = randi(numPlayers);
    }

    auto id = FIGHTER_MANAGER->getEntryIdFromIndex(givingPlayer);

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players heart
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            getFighter(targetPlayer)->setHeartSwap(id, 0xffffffff, setEffect);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        getFighter(targetPlayer)->setHeartSwap(id, 0xffffffff, setEffect);
    }

    return RESULT_EFFECT_SUCCESS;
}

//// Credit: Kapedani
EXIStatus effectStatusGiveSlow(u16 numPlayers, u16 targetPlayer, bool setEffect, int slowStrength, int duration){

    // Timer item normally has 720 duration

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players slow
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            getFighter(targetPlayer)->setSlow(setEffect, slowStrength, duration*60, 1);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        getFighter(targetPlayer)->setSlow(setEffect, slowStrength, duration*60, 1);
    }

    return RESULT_EFFECT_SUCCESS;
}

//// Credit: Kapedani
EXIStatus effectStatusGiveMushroom(u16 numPlayers, u16 targetPlayer, bool setEffect, bool isPoison){

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players slow
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            getFighter(targetPlayer)->startScaling(setEffect, isPoison);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        getFighter(targetPlayer)->startScaling(setEffect, isPoison);
    }

    return RESULT_EFFECT_SUCCESS;
}

//// Credit: Kapedani
EXIStatus effectStatusGiveEquip(u16 numPlayers, u16 targetPlayer, u16 itemId){

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }

    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players equipment
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            getFighter(targetPlayer)->modules->itemManageModule->attachItem(itemId, 0, 1);
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        getFighter(targetPlayer)->modules->itemManageModule->attachItem(itemId, 0, 1);
    }

    return RESULT_EFFECT_SUCCESS;
}

//// Credit: Eon
EXIStatus effectStatusGiveSwap(u16 numPlayers, u16 targetPlayer1, u16 targetPlayer2, bool setEffect, int duration){
    if (targetPlayer1 >= numPlayers or targetPlayer2 >= numPlayers or targetPlayer1 == targetPlayer2) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        setSwap(*FIGHTER_MANAGER->entryManager,
                setEffect,
                FIGHTER_MANAGER->getEntryIdFromIndex(targetPlayer1),
                FIGHTER_MANAGER->getEntryIdFromIndex(targetPlayer2),
                1,
                duration*60);
        return RESULT_EFFECT_SUCCESS;
    }
}

//// Credit: Kapedani
EXIStatus effectStatusGiveFinalSmash(u16 numPlayers, u16 targetPlayer, bool setEffect) {
    // TODO: Check if other effects conflict while player is performing final smash
    // Should have memory extended for FighterEffect and FighterTechqniq otherwise would need to handle one person receiving Final Smash at a time

    if (targetPlayer == MAX_PLAYERS) {
        targetPlayer = randi(numPlayers);
    }
    if (targetPlayer == MAX_PLAYERS + 1) {
        // give all players Final Smash
        for (u16 targetPlayer = 0; targetPlayer < numPlayers; targetPlayer++) {
            ftEntry* ftEntryPtr = getFighter(targetPlayer)->getOwner()->ftInputPtr->ftEntryPtr;
            //if (setEffect) {
            ftEntryPtr->setFinal(0);
            //}
            //else {
            //    endFinal(*getFighterPtr(targetPlayer), 0, 1,0);
            //}
        }
    }
    else if (targetPlayer >= numPlayers) {
        return RESULT_EFFECT_UNAVAILABLE;
    }
    else {
        ftEntry* ftEntryPtr = getFighter(targetPlayer)->getOwner()->ftInputPtr->ftEntryPtr;
        //if (setEffect) {
        ftEntryPtr->setFinal(0);
        //}
        //else {
            //endFinal(*getFighterPtr(targetPlayer), 0, 1,0); // will get Landmaster stuck if activated during, also doesn't remove the effect
        //}
    }

    return RESULT_EFFECT_SUCCESS;
}


// 80820430 remove/[ftSlot]