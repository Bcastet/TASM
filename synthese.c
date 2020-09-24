#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FE 44100
#define DUREE 5
#define N 1024


/* output */
static char *RAW_OUT = "tmp-out.raw";
static char *FILE_OUT = "out.wav";

FILE *
sound_file_open_write (void)
{
 return fopen (RAW_OUT, "wb");
}

void
sound_file_close_write (FILE *fp)
{
  char cmd[256];
  fclose (fp);
  snprintf(cmd, 256, "sox -c 1 -r %d -e signed-integer -b 16 %s %s", (int)FE, RAW_OUT, FILE_OUT);
  system (cmd);
}

void
sound_file_write (double *s, FILE *fp)
{
  int i;
  short tmp[N];
  for (i=0; i<N; i++)
    {
      tmp[i] = (short)(s[i]*32768);
    }
  fwrite (tmp, sizeof(short), N, fp);
}

void 
naive_sinus(double *s,double f,int current){
    for(int i=current;i<current + N;i++){
      //printf("%d\n",i-current);
      s[i-current] += sin((1./44100) * i * 2 * M_PI * f );
      //printf("%f\n",s[i-current]);
    }
}

void
multiple_sinus_synthesis(double *s,double *f,double num,int current){
    
    for(int x=0;x<num;x++){
        naive_sinus(s,f[x],current);
    }
    for(int i=current;i<current+N;i++){
        s[i-current] /=num;
    }
}

void
fm_synthesis(double *s,double a,double fc,double fm,double i,int current){
    for(int t=current;t<N+current;t++){
        s[t-current] = a * sin(2 * M_PI * fc * t * i + i * sin( 2 * M_PI * fm * t));
        printf("%f\n", s[t-current]);
    }
}

int
main (int argc, char *argv[])
{
  int i;
  FILE *output;
  
  output = sound_file_open_write ();
  if (argc != 1)
    {
      printf ("usage: %s\n", argv[0]);
      exit (EXIT_FAILURE);
    }
  int current = 0;
  while (current<FE*DUREE)
  { 
    double s[N];
    for(int i=0;i<N;i++)
      s[i] = 0;
    double f[4] = {440,5000,6500,4999};
    
    fm_synthesis(s,1000,440,5000,300,current);
    
    sound_file_write (s, output);
    
    current+=N;
  }
  sound_file_close_write (output);
  exit (EXIT_SUCCESS);
}
