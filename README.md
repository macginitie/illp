# illp
interactive linocut layering planner

hi there! thanks for reading this.

2024.01.08
I've updated the code so it runs with OpenFrameworks of_v0.11.2_osx_release. As a result, 
step 1 below is out of date; instead, you can use the project generator that OpenFrameworks provides,
and replace the generated ofApp.h/cpp files with the corresponding files here. Also, since
OpenFrameworks is "multiplatform" the reference to ".exe" may be ".app" on a Mac platform.
Also, note that the output files created (see steps 4 & 9 below) have generic names, and may be overwritten on subsequent runs
of the program so you might want to move them, to keep them from being overwritten... 
to be fixed in a future enhancement, perhaps?

2011.03.09.01.16
the project currently has 3 files, and depends on OpenFrameworks for most of its functionality.
for information about OpenFrameworks, please refer to http://openframeworks.cc.
for information about layered linocuts, I recommend Sherrie York's blog "Brush & Baren."

to use: 
(obsolete, see above) 1. create an openFrameworks project (after downloading OpenFrameworks), replace the 
default main.cpp, remove testApp.h/cpp from the solution, and add illp.h/cpp in their stead.
2. create a text file named imageloc.txt in the folder that contains the .exe (bin, by default).
3. add a path specification as the first line in that text file, that will point to
a folder containing one or more image files (.jpg, .bmp, .png, ...)
4. add a path specification as the second line in that text file, that will point to
an existing folder in which files produced by illp will be stored. If the folder does
not exist, no files will be written.
5. when the program runs, it will display, in the left half of its window, the first 
image file it finds in the given folder, scaled to fit; and a greyscale version on the right. 
6. when you click (the left mouse button) in the left image, the right image will change to the 
color of the pixel you clicked on, and a small circle of that color will be displayed 
below the image. You can also click on a color from the 'palette' displayed below the images.
7. when you click on the left image (or a palette color) again, the program goes through each pixel in the
image and decides if it's "closer" in color to the first pixel you clicked on or the
second; it then creates a two-color image which will be displayed on the right. The second
(and each subsequent) color you clicked on is also displayed in a circle under the images.
8. click on up to 20 pixels in the left image.
9. if you see something you like, press the 's' key to save it (see step 4 above).
10. if things get worse-looking, you can right-click to undo the last selected color.
11. right-click as many times as you like, to undo selections.
12. press the 'n' or 'p' keys to cycle through the images in the image folder (see
step 3 above). Color selections you've made (if any) are retained when the image changes.
13. You can toggle between 2 (simplistic) algorithms that map colors by pressing the 'e' key. 

The files created in step 9 will each contain 1 "linocut layer" corresponding to the 
selected color. A copy of the 'posterized' image is also created.
