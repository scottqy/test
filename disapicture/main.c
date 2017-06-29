#include "libcloud.h"
#include "png.h"


void print_useage()
{
	printf("useage: [cloudimg:png/bmp/jpg/dfiff/surface/rle] [pngfile]\n");
}

int main(int argc,char **argv)
{
	if(argc != 3) print_useage();


	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	png_colorp palette;

	/* Open the file */
	fp = fopen(argv[2], "wb");
	if (fp == NULL)
	  return (0);

	/* Create and initialize the png_struct with the desired error handler
	* functions.  If you want to use the default stderr and longjump method,
	* you can supply NULL for the last three parameters.  We also check that
	* the library version is compatible with the one used at compile time,
	* in case we are using dynamically linked libraries.  REQUIRED.
	*/
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);

	if (png_ptr == NULL)
	{
	  fclose(fp);
	  return (0);
	}

	/* Allocate/initialize the image information data.  REQUIRED */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
	  fclose(fp);
	  png_destroy_write_struct(&png_ptr,  NULL);
	  return (0);
	}

	/* Set error handling.  REQUIRED if you aren't supplying your own
	* error handling functions in the png_create_write_struct() call.
	*/
	if (setjmp(png_jmpbuf(png_ptr)))
	{
	  /* If we get here, we had a problem writing the file */
	  fclose(fp);
	  png_destroy_write_struct(&png_ptr, &info_ptr);
	  return (0);
	}

	/* One of the following I/O initialization functions is REQUIRED */
	Image img;
	img.SetFile(string(argv[1]));
	img.GetSurface(RWindow::GetImageManager());
	// 分配显存成功后加载图片
	if(!img.LoadIntoSurface())
	{
		print_useage();
		return 0;
	}
	png_init_io(png_ptr, fp);
	int width, height, bit_depth;
	width = img.GetWidth();
	height = img.GetHeight();
	bit_depth = 8;

	png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, PNG_COLOR_TYPE_RGB_ALPHA,
	  PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	 png_text text_ptr[2]={0,0};
	/* Optionally write comments into the image */
	text_ptr[0].key = "author";
	text_ptr[0].text = "scott'qiu";
	text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
	text_ptr[0].itxt_length = 0;
	text_ptr[0].lang = NULL;
	text_ptr[0].lang_key = NULL;
	text_ptr[1].key = "Description";
	text_ptr[1].text = "bmcloud";
	text_ptr[1].compression = PNG_TEXT_COMPRESSION_NONE;
	text_ptr[1].itxt_length = 0;
	text_ptr[1].lang = NULL;
	text_ptr[1].lang_key = NULL;

	png_set_text(png_ptr, info_ptr, text_ptr, 2);

	png_write_info(png_ptr, info_ptr);

	png_byte *image = (png_byte *)img.GetSurface()->vir_addr;
	png_bytep row_pointers[height];
	int src_bytes_per_pixel = img.GetSurface()->size * 4/width/height;
	if (height > PNG_UINT_32_MAX/png_sizeof(png_bytep))
	 png_error (png_ptr, "Image is too tall to process in memory");

	for (int i = 0; i < height; i++)
	 row_pointers[i] = image + i*width*src_bytes_per_pixel;

	/* One of the following output methods is REQUIRED */

	/* The number of passes is either 1 for non-interlaced images,
	* or 7 for interlaced images.
	*/
    for(int y=0;y<height;y++)  
    {  
        for(int x=0;x<width;x++)  
        {  
            int dx = x;//+pInfo->left;  
            int dy = y;//+pInfo->top;  

            if(dx>width || dy>height)  
            {  
                printf("invalid offset operation\n");  
                return 0;
            }  

            const unsigned char* psrc = image + y*width*src_bytes_per_pixel+ x*src_bytes_per_pixel;
            png_bytep pdst = NULL;
              
            pdst = row_pointers[y]  +dx*src_bytes_per_pixel;  
              
            if(src_bytes_per_pixel==4)  
            {
                pdst[0] = psrc[0];  
                pdst[1] = psrc[1];  
                pdst[2] = psrc[2];  
                pdst[3] = psrc[3];  
            }
        }  
          
        png_write_row(png_ptr,row_pointers[y]);  

    } 

	png_write_end(png_ptr, info_ptr);

	/* Clean up after the write, and free any memory allocated */
	png_destroy_write_struct(&png_ptr, &info_ptr);

	/* Close the file */
	fclose(fp);

	/* That's it */
	return 1;
}
