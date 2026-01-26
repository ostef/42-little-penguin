// SPDX-License-Identifier: GPL-2.0-only

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int file = open("/sys/kernel/debug/fortytwo/id", O_RDWR);

	if (file == -1) {
		printf("ERROR: could not open /sys/kernel/debug/fortytwo/id (%s)\n", strerror(errno));
		return 1;
	}

	int status = 0;

	int n = write(file, "abc", 3);

	if (n != -1 || errno != EINVAL) {
		printf("ERROR: write 'abc' returned %d\n", n);
		status = 1;
	}

	n = write(file, "soumanso", strlen("soumanso"));
	if (n != strlen("soumanso")) {
		printf("ERROR: write 'soumanso' returned %d (%s)\n", n, strerror(errno));
		status = 1;
	}

	n = write(file, "souman", strlen("souman"));
	if (n != -1 || errno != EINVAL) {
		printf("ERROR: write 'souman' returned %d\n", n);
		status = 1;
	}

	char buff[100];

	n = read(file, buff, 3);
	if (n != 3) {
		printf("ERROR: read 3 bytes returned %d\n", n);
		status = 1;
	}
	if (memcmp(buff, "soumanso", 3) != 0) {
		printf("ERROR: buff == '%.*s'\n", 3, buff);
		status = 1;
	}

	n = read(file, buff + 3, sizeof(buff) - 3);
	if (n != strlen("soumanso") - 3) {
		printf("ERROR: read returned %d\n", n);
		status = 1;
	}
	if (memcmp(buff, "soumanso", strlen("soumanso")) != 0) {
		printf("ERROR: buff == '%.*s'\n", (int)strlen("soumanso"), buff);
		status = 1;
	}

	if (status == 1)
		printf("TESTS FAILED\n");
	else
		printf("TESTS PASSED\n");

	close(file);

	return status;
}
