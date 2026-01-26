// SPDX-License-Identifier: GPL-2.0-only

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

static int file = -1;

static void *thread_func(void *data)
{
	write(file, data, strlen(data));
	return NULL;
}

int main(int argc, char **argv)
{
	file = open("/sys/kernel/debug/fortytwo/foo", O_RDWR);

	if (file == -1) {
		printf("ERROR: could not open /sys/kernel/debug/fortytwo/foo (%s)\n", strerror(errno));
		return 1;
	}

	pthread_t threads[5];
	char *thread_data[5] = {
		"Hello\n",
		"World\n",
		"Linux\n",
		"Kernel\n",
		"Module\n",
	};

	for (int i = 0; i < 5; i += 1) {
		pthread_create(&threads[i], NULL, thread_func, thread_data[i]);
	}

	for (int i = 0; i < 5; i += 1) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}
