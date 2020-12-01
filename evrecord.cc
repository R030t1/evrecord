#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <fstream>
#include <iostream>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include <linux/version.h>
#include <linux/input.h>

#include "names.h"

using namespace std;
using namespace boost::iostreams;

int main(int argc, char *argv[]) {
	ofstream fs;
	filtering_ostream gs;

	gs.push(gzip_compressor());
	gs.push(fs);

	for (int i = 0; events[i]; i++)
		printf("%s\n", events[i]);

	for (int i = 0; keys[i]; i++)
		printf("%s\n", keys[i]);

	return EXIT_SUCCESS;
}
