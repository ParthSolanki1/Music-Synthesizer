
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<math.h>

#define FS 44100			

typedef struct note_struct{		
	double freq;			
	int bar;			
	double index;			
					
	double *waveform;		

        double out_tminus1;		
	int wave_length;		
	int input_idx, output_idx;	
	int n_sampled;			
					
	struct note_struct *next;	
} note;

note    *playlist_head=NULL;		
char    note_names[100][5];		
double  note_freq[100];			

void read_note_table(void)
{
    FILE *f;
    char line[1024];
    int idx;
   
    f=fopen("note_frequencies.txt","r");
    if (f==NULL)
    {
      printf("Unable to open note frequencies file!\n");
      return;
    }
    
    idx=0;
    while (fgets(&line[0],1024,f))
    {
      note_names[idx][0]=line[0];
      note_names[idx][1]=line[1];
      if (line[2]!='\t') note_names[idx][2]=line[2]; else note_names[idx][2]='\0';
      note_names[idx][3]='\0';

      if (line[2]=='\t') note_freq[idx]=strtod(&line[3],NULL);
      else note_freq[idx]=strtod(&line[4],NULL);
      
      printf("Read note for table with name %s, frequency=%f\n",&note_names[idx][0],note_freq[idx]);
      idx++;
    }
    printf("Processed %d notes!\n",idx);
}

note *new_note(double freq, int bar, double index)
{

 note *n;
 
 n=(note *)calloc(1,sizeof(note));
 if (!n)
 {
  fprintf(stderr,"new_note(): Out of Memory!, are you trying to load a Great Symphony???\n");
  return(NULL);
 }
 
 n->freq=freq;
 n->bar=bar;
 n->index=index;
 n->input_idx=0;
 n->output_idx=1;
 n->out_tminus1=((double)rand()/(double)RAND_MAX)-.5;
 n->n_sampled=0;
 n->next=NULL;

 n->wave_length=round((double)FS/n->freq);
 n->waveform=(double *)calloc(n->wave_length,sizeof(double));
 if (!n)
 {
  fprintf(stderr,"new_note(): Ran out of memory allocating waveform. Toast!\n");
  return(NULL);
 }

 for (int i=0;i<n->wave_length;i++)
 {
  *(n->waveform + i)=(1.0*(double)rand()/(double)RAND_MAX)-.5;	// A random number in [-.5, .5]
 }

 return(n);
}

note *playlist_insert(note *head, double freq, int bar, double index)
{

  
 note *n_n, *p;
 
 n_n=new_note(freq,bar,index);
 if (n_n==NULL) return head;
 
 if (head==NULL) return n_n;

 p=head;
 while (p->next!=NULL) p=p->next;
 p->next=n_n;
 return head;  
}

void delete_playlist(note *head)
{
   note *t;
   
   while (head!=NULL)
   {
     t=head->next;
     free(head->waveform);
     free(head);
     head=t;
   }
}

int key_from_time_index(int bar, int index)
{
 int key=0;

 key=(bar*100) + (index+1);
 return(key);
}

double KS_string_sample(note *n)
{
 
 double output, new_input;

 n->n_sampled++;		

 output=*(n->waveform+n->output_idx);
 new_input=.995*((.25*n->out_tminus1)+(.75*output));

 n->out_tminus1=output;
 *(n->waveform+n->input_idx)=new_input;

 n->output_idx++;
 if (n->output_idx==n->wave_length) n->output_idx=0;
 n->input_idx++;
 if (n->input_idx==n->wave_length) n->input_idx=0;

 return(output);
}

