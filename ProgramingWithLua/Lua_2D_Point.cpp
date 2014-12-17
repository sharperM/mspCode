/*
** Lua binding: point2
** Generated automatically by tolua++-1.0.92 on 12/12/14 15:50:46.
*/
#include "stdafx.h"

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_point2_open (lua_State* tolua_S);

#include "1D_Point.h"
#include "2D_Point.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_TwoD_Point (lua_State* tolua_S)
{
 TwoD_Point* self = (TwoD_Point*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"OneD_Point");
 tolua_usertype(tolua_S,"TwoD_Point");
}

/* get function: y of class  TwoD_Point */
#ifndef TOLUA_DISABLE_tolua_get_TwoD_Point_y
static int tolua_get_TwoD_Point_y(lua_State* tolua_S)
{
  TwoD_Point* self = (TwoD_Point*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  TwoD_Point */
#ifndef TOLUA_DISABLE_tolua_set_TwoD_Point_y
static int tolua_set_TwoD_Point_y(lua_State* tolua_S)
{
  TwoD_Point* self = (TwoD_Point*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((double)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  TwoD_Point */
#ifndef TOLUA_DISABLE_tolua_point2_TwoD_Point_new00
static int tolua_point2_TwoD_Point_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TwoD_Point",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double x = ((double)  tolua_tonumber(tolua_S,2,0));
  double y = ((double)  tolua_tonumber(tolua_S,3,0));
  {
   TwoD_Point* tolua_ret = (TwoD_Point*)  Mtolua_new((TwoD_Point)(x,y));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"TwoD_Point");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  TwoD_Point */
#ifndef TOLUA_DISABLE_tolua_point2_TwoD_Point_new00_local
static int tolua_point2_TwoD_Point_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"TwoD_Point",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  double x = ((double)  tolua_tonumber(tolua_S,2,0));
  double y = ((double)  tolua_tonumber(tolua_S,3,0));
  {
   TwoD_Point* tolua_ret = (TwoD_Point*)  Mtolua_new((TwoD_Point)(x,y));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"TwoD_Point");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  TwoD_Point */
#ifndef TOLUA_DISABLE_tolua_point2_TwoD_Point_delete00
static int tolua_point2_TwoD_Point_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TwoD_Point",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TwoD_Point* self = (TwoD_Point*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: dosomething of class  TwoD_Point */
#ifndef TOLUA_DISABLE_tolua_point2_TwoD_Point_dosomething00
static int tolua_point2_TwoD_Point_dosomething00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"TwoD_Point",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  TwoD_Point* self = (TwoD_Point*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'dosomething'", NULL);
#endif
  {
   double tolua_ret = (double)  self->dosomething();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dosomething'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_point2_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"TwoD_Point","TwoD_Point","OneD_Point",tolua_collect_TwoD_Point);
  #else
  tolua_cclass(tolua_S,"TwoD_Point","TwoD_Point","OneD_Point",NULL);
  #endif
  tolua_beginmodule(tolua_S,"TwoD_Point");
   tolua_variable(tolua_S,"y",tolua_get_TwoD_Point_y,tolua_set_TwoD_Point_y);
   tolua_function(tolua_S,"new",tolua_point2_TwoD_Point_new00);
   tolua_function(tolua_S,"new_local",tolua_point2_TwoD_Point_new00_local);
   tolua_function(tolua_S,".call",tolua_point2_TwoD_Point_new00_local);
   tolua_function(tolua_S,"delete",tolua_point2_TwoD_Point_delete00);
   tolua_function(tolua_S,"dosomething",tolua_point2_TwoD_Point_dosomething00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_point2 (lua_State* tolua_S) {
 return tolua_point2_open(tolua_S);
};
#endif

