#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include <linux/version.h>
#include <linux/input.h>

#include "names.h"

namespace fs = std::filesystem;
using namespace std;
using namespace boost::iostreams;

void print_device(int fd);

int main(int argc, char *argv[]) {
	ofstream fs;
	filtering_ostream gs;

	gs.push(gzip_compressor());
	gs.push(fs);

	for (int i = 0; events[i]; i++)
		printf("%s\n", events[i]);

	for (int i = 0; keys[i]; i++)
		printf("%s\n", keys[i]);

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

void print_device(int fd) {
	char name[256] = "Uknown";
	ioctl(fd, EVIOCGNAME(sizeof name), name);
	printf("%s\n", name);
}
