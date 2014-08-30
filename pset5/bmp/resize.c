/**
 * copy.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }
    
    int res_factor = atoi(argv[1]);
    if (res_factor < 1 || res_factor > 100)
    {
        printf("Resize-factor must be in range 1-100!\n");
        return 5;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf, bf_res;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    bf_res = bf;

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi, bi_res;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    bi_res = bi;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // update height and width for resized bmp
    bi_res.biWidth = bi.biWidth * res_factor;
    bi_res.biHeight = bi.biHeight * res_factor;
    
    // new padding
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int res_padding = (4 - (bi_res.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // new image sizes
    bi_res.biSizeImage = (bi_res.biWidth * sizeof(RGBTRIPLE) + res_padding) * abs(bi_res.biHeight);
    bf_res.bfSize = bf.bfSize - bi.biSizeImage + bi_res.biSizeImage;

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_res, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_res, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // read each scanline (resize)factor times
        for (int j = 0; j < res_factor; j++)
        {
            // iterate over pixel in scanline
            for (int k = 0; k < bi.biWidth; k++)
            {
                // temp storage
                RGBTRIPLE triple;
                
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                // write RGB triple to outfile, multiplied by resize factor
                for (int l = 0; l < res_factor; l++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            // add NEW padding
            for (int k = 0; k < res_padding; k++)
            {
                fputc(0x00, outptr);
            }
            
            // return to beginning of scanline
            if (j < res_factor - 1)
            {
                fseek(inptr, -bi.biWidth * sizeof(RGBTRIPLE), SEEK_CUR);
            }
        }
        // skip padding, if any.
        fseek(inptr, padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
