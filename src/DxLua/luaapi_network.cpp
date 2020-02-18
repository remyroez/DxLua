#include <tuple>

#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>

#include "luaapi.h"

namespace DxLua::detail {

void port_network(sol::state_view &lua, sol::table &t) {
#ifndef DX_NON_NETWORK

	DXLUA_PORT(t, ProcessNetMessage);

	t["GetHostIPbyName"] = [](const TCHAR *HostName, sol::variadic_args va) {
		std::vector<IPDATA> ipdatas;
		auto size = va.leftover_count() > 0 && va[0].is<int>() ? va[0].as<int>() : 1;
		ipdatas.resize(size, { 0, 0, 0, 0 });
		int num = 0;
		auto result = GetHostIPbyName(HostName, ipdatas.data(), ipdatas.size(), &num);
		return std::make_tuple(result, sol::as_table(ipdatas), num);
	};
	t["GetHostIPbyName_IPv6"] = [](const TCHAR *HostName, sol::variadic_args va) {
		std::vector<IPDATA_IPv6> ipdatas;
		auto size = va.leftover_count() > 0 && va[0].is<int>() ? va[0].as<int>() : 1;
		ipdatas.resize(size, { 0, 0, 0, 0 });
		int num = 0;
		auto result = GetHostIPbyName_IPv6(HostName, ipdatas.data(), ipdatas.size(), &num);
		return std::make_tuple(result, sol::as_table(ipdatas), num);
	};
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
	t["ConnectNetWork_IPv6"] = [](const sol::table table, sol::variadic_args va) {
		IPDATA_IPv6 ipdata{ 0 };
		if (sol::object ScopeID = table["ScopeID"]; ScopeID.valid() && ScopeID.is<int>()) {
			ipdata.ScopeID = ScopeID.as<int>();
		}

		int port = -1;
		if (sol::object p = table["port"]; p.valid() && p.is<int>()) {
			port = p.as<int>();
		} else if (va.leftover_count() > 0 && va[0].is<int>()) {
			port = va[0].as<int>();
		}

		return ConnectNetWork_IPv6(ipdata, port);
	};
	t["ConnectNetWork_ASync"] = [](const sol::table table, sol::variadic_args va) {
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

		return ConnectNetWork_ASync(ipdata, port);
	};
	t["ConnectNetWork_IPv6_ASync"] = [](const sol::table table, sol::variadic_args va) {
		IPDATA_IPv6 ipdata{ 0 };
		if (sol::object ScopeID = table["ScopeID"]; ScopeID.valid() && ScopeID.is<int>()) {
			ipdata.ScopeID = ScopeID.as<int>();
		}

		int port = -1;
		if (sol::object p = table["port"]; p.valid() && p.is<int>()) {
			port = p.as<int>();
		} else if (va.leftover_count() > 0 && va[0].is<int>()) {
			port = va[0].as<int>();
		}

		return ConnectNetWork_IPv6_ASync(ipdata, port);
	};
	t["PreparationListenNetWork"] = [](sol::variadic_args va) {
		return PreparationListenNetWork(va.leftover_count() > 0 && va[0].is<int>() ? va[0].as<int>() : -1);
	};
	t["PreparationListenNetWork_IPv6"] = [](sol::variadic_args va) {
		return PreparationListenNetWork_IPv6(va.leftover_count() > 0 && va[0].is<int>() ? va[0].as<int>() : -1);
	};
	DXLUA_PORT(t, StopListenNetWork);
	DXLUA_PORT(t, CloseNetWork);

	DXLUA_PORT(t, GetNetWorkAcceptState);
	DXLUA_PORT(t, GetNetWorkDataLength);
	DXLUA_PORT(t, GetNetWorkSendDataLength);
	DXLUA_PORT(t, GetNewAcceptNetWork);
	DXLUA_PORT(t, GetLostNetWork);
	t["GetNetWorkIP"] = [](int NetHandle) {
		IPDATA ipdata;
		auto result = GetNetWorkIP(NetHandle, &ipdata);
		return std::make_tuple(result, ipdata);
	};
	t["GetNetWorkIP_IPv6"] = [](int NetHandle) {
		IPDATA_IPv6 ipdata;
		auto result = GetNetWorkIP_IPv6(NetHandle, &ipdata);
		return std::make_tuple(result, ipdata);
	};
	//DXLUA_PORT(t, GetMyIPAddress);
	//DXLUA_PORT(t, GetMyIPAddress_IPv6);
	DXLUA_PORT(t, SetConnectTimeOutWait);
	DXLUA_PORT(t, SetUseDXNetWorkProtocol);
	DXLUA_PORT(t, GetUseDXNetWorkProtocol);
	DXLUA_PORT(t, SetUseDXProtocol);
	DXLUA_PORT(t, GetUseDXProtocol);
	DXLUA_PORT(t, SetNetWorkCloseAfterLostFlag);
	DXLUA_PORT(t, GetNetWorkCloseAfterLostFlag);

	t["NetWorkRecv"] = [](int NetHandle, int Length) {
		std::vector<std::byte> buffer;
		buffer.resize(Length, std::byte(0));
		auto result = NetWorkRecv(NetHandle, buffer.data(), Length);
		return std::make_tuple(result, sol::as_table(buffer));
	};
	t["NetWorkRecvToPeek"] = [](int NetHandle, int Length) {
		std::vector<std::byte> buffer;
		buffer.resize(Length, std::byte(0));
		auto result = NetWorkRecvToPeek(NetHandle, buffer.data(), Length);
		return std::make_tuple(result, sol::as_table(buffer));
	};
	DXLUA_PORT(t, NetWorkRecvBufferClear);
	t["NetWorkSend"] = [](int NetHandle, sol::object object, sol::variadic_args va) {
		std::vector<std::byte> buffer;
		auto size = va.leftover_count() > 0 && va[0].is<int>() ? va[0].as<int>() : -1;
		if (object.is<sol::table>()) {
			auto table = object.as<sol::table>();
			for (const auto &[k, v] : table) {
				buffer.push_back(std::byte(v.as<int>()));
			}

		} else if (object.is<std::string>()) {
			auto string = object.as<std::string>();
			for (auto c : string) {
				buffer.push_back(std::byte(c));
			}
			buffer.push_back(std::byte());

		} else if (object.is<int>()) {
			auto number = object.as<int>();
			for (auto i = 0; i < sizeof(int); ++i) {
				buffer.push_back(std::byte(number & UCHAR_MAX));
				number >>= CHAR_BIT;
			}
		}
		return NetWorkSend(NetHandle, buffer.data(), buffer.size());
	};

	t["MakeUDPSocket"] = [](sol::variadic_args va) {
		return MakeUDPSocket(va.leftover_count() > 0 && va[0].is<int>() ? va[0].as<int>() : -1);
	};
	t["MakeUDPSocket_IPv6"] = [](sol::variadic_args va) {
		return MakeUDPSocket_IPv6(va.leftover_count() > 0 && va[0].is<int>() ? va[0].as<int>() : -1);
	};
	DXLUA_PORT(t, DeleteUDPSocket);
	//DXLUA_PORT(t, NetWorkSendUDP);
	//DXLUA_PORT(t, NetWorkSendUDP_IPv6);
	//DXLUA_PORT(t, NetWorkRecvUDP);
	//DXLUA_PORT(t, NetWorkRecvUDP_IPv6);
	DXLUA_PORT(t, CheckNetWorkRecvUDP);

#endif // DX_NON_NETWORK
}

} // namespace DxLua::detail
