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

#pragma once

#include <stdint.h>
#include <Arduino.h>
#include "PluggableUSB.h"

namespace GuessedHost {
typedef enum {
    UNSURE,
    LINUX,
    WINDOWS,
    MACOS,
} OSVariant;
};

class FingerprintUSBHost_ : public PluggableUSBModule {
  public:
    FingerprintUSBHost_(void);
    int begin(void);
    GuessedHost::OSVariant guessHostOS(void);
    void guessHostOS(String &os);
#ifdef DEBUG
    USBSetup usbSetups[32];
    int usbSetupCount = 0;
#endif

  protected:
    // Implementation of the PluggableUSBModule
    int getInterface(uint8_t* interfaceCount);
    int getDescriptor(USBSetup& setup);
    bool setup(USBSetup& setup);


  private:
    uint8_t epType[0];
    struct {
        uint8_t maybe_linux:1;
        uint8_t maybe_win:1;
        uint8_t maybe_mac:1;
        uint8_t not_linux:1;
        uint8_t not_win:1;
        uint8_t not_mac:1;
    } guess;


};

/*
 typedef struct
 {
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint8_t wValueL;
    uint8_t wValueH;
    uint16_t wIndex;
    uint16_t wLength;
 } USBSetup;
*/

extern FingerprintUSBHost_ FingerprintUSBHost;
