# Questions

## What's `stdint.h`?

stdint.h is a standard C library that specifies datatypes for exact-width integers.
This means you can designate signed or unsigned 8-, 16, 32, or 64-bit integers.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

These datatypes make your code more precise. If your code is running on a variety of systems,
there can be surprises in regards to the actual size of variables. With an uint32_t type,
for example, you know the size on disk will be 32 bits.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

BYTE - 8 bits or 1 byte
DWORD - 32 bits or 4 bytes
LONG - also 32 bits or 4 bytes
WORD - 16 bits or 2 bytes

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be?
## Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

According to https://msdn.microsoft.com/en-us/library/dd183374(v=vs.85).aspx,
the first two bytes need to represent 'BM'.
The software opening this file should check these in order to determine the file type.
In decimal this would be: 66 77
In binary this would be: 01000010 01001101
In hexadecimal this would be: 0100 0010 0100 1101 => 0x42 0x4D

## What's the difference between `bfSize` and `biSize`?

bfSize, (DWORD - 4 bytes) The size, in bytes, of the bitmap file.
Because this is 4 bytes, theoretically this could represent a file over 4GB.
However, it is safer to assume that an editor could treat this as a signed value,
therefore a 2GB limit is more reasonable.

biSize, (DWORD - 4 bytes) The number of bytes required by the structure - that is the image header
in which biSize resides.
Nearly all BMP files use the basic 40-byte header from the Windows Version 3 specification.


## What does it mean if `biHeight` is negative?

The sign indicates the starting point of the image grid.
A negative number means a 'top down' grid, starting at the top-left corner.
Positive means lower-left corner.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount: must be 0, 1, 2, 4, 8, 16, 24, or 32.

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

`fopen`, when using the `r`ead mode, must point to an EXISTING file.
If the file doesn't exist, `NULL` will be returned.

## Why is the third argument to `fread` always `1` in our code?

The size of our buffer is exactly the size of 1 BITMAPFILEHEADER or BITMAPINFOHEADER.
There is only 1 of these per file so it makes sense to go directly tothe desired address
and read exactly what we need 1 time and no more.

## What value does line 65 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

3
`sizeof(RGBTRIPLE) == 3`
`padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4`


## What does `fseek` do?

`fseek` allows you to change the location of the file pointer.
Rather than simply going forward, starting at the beginning, changing the offset parameter
can add or subtract from the current pointer location.

## What is `SEEK_CUR`?

This is a value for the `from_where` parameter that stipulates from where the given offset
should be applied. `SEEK_CUR` refers to the current location of the pointer.
Alternatives include the beginning and end of the file.