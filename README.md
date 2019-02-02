# BatchResize

BatchResize is a front end for Image Magick to resize images as fast as possible with no overheads. It is coded in C++ using the Qt library and is distributed under GPL v3.0 license.

BatchResize is a front end for [Image Magick](https://www.imagemagick.org/) to process images in batch. BatchResize is ditributed with icons packs taken from [Breeze icons](http://https://github.com/KDE/breeze-icons), [Tango icons](https://github.com/Distrotech/tango-icon-theme) and [Faenza icons](https://github.com/shlinux/faenza-icon-theme).
For more information on how to use the software you can jump over the [wiki](https://gitlab.com/pierre-db/BatchResize/wikis/home). To download the binaries you can go to the project's page on [Sourceforge](https://sourceforge.net/projects/batchresize/).

## Build

Here are the instructions to compile the project on Linux. You will need the qt5-dev package installed.

1. First you need to download the sources.

There are several ways of downloading the sources. If you have git installed you can do:

`git clone https://gitlab.com/pierre-db/BatchResize/`

Let's assume you have the *Resize* and *BatchResize* directories in a folder named */path/to/BatchResize*. You can go into this folder:

`cd /path/to/BatchResize`

2. Once downloaded we are goind to build the project file Resize.pro.

Go into the Resize directory:

`cd /path/to/BatchResize/Resize`

Run qmake to create the makefile:

`qmake "CONFIG+=release" ./Resize.pro`

Then make the makefile:

`make all ./Makefile`

Let's tidy up a bit and move the executable file into the root of our work folder:

`rm *.o`

`rm moc_*`

`mv ./Resize ../resize`

3. Now let's compile BatchResize with the same commands:

`cd /path/to/BatchResize/BatchResize`

`qmake "CONFIG+=release" ./BatchResize.pro`

`make all ./Makefile`

`rm *.o`

`rm moc_*`

`mv ./BatchResize ../BatchResize`

4. You should be all set! The last step is to launch BatchResize:

`/path/to/BatchResize/BatchResize`

Note that the first time you run BatchResize it will automatically generate a configuration file into your home directory.
By default the program will look for the icons displayed in the program in a directory called *icons* located in the same folder as the executable file.
If the icons are located in another folder, you can reconfigure the icons location by going into the settings.

## Binaries

You can go to the [Sourceforge](https://sourceforge.net/projects/batchresize/files/) page and grab the latest bineries for your plateform. On linux you may need to install the dependencies qt5 and magick.