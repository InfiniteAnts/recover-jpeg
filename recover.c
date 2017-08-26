#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

// defining BYTE
typedef uint8_t  BYTE;

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    
    // open input file
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Image cannot be opened\n");
        return 2;
    }
    
    // initialising image count and JPEGrunning flag to 0
    int imgcount = 0;
    bool JPEGrunning = false;
    
    // declaring pointers
    FILE* img = NULL;
    BYTE *buffer = malloc(sizeof(BYTE) * 512);
    if (buffer == NULL)
        return 3;
    char *filename = malloc(sizeof(char) * 8);
    if (filename == NULL)
        return 4;

    // repeat forever
    while (true)
    {
        // read 512 bytes into a buffer
        fread(buffer, 1, 512, inptr);
        
        // if EOF is reached, loop ends
        if (feof(inptr))
            break;
        
        // if JPEG header is found
        if (buffer[0] == 0xff && 
            buffer[1] == 0xd8 && 
            buffer[2] == 0xff && 
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (!JPEGrunning)
            {
               // set JPEGrunning flag
                JPEGrunning = true;
                
                // set imagecount as the filename
                sprintf(filename, "%03i.jpg", imgcount);
                
                // open JPEG file
                img = fopen(filename, "w");
                
                // write 512 bytes to the JPEG file
                fwrite(buffer, 512, 1, img);
                
            }
            else if (JPEGrunning)
            {
                // close previous file
                fclose(img);
                
                // increment image count
                imgcount++;
                
                // set imagecount as the filename
                sprintf(filename, "%03i.jpg", imgcount);
                
                // open JPEG file
                img = fopen(filename, "w");
                
                // write 512 bytes to the JPEG file
                fwrite(buffer, 512, 1, img);
                
            }    
        }
        
        else if (JPEGrunning) 
        {
            // write 512 bytes to the JPEG file
            fwrite(buffer, 512, 1, img);
        }
    }
   
    // free malloc 
    free(buffer);
    free(filename);
    
    // closing image file
    fclose(inptr);
    fclose(img);
    
    return 0;
}