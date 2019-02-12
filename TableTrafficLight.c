// ***** 0. Documentation Section *****
// TableTrafficLight.c
// Runs on LM4F120/TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.  

// east/west red light connected to PB5               20
// east/west yellow light connected to PB4						10
// east/west green light connected to PB3							08
// north/south facing red light connected to PB2			04
// north/south facing yellow light connected to PB1		02
// north/south facing green light connected to PB0		01
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)


// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void delay(unsigned long sec);
void delay10msec(unsigned long TENmsec);
void SYSTICK(void);
void PORT_INIT(void);
typedef const struct {				//Defining the struct and storing it in the ROM.
	unsigned long out[2];				//Two element sub-struct contains outputs for 1-Vehicles Traffic light & 2-Pedestrians Traffic light.
	unsigned long dly;					//Time delay excuted for each particular state.
	unsigned long next[8];			//Eight elements sub-struct contains the potential next state.
}FSM;													//Naming the defined struct datatype.

#define goN 		0
#define waitN 	1
#define goE 		2
#define waitE 	3
#define walkG		4
#define walkF0	5
#define walkF1	6
#define walkF2	7
#define walkF3	8

#define V_light 		(*((volatile unsigned long *)0x400053FC))
#define sensor			(*((volatile unsigned long *)0x4002400C))
#define walk_Light	(*((volatile unsigned long *)0x400253FC))
#define walk_S			(*((volatile unsigned long *)0x40024010))

FSM TL[9]={										//Nine states ruling the transitions of states depending on the current state and the input.
	{{0x21,0x02}, 100, {goN,waitN,goN,waitN,waitN,waitN,waitN,waitN}},
	{{0x22,0x02}, 50 , {goE,goE,goE,goE,walkG,walkG,walkG,walkG}},
	{{0x0C,0x02}, 100, {goE,goE,waitE,waitE,waitE,waitE,waitE,waitE}},
	{{0x14,0x02}, 50 , {goN,goN,goN,goN,walkG,walkG,walkG,goN}},
	{{0x24,0x08}, 100, {walkG,walkF0,walkF0,walkF0,walkG,walkF0,walkF0,walkF0}},
	{{0x24,0x02}, 50 , {walkF1,walkF1,walkF1,walkF1,walkF1,walkF1,walkF1,walkF1}},
	{{0x24,0x00}, 50 , {walkF2,walkF2,walkF2,walkF2,walkF2,walkF2,walkF2,walkF2}},
	{{0x24,0x02}, 50 , {walkF3,walkF3,walkF3,walkF3,walkF3,walkF3,walkF3,walkF3}},
	{{0x24,0x00}, 50 , {goE,goE,goN,goN,goE,goE,goN,goE}}
};

// ***** 3. Subroutines Section *****

int main(void){ unsigned long s,input,i,r;
	PORT_INIT();
	SYSTICK();
  TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210,ScopeOff); // activate grader and set system clock to 80 MHz
 
  EnableInterrupts();
	s=goN;

  while(1){
    V_light =TL[s].out[0];		//Outputs the indexed state result into Vehicles traffic lights.
		walk_Light=TL[s].out[1];	//Outputs the indexed state result into pedestrians traffic lights.
		delay10msec(TL[s].dly);		//Calling the delay SysTick function and passing delay value indexed in the particular state.
		i=walk_S;									//Acquiring the inputs from switches connected to the pedestrians sensor.
		r=sensor;									//Acquiring the inputs from switches connected to the Vehicles sensor.
		input=i|r;								//Combining both inputs from sensors to form the input index of the next state.
		s=TL[s].next[input];			//next state determination will be based on the value of the index(s).
  }
}

void delay10msec(unsigned long TENmsec){unsigned long i;
	for (i=0; i<TENmsec; i++){
	delay(800000);
	}
}
void delay (unsigned long sec){volatile unsigned long Time;
	unsigned long now=NVIC_ST_CURRENT_R;
		do{
		Time= (now-NVIC_ST_CURRENT_R)&0x00FFFFFF;}
		while (Time<=sec);
}

void PORT_INIT(void){volatile unsigned long delay;
	
	SYSCTL_RCGC2_R=0x00000032;
	delay=SYSCTL_RCGC2_R;
	
	GPIO_PORTB_AFSEL_R=(0x0);
	GPIO_PORTB_AMSEL_R=(0x0);
	GPIO_PORTB_PCTL_R=(0x0);
	GPIO_PORTB_DIR_R=(0xFF);
	GPIO_PORTB_DEN_R=(0xFF);
	
	GPIO_PORTE_AFSEL_R=(0x00);
	GPIO_PORTE_AMSEL_R=(0x00);
	GPIO_PORTE_PCTL_R=(0x00);
	GPIO_PORTE_DIR_R=(0x00);
	GPIO_PORTE_DEN_R=(0xFF);
	
	GPIO_PORTF_AFSEL_R=(0x0);
	GPIO_PORTF_LOCK_R=0x4C4F434B;
	GPIO_PORTF_CR_R=0xFF;
	GPIO_PORTF_AMSEL_R=(0x0);
	GPIO_PORTF_PCTL_R=(0x0);
	GPIO_PORTF_DIR_R=(0xFF);
	GPIO_PORTF_DEN_R=(0xFF);
		
}

void SYSTICK (void){
	NVIC_ST_CTRL_R&=~(0x01);
	NVIC_ST_RELOAD_R= (0x00FFFFFF);
	NVIC_ST_CURRENT_R=0x0;
	NVIC_ST_CTRL_R=(0x05);
}
