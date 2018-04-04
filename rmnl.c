// removes newlines from text files
#include <stdio.h>
#include <stdlib.h>	// for exit()
#include <string.h>	// for strlen() strncpy() and strcat()
#include <ctype.h>	// for isspace()
#define LINELIMIT 30	// limit of characters per line in output file
int main(int argc, char *argv[])
{
	char ch;
	int slen, i, j, nch, nfiles;
	long fsize;
	FILE * fpsrc, * fpdest;
	nch = nfiles = 0;

	if (argc == 1)
		printf("Usage:  %s [filename]...\n", argv[0]);		// display usage information
	else
	{
		for (i = 1; i < argc; i++)					// for every command line argument
		{
			printf("\nProceeding to file \"%s\"...\n", argv[i]);	// state source file

			if ((fpsrc = fopen(argv[i], "r")) == NULL)
			{
				printf("Could not open \"%s\" for reading!\n",  argv[i]);
				exit(EXIT_FAILURE);
			}
			fseek(fpsrc, 0L, SEEK_END);
			fsize = ftell(fpsrc);			// find size of source file
			rewind(fpsrc);

			slen = strlen(argv[i]) + 5;
			char fname[slen];
			strncpy(fname, argv[i], slen - 5);	// set destination filename to source filename
			fname[slen - 5] = '\0';
			strcat(fname, ".nnl");			// append .nnl to destination filename
			
			if ((fpdest = fopen(fname, "w")) == NULL)
			{
				printf("Could not open \"%s\" for writing!\n", fname);
				exit(EXIT_FAILURE);
			}
			fseek(fpdest, 0L, SEEK_SET);		

			printf("Writing to \"%s\"...\n", fname);
			for (j = 1; j <= fsize; j++)			// for each byte in source file
			{
				if ((ch = getc(fpsrc)) == '\n')		// if source character is newline, ignore
				{
					if (ferror(fpsrc))
					{
						printf("Error reading from \"%s\" at byte %d!\n", argv[i], j);
						exit(EXIT_FAILURE);
					}
					continue;
				} else					// else add source character to destination file
				{
					putc(ch, fpdest);
					if (!isspace(ch))		// if source character is not whitespace
						nch++;			// count character
					if (ferror(fpdest))
					{
						printf("Error writing to \"%s\" at byte %d!\n", fname, j);
						exit(EXIT_FAILURE);
					}
					if (nch >= LINELIMIT)		// if character limit per line has been reached
					{
						putc('\n', fpdest);	// add newline to destination file
						nch = 0;		// set character count to 0
					}
				}
			}
			putc('\n', fpdest);
			nfiles++;		// increment files processed
			printf("Successfully wrote file \"%s\" to \"%s\"!\n", argv[i], fname);
			fclose(fpsrc);
			fclose(fpdest);		// close files
		}
	}
	printf("\nDone! Wrote %u ", nfiles);	// state number of file(s) written
	if (nfiles == 1)
		puts("file.");
	else
		puts("files.");

	return 0;
}
