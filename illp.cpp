#include "illp.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// copied from ofxDirList.cpp:
static std::string::size_type idx;
// given 'filename.txt', returns 'txt', e.g.
static string getExt(string filename){
	idx = filename.rfind('.');

	if(idx != std::string::npos){
		return filename.substr(idx+1);
	}
	else{
		return "";
	}
}

//--------------------------------------------------------------
void illp::setup()
{
	// TO DO: read windowWidth/Height from .cfg file, or screen dimensions
	windowWidth = 1440;
	windowHeight = 900;
	imageFolderName[0] = 0;
	image = NULL;
	// try to read folder names from config file
	readConfigFile();
	// if no luck, default to "data" folder
	if (imageFolderName[0] == 0)
	{
		strcpy(imageFolderName, "data");
		strcpy(saveFolder, "data");
	}
	// this will include subfolders and non-image files,
	// if any are present...
	int numDirEntries = DIR.listDir(imageFolderName);
	if (numDirEntries == 0)
	{
		// terminate with error message
		cout << "no files found in image folder [" << imageFolderName << "], exiting (sorry)\n";
		return;
	}
	// ...so we may allocate a few unused strings here:
	imgFilenames = new string[numDirEntries];
	numBitPlanes = new int[numDirEntries];
	nImages = 0;
	for(int i = 0; i < numDirEntries; i++)
	{
		// we're only interested in image files
		if (getExt(DIR.getPath(i)).compare("bmp") == 0)
		{
			numBitPlanes[nImages] = 3;
			imgFilenames[nImages++] = DIR.getPath(i);
		}
		// TO DO: fix handling of png files
		//else if (getExt(DIR.getPath(i)).compare("png") == 0)
		//{
		//	numBitPlanes[nImages] = 4;
		//	imgFilenames[nImages++] = DIR.getPath(i);
		//}
		else if (getExt(DIR.getPath(i)).compare("jpg") == 0)
		{
			numBitPlanes[nImages] = 3;
			imgFilenames[nImages++] = DIR.getPath(i);
		}
	}
	if (nImages == 0)
	{
		// terminate with error message
		cout << "we're only interested in image files, but we didn't find any in\n";
		cout << "[" << imageFolderName << "]\n";
		return;
	}
	setupStdPalette();
	currentImage = 0;
    NewImage();
	newImgColor = false;
	newPaletteColor = false;
	deletedColor = false;
	euclidean = true;
	numColors = 0;
	createGreyScaleImage();
	// TO DO: set proper size for window
	paletteLeft = stdPaletteSquareSide + 2;
	paletteRight = paletteLeft + (numStandardColors * stdPaletteSquareSide) - 2;
}

void illp::update()
{
	if (nImages > 0)
	{
		pathInfo = imgFilenames[currentImage]
		+ "\n" + "press n for next image, p for previous"
		+ "\n" + "press s to save the current layers"
		+ "\n" + "press e to change the color mapping algorithm";
		if (newImgColor)
		{
			newImgColor = false;
			addNewColorFromOriginalImage();
		}
		else if (newPaletteColor)
		{
			newPaletteColor = false;
			addNewColorFromStdPalette();
		}
		else if (deletedColor)
		{
			deletedColor = false;
			createNewImage();
		}
	}
	ofBackground(0xF5, 0xF5, 0xFF);
}

void illp::draw()
{
	if (nImages > 0)
	{
		// display the original image
		ofSetColor(0xFF, 0xFF, 0xFF);
		//images[currentImage].draw(leftBorder, topBorder, displayedImageWidth, displayedImageHeight);
		image->draw(leftBorder, topBorder, displayedImageWidth, displayedImageHeight);
		// caption
        ofSetColor(0x06, 0x05, 0x65);
        ofDrawBitmapString(pathInfo, leftBorder, displayedImageHeight + captionOffset);
		// standard palette
		drawStandardPalette();

		if (numColors < 2)
		{
			ofSetColor(0xFF, 0xFF, 0xFF);
			greyScale.draw(displayedImageWidth + (2*leftBorder), topBorder, displayedImageWidth, displayedImageHeight);
		}
		else
		{
            // show the current selected colors...
            drawSelectedPalette();
			// ...and show the "poster" they generate
			ofSetColor(0xFF, 0xFF, 0xFF);
			posterized.draw(displayedImageWidth + (2*leftBorder), topBorder, displayedImageWidth, displayedImageHeight);
		}
	}
}

