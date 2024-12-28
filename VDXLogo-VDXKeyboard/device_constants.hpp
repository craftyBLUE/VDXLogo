#define RED565 0b1111100000000000
#define GREEN565 0b0000011111100000
#define BLUE565 0b0000000000011111
#define RED1 0b100
#define GREEN1 0b010
#define BLUE1 0b001

const int screenW = 320;
const int screenH = 240;

const int W = screenW;
const int H = screenH - 44;

double xCenter = W / 2;
double yCenter = H / 2;
double zCenter = 0;

double precision = 2048;
double turtleSize = 12;

int screenColor = 0b111;

//debug
bool debugEnabled = false;
bool fastTurtle = false;
