#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <fstream>
#include <iostream>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>

using namespace std;
using namespace boost::iostreams;

int main(int argc, char *argv[]) {
	ofstream fs;
	filtering_ostream gs;

	gs.push(gzip_compressor());
	gs.push(fs);

	return EXIT_SUCCESS;
}
