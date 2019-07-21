/* FingerprintUSBHost - USB Host Fingerprinter library
 * Copyright (c) 2016 Jesse Vincent
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "FingerprintUSBHost.h"

#if defined(USBCON)

FingerprintUSBHost_ FingerprintUSBHost;

int FingerprintUSBHost_::getInterface(uint8_t* interfaceCount) {
    return 0;
}

int FingerprintUSBHost_::getDescriptor(USBSetup& setup) {
#ifdef DEBUG
    usbSetups[usbSetupCount++] = setup;
#endif

    if (setup.bmRequestType != REQUEST_DEVICETOHOST  || setup.bRequest != GET_DESCRIPTOR || setup.wValueH != USB_STRING_DESCRIPTOR_TYPE)  {

        return 0;
    }


    if (setup.wLength == 0xff) {
        guess.maybe_linux = 1;
        guess.maybe_win = 1;
        guess.not_mac = 1; // In testing, MacOS NEVER sets a descript request lenght of 255
    } else {
        guess.not_linux = 1; // In testing, Linux ALWAYS sets a descriptor request length of 255;
    }

    return 0;
}

GuessedHost::OSVariant FingerprintUSBHost_::guessHostOS(void) {

    if (guess.not_mac && guess.not_linux && guess.maybe_win) {
        return GuessedHost::WINDOWS;
    } else if (guess.maybe_linux && !guess.not_linux) {
        return GuessedHost::LINUX;
    } else if (! guess.not_mac)  {
        return GuessedHost::MACOS;

    } else {
        return GuessedHost::UNSURE;
    }
}

void FingerprintUSBHost_::guessHostOS(String &os) {
    switch (guessHostOS()) {
    case GuessedHost::WINDOWS:
        os="Windows";
        break;
    case GuessedHost::LINUX:
        os="Linux";
        break;
    case GuessedHost::MACOS:
        os="Mac";
        break;
    default:
        os="unsure";
        break;
    }
}


bool FingerprintUSBHost_::setup(USBSetup& setup) {
    return false;
}

FingerprintUSBHost_::FingerprintUSBHost_(void) : PluggableUSBModule(0, 0, epType) {
    memset(&guess, 0, sizeof(guess));
    PluggableUSB().plug(this);
}

int FingerprintUSBHost_::begin(void) {
    return 0;
}



#endif /* if defined(USBCON) */
