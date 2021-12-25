#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF
#pragma config CCP2MX = PORTBE

#define debug 1
//LEDS
#define _XTAL_FREQ      8000000         // Set operation for 8 Mhz
#define TMR_CLOCK       _XTAL_FREQ/4    // Timer Clock 2 Mhz

#define TFT_DC          PORTDbits.RD3            		// Location of TFT D/C
#define TFT_CS          PORTDbits.RD4                   // Location of TFT Chip Select TYPO ON SCHEMATIC
#define TFT_RST         PORTDbits.RD2                   // Location of TFT Reset
#define SEC_LED         PORTDbits.RD7                  

#define NS_RED          PORTBbits.RB4
#define NS_GREEN        PORTBbits.RB5

#define NSLT_RED        PORTCbits.RC0
#define NSLT_GREEN      PORTCbits.RC1

#define EW_RED          PORTDbits.RD0
#define EW_GREEN        PORTDbits.RD1

#define EWLT_RED        PORTEbits.RE1
#define EWLT_GREEN      PORTEbits.RE2

#define NS_LT_SW        PORTAbits.RA4

#define EW_LT_SW        PORTAbits.RA5

#define MODE_LED        PORTBbits.RB7      
//colors
#define OFF             0               // Defines OFF as decimal value 0
#define RED             1               // Defines RED as decimal value 1
#define GREEN           2               // Defines GREEN as decimal value 2
#define YELLOW          3               // Defines YELLOW as decimal value 3

#define NS          	0
#define NSLT 		    1
#define EW          	2
#define EWLT 		    3

#define Circle_Size     7               // Size of Circle for Light
#define Circle_Offset   15              // Location of Circle
#define TS_1            1               // Size of Normal Text
#define TS_2            2               // Size of PED Text
#define Count_Offset    10              // Location of Count

#define XTXT            30              // X location of Title Text 
#define XRED            40              // X location of Red Circle
#define XYEL            60              // X location of Yellow Circle
#define XGRN            80              // X location of Green Circle
#define XCNT            100             // X location of Sec Count

#define NS              0               // Number definition of North/South
#define NSLT            1               // Number definition of North/South Left Turn
#define EW              2               // Number definition of East/West
#define EWLT            3               // Number definition of East/West Left Turn
 
#define Color_Off       0               // Number definition of Off Color
#define Color_Red       1               // Number definition of Red Color
#define Color_Green     2               // Number definition of Green Color
#define Color_Yellow    3               // Number definition of Yellow Color

#define NS_Txt_Y        20
#define NS_Cir_Y        NS_Txt_Y + Circle_Offset
#define NS_Count_Y      NS_Txt_Y + Count_Offset
#define NS_Color        ST7735_BLUE 

#define NSLT_Txt_Y      50
#define NSLT_Cir_Y      NSLT_Txt_Y + Circle_Offset
#define NSLT_Count_Y    NSLT_Txt_Y + Count_Offset
#define NSLT_Color      ST7735_MAGENTA

#define EW_Txt_Y        80
#define EW_Cir_Y        EW_Txt_Y + Circle_Offset
#define EW_Count_Y      EW_Txt_Y + Count_Offset
#define EW_Color        ST7735_CYAN

#define EWLT_Txt_Y      110
#define EWLT_Cir_Y      EWLT_Txt_Y + Circle_Offset
#define EWLT_Count_Y    EWLT_Txt_Y + Count_Offset
#define EWLT_Color      ST7735_WHITE

#define PED_NS_Count_Y  30
#define PED_EW_Count_Y  90
#define PED_Count_X     2
#define Switch_Txt_Y    140

#include "ST7735_TFT.c"

char buffer[31];                        // general buffer for display purpose
char *nbr;                              // general pointer used for buffer
char *txt;

char EW_Count[]     = "00";             // text storage for EW Count
char EWLT_Count[]   = "00";             // text storage for EW Left Turn Count
char NS_Count[]     = "00";             // text storage for NS Count
char NSLT_Count[]   = "00";             // text storage for NS Left Turn Count

char PED_EW_Count[] = "00";             // text storage for EW Pedestrian Count
char PED_NS_Count[] = "00";             // text storage for NS Pedestrian Count

