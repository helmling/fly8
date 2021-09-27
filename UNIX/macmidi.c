/* --------------------------------- grsdlgl.c ------------------------------ */

/* This is part of the flight simulator 'fly8'.
 * Author: Matthias Helmling
*/

#include <AudioUnit/AudioUnit.h>

#include "fly.h"
#include "plsound.h"
#include "notes.h"

#define INITED		0x8000

#define FourCC2Str(fourcc) (const char[]){*(((char*)&fourcc)+3), *(((char*)&fourcc)+2), *(((char*)&fourcc)+1), *(((char*)&fourcc)+0),0}

static int	midi_program = 80;
static int	midi_channel = 0;
static int	midi_note = -1;
static int	midi_volume = 60;
static int	notes[] = {
	4*BN_/2,
	4*CN_, 4*CSH_, 4*DN_, 4*DSH_, 4*EN_, 4*FN_,
	4*FSH_, 4*GN_, 4*GSH_, 4*AN_, 4*ASH_, 4*BN_
};

static AudioUnit musicDevice;
static AudioUnit ouputDevice;

static void
MidiTerm (void) {
	if (Snd->flags & INITED) {
		PlsTerm();
		Snd->flags &= ~INITED;
		LogPrintf ("%s: term ok\n", Snd->name);
	}
	AudioOutputUnitStop(ouputDevice);
}

static int
MidiInit (char *options) {
	int err;
	Ulong l;
	AudioUnitConnection auconnect;
	AudioComponentDescription ouput;
	AudioComponentDescription music;
	AudioComponentDescription found;
	AudioComponent musicId;
	AudioComponent ouputId;

	Snd->flags &= ~INITED;

	if (get_narg (options, "channel=", &l))
		midi_channel = 0;
	else if (l > 127) {
		LogPrintf ("%s: channel too large\n",
			Snd->name);
		midi_channel = 0;
	} else
		midi_channel = (int)l;

	if (get_narg (options, "program=", &l))
		midi_program = 80;
	else if (l > 127) {
		LogPrintf ("%s: program too large\n",
			Snd->name);
		midi_program = 80;
	} else
		midi_program = (int)l;

	if (get_narg (options, "volume=", &l))
		midi_volume = 100;
	else if (l > 127) {
		LogPrintf ("%s: volume too large\n",
			Snd->name);
		midi_volume = 100;
	} else
		midi_volume = (int)l;
	
	music.componentType = kAudioUnitType_MusicDevice;
	music.componentSubType = 0; // kAudioUnitSubType_HALOutput;
	music.componentManufacturer = 0; // kAudioUnitManufacturer_Apple;
	music.componentFlags = 0;
	music.componentFlagsMask = 0;

	// Open the Music Device
	LogPrintf("MAC: found %d music devices\n",AudioComponentCount(&music));
	AudioComponent test_comp = NULL;
	musicId = AudioComponentFindNext(test_comp, &music);
	if (!musicId) {
		LogPrintf("MAC: No midi device found\n");
		return -1;
	}
	CFStringRef musicCompName;
	AudioComponentCopyName(musicId, &musicCompName);
	LogPrintf("MAC: Using Music device: %s\n", CFStringGetCStringPtr( musicCompName, kCFStringEncodingASCII ));
	CFRelease(musicCompName);
	
	AudioComponentGetDescription(musicId, &found);
	LogPrintf("Found music device of type: %s\n", FourCC2Str(found.componentType));
	LogPrintf("                  sub type: %s\n", FourCC2Str(found.componentSubType));

	if (AudioComponentInstanceNew(musicId, &musicDevice)) {
		LogPrintf("Failed to to open the music device\n");
		return -1;
	}

	ouput.componentType = kAudioUnitType_Output;
	ouput.componentSubType = 0; // kAudioUnitSubType_HALOutput;
	ouput.componentManufacturer = 0; // kAudioUnitManufacturer_Apple;
	ouput.componentFlags = 0;
	ouput.componentFlagsMask = 0;

	// Open the Ouptut Device
	LogPrintf("MAC: found %d output devices\n",AudioComponentCount(&ouput));
	ouputId = AudioComponentFindNext(test_comp, &ouput);
	if (!ouputId) {
		LogPrintf("MAC: No output device found\n");
		return -1;
	}
	CFStringRef ouputCompName;
	AudioComponentCopyName(musicId, &ouputCompName);
	LogPrintf("MAC: Using output device: %s\n", CFStringGetCStringPtr( ouputCompName, kCFStringEncodingASCII ));
	CFRelease(ouputCompName);
	
	AudioComponentGetDescription(ouputId, &found);
	LogPrintf("MAC: Found output device of type: %s\n", FourCC2Str(found.componentType));
	LogPrintf("                   sub type: %s\n", FourCC2Str(found.componentSubType));

	if (AudioComponentInstanceNew(ouputId, &ouputDevice)) {
		LogPrintf("MAC: Failed to open the output device\n");
		return -1;
	}

	// connect the units
	auconnect.sourceAudioUnit = musicDevice;
	auconnect.sourceOutputNumber = 0;
	auconnect.destInputNumber = 0;
	if (AudioUnitSetProperty(ouputDevice, kAudioUnitProperty_MakeConnection, kAudioUnitScope_Input, 0,
												 (void *)&auconnect, sizeof(AudioUnitConnection))) {
		LogPrintf("MAC: Failed to connect music to output\n");
		return -1;
	} else {
		LogPrintf("MAC: Connect music to output\n");
	}

	// initialize the devices
	if ((err = AudioUnitInitialize(musicDevice))) {
		LogPrintf("MAC: Failed to initialize music device: %d\n", err);
		return -1;
	}
	if ((err = AudioUnitInitialize(ouputDevice))) {
		LogPrintf("MAC: Failed to initialize output device: %d\n", err);
		return -1;
	}

	// start the output
	if ((err = AudioOutputUnitStart(ouputDevice))) {
		LogPrintf("MAC: Failed to start output device: %d\n", err);
		return -1;
	}

	if (PlsInit (options))
		goto badret;

	Snd->flags |= INITED;
	LogPrintf ("%s: init ok\n", Snd->name);

	MusicDeviceMIDIEvent(musicDevice, 0xc0+midi_channel, midi_program, 0, 0); // change program
	return (0);

badret:
	MidiTerm ();
	return (1);
}

static void
MidiStart(int n) {
	int	i, j;

	if (n <= 0)
		n = 1;
	for (i = 10*12; n < (notes[1]+notes[0])/2; i -= 12)
		n <<= 1;
	for (j = rangeof (notes)-1; n < (notes[j]+notes[j-1])/2; --j)
		{}
	if ((midi_note = i+j) > 127)
		midi_note = 127;
	else if (midi_note < 0)
		midi_note = 0;

	MusicDeviceMIDIEvent(musicDevice, 0x90+midi_channel, midi_note, midi_volume, 0);
}

static void
MidiStop(void) {
	MusicDeviceMIDIEvent(musicDevice, 0x80+midi_channel, midi_note, 0x7f, 0);
	midi_note = -1;
}

static struct plsextra MidiExtra = {
	MidiStart,
	MidiStop,
	NULL,		/* beeps */
	0L,		/* lasttime */
	0L,		/* nexttime */
	0,		/* playing */
	0		/* nbeeps */
};

struct SndDriver NEAR MacPlMidi = {
	"MacMidi",
	0,
	&MidiExtra,
	MidiInit,
	MidiTerm,
	PlsPoll,
	PlsBeep,
	PlsEffect,
	PlsList
};

