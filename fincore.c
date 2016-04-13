#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>

static int pages(size_t bytes) {

	if (bytes % sysconf(_SC_PAGESIZE))
		return bytes/sysconf(_SC_PAGESIZE) + 1;
	else
		return bytes/sysconf(_SC_PAGESIZE);
}

static int report(const char *func, int val) {

	fprintf(stderr,"%s: %d = %s.\n",
		func, errno, strerror(errno));
	return val;
}


int run(const char *file) {

	int fd = open(file, O_RDONLY);
	if ( fd < 0 )
		return report("open", fd);

	struct stat fstat;
	if ( stat(file, &fstat) == -1 )
		return report("stat", -1);

	void *mmio = mmap(NULL, fstat.st_size, PROT_READ,
			  MAP_SHARED, fd, 0);
	if ( mmio == NULL )
		return report("mmap", -1);

	char *res = malloc(pages(fstat.st_size));
	if ( res==NULL )
		return report("malloc", -1);

	if ( mincore(mmio, fstat.st_size, res) == -1 )
		return report("mincore", -1);

	size_t cached = 0;
	for (unsigned i=0; i<pages(fstat.st_size); i++)
		if ( res[i] & 0x1 )
			cached++;
	
	fprintf(stdout, "%s : %zd / %d : %zd\n", file, cached,
		pages(fstat.st_size), fstat.st_size);
	
free:
	free(res);
unmap:
	munmap(mmio, fstat.st_size);
close:
	close(fd);

	
}

int main(int argc, char **argv) {

	run(argv[1]);
	return 0;
}
