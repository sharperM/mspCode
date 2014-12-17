#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#ifdef __cplusplus
}
#endif
#include <string>
void error(lua_State *L, const char *fmt, ...);
void load(lua_State *L, const char *fname, int *w, int *h);
void runLua(std::string filePath);
void useLuaFun();
void useCfunction();
void testStack();
void BindingClass();
void BindingClass2();
void BindingClass3();