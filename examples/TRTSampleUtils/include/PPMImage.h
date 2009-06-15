//=====================================================================================================================
//
//   PPMImage.h
// 
//   Simple PPM image wrapper
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2009 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================


#ifndef _IMAGE_H_
#define _IMAGE_H_


typedef struct tag_pixel
{
	unsigned char r;
	unsigned char g;
	unsigned char b;

} PIXEL;



class PPMImage
{
public:
	/***************************
	* Image Constructor:
	*	Sets the width and height of the image, in pixels
	*  allocates memory for pixels.  Each pixel is initialized to
	*  0,0,0 (black)
	***************************/
	PPMImage(int width=256, int height=256);

    PPMImage(const PPMImage& img);

    const PPMImage& operator=(const PPMImage& img);

	/***************************
	* Image Destructor:
	*	Frees all dynamic memory.
	***************************/
	virtual ~PPMImage();

	/***************************
	* Accessors/Mutators:
	*	Accessors for width and height are provided.
	*  they return the width or height of the image, in pixels
	***************************/
	inline int GetHeight() const { return p_miHeight;};
	inline int GetWidth() const { return p_miWidth;};


    void GetPixelBytes( int x, int y, unsigned char bytes[3] ) const;

    const void* GetPixelBits() const { return (void*)p_mlpPixels; };

	void SetPixel(int x, int y, float r, float g, float b );

	/***********************************************************
	* SaveFile
	*   Outputs the image to a PPM (P6) file
	*   with the specified filename.  Returns true if the
	*   output was a success, false if not.
	*********************************************************/
	bool SaveFile(const char* sFile);

	/*********************************************************
	* LoadFile
	*	Loads an image from a PPM (P6) file
	*   with the specified filename.  Returns true if
	*   the image was loaded ok.  False if it was not
	* 
	************************************************************/
	bool LoadFile(const char* sFile);

   /*******************************************************************************************
	* Resample
    *   Re-sizes the image, using nearest-neighbor filtering to stretch or shrink the contents
	*********************************************************************************************/
    void Resample(int nwidth, int nheight);

private:

	void AllocPixels(int iWidth, int iHeight);
	void FreePixels();

    PIXEL* AccessPixel( int x, int y );

	PIXEL* p_mlpPixels;
	int p_miHeight;
	int p_miWidth;

};


#endif

