# Zen

### the-modern-hourglass
Ryan Chen, Akash Harapanahalli, Andrew Hellrigel, Jinwoo Park

## Inspiration
There is no doubt that life the past few years has been hectic to say the least. That is why we built Zen, a device that is equal parts complex data-visualization tool and gorgeous art piece. Underneath the calming sandy surface of Zen lies a complex mechanism built from the scrapped parts of a broken 3d printer. A robotic arm magnetically guides a stainless steel marble in the sand to create grooves that can tell you the time, display text, and more. This combination of art with data reminds us that there is always beauty in information: you just need to _dig a bit deeper_.

## What it does
An onboard microcontroller first chooses what data it wishes to display: say the current time. Using computer vision, we calculate the coordinates that would carve out the numbers in the sand. These coordinates are passed through our custom pathing algorithm to determine a most efficient route, which is then sent to the mechanical assembly. An on-board computer translates the rectangular coordinates to polar coordinates to determine which angle and radius values to move the magnetic arm. As the arm traces its path under the surface of the table, the magnetic ball slowly imprints out the number in the sand, giving us a dynamic and physical means to visualize data.

## How we built it
To view the plotting process in real time, follow the Colab notebook [here](https://colab.research.google.com/drive/160TNZnRWTsK02QmsMjm_axGDtRKRd9Zn?usp=sharing). The project broke down into software focused on the plotting algorithm and hardware focusing on the mechanism of the magnetic arm. The software team used the opencv library to skeletonize the paths of a single-stroke font family. This allowed us to select the coordinates of the image that should be part of the letter/number and put them into our algorithm that will trace an optimal path through those points whilst removing outliers at the same time. This trace information can therefore be used to plot any set of letter and numbers, as long as they fit within the dimensions of the display.

The hardware side involved iterating over several designs until we settled on the current radially pointed magnetic arm. A lot of soldering, CADing, sawing, lasering, and drilling eventually gave us the semi-transparent design for Zen.

## Challenges we ran into
On the hardware side, we ran into several issues, the biggest being that the tolerances from our 3d prints were too tight, leading to our entire assembly being unable to spin freely around the center. But with a little ingenuity (and a little hammering), we managed to salvage the situation without having to cut, break, sand, or print any new parts. On the software side, we were dealing with an unfamiliar library and for some, a completely new language, so we first had to climb a learning curve to come out on the other end successful.

## Accomplishments that we're proud of
With this being the third hackathon we have completed together as team Big Fishies, we have noticed a significant increase in both our design quality and aesthetics. The overall look of the device and it's capabilities has demonstrated our collective growth as a team of engineers and a group of friends.

## What we learned
We came into this project idea unsure about whether we would be able to complete this goliath task over the course of the short 36 hours we were allotted. Learning new languages, new skills, and new techniques in such a short amount of time is never an easy task, but creating Zen has been the challenge that has proven there are no limits to the things we can accomplish.

## What's next for Zen
We have more plans for what sort of information we can display on our 550 square inch sand display. Using complex apis, we can display stock charts in real time, or even communicate wirelessly with a drawing application to allow users to draw their own paths in the sand. There are as many possibilities for Zen as there are grains of sand on Earth.
