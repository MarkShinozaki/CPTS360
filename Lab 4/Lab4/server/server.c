// Mark Shinozaki - 11672355
// LAB 4 

// INCLUDES 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h> // Access to POSIX operating system API
#include <netdb.h> // Defines a servent structure/Network Database operations 
#include <arpa/inet.h> // definitions for internet operations 
#include <netinet/in.h> // Internet Address family  
#include <sys/socket.h> // 32bit - main sockets header

#include <libgen.h> // lists definitions used for string pattern matching 
#include <pwd.h>
#include <grp.h> // provides funcs & data structs for working group info 
#include <sys/stat.h> // provides funcs & data structs for file system info (file permission, types, sizes) 
#include <fcntl.h> // file descriptors, opening, closing and manipulating files 
#include <dirent.h> // funcs for directories & directory entires 
#include <libgen.h> // funcs for file names ans paths (extacting directories and base name from path)
#include <time.h> // funcs for date info, time formating and manipulation 



/************ socket address structure ************
struct sockaddr_in {
   sa_family_t    sin_family;   // AF_INET for TCP/IP
   in_port_t      sin_port;     // port number
   struct in_addr sin_addr;     // IP address
};
***************************************************/



char *serverIP = "127.0.0.1";  // local host IP address
//char *serverIP = "172.19.44.43";  // by ifconfig
#define PORT 1234              // hardcoded port number
#define MAX 256
int n;
char line[MAX], ans[MAX];
char* local_commands[64] = {"menu","lcat","lls","lcd","lpwd","lmkdir","lrmdir","lrm",NULL};
char* server_commands[64] = {"get","put","ls","cd","pwd","mkdir","rmdir","rm",NULL};
char virtual_root[MAX];



//////////////////////////////////////////////////////////////////////
/*********************************************************************/
//////////////////////////////////////////////////////////////////////



// FUNCS

