//==============================================================================
// Coder_muLaw.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "MuLawCoder.h"
#include <stdio.h>


//==============================================================================

#define LINEAR_CLIP   0x7F7B
#define LINEAR_BIAS   0x0084


//==============================================================================

static inline uint8_t get_chord ( int16_t val );
static inline uint8_t get_step  ( int16_t val, uint8_t chord );


//==============================================================================

/**
 *  \brief Encode the 16 bits signed words into 8 bits codewords with mu-law
 *
 *  Encoded word format (8 bits):
 *
 *  7             0
 *  X X X X X X X X
 *  _ _____ _______
 *  |   |      |
 *  |   |      '--- step
 *  |   '---------- chord
 *  '-------------- sign
 *
 *  Signal (sign):
 *    - 0 = negative
 *    - 1 = positive
 *
 *  Usually the coded word is inverted for transmission
 *   optimization, so this method returns the inverted codeword.
 *
 *  References:
 *    - http://www.ti.com/lit/an/spra163a/spra163a.pdf
 *    - https://github.com/torvalds/linux/blob/v3.8-rc2/sound/core/oss/mulaw.c
 *    - http://cgit.freedesktop.org/gstreamer/gst-plugins-good/tree/gst/law/mulaw-conversion.c?id=1.0.4
 */
uint8_t MuLawCoder::encode ( int16_t val )
{
	uint8_t sign = 0x00;                          // Negative sign

	if (val < 0) {                                // Extract sign and clip
		if (val < -LINEAR_CLIP)
			val = -LINEAR_CLIP;

		val = -val;
	} else {
		if (val > LINEAR_CLIP)
			val = LINEAR_CLIP;

		sign = 0x80;
	}

	val += LINEAR_BIAS;                           // Bias value

	uint8_t chord = get_chord(val);               // Get chord

	uint8_t step  = get_step(val, chord);         // Get step

	return ~( sign | (chord << 4) | step );       // Return codeword complement
}

int16_t MuLawCoder::decode ( uint8_t code )
{
	int16_t val = 0;

	code = ~code;                                 // Complement codeword

	uint8_t chord = (code & 0x70) >> 4;           // Get chord

	val = 1 << (chord + 7);                       // Add chord

	val |= (code & 0x0F) << (chord + 3);          // Add step

	if ((code & 0x80)) {                          // Apply sign and remove bias
		val = val - LINEAR_BIAS;                  //  Positive val
	} else {
		val = LINEAR_BIAS - val;                  //  Negative val
	}

	return val;                                   // Return val
}


//==============================================================================

static inline uint8_t get_chord ( int16_t val )
{
	uint8_t chord = 0;

	val >>= 7;

	if (val & 0xf0) {
		val >>= 4;
		chord += 4;
	}

	if (val & 0x0c) {
		val >>= 2;
		chord += 2;
	}

	if (val & 0x02) {
		chord += 1;
	}

	return chord;
}

static inline uint8_t get_step ( int16_t val, uint8_t chord )
{
	return ((val >> (chord + 3)) & 0x0F);
}


// EOF =========================================================================
