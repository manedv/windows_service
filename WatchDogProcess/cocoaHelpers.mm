#include "cocoaHelpers.h"
#include <QWidget>
#include <Cocoa/Cocoa.h>
#include <QtMacExtras>
void SetAsOverlayWindow(QWidget * w, int offset)
{
    NSWindow *window = [(NSView*)w->winId() window];
    [window setLevel:kCGOverlayWindowLevel + offset];
}

QPixmap GrabScreen(unsigned int screenId)
{
    QPixmap resImage;

    uint32_t displayCount = 0;
    CGError err = CGGetActiveDisplayList(0, NULL, &displayCount);

    if(err == kCGErrorSuccess && screenId < displayCount)
    {
        CGDirectDisplayID *activeDisplays = new CGDirectDisplayID[displayCount];
        err = CGGetActiveDisplayList(displayCount, activeDisplays, &displayCount);

        if(err == kCGErrorSuccess && screenId < displayCount)
        {
            CGImageRef image = CGDisplayCreateImage(activeDisplays[screenId]);

            if(image != NULL)
            {
                resImage = QtMac::fromCGImageRef(image);
                CGImageRelease(image);
            }
            else
            {
                 printf("failed grabbing image for screen %d\n", screenId);
            }
        }
        else
        {
             printf("Failed getting active displays!\n");
        }

        delete []activeDisplays;
    }
    else
    {
        printf("Failed getting the number of displays!\n");
    }

    return resImage;

}
