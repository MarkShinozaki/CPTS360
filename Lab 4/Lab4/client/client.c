#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <arpa/inet.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <libgen.h>     
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define MAX 256
#define PORT 1234

// Global variables
char line[MAX], ans[MAX];
int n;
struct sockaddr_in saddr; 
int sfd;

// Functions used locally
// prints menu of options for commands
void mymenu(void){
  printf("******************************** menu ********************************\n");
  printf("THESE ARE YOUR SERVER COMMANDS:\n");
  printf(" get     - downloads file from pathname on server to cwd\n");
  printf(" put     - upload file from pathname on client to server\n");
  printf(" ls      - null: ls files locally| path: ls files on path\n");
  printf(" cd      - null: set cwd to root| path: change cwd to path\n");
  printf(" pwd              (prints server's cwd)\n");
  printf(" mkdir   - makes a dir on the server on pathname if able\n");
  printf(" rmdir   - removes a dir on the server on pathname if able\n");
  printf(" rm      - removes a file on the server on pathname if able\n");
  printf("\n* THESE ARE THE CLIENT COMMANDS:\n");
  printf(" menu    - prints out available options for commands\n");
  printf(" lcat    - prints contents of file on pathname to terminal\n");
  printf(" lls     - null: ls files locally| path: ls files on path\n");
  printf(" lcd     - null: set cwd to root| path: change cwd to path\n");
  printf(" lpwd             (prints client's cwd)\n");
  printf(" lmkdir  - makes a dir on the client on pathname if able\n");
  printf(" lrmdir  - removes a dir on the client on pathname if able\n");
  printf(" lrm     - removes a file on the client on pathname if able\n");
  printf("******************************** menu ********************************\n");
}

//////////////////////////////////////////////////////////

int mycat(char *filename){

  int fd = open(filename, O_RDONLY);
  int oldfd;
  char buf[MAX];
  int bytes_read = 0;
  oldfd = dup(0);
  if(fd == -1){
    printf("client: Failed to open file [%s]. Exiting lcat\n",filename);
    return 0;
  }
  while((bytes_read=read(fd,buf,sizeof(buf))>0))printf("%s",buf);
  close(fd);
  dup2(oldfd,1);
  
  return 1;
  
}

///////////////////////////////////////////////////////////


int myls(char* path){

  DIR* dir_pointer;
  struct dirent* current_dir_pointer;
  struct stat file_attribute_accessor;
  struct tm* time_information;
  char time_holder[50]="";
  char file_type = '\0';
  char current_dir[MAX];
  char abs_file_name[MAX] = "";
  char buf[MAX];

  if(!strcmp("",path)){
    printf("client: ls -l starting at cwd\n");
    getcwd(current_dir,MAX);
    dir_pointer = opendir(current_dir);
  } 
  else{
    if((dir_pointer = opendir(path))==NULL){
      printf("client: Failed to list contents of %s\n",path);
      return 0;
    }
    printf("client: ls -l starting at %s\n",path);
    strcpy(current_dir,path);
  }

  while((current_dir_pointer = readdir(dir_pointer)) != NULL){
    strcpy(abs_file_name,current_dir);
    strcat(abs_file_name,"/");
    strcat(abs_file_name,current_dir_pointer->d_name);
    
    if(stat(abs_file_name,&file_attribute_accessor)!=-1){

      (S_ISDIR(file_attribute_accessor.st_mode)) ? printf("d") : printf("-");
      (file_attribute_accessor.st_mode & S_IRUSR) ? printf("r") : printf("-");
      (file_attribute_accessor.st_mode & S_IWUSR) ? printf("w") : printf("-");
      (file_attribute_accessor.st_mode & S_IXUSR) ? printf("x") : printf("-");
      (file_attribute_accessor.st_mode & S_IRGRP) ? printf("r") : printf("-");
      (file_attribute_accessor.st_mode & S_IWGRP) ? printf("w") : printf("-");
      (file_attribute_accessor.st_mode & S_IXGRP) ? printf("x") : printf("-");
      (file_attribute_accessor.st_mode & S_IROTH) ? printf("r") : printf("-");
      (file_attribute_accessor.st_mode & S_IWOTH) ? printf("w") : printf("-");
      (file_attribute_accessor.st_mode & S_IXOTH) ? printf("x") : printf("-");
      printf("  %5li ", file_attribute_accessor.st_nlink);

    
      struct passwd *uid;
      struct group *gid;
      
      uid = getpwuid(file_attribute_accessor.st_uid);
      printf("%s ",uid->pw_name);
      gid = getgrgid(file_attribute_accessor.st_gid);
      printf("%s ",gid->gr_name);

      
      printf("%8li ", file_attribute_accessor.st_size);
      
      
      time_information = localtime(&(file_attribute_accessor.st_mtime));
      strftime(time_holder,49,"%b %d %R",time_information);
      printf("%s ",time_holder);

      
      printf("%s\n",current_dir_pointer->d_name);

    }    
  }

  
  
  closedir(dir_pointer);
  return  1;
  
}


