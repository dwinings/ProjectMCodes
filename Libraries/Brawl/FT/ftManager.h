//
// Created by johno on 7/15/2020.
//

#ifndef PROJECTMCODES_FTMANAGER_H
#define PROJECTMCODES_FTMANAGER_H

#include "Fighter.h"

struct FtOwner;
struct AiInput;
struct FtOwner;
struct ftEntryManager;
struct ftSlotManager;

//the port of the fighter
typedef char playerNumber;

#define __getFighterOperationStatus_ftManager ((int (*)(FtManager* self, EntryID entryId)) 0x80814b78)
#define __setFighterOperationStatus_ftManager ((void (*)(FtManager* self, EntryID entryId, int fighterOperationStatus)) 0x80814b3c)
#define __getFighterOperationType_ftManager ((int (*)(FtManager* self, EntryID entryId)) 0x80814c5c)
struct FtManager {
    playerNumber getPlayerNo(EntryID entry);
    FtOwner* getOwner(EntryID entry);
    EntryID getEntryId(playerNumber playerNo);
    EntryID getEntryIdFromIndex(int index);
    // a fighter gftask's task name
    EntryID getEntryIdFromTaskID(int taskId);
    //fighterNo: -1 for load option from ftEntry, 0 for main fighter, 1 for follower
    Fighter* getFighter(EntryID entry, bool getFollower=false);
    Fighter* getSubFighter(EntryID entry);
    unsigned int getFighterCount(EntryID entry);
    // gets the total number of fighters in the match
    unsigned int getFighterNo(EntryID entry);
    AiInput* getInput(EntryID entry);
    void setFighterOperationStatus(EntryID entryId, int fighterOperationStatus) { __setFighterOperationStatus_ftManager(this, entryId, fighterOperationStatus); };
    // 0 is locked I think
    int getFighterOperationStatus (EntryID entryId) { return __getFighterOperationStatus_ftManager(this, entryId); };
    int getFighterOperationType(EntryID entryId) { return __getFighterOperationType_ftManager(this, entryId); };
    bool isFighterActivate(EntryID entry, int getFollower=-1);
    void setSlow(int excludeTeam, int unknown, int slowStrength, int durationInFrames);
    int getEntryCount();
    bool isCpuActive(EntryID entry);

    char _spacer[0x154];
    ftEntryManager* entryManager;
    ftSlotManager* slotManager;
//  ftDataProvider* dataProvider;

};

//static location of global FtManager object
#define FIGHTER_MANAGER ((FtManager*) 0x80629a00)

#define _getPlayerNo_ftManager ((playerNumber (*)(FtManager * self, EntryID entry)) 0x80815ad0)
#define _getOwner_ftManager ((FtOwner* const (*)(FtManager * This, EntryID entry)) 0x808159e4)
#define _getEntryID_ftManager ((EntryID (*)(FtManager * self, playerNumber playerNo)) 0x80815c40)
#define _getEntryIDFromIndex_ftManager ((EntryID (*)(FtManager * self, int index)) 0x80815bf8)
#define _getEntryIDFromTaskId_ftManager ((EntryID (*)(FtManager * self, int taskId, int unk)) 0x80815cb0)

//fighterNo: -1 for load option from ftEntry, 0 for main fighter, 1 for follower
#define _getFighter_ftManager ((Fighter* (*)(FtManager * self, EntryID entry, int fighterNo)) 0x80814f20)
#define _getSubFighter_ftManager ((Fighter* (*)(FtManager * self, EntryID entry)) 0x80814fb8)

//gets the number of fighters attached to the given fighter entryid (ex. ice climbers = 1)
#define _getFighterCount_ftManager ((unsigned int (*)(FtManager * self, EntryID entry)) 0x80814ec4)
// gets the total number of fighters in the match
#define _getFighterNo_ftManager ((unsigned int (*)(FtManager * self, EntryID entry)) 0x808154a0)

#define _getInput_ftManager ((AiInput* (*)(FtManager * self, EntryID entry)) 0x80815a38)

//gets the number of fighters in a match
#define _getEntryCount_ftManager ((int (*)(FtManager * self)) 0x80815be4)

#define _setSlow_ftManager ((void (*)(FtManager * self, int excludeTeam, int unknown, int slowStrength, int durationInFrames)) 0x80817c48)

#define _isCpuActive_ftManager ((bool (*)(FtManager * self, EntryID entry)) 0x80814e5c)
#define _isFighterActivate_ftManager ((bool (*)(FtManager* self, EntryID entry, int getFollower)) 0x80815034)

#endif //PROJECTMCODES_FTMANAGER_H
