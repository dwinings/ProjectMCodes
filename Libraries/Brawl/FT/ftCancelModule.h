//
// Created by dareb on 9/19/2020.
//

#ifndef PROJECTMCODES_FTCANCELMODULE_H
#define PROJECTMCODES_FTCANCELMODULE_H

typedef union {
    unsigned short value;
    struct {
        /* I think these are the 4 transition groups, I've only seen 0 and 2 used. */
        unsigned short cancelGroup1 : 1;
        unsigned short cancelGroup2 : 1;
        unsigned short cancelGroup3 : 1;
        unsigned short cancelGroup4 : 1;
        unsigned short unknown : 12;
    }__attribute__((packed, aligned(2))) bits;
} CancelGroups;

struct ftCancelModule {
    int isEnableCancel();
    int unableCancelStatus();
    void enableCancelTransitionGroup(int groupID);

    char _spacer[0x34];
    int unknown0;

    CancelGroups cancelGroups;
};

#define _unableCancelStatus_ftCancelModule ((int (*)(ftCancelModule* self)) 0x8084bd74)
#define _isEnableCancel_ftCancelModule ((int (*)(ftCancelModule* self)) 0x8084bd6c)
#define _enableCancelTransitionGroup_ftCancelModule ((void (*)(ftCancelModule* self, int groupID)) 0x8084B6D8)

#endif //PROJECTMCODES_FTCANCELMODULE_H