int mycd(char* path){

  
  if(!strcmp(path,"")){
    printf("client: Path set to root\n");
    chdir(getenv("HOME"));
    return 1;
  }
  
  else{
    if(chdir(path)==0){
      printf("client: Successfully changed directories.\n");
      return 1;
    }
    else{
      printf("client: Failed to change directories.\n");
      return 0;
    }  
  }
}
// prints cwd
int mypwd(){

  char current_dir[MAX];
   // Get cwd and store it in current_dir[]
   getcwd(current_dir, MAX);
    
   // Display current working directory before each command
   printf("cwd:%s\n",current_dir);

   // return when done
   return 1;
      
}



int mymkdir(char* path){

  if(!strcmp(path,"")){
      printf("client: filepath empty, mkdir failed.\n");
      return 0;
  }

  char old_cwd[MAX];
  getcwd(old_cwd,MAX);
  char current_dir[MAX] = "";
  strcpy(current_dir,old_cwd);

  
  if(path[0]!=(char)47){
    strcat(current_dir,"/");
    strcat(current_dir,path);
  }
  else{
    strcpy(current_dir,getenv("HOME"));
    strcat(current_dir,path);
  }


  if(!mkdir(current_dir, S_IRUSR|S_IWUSR|S_IXUSR)){
    printf("client: Created directory in specified path\n");

    
    char holder[MAX] = "";
    int last_slash=0;
    strcpy(holder,current_dir);
    for(int i = 0;holder[i]!= '\0';i++){
      if(holder[i]==(char)47)last_slash=i;
    }
    holder[last_slash]='\0';
    myls(holder);
    return 1;
  }
  else{
    printf("client: Failed to create directory in specified path\n");
    return 0;
  }
  
}

int myrmdir(char* path){

  
  if(!strcmp(path,"")){
      printf("client: filepath empty, rmdir failed.\n");
      return 0;
  }

  char old_cwd[MAX];
  getcwd(old_cwd,MAX);
  char current_dir[MAX] = "";
  strcpy(current_dir,old_cwd);

  if(path[0]!=(char)47){
    strcat(current_dir,"/");
    strcat(current_dir,path);
  }
  else{
    strcpy(current_dir,getenv("HOME"));
    strcat(current_dir,path);
  }



  if(!rmdir(current_dir)){
    printf("client: Deleted directory in specified path\n");
    char holder[MAX] = "";
    int last_slash=0;
    strcpy(holder,current_dir);
    for(int i = 0;holder[i]!= '\0';i++){
      if(holder[i]==(char)47)last_slash=i;
    }
    holder[last_slash]='\0';
    myls(holder);
    return 1;
  }
  else{
    printf("client: Failed to delete directory in specified path\n");
    return 0;
  }
}



int myrm(char*path){
	
    char old_cwd[MAX];
	getcwd(old_cwd,MAX);
	char current_dir[MAX] = "";
	strcpy(current_dir,old_cwd);



	if(!strcmp(path,"")){
		printf("client: filepath empty, rm failed.\n");
		return 0;
	}
	

	if(path[0]!=(char)47){
		strcat(current_dir,"/");
		strcat(current_dir,path);
	}
	else{
		strcpy(current_dir,getenv("HOME"));
		strcat(current_dir,path);
	}
	
	if(!unlink(current_dir)){
		printf("client: Deleted file at specified path\n");
		
		
		char holder[MAX] = "";
		int last_slash=0;
		strcpy(holder, current_dir);
		for(int i = 0; holder[i] != '\0'; i++){
			if(holder[i] == (char)47)
				last_slash = i;
		}
		holder[last_slash] = '\0';
		myls(holder);
		return 1;
	}
	else{
		printf("client: failed to remove file at %s\n", path);
		return 0;
	}
}

