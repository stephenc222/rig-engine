// dummy app, designed to test the engine API
#include <iostream>
#include <string>
#include "../engine/engine.h"
#include "./app.h"


App::App() {
  std::cout << "new App" << std::endl;
}

App::~App() {
  std::cout << "destroy App" << std::endl;
}

int App::init() {
  std::cout << "app init" << std::endl;
  Engine engine;

  engine.init();
  return 0;
}