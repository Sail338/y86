#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
int registers[8];
typedef union bitfeild{
	struct whyisthisathing{
		unsigned int hi: 4;
		unsigned int lo: 4;
	}whyisthisathing;
	 unsigned char  byte;
} omglol;
unsigned int program_counter = 0;
unsigned int inital = 0;
char *memory;
omglol *kek;
int ZF;
int OF;
int SF;

unsigned int  memsize;
char * loadmem (char *filename, char* buf){
	FILE* filetoread = fopen(filename,"r");
       	//maxx legnth
	int lineno = 0;
		
	char* memory;
	while(1){
		if(fgets(buf,1000,filetoread) == NULL) break;
		if(lineno == 0){
			//exculde everything but digits and plop it as a hex in memsize
		//TODO Possibbly have to ignore size
			sscanf(buf,"%*[^0123456789]%x",&memsize);
			memory = malloc(sizeof(char) * memsize);
		}
		lineno++;
		//read text directive
		if(strstr(buf,".text") != NULL){
			char *ptr = buf;
			//read program counter
			while(!isdigit(*ptr)){
				ptr ++;		
			}
			//hit program counter
			sscanf(ptr,"%x",&program_counter);
			inital = program_counter;
			//now readevery two and plop it in the shit
			while(!isspace(*ptr)){
				ptr ++;
			}

			while(isspace(*ptr)){
				ptr ++;
			}
			while(*ptr != '\0'){

				if(isspace(*ptr)){
					break;
				}
				omglol* test = malloc(sizeof(omglol));
				unsigned int blah;	
				//ideally points to first irl instruction
				sscanf(ptr,"%2x",&blah);
				test->byte = (unsigned char)blah;
				memory[program_counter] = test->byte;
				test->byte = memory[program_counter];
				program_counter ++;
				ptr +=2;
				
				free(test);
			}
		}
		char x[2];
		x[0] = 'S';
		x[1] = '\0';
		memcpy(&memory[program_counter],x,2);
		//byte
		if(strstr(buf,".byte") != NULL){
			char *ptr = buf;
			ptr +=5;
			unsigned int adress;
			 int value;
			sscanf(ptr,"%x %x",&adress,&value);
			memory[adress] = (unsigned char)value;

		}
		//long>
		if(strstr(buf,".long") != NULL){
			char *ptr = buf;
			ptr +=6;
			unsigned int adress;
			unsigned int actualval;
			 char  value [1999];
			sscanf(ptr,"%x %s",&adress,value);
			actualval = atoi(value);
			unsigned int* pt = (unsigned int*)&memory[adress];
			*pt = actualval;	
		//	memcpy(&memory[adress],&actualval,4);

		}

		if(strstr(buf,".string") != NULL){
			char *ptr = buf;
			ptr +=7;
			unsigned int adress;

			 char  value [1999];

			sscanf(ptr,"%x  \"%[^\"]\"",&adress,value);
			int i;
			unsigned int it = adress;

			for(i=0;i<strlen(value);i++){
				memory[it] = value[i];
				it++;	
			}
			memory[it] = '\0';

		}
		
	}	
	fclose(filetoread);
	return memory;

}
void setFlags(int x){
	if(x<0){
		SF =1;
	}
	if(x>0){
		SF = 0;
	}
	if(x==0){
		ZF =1;
	}
	if(x!=0){
		ZF = 0;
	}
}

void halt(){
	printf("HALT\n");
	inital ++;
}
void NoOp(){
	inital ++;
	return;
}

void printregister(int x){
	if(x==0){
		printf("eax");
	}
	if(x==1){
		printf("ecx");
	}
	if(x==2){
		printf("edx");
	}
	if(x==3){
		printf("ebx");
	}
	if(x==4){
		printf("esp");
	}
	if(x==5){
		printf("ebp");
	}
	if(x==6){
		printf("esi");
	}
	if(x==7){
		printf("edi");
	}

}
void irmovl(){
	inital++;
	omglol *reg = malloc(sizeof(omglol));
	reg ->byte = memory[inital];
	int index = reg->whyisthisathing.hi;
	inital++;
	int * val =  (int *)&memory[inital];
	inital +=4;
	printf("irmovl 0x%x,",*val);
	printregister(index);
	printf("\n");
	free(reg);
}
void rrmovl(){
	inital++;
	omglol *reg = malloc(sizeof(omglol));
	reg ->byte = memory[inital];
	int  rB = reg->whyisthisathing.hi;
	int rA = reg->whyisthisathing.lo;
	free(reg);
	printf("rrmovl ");
	printregister(rA);
	printf(",");
	printregister(rB);
	inital ++;
	printf("\n");
}
void rmmovl(){
	//register to memory move;
	inital ++;
	omglol *reg = malloc(sizeof(omglol));
	reg->byte = memory[inital];
	int rA = reg->whyisthisathing.lo;
	int rB = reg->whyisthisathing.hi;
	inital++;
	int *offset = (int *)&memory[inital];
	inital +=4;
	printf("rmmovl ");
	printregister(rA);
	printf(", %d(",*offset);
	printregister(rB);
	printf(")");
	printf("\n");	

}