void illp::setupStdPalette()
{
	float val = 255;
	float decr = (255/10);
	// 9 monochrome shades from pure white to dark grey
    for (int c = 0; c < 9; ++c)
    {
		stdColor[c] = ofColor(val, val, val, 255);
		val -= decr;
	}
	// pure black
	stdColor[9] = ofColor(0, 0, 0, 255);
	// colors
	val = 255;
    for (int c = 10; c < 19; ++c)
    {
		stdColor[c] = ofColor(val, 0, 0, 255);
		stdColor[c+9] = ofColor(0, val, 0, 255);
		stdColor[c+18] = ofColor(0, 0, val, 255);
		stdColor[c+27] = ofColor(val, val, 0, 255);
		stdColor[c+36] = ofColor(0, val, val, 255);
		stdColor[c+45] = ofColor(val, 0, val, 255);
		stdColor[c+54] = ofColor(val, val*0.4, val*0.4, 255);
		stdColor[c+63] = ofColor(val*0.4, val, val*0.4, 255);
		stdColor[c+72] = ofColor(val*0.4, val*0.4, val, 255);
		stdColor[c+81] = ofColor(val, val, val*0.4, 255);
		stdColor[c+90] = ofColor(val*0.4, val, val, 255);
		stdColor[c+99] = ofColor(val, val*0.4, val, 255);
		stdColor[c+108] = ofColor(val, val*0.8, val*0.8, 255);
		stdColor[c+117] = ofColor(val*0.8, val, val*0.8, 255);
		stdColor[c+126] = ofColor(val*0.8, val*0.8, val, 255);
		stdColor[c+135] = ofColor(val, val, val*0.8, 255);
		stdColor[c+144] = ofColor(val*0.8, val, val, 255);
		stdColor[c+153] = ofColor(val, val*0.8, val, 255);
		val -= decr;
	}
}

void illp::drawStandardPalette()
{
	paletteTop = displayedImageHeight + stdPaletteSquareOffset + 2;
	paletteBottom = paletteTop + 3*stdPaletteSquareOffset - 8;
	int rowSize = numStandardColors/3;
	int vertSpacing = 0; // 4 l8r use, mebbe
    for (int c = 0; c < rowSize; ++c)
    {
		for (int r = 0; r < 3; ++r)
		{
			// black outline
			ofSetColor(0, 0, 0);
			int xCenter = (1+c) * stdPaletteSquareSide;
			ofRect(xCenter, paletteTop + r*(stdPaletteSquareSide), stdPaletteSquareSide, stdPaletteSquareSide);
			// around a colored square
			ofSetColor(stdColor[c+r*rowSize][0], stdColor[c+r*rowSize][1], stdColor[c+r*rowSize][2]);
			ofRect(xCenter+2, paletteTop + r*(stdPaletteSquareSide) + 2, stdPaletteSquareSide - 4, stdPaletteSquareSide - 4);
		}
    }
}

void illp::drawSelectedPalette()
{
    // show the current selected colors...
    int yCenter = displayedImageHeight + colorCircleOffset;
    for (int c = 0; c < numColors; ++c)
    {
        int xCenter = (1+c) * 2*(colorCircleRadius + 5);
        // circular black outline
        ofSetColor(0, 0, 0);
        ofCircle(xCenter, yCenter, colorCircleRadius + 2);
        // around a colored circle
        ofSetColor(color[c][0], color[c][1], color[c][2]);
        ofCircle(xCenter, yCenter, colorCircleRadius);
    }
}

void illp::NewImage()
{
    if (image)
    {
        delete image;
    }
    image = new ofImage();
    image->loadImage(imgFilenames[currentImage]);
    cout << "b currentImage:" << currentImage << " w=" << image->getWidth() << " isAlloc=" << image->bAllocated() << endl;
    createNewImage();
    cout << "a currentImage:" << currentImage << " w=" << image->getWidth() << " isAlloc=" << image->bAllocated() << endl;
}

