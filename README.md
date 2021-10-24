the-modern-hourglass

## Inspiration
There is no doubt that life the past few years has been hectic to say the least. That is why we built Zen, a device that is equal parts complex data-visualization tool and gorgeous art piece. Underneath the calming sandy surface of Zen lies a complex mechanism built from the scrapped parts of a broken 3d printer. A robotic arm magnetically guides a stainless steel marble in the sand to create grooves that can tell you the time, display text, and more. This combination of art with data reminds us that there is always beauty in information: you just need to _dig a bit deeper_.

## What it does
An onboard microcontroller first chooses what data it wishes to display: say the current time. Using computer vision, we calculate the coordinates that would carve out the numbers in the sand. These coordinates are passed through our custom pathing algorithm to determine a most efficient route, which is then sent to the mechanical assembly. An on-board computer translates the rectangular coordinates to polar coordinates to determine which angle and radius values to move the magnetic arm. As the arm traces its path under the surface of the table, the magnetic ball slowly imprints out the number in the sand, giving us a dynamic and physical means to visualize data.

## How we built it
To view the plotting process in real time, follow the Colab notebook [here](https://colab.research.google.com/drive/160TNZnRWTsK02QmsMjm_axGDtRKRd9Zn?usp=sharing). The project broke down into software focused on the plotting algorithm and hardware focusing on the mechanism of the magnetic arm.

## Challenges we ran into

## Accomplishments that we're proud of

## What we learned


## What's next for Zen
We have more plans for what sort of information we can display on our 550 square inch sand display. Using complex apis, we can display stock charts in real time, or even communicate wirelessly with a drawing application to allow users to draw their own paths in the sand. There are as many possibilities for Zen as there are grains of sand on Earth.
