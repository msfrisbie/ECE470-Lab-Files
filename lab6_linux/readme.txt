This is a standalone linux g++ compiled implementation of the ECE470 image analysis algorithm.  

The purpose of this program is to allow development of lab 6 code without needing to be in the ECE470
lab, or needing a webcam.  

Provided are the source file and makefile, and also an example image file.  

To run, compile it using 'make', and run it using ./process

The program reads the file 'image.txt' from the same directory, analyzes it, and writes the analyzed
image to the file 'out.txt' in the same directory.  

The image, similar to the binary black/white image returned from the thresholded image from lab 5,
must be comprised of 0's and 1's.  

The image.txt file must be structured as follows:

line 0: width of image
line 1: height of image
<IMAGE VALUES>

See the included image.txt for an example.  

The program's image analysis finds the size, min+max values of the blocks, and centroid locations.

Although this program is configured for my lab solution, it can be easily tailored to work with your
solution.  
