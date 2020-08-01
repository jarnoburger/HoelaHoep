extern volatile unsigned long timer0_millis; //timer0_millis is defined in wiring.c

#ifndef cbi // Definitions for setting and clearing register bits
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define SYSCLOCK 16000000  // main system clock of Arduino Board (Hz)

#define IROUT 11    // pin 11 is OC2A output from TIMER2
#define LED LED_BUILTIN

#define BURST_FREQUENCY 32600 //IR Burst Frequency in Hz

unsigned long time = 0;

unsigned long pulses[] = {500,5300,500};
unsigned int count = 3;
boolean toggle = LOW;

unsigned long microseconds();
void setup_timer2(unsigned int freq);
uint8_t timer2top(unsigned int freq) ;

//#########################################################

void setup()
{
 pinMode(LED, OUTPUT);
 pinMode(IROUT, OUTPUT) ;
 
 setup_timer2(BURST_FREQUENCY);
}

//#########################################################

void loop()
{
 TCNT0 = 0;
 time = microseconds();
 digitalWrite(LED, HIGH) ;
 sbi(TCCR2A,COM2A0) ;   // connect pulse clock
 for(int i=0;i<count;i++)
 {
   time = time + pulses[i];
   while(microseconds()<time){};
   if(toggle)
   {
     digitalWrite(LED, HIGH) ;
     sbi(TCCR2A,COM2A0) ;   // connect pulse clock
   }
   else
   {
     digitalWrite(LED, LOW) ;
     cbi(TCCR2A,COM2A0) ;   // disconnect pulse clock
   }
   toggle = !toggle;
   

 }
 toggle = LOW;
 delay(6000);
}

//#########################################################

// return TIMER2 TOP value per given desired frequency (Hz)
uint8_t timer2top(unsigned int freq)
{
 return((byte)((unsigned long)SYSCLOCK/2/freq) - 1) ;
}

void setup_timer2(unsigned int freq)
{
 cbi(TCCR2A,COM2A1); // disconnect OC2A for now (COM2A0 = 0)
 cbi(TCCR2A,COM2A0);
 
 cbi(TCCR2B,WGM22);  // CTC mode for TIMER2
 sbi(TCCR2A,WGM21);
 cbi(TCCR2A,WGM20);
 
 TCNT2 = 0;
 
 cbi(ASSR,AS2);  // use system clock for timer 2
 
 OCR2A = timer2top(freq);
 
 cbi(TCCR2B,CS22);  // TIMER2 prescale = 1
 cbi(TCCR2B,CS21);
 sbi(TCCR2B,CS20);
 
 cbi(TCCR2B,FOC2A);  // clear forced output compare bits
 cbi(TCCR2B,FOC2B);
}

unsigned long microseconds()
{
 return (timer0_millis * 1000 + 4*TCNT0);
}
