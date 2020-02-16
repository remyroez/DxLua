#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>

#include "luaapi.h"

namespace DxLua::detail {

void port_network(sol::state_view &lua, sol::table &t) {
	DXLUA_PORT(t, ProcessNetMessage);
#if 0
	t["GetHostIPbyName"] = [](const TCHAR *HostName) -> sol::variadic_results {
		std::vector<IPDATA> ipdatas;
		ipdatas.resize(10, { 0, 0, 0, 0 });
		int num = 0;
		auto result = GetHostIPbyName(HostName, ipdatas.data(), ipdatas.size(), &num);
		return ipdatas;
	};
#endif
	DXLUA_PORT(t, GetHostIPbyName);
	DXLUA_PORT(t, GetHostIPbyName_IPv6);

	t["ConnectNetWork"] = [](const sol::table table, sol::variadic_args va) {
		IPDATA ipdata{ 0, 0, 0, 0 };
		if (sol::object d1 = table["d1"]; d1.valid() && d1.is<int>()) {
			ipdata.d1 = d1.as<int>();
		}
		if (sol::object d2 = table["d2"]; d2.valid() && d2.is<int>()) {
			ipdata.d2 = d2.as<int>();
		}
		if (sol::object d3 = table["d3"]; d3.valid() && d3.is<int>()) {
			ipdata.d3 = d3.as<int>();
		}
		if (sol::object d4 = table["d4"]; d4.valid() && d4.is<int>()) {
			ipdata.d4 = d4.as<int>();
		}

		int port = -1;
		if (sol::object p = table["port"]; p.valid() && p.is<int>()) {
			port = p.as<int>();
		} else if (va.leftover_count() > 0 && va[0].is<int>()) {
			port = va[0].as<int>();
		}

		return ConnectNetWork(ipdata, port);
	};
}

} // namespace DxLua::detail