// put
int myput(char* path, int cfd){
	// If path is empty, put cannot execute so it fails.
	if(!strcmp(path,"")){
		printf("server: filepath empty, put failed.\n");
		write(cfd,"fail put",MAX);
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
		strcpy(current_dir,virtual_root);
		strcat(current_dir,path);
	}

  // open file in read mode
	int fd = open(current_dir, O_RDONLY);			
	if(fd < 0){
		printf("server: Failed to open file on path %s. put failed\n",path);
		write(cfd,"fail put",MAX);
		return 0;
	}

	struct stat filestats;
	fstat(fd, &filestats);							
	int size = filestats.st_size;					
	char sizestr[MAX] = "";
	sprintf(sizestr,"%d",size);
	
	printf("stat %s stat OK: size=%d\n", path, size);
	printf("send OK %d to client\n", size);
	write(cfd,"OK",MAX);
	write(cfd,sizestr,MAX);
	printf("open %s for READ\n", path);
	
	int bytes_sent = 0;
	
	while(bytes_sent < size){	
		char holder[MAX+1] = "";
		n=read(fd,	holder,MAX);				
		if(!write(cfd, holder, MAX))
			break;
		bytes_sent += n;
		printf("n=%d total=%d\n",n,bytes_sent);
	}
	close(fd);
	if(bytes_sent == size){							// tell the client we done here
		printf("sent %d bytes\n", bytes_sent);
		return 1;
	}
	else{
		printf("server: error, file partially sent, %d/%d bytes.\n", n, size);
		return 0;
	}

}
// get
int myget(char* path, int cfd){
	write(cfd,"OK",MAX);
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
	n = read(cfd, ans, MAX);
	size = atoi(ans);
	
	printf("read from client, write to file\n");
	printf("size : %d\n", size);
	
	while(bytes_received < size){
		n = read(cfd,ans,MAX);									//read stream to string
		if((bytes_received+n) > size){
			n -= (bytes_received+n)-size;
		}
		bytes_received += n;
		printf("n=%d total=%d\n", n, bytes_received);
		write(fd,ans,n);										//write string to file
		
	}
	printf("%d bytes received\n", bytes_received);
	close(fd);
	return 1;
}
// lists local directory contents
int myls(char* path, int cfd){
	// Directory access pointers
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
		printf("server: ls -l starting at cwd\n");
		getcwd(current_dir,MAX);
		dir_pointer = opendir(current_dir);
	} 
	// else, set to path
	else{
		
		if((dir_pointer = opendir(path))==NULL){
			printf("server: Failed to list contents of %s\n",path);
			return 0;
		}
		printf("server: ls -l starting at %s\n",path);
		strcpy(current_dir,path);
	}
	
	write(cfd,"ok",MAX);
	
	
	while((current_dir_pointer = readdir(dir_pointer)) != NULL){
		char line[MAX] = "";						
		
		strcpy(abs_file_name,current_dir);
		strcat(abs_file_name,"/");
		strcat(abs_file_name,current_dir_pointer->d_name);

		
		if(stat(abs_file_name,&file_attribute_accessor)!= -1){
		

			(S_ISDIR(file_attribute_accessor.st_mode)) ? strcat(line,"d") : strcat(line,"-");
			(file_attribute_accessor.st_mode & S_IRUSR) ? strcat(line,"r") : strcat(line,"-");
			(file_attribute_accessor.st_mode & S_IWUSR) ? strcat(line,"w") : strcat(line,"-");
			(file_attribute_accessor.st_mode & S_IXUSR) ? strcat(line,"x") : strcat(line,"-");
			(file_attribute_accessor.st_mode & S_IRGRP) ? strcat(line,"r") : strcat(line,"-");
			(file_attribute_accessor.st_mode & S_IWGRP) ? strcat(line,"w") : strcat(line,"-");
			(file_attribute_accessor.st_mode & S_IXGRP) ? strcat(line,"x") : strcat(line,"-");
			(file_attribute_accessor.st_mode & S_IROTH) ? strcat(line,"r") : strcat(line,"-");
			(file_attribute_accessor.st_mode & S_IWOTH) ? strcat(line,"w") : strcat(line,"-");
			(file_attribute_accessor.st_mode & S_IXOTH) ? strcat(line,"x") : strcat(line,"-");
			sprintf(line+strlen(line),"  %5li ", file_attribute_accessor.st_nlink);

			//print out group and user
			struct passwd *uid;
			struct group *gid;
			
			// read in uid as a formatted type and print it out.
			uid = getpwuid(file_attribute_accessor.st_uid);
			sprintf(line+strlen(line),"%s ",uid->pw_name);
			gid = getgrgid(file_attribute_accessor.st_gid);
			sprintf(line+strlen(line),"%s ",gid->gr_name);

			//print file size 
			sprintf(line+strlen(line),"%8li ", file_attribute_accessor.st_size);

			//update timeinfo 
			time_information = localtime(&(file_attribute_accessor.st_mtime));
			strftime(time_holder,49,"%b %d %R",time_information);
			sprintf(line+strlen(line),"%s ",time_holder);

			//print file name
			sprintf(line+strlen(line),"%s\n",current_dir_pointer->d_name);
			
			//print the line and send to client
			printf("%s",line);
			write(cfd,line,MAX);
		}    
	}
	// tell the client we are done
	write(cfd,"END LS",MAX);
	// close return when done
	closedir(dir_pointer);
	return 1;
}
// changes directory locally
int mycd(char* path){

  //C Change directory to home if nothing is inputted
  if(!strcmp(path,"")){
    printf("server: Path set to root\n");
    chdir(virtual_root);
    return 1;
  }
  
  else{

    char current_dir[MAX] = "";
    if(path[0]==(char)47){
      strcpy(current_dir,virtual_root);
      strcat(current_dir,path);
  }
  else{
    strcpy(current_dir,path);
  }
    printf("server: changing directory to %s\n",current_dir);
    
    if(chdir(current_dir) == 0){
      printf("server: Successfully changed directories.\n");
      return 1;
    }
    else{
      printf("server: Failed to change directories.\n");
      return 0;
    }  
  }
}
// prints cwd
int mypwd(){

  char current_dir[MAX];
  getcwd(current_dir, MAX);
    
   
  printf("cwd:%s\n",current_dir);

  // return when done
  return 1;
      
}