void mrmovl(){
	inital ++;
	omglol *reg = malloc(sizeof(omglol));
	reg->byte = memory[inital];
	int rA = reg->whyisthisathing.lo;
	int rB = reg->whyisthisathing.hi;
	inital ++;
	int *offset = (int *)&memory[inital];
	free(reg);
	printf("mrmovl ");
	printf("%d (",*offset);
	printregister(rB);
	printf(")");
	printf(",");
	printregister(rA);
	inital +=4;
	printf("\n");

}


void addl(){

	inital++;
	omglol *reg = malloc(sizeof(omglol));
	reg->byte = memory[inital];
	int rA = reg->whyisthisathing.lo;
	int rB = reg->whyisthisathing.hi;
	//overflow detection
	printf("addl ");
	printregister(rA);
	printf(",");
	printregister(rB);
	free(reg);
	printf("\n");
	inital ++;	
}
void subl(){
	inital ++;
	omglol *reg = malloc(sizeof(omglol));
	reg->byte = memory[inital];
	int rA = reg->whyisthisathing.lo;
	int rB = reg->whyisthisathing.hi;
	printf("subl ");
	printregister(rA);
	printf(",");
	printregister(rB);
	printf("\n");
	free(reg);
	inital ++;	
}

void andl(){
	OF = 0;  /*bitwise operators get a 0 */

	inital++;
	omglol *reg = malloc(sizeof(omglol));
	reg->byte = memory[inital];
	int rA = reg->whyisthisathing.lo;
	int rB = reg->whyisthisathing.hi;

	printf("andl ");
	printregister(rA);
	printf(",");
	printregister(rB);
	printf("\n");
	free(reg);
	inital ++;

}
void xorl(){
	OF = 0;
	//set OF to 0
	inital++;
	omglol *reg = malloc(sizeof(omglol));
	reg->byte = memory[inital];
	int rA = reg->whyisthisathing.lo;
	int rB = reg->whyisthisathing.hi;

	printf("xorl ");
	printregister(rA);
	printf(",");
	printregister(rB);
	printf("\n");
	free(reg);
	inital ++;
}
void mull(){
	inital++;
	omglol *reg = malloc(sizeof(omglol));
	reg->byte = memory[inital];
	int rA = reg->whyisthisathing.lo;
	int rB = reg->whyisthisathing.hi;

	printf("mull ");
	printregister(rA);
	printf(",");
	printregister(rB);
	printf("\n");
	free(reg);
	inital ++;
}
void pushl(){
	inital ++;
	omglol *reg = malloc(sizeof(omglol));
	reg ->byte = memory[inital];
	int rA = reg->whyisthisathing.lo;
	
	printf("pushl ");
	printregister(rA);
	printf("\n");
	//subtracts four from ESP
	free(reg);
	inital ++;	
}
void popl(){
	inital ++;
	omglol *reg = malloc(sizeof(omglol));
	reg -> byte = memory[inital];
	int rA = reg->whyisthisathing.lo;

	printf("popl ");
	printregister(rA);
	printf("\n");
	free(reg);
	inital++;

}
void call(){
	inital ++;
	unsigned int offset = *((unsigned int *)&memory[inital]);
	//grab 32 bit dest and place it in dest
	
	printf("call %d",offset);
	printf("\n");
	inital +=4;
	}

void ret(){
	printf("ret");
	printf("\n");
	inital ++;
	//dscrement one from inital (will be added back anyway after the instruction)

}
void cmpl(){
	inital ++;
	omglol *reg = malloc(sizeof(omglol));
	reg->byte = memory[inital];
	int rA = reg->whyisthisathing.lo;
	int rB = reg->whyisthisathing.hi;

	printf("cmpl ");
	printregister(rA);
	printf(",");
	printregister(rB);
	printf("\n");
	inital ++;
}


//MAKE SURE TO GOTO ADRESS -1, casue after instruction we add it back anyway, but we dont want to double count
void jmp(){
	inital ++;
	//JUMP
	//set inital to the ofset
	unsigned int dest; 
	memcpy(&dest,&memory[inital],4);
	printf("jmp %x",dest);
	printf("\n");
	inital +=4;
}

