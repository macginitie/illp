#include "ofApp.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// copied from ofxDirList.cpp:
static std::string::size_type idx;
// given 'filename.txt', returns 'txt', e.g.
static string getExt(string filename)
{
    idx = filename.rfind('.');

    if (idx != std::string::npos)
    {
        return filename.substr( idx + 1 );
    }
    else
    {
        return "";
    }
}

//--------------------------------------------------------------
void ofApp::setup(){
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
        strcpy(imageFolderName, "input");
        strcpy(saveFolder, "output");
    }
    // this will include subfolders and non-image files,
    // if any are present...
    std::size_t numDirEntries = DIR.listDir(imageFolderName);
    if (numDirEntries == 0)
    {
        // terminate with error message
        cout << "no files found in image folder [" << imageFolderName << "], exiting (sorry)\n";
        return;
    }
    // ...so we may allocate a few unused strings here:
    imgFilenames = new string[numDirEntries];
    nImages = 0;
    for(int i = 0; i < numDirEntries; i++)
    {
        // we're only interested in image files
        if (getExt(DIR.getPath(i)).compare("bmp") == 0)
        {
            imgFilenames[nImages++] = DIR.getPath(i);
        }
        // TO DO: fix handling of png files
        else if (getExt(DIR.getPath(i)).compare("png") == 0)
        {
            imgFilenames[nImages++] = DIR.getPath(i);
        }
        else if (getExt(DIR.getPath(i)).compare("jpg") == 0)
        {
            imgFilenames[nImages++] = DIR.getPath(i);
        }
        else if (getExt(DIR.getPath(i)).compare("jpeg") == 0)
        {
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

//--------------------------------------------------------------
void ofApp::update(){
    if (nImages > 0)
    {
        pathInfo = imgFilenames[currentImage]
        + "\n" + "press n for next image, p for previous"
        + "\n" + "press s to save the current layers"
        + "\n" + "press e to change to "
        + ((euclidean) ? "brightness" : "Euclidean")
        + " color mapping algorithm";
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

//--------------------------------------------------------------
void ofApp::draw(){
    if (nImages > 0)
    {
        // display the original image
        ofSetColor(0xFF, 0xFF, 0xFF);
        image->draw(leftBorder, topBorder, displayedImageWidth, displayedImageHeight);
        // caption
        ofSetColor(0x06, 0x05, 0x65);
        ofDrawBitmapString(pathInfo, displayedImageWidth + (2*leftBorder), displayedImageHeight + captionOffset);
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

        if (mouseOverColor)
        {
            drawColorSample(); // show magnified color sample of the pixel the mouse is over in the image
        }
    }
}

void ofApp::setupStdPalette()
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
    
    // colors    // TO DO: these colors kinda suck
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

    // pure red, green, blue
    int c = 10;
    stdColor[c] = ofColor( 255, 0, 0, 255 );
    stdColor[++c] = ofColor( 0, 255, 0, 255 );
    stdColor[++c] = ofColor( 0, 0, 255, 255 );
    // "pure" cyan, magenta, yellow
    stdColor[++c] = ofColor( 0, 255, 255, 255 );
    stdColor[++c] = ofColor( 255, 0, 255, 255 );
    stdColor[++c] = ofColor( 255, 255, 0, 255 );
}

void ofApp::drawStandardPalette()
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
            ofDrawRectangle(xCenter, paletteTop + r*(stdPaletteSquareSide), stdPaletteSquareSide, stdPaletteSquareSide);
            // around a colored square
            ofSetColor(stdColor[c+r*rowSize][0], stdColor[c+r*rowSize][1], stdColor[c+r*rowSize][2]);
            ofDrawRectangle(xCenter+2, paletteTop + r*(stdPaletteSquareSide) + 2, stdPaletteSquareSide - 4, stdPaletteSquareSide - 4);
        }
    }
}

void ofApp::drawColorSample()
{
    const int half = 127*3;
    int brightness = imgSampleColor.r + imgSampleColor.g + imgSampleColor.b;
    if (brightness > half)
    {
        // circular black outline
        ofSetColor(0, 0, 0);
    }
    else
    {
        // circular white outline
        ofSetColor(ofColor::white);
    }
    int xCenter = mouseX + 3*(colorCircleRadius + 5);
    int yCenter = mouseY + 3*(colorCircleRadius + 5);
    ofDrawCircle(xCenter, yCenter, colorCircleRadius + 2);
    // around a colored circle
    ofSetColor(imgSampleColor.r, imgSampleColor.g, imgSampleColor.b);
    ofDrawCircle(xCenter, yCenter, colorCircleRadius);
}

void ofApp::drawSelectedPalette()
{
    // show the current selected colors...
    int yCenter = displayedImageHeight + colorCircleOffset;
    for (int c = 0; c < numColors; ++c)
    {
        int xCenter = (1+c) * 2*(colorCircleRadius + 5);
        // circular black outline
        ofSetColor(0, 0, 0);
        ofDrawCircle(xCenter, yCenter, colorCircleRadius + 2);
        // around a colored circle
        ofSetColor(color[c][0], color[c][1], color[c][2]);
        ofDrawCircle(xCenter, yCenter, colorCircleRadius);
    }
}

