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

int n;

char ans[MAX];
char line[MAX];

char* local_commands[64] = {"menu","lcat","lls","lcd","lpwd","lmkdir","lrmdir","lrm",NULL};
char* server_commands[64] = {"get","put","ls","cd","pwd","mkdir","rmdir","rm",NULL};

char virtual_root[MAX];

// put
int myput(char* path, int cfd){
	// If path is empty, put cannot execute so it fails.
	if(!strcmp(path,"")){
		printf("server: filepath empty, put failed.\n");
		write(cfd,"fail put",MAX);
		return 0;
	}
	
	// **Parse and change current dir to path if able.
	char old_cwd[MAX];
	getcwd(old_cwd,MAX);
	char current_dir[MAX] = "";
	strcpy(current_dir,old_cwd);

	// If not abs, append path onto cwd and exec from there. Otherwise, just use path.
	if(path[0]!=(char)47){
		strcat(current_dir,"/");
		strcat(current_dir,path);
	}
	else{
		strcpy(current_dir,virtual_root);
		strcat(current_dir,path);
	}

	int fd = open(current_dir, O_RDONLY);			// open file in read mode
	if(fd < 0){
		printf("server: Failed to open file on path %s. put failed\n",path);
		write(cfd,"fail put",MAX);
		return 0;
	}
	struct stat filestats;
	fstat(fd, &filestats);							// generate file stats
	int size = filestats.st_size;					// get the file size
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
		n=read(fd,	holder,MAX);				// send data to client
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
	// Check ans to make sure the file was opened on the server side for writing. If not
	// break here and continue loop.

	write(cfd,"OK",MAX);
	
	// This sequence gets the file name at the end of the path string.
	char holder[MAX] = "";
	strcpy(holder,path);
	char* holder2 = strtok(path,"/");
	while(holder2){
		strcpy(holder,holder2);
		holder2 = strtok(NULL,"/");
	}
	
	// Open a file in cwd with the name specified by the end of the path for writing		
	int fd = open(holder, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	int size = 0, bytes_received = 0;
	printf("open %s for WRITE : filename=%s\n", holder, holder);
	
	n = read(cfd, ans, MAX);
	size = atoi(ans);
	
	printf("read from client, write to file\n");
	printf("size : %d\n", size);
	
	// Read byte chunks from the server and write them to the file. Write until all chunks have
	// been sent.
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

	char buf[MAX]; 							// buffer to read data into

	// if path is empty, set directory equal to cwd
	if(!strcmp("",path)){
		printf("server: ls -l starting at cwd\n");
		getcwd(current_dir,MAX);
		dir_pointer = opendir(current_dir);
	} 
	// else, set to path
	else{
		// if the path can't be opened, return.
		if((dir_pointer = opendir(path))==NULL){
			printf("server: Failed to list contents of %s\n",path);
			return 0;
		}
		printf("server: ls -l starting at %s\n",path);
		strcpy(current_dir,path);
	}
	
	write(cfd,"OK",MAX);
	
	// print contents until no more can be found
	while((current_dir_pointer = readdir(dir_pointer)) != NULL){
		char line[MAX] = "";						// stores line that is sent to client
		
		//update abs_file_name to meet stat() requirements
		strcpy(abs_file_name,current_dir);
		strcat(abs_file_name,"/");
		strcat(abs_file_name,current_dir_pointer->d_name);

		// If read the name successfully, print in ls -l format
		if(stat(abs_file_name,&file_attribute_accessor)!=-1){
			//print out permission data
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

			//print file size in bytes
			sprintf(line+strlen(line),"%8li ", file_attribute_accessor.st_size);

			//update timeinfo and print
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
  // Else, attempt to change to inputted directory. If it doesn't exist, cwd doesn't change and fail is printed.
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
   // Get cwd and store it in current_dir[]
   getcwd(current_dir, MAX);
    
   // Display current working directory before each command
   printf("cwd:%s\n",current_dir);

   // return when done
   return 1;
      
}
// makes directory based on path. if empty, makes in cwd. if not, attempts to make in path
int mymkdir(char* path, int cfd){

  // If path is empty, mkdir cannot execute so it fails.
  if(!strcmp(path,"")){
      printf("server: filepath empty, mkdir failed.\n");
      return 0;
  }

  // **Parse and change current dir to path if able. Then create the directory in the path if able
  char old_cwd[MAX];
  getcwd(old_cwd,MAX);
  char current_dir[MAX] = "";
  strcpy(current_dir,old_cwd);

  // If not abs, append path onto cwd and exec from there. Otherwise, just use path.
  if(path[0]!=(char)47){
    strcat(current_dir,"/");
    strcat(current_dir,path);
  }
  else{
    strcpy(current_dir,virtual_root);
    strcat(current_dir,path);
  }

  // new directory to be made is stored in path, so we attempt to make a directory in path
  if(!mkdir(current_dir, S_IRUSR|S_IWUSR|S_IXUSR)){
    printf("server: Created directory in specified path\n");

    //print out directory the new directory was made in
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
// removes a directory following the path. if empty, fails. if not, follows path
int myrmdir(char* path, int cfd){

  // If path is empty, mkdir cannot execute so it fails.
  if(!strcmp(path,"")){
      printf("server: filepath empty, rmdir failed.\n");
      return 0;
  }

  // **Parse and change current dir to path if able. Then create the directory in the path if able
  char old_cwd[MAX];
  getcwd(old_cwd,MAX);
  char current_dir[MAX] = "";
  strcpy(current_dir,old_cwd);

  // If not abs, append path onto cwd and exec from there. Otherwise, just use path.
  if(path[0]!=(char)47){
    strcat(current_dir,"/");
    strcat(current_dir,path);
  }
  else{
    strcpy(current_dir,virtual_root);
    strcat(current_dir,path);
  }

  // new directory to be made is stored in path, so we attempt to make a directory in path
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
	
	// **Parse and change current dir to path if able. Then rm the directory in the path if able
	char old_cwd[MAX];
	getcwd(old_cwd,MAX);
	char current_dir[MAX] = "";
	strcpy(current_dir,old_cwd);

	// If not abs, append path onto cwd and exec from there. Otherwise, just use path.
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
		
		//create null-terminated string for directory name
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

int main() 
{ 
	int sfd, cfd, len; 
	struct sockaddr_in saddr, caddr; 
	int i, length;

	// Switch variable used for determining if the command should be executed localled or remotely (in reference to client).
	int is_local_command = 0;

	printf("1. create a socket\n");
	sfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sfd < 0) { 
		printf("socket creation failed\n"); 
		exit(0); 
	}

	printf("2. fill in server IP and port number\n");
	bzero(&saddr, sizeof(saddr)); 
	saddr.sin_family = AF_INET; 
	//saddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Local host ID Adress 
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

		// Try to accept a client connection as descriptor newsock
		length = sizeof(caddr);
		cfd = accept(sfd, (struct sockaddr *)&caddr, &length);
		if (cfd < 0){
			printf("server: accept error\n");
			exit(1);
		}

		printf("server: accepted a client connection from\n");
		printf("-----------------------------------------------\n");
		printf("    IP=%s  port=%d\n", "127.0.0.1", ntohs(caddr.sin_port));
		printf("-----------------------------------------------\n");

		// Set virtual root
		getcwd(virtual_root, MAX);
		
		// Processing loop
		while(1){
			// Reset is_local_command at beginning of each loop.
			is_local_command = 0;
		
			// Get input from client.
			printf("server ready for next request ....\n");
			n = read(cfd, line, MAX);

			// If the client is no longer connected, disconnect and close.
			if (n==0){
			  printf("server: client died, server loops\n");
			  close(cfd);
			  break;
			}

			// show the read line from the client.
			printf("server: read  n=%d bytes; line=[%s]\n", n, line);

			// Process command name. If the command is for the server side, execute there. Otherwise, return line indicating that execution needs to occur on client side. If neither apply, echo command and state the command wasn't recognized.
			char* command_holder = strtok(line," ");
			
			for(int i = 0;local_commands[i]!=NULL;i++){
				// If we find that the command is a local command, we mark the is_local_command switch and print
				// "local command found" back to the client, indicating that the client should execute the command
				// on their side.
				if(!strcmp(local_commands[i],command_holder)){
					n = write(cfd,"local command found",MAX);
					printf("server: wrote n=%d bytes; [local command found]\nserver: command sent back to be executed locally\n",n);
					is_local_command = 1;
					break;
				}
			}


			// If the command isn't local, process it for the server
			if(!is_local_command){
				// create arrays for tokenizing line into commands
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
				// default case. command not found.
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
	}
}