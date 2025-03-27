#include "Arduino.h"
#include <hellodrum.h>
#include <MIDI.h>

///////////////////////////////   SETTING VALUE   ///////////////////////////////////

byte SNARE[10] = {
    100, // sensitivity (1-100)
    10,  // threshold (1-100)
    30,  // scan time (1-100)
    10,  // mask time (1-100)
    10,  // rim sensitivity (1-100)
    3,   // rim threshold (1-100)
    1,   // curve type (0-4)
    38,  // note (0-127)
    40,  // note of rim shot (0-127)
    37   // note of cross stick (0-127)
};

byte HI_TOM[6] = {
    100, // sensitivity (1-100)
    10,  // threshold (1-100)
    10,  // scan time (1-)
    30,  // mask time(1-)
    50,  // note (0-127)
    1    // curve type(0-4)
};

byte MEDIUM_TOM[6] = {
    100, // sensitivity (1-100)
    10,  // threshold (1-100)
    10,  // scan time (1-)
    30,  // mask time(1-)
    45,  // note (0-127)
    1    // curve type(0-4)
};

byte FLOOR_TOM[6] = {
    100, // sensitivity (1-100)
    10,  // threshold (1-100)
    10,  // scan time (1-)
    30,  // mask time(1-)
    43,  // note (0-127)
    1    // curve type(0-4)
};

byte KICK[6] = {
    100, // sensitivity (1-100)
    10,  // threshold (1-100)
    10,  // scan time (1-)
    30,  // mask time(1-)
    36,  // note (0-127)
    1    // curve type(0-4)
};

byte RIDE[6] = {
    100, // sensitivity
    10,  // threshold
    10,  // scan time
    30,  // mask time
    51,  // note of bow
    1,   // curve type(0-4)
};

byte CRASH[6] = {
    100, // sensitivity
    10,  // threshold
    10,  // scan time
    30,  // mask time
    51,  // note of bow
    1,   // curve type(0-4)
};

byte HIHAT[10] = {
    100, // sensitivity
    10,  // threshold
    10,  // scan start
    30,  // scan end
    10,  // edge threshold
    46,  // note of open
    42,  // note of close
    26,  // note of open edge
    22,  // note of close edge
    1    // curve type(0-4)
};

byte HIHAT_PEDAL[7] = {
    90, // sensitivity
    30, // threshold
    60, // scan start
    80, // scan end
    10, // pedal sensitivity
    44, // note of pedal
    1   // curve type(0-4)
};

/////////////////////////////////////////////////////////////////////////////////////

MIDI_CREATE_DEFAULT_INSTANCE();

typedef byte Channel;
Channel MIDI_CHANNEL = 10;

// INIT ANALOG PINS
HelloDrum snare(0, 1);
HelloDrum hiTom(2);
HelloDrum mediumTom(3);
HelloDrum floorTom(4);
HelloDrum kick(5);
HelloDrum ride(6);
HelloDrum crash(7);
HelloDrum hihat(8, 9);
HelloDrum hihatPedal(10);

void setup()
{
  MIDI.begin(10);

  snare.setCurve(SNARE[5]);
  hiTom.setCurve(HI_TOM[5]);
  mediumTom.setCurve(MEDIUM_TOM[5]);
  floorTom.setCurve(FLOOR_TOM[5]);
  kick.setCurve(KICK[5]);
  ride.setCurve(RIDE[5]);
  crash.setCurve(CRASH[6]);
  hihat.setCurve(HIHAT[9]);
  hihatPedal.setCurve(HIHAT_PEDAL[6]);
}

