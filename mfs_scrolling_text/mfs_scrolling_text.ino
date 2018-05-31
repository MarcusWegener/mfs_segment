/*
 *  Marcus Wegener 2018-05-31
 *  
 *  Scrolling Text on Multi Function Shield
 *  
 *  Segment-Display with timer
 */
 
const uint8_t LATCH = 4; 
const uint8_t CLK = 7;
const uint8_t DATA = 8;

/*  Segments that should light up for each character (Low-Aktiv)
 *  0xC7 = L
 *  0xC0 = O
 *  0xC1 = U
 *  0xCF = I
 *  0x92 = S
 *  0xFF = [blank]
 */
const uint8_t SEGMENT_CHAR[] = {0xC7,0xC0,0xC1,0xCF,0x92,0xFF};
const uint8_t SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};

volatile uint8_t SegmentDisplay[4] = {0xFF,0xFF,0xFF,0xFF};
volatile uint8_t SegmentCharSize = 0;
volatile uint8_t ActDisplay = 0;

ISR(TIMER1_COMPA_vect)          
{
    digitalWrite(LATCH,LOW);                                
    shiftOut(DATA, CLK, MSBFIRST, SegmentDisplay[ActDisplay]);      
    shiftOut(DATA, CLK, MSBFIRST, SEGMENT_SELECT[ActDisplay]);  
    digitalWrite(LATCH,HIGH);
    ActDisplay < 3 ? ActDisplay++ : ActDisplay = 0;   
}

void setup() {
  pinMode(LATCH,OUTPUT);
  pinMode(CLK,OUTPUT);
  pinMode(DATA,OUTPUT);

  // Timer1, f = 124,87 Hz
  TCCR1A = 0;
  OCR1A = 1000;
  TCCR1B |= (1 << CS10) | (1 << CS11) | (1 << WGM12);
  TIMSK1 |= (1 << OCIE1A);
  sei();
  
  SegmentCharSize = sizeof(SEGMENT_CHAR) / sizeof(uint8_t);
}

void loop() {
  for (uint8_t Offset = 0; Offset < SegmentCharSize; Offset++)
  {
    for (uint8_t Display = 0; Display < 4; Display++)
    {
      SegmentDisplay[Display] = SEGMENT_CHAR[Display + Offset < SegmentCharSize ? Display + Offset : (Display + Offset) - SegmentCharSize];
    }
    delay(1000);
  }
}
