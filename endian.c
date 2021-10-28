#include <stdio.h>


void
printbytes(void *p, unsigned int len)
{
	unsigned int i;
	unsigned char *cp = (unsigned char *)p; 

	for (i = 0; i < len; i++) {
		printf("%02X", *cp++);
	}
}


int
main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	int  myint = 0x1A2B3C4E;
	int pos = 258;
	int neg = (-2);
	float float12 = 12.0;
	float floatneg12 = (-12.0);
	
	printf("The bytes in memory for signed integer 0x%08x are ", myint);
	printbytes(&myint, sizeof(myint));
	printf("\n");

	printf("The bytes in memory for signed integer %d are ", pos);
	printbytes(&pos, sizeof(pos));
	printf("\n");

	printf("The bytes in memory for signed integer %d are ", neg);
	printbytes(&neg, sizeof(neg));
	printf("\n");

	printf("The bytes in memory for float %f are ", float12);
	printbytes(&float12, sizeof(float12));
	printf("\n");
   
	printf("The bytes in memory for float %f are ", floatneg12);
	printbytes(&floatneg12, sizeof(floatneg12));
	printf("\n");   
}