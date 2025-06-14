<h1>README</h1>

<h2>How To Play The Demo</h2>

This is the project strucuture

- Game Folder
  - The application
  - Assets Folder
      - "Audio" Folder
      - "Scripts" Folder
      - "Images" Folder
      - "Fonts" Folder

To figure out how the scripting language works, please refer to the demo
All the code is stored in text files that get parsed at runtime
Should be straightforward enough for someone with proggramming experience to learn

The Assets Folder for the demo can be found through the google drive link below
https://drive.google.com/file/d/1WxtlcQaG6NjX-KlW07qK-aOOR9VlWNWi/view?usp=sharing

<h2>How To Build</h2>

To Build the Game Engine first create a folder and move all of the source code files into the folder.
You can open the files in visual studio through the sln file however make sure you have the proper
platform toolset installed, this should be <u>Visual Studio 2019 (v142)</u>, the project requires this to properly be edited.

Once opening the files, it's likely that visual studio is trying to inform you that it can't find any of the needed SDL2 files.
Don't worry, this is normal, you will have to configure the dlls through the VC++ Directories tab in the property pages.

You can find these dll files either from <i>SDL's website</i> or through the demo as part of the files that are needed to run the engine.
When building through this system you will end up with only an exe, when you run the program make sure that the needed dlls (again these can be found in the demo folder)
are in the same folder.

From here you can really do anything that the standard engine can, keep in mind that even when running from the IDE, the program will expect an Assets folder in the same directory.
