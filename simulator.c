#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int r[8],flags[100],pc;
FILE *verilog;
int store(char adresa[1000],int mem[1000], char linie[1000],int param){
 int locatie;
 if(param==1) locatie=adresa[1]-'0';
 if(param==2) locatie=10*(adresa[1]-'0')+adresa[2]-'0';
 if(param==3) locatie=100*(adresa[1]-'0')+10*(adresa[2]-'0')+adresa[3]-'0';
 if(param==4) {
    locatie=r[adresa[2]-'0'];
    fprintf(verilog,"000001_%c_%c_0_00000000\n",adresa[2],adresa[0]);
 }
 if(adresa[0]=='0') mem[locatie]=r[0]; 
 if(adresa[0]=='1') mem[locatie]=r[1]; 
 if(adresa[0]=='2') mem[locatie]=r[2]; 
 if(adresa[0]=='3') mem[locatie]=r[3]; 
 if(adresa[0]=='4') mem[locatie]=r[4]; 
 if(adresa[0]=='5') mem[locatie]=r[5]; 
 if(adresa[0]=='6') mem[locatie]=r[6]; 
 if(adresa[0]=='7') mem[locatie]=r[7]; 
 return 0;
}
int load(char adresa[1000],int mem[1000], char linie[1000],int param){
 int locatie;
 if(param==1) locatie=adresa[1]-'0';
 if(param==2) locatie=10*(adresa[1]-'0')+adresa[2]-'0';
 if(param==3) locatie=100*(adresa[1]-'0')+10*(adresa[2]-'0')+adresa[3]-'0';
 if(param==4) {
    locatie=r[adresa[2]-'0'];
    fprintf(verilog,"000000_%c_%c_0_00000000\n",adresa[2],adresa[0]);
 }
 if(adresa[0]=='0') r[0]=mem[locatie]; 
 if(adresa[0]=='1') r[1]=mem[locatie]; 
 if(adresa[0]=='2') r[2]=mem[locatie]; 
 if(adresa[0]=='3') r[3]=mem[locatie]; 
 if(adresa[0]=='4') r[4]=mem[locatie]; 
 if(adresa[0]=='5') r[5]=mem[locatie]; 
 if(adresa[0]=='6') r[6]=mem[locatie]; 
 if(adresa[0]=='7') r[7]=mem[locatie]; 
 return 0;
}
void afisare(FILE *fileout,char adresa[1000]){
    int a=r[adresa[0]-'0'];
    if(a>32767) a=32768-a;
    fprintf(fileout,"%d ",a);
}
void citire(char adresa[1000]){
    if(adresa[0]=='0') scanf("%d",&r[0]);
    if(adresa[0]=='1') scanf("%d",&r[1]);
    if(adresa[0]=='2') scanf("%d",&r[2]);
    if(adresa[0]=='3') scanf("%d",&r[3]);
    if(adresa[0]=='4') scanf("%d",&r[4]);
    if(adresa[0]=='5') scanf("%d",&r[5]);
    if(adresa[0]=='6') scanf("%d",&r[6]);
    if(adresa[0]=='7') scanf("%d",&r[7]);
}
void move(char adresa[1000]){
    r[adresa[0]-'0']=r[adresa[1]-'0'];
    fprintf(verilog,"001111_%c_%c_%c_00000000\n",adresa[0],'0',adresa[1]);
}
void compare(char adresa[1000]){
    int a,b,i;
    for(i=0;i<100;i++){
        flags[i]=0;
    }
    a=r[adresa[0]-'0'];
    b=r[adresa[1]-'0'];
    fprintf(verilog,"010111_%c_%c_0_00000000\n",adresa[0],adresa[1]);
    if(a==b) {
        flags[0]=1; //zero
        return;
    }
    if(a<b){
        flags[1]=1;  //negative
        return;
    }
    flags[0]=0;
    flags[1]=0;
}