//client side of the put function
int clientput(char *path){
	
	if(strcmp(ans,"OK") != 0){
		printf("client: failed to open file on server side, put failed\n");
		return 0;
	}
	char holder[MAX] = "";
	strcpy(holder,path);
	char* holder2 = strtok(path,"/");
	while(holder2){
		strcpy(holder,holder2);
		holder2 = strtok(NULL,"/");
	}
	
	int fd = open(holder, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	int size = 0, bytes_received = 0;
	printf("open %s for WRITE : filename=%s\n", holder, holder);
	
	n = read(sfd, ans, MAX);
	size = atoi(ans);
	
	printf("read from server, write to file\n");
	printf("size : %d\n", size);
	
	
	while(bytes_received < size){
		n = read(sfd,ans,MAX);									
		if((bytes_received+n) > size){
			n -= (bytes_received+n)-size;
		}
		bytes_received += n;
		printf("n=%d total=%d\n", n, bytes_received);
		write(fd,ans,n);										
		
	}
	close(fd);
	
	if(bytes_received == size){
		printf("%d bytes received\n", bytes_received);
		return 1;
	}
	else{
		printf("client: error, file partially received, %d/%d bytes.\n", n, size);
		return 0;
	}
}

//client side of the get function
int clientget(char *path){
	
	if(!strcmp(path,"")){
		printf("client: filepath empty, put failed.\n");
		write(sfd,"fail get",MAX);
		return 0;
	}

	char old_cwd[MAX];
	getcwd(old_cwd,MAX);
	char current_dir[MAX] = "";
	strcpy(current_dir,old_cwd);

	
	if(path[0]!=(char)47){
		strcat(current_dir,"/");
		strcat(current_dir,path);
	}
	else{
		strcpy(current_dir,getenv("HOME"));
		strcat(current_dir,path);
	}

	int fd = open(current_dir, O_RDONLY);			
	if(fd < 0){
		printf("client: Failed to open file on path %s. put failed\n",path);
		write(sfd,"fail put",MAX);
		return 0;
	}
	struct stat filestats;
	fstat(fd, &filestats);							
	int size = filestats.st_size;					
	char sizestr[MAX] = "";
	sprintf(sizestr,"%d",size);
	
	printf("stat %s stat OK: size=%d\n", path, size);
	write(sfd,sizestr,MAX);
	printf("open %s for READ\n", path);
	
	int bytes_sent = 0;
	
	while(bytes_sent < size){	
		char holder[MAX+1] = "";
		n=read(fd,holder,MAX);				
		if(!write(sfd, holder, MAX))
			break;
		bytes_sent += n;
		printf("n=%d total=%d\n",n,bytes_sent);
	}
	close(fd);
	if(bytes_sent == size){							
		printf("sent %d bytes\n", bytes_sent);
		return 1;
	}
	else{
		printf("client: error, file partially sent, %d/%d bytes.\n", n, size);
		return 0;
	}
}

int clientls(){
	char line[MAX] = "";
	
	while(strcmp(line, "END LS") != 0){
		printf("%s",line);
		read(sfd,line,MAX);
	}
}

// code start head
int main(int argc, char *argv[], char *env[]) 
{
	int n; char how[64];
	int i;
	char current_dir[MAX];


	printf("1. create a socket\n");
	sfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sfd < 0) { 
	  printf("socket creation failed\n"); 
	  exit(0); 
	}

	printf("2. fill in server IP and port number\n");
	bzero(&saddr, sizeof(saddr)); 
	saddr.sin_family = AF_INET; 
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	saddr.sin_port = htons(PORT); 

	printf("3. connect to server\n");
	if (connect(sfd, (struct sockaddr *)&saddr, sizeof(saddr)) != 0) { 
	  printf("connection with the server failed...\n"); 
	  exit(0); 
	} 


	printf("********  processing loop  *********\n");
	while (1){
		
		getcwd(current_dir, MAX);
		printf("\nclient: cwd:%s\n",current_dir);

		printf("input a line : ");
		bzero(line, MAX);               
		fgets(line, MAX, stdin);         

	
		if(!strcmp(line,"exit\n"))
			exit(0);  
		line[strlen(line)-1] = 0;        
		if (line[0]==0)                 
			exit(0);

		
		n = write(sfd, line, MAX);
		printf("client: wrote n=%d bytes; line=(%s)\n", n, line);
		n = read(sfd, ans, MAX);
		printf("client: read  n=%d bytes; echo=(%s)\n",n, ans);
		
		
		char command[MAX] = "";
		char path[MAX] = "";
		char* holder = (char*)malloc(2);
		holder[0]='\0';

		holder = strtok(line," ");
		strcpy(command,holder);
		holder = strtok(NULL,"\0");
		if(holder)
			strcpy(path,holder);
		
		
		if(strcmp(command, "put") == 0){														// put
			clientput(path);
		}
		else if(strcmp(command, "get") == 0){															// get
			clientget(path);
		}
		else if(strcmp(command, "ls") == 0){
			clientls(path);
			n = read(sfd, ans, MAX);
			printf("client: %s\n", ans);
		}
		
		
		
		else if(!strcmp(ans,"local command found")){
			printf("client: server indicated command to be executed locally\n");
			if(!strcmp("menu",command)){								
				printf("client: executing menu\n");
				mymenu();
			}
			else if(!strcmp("lcat",command)){						
				printf("client: executing cat\n");
				mycat(path);
			}
			else if(!strcmp("lls",command)){							
				printf("client: executing ls\n");
				myls(path);
			}
			else if(!strcmp("lcd",command)){							
				printf("client: executing cd\n");
				mycd(path);
			}
			else if(!strcmp("lpwd",command)){						
				printf("client: executing pwd\n");
				mypwd();
			}
			else if(!strcmp("lmkdir",command)){						
				printf("client: executing mkdir\n");
				mymkdir(path);
			}
			else if(!strcmp("lrmdir",command)){						
				printf("client: executing rmdir\n");
				myrmdir(path);
			}
			else if(!strcmp("lrm",command)){							
				printf("client: executing rm\n");
				myrm(path);
			}
			// default case. command not found.
			else{
				printf("client: local command [%s] not found\n",command);
			}
		}
	}
}