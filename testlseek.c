#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int lseek(int fd, int offset);
int main() {
	int fd = open("testfile", O_CREATE | O_RDWR);
	if (fd < 0) {
		printf(1, "testlseek: Error: Failed to open file\n");
		exit();
	}

	char buf1[] = "Hello";
	if (write(fd, buf1, sizeof(buf1)) < 0) {
		printf(1, "testlseek: Error: Write failed\n");
		close(fd);
		exit();
	}

	if (lseek(fd, 20) < 0) {
		printf(1, "testlseek: Error: lseek failed\n");
		close(fd);
		exit();
	}

	char buf2[] = "World";
	if (write(fd, buf2, sizeof(buf2)) < 0) {
		printf(1, "testlseek: Error: Write failed\n");
		close(fd);
		exit();
	}

	close(fd);
	printf(1, "Test completed successfully. Check 'testfile'.\n");
	exit();

}
