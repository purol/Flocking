# Flocking
It is made for describing flocking behavior.

You can build it by Visual Studio 2019 (different version can cause some problem.)
or you can just run x64\Release\opencvtest.exe. 

Motivation
-------------
When I visited Kyoto Aquarium(October 12, 2019), I saw lots of tiny fishes which moved with synchronization. I was inspired about that, so I have tried to the way how I can described it.

How to use it
-------------
> If you do not have Visual Studio, then you need do install Visual C++ Redistributable. Here is the link (https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads)
Build the code or just run x64\Release\flocking.exe

Gif files
-------------
![Alt text](/flocking/image/add_wall_2020-03-01_10-24-07.gif "1")
![Alt text](/flocking/image/align_angle_only.gif "2")
![Alt text](/flocking/image/2020-03-02_20-39-55.gif "3")
![Alt text](/flocking/image/2020-03-02_20-40-45.gif "4")
![Alt text](/flocking/image/2020-03-02_20-41-39.gif "5")
![Alt text](/flocking/image/2020-03-02_20-42-42.gif "6")

How to play
-------------
Q button : adding one creature

W button : removing one reature

mouse right button : attracting creatures

mouse left button : repulse creatures

I hope you enjoy it!

How it works
-------------
There are three rules.
1. separation among creatures. Within specific distance, creatures try to break away from others. 
2. alignment among creatures. Within specific distance, creatures try to get average velocity and orientation with others. 
3. cohesion among creatures. Within specific distance, creatures try to go to average point of others. 

Moreover, I added additional rule for evading wall and mouse control(repulse and allure to mouse)

Photography
-------------
![Alt text](/flocking/image/20191012_111544.jpg "7")
![Alt text](/flocking/image/20191012_111822.jpg "8")
![Alt text](/flocking/image/20191012_112216.jpg "9")
These are the photography which I took on kyoto aquarium. You can see tiny fishes moving on the tank.

Reference
-------------
http://aidev.co.kr/alife/477
