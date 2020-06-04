#include "Animation.h"

void Animation::reloadConf() {
  p_colorStorage->reload();
  p_animationTimeStorage->reload();
  p_animationTypeStorage->reload();
  p_orientationStorage->reload();
  p_statusStorage->reload();
  p_storage->setCrc();
}

void Animation::readConf() {
  if(!p_storage->compareCrcs()) {
    reloadConf();
  }

  pi_color      = p_storage->read(p_colorStorage);
  i_time        = p_storage->read(p_animationTimeStorage);
  c_type        = p_storage->read(p_animationTypeStorage);
  c_orientation = p_storage->read(p_orientationStorage);
  b_status      = p_storage->read(p_statusStorage);

  restart();
}

void Animation::writeConf() {
  p_storage->write(p_colorStorage, pi_color);
  p_storage->write(p_animationTimeStorage, i_time);
  p_storage->write(p_animationTypeStorage, c_type);
  p_storage->write(p_orientationStorage, c_orientation);
  p_storage->write(p_statusStorage, b_status);
  p_storage->commit();
}

char* Animation::getConfAsJSON() {
  String str_msg;
  str_msg+= "{";

  str_msg+= b_status ? "\"status\": true," : "\"status\": false,";

  str_msg+= "\"color\": {";
  str_msg+="\"red\": " + String(pi_color[0]) + ",";
  str_msg+="\"green\": " + String(pi_color[1]) + ",";
  str_msg+="\"blue\": " + String(pi_color[2]);
  str_msg+= "},";
  str_msg+= "\"orientation\": \"" + String(c_orientation) + "\",";

  switch(c_type) {
    case 'f':
      str_msg+= "\"type\": \"fade\",";
      break;
    case 'r':
      str_msg+= "\"type\": \"rainbow\",";
      break;
    default:
      str_msg+= "\"type\": \"toColor\",";
      break;
  }
  str_msg+= "\"time\": " + String(i_time);
  str_msg+= "}";

  char* pc_msg = new char[255];
  str_msg.toCharArray(pc_msg, 255);
  return pc_msg;
}

void Animation::setTime(int i_time) {
  Animation::i_time = i_time;
  restart();
}

void Animation::setOrientation(char c_orientation) {
  Animation::c_orientation = c_orientation;
  restart();
}

void Animation::setType(char c_type) {
  Animation::c_type = c_type;
  restart();
}

void Animation::setColor(int* pi_color) {
  Animation::pi_color = pi_color;
  restart();
}

void Animation::setStatus(boolean b_status) {
  Animation::b_status = b_status;
  restart();
}

boolean Animation::getStatus() {
  return b_status;
}

void Animation::animate() {
  //prüfen, ob Zustand Strip aktiv oder standby is
  if(getStatus()) {
    //Zustand ist aktiv

    if(b_isChange) {
      idle();
      b_isChange = false;

      switch(c_type) {
        case 'f':
          p_strip->fade(pi_color, c_orientation, i_time);
          break;
        case 'r':
          if(c_orientation != 'r' && c_orientation != 'l') {
            c_orientation = 'l';
          }
          p_strip->rainbow(c_orientation, i_time);
          break;
        default:
          p_strip->toColor(pi_color);
          break;
      }
    }
  } else {
    //Zustand ist im Leerlauf
    if(b_isChange) {
      idle();
      b_isChange = false;
    }
  }
}

void Animation::restart() {
  b_isChange = true;
}

void Animation::idle() {
  p_strip->toColor(p_color->black());
}
