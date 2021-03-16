#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{

    // Define our buffer
    BYTE buffer[512];
    // Boolean data
    bool isFirstBlock = true;
    bool newImageFound = false;
    bool isFinalFile = false;
    // File name & number
    int newFileNumber = 0;
    char newFileName[100];
    size_t results;

    // Check for invalid usage
    if (argc != 2)
    {
        printf("Usage Error - only one argument permitted: [File name]..\n");
        return 1;
    }

    string filename = argv[1];

    FILE *file = fopen(filename, "r");
    FILE *newFile;

    // Checking null file
    if (file == NULL)
    {
        printf("Data Error - NULL file..\n");
        return 1;
    }


    while(!isFinalFile)
    {
        results = fread(buffer, sizeof(BYTE), 512, file);

        // Checking if continue or stop my loop
        if (results == 0)
        {
            isFinalFile = true;
        }
        else
        {
            // Checking if this block is a new jpg
            if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0)==0xe0)
            {
                newImageFound = true;
                // We need to know if this is the first block (We do not have a previous file to close yet)
                if (isFirstBlock)
                {
                    isFirstBlock = false;
                }
                else
                {
                    // If not first block, just close the file
                    fclose(newFile);
                }

                // I prepare for a new image file
                sprintf(newFileName, "%03i.jpg", newFileNumber);
                newFileNumber++;
                newFile = fopen(newFileName, "w");
                fwrite(buffer, sizeof(BYTE), results, newFile);

                // else we continue reading
            }
            else
            {
                // read the current block if found it
                if (newImageFound)
                {
                     fwrite(buffer, sizeof(BYTE), results, newFile);
                }
            }

        }

    }

    // Closing files
    fclose(file);
    fclose(newFile);
    return 0;
}