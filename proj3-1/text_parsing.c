#include "text_parsing.h"
#define MAX_BUFFER 1024
#define MAX_CHILDREN 64

/*
Open the file "makefile" or "Makefile"

Input:   filename
Return:  array of TNode
*/
TNode **parse_nodes(char* filename){
  FILE *file = NULL; 
  struct dirent *in_dir;
  DIR *dir = opendir("."); 
  char *m_file = "makefile";
  char *M_file = "Makefile";
  int has_file = 0;
  TNode **node;
 
  if(dir == NULL){
    fprintf(stderr, "Error: Cannot open directory\n");
      exit(-1);
  }
  
  // Find makefile in current directory. Open if exists
  while((in_dir=readdir(dir)) != NULL){
     if(filename != NULL){
        if(strcmp(in_dir->d_name, filename) == 0){
           file = fopen(in_dir->d_name, "r");
           has_file = 1;
        }
     }else{
       if(strcmp(in_dir->d_name, m_file) == 0 || strcmp(in_dir->d_name, M_file) == 0){
         file = fopen(in_dir->d_name, "r");
         break;
       }else{
        continue;
       }
     }
  }
  
  // Exit if there is
  if(!has_file && filename != NULL){
     fprintf(stderr, "Error: There is no such filename (%s)\n", filename);
    exit(1); 
  }
  
  // get array of TNode struct
  node = parse_lines(file);
  closedir(dir);
  fclose(file);
  return node;
}


/*
Parse target, dependencies, and commands and store them in a TNode. 
Make an array of TNodes, each of which has parsed data

Input:  file to parse
Return: array of TNode
*/
TNode **parse_lines(FILE *file){
   char *str;
   char *parent;
   char *child;
  char *input, char_in, tmp;
   int iscommand, isdpdncy, line_end, file_end, empty_line;    
   int line = 0, cnt, n_cnt = -1; // n_cnt: node count
   TNode *p_node, *c_node= NULL;
 
  // allocate memory for array of TNodes to be returned
   TNode **nodes = (TNode**)calloc(MAX_CHILDREN, MAX_CHILDREN*sizeof(TNode*));
   for(int i =0; i < MAX_CHILDREN; i++){
      nodes[i] = NULL; 
   }
 
   //loop until the file ends
   while(!feof(file)){


      if((input =(char*)malloc(MAX_BUFFER*sizeof(char))) == NULL){
         fprintf(stderr, "ERROR: Fail to allocate Buffer\n");
         exit(1);
      }
      //check the first line
      iscommand = 0;
      isdpdncy = 0;
      empty_line = 0;

      tmp = fgetc(file);
      if(tmp == -1){
         break;
      }
    
    // handles different cases for every char, and exit if there are error
      if(tmp == ' '){
         fprintf(stderr, "Line %d: Cannot start the line with a space\n", line);
         exit(1);
      } else if(tmp == '\t'){
         //command start
         iscommand = 1;
      } else if(isalpha(tmp) || isdigit(tmp)){
         //parent start
         isdpdncy = 1;
         if(n_cnt != -1){
            nodes[n_cnt] = p_node;
         }
         n_cnt++;
      } else if(tmp == '#'){
         //iscomment = 1;
         //comment start
      } else if(tmp == '\n'){
         empty_line = 1;
         line++;
      } else{
         //invalid start
         fprintf(stderr, "Line %d: Cannot start the line with an invalid character (%c)\n", line, tmp);
         exit(1);
      }
   
      cnt = 0;
      line_end = 0;
      file_end = 0;
      
      //if there is no tab for the first character, it should be the parent and children
      //if line starts with a tab, it is the start of the command
      while(!line_end && !file_end && !empty_line){
         //if tmp is a start of parent
         if(isdpdncy && tmp){
            input[cnt] = tmp;
            tmp = 0;
            cnt++;
            continue;
         }
        
         char_in = fgetc(file);
         if(char_in == EOF){
            line_end = 1;
            file_end = 1;
         } else if(char_in == '\n'){
            if(iscommand && !tmp){
               fprintf(stderr, "Line %d: Command cannot be empty string\n", line);
               exit(1);
            }else{
               tmp = 0;
               line_end = 1;
            }
         } else if(cnt < MAX_BUFFER){
            input[cnt] = char_in;
            cnt++;
         }
      }
   
      //end of the line
      if(line_end){
         line++;
      }
   
    // if char count is less than the MAX_BUFFER, make a TNode and store parent and child
    // exit if there are error
      if(cnt < MAX_BUFFER){
         //parse line and add it to Node
         input[cnt] = '\0';
         if(isdpdncy){
            str = strtok(input, ":");
            parent = remove_space(str);

            p_node = initNode(parent, line); //make parent node

            //get children by tokenizing 
            if((str = strtok(NULL, ":")) != NULL){
               child = strtok(str, " ");
               
               c_node = initNode(child, 0); //init child node
               addChild(p_node, c_node); //add child node into parent node
            
               while((child = strtok(NULL, " ")) != NULL){
                  c_node = initNode(child, 0);
                  addChild(p_node, c_node);
             	}
	       
			   
            }
         } else if(iscommand){ // add command to TNode
            addCommand(p_node, input);
         }
      }else{
        fprintf(stderr, "Line %d: Command line too long\n", line);
      }
    
   }
   nodes[n_cnt] = p_node;
   return nodes;
}

/*
Helper function to remove " " in a given string
*/
char* remove_space(char* str){
   char* result = (char*)calloc(strlen(str)+ 1, sizeof(char));   
   int d = 0;
   for(int i = 0; str[i] != '\0'; i++){
      if(str[i] != ' '){
         result[d] = str[i];
         d++;
      }
   }
   
   return result;
}
