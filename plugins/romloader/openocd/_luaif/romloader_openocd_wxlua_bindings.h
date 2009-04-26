// ---------------------------------------------------------------------------
// romloader_openocd_lua.h - headers and wxLua types for wxLua binding
//
// This file was generated by genwxbind.lua 
// Any changes made to this file will be lost when the file is regenerated
// ---------------------------------------------------------------------------

#ifndef __HOOK_WXLUA_romloader_openocd_lua_H__
#define __HOOK_WXLUA_romloader_openocd_lua_H__


#include "wxlua/include/wxlstate.h"
#include "wxlua/include/wxlbind.h"

// ---------------------------------------------------------------------------
// Check if the version of binding generator used to create this is older than
//   the current version of the bindings.
//   See 'bindings/genwxbind.lua' and 'modules/wxlua/include/wxldefs.h'
#if WXLUA_BINDING_VERSION > 26
#   error "The WXLUA_BINDING_VERSION in the bindings is too old, regenerate bindings."
#endif //WXLUA_BINDING_VERSION > 26
// ---------------------------------------------------------------------------

// binding class
class WXLUA_NO_DLLIMPEXP wxLuaBinding_romloader_openocd_lua : public wxLuaBinding
{
public:
    wxLuaBinding_romloader_openocd_lua();


private:
    DECLARE_DYNAMIC_CLASS(wxLuaBinding_romloader_openocd_lua)
};


// initialize wxLuaBinding_romloader_openocd_lua for all wxLuaStates
extern WXLUA_NO_DLLIMPEXP bool wxLuaBinding_romloader_openocd_lua_init();

// ---------------------------------------------------------------------------
// Includes
// ---------------------------------------------------------------------------

#include "../../romloader.h"
#include "../romloader_openocd_main.h"

// ---------------------------------------------------------------------------
// Lua Tag Method Values and Tables for each Class
// ---------------------------------------------------------------------------

extern WXLUA_NO_DLLIMPEXP_DATA(int) wxluatype_romloader;
extern WXLUA_NO_DLLIMPEXP wxLuaBindMethod romloader_methods[];
extern WXLUA_NO_DLLIMPEXP_DATA(int) romloader_methodCount;


// ---------------------------------------------------------------------------
// Encapsulation Declarations - need to be public for other bindings.
// ---------------------------------------------------------------------------


#endif // __HOOK_WXLUA_romloader_openocd_lua_H__

