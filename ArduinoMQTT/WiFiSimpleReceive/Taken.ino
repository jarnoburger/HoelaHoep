int pSpeed;      // 0-100 , hoe snel het effect gaat
int pIntensity;  // 0-100 , hoe intens het effect is , intens is effect specifiek 
int pEffect;     // 0-?   , kies het effect
int pPowerA;     // 0-1000, hoe fel is kleur a
int pPowerB;     // 0-1000, hoe fel is kleur b
int pColorARed;  // 0-255 , color A red
int pColorAGreen;// 0-255 , color A green
int pColorABlue; // 0-255 , color A blue
int pColorBRed;  // 0-255 , color B red
int pColorBGreen;// 0-255 , color B green
int pColorBBlue; // 0-255 , color B blue

// effects
// 1 : alles aan
// 2 : fade tussen 2 kleuren (daardoor ook strobe)
// 3 : roteer een streep met kleur A , de anderen zijn kleur B
// 4 : wave een streep met kleur A , de anderen zijn kleur B
// 5 : random met kleur A over kleur B

// tussen 0 en 100
void SetSpeed(int s) {
  
}

// tussen 0 en 100
void SetEffectIntensity(int i){

}

// tussen 0 en het aantal effecten minus 1
void SetEffect(int e){
  
}

// tussen 0 en 1000 , de lichtkracht van kleur A
void SetPowerA(int p){
  
}

// tussen 0 en 1000 , de lichtkracht van kleur B
void SetPowerB(int p){
  
}

// tussen 0 en 256 , kleur A
void SetColorA(int r, int g, int b){
  
}

// tussen 0 en 256 , kleur B
void SetColorB(int r, int g, int b){
  
}
