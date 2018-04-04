// removes newlines from text files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int main(int argc, char *argv[])
{
	char ch;
	int slen, i, j, nch, nfiles;
	long fsize;
	FILE * fpsrc, * fpdest;
	nch = nfiles = 0;

	if (argc == 1)
		printf("Usage:  %s [filename]...\n", argv[0]);	// display usage information
	else
	{
		for (i = 1; i < argc; i++)
		{
			printf("\nProceeding to file \"%s\"...\n", argv[i]);

			if ((fpsrc = fopen(argv[i], "r")) == NULL)
			{
				printf("Could not open \"%s\" for reading!\n",  argv[i]);
				exit(EXIT_FAILURE);
			}
			fseek(fpsrc, 0L, SEEK_END);
			fsize = ftell(fpsrc);
			rewind(fpsrc);

			slen = strlen(argv[i]) + 5;
			char fname[slen];
			strncpy(fname, argv[i], slen - 5);
			fname[slen - 5] = '\0';
			strcat(fname, ".nnl");
			
			if ((fpdest = fopen(fname, "w")) == NULL)
			{
				printf("Could not open \"%s\" for writing!\n", fname);
				exit(EXIT_FAILURE);
			}
			fseek(fpdest, 0L, SEEK_SET);

			printf("Writing to \"%s\"...\n", fname);
			for (j = 1; j <= fsize; j++)
			{
				if ((ch = getc(fpsrc)) == '\n')
				{
					if (ferror(fpsrc))
					{
						printf("Error reading from \"%s\" at byte %d!\n", argv[i], j);
						exit(EXIT_FAILURE);
					}
					continue;
				} else
				{
					putc(ch, fpdest);
					if (!isspace(ch))
						nch++;
					if (ferror(fpdest))
					{
						printf("Error writing to \"%s\" at byte %d!\n", fname, j);
						exit(EXIT_FAILURE);
					}
					if (nch == 30)
					{
						putc('\n', fpdest);
						nch = 0;
					}
				}
			}
			putc('\n', fpdest);
			nfiles++;
			printf("Successfully wrote file \"%s\" to \"%s\"!\n", argv[i], fname);
			fclose(fpsrc);
			fclose(fpdest);
		}
	}
	printf("\nDone! Wrote %u ", nfiles);
	if (nfiles == 1)
		puts("file.");
	else
		puts("files.");

	return 0;
}
