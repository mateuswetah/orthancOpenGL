#include "orthanc.h"

// Vectors to hold textures
GLfloat _textureId[6];       // skybox
GLfloat texture_octagon;     // octagon 
GLfloat _texture_Grass;     // grass 
GLfloat _texture_stoneId[2]; // stones
GLubyte *_textureShadow;  // shadow (png)

//----FUNCTIONS-RELATED-TO-THE-TEXTURES----------------------------------------------
// Load png with transparency by libpng
bool loadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData) 
{
    
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int color_type, interlace_type;
    
    FILE *fp;
 
    if ((fp = fopen(name, "rb")) == NULL)
        return false;
 
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
 
    if (png_ptr == NULL) 
    {
        fclose(fp);
        return false;
    }
 
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) 
    {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return false;
    }
 

    if (setjmp(png_jmpbuf(png_ptr))) 
    {

        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);

        return false;
    }
 

    png_init_io(png_ptr, fp);
 
    png_set_sig_bytes(png_ptr, sig_read);
 

    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
 
    png_uint_32 width, height;
    int bit_depth;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, NULL, NULL);
    outWidth = width;
    outHeight = height;
 
    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    *outData = (unsigned char*) malloc(row_bytes * outHeight);
 
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
 
    for (int i = 0; i < outHeight; i++) 
    {

        memcpy(*outData+(row_bytes * (outHeight-1-i)), row_pointers[i], row_bytes);
    }
 
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
 
  
    fclose(fp);

    return true;
}

// Load textures by imageloader (bmp, 24 bits)
GLuint loadTexture(Image* image) 
{
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RGB,
					image->width, image->height,
					0,
					GL_RGB,
					GL_UNSIGNED_BYTE,
					image->pixels
				);

	return textureId;
}

void loadAllTextures()
{
    // Load images referent to each place on the texture vectors
    // BMPs
    Image* image0 = loadBMP("nightsky_down.bmp");
    _textureId[0] = loadTexture(image0);
    delete image0;

    Image* image1 = loadBMP("nightsky_east.bmp");
    _textureId[1] = loadTexture(image1);
    delete image1;

    Image* image2 = loadBMP("nightsky_north.bmp");
    _textureId[2] = loadTexture(image2);
    delete image2;

    Image* image3 = loadBMP("nightsky_south.bmp");
    _textureId[3] = loadTexture(image3);
    delete image3;

    Image* image4 = loadBMP("nightsky_up.bmp");
    _textureId[4] = loadTexture(image4);
    delete image4;

    Image* image5 = loadBMP("nightsky_west.bmp");
    _textureId[5] = loadTexture(image5);
    delete image5;

    Image* imagestone1 = loadBMP("stone_1.bmp");
    _texture_stoneId[0] = loadTexture(imagestone1);
    delete imagestone1;

    Image* imagestone2 = loadBMP("stone_2.bmp");
    _texture_stoneId[1] = loadTexture(imagestone2);
    delete imagestone2;

    Image* imageoctagon = loadBMP("octagon.bmp");
    texture_octagon = loadTexture(imageoctagon);
    delete imageoctagon;

    Image* imagegrass = loadBMP("grass.bmp");
    _texture_Grass = loadTexture(imagegrass);
    delete imagegrass;
    
    // PNG
    int width = 1151;
    int height = 1151;
    bool hasAlpha = true;
    char filename[] = "shadow2.png";

    bool success = loadPngImage(filename, width, height, hasAlpha, &_textureShadow);
    if (!success) {
        std::cout << "Unable to load png file" << std::endl;
        return;
    }
    std::cout << "Image loaded " << width << " " << height << " alpha " << hasAlpha << std::endl;   
    
}