void ofApp::NewImage()
{
    if (image)
    {
        delete image;
    }
    image = new ofImage();
    image->load(imgFilenames[currentImage]);
    createNewImage();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
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
                greyScale.save(savename);
            }
            else
            {
                saveLayers();
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    // is mouse pointer within image?
    if (pointInImage( x, y ))
    {
        mouseOverColor = true;
        selectedX = (int)((x - leftBorder) * (1.0/rescaleRatio));
        selectedY = (int)((y - topBorder) * (1.0/rescaleRatio));
        // get the color of the pixel @ (selectedX, selectedY)
        imgSampleColor = image->getColor( selectedX, selectedY );
    }
    else
    {
        if (pointInStdPalette( x, y ))
        {
            mouseOverColor = true;
            // TO DO: get color from stdPalette
            ofImage theScreen; //declare variable
            theScreen.grabScreen( x, y, 1, 1 ); //grab at x,y a rect of 1x1
            imgSampleColor = theScreen.getColor( 0, 0 );
        }
        else
        {
            mouseOverColor = false;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

int ofApp::LocInSelectedColor(int x, int y)
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

bool ofApp::pointInStdPalette( int x, int y )
{
    return (x >= paletteLeft && x <= paletteRight
         && y >= paletteTop && y <= paletteBottom);
}

bool ofApp::pointInImage( int x, int y )
{
    return (x >= leftBorder && x < (displayedImageWidth + leftBorder)
         && y >= topBorder && y < (displayedImageHeight + topBorder));
}

//--------------------------------------------------------------
void ofApp::mouseReleased( int x, int y, int button )
{
    if (button == 0)    // left click to select a color from the image
    {
        if (numColors < MAX_LAYERS)
        {
            // click within image?
            if (pointInImage( x, y ))
            {
                selectedX = (int)((x - leftBorder) * (1.0/rescaleRatio));
                selectedY = (int)((y - topBorder) * (1.0/rescaleRatio));
                newImgColor = true;
            }
            // click within palette?
            else if (pointInStdPalette( x, y ))
            {
                selectedX = x;
                selectedY = y;
                newPaletteColor = true;
            }
        }
    }
    else    // right-click to delete the selected color from the list
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
void ofApp::windowResized(int w, int h)
{
    cout << "windowResized to width=" << w << ", height=" << h << endl;
    windowWidth = w;
    windowHeight = h;
    updateImageScaling();
}

// standard OF app methods above here, additions below

void ofApp::updateImageScaling()
{
    displayedImageWidth = image->getWidth();
    cout << "diw before=" << displayedImageWidth << endl;
    displayedImageHeight = image->getHeight();
    rescaleKeepingAspectRatio();
    cout << "diw after=" << displayedImageWidth << endl;
}

void ofApp::createGreyScaleImage()
{
    // get the pixels out of an image, and then
    // average the color_plane values to create new grey-scale pixels
    unsigned char * pixels = image->getPixels().getData();
    greyScale.allocate(image->getWidth(), image->getHeight(), image->getImageType());
    unsigned char * greyPixels = greyScale.getPixels().getData();
    size_t bytes_per_pixel = image->getPixels().getBytesPerPixel();    // 3 ==> RGB or 4 ==> RGBA
    unsigned int w = image->getWidth() * bytes_per_pixel;
    unsigned int h = image->getHeight();
    unsigned int max = w * h;
    cout << "max=" << max << endl;
    for (int i = 0; i < w; i += bytes_per_pixel){
        for (int j = 0; j < h; j++){
            int red = pixels[(j * w) + i];
            int green = pixels[(j * w) + i + 1];
            int blue = pixels[(j * w) + i + 2];
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
    greyScale.setFromPixels(greyPixels, image->getWidth(), image->getHeight(), image->getImageType());
}

void ofApp::addNewColorFromOriginalImage()
{
    // get the color of the pixel @ (selectedX, selectedY)
    unsigned char * pixels = image->getPixels().getData();
    size_t bytes_per_pixel = image->getPixels().getBytesPerPixel();
    int w = image->getWidth() * bytes_per_pixel;
    int h = image->getHeight();
    size_t offset = (selectedY * w) + (selectedX * bytes_per_pixel);
    color[numColors].r = pixels[offset];
    color[numColors].g = pixels[offset + 1];
    color[numColors].b = pixels[offset + 2];
    ++numColors;
    // update the posterized image to use the newly added color
    createPosterizedImage();
}

void ofApp::addNewColorFromStdPalette()
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

void ofApp::createPosterizedImage()
{
    // clear the histogram, not yet used for anything
    for (int c = 0; c < MAX_LAYERS; ++c)
    {
        pxlCount[c] = 0;
    }
    // get the pixels out of an image, and compare each pixel's color
    // to the list of selected color values, replacing it with the closest match
    unsigned char * pixels = image->getPixels().getData();
    // TO DO: handle alpha channel?
    posterized.allocate(image->getWidth(), image->getHeight(), OF_IMAGE_COLOR);
    unsigned char * posterPixels = posterized.getPixels().getData();
    size_t bytes_per_pixel = image->getPixels().getBytesPerPixel();
    size_t channels = image->getPixels().getNumChannels();
    cout << "bpp=" << bytes_per_pixel << ", channels=" << channels;
    int w = image->getWidth() * bytes_per_pixel;
    int h = image->getHeight();
    for (int i = 0; i < w; i += bytes_per_pixel){
        for (int j = 0; j < h; j++){
            if (channels > 1)
            {
                int red = pixels[(j * w) + i];
                int green = pixels[(j * w) + i + 1];
                int blue = pixels[(j * w) + i + 2];
                mapToClosestColor(&red, &green, &blue);
                posterPixels[(j * w) + i] = (unsigned char)red;
                posterPixels[(j * w) + i + 1] = (unsigned char)green;
                posterPixels[(j * w) + i + 2] = (unsigned char)blue;
            }
        }
    }
    posterized.setFromPixels(posterPixels, image->getWidth(), image->getHeight(), OF_IMAGE_COLOR);
}

// replace red, green, blue with the "closest" of the selected colors,
// where "closest" is defined by the Euclidean distance formula[1], applied
// as though red, green, blue were x,y,z coordinates in a Euclidean 3-space
// [1] technically, we use the square of the Euclidean distance formula
void ofApp::mapToClosestColor( int* red, int* green, int* blue )
{
    int index = 0;
    if (euclidean)
    {
        float dist = 3 * 255*255 + 1;
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
    pxlCount[index]++;
}

void ofApp::createNewImage()
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
void ofApp::saveLayers()
{
    // we want this function to be callable multiple times
    // during a program run, without overwriting any files;
    // to that end, we encode this counter in the file names
    static int numSaves = 0;
    // save a pic of what all the layers look like together
    char filename[512];
    snprintf(filename, 512, "%s/posterized%d.jpg", saveFolder, numSaves);
    posterized.save(filename);
    // then save each individual layer as black on white,
    // with a bar across the bottom showing the layer's color
    int w = posterized.getWidth();
    int h = posterized.getHeight();
    ofImage* layer = new ofImage[numColors];
    unsigned char** layerpixels = new unsigned char*[numColors];
    unsigned char* pixels = posterized.getPixels().getData();
    for (int lyr = 0; lyr < numColors; ++lyr)
    {
        layer[lyr].allocate(w, h+10, OF_IMAGE_COLOR);
        layerpixels[lyr] = layer[lyr].getPixels().getData();
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
        snprintf(layerfilename, 512, "%s\\save%dlayer%d.jpg", saveFolder, numSaves, c);
        layer[c].save(layerfilename);
    }
    // clean up
    delete [] layer;
    // prepare for the next save
    ++numSaves;
}

void ofApp::readConfigFile()
{
    std::size_t numDirEntries = DIR.listDir(".");
    if (numDirEntries > 0)
    {
        string filename("/imageloc.txt");
        bool found = false;
        for(int i = 0; i < numDirEntries; i++)
        {
            cout << DIR.getPath(i) << endl;
            string forcomparing(".");
            forcomparing.append(filename);
            // we're only interested in image files
            if (DIR.getPath(i).compare(forcomparing) == 0)
            {
                string abs_path(DIR.getAbsolutePath());
                abs_path.append(filename);
                try {
                    // danke zu Jan-Philip Gehrcke, PhD fur dieses bischen
                    cout << "trying to open " << abs_path << endl;
                    ifstream f (abs_path);
                    // After this attempt to open a file, we can safely use perror() only
                    // in case f.is_open() returns False.
                    if (!f.is_open())
                    {
                        perror(("error while opening file " + filename).c_str());
                        return;
                    }
                    // Read the file via std::getline(). Rules obeyed:
                    //   - first the I/O operation, then error check, then data processing
                    //   - failbit and badbit prevent data processing, eofbit does not
                    if (f.getline(imageFolderName, 511)) {
                        cout << "image folder: " << imageFolderName << endl;
                    }
                    // Only in case of set badbit we are sure that errno has been set in
                    // the current context. Use perror() to print error details.
                    if (f.bad()) {
                        perror(("error while reading file " + filename).c_str());
                    }
                    else {
                        if (f.getline(saveFolder, 511)) {
                            cout << "save folder: " << saveFolder << endl;
                        }
                    }
                    f.close();
                }
                catch(exception ex) {
                    cout << "readConfigFile(): " << ex.what() << endl;
                }
                break;
            }
        }
        if (!found){
            cout << "did not find imageloc.txt in default location" << endl;
            return;
        }
    }
}

// TO DO: stack images vertically if w > h ?
void ofApp::rescaleKeepingAspectRatio()
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
