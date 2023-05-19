// Global.h


#ifndef GLOBAL_H
#define GLOBAL_H
/** \brief include all global variable declaration that must be used in separate files
 * \remark the global variable declaration is done in Global.c
*/

extern volatile bool serialPCFlag;
extern volatile bool timerFlag;
extern volatile uint8_t evInputCode;

#endif // GLOBALS_H