int r;
int g;
int b;

void setup() {
 Serial.begin(115200);
 Serial.println("Beginning");
}

void loop() {
  // put your main code here, to run repeatedly:
 colorConverter("#FF00FF");
 colorConverter("#1234FE");
}

 // put your setup code here, to run once:
void colorConverter(String hexValue)
{
  hexValue.toUpperCase();
  
  Serial.print("Converting [");
  Serial.print(hexValue);
  Serial.println("]");

  char c[7];
  hexValue.toCharArray(c, 8);
  r = hexcolorToInt(c[1], c[2]);
  g = hexcolorToInt(c[3], c[4]);
  b = hexcolorToInt(c[5], c[6]);
  
  Serial.println(r);
  Serial.println(g);
  Serial.println(b);
  delay(5000); 
}

int hexcolorToInt(char upper, char lower)
{
  int uVal = (int)upper;
  int lVal = (int)lower;
  uVal = uVal > 64 ? uVal - 55 : uVal - 48;
  uVal = uVal << 4;
  lVal = lVal > 64 ? lVal - 55 : lVal - 48;
  //  Serial.println(uVal+lVal);
  return uVal + lVal;
}
