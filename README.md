# BatchResize

BatchResize (formerly or Quick Batch Resize or QBatch Resize) is a front end for Image Magick to resize images as fast as possible with no overheads. It is coded in C++ using the Qt library and is distributed under GPL v3.0 license.

## Build

Here are the instructions to compile the project on Linux. You will need the qt5-dev package installed.

1. First you need to download the sources. I won't go in to the details of this step and let you decide your favorite way of doing this.

Let's assume you have the *Resize* and *BatchResize* directories in a folder named */path/to/BatchResize*.
We're going to create a new folder *bin* to put the binaries into once the compilation is done:

`cd /path/to/BatchResize`
`mkdir ./bin`

2. Once downloaded we are goind to build the project file Resize.pro.

Go into the Resize directory:

`cd /path/to/BatchResize/Resize`

Run qmake to create the makefile:

`qmake "CONFIG+=release" ./Resize.pro`

Then make the makefile:

`make all ./Makefile`

Let's tidy up a bit and move the executable file somewhere else:

`rm *.o`
`rm moc_*`
`mv ./Resize ../bin/resize`

3. Now let's compile BatchResize with the same commands:

`cd /path/to/BatchResize/BatchResize`
`qmake "CONFIG+=release" ./BatchResize.pro`
`make all ./Makefile
`rm *.o`
`rm moc_*`
`mv ./BatchResize ../bin/BatchResize`

4. You should be all set! The last step is to launch BatchResize:

`/path/to/BatchResize/bin/BatchResize`

## Binaries

You can go to the [Sourceforge](https://sourceforge.net/projects/batchresize/files/) page and grab the latest bineries for your plateform. On linux you may need to install the dependencies qt5 and magick.