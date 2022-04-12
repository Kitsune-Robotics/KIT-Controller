from shutil import copyfile
from PIL import Image, ImageFont, ImageDraw
from pysstv import color

import logging as log

try:
    from picamera import PiCamera
except ModuleNotFoundError:
    log.info("Running in simulator mode")


def take_photo():
    # This def is meant to take a photograph from the robot, 
    # it should include all steps and error checking to raise the mast
    # Take the photo, and put the mast down.

    # Copy in the test pattern png (if photo process errors out, this will be used instead)
    log.debug('Copying test pattern.')
    copyfile('photos/TEST_PATTERN.jpg', 'working/working.jpg')

    # Software to take the photo should be here
    #copyfile('photos/camera_latest.jpg', 'working/working.jpg')
    log.debug('Initalizing camera.')
    try:
        camera = PiCamera()
        log.info('Saving photo.')
        camera.capture('working/working.jpg')
    except NameError:
        log.info("Running in simulator mode, not replacing test pattern")

def mark_photo():
    log.info('Opening photo for viewing.')
    raw_img = Image.open("working/working.jpg") # Open the current working image
    log.info('Resizing image.')
    img = raw_img.resize((320, 240), Image.ANTIALIAS) # resize it for the radio

    log.info('Drawing on image.')
    if False:
        TINT_COLOR = (255, 255, 255) # White text bg
        TEXT_COLOR = (0,0,0)
    else:
        TINT_COLOR = (0, 0, 0) # Black text bg
        TEXT_COLOR = (255,255,255)
    TRANSPARENCY = .25  # Degree of transparency, 0-100%
    OPACITY = int(255 * TRANSPARENCY)
    overlay = Image.new('RGBA', img.size, TINT_COLOR+(0,))

    draw = ImageDraw.Draw(overlay)
    #bigfont = ImageFont.truetype(r'C:\Users\System-Pc\Desktop\arial.ttf', 20) 
    #smallfont = ImageFont.truetype(r'C:\Users\System-Pc\Desktop\arial.ttf', 17) 
    
    draw.rectangle(((0, 0), (90, 20)), fill=TINT_COLOR+(OPACITY,))
    #draw.text((0, 0),"KW1FOX",TEXT_COLOR,font=bigfont) # Draw KW1FOX in the top left

    draw.rectangle(((0, 40), (83, 80)), fill=TINT_COLOR+(OPACITY,))
    #draw.text((0, 40),"day: 25.2",TEXT_COLOR,font=smallfont)
    #draw.text((0, 60),"Volt: 13.8",TEXT_COLOR,font=smallfont)
    #draw.text((0, 80),"Miles: 1.02",TEXT_COLOR,font=smallfont)

    log.info('Converting image color.')
    img = img.convert("RGBA")
    img = Image.alpha_composite(img, overlay)
    img = img.convert("RGB")
    img.save('working/working.jpg') # Save the working image
    return img

if __name__ == "__main__":
    log.basicConfig(level=log.DEBUG)

    # Take photograph. 
    log.info('Taking Photograph')
    take_photo() # Saves a photograph to the working/working.jpg location
    log.info('Done taking photo.')

    # Draw neccicary text on photo
    log.info('Drawing on photo.')
    radio_photo = mark_photo() # draws text on working/working.jpg and returns a PIL image
    log.info('Done drawing on photo.')

    log.info('Creating slowscan.')
    slowscan = color.Robot36(radio_photo, 48000, 16) # Image, rate, bits
    #slowscan = color.MartinM1(radio_photo, 48000, 16) # Image, rate, bits
    log.info('Saving out slowscan.')
    slowscan.write_wav('working/working.wav')

    #sstv('working/working.png', 'working/radio.wav', mode='Robot36')
