#ifndef INPUTS
#define INPUTS

#include <stdbool.h>
#include <windows.h>

typedef struct HInputs
{
    HHOOK hHook;
    MSG msg;
    int (*stopFunction)(struct HInputs*, POINT);
    POINT (*customParamFunction)(void);
    void (*customFunction)(void);
    bool isListening;
    bool error;
} HInputs;

int inputs_INIT(HInputs *hInputs);

int inputs_startListening(HInputs *hInputs);

void inputs_stopListening(HInputs * hInputs);

void inputs_poll(HInputs * hInputs);

#endif