void write_wav_header(FILE *f, unsigned int samples)
{
 char txt[250];
 uint32_t chunk2size;
 uint32_t tmp;
 unsigned char tmp2;
 chunk2size=samples*2*16/8;			

 strcpy(&txt[0],"RIFF");			
 fwrite(&txt[0],4*sizeof(unsigned char),1,f);
 tmp=chunk2size+36;
 fwrite(&tmp,4*sizeof(unsigned char),1,f);	
 strcpy(&txt[0],"WAVE");
 fwrite(&txt[0],4*sizeof(unsigned char),1,f);	
 strcpy(&txt[0],"fmt ");
 fwrite(&txt[0],4*sizeof(unsigned char),1,f);	
 tmp=16;
 fwrite(&tmp,4*sizeof(unsigned char),1,f);	
 tmp2=1;
 fwrite(&tmp2,sizeof(unsigned char),1,f);
 tmp2=0;		
 fwrite(&tmp2,sizeof(unsigned char),1,f);	
 tmp2=2;
 fwrite(&tmp2,sizeof(unsigned char),1,f);
 tmp2=0;		
 fwrite(&tmp2,sizeof(unsigned char),1,f);	
 tmp=FS;
 fwrite(&tmp,4*sizeof(unsigned char),1,f);	
 tmp=FS*2*16/8;					
 fwrite(&tmp,4*sizeof(unsigned char),1,f);	
 tmp2=2*16/8;
 fwrite(&tmp2,sizeof(unsigned char),1,f);
 tmp2=0;
 fwrite(&tmp2,sizeof(unsigned char),1,f);	
 tmp2=16;
 fwrite(&tmp2,sizeof(unsigned char),1,f);
 tmp2=0;
 fwrite(&tmp2,sizeof(unsigned char),1,f);	
 strcpy(&txt[0],"data");
 fwrite(&txt[0],4*sizeof(unsigned char),1,f);	
 fwrite(&chunk2size,4*sizeof(unsigned char),1,f);	
}

void play_notes(int bar_length)
{
 note *q, *st, *ed;
 int bar, max_bar, max_length, s_idx;
 double index;
 unsigned int sample_idx, max_sample_idx;
 int16_t this_sample_int;
 double this_sample, this_sample_r, this_sample_l;
 double balance,fAmp;
 int done;
 FILE *f;

 if (playlist_head==NULL) 
 {
   printf("Input playlist is empty!\n");
   return;
 }
 max_bar=0;
 q=playlist_head;
 while (q!=NULL)
 {
  max_bar=q->bar;
  q=q->next;
 }

 
 max_length=3*FS;		
 max_sample_idx=bar_length*FS*(max_bar+1);

 fprintf(stderr,"\nPlaying song. %d bars, max note length is %d samples. Output is 'output.wav'\n\n",max_bar,max_length); 

 f=fopen("output.wav","wb+");	
 if (f)
 {
  write_wav_header(f,max_sample_idx);

  st=ed=playlist_head;		
  sample_idx=0;				

  for (bar=playlist_head->bar;bar<=max_bar;bar++)
  {
   for (index=0;index<=1.0;index+=1.0/(bar_length*44100.0))
   {
    done=0;
    while(!done)		
    {
     done=1;
     if (ed->next!=NULL) 
     {
       if ((double)ed->next->bar+ed->next->index<=(double)bar+index) 
       {
	 ed=ed->next;
	 done=1;
       }
     }
    }
    
    if (st!=NULL)
    {
      q=st;
      this_sample_l=0;
      this_sample_r=0;
      while(ed->next!=q)
      {
       balance=(q->freq-525.0);			
       if (balance<0) balance=(525.0+balance)/525.0;
       else balance=balance/(5000.0-525.0);
       fAmp=(pow(q->freq,.33)/18.0);		
       this_sample=KS_string_sample(q)*fAmp;
       this_sample_l+=(balance)*this_sample;
       this_sample_r+=(1.0-balance)*this_sample;
       q=q->next;
      } 
      
      
      this_sample_r=tanh(this_sample_r);
      this_sample_int=(int16_t)(this_sample_r*32700);
      fwrite(&this_sample_int,2*sizeof(unsigned char),1,f);    	
      this_sample_l=tanh(this_sample_l);
      this_sample_int=(int16_t)(this_sample_l*32700);
      fwrite(&this_sample_int,2*sizeof(unsigned char),1,f);    	

      sample_idx++;
    }
    else	
    {
     for (s_idx=0;s_idx<44100;s_idx++)
     {
      this_sample_int=0;
      fwrite(&this_sample,2*sizeof(unsigned char),1,f);    	// L channel
      fwrite(&this_sample,2*sizeof(unsigned char),1,f);    	// R channel
      sample_idx++;
     }
    }
 
    done=0;
    while (!done)
    {
     done=1;
     if (st!=NULL&&st!=ed->next)				// Should never go beyond ed
      if (st->n_sampled>max_length) 
      {
       st=st->next;
       done=0;
      }       
    }
    if (sample_idx>max_sample_idx) break;
   }		
   if (sample_idx>max_sample_idx) break;
  }		
	
  fclose(f);
 }
 else fprintf(stderr,"Unable to open file for output!\n");

 delete_playlist(playlist_head);
 playlist_head=NULL;
}