char SW_EWPED_Txt[] = "0";              // text storage for EW Pedestrian Switch
char SW_EWLT_Txt[]  = "0";              // text storage for EW Left Turn Switch
char SW_NSPED_Txt[] = "0";              // text storage for NS Pedestrian Switch
char SW_NSLT_Txt[]  = "0";              // text storage for NS Left Turn Switch

char SW_MODE_Txt[]  = "D";              // text storage for Mode Light Sensor
	
char Act_Mode_Txt[]  = "D";             // text storage for Actual Mode
char FlashingS_Txt[] = "0";             // text storage for Emergency Status
char FlashingR_Txt[] = "0";             // text storage for Flashing Request

int INT0_flag, INT1_flag, INT2_flag = 0;
char dir;
char Count;                             // RAM variable for Second Count
char PED_Count;                         // RAM variable for Second Pedestrian Count

char SW_EWPED;                          // RAM variable for EW Pedestrian Switch
char SW_EWLT;                           // RAM variable for EW Left Turn Switch
char SW_NSPED;                          // RAM variable for NS Pedestrian Switch
char SW_NSLT;                           // RAM variable for NS Left Turn Switch
char FLASHING;
char FLASHING_REQUEST;
char SW_MODE;                           // RAM variable for Mode Light Sensor
int MODE;
char direction;
float volt;

char NS_PED_SW;
char EW_PED_SW;
//char INT2_flag;
    
unsigned int get_full_ADC(void);
void Init_ADC(void);
void init_IO(void);
void putch (char c);  
void init_UART();
void Do_Init();

void Set_NS(char color);
void Set_NS_LT(char color);
void Set_EW(char color);
void Set_EW_LT(char color);

void PED_Control( char direction, char Num_Sec);
void Day_Mode(void);
void Night_Mode(void);

void Wait_One_Second(void);
void Wait_Half_Second(void);
void Wait_N_Seconds (char seconds);
void Wait_One_Second_With_Beep(void);

void Initialize_Screen(void);
void update_LCD_color(char direction , char color );
void update_LCD_PED_Count(char direction, char count);
void update_LCD_misc(void);
void update_LCD_count(char direction, char count);

void interrupt high_priority chkisr();
void INT0_ISR();
void INT1_ISR();
void INT2_ISR();
void Do_Flashing();

