Dependencies: SFML-2.6.2. I installed it into "/usr/local/", so that compiler links to the default path. It is the only way it worked for me.
But this is not all – I had to append "/usr/local/lib" to my /etc/ld.so.conf.d/lib32-glibc.conf". Only then it could launch.
Otherwise there are instructions on https://www.sfml-dev.org/tutorials/2.6/start-linux.php for Linux and also for other platforms.

Build: run compile.sh.
