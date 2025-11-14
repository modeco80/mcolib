//! Win32 implementation of FileStream.

#define _WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <mco/base_types.hpp>
#include <mco/io/file_stream.hpp>
#include <system_error>

namespace mco::impl {

	// helper to convert mco generic modes to posix mode
	constexpr int convertToCreateFileAccess(int mode) {
		int newMode = 0;
		if(mode & mco::FileStream::Read)
			newMode |= GENERIC_READ;
		if(mode & mco::FileStream::Write)
			newMode |= GENERIC_WRITE;
		return newMode;
	}

	constexpr int convertToCreateFileDisposition(int mode) {
		int disposition = OPEN_EXISTING;
		if(mode & mco::FileStream::Create)
			disposition = CREATE_ALWAYS;
		return disposition;
	}

	struct FileStreamImpl {
		FileStreamImpl() = default;
		FileStreamImpl(FileStreamImpl&& mv) {
			hFile = mv.hFile;
			mv.hFile = INVALID_HANDLE_VALUE;
		}

		~FileStreamImpl() {
			close();
		}

		void open(const char* path, int mcoMode) {
			// do the dance
			hFile = CreateFileA(
			path,
			convertToCreateFileAccess(mcoMode),	 // dwDesiredAccess
			FILE_SHARE_READ | FILE_SHARE_DELETE, // dwShareMode
			nullptr,
			convertToCreateFileDisposition(mcoMode), // dwCreationDisposition
			FILE_ATTRIBUTE_NORMAL,
			INVALID_HANDLE_VALUE);

			if(auto le = GetLastError(); le != 0) {
				if(le != ERROR_ALREADY_EXISTS) {
					// TODO: throw an exception for other lasterrors.
					// this LastError is not fatal
					return;
				}
			}

			// Cache size
			seek(0, Stream::End);
			size = tell();
			seek(0, Stream::Begin);
		}

		u64 read(void* buffer, u64 length) {
			DWORD nRead;
			if(!ReadFile(hFile, buffer, length, &nRead, nullptr)) {
				return -1;
			}
			return nRead;
		}

		u64 write(const void* buffer, u64 length) {
			DWORD nRead;
			if(!WriteFile(hFile, buffer, length, &nRead, nullptr)) {
				return -1;
			}
			return nRead;
		}

		i64 seek(i64 offset, Stream::Whence whence) {
			int windowsWhence;
			switch(whence) {
				case Stream::Begin:
					windowsWhence = FILE_BEGIN;
					break;
				case Stream::Current:
					windowsWhence = FILE_CURRENT;
					break;
				case Stream::End:
					windowsWhence = FILE_END;
					break;
				default:
					// This actually should probably assert
					return -1;
					break;
			}
			LARGE_INTEGER liNewFp {};
			LARGE_INTEGER liOffset = {
				.QuadPart = offset
			};
			SetFilePointerEx(hFile, liOffset, &liNewFp, windowsWhence);
			return liNewFp.QuadPart;
		}

		i64 tell() {
			return seek(0, Stream::Current);
		}

		void truncate(u64 size) {
			// i think there's an API for this I just don't remember it.
			static_cast<void>(size);
			return;
		}

		void close() {
			if(hFile != INVALID_HANDLE_VALUE) {
				CloseHandle(hFile);
				hFile = INVALID_HANDLE_VALUE;
			}
		}

		u64 getSize() {
			return size;
		}

		bool hasEnded() {
			return tell() == static_cast<i64>(size);
		}

	   private:
		HANDLE hFile { INVALID_HANDLE_VALUE };
		u64 size;
	};

} // namespace mco::impl