void jle(){
	inital ++;
	unsigned int dest; 
	memcpy(&dest,&memory[inital],4);
	printf("jle %x",dest);
	printf("\n");
	inital +=4;
}
void jl(){
	inital ++;
	unsigned int dest; 
	memcpy(&dest,&memory[inital],4);
	printf("jl %x",dest);
	printf("\n");
	inital +=4;
}

void je(){
	inital ++;
	unsigned int dest; 
	memcpy(&dest,&memory[inital],4);
	printf("je %x",dest);
	printf("\n");
	inital +=4;
}
void jne(){
	inital ++;
	unsigned int dest; 
	memcpy(&dest,&memory[inital],4);
	printf("jne %x",dest);
	printf("\n");
	inital +=4;
}
void jge(){
	inital++;
	unsigned int dest; 
	memcpy(&dest,&memory[inital],4);
	printf("jge %x",dest);
	printf("\n");
	inital +=4;
}
void jg(){
	inital ++;
	unsigned int dest; 
	memcpy(&dest,&memory[inital],4);
	printf("jg %x",dest);
	printf("\n");
	inital +=4;

}
void writeB(){
	inital ++;
	omglol *reg = malloc(sizeof(omglol));
	reg->byte = memory[inital];
	int rA = reg->whyisthisathing.lo;
	inital ++;
	//get offset
	unsigned int *offset = (unsigned int *)&memory[inital];
	printf("writeB ");
	printf("%d(",*offset);
	printregister(rA);
	printf(")");
	printf("\n");
	inital +=4;
	free(reg);
}
void writeL(){
	
	inital ++;
	omglol *reg = malloc(sizeof(omglol));
	reg->byte = memory[inital];
	int rA = reg->whyisthisathing.lo;
	inital ++;
	//get offset
	unsigned int *offset = (unsigned int *)&memory[inital];

	printf("writeL ");
	printf("%d(",*offset);
	printregister(rA);
	printf(")");
	printf("\n");
	inital +=4;
	free(reg);
}
void readB(){
	inital ++;
	omglol *reg = malloc(sizeof(omglol));
	reg->byte = memory[inital];
	int rA = reg->whyisthisathing.lo;
	inital ++;
	unsigned int *offset = (unsigned int *)&memory[inital];
				
	printf("readB ");
	printf("%d(",*offset);
	printregister(rA);
	printf(")");
	printf("\n");
	inital +=4;
	
}

void readL(){
	
	inital ++;
	omglol *reg = malloc(sizeof(omglol));
	reg->byte = memory[inital];
	int rA = reg->whyisthisathing.lo;
	inital ++;
	unsigned int *offset = (unsigned int *)&memory[inital];

	printf("readL ");
	printf("%d(",*offset);
	printregister(rA);
	printf(")");
	printf("\n");
	inital +=4;

}
void(*readX[2])() = {readB,readL};
void(*writeX[2])() = {writeB,writeL};
void reads(){
	(*readX[kek->whyisthisathing.hi])();
}
void writes(){
	(*writeX[kek->whyisthisathing.hi])();
}
void movsbl(){
	inital ++;
	omglol *reg = malloc(sizeof(omglol));
	reg->byte = memory[inital];
        int rA = reg->whyisthisathing.lo;
	int rB = reg->whyisthisathing.hi;
	inital ++;
	 int *offset = ( int *)(&memory[inital]);
	
	printf("movsbl ");
	printregister(rA);
	printf(", %d(",*offset);
	printregister(rB);
	printf(")");
	printf("\n");
	inital +=4;
		
}
void (*jXX[7]) () = {jmp,jle,jl,je,jne,jge,jg};
void (*op1[6])() = {addl,subl,andl,xorl,mull,cmpl};
void op(){
	(*op1[kek->whyisthisathing.hi])();

}
void jmps(){
	(*jXX[kek->whyisthisathing.hi])();

}
void (*y86table[16])() = {NoOp,halt,rrmovl,irmovl,rmmovl,mrmovl,op,jmps,call,ret,pushl,popl,reads,writes,movsbl};
int  execute(){
	 unsigned int x;
	
	while(strcmp("S",&memory[inital]) !=0){
		x = memory[inital];
		
		kek = malloc(sizeof(omglol));
		kek -> byte = x;
		(*y86table[kek->whyisthisathing.lo])();
		free(kek);
	}	
	return 0;
}
int main(int argc, char**argv){
	char buf[1000];
	memory = loadmem(argv[1],buf);
	execute();
	free(memory);
	return 0;


}