int mymkdir(char* path, int cfd){

  if(!strcmp(path,"")){
    printf("server: filepath empty, mkdir failed.\n");
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
    strcpy(current_dir,virtual_root);
    strcat(current_dir,path);
  }

  if(!mkdir(current_dir, S_IRUSR|S_IWUSR|S_IXUSR)){
    printf("server: Created directory in specified path\n");

    
    char holder[MAX] = "";
    int last_slash=0;
    strcpy(holder,current_dir);
    for(int i = 0;holder[i]!= '\0';i++){
      if(holder[i]==(char)47)last_slash=i;
    }
    holder[last_slash]='\0';
    myls(holder, cfd);
    return 1;
  }
  else{
    printf("server: Failed to create directory in specified path\n");
    return 0;
  }
  
}

int myrmdir(char* path, int cfd){
  if(!strcmp(path,"")){
    printf("server: filepath empty, rmdir failed.\n");
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
    strcpy(current_dir,virtual_root);
    strcat(current_dir,path);
  }

  if(!rmdir(current_dir)){
    printf("server: Deleted directory in specified path\n");

    //print out directory the directory was deleted
    char holder[MAX] = "";
    int last_slash=0;
    strcpy(holder,current_dir);
    for(int i = 0;holder[i]!= '\0';i++){
      if(holder[i]==(char)47)last_slash=i;
    }
    holder[last_slash]='\0';
    myls(holder, cfd);
    return 1;
  }
  else{
    printf("server: Failed to delete directory in specified path\n");
    return 0;
  }
}
// unlinks desired file from filesystem
int myrm(char*path, int cfd){
	// If path is empty, rm cannot execute so it fails.
	if(!strcmp(path,"")){
		printf("server: filepath empty, rm failed.\n");
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
		strcpy(current_dir,virtual_root);
		strcat(current_dir,path);
	}
	
	if(!unlink(current_dir)){
		printf("server: Deleted file at specified path\n");
		
		char holder[MAX] = "";
		int last_slash=0;
		strcpy(holder, current_dir);
		for(int i = 0; holder[i] != '\0'; i++){
			if(holder[i] == (char)47)
				last_slash = i;
		}
		holder[last_slash] = '\0';
		myls(holder, cfd);
		return 1;
	}
	else{
		printf("server: Failed to remove file at %s\n", path);
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////
/*********************************************************************/
//////////////////////////////////////////////////////////////////////

// DRIVER
int main() 
{
    int sfd, cfd;              // sockets 
    int n, length;
    int Local_Command = 0; // local or remote command
    struct sockaddr_in saddr;  // server addr struct
    struct sockaddr_in caddr;  // client addr struct 

   
    
   // int me = getpid();
    
    // Creating Socket 
    printf("1. create a socket\n");
    sfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sfd < 0) { 
        printf("socket creation failed\n"); 
        exit(0); 
    }
    
    printf("2. fill in server IP and port number\n");
    bzero(&saddr, sizeof(saddr)); 
    saddr.sin_family = AF_INET; 
    saddr.sin_addr.s_addr = inet_addr(serverIP); 
    saddr.sin_port = htons(PORT);
    
    printf("3. bind socket to server\n");
    if ((bind(sfd, (struct sockaddr *)&saddr, sizeof(saddr))) != 0) { 
        printf("socket bind failed\n"); 
        exit(0); 
    }
      
    // Now server is ready to listen and verification 
    if ((listen(sfd, 5)) != 0) { 
        printf("Listen failed\n"); 
        exit(0); 
    }

    while(1){

      //printf("server %d ready for client to connect\n", me);
      
       // Try to accept a client connection as socket descriptor cfd
       length = sizeof(caddr);
       cfd = accept(sfd, (struct sockaddr *)&caddr, &length);
       if (cfd < 0){
          printf("server: accept error\n");
          exit(1);
       }

      /*
       printf("server %d: accepted a client connection from\n", me);
       printf("-----------------------------------------------\n");
       printf("IP=%s  port=%d\n, ", inet_ntoa(caddr.sin_addr),
                                      ntohs(caddr.sin_port));
       printf("-----------------------------------------------\n");
      */

      printf("server: accepted a client connection from\n");
		  printf("-----------------------------------------------\n");
		  printf("    IP=%s  port=%d\n", "127.0.0.1", ntohs(caddr.sin_port));
		  printf("-----------------------------------------------\n");

      getcwd(virtual_root, MAX); // FIX

      while(1){
// Reset is_local_command at beginning of each loop.
			Local_Command = 0;
	
			printf("server ready for next request ....\n");
			n = read(cfd, line, MAX);

			if (n==0){
			  printf("server: client died, server loops\n");
			  close(cfd);
			  break;
			}

			printf("server: read  n=%d bytes; line=[%s]\n", n, line);
			char* command_holder = strtok(line," ");
			
			for(int i = 0;local_commands[i]!=NULL;i++){
				
				if(!strcmp(local_commands[i],command_holder)){
					n = write(cfd,"local command found",MAX);
					printf("server: wrote n=%d bytes; [local command found]\nserver: command sent back to be executed locally\n",n);
					Local_Command = 1;
					break;
				}
			}

			if(!Local_Command){
				
				char command[MAX] = "";
				strcpy(command,command_holder);
				char path[MAX] = "";
				char* holder = (char*)malloc(2);
				holder[0]='\0';

				char client_send_string[MAX] = "";

				holder = strtok(NULL,"\0");
				if(holder)strcpy(path,holder);

				// put
				if(!strcmp("put",command)){
					printf("server: executing put\n");
					myput(path,cfd);
				}
				// get
				else if(!strcmp("get",command)){
					printf("server: executing get\n");
					myget(path, cfd);
				}
				// ls
				else if(!strcmp("ls",command)){
					printf("server: executing ls\n");
					(myls(path,cfd)) ? strcpy(client_send_string,"Successfully listed files and subdirectories") : strcpy(client_send_string,"Unsuccessfully listed files and subdirectories");
				}
				// cd
				else if(!strcmp("cd",command)){
					printf("server: executing cd\n");
					(mycd(path)) ?strcpy(client_send_string,"Successfully changed directories") : strcpy(client_send_string,"Unsuccessfully changed directories");
				}
				// pwd
				else if(!strcmp("pwd",command)){
					printf("server: executing pwd\n");
					(mypwd()) ? strcpy(client_send_string,"Successfully printed cwd") : strcpy(client_send_string,"Unsuccessfully printed cwd");
				}
				// mkdir
				else if(!strcmp("mkdir",command)){
					printf("server: executing mkdir\n");
					(mymkdir(path,cfd)) ? strcpy(client_send_string,"Successfully made directory on path") : strcpy(client_send_string,"Unsuccessfully made directory on path");
				}
				// rmdir
				else if(!strcmp("rmdir",command)){
					printf("server: executing rmdir\n");
					(myrmdir(path,cfd)) ? strcpy(client_send_string,"Successfully removed directory on path") : strcpy(client_send_string,"Unsuccessfully removed directory on path");
				}
				// rm
				else if(!strcmp("rm",command)){
					printf("server: executing rm\n");
					(myrm(path,cfd)) ? strcpy(client_send_string,"Successfully removed file on path") : strcpy(client_send_string,"Unsuccessfully removed file on path");
				}
				// default case.
				else{
					printf("server: command [%s] not found\n",command);
					strcpy(client_send_string,"ERROR: Command unrecognized");
				}
				// Send data to client
				if(strcmp("put",command)!=0 && strcmp("get",command)!=0){
					n = write(cfd,client_send_string,MAX);

					// Print that data was sent.
					printf("server: wrote n=%d bytes; [%s]\n",n,client_send_string);
				}
			}
		}	 





      /*
       // Processing loop
       while(1){
         printf("server %d: ready for next request ....\n", me);

	      n = read(cfd, line, MAX);
         if (n==0){
           printf("server: client gone, server %d loops\n", me);
           close(cfd);
           break;
         }

         // show the line string
         printf("server %d: read  n=%d bytes; recv=[%s]\n", me, n, line);

         strcat(line, " ECHO");
         // send the echo line to client 
         n = write(cfd, line, MAX);

         printf("server %d: wrote n=%d bytes; send=[%s]\n", me, n, line);
       }
       */
  }
}

