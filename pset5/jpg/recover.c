/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Recovers JPEGs from a forensic image.
 *
 * by flxio.
 */
 
 #include <stdio.h>
 #include <stdint.h>
 
 #define BLOCKSIZE 512

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: recover <image>\n");
        return 1;
    }
    
    // remember image
    char* image = argv[1];
    
    // open image
    FILE* inptr = fopen(image, "r");
    if (inptr == NULL)
    {
        printf("Error opening image.\n");
        return 2;
    }
    
    uint8_t buffer[BLOCKSIZE];
    
    int filecounter = 0;
    FILE* outptr = NULL;
    
    // iterate over image
    while (fread(buffer, BLOCKSIZE, 1, inptr))
    {
        // check for jpeg identifiers
        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff) && 
            ((buffer[3] == 0xe0) || (buffer[3] == 0xe1)))
        {
            // close file, if opened
            if (outptr != NULL)
            {
                fclose(outptr);
            }
            
            char jpgname[8];
            sprintf(jpgname, "%03d.jpg", filecounter);
            
            // create new jpg
            outptr = fopen(jpgname, "w");
            
            filecounter++;
            
        }
        if (outptr != NULL)
        {
            fwrite(buffer, BLOCKSIZE, 1, outptr);
        }
    }
    
    if (outptr != NULL)
    {
        fclose(outptr);
    }
    
    fclose(inptr);
    
    
}
