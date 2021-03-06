#include <stddef.h>

static int argc, argi;
static char **argv, *flag;

static char *
nextarg()
{
	char *temp = flag;
	flag = NULL;
	if (temp[1])
		return temp + 1;
	if (++argi == argc)
		return NULL;
	return argv[argi];
}

static void
args_parse(int argc_, char **argv_,
    void flagfn(char, char*()), void plainfn(char*))
{
	argc = argc_;
	argv = argv_;
	for (argi = 1; argi < argc; argi++) {
		char *arg = argv[argi];
		if (!arg[0])
			continue;

		/* if ... || arg == "-" */
		if (arg[0] != '-' || !arg[1]) {
			if (plainfn)
				plainfn(arg);
			continue;
		}
		/* if arg == "--" */
		if (arg[1] == '-' && !arg[2]) {
			argi++;
			break;
		}

		for (flag = arg + 1; *flag; flag++) {
			flagfn(*flag, nextarg);
			if (!flag)
				break;
		}
	}
	for (; argi < argc; argi++)
		if (plainfn)
			plainfn(argv[argi]);
}
