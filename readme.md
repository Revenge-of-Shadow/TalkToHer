# Dependencies:

A lot of compatibility issues might arise.
I frankly do not remember how many things I ended up dealing with.
I had to fight libFLAC.so.12 issue... I frankly just communicated with Claude Sonnet and walked off with a PKGBUILD.

## SFML-2.6.2. 

Get it from https://www.sfml-dev.org/download/sfml/2.6.2/ and paste "SFML-2.6.2" into this directory.
Or remap it to your installation location in "compile.sh".

Otherwise there are instructions on https://www.sfml-dev.org/tutorials/2.6/start-linux.php for Linux and also for other platforms.


## C++ 17 or newer

To prepare for **first launch**, run 
**precomp.sh**
**compile.sh.**
Then, run 
**run.sh**
for each and any launch. FROM TERMINAL.



# Introduction

## What's this?

A visual novel engine on C++ and SFML. It integrates the most vivid part of the story from my interactive novel game Awakening, being, in a way, an upgrade to it.



# Usage

## Controls

Enter, Right, D - forward, agree

BackSpace, Left, A - back

Down, S - move to the next option; scroll the text

Up, W - move to the previous option; scroll the text back

Escape - close

Space - show next dialogue (forward for text exclusively)

## Functionality

This game allows for reading the scenarios which do **not** depend on the source code. More on scenarios later.

Saving and basic settings as well as extras are also available.





This program allows for creating custom scenarios and working with them without changes to the source code. Obviously.



# Operation

## Scenario, script

Those two terms can be synonymous. I tend to think that **script** must have executable lines unlike **scenario**.

It seems I still had not fixed the issue that **the first line of the script can not be a command** because it shall **not be executed**.




### File structure

**All the written paths are processed depending on the operational system!**



Meaning that directory separator would be replaced with "\\" for Windows and with "/" for Linux.





Scenarios are placed in the "scenarios" directory into their own directories. 





scenarios

--Talk to her

----script.txt    <- Contains all of the text that appears on screen as well as the commands to execute.

----options.txt     <- Contains options list that redirect to other scenarios.

----title.txt   <- Little trinket that changes the window title and **is displayed in scenario selection** in main menu.

----ask     <-

----pet     <- Those two are directories. I recommend nesting scenario parts in one directory, but this is optional.

------script.txt

------...

### script.txt

Any line that starts with "**//**" is not being shown or processed. It is considered a commentary.

Any line that starts with "**/**" is considered a command and is parsed to be executed.

Any other line is processed to be shown on screen with comments and spaces and the end of it getting truncated.



#### Commands



- sleep(milliseconds) sleep for given amount of milliseconds.





##### Object commands: executed for objects. Syntax: object.command(argument)

Just like sounds, objects **will not** be removed as scenario ends... however it does not really matter, since objects are only shown in scenarios. It is recommended but not necessary to manually remove them at the end of the script.



Syntax:



object.

- (name) - load the character (or just an object). Has to be called before any other object commands.

- (name).loadSprite(filename) - loads the sprite from an image file "sprites/filename".

- (name).setScale(xscale, yscale) - sets width and height scaling multiplier.

- (name).setPosition(xpos, ypos) - sets the position according to aforementioned rules.

- (name).move(xpos, ypos) - moves the position according to aforementioned rules.

- (name).remove() - unloads the object. Effectively, it just makes it disappear.



##### Sounds

To play sounds and music.

**Will not stop** as the scenario (script) ends. Music is reset on scenario **load**, not scenario **closing**, so it is necessary to manually end the playback at the end of the script.



- play(filename) - load and play the sound file "sounds/filename".

- loop(filename) - load and loop the sound file "sounds/filename".

- stop(filename) - stops the sound file "sounds/filename". 



#### Graphics definitions



One size unit is a pixel. 

HOWEVER

Positions are relative! Each position/move value is given in percents of window width and height - x and y position respectively. 

(0,0) is top left.

(100, 100) is bottom right.



Default window size is (1600, 900) pixels. Sprites, respectively, are images, and have size in pixels as well. Whatever does not fit in the screen would not be visible. This is to be used when parts of the sprite must be hidden off-frame. Otherwise the sprites can be scaled.



Backgrounds are represented as objects.



### options.txt

The file contains options separated by line breaks.

The part before ':' is the relative path from "scenarios" directory; the part after ':' is the text shown during the option showcase in game.





Talk to her/pet:Pet her.



Talk to her/ask:Ask her what she wants.

### title.txt

Optional, especionally for nested parts of the scenario. 

Contains the title to override the window title and to be shown in scenario selection menu as well as load menu.





In scenario selection menu will be shown only those scenarios which are placed within the directories in scenarios. 



"Talk to her" would appear in scenario selection, whereas "ask" (Talk to her/ask) would not.

## Sprites

Image files placed into "sprites" directory.

Currently tested only for PNG files.



Sprites are used for characters, objects and backgrounds.

Management of sprites is handled via **script.txt**.

## Storage

Directory to store settings and extra generated files.

It is safe to delete any of these files.

Not the directory itself, though.

## Saves

A directory to store save files which are safe to delete.

With obvious consequences.

## Crimes

The comedy.