//--------------------------------------------------------------
void illp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void illp::keyReleased(int key)
{
	if (key == 'e')
	{
		euclidean = !euclidean;
		createPosterizedImage();
	}
	else if (nImages > 0)
	{
		if (key == 'n')
		{
			currentImage++;
			currentImage %= nImages;
			NewImage();
		}
		else if (key == 'p')
		{
			currentImage = (0 == currentImage) ? nImages - 1 : currentImage - 1;
			NewImage();
		}
		else if (key == 's')
		{
			if (numColors == 0)
			{
				string savename = imgFilenames[currentImage] + "-grey.jpg";
				greyScale.saveImage(savename);
			}
			else
			{
				saveLayers();
			}
		}
    }
}

//--------------------------------------------------------------
void illp::mouseMoved(int x, int y )
{
}

//--------------------------------------------------------------
void illp::mouseDragged(int x, int y, int button)
{
	// cout << "[" << x << "," << y << "]";
}

//--------------------------------------------------------------
void illp::mousePressed(int x, int y, int button)
{
	// cout << "(" << x << "," << y << ")";
}

int illp::LocInSelectedColor(int x, int y)
{
    int yCenter = displayedImageHeight + colorCircleOffset;
    for (int c = 0; c < numColors; ++c)
    {
        int xCenter = (1+c) * 2*(colorCircleRadius + 5);
		int xdist = (xCenter - x);
		int ydist = (yCenter - y);
		int eucdsq = xdist*xdist + ydist*ydist;
		float d = sqrt(eucdsq);
		if (d < colorCircleRadius)
		{
			return c;
		}
    }

	return -1;
}

//--------------------------------------------------------------
void illp::mouseReleased(int x, int y, int button)
{
	if (button == 0)	// left click to select a color from the image
	{
		if (numColors < MAX_LAYERS)
		{
			// click within image?
			if (x >= leftBorder && x < (displayedImageWidth + leftBorder)
				&& y >= topBorder && y < (displayedImageHeight + topBorder))
			{
				selectedX = (int)((x - leftBorder) * (1.0/rescaleRatio));
				selectedY = (int)((y - topBorder) * (1.0/rescaleRatio));
				newImgColor = true;
			}
			// click within palette?
			else if (x >= paletteLeft && x <= paletteRight
				&& y >= paletteTop && y <= paletteBottom)
			{
				selectedX = x;
				selectedY = y;
				newPaletteColor = true;
			}
		}
	}
	else	// right-click to delete the last selected color from the list
	{
		if (numColors > 0)
		{
			// click within selected palette?
			int c = LocInSelectedColor(x, y);
			if (c >= 0)
			{
				// remove indexed color (shifting the rest (if any) down one)
				for (int i = c; i < (numColors - 1); ++i)
				{
					color[i] = color[i + 1]; 
				}
			}
			// if no selected color found, we just remove the last one
			--numColors;
			// flag to recalc/repaint
			deletedColor = true;
		}
	}
}

//--------------------------------------------------------------
void illp::windowResized(int w, int h)
{
	cout << "windowResized to width=" << w << ", height=" << h << endl;
	windowWidth = w;
	windowHeight = h;
	updateImageScaling();
}

// standard OF app methods above here, additions below

void illp::updateImageScaling()
{
//	displayedImageWidth = images[currentImage].width;
//	displayedImageHeight = images[currentImage].height;
	displayedImageWidth = image->width;
	cout << "diw before=" << displayedImageWidth << endl;
	displayedImageHeight = image->height;
	rescaleKeepingAspectRatio();
	cout << "diw after=" << displayedImageWidth << endl;
}

void illp::createGreyScaleImage()
{
	// get the pixels out of an image, and then
	// average the color_plane values to create new grey-scale pixels
	//unsigned char * pixels = images[currentImage].getPixels();
	unsigned char * pixels = image->getPixels();
	//greyScale.allocate(images[currentImage].width, images[currentImage].height, OF_IMAGE_COLOR);
	greyScale.allocate(image->width, image->height, OF_IMAGE_COLOR);
	unsigned char * greyPixels = greyScale.getPixels();
	unsigned int bytes_per_pixel = numBitPlanes[currentImage];	// 3 ==> RGB or 4 ==> RGBA
	//int w = images[currentImage].width * bytes_per_pixel;
	//int h = images[currentImage].height;
	unsigned int w = image->width * bytes_per_pixel;
	unsigned int h = image->height;
	unsigned int max = w * h;
	cout << "max=" << max << endl;
	for (int i = 0; i < w; i += bytes_per_pixel){
		for (int j = 0; j < h; j++){
			int red = pixels[(j * w) + i];
			int green = pixels[(j * w) + i + 1];
			int blue = pixels[(j * w) + i + 2];
			//int alpha = pixels[(j * w) + i + 3];
			float avg = (red + green + blue)/3.0;
			unsigned char grey = (unsigned char)(int)avg;
			int idx = (j * w) + i;
			if (idx < max) {
				greyPixels[idx] = grey;
				greyPixels[idx + 1] = grey;
				greyPixels[idx + 2] = grey;
			}
			else
			{
				cout << "overflow @ j=" << j << ", i=" << i << endl;
				return;
			}
		}
	}
	//greyScale.setFromPixels(greyPixels, images[currentImage].width, images[currentImage].height,
	greyScale.setFromPixels(greyPixels, image->width, image->height, OF_IMAGE_COLOR);
}

