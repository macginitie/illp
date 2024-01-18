# illp
interactive linocut layering planner

hi there! thanks for reading this.

2024.01.08
I've updated the code so it runs with OpenFrameworks of_v0.11.2_osx_release. Also, since
OpenFrameworks is "multiplatform" the reference to "executable" may be ".app" on a Mac platform.
Also, note that the output files created (see steps 4 & 9 below) have generic names, and may be overwritten on subsequent runs
of the program; so you might want to move them, to keep them from being overwritten. 
To be fixed in a future enhancement, perhaps?

2011.03.09.01.16
the project currently has 3 files, and depends on OpenFrameworks for most of its functionality.
for information about OpenFrameworks, please refer to http://openframeworks.cc.
for information about layered linocuts, I recommend Sherrie York's blog "Brush & Baren."

to use: 
- use the project generator that OpenFrameworks provides,
and replace the generated ofApp.h/cpp files with the corresponding files here.
- create a text file named imageloc.txt in the 'data' subfolder of the folder that contains the executable (bin, by default).
- add a path specification as the first line in that text file, that will point to
a folder containing one or more image files (.jpg, .bmp, .png, ...)
- add a path specification as the second line in that text file, that will point to
an existing folder in which files produced by illp will be stored. If the folder does
not exist, no files will be written.
- when the program runs, it will display, in the left half of its window, the first 
image file it finds in the given folder, scaled to fit; and a greyscale version on the right. 
- when you click the left mouse button in the left image, the right image will change to the 
color of the pixel you clicked on, and a small circle of that color will be displayed 
below the image. You can also click on a color from the 'palette' displayed below the images.
- when you click on the left image (or a palette color) again, the program goes through each pixel in the
image and decides if it's "closer" in color to the first pixel you clicked on or the
second; it then creates a two-color image which will be displayed on the right. The second
(and each subsequent) color you click on is also displayed in a circle under the images.
- click on up to 20 pixels in the left image (and/or the 'palette').
- if you see something you like, you can press the 's' key to save it (see step 4 above).
- if things get worse-looking, you can right-click to undo the last selected color.
- right-click as many times as you like, to undo selections.
- press the 'n' or 'p' keys to cycle through the images in the image folder (see
step 3 above). Color selections you've made (if any) are retained when the image changes.
- You can toggle between 2 (simplistic) algorithms that map image colors to your selected colors by pressing the 'e' key. 

The files created in step 9 will each contain 1 "linocut layer" corresponding to the 
selected color (they sometimes make interesting images). A copy of the 'posterized' image is also created.
