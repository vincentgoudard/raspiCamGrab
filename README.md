# About raspiCamGrab

![Screenshot of raspiCamGrab](screenshot.png)

### What??

raspiCamGrab is a program that displays a USB camera (namely a supereyes B008 microscope, here) onto a screen.
The image can be scaled and moved on X and Y axes.
Image adjustment is applied with a dedicated shader and allows the following effects :
- hue
- saturation
- brightness
- Red/Green/Blue gains
- contrast

### Expected Behavior

The camera should just show up with the settings stored in `/bin/data/settings.xml`.

- Press <kbd>h</kbd> to display/hide mouse and image adjustments (mouse control only).
-  Press <kbd>s</kbd> to save image adjustments in `/bin/data/settings.xml`.
- Press <kbd>l</kbd> to (re)load image adjustments from `/bin/data/settings.xml`.
- Press <kbd>s</kbd> to capture a snapshot in `/bin/data/screenshot.png`
- Press <kbd>Esc</kbd> to exit.

### Other classes used in this file

This example uses no other classes.

### Hiding boot sequence
There are various recommandations for hiding the boot sequence. Here's what worked for me, by editing the file /boot/cmdline.txt on a raspberry Pi 3 Model B+, running raspbian stretch :

- type `sudo nano /boot/cmdline.txt` in a shell
- then **remove** `console=tty1` (setting it to tty3, as recommanded by some didn't work here) 
- and add `logo.nologo quiet` at the end.

Beware that incorrect syntax might result in boot failure!

Also, if not starting X11, the above doesn't yield a login prompt in tty1. A workaround is to switch to another console with the command <kbd>alt</kbd>+<kbd>shift</kbd>+<kbd>F2</kbd>, then back to tty1 with <kbd>alt</kbd>+<kbd>shift</kbd>+<kbd>F1</kbd>.
