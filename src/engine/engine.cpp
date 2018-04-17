#include <iostream>
#include "./engine.h"


Engine::Engine() {
  std::cout << "new Engine" << std::endl;
}

Engine::~Engine() {
  std::cout << "destroy Engine" << std::endl;
}

int Engine::init() {
  std::cout << "engine init" << std::endl;
  return 0;
}

void Engine::render() {
  std::cout << "engind render" << std::endl;
}