void loop()
{
  // Piezo sensing is done in this line. And it is returned as a velocity of 127 stages.
  // For each piezo, one line is required.
  snare.dualPiezo(SNARE[0], SNARE[1], SNARE[2], SNARE[3], SNARE[4], SNARE[5]);
  hiTom.singlePiezo(HI_TOM[0], HI_TOM[1], HI_TOM[2], HI_TOM[3]);
  mediumTom.singlePiezo(MEDIUM_TOM[0], MEDIUM_TOM[1], MEDIUM_TOM[2], MEDIUM_TOM[3]);
  floorTom.singlePiezo(FLOOR_TOM[0], FLOOR_TOM[1], FLOOR_TOM[2], FLOOR_TOM[3]);
  kick.singlePiezo(KICK[0], KICK[1], KICK[2], KICK[3]);
  ride.singlePiezo(RIDE[0], RIDE[1], RIDE[2], RIDE[3]);
  crash.singlePiezo(CRASH[0], CRASH[1], CRASH[2], CRASH[3]);
  hihat.HH2zone(HIHAT[0], HIHAT[1], HIHAT[2], HIHAT[3], HIHAT[4]);
  hihatPedal.hihatControl(HIHAT_PEDAL[0], HIHAT_PEDAL[1], HIHAT_PEDAL[2], HIHAT_PEDAL[3], HIHAT_PEDAL[4]);

  if (snare.hit == true)
  {
    MIDI.sendNoteOn(SNARE[7], snare.velocity, MIDI_CHANNEL); //(note, velocity, channel)
    MIDI.sendNoteOff(SNARE[7], 0, MIDI_CHANNEL);
  }
  else if (snare.hitRim == true)
  {
    if (snare.velocity > 60)
    {
      MIDI.sendNoteOn(SNARE[8], snare.velocity, MIDI_CHANNEL); //(note, velocity, channel)
      MIDI.sendNoteOff(SNARE[8], 0, MIDI_CHANNEL);
    }
    else
    {
      MIDI.sendNoteOn(SNARE[9], snare.velocity * 2, MIDI_CHANNEL); //(note, velocity, channel)
      MIDI.sendNoteOff(SNARE[9], 0, MIDI_CHANNEL);
    }
  }

  if (hihat.hit == true)
  {
    // check open or close
    // 1.open
    if (hihatPedal.openHH == true)
    {
      MIDI.sendNoteOn(HIHAT[5], hihat.velocity, 10); //(note, velocity, channel)
      MIDI.sendNoteOff(HIHAT[5], 0, 10);
    }
    // 2.close
    else if (hihatPedal.closeHH == true)
    {
      MIDI.sendNoteOn(HIHAT[6], hihat.velocity, 10); //(note, velocity, channel)
      MIDI.sendNoteOff(HIHAT[6], 0, 10);
    }
  }
  else if (hihat.hitRim == true)
  {
    // check open or close
    // 1.open
    if (hihatPedal.openHH == true)
    {
      MIDI.sendNoteOn(HIHAT[7], hihat.velocity, 10); //(note, velocity, channel)
      MIDI.sendNoteOff(HIHAT[7], 0, 10);
    }
    // 2.close
    else if (hihatPedal.closeHH == true)
    {
      MIDI.sendNoteOn(HIHAT[8], hihat.velocity, 10); //(note, velocity, channel)
      MIDI.sendNoteOff(HIHAT[8], 0, 10);
    }
  }

  // when pedal is closed
  if (hihatPedal.hit == true)
  {
    MIDI.sendNoteOn(HIHAT_PEDAL[5], hihatPedal.velocity, 10); //(note, velocity, channel)
    MIDI.sendNoteOff(HIHAT_PEDAL[5], 0, 10);
  }

  // sending state of pedal with controll change
  if (hihatPedal.moving == true)
  {
    MIDI.sendControlChange(4, hihatPedal.pedalCC, 10);
  }

  if (hiTom.hit == true)
  {
    MIDI.sendNoteOn(HI_TOM[4], hiTom.velocity, MIDI_CHANNEL); //(note, velocity, channel)
    MIDI.sendNoteOff(HI_TOM[4], 0, MIDI_CHANNEL);
  }

  if (mediumTom.hit == true)
  {
    MIDI.sendNoteOn(MEDIUM_TOM[4], mediumTom.velocity, MIDI_CHANNEL); //(note, velocity, channel)
    MIDI.sendNoteOff(MEDIUM_TOM[4], 0, MIDI_CHANNEL);
  }

  if (floorTom.hit == true)
  {
    MIDI.sendNoteOn(FLOOR_TOM[4], floorTom.velocity, MIDI_CHANNEL); //(note, velocity, channel)
    MIDI.sendNoteOff(FLOOR_TOM[4], 0, MIDI_CHANNEL);
  }

  if (kick.hit == true)
  {
    MIDI.sendNoteOn(KICK[4], kick.velocity, MIDI_CHANNEL); //(note, velocity, channel)
    MIDI.sendNoteOff(KICK[4], 0, MIDI_CHANNEL);
  }

  if (ride.hit == true)
  {
    MIDI.sendNoteOn(RIDE[4], ride.velocity, MIDI_CHANNEL); //(note, velocity, channel)
    MIDI.sendNoteOff(RIDE[4], 0, MIDI_CHANNEL);
  }
  if (crash.hit == true)
  {
    MIDI.sendNoteOn(CRASH[4], crash.velocity, MIDI_CHANNEL); //(note, velocity, channel)
    MIDI.sendNoteOff(CRASH[4], 0, MIDI_CHANNEL);
  }
}