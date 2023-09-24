# imresize
C++ application that resizes images using OpenCV functions.

## Installation
I used a conda environment to install the dependencies.
My cmake file is not robust, it was specifically designed to look for a conda environment.
The steps I use to install the environment.
```bash
conda create --prefix $(pwd)/env
conda activate $(pwd)/env
conda install -c conda-forge opencv boost
```
## Building
Required Libraries:
* OpenCV - For image loading/processing/writing
* Boost - For command lind parsing