void illp::addNewColorFromOriginalImage()
{
	// get the color of the pixel @ (selectedX, selectedY)
	//unsigned char * pixels = images[currentImage].getPixels();
	unsigned char * pixels = image->getPixels();
	int bytes_per_pixel = numBitPlanes[currentImage];
//	int w = images[currentImage].width * bytes_per_pixel;
//	int h = images[currentImage].height;
	int w = image->width * bytes_per_pixel;
	int h = image->height;
	int offset = (selectedY * w) + (selectedX * bytes_per_pixel);
	color[numColors].r = pixels[offset];
	color[numColors].g = pixels[offset + 1];
	color[numColors].b = pixels[offset + 2];
	++numColors;
	// update the posterized image to use the newly added color
	createPosterizedImage();
}

void illp::addNewColorFromStdPalette()
{
	int row = (numStandardColors/3); // HCM# rows, TO DO: fix
	int paletteColorColumn = (selectedX - paletteLeft) / stdPaletteSquareSide;
	// some defensive programming:
	if (paletteColorColumn < 0)
	{
		paletteColorColumn = 0;
	}
	else if (paletteColorColumn >= row)
	{
		paletteColorColumn = (row - 1);
	}
	int paletteRow = (selectedY - paletteTop) / stdPaletteSquareSide;
	color[numColors] = stdColor[paletteRow * row + paletteColorColumn];
	++numColors;
	// update the posterized image to use the newly added color
	createPosterizedImage();
}

void illp::createPosterizedImage()
{
	// get the pixels out of an image, and compare each pixel's color
	// to the list of selected color values, replacing it with the closest match
	//unsigned char * pixels = images[currentImage].getPixels();
	unsigned char * pixels = image->getPixels();
	// TO DO: handle alpha channel?
	//posterized.allocate(images[currentImage].width, images[currentImage].height, OF_IMAGE_COLOR);
	posterized.allocate(image->width, image->height, OF_IMAGE_COLOR);
	unsigned char * posterPixels = posterized.getPixels();
	int bytes_per_pixel = numBitPlanes[currentImage];	// 3 ==> RGB or 4 ==> RGBA
//	int w = images[currentImage].width * bytes_per_pixel;
//	int h = images[currentImage].height;
	int w = image->width * bytes_per_pixel;
	int h = image->height;
	for (int i = 0; i < w; i += bytes_per_pixel){
		for (int j = 0; j < h; j++){
			int red = pixels[(j * w) + i];
			int green = pixels[(j * w) + i + 1];
			int blue = pixels[(j * w) + i + 2];
			mapToClosestColor(&red, &green, &blue);
			posterPixels[(j * w) + i] = (unsigned char)red;
			posterPixels[(j * w) + i + 1] = (unsigned char)green;
			posterPixels[(j * w) + i + 2] = (unsigned char)blue;
		}
	}
	//posterized.setFromPixels(posterPixels, images[currentImage].width, images[currentImage].height,
	posterized.setFromPixels(posterPixels, image->width, image->height, OF_IMAGE_COLOR);
}

