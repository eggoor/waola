#pragma once

namespace WaolaCli {
	/*
	Keep in sync with Waola::Task
	*/
	public enum class WaolaTask {
		Idle
		, Discover = 0x01
		, Refresh = 0x02
	};
}
