//! POSIX implementation of FileStream.
//! Really this provides a layer FileStream will use
//! to remain as portable as possible across varying
//! POSIX implementations.
#include <fcntl.h>
#include <unistd.h>

#include <system_error>

#include <mco/base_types.hpp>
#include <mco/io/file_stream.hpp>

namespace mco::impl {

	// helper to convert mco generic modes to posix mode
	constexpr int convertToCreateFile(int mode) {
		int posixMode = O_CLOEXEC;
		if(mode & mco::FileStream::Read)
			posixMode |= O_RDONLY;
		if(mode & mco::FileStream::Write)
			posixMode |= O_WRONLY;
		if(mode & mco::FileStream::Create)
			posixMode |= O_CREAT;
		return posixMode;
	}

	struct FileStreamImpl {
        FileStreamImpl() = default;
		FileStreamImpl(FileStreamImpl&& mv) {
			fd = mv.fd;
			mv.fd = -1;
		}

        ~FileStreamImpl() {
            close();
        }

		void open(const char* path, int mcoMode) {
            int mode = convertToCreateFile(mcoMode);
			if(mode & O_CREAT)
				fd = ::open(path, mode | O_CLOEXEC, 0666);
			else
				fd = ::open(path, mode | O_CLOEXEC);

			if(fd == -1) {
				// errno is mappable to system_category
				throw std::system_error { errno, std::generic_category() };
			}

            // Cache size
            seek(0, Stream::End);
            size = tell();
            seek(0, Stream::Begin);
		}

        u64 read(void* buffer, u64 length) {
            auto n = ::read(fd, buffer, length);
            if(n == -1) {
                throw std::system_error { errno, std::generic_category() };
            }
            return n;
        }

        u64 write(const void* buffer, u64 length) {
            auto n = ::write(fd, buffer, length);
            if(n == -1) {
                throw std::system_error { errno, std::generic_category() };
            }
            return n;
        }

        i64 seek(i64 offset, Stream::Whence whence) {
            int posixWhence;
            switch(whence) {
                case Stream::Begin: posixWhence = SEEK_SET; break;
                case Stream::Current: posixWhence = SEEK_CUR; break;
                case Stream::End: posixWhence = SEEK_END; break;
                default:
                    // This actually should probably assert
                    return -1;
                    break;
            }
            return lseek64(fd, offset, posixWhence);
        }

        i64 tell() {
            return seek(0, Stream::Current);
        }

        void truncate(u64 size) {
            ftruncate(fd, size);
        }

        void close() {
            if(fd != -1) {
                ::close(fd);
                fd = -1;
            }
        }

        u64 getSize() {
            return size;
        }
        
        bool hasEnded() {
            return tell() == static_cast<i64>(size);
        }

	   private:
		int fd { -1 };
		u64 size;
	};

} // namespace mco::impl