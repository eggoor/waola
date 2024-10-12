#pragma once

namespace WaolaCli {
	/*
	Keep in sync ith Waola::LogLevel
	*/
	public enum class LogLevel {
		llQuiet = -8,
		llPanic = 0,
		llfatal = 8,
		llError = 16,
		llWarning = 24,
		llInfo = 32,
		llVerbose = 40,
		llDebug = 48,
		llTrace = 56
	};
}
