#pragma once

BOOL ProcessResolverStatus(int opCode, const void* const restrict data);

static UINT ResolverOpCodeToFormatId(int opCode);

static BOOL OnResolverPassedHostname(BOOL bUpdated,
	int opCode, const char* const restrict hostname);
static BOOL OnResolverPassedIpAddr(BOOL bUpdated,
	int opCode, uint32_t ipAddr);
static BOOL OnResolverGetNameInfo(BOOL bUpdated,
	uint32_t ipAddr, UINT nFormatId);
static void ProcessResolverHostname(const char* hostname,
	UINT nFormatId);
