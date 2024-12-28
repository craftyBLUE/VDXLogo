//D-PAD
int y_axis() { //UP & DOWN
  //-1 UP
  // 0 CENTER
  // 1 DOWN
  int yy = 0, val = 0;

  val = analogRead(35); //GPIO35

  if (val >= 4000) yy = -1;
  else if (val) yy = 1;

  return yy;
}

int x_axis() { //L & R
  //-1 LEFT
  // 0 CENTER
  // 1 RIGHT
  int xx = 0, val = 0;

  val = analogRead(34); //GPIO34

  if (val >= 4000) xx = -1;
  else if (val) xx = 1;

  return xx;
}

#define y_axis y_axis()
#define x_axis x_axis()
