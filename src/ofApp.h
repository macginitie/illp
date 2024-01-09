#pragma once

#include "ofMain.h"
#include "ofFileUtils.h"

class ofApp : public ofBaseApp {
    
        // screen layout
        static const int leftBorder = 5;
        static const int topBorder = 5;
        static const int colorCircleOffset = 20;
        static const int colorCircleRadius = 10;
        static const int stdPaletteSquareOffset = 35;
        static const int stdPaletteSquareSide = 20;
        static const int captionOffset = 120;
        static const int bottomBorder = 2*captionOffset;
        static const int slightlySaturated = 75;
        static const int moreSaturated = 150;

        // we will have a dynamic number of images,
        int         nImages;
        ofImage*    image;
        string*     imgFilenames;
        // based on the content of a directory:
        ofDirectory DIR;
        // which is specified in a config file, & stored here
        char        imageFolderName[512];

        // the name/path of the folder to save files in is stored here
        char        saveFolder[512];

        // we'll work with one image at a time
        int         currentImage;
        string      pathInfo;

        // we show a greyscale version of the image we're working with
        // if fewer than two colors have been selected yet
        ofImage     greyScale;
        // ...otherwise we'll show what the finished linocut might look like
        ofImage     posterized;

        // how many colors (a.k.a. layers) has user selected?
        int         numColors;

        // a standard palette
        static const int numStandardColors =
            10    // 10 shades of grey from black to white
            + 27*3 // 9 shades @ of pure r,g,b, 9 of slightly satur8d r,g,b, 9 of more satur8d r,g,b
            + 27*3 // 9 shades @ of pure magenta, yellow, cyan, 9 slightly sat, 9 more sat
            ;

        ofColor     stdColor[numStandardColors];
        // palette placement
        int paletteLeft, paletteRight, paletteTop, paletteBottom;

        // to do: dynamic, instead of hard-coded?
        // I don't want to carve or print that many separate layers, though ;-)
        static const int MAX_LAYERS = 20;
        ofColor     color[MAX_LAYERS];
        int         pxlCount[MAX_LAYERS];    // for histogram

        ofColor     imgSampleColor;

        bool        newImgColor;
        bool        newPaletteColor;
        bool        deletedColor;
        bool        mouseOverColor;
        bool        euclidean;

        int         selectedX;
        int         selectedY;

        int         selectedRed;
        int         selectedGreen;
        int         selectedBlue;

        int         windowWidth; // = 1440;
        int         windowHeight; // = 900;
        int         displayedImageWidth;
        int         displayedImageHeight;
        float       rescaleRatio;

        // added methods
        void updateImageScaling();
        void rescaleKeepingAspectRatio();
        void readConfigFile();
        void addNewColorFromOriginalImage();
        void addNewColorFromStdPalette();
        void createGreyScaleImage();
        void createPosterizedImage();
        void mapToClosestColor( int* red, int* green, int* blue );
        void NewImage();
        void createNewImage();
        void saveLayers();
        void setupStdPalette();
        void drawStandardPalette();
        void drawSelectedPalette();
        void drawColorSample();
        int LocInSelectedColor( int x, int y );
        bool pointInStdPalette( int x, int y );
        bool pointInImage( int x, int y );

    public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
    
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
    
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
};
