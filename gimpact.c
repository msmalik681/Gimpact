#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define MAX_STRING 6000
#define MIN_STRING 200

///////////////////////////////////////////////
////////////////// FUNCTIONS //////////////////
///////////////////////////////////////////////

int read_act(int i)
{//function to read act file
    int a=0;
    FILE *act_file = fopen("pal.act","rb"); // open input act file
    fseek(act_file, i, SEEK_SET );
    fread(&a, 1, 1, act_file);
    //printf("%x ", a);
    fclose(act_file);
    return a;
}

void write_gpl(char text[MAX_STRING], char output_file[MIN_STRING])
{//function to write to gpl file
    FILE *gpl_file = fopen(output_file,"w"); // make output gpl file
    fprintf(gpl_file,"%s",text);
    fclose(gpl_file);
    printf("Conversion complete!\n");
    return;
}


void shutdown()
{
    printf("Example usage: ./gimpact [path to .gpl or .act file]\nGimpact tool made by msmalik681 to convert between .act and .gpl palette files.\n");
    exit(0);
}

///////////////////////////////////////////////
////////////////// MAIN LOOP //////////////////
///////////////////////////////////////////////

int main(int argc, char *argv[])
{
    char temp[MIN_STRING]=".gpl";
    
     if ( access(argv[1], F_OK) != 0)
     {
        shutdown();
     } else if( strcasecmp(".gpl", strrchr(argv[1], '.')) == 0 )
     {// valid gimp palette lets convert to act file.

        // make some variables needed to run the code
        char buffer[MIN_STRING],text[MAX_STRING];
        int i=0,j=0;
        char *last_token;
        char output_file[MIN_STRING];

        printf("please pick .act palette name:");
        scanf("%s", output_file);
        strcat(output_file, ".act");

    
        FILE *gpl_file = fopen(argv[1],"rb"); // open input gimp gpl file.
        FILE *act_file = fopen(output_file, "wb"); // make output act file.
    
        while( fgets(text, MAX_STRING, gpl_file) != NULL ) // read input gpl file until the end.
        {
            if (i>3) // gpl files use first 4 lines for header data lets skip them.
            {
                last_token = strtok( text, " \t" ); // move to the first token.
                j = atoi(last_token); // convert string to integer.
                fwrite(&j, 1, 1, act_file); // write 1 byte.
                last_token = strtok( NULL, " \t" ); // move to next token and repeat.
                j = atoi(last_token);
                fwrite(&j, 1, 1, act_file);
                last_token = strtok( NULL, " \t" );
                j = atoi(last_token);
                fwrite(&j, 1, 1, act_file);
            }
            i++; // move counter along.
        }
        
        // lets close the files we opened.
        fclose(gpl_file); 
        fclose(act_file);
    } else if( strcasecmp(".act", strrchr(argv[1], '.')) == 0 )
    {//valid .act file lets convert to .gpl
        
                // make some variables needed to run the code
        char buffer[MIN_STRING],text[MAX_STRING];
        int i=0,j=0;
        char output_file[MIN_STRING];

        printf("please pick .gpl palette name:");
        scanf("%s", output_file);
        strcat(output_file, ".gpl");

        for(i=0;i<768;i++) // read only the palette colours 256*3=768
        {
            if(i==0) sprintf(text,"GIMP Palette\nName: ACT2GPL\nColumns: 16\n#\n"); // on the first run should write header data.
    
            sprintf(buffer,"\t%d\t%d\t%d\tIndex %d\n",read_act(i),read_act(i+1),read_act(i+2),j); // store gimp format line data in buffer.
            strcat(text,buffer); // append the line data to the text variable
            i=i+2; j++; // move counters along. j follows the line number for gpl and i reads 3 tokens at a time from the act file.
        }
    
        write_gpl(text, output_file); // when finished use function to write everything to gpl file.
    } else { shutdown(); }
}


