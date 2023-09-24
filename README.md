# imresize
C++ application that resizes images using OpenCV functions.
This application follows the KISS epistemology.
KISS - "Keep it simple stupid"

## Dependencies
Required Libraries:
* OpenCV - For image loading/processing/writing
* Boost - For command lind parsing

## Installation
I used a conda environment to install the dependencies.
My cmake file is not robust, it was specifically designed to look for a conda environment.
The steps I use to install the environment.
```bash
conda create --prefix $(pwd)/env
conda activate $(pwd)/env
conda install -c conda-forge opencv boost
```

A better thing to implement in the cmake file, is to allow the user to provide a base directory
for these dependencies, instead of forcing the cmake to look for the libraries in the $(pwd)/env
directory.



