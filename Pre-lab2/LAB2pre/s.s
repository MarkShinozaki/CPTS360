          .global  running, scheduler, tswitch
tswitch:
SAVE:	  pushl %eax
	  pushl %ebx
	  pushl %ecx
	  pushl %edx
	  pushl %ebp
	  pushl %esi
	  pushl %edi
          pushfl

          movl   running,%ebx 
          movl   %esp,4(%ebx)

FIND:	  call   scheduler

RESUME:	  movl   running,%ebx
          movl   4(%ebx),%esp

          popfl
	  popl  %edi
          popl  %esi
          popl  %ebp
          popl  %edx
          popl  %ecx
          popl  %ebx
          popl  %eax
	
          ret

# stack contents = |retPC|eax|ebx|ecx|edx|ebp|esi|edi|eflag|
#                    -1   -2  -3  -4  -5  -6  -7  -8   -9
	
