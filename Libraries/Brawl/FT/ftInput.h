#ifndef PROJECTMCODES_FTINPUT_H
#define PROJECTMCODES_FTINPUT_H

struct FtEntry;
struct AiInput;

#define __get_type_ftInput ((int (*)(FtInput* self)) 0x8082f094)
struct FtInput {
  int getType() { return __get_type_ftInput(this); }
  char _spacer[0x4];

  // 0x4
  FtEntry* ftEntryPtr;

  char _spacer2[0xC - 0x4 - 4];
  // 0xC
  AiInput* aiInputMain;
  // 0x10
  AiInput* aiInputSub;
};


#endif //PROJECTMCODES_FTINPUT_H