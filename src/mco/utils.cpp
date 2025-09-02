#include <cstdio>
#include <mco/utils.hpp>

namespace mco {
	std::string makeHumanReadableByteSize(u64 bytes) {
		char buf[64];
		constexpr static auto unit = 1024;
		constexpr static char unitExponents[] = "kMG";

		usize exp {};
		usize div = unit;

		if(bytes < unit) {
			sprintf(buf, "%zu B", bytes);
			return buf;
		} else {
			for(std::uint64_t i = bytes / unit; i >= unit; i /= unit) {
				// Break out if we're gonna set the exponent too high
				if((exp + 1) > 2)
					break;

				div *= unit;
				exp++;
			}
		}

		sprintf(buf, "%0.2f %cB", float(bytes) / float(div), unitExponents[exp]);
		return buf;
	}
} // namespace mco