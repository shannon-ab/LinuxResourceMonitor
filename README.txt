HOW TO EXECUTE THE PROGRAM:

There are several acceptable command line arguments:
	--system
 		to indicate that only the system usage should be generated
	--user
		to indicate that only the users usage should be generated
	--graphics
		 to include graphical output in the cases where a graphical outcome is possible as indicated below.
	--samples=N
		if used the value N will indicate how many times the statistics are going to be collected and results will be average
		and reported based on the N number of repetitions.
		default value is 10
	--tdelay=T
		 to indicate how frequently to sample in seconds.
		default value is 1
(the last two may be given in the form of positional arguments in the order: samples, tdelay) N and T are integers

In this program, we assume that if there are positional arguments, they will be the second-last and last arguments,
and if there is only one positional argument, it is assumed to be the number of samples.
samples=10 and tdelay=1 are the default values.

GRAPHICAL OUTPUT INTERPRETATION:
FOR MEMORY USAGE
    ::::::@  total relative negative change
    ######*  total relative positive change
    |*    zero+
    |@    zero-
    |o    zero

    where each : and # represent a 0.01 GB change in memory usage, negative and positive respectively

    negative argument indicates that it is the first sample, difference in memory usage must be zero |o
FOR CPU USAGE
    The graphical representation indicates the percentage of the cpu usage, where "|" represents a 1% usage,
    and "|||" is the base.


KNOWN ISSUES:
	- terminal must be at full size, or must be able to fit all components, otherwise the printing will get shuffled,
	  since I am working with escape codes, particularly saving and restoring cursor positions, which is relative to
	  the screen
	- floating point rounding may cause undesired rounding in 2 decimal places numbers in memory usage, and may cause
	  -0.00 values

OVERVIEW OF FUNCTIONS:

int check_user_flag(int argc, char *argv[])
/*
    Iterates through argv and returns 1 if user flag is present, returns 0 otherwise.                 
*/


int check_system_flag(int argc, char *argv[])
/*
    Iterates through argv and returns 1 if system flag is present, returns 0 otherwise.                 
*/


int check_graphics_flag(int argc, char *argv[])
/*
    Iterates through argv and returns 1 if graphics flag is present, returns 0 otherwise.                 
*/


