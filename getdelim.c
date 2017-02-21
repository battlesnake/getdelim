#if 0
(
set -e
declare -r tmp="$(mktemp)"
trap "rm -f -- '$tmp'" EXIT ERR
gcc -DTEST_getdelim -std=gnu11 -Ic_modules -o "$tmp" $(find -name '*.c')
echo 'Newline'
printf "%s\n" potato lemon lime onion | valgrind --quiet --leak-check=full --track-origins=yes "$tmp"
echo 'Comma'
printf "%s," potato lemon lime onion | valgrind --quiet --leak-check=full --track-origins=yes "$tmp" ','
echo 'Null'
printf "%s\0" potato lemon lime onion | valgrind --quiet --leak-check=full --track-origins=yes "$tmp" $'\0'
)
exit 0
#endif
#include "getdelim.h"

bool fstr_getdelim(struct fstr *fs, char delim, FILE *in)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t res = getdelim(&line, &len, delim, in);
	if (res > 0) {
		size_t len = res - 1;
		line[len] = 0;
		fstr_destroy(fs);
		/* Force null-termination */
		fstr_init_takeown_n(fs, line, len);
		return true;
	} else {
		free(line);
		fstr_set_null(fs);
		return false;
	}
}

bool fstr_getline(struct fstr *fs, FILE *in)
{
	return fstr_getdelim(fs, '\n', in);
}

#if defined TEST_getdelim
int main(int argc, char *argv[])
{
	struct fstr fs = FSTR_INIT;
	char delim = '\n';
	if (argc == 2) {
		delim = argv[1][0];
	}
	while (fstr_getdelim(&fs, delim, stdin)) {
		/* printf(" - [" PRIfs "]\n", prifs(&fs)); */
		printf(" - [%s]\n", fstr_get(&fs));
	}
	fstr_destroy(&fs);
	printf("Done (errno=%d)\n\n", errno);
	return 0;
}
#endif
