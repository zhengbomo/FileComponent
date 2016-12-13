#include "pch.h"
#include "StringHelper.h"

std::wstring stows(std::string s)
{
	std::wstring ws;
	ws.assign(s.begin(), s.end());
	return ws;
}

Platform::String^ stops(std::string s)
{
	return ref new Platform::String(stows(s).c_str());
}

std::string wstos(std::wstring ws)
{
	std::string s;
	s.assign(ws.begin(), ws.end());
	return s;
}

std::string pstos(Platform::String^ ps)
{
	return wstos(std::wstring(ps->Data()));
}