## You can [download it here](https://github.com/softblueish/foxy/releases/download/Dialogue/Foxy-x86-64-Windows.zip)
# A program that has a chance of withered foxy jumpscaring you every keypress
<img width="630" height="300" alt="example" src="https://github.com/user-attachments/assets/9711932b-ccd6-4149-8f16-df256277fe95" />
<video width="630" height="300" src="https://github.com/user-attachments/assets/7532b7e1-2bbd-4d78-922d-77c35ba67643"></video>
Video example by [LilaPMD on Youtube](https://www.youtube.com/channel/UCT550UpYYhsUmybnP1ojX-g) (Watch her content 💜)

## How to use
You just run the exe and it'll start working immediately system-wide as a background service! Doesn't work on Linux or macOS, I am so sorry please do not crucify me.
## Changing settings
You can change the % chance in ``settings.conf``. (100 means 100%, 0.1 means 0.1%)    
You can also have the chance trigger every second if you'd like by setting ``timerMode=true`` in the ``settings.conf`` file.  
If you're running OBS, you can turn on ``streamerMode=true`` which adds a secondary window you can add to OBS as a source.  
## How to build
``g++ .\main.cpp -lSDL3 -mwindows -o foxy.exe``
## Inspiration
This program was inspired by the [1/10000 Chance for Withered Foxy Jumpscare Every Second](https://steamcommunity.com/sharedfiles/filedetails/?id=3481943642) terraria mod by YMY.
