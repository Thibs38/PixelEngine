#include "WREN.hpp"
#include <iostream>

void writeFn(WrenVM* vm, const char* text)
{
	std::cout << text << std::endl;
}

void errorFn(WrenVM* vm, WrenErrorType errorType,
    const char* module, const int line,
    const char* msg)
{
    switch (errorType)
    {
    case WREN_ERROR_COMPILE:
    {
        printf("[%s line %d] [Error] %s\n", module, line, msg);
    } break;
    case WREN_ERROR_STACK_TRACE:
    {
        printf("[%s line %d] in %s\n", module, line, msg);
    } break;
    case WREN_ERROR_RUNTIME:
    {
        printf("[Runtime Error] %s\n", msg);
    } break;
    }
}
void testWren(WrenVM* vm)
{
    const char* module = "main";
    const char* script = "System.print(\"I am running in a VM!\")";

    WrenInterpretResult result = wrenInterpret(vm, module, script);
    WrenHandle* handle = wrenGetSlotHandle(vm, 0);
}

WrenVM* WrenManager::VM;
void WrenManager::init()
{
    WrenConfiguration config;
    wrenInitConfiguration(&config);
    config.writeFn = &writeFn;
    config.errorFn = &errorFn;

    WrenManager::VM = wrenNewVM(&config);

    testWren(WrenManager::VM);
}


 void WrenManager::destroy()
{
	wrenFreeVM(WrenManager::VM);
}