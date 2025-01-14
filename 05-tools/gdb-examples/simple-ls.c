/*
 * simple-ls.c
 * Extremely low-power ls clone.
 * ./simple-ls .
 */

#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void
printOwner(char *name, struct stat sb) {
	struct passwd *pwd;
	if ((pwd = getpwuid(sb.st_uid)) == NULL) {
		printf("%s (%d)\n", name, sb.st_uid);
	} else {
		printf("%s (%s)\n", name, pwd->pw_name);
	}
}

int
main(int argc, char **argv) {

	DIR *dp;
	struct dirent *dirp;

	if (argc != 2) {
		fprintf(stderr, "usage: %s dir_name\n", argv[0]);
		exit(1);
	}

	if ((dp = opendir(argv[1])) == NULL) {
		fprintf(stderr, "can't open '%s'\n", argv[1]);
		exit(1);
	}

	if (chdir(argv[1]) == -1) {
		fprintf(stderr, "can't chdir to '%s': %s\n", argv[1], strerror(errno));
		exit(EXIT_FAILURE);
	}

	while ((dirp = readdir(dp)) != NULL) {
		struct stat sb;
		if (stat(dirp->d_name, &sb) == -1) {
			fprintf(stderr, "Can't stat %s: %s\n", dirp->d_name,
						strerror(errno));
			continue;
		}
		printOwner(dirp->d_name, sb);
	}

	closedir(dp);
	return(0);
}
