#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

#include <boost/asio.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/bzip2.hpp>

#include <linux/version.h>
#include <linux/input.h>

#include "names.h"

namespace fs = std::filesystem;
using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::posix;
using namespace boost::iostreams;

void print_device(int fd);

int main(int argc, char *argv[]) {
	ofstream fs;
	filtering_ostream gs;

	gs.push(bzip2_compressor());
	gs.push(fs);

	int evfd = open(argv[1], O_RDWR);

	// io_service is boost::asio's binding to OS primitives.
	// It maintains a thread pool.
	io_service svc;
	//io_service::work wrk(svc);

	char b[1024];
	stream_descriptor evdev(svc, evfd);

	function<void(system::error_code const&, size_t)> on_receive =
	[&evdev, &b, &on_receive](system::error_code const& ec, size_t bytes) {
		printf("got %ld\n", bytes);
		evdev.async_read_some(buffer(b), on_receive);
	};
	evdev.async_read_some(buffer(b), on_receive);

	//std::thread thd([&svc]() { svc.run(); });
	//thd.join();
	svc.run();

	return EXIT_SUCCESS;

	// Gets all of the top level eventN devices.
	for (const auto &dirent : fs::directory_iterator("/dev/input")) {
		if (!dirent.is_character_file())	
			continue;
		std::cout << dirent << std::endl;
		int fd = open(dirent.path().c_str(), O_RDWR);
		print_device(fd);
		close(fd);
	}

	return EXIT_SUCCESS;
}

static inline const char* type_name(unsigned int type)
{
	return (type <= EV_MAX && events[type]) ? events[type] : "?";
}

static inline const char* code_name(unsigned int type, unsigned int code)
{
	return (type <= EV_MAX && code <= maxval[type] && names[type] && names[type][code]) ? names[type][code] : "?";
}

void print_device(int fd) {
	char name[256] = "Uknown";
	ioctl(fd, EVIOCGNAME(sizeof name), name);
	printf("%s\n", name);

	#define BITS_PER_LONG (sizeof(long) * 8)
	#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
	#define OFF(x)  ((x)%BITS_PER_LONG)
	#define BIT(x)  (1UL<<OFF(x))
	#define LONG(x) ((x)/BITS_PER_LONG)
	#define test_bit(bit, array)	((array[LONG(bit)] >> OFF(bit)) & 1)

	unsigned long bit[EV_MAX][NBITS(KEY_MAX)] = { 0 };
	ioctl(fd, EVIOCGBIT(0, EV_MAX), bit[0]);
	
	for (int type = 0; type < EV_MAX; type++) {
		if (test_bit(type, bit[0]) && type != EV_REP) {
			printf("Event type %d (%s)\n", type, type_name(type));
			if (type == EV_SYN) continue;

			ioctl(fd, EVIOCGBIT(type, KEY_MAX), bit[type]);
			for (int code = 0; code < KEY_MAX; code++) {
				if (test_bit(code, bit[type])) {
					printf("    Event code %d (%s)\n", code, code_name(type, code));
					if (type == EV_ABS) {
						int abs[6] = { 0 };
						// code is axis.
						ioctl(fd, EVIOCGABS(code), abs);
						for (int k = 0; k < 6; k++)
							if ((k < 3) || abs[k])
								printf("        %s %6d\n", absval[k], abs[k]);
					}
				}
			}
		}
	}

	// Repeat handling and properties.

}
