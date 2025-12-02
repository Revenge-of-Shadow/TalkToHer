Dependencies: SFML-2.6.2. I installed it into "/usr/local/", so that compiler links to the default path. It is the only way it worked for me.
But this is not all – I had to append "/usr/local/lib" to my "/etc/ld.so.conf.d/lib32-glibc.conf". Only then it could launch.
Otherwise there are instructions on https://www.sfml-dev.org/tutorials/2.6/start-linux.php for Linux and also for other platforms.

Build: run compile.sh.


Script:

Ho... 'tis a long one.

One size unit is a pixel. 
HOWEVER
Positions are relative! Each position/move value is given in percents of window width and height - x and y position respectively. 
(0,0) is top left.
(100, 100) is bottom right.

Default window size is (1600, 900) pixels. Sprites, respectively, are images, and have size in pixels as well. Whatever does not fit in the screen would not be visible. This is to be used when parts of the sprite must be hidden off-frame. Otherwise the sprites can be scaled. When that would be IMPLEMENTED, of course.
Scratch that! Scale is now forced to fit the resized window. Until I implement separate scaling.

Note: It is recommended to make background images oversized so that they shall be cropped by resizes instead of leaving black background behind.
