Sonetta
==========

Current status
----------
![Alt text](https://raw.github.com/Andlon/sonetta/master/images/tracks.png "Sonetta's track view as of 28/11/13")

Sonetta implements a good chunk of Spotify's features, but there is a lot of work to be done, mostly in the user interface department. More information will appear when Sonetta gets closer to its first release. Stay tuned!

How to compile
----------
Detailed instructions coming soon (hopefully). In short, you need to init/update submodules, build spotinetta, get appkey.c from Spotify, place it in Sonetta's root folder, and make libspotify headers/library available in include/library paths. Then

	qmake
	
### Windows

	nmake
	deploy.bat

### Linux:
	make
	sh deploy.sh
	