void Initialize_Screen()
{
    LCD_Reset();
    TFT_GreenTab_Initialize();
    fillScreen(ST7735_BLACK);
  
    /* TOP HEADER FIELD */
    txt = buffer;
    strcpy(txt, "ECE3301L Fall 21-S3");  
    drawtext(2, 2, txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  
    /* MODE FIELD */
    strcpy(txt, "Mode:");
    drawtext(2, 10, txt, ST7735_WHITE, ST7735_BLACK, TS_1);
    drawtext(35,10, Act_Mode_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);

    /* EMERGENCY REQUEST FIELD */
    strcpy(txt, "FR:");
    drawtext(50, 10, txt, ST7735_WHITE, ST7735_BLACK, TS_1);
    drawtext(70, 10, FlashingR_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  
    /* EMERGENCY STATUS FIELD */
    strcpy(txt, "FS:");
    drawtext(80, 10, txt, ST7735_WHITE, ST7735_BLACK, TS_1);
    drawtext(100, 10, FlashingS_Txt, ST7735_WHITE, ST7735_BLACK, TS_1); 
  
    /* SECOND UPDATE FIELD */
    strcpy(txt, "*");
    drawtext(120, 10, txt, ST7735_WHITE, ST7735_BLACK, TS_1);
      
    /* EAST/WEST UPDATE FIELD */
    strcpy(txt, "EAST/WEST");
    drawtext  (XTXT, EW_Txt_Y, txt, EW_Color, ST7735_BLACK, TS_1);
    drawRect  (XTXT, EW_Cir_Y-8, 60, 18, EW_Color);
    fillCircle(XRED, EW_Cir_Y, Circle_Size, ST7735_RED);
    drawCircle(XYEL, EW_Cir_Y, Circle_Size, ST7735_YELLOW);
    drawCircle(XGRN, EW_Cir_Y, Circle_Size, ST7735_GREEN);  
    drawtext  (XCNT, EW_Count_Y, EW_Count, EW_Color, ST7735_BLACK, TS_2);

    /* EAST/WEST LEFT TURN UPDATE FIELD */
    strcpy(txt, "E/W LT");
    drawtext  (XTXT, EWLT_Txt_Y, txt, EWLT_Color, ST7735_BLACK, TS_1);
    drawRect  (XTXT, EWLT_Cir_Y-8, 60, 18, EWLT_Color);  
    fillCircle(XRED, EWLT_Cir_Y, Circle_Size, ST7735_RED);
    drawCircle(XYEL, EWLT_Cir_Y, Circle_Size, ST7735_YELLOW);
    drawCircle(XGRN, EWLT_Cir_Y, Circle_Size, ST7735_GREEN);   
    drawtext  (XCNT, EWLT_Count_Y, EWLT_Count, EWLT_Color, ST7735_BLACK, TS_2);

    /* NORTH/SOUTH UPDATE FIELD */
    strcpy(txt, "NORTH/SOUTH");
    drawtext  (XTXT, NS_Txt_Y  , txt, NS_Color, ST7735_BLACK, TS_1);
    drawRect  (XTXT, NS_Cir_Y-8, 60, 18, NS_Color);
    drawCircle(XRED, NS_Cir_Y  , Circle_Size, ST7735_RED);
    drawCircle(XYEL, NS_Cir_Y  , Circle_Size, ST7735_YELLOW);
    fillCircle(XGRN, NS_Cir_Y  , Circle_Size, ST7735_GREEN);
    drawtext  (XCNT, NS_Count_Y, NS_Count, NS_Color, ST7735_BLACK, TS_2);
    
    /* NORTH/SOUTH LEFT TURN UPDATE FIELD */
    strcpy(txt, "N/S LT");
    drawtext  (XTXT, NSLT_Txt_Y, txt, NSLT_Color, ST7735_BLACK, TS_1);
    drawRect  (XTXT, NSLT_Cir_Y-8, 60, 18, NSLT_Color);
    fillCircle(XRED, NSLT_Cir_Y, Circle_Size, ST7735_RED);
    drawCircle(XYEL, NSLT_Cir_Y, Circle_Size, ST7735_YELLOW);
    drawCircle(XGRN, NSLT_Cir_Y, Circle_Size, ST7735_GREEN);  
    drawtext  (XCNT, NSLT_Count_Y, NSLT_Count, NSLT_Color, ST7735_BLACK, TS_2);
  

    /* EAST/WEST PEDESTRIAM UPDATE FIELD */  
    drawtext(2, PED_EW_Count_Y, PED_EW_Count, EW_Color, ST7735_BLACK, TS_2);
    strcpy(txt, "PEW");  
    drawtext(3, EW_Txt_Y, txt, EW_Color, ST7735_BLACK, TS_1);

    /* NORTH/SOUTH PEDESTRIAM UPDATE FIELD */
    strcpy(txt, "PNS");  
    drawtext(3, NS_Txt_Y, txt, NS_Color, ST7735_BLACK, TS_1);
    drawtext(2, PED_NS_Count_Y, PED_NS_Count, NS_Color, ST7735_BLACK, TS_2);
    
    /* MISCELLANEOUS UPDATE FIELD */  
    strcpy(txt, "EWP EWLT NSP NSLT MD");
    drawtext(1,  Switch_Txt_Y, txt, ST7735_WHITE, ST7735_BLACK, TS_1);
    drawtext(6,  Switch_Txt_Y+9, SW_EWPED_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
    drawtext(32, Switch_Txt_Y+9, SW_EWLT_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
    drawtext(58, Switch_Txt_Y+9, SW_NSPED_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
    drawtext(87, Switch_Txt_Y+9, SW_NSLT_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  
    drawtext(112,Switch_Txt_Y+9, SW_MODE_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
}

void Do_Init() // Initialize the ports
{
    init_UART(); // Initialize the UART
    Init_ADC(); // Initialize the ADC with the
     //programming of ADCON1
    OSCCON= 0x70; // Set clock to 8 MHz
    TRISB = 0x07; 
    TRISA = 0x1F;                               //
    TRISC = 0x00;                               // 
    TRISD = 0x00;                               // 
    TRISE = 0x00;   
     //that all the INTx pins are
     //inputs
    INTCONbits.INT0IF = 0 ; // Clear INT0IF
    INTCON3bits.INT1IF = 0; // Clear INT1IF
    INTCON3bits.INT2IF =0; // Clear INT2IF
    INTCON2bits.INTEDG0=0 ; // INT0 EDGE falling
    INTCON2bits.INTEDG1=0; // INT1 EDGE falling
    INTCON2bits.INTEDG2=1; // INT2 EDGE rising
    INTCONbits.INT0IE =1; // Set INT0 IE
    INTCON3bits.INT1IE=1; // Set INT1 IE
    INTCON3bits.INT2IE=1; // Set INT2 IE
    INTCONbits.GIE=1; // Set the Global Interrupt Enable
}

void init_UART()
{
    OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF &
    USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX &
    USART_BRGH_HIGH, 25);
    OSCCON = 0x60;
}

void putch (char c)
{
    while (!TRMT);
    TXREG = c;
}

void main()
{
//    Do_Init();
    init_IO();
    Init_ADC();
    init_UART();
    OSCCON = 0x70; 
           
                                                                                                    // set the system clock to be 1MHz 1/4 of the 4MHz
    Initialize_Screen();                                                                            // Initialize the TFT screen

    INTCONbits.INT0IF = 0;  // Clear INT0IF
    INTCON3bits.INT1IF = 0;  // Clear INT1IF
    INTCON3bits.INT2IF = 0;  // Clear INT2IF
    
    INTCON2bits.INTEDG0 = 0;  // INT0 EDGE falling
    INTCON2bits.INTEDG1 = 0;  // INT1 EDGE falling
    INTCON2bits.INTEDG2 = 1;  // INT2 EDGE rising
    
    INTCONbits.INT0IE = 1;  // Clear INT0 IE
    INTCON3bits.INT1IE = 1;  // Clear INT1 IE
    INTCON3bits.INT2IE = 1;  // Clear INT2 IE
    
    INTCONbits.GIE = 1;  // Set the Global Interrupt Enable
    

    int nStep = get_full_ADC();                                                                     // calculates the # of steps for analog conversion
    volt = nStep * 5 /1024.0;                                                                       // gets the voltage in Volts, using 5V as reference s instead of 4, also divide by 1024 
    SW_MODE = volt < 2.5 ? 1:0;                                                                     // Mode = 1, Day_mode, Mode = 0 Night_mode

    while (1)
    {                                                       // Do nothing,

        if (INT0_flag == 1)                                 // if software INT0 flag is set
        {
            INT0_flag = 0;
            SW_NSPED=0;// clear that software flag
            printf("INT0 interrupt pin detected %d\r\n", SW_NSPED);
                                                            // print a message that INT0 has occurred
        }
                                                            // take the code for INT0, duplicate it and modify it to handle the other two INT1 and INT2 interrupts.
        if (INT1_flag == 1)                                 // if software INT0 flag is set
        {
            INT1_flag = 0;
            SW_EWPED=0;// clear that software flag
            printf("INT1 interrupt pin detected %d\r\n", SW_EWPED);
                                                            // print a message that INT0 has occurred
        }
        if (INT2_flag == 1)                                 // if software INT0 flag is set
        {
            INT2_flag = 0;                                  // clear that software flag
            printf("INT2 interrupt pin detected %d\r\n", 7);
                                                            // print a message that INT2 has occurred
        }
    
       if(SW_MODE)                                                                                 // Compare voltages from photo resistor circuit to switch to proper mode
        {                                                                                           // Light on signifies we are in Day Mode
            Day_Mode();
        }
        else
        {                                                                                           // Light off signifies Night Mode
            Night_Mode();
        }
        if (FLASHING_REQUEST == 1)
        {
            FLASHING_REQUEST = 0;
            Do_Flashing();
        }
    }
}



void interrupt high_priority chkisr()
{
    if (INTCONbits.INT0IF == 1) INT0_ISR();                 // check if INT0 has occurred
    if (INTCON3bits.INT1IF == 1) INT1_ISR();
    if (INTCON3bits.INT2IF == 1) INT2_ISR();
}

void INT0_ISR()
{
    INTCONbits.INT0IF=0;                                    // Clear the interrupt flag
    NS_PED_SW = 1;
    //INT0_flag = 1;// set software INT0_flag
}

void INT1_ISR()
{
    INTCON3bits.INT1IF=0;                                   // Clear the interrupt flag
    EW_PED_SW = 1;
    //INT1_flag = 1;// set software INT1_flag
}

void INT2_ISR()
{
    INTCON3bits.INT2IF=0;                                   // Clear the interrupt flag
    FLASHING_REQUEST = 1;
    INT2_flag = 1;// set software INT2_flag
} 

void Do_Flashing()
{
    FLASHING = 1;
    while (FLASHING == 1)
    {
        if (FLASHING_REQUEST == 1)
        {
            FLASHING_REQUEST = 0;
            FLASHING = 0;
        }
        else
        {
            Set_NS(RED);
            Set_NS_LT(RED);
            Set_EW(RED);
            Set_EW_LT(RED);
            Wait_One_Second();
    
            Set_NS(OFF);
            Set_NS_LT(OFF);
            Set_EW(OFF);
            Set_EW_LT(OFF);
            Wait_One_Second();
        }
    }
}

void init_IO()
{
    TRISA = 0x1F;                               //
    TRISB = 0x07;                               // 
    TRISC = 0x00;                               // 
    TRISD = 0x00;                               // 
    TRISE = 0x00;                               // 
}

void Init_ADC()
{
    ADCON0 = 0x01;
    ADCON1= 0x0E; 
    ADCON2= 0xA9;
}

unsigned int get_full_ADC()
{
   unsigned int result;
   ADCON0bits.GO=1;                     // Start Conversion
   while(ADCON0bits.DONE==1);           // wait for conversion to be completed
   result = (ADRESH * 0x100) + ADRESL;  // combine result of upper byte and
                                        // lower byte into result
   return result;                       // return the result.
}

void Set_NS(char color)
{
    direction = NS;			// Add these two lines here
    update_LCD_color(direction, color);
    
    switch (color)
    {
        case OFF: NS_RED =0; NS_GREEN=0;break;           // Turns off the NS LED
        case RED: NS_RED =1; NS_GREEN=0;break;           // Sets NS LED RED
        case GREEN: NS_RED =0; NS_GREEN=1;break;         // sets NS LED GREEN
        case YELLOW: NS_RED =1; NS_GREEN=1;break;        // sets NS LED YELLOW
    }
}

void Set_NS_LT(char color)
{
    direction = NSLT;			// Add these two lines here 
    update_LCD_color(direction, color);
    
    switch (color)
    {
        case OFF: NSLT_RED =0; NSLT_GREEN=0;break;           // Turns off the NSLT LED
        case RED: NSLT_RED =1; NSLT_GREEN=0;break;           // Sets NSLT LED RED
        case GREEN: NSLT_RED =0; NSLT_GREEN=1;break;         // sets NSLT LED GREEN
        case YELLOW: NSLT_RED =1; NSLT_GREEN=1;break;        // sets NSLT LED YELLOW
    }
}

void Set_EW(char color)
{
    direction = EW;			// Add these two lines here
    update_LCD_color(direction, color);
    
    switch (color)
    {
        case OFF: EW_RED =0;EW_GREEN=0;break;           // Turns off the EW LED
        case RED: EW_RED =1;EW_GREEN=0;break;           // Sets EW LED RED
        case GREEN: EW_RED =0;EW_GREEN=1;break;         // sets EW LED GREEN
        case YELLOW: EW_RED =1;EW_GREEN=1;break;        // sets EW LED YELLOW
    }
}

void Set_EW_LT(char color)
{
    direction = EWLT;			// Add these two lines here
    update_LCD_color(direction, color);
    
    switch (color)
    {
        case OFF: EWLT_RED =0;EWLT_GREEN=0;break;           // Turns off the EWLT LED
        case RED: EWLT_RED =1;EWLT_GREEN=0;break;           // Sets EWLT LED RED
        case GREEN: EWLT_RED =0;EWLT_GREEN=1;break;         // sets EWLT LED GREEN
        case YELLOW: EWLT_RED =1;EWLT_GREEN=1;break;          // sets EWLT LED YELLOW
    } 
}

void Activate_Buzzer()
{
    PR2 = 0b11111001;
    T2CON = 0b00000101;
    CCPR2L = 0b01001010;
    CCP2CON = 0b00111100; 
}

void Deactivate_Buzzer()
{
    CCP2CON = 0x0;
    PORTBbits.RB3 = 0;
}

void PED_Control(char direction, char Num_Sec)                                                      // Pedestrian control function
{
    for(char i = Num_Sec-1;i>0; i--)
    {
        update_LCD_misc();
        update_LCD_PED_Count(direction, i);
        Wait_One_Second_With_Beep();                                                                // hold the number on 7-Segment for 1 second
    }
    update_LCD_misc();
    update_LCD_PED_Count(direction, 0);
    Wait_One_Second_With_Beep();                                                                    // leaves the 7-Segment off for 1 second
    if (direction == 1)
    {
        NS_PED_SW = 0;
    }
    if (direction == 0)
    {
        EW_PED_SW = 0;
    }
}

void Day_Mode()
{
    MODE = 1;                                   // turns on the MODE_LED
    MODE_LED = 1;
    Act_Mode_Txt[0] = 'D';
    
    Set_NS(GREEN);
    
    Set_EW(RED);
    Set_NS_LT(RED);
    Set_EW_LT(RED);
    
    if(NS_PED_SW == 1)
    {
        PED_Control(0,8);
        
    }
    

    
        Wait_N_Seconds(7);
        
        Set_NS(YELLOW);
        Wait_N_Seconds(3);
        
        Set_NS(RED);
        
 
        
        if(EW_LT_SW == 1)
        {
            Set_EW_LT(GREEN);
            Wait_N_Seconds(8);
            
            Set_EW_LT(YELLOW);
            Wait_N_Seconds(3);
            
            Set_EW_LT(RED);
            
        }
        
        Set_EW(GREEN);
        
        if( EW_PED_SW == 1 ) 
        {
            PED_Control(1,9);
        }
        
  
        
        Wait_N_Seconds(9);
        
        Set_EW(YELLOW);
        Wait_N_Seconds(3);
        
        Set_EW(RED);
        
   
        
        if(NS_LT_SW == 1) 
        {
            Set_NS_LT(GREEN);
            Wait_N_Seconds(8);
            
            Set_NS_LT(YELLOW);
            Wait_N_Seconds(3);
            
            Set_NS_LT(RED);
            
        }
}

void Night_Mode()
{ 
    MODE = 0;                                                                        // turns on the MODE_LED
    MODE_LED = 0;
    Act_Mode_Txt[0] = 'N';
    
    MODE = 0;                                   // turns on the MODE_LED
    MODE_LED = 0;
    Act_Mode_Txt[0] = 'N';
    
    Set_NS(GREEN);
    
    Set_EW(RED);
    Set_NS_LT(RED);
    Set_EW_LT(RED);
    
    Wait_N_Seconds(6);
    
    Set_NS(YELLOW); 
    Wait_N_Seconds(3);
    
    Set_NS(RED);
    
    
    if (EW_LT_SW == 1) 
    {
        Set_EW_LT(GREEN);
        Wait_N_Seconds(6);
        
        Set_EW_LT(YELLOW);
        Wait_N_Seconds(3);
        
        Set_EW_LT(RED);
        
    }
    
    Set_EW(GREEN);
    Wait_N_Seconds(6);
    
    Set_EW(YELLOW);
    Wait_N_Seconds(3);
    
    Set_EW(RED);
    
   
    
    if ( NS_LT_SW == 1 ) 
    {
        Set_NS_LT(GREEN);
        Wait_N_Seconds(8);
        
        Set_NS_LT(YELLOW);
        Wait_N_Seconds(3);
        
        Set_NS_LT(RED);

    }


}

void Old_Wait_One_Second()
{
    SEC_LED = 0;                                                                                    // First, turn off the SEC LED
    Wait_Half_Second();                                                                             // Wait for half second, 500msec
    SEC_LED = 1;                                                                                    // turn on SEC LED
    Wait_Half_Second();                                                                             // Wait for half second
}

void Wait_One_Second()                                                                              //creates one second delay and blinking asterisk
{
    SEC_LED = 1;
    strcpy(txt,"*");
    drawtext(120,10,txt,ST7735_WHITE,ST7735_BLACK,TS_1);
    Wait_Half_Second();                                                                             // Wait for half second (or 500 msec)

    SEC_LED = 0;
    strcpy(txt," ");
    drawtext(120,10,txt,ST7735_WHITE,ST7735_BLACK,TS_1);
    Wait_Half_Second();                                                                             // Wait for half second (or 500 msec)
    update_LCD_misc();                                                                              // Keeps LCD refreshed
}
void Wait_One_Second_With_Beep()                                                                    // Waits one second while calling activate buzzer protocol for PED
{
    SEC_LED = 1;
    strcpy(txt,"*");
    drawtext(120,10,txt,ST7735_WHITE,ST7735_BLACK,TS_1);
    Activate_Buzzer();
    Wait_Half_Second();                                                                             // Wait for half second (or 500 msec)

    SEC_LED = 0;
    strcpy(txt," ");
    drawtext(120,10,txt,ST7735_WHITE,ST7735_BLACK,TS_1);
    Deactivate_Buzzer();
    Wait_Half_Second();                                                                             // Wait for half second (or 500 msec)
    update_LCD_misc();                                                                              // Refreshes our LCD
}

void Wait_Half_Second()
{
    T0CON = 0x03;                                                                                   // Timer 0, 16-bit mode, prescaler 1:16
    TMR0L = 0xDB;                                                                                   // set the lower byte of TMR
    TMR0H = 0x0B;                                                                                   // set the upper byte of TMR
    INTCONbits.TMR0IF = 0;                                                                          // clear the Timer 0 flag
    T0CONbits.TMR0ON = 1;                                                                           // Turn on the Timer 0
        
    while(INTCONbits.TMR0IF == 0);                                                                  // wait for the Timer Flag to be 1 for done
    T0CONbits.TMR0ON = 0;                                                                           // turn off the Timer 0
}   
void Wait_N_Seconds(char seconds)                                                                    // Waits for N seconds
{
    for(char I = seconds; I > 0; I--)
    {
        update_LCD_misc();
        update_LCD_count(direction, I);
        Wait_One_Second();                                                                          // Accomplishes task by calling wait one second an N number of times
    }
    update_LCD_misc();
    update_LCD_count(direction, 0);
    Wait_One_Second();
}

void update_LCD_color(char direction, char color)
{
    char Circle_Y;
    Circle_Y = EW_Cir_Y + direction * 30;    
    
    if (color == Color_Off)     //if Color off make all circles black but leave outline
    {
        fillCircle(XRED, Circle_Y, Circle_Size, ST7735_BLACK);
        fillCircle(XYEL, Circle_Y, Circle_Size, ST7735_BLACK);
        fillCircle(XGRN, Circle_Y, Circle_Size, ST7735_BLACK); 
        drawCircle(XRED, Circle_Y, Circle_Size, ST7735_RED);            
        drawCircle(XYEL, Circle_Y, Circle_Size, ST7735_YELLOW);
        drawCircle(XGRN, Circle_Y, Circle_Size, ST7735_GREEN);                       
    }    
    
          
    if (color == Color_Red)     //if Color off make all circles black but leave outline
    {
        fillCircle(XRED, Circle_Y, Circle_Size, ST7735_RED);
        fillCircle(XYEL, Circle_Y, Circle_Size, ST7735_BLACK);
        fillCircle(XGRN, Circle_Y, Circle_Size, ST7735_BLACK); 
        drawCircle(XRED, Circle_Y, Circle_Size, ST7735_RED);            
        drawCircle(XYEL, Circle_Y, Circle_Size, ST7735_YELLOW);
        drawCircle(XGRN, Circle_Y, Circle_Size, ST7735_GREEN); 
    }
        
    if (color == Color_Green)     //if Color off make all circles black but leave outline
    {
        fillCircle(XRED, Circle_Y, Circle_Size, ST7735_BLACK);
        fillCircle(XYEL, Circle_Y, Circle_Size, ST7735_BLACK);
        fillCircle(XGRN, Circle_Y, Circle_Size, ST7735_GREEN); 
        drawCircle(XRED, Circle_Y, Circle_Size, ST7735_RED);            
        drawCircle(XYEL, Circle_Y, Circle_Size, ST7735_YELLOW);
        drawCircle(XGRN, Circle_Y, Circle_Size, ST7735_GREEN);                       
    }    
        
    if (color == Color_Yellow)     //if Color off make all circles black but leave outline
    {
        fillCircle(XRED, Circle_Y, Circle_Size, ST7735_BLACK);
        fillCircle(XYEL, Circle_Y, Circle_Size, ST7735_YELLOW);
        fillCircle(XGRN, Circle_Y, Circle_Size, ST7735_BLACK); 
        drawCircle(XRED, Circle_Y, Circle_Size, ST7735_RED);            
        drawCircle(XYEL, Circle_Y, Circle_Size, ST7735_YELLOW);
        drawCircle(XGRN, Circle_Y, Circle_Size, ST7735_GREEN);                       
    }    
}

void update_LCD_count(char direction, char count)
{
    switch (direction)                                                                              //update traffic light no ped time
    {
        case EW:                                                                                    //Case statements follow defined values at the top of our code
            EW_Count[0] = count/10 + '0';                                                           //Gives upper digit
            EW_Count[1] = count%10 + '0';                                                           //Gives lower digit
            drawtext(XCNT, EW_Count_Y, EW_Count, EW_Color, ST7735_BLACK, TS_2);
            break;
        case EWLT:
            EWLT_Count[0] = count/10 + '0';
            EWLT_Count[1] = count%10 + '0';
            drawtext(XCNT, EWLT_Count_Y, EWLT_Count, EWLT_Color, ST7735_BLACK, TS_2);
            break;
        case NS:
            NS_Count[0] = count/10 + '0';
            NS_Count[1] = count%10 + '0';
            drawtext(XCNT, NS_Count_Y, NS_Count, NS_Color, ST7735_BLACK, TS_2);
            break;
        case NSLT:
            NSLT_Count[0] = count/10 + '0';
            NSLT_Count[1] = count%10 + '0';
            drawtext(XCNT, NSLT_Count_Y, NSLT_Count, NSLT_Color, ST7735_BLACK, TS_2);
            break;
    } 
}

void update_LCD_PED_Count(char direction, char count)
{
    switch (direction)
    {
        case EW:
            PED_EW_Count[0] = count/10 + '0';                                                       // PED count upper digit
            PED_EW_Count[1] = count%10 + '0'; // PED Lower
            drawtext(PED_Count_X, PED_EW_Count_Y, PED_EW_Count, EW_Color, ST7735_BLACK, TS_2);      //Put counter digit on screen
            break;
        case NS:
            PED_NS_Count[0] = count/10 + '0'; // PED count upper digit
            PED_NS_Count[1] = count%10 + '0'; // PED Lower
            drawtext(PED_Count_X, PED_NS_Count_Y, PED_NS_Count, NS_Color, ST7735_BLACK, TS_2);      //Put counter digit on screen
            break;
    }
}

void update_LCD_misc()
{
    int nStep = get_full_ADC();         // calculates the # of steps for analog conversion
    volt = nStep * 5 /1024.0;           // gets the voltage in Volts, using 5V as reference s instead of 4, also divide by 1024 
    //printf("Voltage = %f V \r\n",volt); // Outputs voltage in TeraTerm
    SW_MODE = volt < 2.5 ? 1:0;         // Mode = 1, Day_mode, Mode = 0 Night_mode
    SW_EWPED = EW_PED_SW;
    SW_EWLT = EW_LT_SW;    
    SW_NSPED = NS_PED_SW;
    SW_NSLT = NS_LT_SW;
    
    if (SW_MODE == 0) SW_MODE_Txt[0]= 'N'; else SW_MODE_Txt[0] = 'D';                               // Sets the switch states and mode at the bottom of LCD Screen
    if (SW_NSLT == 0) SW_NSLT_Txt[0]= '0'; else SW_NSLT_Txt[0] = '1';
    if (SW_NSPED == 0) SW_NSPED_Txt[0]= '0'; else SW_NSPED_Txt[0] = '1';
    if (SW_EWLT == 0) SW_EWLT_Txt[0]= '0'; else SW_EWLT_Txt[0] = '1';
    if (SW_EWPED == 0) SW_EWPED_Txt[0]= '0'; else SW_EWPED_Txt[0] = '1';
    if (FLASHING_REQUEST == 0) FlashingR_Txt[0]= '0'; else FlashingR_Txt[0] = '1';
    if (FLASHING == 0) FlashingS_Txt[0]= '0'; else FlashingS_Txt[0]= '1';

    drawtext(35,10, Act_Mode_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
    drawtext(6, Switch_Txt_Y+9, SW_EWPED_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
    drawtext(32, Switch_Txt_Y+9, SW_EWLT_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
    drawtext(58, Switch_Txt_Y+9, SW_NSPED_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
    drawtext(87, Switch_Txt_Y+9, SW_NSLT_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
    drawtext(112, Switch_Txt_Y+9, SW_MODE_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);  
    drawtext(70, 10, FlashingR_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
    drawtext(100, 10, FlashingS_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
}