int check_if_digit(char *s){
/*
    Iterates through characters in s and returns 1 if all characters are digits,
    returns 0 otherwise.             
*/


int check_samples_flag(int argc, char *argv[])
/*
    Iterates through argv and returns an integer representing the number of samples,
    indicated after "--samples=" if samples flag is present, returns 0 otherwise.              
*/


int check_tdelay_flag(int argc, char *argv[])
/*
    Iterates through argv and returns an integer representing the frequency of sampling in seconds,
    indicated after "--tdelay=" if tdelay flag is present, returns 0 otherwise.              
*/


int check_positional_arg(int argc, char *argv[])
/*
    Iterates through argv and returns integer 2 if last two elements are numbers with no flags,
    indicating the samples and tdelay respectively, returns integer 1 if only the last element 
    is a number with no flag, and returns 0 otherwise.       
*/


void print_memory()
/*
    Prints the utilization of memory, by accessing the sysinfo struct, obtained from importing the <sys/sysinfo.h>
    library, and extracting the totalram, freeram, totalswap, and freeswap, given in bytes, and calculating the 
    corresponding memory usage at that time,

    using the following formulas:

    phytot = s.totalram/GIGA;
    phys_used = (totalram-freeram)/GIGA;
    virtualtot = (totalram + totalswap)/GIGA;
    virtualused = virtualtot - (freeram + freeswap)/GIGA;

    where GIGA is a constant used to convert bytes to gigabytes.

    Print is in following format:
    printf("%0.2f GB / %0.2f GB  -- %0.2f GB / %0.2f GB\n", phys_used, phy_tot, virtual_used, virtual_tot);  
*/


double print_memory_graphics(double last_used)
/*
    Prints the utilization of memory, by accessing the sysinfo struct, obtained from importing the <sys/sysinfo.h>
    library, and extracting the totalram, freeram, totalswap, and freeswap, given in bytes, and calculating the 
    corresponding memory usage at that time.

    Prints the difference in memory usage from last sample taken, which is provided as an argument to the function,
    difference is represented by the following symbols:

    ::::::@  total relative negative change
    ######*  total relative positive change
    |*    zero+
    |@    zero-
    |o    zero

    where each : and # represent a 0.01GB change in memory usage, negative and positive respectively

    negative argument indicates that it is the first sample, difference in memory usage must be zero |o

    returns a double representing the memory usage of the current sample.
*/


void print_cpu_usage()
/*
    Prints the utilization of the CPU, by accessing the /proc/stat file, reading the first line, and
    summing all the values in that line except for the first one to find the total time. The time spent
    being idle is the fourth element of that line. Cpu usage is given by 1-(idle/total) multiplied by 100
    
    Prints in following format:
    printf("total cpu use = %0.2Lf%%\n", cpu_usage);

*/


double print_cpu_usage_graphics()
/*
    Prints the utilization of the CPU, along with its graphical representation, by accessing the 
    /proc/stat file, reading the first line, and summing all the values in that line except for 
    the first one to find the total time. The time spent being idle is the fourth element of that 
    line. Cpu usage is given by 1-(idle/total) multiplied by 100.

    The graphical representation indicates the percentage of the cpu usage, where "|" represents a 1% usage,
    and "|||" is the base.

*/


int print_users()
/*
    Prints users connected at a given time, and how many sessions each user is connected to,
    by accessing the utmp struct, obtained from importing the <utmp.h> library. Compares ut_type
    with constant USER_PROCESS to check if it is a normal user and not a system user

    returns an integer value representing the total number of users connected.

*/


void print_system_info(){
/*
    Prints system information, by extracting information from the utsname struct,
    obtained by importing the <sys/utsname.h> library, and accessing the sysname,
    nodename, version, release, machine, for the corresponding system name, machine
    name, version, release and architecture of the system respectively.

*/


void print_main_header(int samples, int tdelay){
    //Prints top main header. 


void print_line(){
    //Prints dotted-line separators 


void print_memory_header(){
    //Prints memory header


void print_users_header(){
    //Prints user header.


void print_user_info(int samples, int tdelay)
/*
    Loops according to the number of samples and the tdelay,
    clears screen on each loop using escape code and prints the
    list of users and how many sessions each user is connected to.
*/   


void print_system(int samples, int tdelay)
/*
    Loops according to the number of samples and the tdelay,
    prints the memory usage and total cpu usage, along with
    the system information.
*/   


void print_user_system(int samples, int tdelay)
/*
    Loops according to the number of samples and the tdelay,
    prints the memory usage, user information, and total cpu usage, 
    along with the system information.
*/       


void print_user_system_graphics(int samples, int tdelay)
/*
    Loops according to the number of samples and the tdelay, prints 
    the memory usage along with the graphics, user information, and 
    total cpu usage, along with the cpu usage graphics, and the 
    system information.
*/   


void print_system_graphics(int samples, int tdelay)
/*
    Loops according to the number of samples and the tdelay, prints 
    the memory usage along with the graphics, and total cpu usage, 
    along with the cpu usage graphics, and the system information.
*/   


void check_samples_tdelay(int *samples, int *tdelay, int argc, char *argv[])
/*
    Arguments passed into functions are pointers to the int value of samples, tdelay,
    and the argc and argv arguments. Function changes values of samples and tdelay if
    properly indicated by user, uses functions check_samples_flag, check_tdelay_flag,
    and check_positional_arg
*/


OVERVIEW OF THE PROGRAM'S ALGORITHM:

In main, we first clear the screen, assign default values of samples and tdelay, and set variables is_user, is_system,
is_graphics, to the corresponding values from functions check_user_flag, check_system_flag, check_graphics_flag respectively.

Checking for flags are separated into their own corresponding functions to conform to the open closed principle, which ensures
that the code is open for extension, but closed for modification, meaning that if I were to add another acceptable command line,
I would only have to add another function to check for the flag, and not modify existing checking functions, which prevents errors.

Now that the is_user, is_system, and is_graphics variables have their corresponding values of 1s or 0s returned from the functions,
we can move on to checking for samples and tdelay flags, which is done by the check_samples_tdelay function, which takes pointers
to the samples and tdelay values, argc and argv as arguments. It changes the samples and tdelay values accordingly if the user has
used the appropriate flags/positional arguments.

For this program, system info is always printed, for both --user and --system

Now we conduct several if statements to print the desired output according to the flags, namely is_users, is_graphics, is_system.
the first if checks if the user flag is raised, and this will only print the list of users and how many sessions each user is connected
to, according to the samples and tdelay that we have derived earlier. We do not need to check for the graphics flag since it will not 
make a difference in the case of a list of users. this if statement will result in calling the print_user_info function, which takes
samples and tdelay as arguments, and prints the output accordingly, by using escape codes to clear the screen.

Next is the case where the system flag is present, and both user and graphics flag are NOT present. In this case, the print_system
function will be called, which takes samples and tdelay as arguments, and prints the output accordingly using escape codes(refer to
docstring and comments of function to see specifics on how this data is printed). it prints the memory usage and cpu usage and updates 
memory usage and total cpu usage accordingly using escape codes to move the cursor accordingly, and prints the system information at the end.

Next is the case where system and user flags are present, but graphics flag is not, OR when none of the flags are present. In this program, 
if user and system are both present, both information are printed. Since the program prints the user information, memory usage, cpu usage
when NO flags are present, this also deals with the case where the user inputs invalid flags, where the program still prints user and system 
information, with no graphics, using the print_user_system function (refer to docstring and comments of function to see specifics on how this data is printed)
This prints the memory usage, cpu usage, users, and uses escape codes to update memory usage and total cpu usage.

Next is analogous to the previous one, but with the graphics flag present. so if ONLY the graphics flag is provided, the program will output
user information, cpu usage, and memory usage, along with the graphics. and if all flags are provided, the user and system information will be 
printed, along with the graphics as well, using the print_user_system_graphics function(refer to docstring and comments of function to see specifics on how 
this data is printed). This prints memory usage, cpu usage, users, and uses escape codes to update memory usage and cpu usage accordingly by positioning
the cursor, including the graphical representations.

The last case deals with the case where only the system and graphics flag is present, and the user flag is NOT. This will simply print the memory usage,
and cpu usage, printing simultaneously, along with the system information. This is done in the print_system_graphics function(refer to docstring and comments
of function to see specifics on how this data is printed)
