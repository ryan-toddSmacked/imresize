# imresize
C++ application that resizes images using OpenCV functions.
This application follows the KISS epistemology.
KISS - "Keep it simple stupid"

## Dependencies
Required Libraries:
* [OpenCV](https://opencv.org/) - For image loading/processing/writing
* [Boost](https://www.boost.org/) - For command lind parsing

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

## Usage
```bash
./imresize -i <input image> -o <output image> --scale 0.8 # Scale the input image by a factor of 80% in rows and columns
./imresize -i <input image> -o <output image> --width 2000 --height 50 # Resize the image to have 2000 columns and 50 rows.
```

## Contributing
Any advanced cmake users, Pull request are welcome.
For any other changes, please open an issue first to discuss what you feel should be changed.

## License
You can modify my C++ file [imresize.cpp]() all you want.
[MIT](https://choosealicense.com/licenses/mit/)
