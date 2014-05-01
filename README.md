Sonetta
==========

Current status
----------
Check out the following screenshots (admittedly not *entirely* up-to-date) for a taste of what Sonetta is.

#### Album browsing
![Album browsing](https://raw.github.com/Andlon/sonetta/master/images/albumbrowse.jpg)

#### Playlists
![Playlists](https://raw.github.com/Andlon/sonetta/master/images/playlists.jpg)

#### Now playing
![Now Playing](https://raw.github.com/Andlon/sonetta/master/images/nowplaying.jpg)

#### Search
![Search](https://raw.github.com/Andlon/sonetta/master/images/search.jpg)

Sonetta implements a good chunk of Spotify's features, but there is a lot of work to be done, mostly in the user interface department. More information will appear when Sonetta gets closer to its first release. Stay tuned!

Compilation
--------------
### Prerequisites
#### Spotify premium account
A Premium account is required to use libspotify, and hence it is a requirement for Sonetta as well.

#### Application key (appkey.c)
In order to compile Sonetta, a Spotify application key is required. Since Spotify prohibits application keys from being redistributed (with good reason!), you'll have to get one of your own. It is usually quite easily obtained though. Apply for your key here:
https://devaccount.spotify.com/my-account/keys/

Once your key has been approved by Spotify, you may download it as "C-Code" from the same website. This gives you an 'appkey.c' file which you must place in the root folder of Sonetta. That is, the same directory as this document.

#### libspotify
libspotify needs to be accessible for inclusion and linking. Get it from here:
https://developer.spotify.com/technologies/libspotify/

Be sure to choose the appropriate package for your system architecture.

For Linux, simply follow the instructions to install libspotify on your system. For Windows, extract the package to a directory of your choosing, and set the environment variable LIBSPOTIFY_PATH to the appropriate directory.

#### Qt
Sonetta requires Qt 5.2.0 or later, assuming this README is up-to-date. Usually, the safest route is to get the most recent version available from the Qt online maintenance kit.

### Steps
#### Update submodules
After cloning this repository, you need to init and update the submodules, which at the moment only consists of spotinetta, a Qt-based wrapper for libspotify used by Sonetta. Run the following commands:
    
    git submodule init
    git submodule update
    
This should clone the repository of spotinetta and make it reside in the 'dependencies/spotinetta' folder.

#### Building
On Linux, simply type the following commands into the terminal from the root folder of Sonetta:

    qmake
    make
	
which will build Sonetta and its dependencies. As with any other application that uses make, you may use the -j option to specify the amount of cores. For Windows, replace 'make' with 'nmake' (for Visual Studio). If you're a developer and want to run Sonetta's (at the moment very limited) test suite, run 'make check'. 

If you want to develop with Qt Creator, open sonetta.pro in the root directory, and add "bash deploy.sh" (Linux) or "deploy.bat" (Windows) as a custom deploy step. If you're not using Qt Creator, note that you may have to manually call these shell scripts whenever you change any of the deployed assets (QML, fonts etc.).

License
-------
Sonetta is open-source software, released under a permissive 3-clause BSD license. The copyright belongs to Andreas Borgen Longva. There should be a LICENSE file supplied along with this document which contains the legal wording. Qt and libspotify are released under their own respective licenses.
