#ifndef _ILLP
#define _ILLP

#include "ofMain.h"
#include "ofFileUtils.h"

class illp : public ofBaseApp
{
		// screen layout
		static const int leftBorder = 5;
		static const int topBorder = 5;
		static const int captionOffset = 50;
		static const int colorCircleOffset = 20;
		static const int colorCircleRadius = 10;
		static const int bottomBorder = 2*captionOffset;

		// we will have a dynamic number of images,
		int 		nImages;
		//ofImage	* 	images;
		ofImage* 	image;
		string*		imgFilenames;
		// based on the content of a directory:
		ofDirectory  DIR;
		// which is specified in a config file, & stored here
		char imageFolderName[512];

		// the name/path of the folder to save files in is stored here
		char saveFolder[512];

		// (TO DO: .png alpha support)
		// this helps deal with .png files that have an alpha channel
		int*		numBitPlanes;

		// we'll work with one image at a time
		int         currentImage;
		string		pathInfo;

		// we'll either show a greyscale version of the image we're working with
		ofImage		greyScale;
		// ...or else we'll show what the finished linocut might look like
		ofImage		posterized;

		// how many colors (a.k.a. layers) has user selected?
		int			numColors;
		// to do: dynamic, instead of hard-coded?
		// I don't want to carve or print that many separate layers, though ;-)
		static const int MAX_LAYERS = 20;
		ofColor		color[MAX_LAYERS];

		bool		newColor;
		bool		deletedColor;

		int			selectedX;
		int			selectedY;

		int			selectedRed;
		int			selectedGreen;
		int			selectedBlue;

		static const int			windowWidth = 1440;
		static const int			windowHeight = 900;
		int			displayedImageWidth;
		int			displayedImageHeight;
		float		rescaleRatio;

		// added methods
		void updateImageScaling();
		void rescaleKeepingAspectRatio();
		void readConfigFile();
		void addNewColor();
		void createGreyScaleImage();
		void createPosterizedImage();
		void mapToClosestColor(int* red, int* green, int* blue);
		void NewImage();
		void createNewImage();
		void saveLayers();
		void drawPalette();

	public:

		// standard OF methods
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);

		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

};

#endif

