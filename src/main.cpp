#include "engine.h"

int main (int argc, char *argv[]) {
  Engine engine;
  int err = engine.init();
  if (err) {
    return 1;
  } 
  return 0;
}