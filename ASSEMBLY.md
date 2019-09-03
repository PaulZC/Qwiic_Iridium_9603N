# Qwiic Iridium 9603N: Assembly

Here are the assembly instructions for the Qwiic Iridium 9603N:

### Blank PCB

Start by having the blank PCBs manufactured. If you are based in the UK or Europe, I can recommend
[Multi-CB](https://www.multi-circuit-boards.eu/en/index.html) who can manufacture PCBs in 1-8 working days and
can process the Eagle .brd file direct - there's no need to generate Gerber files.

My recommended options are:
- Layers: 2 layers
- Format: single pieces
- Surface finish: chemical gold (ENIG)
- Material: FR4, 1.55mm
- Cu layers: 35um
- Solder stop: both sides, green
- Marking print: both sides, white

![Assembly1](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Assembly1.JPG)

### Add solder paste

Multi-CB can also provide you with a solder paste (SMD) stencil for the PCB. My recommended options are:
- SMD stencil for: top layer
- Make the Y dimension 20mm longer than the PCB itself to allow you to fix it down with tape
- Type: custom
- Pad reduction: yes, 10%
- Thickness: 100um
- Existing fiducials: lasered through
- Text type: half lasered
- Double-sided brushing: yes

I secure the blank PCB onto a flat work surface by locating it between two engineer's squares. I use a sheet of toughened glass
as the work surface as it is both very flat and easy to clean.

![Assembly2](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Assembly2.JPG)

Line up the stencil with the PCB and then secure the stencil with tape.

Apply solder paste close to the component cut-outs and then scrape the paste over the stencil using a knife blade
or a similar straight edge. Take appropriate safety precautions when working with solder paste - particularly if you are using
tin-lead solder instead of lead-free.

![Assembly3](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Assembly3.JPG)

![Assembly4](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Assembly4.JPG)

### Position the surface mount components

Position the components onto the blobs of solder paste using tweezers. A magnifier lamp or a USB microscope will
help you place the components in the correct position. J1 - the 20-way Samtec connector - is probably the trickiest
component to position. Take extra time to make sure the legs are centered accurately on the pads.
You may find the [SMT_Component_Placer](https://github.com/PaulZC/SMT_Component_Placer) useful.

![Assembly5](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Assembly5.JPG)

### Reflow the surface mount components

Use a reflow oven to heat the circuit board to the correct temperatures to melt the solder. A reflow oven doesn't need to be
expensive. The one shown below was assembled from:

- Quest 9L 800W mini-oven
- Inkbird PID temperature controller and 40A solid state relay
- Type K thermocouple

Several people have published good reflow oven construction guides, including [this one](http://tt7hab.blogspot.com/2018/06/the-reflow-oven.html)
and [this one](http://www.die4laser.com/toaster/index.html).

![Oven](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Oven.JPG)

Use the correct temperature profile for your solder paste, but you won't go far wrong with 160C for four minutes, followed by
210C for one minute, followed by a cool-down with the door open. Use a flashlight to check that the solder has melted across
the whole PCB at 210C. Hold the temperature at 210C a little longer if some of the solder paste still appears 'gray' instead of 'silver'.

![Assembly6](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Assembly6.JPG)

### Check for shorts

Carefully examine all the solder joints and correct any shorts you find.

The 'trick' to removing a short is to add more solder or solder paste and then to use
copper solder braid or wick to remove all the solder in one go.

![Shorts_1](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Shorts_1.JPG)

![Shorts_2](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Shorts_2.JPG)

![Shorts_3](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Shorts_3.JPG)

Use a cotton swab dipped in Iso-Propyl Alcohol (IPA / Propanol / rubbing alcohol) to remove any flux residue.

### Press-in nuts

The Iridium 9603N module is held in place by two 2-56 screws. The threaded 2-56 press-in nuts now need to be pressed into the _bottom_ of the
PCB. These are McMaster part number 95117A411. The nuts are best pressed into place using an arbor press.

![Assembly8](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Assembly8.JPG)

![Assembly7](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Assembly7.JPG)

### Add the non-surface mount components

The non-surface mount components (super capacitors & SMA connector) can now be soldered in by hand.

![Assembly9](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Assembly9.JPG)

![Assembly10](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Assembly10.JPG)

### Install the Iridium 9603N module

Take appropriate ESD precautions throughout and especially when handling the 9603N module.

Connect the 9603N to the beacon PCB using a HIROSE (HRS) u.FL-u.FL cable or similar. The cable needs to be 50 Ohm and approximately 25mm long.

Carefully fold the 9603N module over onto the PCB, insert the 20-way connector into the Samtec socket, then secure the module using:
- two 4.5mm OD x 6mm spacers (McMaster 94669A100)
- two 2-56 x 7/16" screws (McMaster 92185A081)

![Assembly11](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Assembly11.JPG)

![Assembly12](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Assembly12.JPG)

### Program the ATtiny841

The ATtiny841 can now be programmed using the hex file in the [Arduino folder](https://github.com/PaulZC/Qwiic_Iridium_9603N/tree/master/Arduino)
using your favourite programmer and the ISP header pads.

![ISP](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/ISP.JPG)

**You will need to change the ATtiny841 fuses to disable the internal clock divider (CKDIV8). Change the LOW fuse byte from 0x42 to 0xC2.**

![ISP_Fuses](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/ISP_Fuses.JPG)

Finally, program the ATtiny841 with the hex file and your board is ready for use.

![ISP_Code](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/ISP_Code.JPG)

### Secure J1 with Epoxy

I recommend securing J1 to the PCB with a small amount of epoxy to make it more robust. 3M Scotch-Weld 2216 is an ideal choice as it remains slightly flexible when cured.
You will need to temporarily remove the 9603N while you apply the epoxy. Apply the mixed epoxy around the body and legs of J1 using a toothpick,
taking great care to keep the epoxy clear of the connector opening and pins. Make sure the epoxy is fully cured before re-installing the 9603N.

### Lacquer the PCB

I recommend giving the PCB a coat of lacquer, especially if you are intending to use it to track a balloon flight.
You will need to temporarily remove the 9603N while you apply the lacquer. Cover all of the surface mount components with
[Acrylic Protective Lacquer (conformal coating)](https://uk.rs-online.com/web/p/conformal-coatings/3217324/) except: J1, J7, the jumper links and the IO pads.

Once the lacquer is dry, re-attach the 9603N and your board is ready to fly.

![Qwiic_Iridium_9603N_2](https://github.com/PaulZC/Qwiic_Iridium_9603N/blob/master/img/Qwiic_Iridium_9603N_2.JPG)

### The Small Print

This project is distributed under a Creative Commons Attribution + Share-alike (BY-SA) licence.
Please refer to section 5 of the licence for the "Disclaimer of Warranties and Limitation of Liability".

Enjoy!

**_Paul_**