void add(char adresa[1000]){
    int sum,a,b;
    int sign=(1<<15);
    int signa,signb,signsum;
    int bit16=65535;
    a=r[adresa[0]-'0'];
    b=r[adresa[1]-'0'];
    fprintf(verilog,"001001_%c_%c_%c_00000000\n",adresa[2],adresa[0],adresa[1]);
    signa=sign&a;
    signb=sign&b;
    sum=a+b;
    sum=bit16&sum;
    signsum=sign&sum;
    r[adresa[2]-'0']=sum;
    if(signa!=signb) {
        flags[2]=0;
        return;
    }
    if(signsum==signa) {
        flags[2]=0;
        return;
    }
    flags[2]=1;
}
void sub(char adresa[1000]){
    int sum,a,b;
    int sign=(1<<15);
    int signa,signb,signsum;
    int bit16=65535;
    a=r[adresa[0]-'0'];
    b=r[adresa[1]-'0'];
    fprintf(verilog,"001001_%c_%c_%c_00000000\n",adresa[2],adresa[0],adresa[1]);
    signa=sign&a;
    signb=sign&b;
    sum=a-b;
    if(sum<0) sum=32768-sum;
    sum=bit16&sum;
    signsum=sign&sum;
    r[adresa[2]-'0']=sum;
    if(signa!=signb) {
        flags[2]=0;
        return;
    }
    if(signsum==signa) {
        flags[2]=0;
        return;
    }
    flags[2]=1;
}
void lsl(char adresa[1000]){
    int result,a,b,carry=0;
    int sign=(1<<15);
    int bit16=65536;
    int nr_biti=65535;
    fprintf(verilog,"001100_%c_%c_%c_00000000\n",adresa[2],adresa[0],adresa[1]);
    a=r[adresa[0]-'0'];
    b=r[adresa[1]-'0'];
    result=a<<b;
    if(result>=bit16) carry=1;
    result=nr_biti&result;
    r[adresa[2]-'0']=result;
    if(carry) {
        flags[3]=1;
        return;
    }
    flags[3]=0;
}
void lsr(char adresa[1000]){
    int result,a,b;
    a=r[adresa[0]-'0'];
    b=r[adresa[1]-'0'];
    result=a>>b;
    r[adresa[2]-'0']=result;
    fprintf(verilog,"001011_%c_%c_%c_00000000\n",adresa[2],adresa[0],adresa[1]);
}
int power2(int numar){
    int i,s,p;
    s=0;p=1;
    for(i=0;i<numar;i++){
        s=s+p;
        p=p*2;
    }
    return s;
}
void rsl(char adresa[1000]){
    int result,a,b,carry=0;
    int sign=(1<<15);
    int bit16=65536;
    int nr_biti=65535;
    int mask,last_bits;
    fprintf(verilog,"001110_%c_%c_%c_00000000\n",adresa[2],adresa[0],adresa[1]);
    a=r[adresa[0]-'0'];
    b=r[adresa[1]-'0'];
    if(b>15) {
        flags[4]=1;
        r[adresa[2]-'0']=0;
        return;
    }
    mask=power2(b);
    last_bits=a>>(16-b);
    last_bits=last_bits&mask;
    result=a<<b;
    result=result+last_bits;
    if(result>=bit16) carry=1;
    result=nr_biti&result;
    r[adresa[2]-'0']=result;
    if(carry) {
        flags[3]=1;
        return;
    }
    flags[3]=0;
}
void rsr(char adresa[1000]){
    int result,a,b;
    int mask,last_bits;
    a=r[adresa[0]-'0'];
    b=r[adresa[1]-'0'];
    if(b>15) {
        flags[4]=1;
        r[adresa[2]-'0']=0;
        return;
    }
    mask=power2(b);
    last_bits=mask&a;
    last_bits=last_bits<<(16-b);
    result=a>>b;
    result=result+last_bits;
    r[adresa[2]-'0']=result;
    fprintf(verilog,"001101_%c_%c_%c_00000000\n",adresa[2],adresa[0],adresa[1]);
}
void multiply(char adresa[1000]){
    int result,a,b,carry=0;
    int sign=(1<<15);
    int bit16=65536;
    int nr_biti=65535;
    fprintf(verilog,"010000_%c_%c_%c_00000000\n",adresa[2],adresa[0],adresa[1]);
    a=r[adresa[0]-'0'];
    b=r[adresa[1]-'0'];
    result=a*b;
    if(result>=bit16) carry=1;
    result=nr_biti&result;
    r[adresa[2]-'0']=result;
    if(carry) {
        flags[3]=1;
        return;
    }
    flags[3]=0;
}
void division(char adresa[1000]){
    int result,a,b;
    a=r[adresa[0]-'0'];
    b=r[adresa[1]-'0'];
    if(b==0) {
        result=0;
        flags[4]=1;
        return;
    }
    flags[4]=0;
    result=a/b;
    r[adresa[2]-'0']=result;
    fprintf(verilog,"010001_%c_%c_%c_00000000\n",adresa[2],adresa[0],adresa[1]);
}
void modulo(char adresa[1000]){
    int result,a,b;
    a=r[adresa[0]-'0'];
    b=r[adresa[1]-'0'];
    if(b==0) {
        result=0;
        flags[4]=1;
        return;
    }
    flags[4]=0;
    result=a%b;
    r[adresa[2]-'0']=result;
    fprintf(verilog,"010010_%c_%c_%c_00000000\n",adresa[2],adresa[0],adresa[1]);
}
void and(char adresa[1000]){
    r[adresa[2]-'0']=r[adresa[0]-'0'] & r[adresa[1]-'0'];
    fprintf(verilog,"010011_%c_%c_%c_00000000\n",adresa[2],adresa[0],adresa[1]);
}
void or(char adresa[1000]){
    r[adresa[2]-'0']=r[adresa[0]-'0'] | r[adresa[1]-'0'];
    fprintf(verilog,"010100_%c_%c_%c_00000000\n",adresa[2],adresa[0],adresa[1]);
}
void xor(char adresa[1000]){
    r[adresa[2]-'0']=r[adresa[0]-'0'] ^ r[adresa[1]-'0'];
    fprintf(verilog,"010101_%c_%c_%c_00000000\n",adresa[2],adresa[0],adresa[1]);
}
void not(char adresa[1000]){
    r[adresa[0]-'0']=r[adresa[0]-'0'] ^ 65535;
    fprintf(verilog,"010110_%c_%c_%c_00000000\n",adresa[0],'0',adresa[1]);
}
void test(char adresa[1000]){
    int a,b,i;
    for(i=0;i<100;i++){
        flags[i]=0;
    }
    a=r[adresa[0]-'0'];
    b=r[adresa[1]-'0'];
    fprintf(verilog,"011000_%c_%c_0_00000000\n",adresa[0],adresa[1]);
    if(a==b) {
        flags[0]=1; //zero
        return;
    }
    if(a<b){
        flags[1]=1;  //negative
        return;
    }
    flags[0]=0;
    flags[1]=0;
}
void increment(char adresa[1000]){
    int sum,a,b;
    int sign=(1<<15);
    int signa,signb,signsum;
    int bit16=65535;
    a=r[adresa[1]-'0'];
    b=1;
    fprintf(verilog,"011001_%c_%c_%c_00000000\n",adresa[0],'0',adresa[1]);
    signa=sign&a;
    signb=sign&b;
    sum=a+b;
    sum=bit16&sum;
    signsum=sign&sum;
    r[adresa[0]-'0']=sum;
    if(signa!=signb) {
        flags[2]=0;
        return;
    }
    if(signsum==signa) {
        flags[2]=0;
        return;
    }
    flags[2]=1;
}
void decrement(char adresa[1000]){
    int sum,a,b;
    int sign=(1<<15);
    int signa,signb,signsum;
    int bit16=65535;
    a=r[adresa[1]-'0'];
    b=1;
    fprintf(verilog,"011010_%c_%c_%c_00000000\n",adresa[0],'0',adresa[1]);
    signa=sign&a;
    signb=sign&b;
    sum=a-b;
    if(sum<0) sum=32768-sum;
    sum=bit16&sum;
    signsum=sign&sum;
    r[adresa[0]-'0']=sum;
    if(signa!=signb) {
        flags[2]=0;
        return;
    }
    if(signsum==signa) {
        flags[2]=0;
        return;
    }
    flags[2]=1;
}
void branch(FILE *fisier, int *lineAdress, int mem[1000], FILE *fileout,char adresa[1000]);
void returnBranch(FILE *fisier, int *lineAdress, int mem[1000], FILE *fileout,char adresa[1000]);
void citire_fisier(FILE *fisier, int *lineAdress, int mem[1000], FILE *fileout, int verificareInceput){
int ok,param,comanda,line,count,pcFor;
char adresa[1000],character,linie[1000];
line=*lineAdress;
pcFor=pc;
while(1){
        ok=0;
        line++;
        if(verificareInceput==1) {
            character=fgetc(fisier);
        if(character!=' ') {
        fscanf(fisier,"%s",linie);
        if(linie[strlen(linie)-1]!=':') {
            printf("Eroare linia %d\n",line);
            break;
        }
        }
        }
        verificareInceput=1;
        fgets(linie,1000,fisier);
        count=0;ok=0;
        while(linie[count]!='\n' && linie[count]!='\0'){
            if(linie[count]==' ') {
                count++;
                continue;
            }
            if(ok==0 && linie[count]=='S' && linie[count+1]=='T' &&linie[count+2]=='R' && linie[count+3]==' ' && linie[count+4]=='R' && strchr("01234567",linie[count+5]) && linie[count+6]==' ' && linie[count+7]=='['){
                comanda=0;
                adresa[0]=linie[count+5];
                if(linie[count+9]==']') {
                    param=1;
                    adresa[1]=linie[count+8];
                    if(!strchr("0123456789",adresa[1])) {
                        ok=0;
                        break;
                    }
                    count=count+10;
                    ok=1;
                }
                else if(linie[count+10]==']') {
                    param=2;
                    if(linie[count+8]=='R') {
                        param=4;
                        adresa[1]=linie[count+8];
                        adresa[2]=linie[count+9];
                        if(!strchr("01234567",adresa[2])) {
                            ok=0;
                            break;
                        }
                    }
                    else {
                        adresa[1]=linie[count+8];
                        adresa[2]=linie[count+9];
                        if(!strchr("0123456789",adresa[1]) || !strchr("0123456789",adresa[2])) {
                            ok=0;
                            break;
                        }
                    }
                    count=count+11;
                    ok=1;
                } 
                else if(linie[count+11]==']'){
                    param=3;
                    adresa[1]=linie[count+8];
                    adresa[2]=linie[count+9];
                    adresa[3]=linie[count+10];
                    if(!strchr("0123456789",adresa[1]) || !strchr("0123456789",adresa[2]) || !strchr("0123456789",adresa[3])) {
                        ok=0;
                        break;
                    }
                    ok=1;
                    count=count+12;
                }
                else {
                    ok=0;
                    break;
                }
            }
            else if(ok==0&&(linie[count]=='H')&&(linie[count+1]=='L')&&(linie[count+2]=='T')) {
                    comanda=1;
                    ok=1;
                    count=count+3;
                }
            else if(ok==0 && (linie[count]=='O') && (linie[count+1]=='U') && (linie[count+2]=='T') && (linie[count+3]==' ') && (linie[count+4]=='R') && strchr("01234567",linie[count+5]) && (linie[count+6]==' ') && (linie[count+7]=='4')) {
                comanda=2;
                ok=1;
                adresa[0]=linie[count+5];
                count=count+8;
            }
            else if(ok==0 && (linie[count]=='I') && (linie[count+1]=='N') && (linie[count+2]=='P') && (linie[count+3]==' ') && (linie[count+4]=='R') && strchr("01234567",linie[count+5]) && (linie[count+6]==' ') && (linie[count+7]=='2')) {
                comanda=3;
                ok=1;
                adresa[0]=linie[count+5];
                count=count+8;
            }
            else if(ok==0 && linie[count]=='L' && linie[count+1]=='D' &&linie[count+2]=='R' && linie[count+3]==' ' && linie[count+4]=='R' && strchr("01234567",linie[count+5]) && linie[count+6]==' ' && linie[count+7]=='['){
                comanda=4;
                adresa[0]=linie[count+5];
                if(linie[count+9]==']') {
                    param=1;
                    adresa[1]=linie[count+8];
                    if(!strchr("0123456789",adresa[1])) {
                        ok=0;
                        break;
                    }
                    count=count+10;
                    ok=1;
                }
                else if(linie[count+10]==']') {
                    param=2;
                    if(linie[count+8]=='R') {
                        param=4;
                        adresa[1]=linie[count+8];
                        adresa[2]=linie[count+9];
                        if(!strchr("01234567",adresa[2])) {
                            ok=0;
                            break;
                        }
                    }
                    else {
                        adresa[1]=linie[count+8];
                        adresa[2]=linie[count+9];
                        if(!strchr("0123456789",adresa[1]) || !strchr("0123456789",adresa[2])) {
                            ok=0;
                            break;
                        }
                    }
                    count=count+11;
                    ok=1;
                } 
                else if(linie[count+11]==']'){
                    param=3;
                    adresa[1]=linie[count+8];
                    adresa[2]=linie[count+9];
                    adresa[3]=linie[count+10];
                    if(!strchr("0123456789",adresa[1]) || !strchr("0123456789",adresa[2]) || !strchr("0123456789",adresa[3])) {
                        ok=0;
                        break;
                    }
                    ok=1;
                    count=count+12;
                }
                else {
                    ok=0;
                    break;
                }
            }
            else if(ok==0 && (linie[count]=='M') && (linie[count+1]=='O') && (linie[count+2]=='V') && (linie[count+3]==' ') && (linie[count+4]=='R') && strchr("01234567",linie[count+5]) && (linie[count+6]==' ') && (linie[count+7]=='R') && strchr("01234567",linie[count+8])) {
                comanda=5;
                ok=1;
                adresa[0]=linie[count+5];
                adresa[1]=linie[count+8];
                count=count+9;
            }
            else if(ok==0 && (linie[count]=='C') && (linie[count+1]=='M') && (linie[count+2]=='P') && (linie[count+3]==' ') && (linie[count+4]=='R') && strchr("01234567",linie[count+5]) && (linie[count+6]==' ') && (linie[count+7]=='R') && strchr("01234567",linie[count+8])) {
                comanda=6;
                ok=1;
                adresa[0]=linie[count+5];
                adresa[1]=linie[count+8];
                count=count+9;
            }
            else if(ok==0 && (linie[count]=='B') && (linie[count+1]=='R') && (linie[count+2]=='A') && (linie[count+3]==' ') ) {
                
                comanda=7;
                ok=1;
                strcpy(adresa,(linie+count+4));
                strcpy(adresa,strtok(adresa,"\n"));
                break;
            }
            else if(ok==0 && (linie[count]=='J') && (linie[count+1]=='M') && (linie[count+2]=='P') && (linie[count+3]==' ') ) {
                
                comanda=28;
                ok=1;
                strcpy(adresa,(linie+count+4));
                strcpy(adresa,strtok(adresa,"\n"));
                break;
            }
            else if(ok==0 && (linie[count]=='B') && (linie[count+1]=='R') && (linie[count+2]=='Z') && (linie[count+3]==' ') ) {
                comanda=8;
                ok=1;
                strcpy(adresa,(linie+count+4));
                strcpy(adresa,strtok(adresa,"\n"));
                break;
            }
            else if(ok==0 && (linie[count]=='B') && (linie[count+1]=='R') && (linie[count+2]=='N') && (linie[count+3]==' ') ) {
                comanda=9;
                ok=1;
                strcpy(adresa,(linie+count+4));
                strcpy(adresa,strtok(adresa,"\n"));
                break;
            }
            else if(ok==0 && (linie[count]=='B') && (linie[count+1]=='R') && (linie[count+2]=='O') && (linie[count+3]==' ') ) {
                comanda=10;
                ok=1;
                strcpy(adresa,(linie+count+4));
                strcpy(adresa,strtok(adresa,"\n"));
                break;
            }
            else if(ok==0 && (linie[count]=='B') && (linie[count+1]=='R') && (linie[count+2]=='C') && (linie[count+3]==' ') ) {
                comanda=11;
                ok=1;
                strcpy(adresa,(linie+count+4));
                strcpy(adresa,strtok(adresa,"\n"));
                break;
            }
            else if(ok==0 && (linie[count]=='A') && (linie[count+1]=='D') && (linie[count+2]=='D') && (linie[count+3]==' ') && (linie[count+4]=='R') && strchr("01234567",linie[count+5]) && (linie[count+6]==' ') && (linie[count+7]=='R') && strchr("01234567",linie[count+8]) && (linie[count+9]==' ') && (linie[count+10]=='R') && strchr("01234567",linie[count+11])) {
                comanda=12;
                ok=1;
                adresa[2]=linie[count+5];
                adresa[0]=linie[count+8];
                adresa[1]=linie[count+11];
                count=count+12;
            }
            else if(ok==0 && (linie[count]=='S') && (linie[count+1]=='U') && (linie[count+2]=='B') && (linie[count+3]==' ') && (linie[count+4]=='R') && strchr("01234567",linie[count+5]) && (linie[count+6]==' ') && (linie[count+7]=='R') && strchr("01234567",linie[count+8]) && (linie[count+9]==' ') && (linie[count+10]=='R') && strchr("01234567",linie[count+11])) {
                comanda=13;
                ok=1;
                adresa[2]=linie[count+5];
                adresa[0]=linie[count+8];
                adresa[1]=linie[count+11];
                count=count+12;
            }
            else if(ok==0 && (linie[count]=='L') && (linie[count+1]=='S') && (linie[count+2]=='R') && (linie[count+3]==' ') && (linie[count+4]=='R') && strchr("01234567",linie[count+5]) && (linie[count+6]==' ') && (linie[count+7]=='R') && strchr("01234567",linie[count+8]) && (linie[count+9]==' ') && (linie[count+10]=='R') && strchr("01234567",linie[count+11])) {
                comanda=14;
                ok=1;
                adresa[2]=linie[count+5];
                adresa[0]=linie[count+8];
                adresa[1]=linie[count+11];
                count=count+12;
            }
            else if(ok==0 && (linie[count]=='L') && (linie[count+1]=='S') && (linie[count+2]=='L') && (linie[count+3]==' ') && (linie[count+4]=='R') && strchr("01234567",linie[count+5]) && (linie[count+6]==' ') && (linie[count+7]=='R') && strchr("01234567",linie[count+8]) && (linie[count+9]==' ') && (linie[count+10]=='R') && strchr("01234567",linie[count+11])) {
                comanda=15;
                ok=1;
                adresa[2]=linie[count+5];
                adresa[0]=linie[count+8];
                adresa[1]=linie[count+11];
                count=count+12;
            }
            else if(ok==0 && (linie[count]=='M') && (linie[count+1]=='U') && (linie[count+2]=='L') && (linie[count+3]==' ') && (linie[count+4]=='R') && strchr("01234567",linie[count+5]) && (linie[count+6]==' ') && (linie[count+7]=='R') && strchr("01234567",linie[count+8]) && (linie[count+9]==' ') && (linie[count+10]=='R') && strchr("01234567",linie[count+11])) {
                comanda=16;
                ok=1;
                adresa[2]=linie[count+5];
                adresa[0]=linie[count+8];
                adresa[1]=linie[count+11];
                count=count+12;
            }
            else if(ok==0 && (linie[count]=='D') && (linie[count+1]=='I') && (linie[count+2]=='V') && (linie[count+3]==' ') && (linie[count+4]=='R') && strchr("01234567",linie[count+5]) && (linie[count+6]==' ') && (linie[count+7]=='R') && strchr("01234567",linie[count+8]) && (linie[count+9]==' ') && (linie[count+10]=='R') && strchr("01234567",linie[count+11])) {
               comanda=17;
               ok=1;
               adresa[2]=linie[count+5];
               adresa[0]=linie[count+8];
               adresa[1]=linie[count+11];
               count=count+12;
            }
            else if(ok==0 && (linie[count]=='M') && (linie[count+1]=='O') && (linie[count+2]=='D') && (linie[count+3]==' ') && (linie[count+4]=='R') && strchr("01234567",linie[count+5]) && (linie[count+6]==' ') && (linie[count+7]=='R') && strchr("01234567",linie[count+8]) && (linie[count+9]==' ') && (linie[count+10]=='R') && strchr("01234567",linie[count+11])) {
               comanda=18;
               ok=1;
               adresa[2]=linie[count+5];
               adresa[0]=linie[count+8];
               adresa[1]=linie[count+11];
               count=count+12;
            }
            else if(ok==0 && (linie[count]=='A') && (linie[count+1]=='N') && (linie[count+2]=='D') && (linie[count+3]==' ') && (linie[count+4]=='R') && strchr("01234567",linie[count+5]) && (linie[count+6]==' ') && (linie[count+7]=='R') && strchr("01234567",linie[count+8]) && (linie[count+9]==' ') && (linie[count+10]=='R') && strchr("01234567",linie[count+11])) {
               comanda=19;
               ok=1;
               adresa[2]=linie[count+5];
               adresa[0]=linie[count+8];
               adresa[1]=linie[count+11];
               count=count+12;
            }
            else if(ok==0 && (linie[count]=='O') && (linie[count+1]=='R') && (linie[count+2]==' ') && (linie[count+3]=='R') && strchr("01234567",linie[count+4]) && (linie[count+5]==' ') && (linie[count+6]=='R') && strchr("01234567",linie[count+7]) && (linie[count+8]==' ') && (linie[count+9]=='R') && strchr("01234567",linie[count+10])) {
               comanda=20;
               ok=1;
               adresa[2]=linie[count+4];
               adresa[0]=linie[count+7];
               adresa[1]=linie[count+10];
               count=count+11;
            }
            else if(ok==0 && (linie[count]=='X') && (linie[count+1]=='O') && (linie[count+2]=='R') && (linie[count+3]==' ') && (linie[count+4]=='R') && strchr("01234567",linie[count+5]) && (linie[count+6]==' ') && (linie[count+7]=='R') && strchr("01234567",linie[count+8]) && (linie[count+9]==' ') && (linie[count+10]=='R') && strchr("01234567",linie[count+11])) {
               comanda=21;
               ok=1;
               adresa[2]=linie[count+5];
               adresa[0]=linie[count+8];
               adresa[1]=linie[count+11];
               count=count+12;
            }
            else if(ok==0 && (linie[count]=='N') && (linie[count+1]=='O') && (linie[count+2]=='T') && (linie[count+3]==' ') && (linie[count+4]=='R') && strchr("01234567",linie[count+5]) && (linie[count+6]==' ') && (linie[count+7]=='R') && strchr("01234567",linie[count+8])) {
               comanda=22;
               ok=1;
               adresa[0]=linie[count+5];
               adresa[1]=linie[count+8];
               count=count+9;
            }
            else if(ok==0 && (linie[count]=='T') && (linie[count+1]=='S') && (linie[count+2]=='T') && (linie[count+3]==' ') && (linie[count+4]=='R') && strchr("01234567",linie[count+5]) && (linie[count+6]==' ') && (linie[count+7]=='R') && strchr("01234567",linie[count+8])) {
               comanda=23;
               ok=1;
               adresa[0]=linie[count+5];
               adresa[1]=linie[count+8];
               count=count+9;
            }
            else if(ok==0 && (linie[count]=='I') && (linie[count+1]=='N') && (linie[count+2]=='C') && (linie[count+3]==' ') && (linie[count+4]=='R') && strchr("01234567",linie[count+5]) && (linie[count+6]==' ') && (linie[count+7]=='R') && strchr("01234567",linie[count+8])) {
               comanda=24;
               ok=1;
               adresa[0]=linie[count+5];
               adresa[1]=linie[count+8];
               count=count+9;
            }
            else if(ok==0 && (linie[count]=='D') && (linie[count+1]=='E') && (linie[count+2]=='C') && (linie[count+3]==' ') && (linie[count+4]=='R') && strchr("01234567",linie[count+5]) && (linie[count+6]==' ') && (linie[count+7]=='R') && strchr("01234567",linie[count+8])) {
               comanda=25;
               ok=1;
               adresa[0]=linie[count+5];
               adresa[1]=linie[count+8];
               count=count+9;
            }
            else if(ok==0 && (linie[count]=='R') && (linie[count+1]=='S') && (linie[count+2]=='R') && (linie[count+3]==' ') && (linie[count+4]=='R') && strchr("01234567",linie[count+5]) && (linie[count+6]==' ') && (linie[count+7]=='R') && strchr("01234567",linie[count+8]) && (linie[count+9]==' ') && (linie[count+10]=='R') && strchr("01234567",linie[count+11])) {
                comanda=26;
                ok=1;
                adresa[2]=linie[count+5];
                adresa[0]=linie[count+8];
                adresa[1]=linie[count+11];
                count=count+12;
            }
            else if(ok==0 && (linie[count]=='R') && (linie[count+1]=='S') && (linie[count+2]=='L') && (linie[count+3]==' ') && (linie[count+4]=='R') && strchr("01234567",linie[count+5]) && (linie[count+6]==' ') && (linie[count+7]=='R') && strchr("01234567",linie[count+8]) && (linie[count+9]==' ') && (linie[count+10]=='R') && strchr("01234567",linie[count+11])) {
                comanda=27;
                ok=1;
                adresa[2]=linie[count+5];
                adresa[0]=linie[count+8];
                adresa[1]=linie[count+11];
                count=count+12;
            }
            else if(ok==0 && (linie[count]=='R') && (linie[count+1]=='E') && (linie[count+2]=='T')) {
                comanda=29;
                ok=1;
                break;
            }
            else {
                ok=0;
                break;
            }
        }
        if(!ok) {
            printf("Eroare la linia %d\n",line);
            break;
        }
        if(comanda==0) store(adresa,mem,linie,param);
        if(comanda==1) break;
        if(comanda==2) afisare(fileout,adresa);
        if(comanda==3) citire(adresa);
        if(comanda==4) load(adresa,mem,linie,param);
        if(comanda==5) move(adresa);
        if(comanda==6) compare(adresa);
        if(comanda==7) {
            fprintf(verilog,"000110_");
            branch(fisier,&line,mem,fileout,adresa);
            break;
        }
        if(comanda==8) {
            fprintf(verilog,"000010_0_0_");
            if(flags[0]==1) {
                branch(fisier,&line,mem,fileout,adresa);
                break;
            }
            else fprintf(verilog,"0_00000000\n");
        }
        if(comanda==9) {
            fprintf(verilog,"000011_0_1_");
            if(flags[1]==1) {
                branch(fisier,&line,mem,fileout,adresa);
                break;
            }
            else fprintf(verilog,"0_00000000\n");
        }
        if(comanda==10) {
            fprintf(verilog,"000101_1_0_");
            if(flags[2]==1) {
                branch(fisier,&line,mem,fileout,adresa);
                break;
            }
            else fprintf(verilog,"0_00000000\n");
        }
        if(comanda==11) {
            fprintf(verilog,"000100_1_1_");
            if(flags[3]==1) {
                branch(fisier,&line,mem,fileout,adresa);
                break;
            }
            else fprintf(verilog,"0_00000000\n");
        }
        if(comanda==12) add(adresa);
        if(comanda==13) sub(adresa);
        if(comanda==14) lsr(adresa);
        if(comanda==15) lsl(adresa);
        if(comanda==16) multiply(adresa);
        if(comanda==17) division(adresa);
        if(comanda==18) modulo(adresa);
        if(comanda==19) and(adresa);
        if(comanda==20) or(adresa);
        if(comanda==21) xor(adresa);
        if(comanda==22) not(adresa);
        if(comanda==23) test(adresa);
        if(comanda==24) increment(adresa);
        if(comanda==25) decrement(adresa);
        if(comanda==26) rsr(adresa);
        if(comanda==27) rsl(adresa);
        if(comanda==28) {
            pc=line;
            fprintf(verilog,"000111_0_0_");
            branch(fisier,&line,mem,fileout,adresa);
            break;
        }
        if(comanda==29) {
            if(pc==0) {
                printf("Eroare return");
                exit(1);
            }
                else {
                    fprintf(verilog,"001000_0_0_");
                    returnBranch(fisier,&line,mem,fileout,adresa);
                }
            break;
        }
    }
}
void toBinary(int numar){
    int mask=256,i;
    if(numar>=mask) {
        fprintf(verilog,"1_");
        numar=numar-mask;
    }
        else fprintf(verilog,"0_");
    mask=mask/2;
    for(i=0;i<8;i++) {
        if(numar>=mask) {
            fprintf(verilog,"1");
            numar=numar-mask;
        }
            else fprintf(verilog,"0");
        mask=mask/2;
    }
    fprintf(verilog,"\n");
}
void returnBranch(FILE *fisier, int *lineAdress, int mem[1000], FILE *fileout,char adresa[1000]){
    int line=pc,i;
    char cuvant[1000];
    rewind(fisier);
    for(i=0;i<pc;i++){
        fgets(cuvant,1000,fisier);
    }
    pc=0;
    toBinary(line+1);
    citire_fisier(fisier,&line,mem,fileout,0);
}
void branch(FILE *fisier, int *lineAdress, int mem[1000], FILE *fileout,char adresa[1000]){
    char cuvant[1000];
    int line=0;
    strcat(adresa,":");
    rewind(fisier);
    while(1) {
        line++;
        if(fscanf(fisier,"%s",cuvant)==EOF) {
            printf("Eroare");
            exit(1);
        }
        if(strcmp(cuvant, adresa)==0) {
            line--;
            toBinary(line+1);
            citire_fisier(fisier,&line,mem,fileout,0);
            break;
        }
        fgets(cuvant,1000,fisier);
    }
}
int main(){
    int mem[1000],param,comanda,line;
    char linie[1000],adresa[1000],character;
    FILE *fisier=fopen("fisier.txt","r");
    FILE *fileout=fopen("fileout.txt","w");
    verilog=fopen("verilog.txt","w");
    line=0;
    citire_fisier(fisier,&line,mem,fileout,1);
    return 0;
}