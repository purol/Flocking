# Flocking
It is made for describing flocking behavior.

Motivation
-------------
When I visited Kyoto Aquarium(October 12, 2019), I saw lots of tiny fishes which moved with synchronization. I was inspired about that, so I have tried to the way how I can described it.

How to use it
-------------
Build the code or just run x64\Release\flocking.exe

Gif files
-------------
![Alt text](/flocking/gif/add_wall_2020-03-01_10-24-07.gif "1")
![Alt text](/flocking/gif/align_angle_only.gif "2")
![Alt text](/flocking/gif/2020-03-02_20-39-55.gif "3")
![Alt text](/flocking/gif/2020-03-02_20-40-45.gif "4")
![Alt text](/flocking/gif/2020-03-02_20-41-39.gif "5")
![Alt text](/flocking/gif/2020-03-02_20-42-42.gif "6")

How to play
-------------
Q button : adding one creature

W button : removing one reature

mouse right button : attracting creatures

mouse left button : repulse creatures

I hope you join it!

How it works
-------------
There are three rules.
1. separation among creatures. Within specific distance, creatures try to break away from others. 
2. alignment among creatures. Within specific distance, creatures try to get average velocity and orientation with others. 
3. cohesion among creatures. Within specific distance, creatures try to go to average point of others. 

Moreover, I added additional rule for evading wall and mouse control(repulse and allure to mouse)

Reference
-------------
http://aidev.co.kr/alife/477
