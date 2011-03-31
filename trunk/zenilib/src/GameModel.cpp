#include <zenilib.h>
#include "GameModel.h"

using namespace std;
using namespace Zeni;

GameModel & getGameModel() {
  static GameModel model;
  return model;
}

GameModel::GameModel() {
}