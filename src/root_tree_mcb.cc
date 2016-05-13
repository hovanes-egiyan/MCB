#include <TFile.h>
#include <TTree.h>

extern char   variable_b_name[20][50];
extern char   variable_i_name[20][50];
extern char   variable_f_name[20][50];
extern char   variable_d_name[20][50];
extern int    b_value        [20];
extern int    i_value        [20];
extern float  f_value        [20];
extern double d_value        [20];
extern   TFile*  root_file;
extern   TTree*  t1;

int open_rootfile(char *fname, char *mode, int n_entries)
{
  char    branch_format[1024];

  int     i,j;

   fprintf(stdout, "  opening root file %s for mcb %s\n",fname,mode);
   fprintf(stdout, "  opening root file for %d entries\n",n_entries);
//
// 8 bit signed integers
      sprintf(variable_b_name[0],"%s","ih");
      sprintf(variable_b_name[1],"%s","ik");
      sprintf(variable_b_name[2],"%s","il");
      sprintf(variable_b_name[3],"%s","col");
// 32 bit signed integers
//      sprintf(variable_i_name[0],"%s","int");
// 32 bit floats
      sprintf(variable_f_name[0], "%s","Eg");
      sprintf(variable_f_name[1], "%s","the");
      sprintf(variable_f_name[2], "%s","phe");
      sprintf(variable_f_name[3], "%s","thg");
      sprintf(variable_f_name[4], "%s","phg");
      sprintf(variable_f_name[5], "%s","xe");
      sprintf(variable_f_name[6], "%s","ye");
      sprintf(variable_f_name[7], "%s","ze");
      sprintf(variable_f_name[8], "%s","intensity");
      sprintf(variable_f_name[9], "%s","sigms");
      sprintf(variable_f_name[10],"%s","th");
      sprintf(variable_f_name[11],"%s","al");
// 64 bit doubles
      sprintf(variable_d_name[0],"%s","q");
      sprintf(variable_d_name[1],"%s","ql2");
      sprintf(variable_d_name[2],"%s","qt2");

//  fprintf(stdout," 1 variables done: %s \n",variable_f_name[1]);
//   root_file = new TFile( fname, "RECREATE"); // overwrite
  root_file = new TFile( fname, "CREATE");
  fprintf(stdout," reCreate done for file: %s\n",fname);
  root_file->SetCompressionLevel(1);
  t1 = new TTree( "mcb_tree" , mode );
  t1->SetMaxTreeSize(1<<29);  // split into files of 512 MByte
  fprintf(stdout," TREE t1 setup for %s mode %s\n",fname,mode);

  j=0;
  for (i=0; i < 4; i++) {    /*  "/B" = 8 bit signed integer */
    sprintf(branch_format, "%s/B", variable_b_name[i]);
    t1->Branch(variable_b_name[i], &b_value[i], branch_format);
    j++;
  }
//   for (i=0; i < -^; i++) {    /*  "/I" = 32 bit signed integer */
//     sprintf(branch_format, "%s/I", variable_i_name[i]);
//     t1->Branch(variable_i_name[i], &i_value[i], branch_format);
//     j++;
//   }
  for (i=0; i < 12; i++) {    /*  "/F" = 32 bit floating point */
    sprintf(branch_format, "%s/F", variable_f_name[i]);
    t1->Branch(variable_f_name[i], &f_value[i], branch_format);
    j++;
  }
  for (i=0; i < 3; i++) {    /*  "/D" = 64 bit floating point */
    sprintf(branch_format, "%s/D", variable_d_name[i]);
    t1->Branch(variable_d_name[i], &d_value[i], branch_format);
    j++;
  }

  if( j != n_entries){
      fprintf(stdout,"\nERROR in setup: inconsistent %d :: %d\n",j,n_entries);
      exit(-1);
  }

fprintf(stdout," TREE t1  done for %s mode %s\n",fname,mode);
return (0);
}

// int root_end(TTree *t1)
// {
//   t1->Print();  // print info on screen

  
//   root_file->Write();
//   root_file->Close();
// }
