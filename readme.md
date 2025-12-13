Dependencies: SFML-2.6.2. I installed it into "/usr/local/", so that compiler links to the default path. It is the only way it worked for me.
But this is not all – I had to append "/usr/local/lib" to my "/etc/ld.so.conf.d/lib32-glibc.conf". And then run "sudo ldconfig". Only then it could launch.
Otherwise there are instructions on https://www.sfml-dev.org/tutorials/2.6/start-linux.php for Linux and also for other platforms.

Build: run compile.sh.


Script:

Ho... 'tis a long one.

One size unit is a pixel. 
HOWEVER
Positions are relative! Each position/move value is given in percents of window width and height - x and y position respectively. 
(0,0) is top left.
(100, 100) is bottom right.

Default window size is (1600, 900) pixels. Sprites, respectively, are images, and have size in pixels as well. Whatever does not fit in the screen would not be visible. This is to be used when parts of the sprite must be hidden off-frame. Otherwise the sprites can be scaled.

Commands:

sleep(milliseconds) sleep for given amount of milliseconds.

Object commands: executed for objects. Syntax: object.command(argument)

object.

- (name) - load the object (or just an object). Has to be called before any other object commands.
- (name).loadSprite(filename) - loads the sprite from an image file "sprites/filename".
- (name).setScale(xscale, yscale) - sets width and height scaling multiplier.
- (name).setPosition(xpos, ypos) - sets the position according to aforementioned rules.
- (name).move(xpos, ypos) - moves the position according to aforementioned rules.
- (name).remove() - unloads the object. Effectively, it just makes it disappear.

Backgrounds are represented as objects.
