#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

using namespace std;

#include <xdiff.h>

static int read_file(mmfile_t *ptr, const char *filename)
{
	struct stat st;
	FILE *f;

	if (stat(filename, &st)) {
		cerr << "could not stat " << filename << endl;
		return 1;
	}
	if ((f = fopen(filename, "rb")) == NULL) {
		cerr << "could not open " << filename << endl;
		return 1;
	}
	ptr->ptr = (char*)malloc(st.st_size);
	if (!ptr->ptr) {
		cerr << "could not allocate " << st.st_size << " bytes" << endl;
		return 1;
	}
	if (fread(ptr->ptr, st.st_size, 1, f) != 1) {
		cerr << "could not read " << filename << endl;
		return 1;
	}
	fclose(f);
	ptr->size = st.st_size;
	return 0;
}


int main(int argc, char** argv)
{
	char* names[3];
	mmfile_t mmfs[3];
	mmbuffer_t result = {NULL, 0};
	xpparam_t xpp = {XDF_NEED_MINIMAL};
	int i, ret;

	if (argc < 5) {
		cerr << "usage: merge <base> <a> <b> <out>" << endl;
		return 1;
	}

	for (i = 0; i < 3; i++) {
		names[i] = argv[i + 1];
		if (read_file(mmfs + i, argv[i + 1]))
			return 1;
	}

	ret = xdl_merge(mmfs + 0, mmfs + 1, names[1], mmfs + 2, names[2],
			&xpp, XDL_MERGE_ZEALOUS, &result);

	for (i = 0; i < 3; i++)
		free(mmfs[i].ptr);

	if (ret >= 0) {
		char *filename = argv[4];
		FILE *f = fopen(filename, "wb");
		fwrite(result.ptr, result.size, 1, f);
		fclose(f);
		free(result.ptr);
	}
	return ret;
}
