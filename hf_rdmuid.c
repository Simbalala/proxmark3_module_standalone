//-----------------------------------------------------------------------------
// Copyright (C) 2024 Mathieu Tourrette
// Copyright (C) 2020 Anze Jensterle <dev@anze.dev>
// Copyright (C) Proxmark3 contributors. See AUTHORS.md for details.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// See LICENSE.txt for the text of the license.
//-----------------------------------------------------------------------------
// main code for hf_rdmuid
// `hf_rdmuid` generate uid 4 bytes for ISO14443a card UID and then emulates it.
//-----------------------------------------------------------------------------

#include "standalone.h"
#include "proxmark3_arm.h"
#include "appmain.h"
#include "fpgaloader.h"
#include "util.h"
#include "dbprint.h"
#include "ticks.h"
#include "string.h"
#include "BigBuf.h"
#include "iso14443a.h"
#include "protocols.h"
#include "cmd.h"

#define STATE_GENE 0
#define STATE_EMUL 1

static unsigned long next = 1;

static void srand(unsigned int seed) {
    next = seed;
}

static int rand(void) {
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % 32768;
}

void ModInfo(void) {
    DbpString("HF RDMUID mode - Generate random UID and emulates ISO14443a UID (rdmuid)");
}

void RunMod(void) {
    StandAloneMode();
    Dbprintf(_YELLOW_("HF RDMUID mode started"));
    FpgaDownloadAndGo(FPGA_BITSTREAM_HF);

    srand(GetTickCount());

    // the main loop for your standalone mode
    for (;;) {
        WDT_HIT();

        // exit from RunMod,   send a usbcommand.
        if (data_available()) break;

        iso14a_card_select_t card;

        SpinDelay(500);

        // 0 = generate, 1 = emul
        int state = STATE_GENE;

        DbpString("Generate...");
        int button_pressed = BUTTON_NO_CLICK;
        for (;;) {
            // Was our button held down or pressed?
            button_pressed = BUTTON_HELD(100);

            if (button_pressed != BUTTON_NO_CLICK || data_available())
                break;
            else if (state == STATE_GENE) {
                card.uidlen = 4;
                for (int i = 0; i < card.uidlen; i++) {
                    card.uid[i] = rand() % 256;
                }
                card.sak = 0x08;
                card.atqa[0] = 0x04;
                card.atqa[1] = 0x00;
                Dbprintf("Generated random UID with SAK: %02X, ATQA: %02X %02X, UID: ", card.sak, card.atqa[0], card.atqa[1]);
                Dbhexdump(card.uidlen, card.uid, 0);
                state = STATE_EMUL;
            } else if (state == STATE_EMUL) {
                uint16_t flags = 0;
                if (card.uidlen == 4) {
                    flags |= FLAG_4B_UID_IN_DATA;
                } else {
                    Dbprintf("Unusual UID length, something is wrong. Try again please.");
                    state = STATE_GENE;
                    continue;
                }

                Dbprintf("Starting simulation, press " _GREEN_("pm3 button") " to stop and go back to search state.");
                if (card.sak == 0x08 && card.atqa[0] == 0x04 && card.atqa[1] == 0) {
                    DbpString("Mifare Classic 1k");
                    SimulateIso14443aTag(1, flags, card.uid, 0);
                } else {
                    Dbprintf("Unrecognized tag type -- defaulting to Mifare Classic emulation");
                    SimulateIso14443aTag(1, flags, card.uid, 0);
                }

                // Go back to search state if user presses pm3-button
                state = STATE_GENE;
            }
        }
        if (button_pressed  == BUTTON_HOLD)        //Holding down the button
            break;
    }

    Dbprintf("-=[ exit ]=-");
    LEDsoff();
}