// replace red, green, blue with the "closest" of the selected colors,
// where "closest" is defined by the Euclidean distance formula[1], applied
// as though red, green, blue were x,y,z coordinates in a Euclidean 3-space
// [1] technically, we use the square of the Euclidean distance formula
void illp::mapToClosestColor( int* red, int* green, int* blue )
{
	if (euclidean)
	{
		float dist = 3 * 255*255 + 1;
		int index = 0;
		for (int c = 0; c < numColors; ++c)
		{
			int rdiff = (*red - color[c].r);
			int gdiff = (*green - color[c].g);
			int bdiff = (*blue - color[c].b);
			float checkdist = (rdiff*rdiff + gdiff*gdiff + bdiff*bdiff);
			if (dist > checkdist)
			{
				dist = checkdist;
				index = c;
			}
		}
		*red = color[index].r;
		*green = color[index].g;
		*blue = color[index].b;
	}
	else
	{
		int dist = 3 * 255 + 1;
		int index = 0;
		for (int c = 0; c < numColors; ++c)
		{
			int checkdist = abs(*red - color[c].r) + abs(*green - color[c].g) + abs(*blue - color[c].b);
			if (dist > checkdist)
			{
				dist = checkdist;
				index = c;
			}
		}
		*red = color[index].r;
		*green = color[index].g;
		*blue = color[index].b;
	}
}

void illp::createNewImage()
{
	if (0 == numColors)
	{
		createGreyScaleImage();
	}
	else
	{
		createPosterizedImage();
	}
	updateImageScaling();
}

// create a set of image files: one per selected color,
// plus one for the posterized image
void illp::saveLayers()
{
	// we want this function to be callable multiple times
	// during a program run, without overwriting any files;
	// to that end, we encode this counter in the file names
	static int numSaves = 0;
	// save a pic of what all the layers look like together
	char filename[512];
	sprintf(filename, "%s/posterized%d.jpg", saveFolder, numSaves);
	posterized.saveImage(filename);
	// then save each individual layer as black on white,
	// with a bar across the bottom showing the layer's color
	int w = posterized.width;
	int h = posterized.height;
	ofImage* layer = new ofImage[numColors];
	unsigned char** layerpixels = new unsigned char*[numColors];
	unsigned char* pixels = posterized.getPixels();
	for (int lyr = 0; lyr < numColors; ++lyr)
	{
		layer[lyr].allocate(w, h+10, OF_IMAGE_COLOR);
		layerpixels[lyr] = layer[lyr].getPixels();
		for (int x = 0; x < w; ++x)
		{
			for (int y = 0; y < h; ++y)
			{
				int offset = y*3*w + x*3;
				if (color[lyr][0] == pixels[offset]
				&& color[lyr][1] == pixels[offset+1]
				&& color[lyr][2] == pixels[offset+2])
				{
					layerpixels[lyr][offset] = 0;
					layerpixels[lyr][offset+1] = 0;
					layerpixels[lyr][offset+2] = 0;
				}
				else
				{
					layerpixels[lyr][offset] = 255;
					layerpixels[lyr][offset+1] = 255;
					layerpixels[lyr][offset+2] = 255;
				}
			}
			for (int y = h; y < (10+h); ++y)
			{
				int offset = y*3*w + x*3;
				layerpixels[lyr][offset] = color[lyr][0];
				layerpixels[lyr][offset+1] = color[lyr][1];
				layerpixels[lyr][offset+2] = color[lyr][2];
			}
		}
	}
	for (int c = 0; c < numColors; ++c)
	{
		char layerfilename[512];
		sprintf(layerfilename, "%s\\save%dlayer%d.jpg", saveFolder, numSaves, c);
		layer[c].saveImage(layerfilename);
	}
	// clean up
	delete [] layer;
	// prepare for the next save
	++numSaves;
}

void illp::readConfigFile()
{
	char* configFileName = "imageloc.txt";
	ifstream configFile;
	configFile.open(configFileName);
	configFile.getline(imageFolderName, 511);
	configFile.getline(saveFolder, 511);
}

// TO DO: stack images vertically if w > h ?
void illp::rescaleKeepingAspectRatio()
{
	if (displayedImageWidth > displayedImageHeight)
	{
		float maxX = (windowWidth - 2*leftBorder) / 2.0;
		rescaleRatio = (maxX/(displayedImageWidth));
		displayedImageHeight = (int)(displayedImageHeight * rescaleRatio);
		displayedImageWidth = (int)maxX;
	}
	else
	{
		float maxY = windowHeight - (topBorder + 100);
		rescaleRatio = (maxY/(displayedImageHeight));
		displayedImageWidth = (int)(displayedImageWidth * rescaleRatio);
		displayedImageHeight = (int)maxY;
	}
}





