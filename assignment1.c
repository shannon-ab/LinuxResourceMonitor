#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include<sys/sysinfo.h>
#include<sys/utsname.h>
#include<utmp.h>
#include<unistd.h>

int check_user_flag(int argc, char *argv[]){
/*
    Iterates through argv and returns 1 if user flag is present, returns 0 otherwise.                 
*/
    for (int i = 1; i < argc; i++){
        if(strcmp(argv[i], "--user") == 0){
            return 1;
        }

    }
    return 0;
}

int check_system_flag(int argc, char *argv[]){
/*
    Iterates through argv and returns 1 if system flag is present, returns 0 otherwise.                 
*/
    for (int i = 1; i < argc; i++){
        if(strcmp(argv[i], "--system") == 0){
            return 1;
        }

    }
    return 0;
}

int check_graphics_flag(int argc, char *argv[]){
/*
    Iterates through argv and returns 1 if graphics flag is present, returns 0 otherwise.                 
*/
    for (int i = 1; i < argc; i++){
        if(strcmp(argv[i], "--graphics") == 0){
            return 1;
        }

    }
    return 0;
}

int check_if_digit(char *s){
/*
    Iterates through characters in s and returns 1 if all characters are digits,
    returns 0 otherwise.             
*/
    int i = 0;
    while(s[i] != '\0'){
        if(isdigit(s[i]) == 0){
            return 0;
        }

        i++;
    }

    return 1;
}

int check_samples_flag(int argc, char *argv[]){
/*
    Iterates through argv and returns an integer representing the number of samples,
    indicated after "--samples=" if samples flag is present, returns 0 otherwise.              
*/
    char *token;
    int samples;

    for (int i = 1; i < argc; i++){

        if(strncmp(argv[i], "--samples=", 10) == 0){

            token = strtok(argv[i], "=");
            token = strtok(NULL, "=");

            if (check_if_digit(token) == 1){
                return strtol(token, NULL, 10);
            }

        }

    }
    return 0;    
}

int check_tdelay_flag(int argc, char *argv[]){
/*
    Iterates through argv and returns an integer representing the frequency of sampling in seconds,
    indicated after "--tdelay=" if tdelay flag is present, returns 0 otherwise.              
*/
    char *token;
    int tdelay;

    for (int i = 1; i < argc; i++){

        if(strncmp(argv[i], "--tdelay=", 9) == 0){

            token = strtok(argv[i], "=");
            token = strtok(NULL, "=");

            if (check_if_digit(token) == 1){
                return strtol(token, NULL, 10);
            }

        }

    }

    return 0;    
}

int check_positional_arg(int argc, char *argv[]){
/*
    Iterates through argv and returns integer 2 if last two elements are numbers with no flags,
    indicating the samples and tdelay respectively, returns integer 1 if only the last element 
    is a number with no flag, and returns 0 otherwise.       
*/
    if(argc > 1 && check_if_digit(argv[argc-2]) == 1 && check_if_digit(argv[argc-1]) == 1){
        return 2;
    }
    else if(check_if_digit(argv[argc-1]) == 1){
        return 1;
    }

    return 0;
}

void print_memory(){
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
    double phy_tot;
    double phys_used;
    double virtual_tot;
    double virtual_used;
    const double GIGA = 1024*1024*1024;

    struct sysinfo s;
    sysinfo(&s);

    phy_tot = s.totalram/GIGA;
    phys_used = (s.totalram-s.freeram)/GIGA;

    virtual_tot = (s.totalram + s.totalswap)/GIGA;
    virtual_used = virtual_tot - (s.freeram + s.freeswap)/GIGA;

    printf("%0.2f GB / %0.2f GB  -- %0.2f GB / %0.2f GB\n", phys_used, phy_tot, virtual_used, virtual_tot);  

}

double print_memory_graphics(double last_used){
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
    double phy_tot;
    double phys_used;
    double virtual_tot;
    double virtual_used;
    const double GIGA = 1024*1024*1024;
    int change;

    struct sysinfo s;
    sysinfo(&s);

    phy_tot = s.totalram/GIGA;
    phys_used = (s.totalram-s.freeram)/GIGA;

    virtual_tot = (s.totalram + s.totalswap)/GIGA;
    virtual_used = virtual_tot - (s.freeram + s.freeswap)/GIGA;
    
    //first sample, difference in usage is 0
    if (last_used < 0){
        printf("%0.2f GB / %0.2f GB  -- %0.2f GB / %0.2f GB   |o %0.2f (%0.2f)\n", phys_used, phy_tot, virtual_used, virtual_tot, 0.00, phys_used);
        return phys_used;
    }

    printf("%0.2f GB / %0.2f GB  -- %0.2f GB / %0.2f GB   |", phys_used, phy_tot, virtual_used, virtual_tot);
    change = (int)((phys_used - last_used)*100);

    //negative difference
    if(phys_used - last_used < 0){
        //multiply change by -1 to iterate through loop, cannot loop through negative value
        for(int i = 0; i < -change; i++){
            printf(":");
        }
        printf("@ %0.2f (%0.2f)\n", phys_used-last_used, phys_used);
    }
    
    //positive difference
    else if(phys_used - last_used > 0){
        for(int j = 0; j < change; j++){
            printf("#");
        }
        printf("* %0.2f (%0.2f)\n", phys_used-last_used, phys_used);
    }else{
        //0 difference, same usage before and after
        printf("o %0.2f (%0.2f)\n", phys_used-last_used, phys_used);
    }

    return phys_used;
}
void print_cpu_usage(){
/*
    Prints the utilization of the CPU, by accessing the /proc/stat file, reading the first line, and
    summing all the values in that line except for the first one to find the total time. The time spent
    being idle is the fourth element of that line. Cpu usage is given by 1-(idle/total) multiplied by 100
    
    Prints in following format:
    printf("total cpu use = %0.2Lf%%\n", cpu_usage);

*/
    const char delimeter[2] = " ";
    char usage[100];
    char *token;
    int total = 0;
    int col = 0;
    int idle;
    double cpu_usage;

    FILE *f = fopen("/proc/stat", "r");
    fgets(usage, 100, f);
    fclose(f);

    token = strtok(usage, delimeter);

    while(token != NULL){

        token = strtok(NULL, delimeter);

        if (token != NULL){
            total = total + strtol(token, NULL, 10);
        }
        if (col == 3){
            idle = strtol(token, NULL, 10);
        }

        col++;
    }
    cpu_usage = (1-(double)idle/(double)total)*100;
    printf("total cpu use = %0.2f%%\n", cpu_usage);
}

double print_cpu_usage_graphics(){
/*
    Prints the utilization of the CPU, along with its graphical representation, by accessing the 
    /proc/stat file, reading the first line, and summing all the values in that line except for 
    the first one to find the total time. The time spent being idle is the fourth element of that 
    line. Cpu usage is given by 1-(idle/total) multiplied by 100.

    The graphical representation indicates the percentage of the cpu usage, where "|" represents a 1% usage,
    and "|||" is the base.

*/
    const char delimeter[2] = " ";
    char usage[100];
    char *token;
    int total = 0;
    int col = 0;
    int idle;
    int val;
    double cpu_usage;

    FILE *f = fopen("/proc/stat", "r");
    fgets(usage, 100, f);
    fclose(f);

    token = strtok(usage, delimeter);

    while(token != NULL){

        token = strtok(NULL, delimeter);

        if (token != NULL){
            total = total + strtol(token, NULL, 10);
        }
        if (col == 3){
            idle = strtol(token, NULL, 10);
        }

        col++;
    }
    cpu_usage = (1-(double)idle/(double)total)*100;
    
    printf("         |||");

    for(int i = 0; i < (int)(cpu_usage); i++){
        printf("|");
    }

    printf(" %0.2f%%\n", cpu_usage);
    return cpu_usage;
}

int print_users(){
/*
    Prints users connected at a given time, and how many sessions each user is connected to,
    by accessing the utmp struct, obtained from importing the <utmp.h> library. Compares ut_type
    with constant USER_PROCESS to check if it is a normal user and not a system user

    returns an integer value representing the total number of users connected.

*/

    struct utmp *u;
    int num_of_users = 0;

    setutent();
    u = getutent();

    while(u != NULL){

        if(u->ut_type == USER_PROCESS){
            printf("%9s%12s (%s)\n", u->ut_user, u->ut_line, u->ut_host);
            num_of_users++;
        }

        u = getutent();
    }

    return num_of_users;
}

void print_system_info(){
/*
    Prints system information, by extracting information from the utsname struct,
    obtained by importing the <sys/utsname.h> library, and accessing the sysname,
    nodename, version, release, machine, for the corresponding system name, machine
    name, version, release and architecture of the system respectively.

*/   
    struct utsname u;
    uname(&u);
    printf("### System Information ###\n");
    printf("System Name = %s\n", u.sysname);
    printf("Machine Name = %s\n", u.nodename);
    printf("Version = %s\n", u.version);
    printf("Release = %s\n", u.release);
    printf("Architecture = %s\n", u.machine);

}

void print_main_header(int samples, int tdelay){
    //Prints top main header   
    printf("Nbr of samples: %d -- every %d secs\n", samples, tdelay);
}

void print_line(){
    //Prints dotted-line separators 
    printf("---------------------------------------\n");
}

void print_memory_header(){
    //Prints memory header
    printf("### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");
}

void print_users_header(){
    //Prints user header.
    printf("### Sessions/users ###\n");
}

void print_user_info(int samples, int tdelay){
/*
    Loops according to the number of samples and the tdelay,
    clears screen on each loop using escape code and prints the
    list of users and how many sessions each user is connected to.
*/   
    for(int i = 0; i < samples; i++){
        printf("\033c");
        print_main_header(samples, tdelay);
        print_line();
        printf("### Sessions/users ###\n");
        print_users();
        sleep(tdelay);
    }

    print_line();
    print_system_info();
}

void print_system(int samples, int tdelay){
/*
    Loops according to the number of samples and the tdelay,
    prints the memory usage and total cpu usage, along with
    the system information.
*/   
    //printing borders and first sampling values, leaving newline spaces for remaining samples
    print_main_header(samples, tdelay);
    print_line();
    print_memory_header();
    print_memory();
    printf("\033[s");

    for(int j = 0; j < samples-1; j++){
        printf("\n");
    }

    print_line();
    printf("total cpu use = \n");
    print_line();
    
    //tdelay after first loop
    sleep(tdelay);

    //positioning cursor below the first entry for memory usage, restoring saved cursor 
    printf("\033[u");

    //loops through the rest of the samples, while including tdelay 
    for (int i = 1; i < samples; i++){
        //printf("\033[u");
        print_memory();
        sleep(tdelay);
    }
    
    //positions cursor on the total cpu usage line
    printf("\033[%dB", 1);
    print_cpu_usage();
    print_line();
    print_system_info();
    print_line();
}

void print_user_system(int samples, int tdelay){
/*
    Loops according to the number of samples and the tdelay,
    prints the memory usage, user information, and total cpu usage, 
    along with the system information.
*/       
    //printing borders and first sampling values, leaving newline spaces for remaining samples
    print_main_header(samples, tdelay);
    print_line();
    print_memory_header();
    print_memory();
    printf("\033[s");

    for(int j = 0; j < samples-1; j++){
        printf("\n");
    }

    print_line();
    printf("total cpu use = \n");

    print_line();
    print_users_header();
    int num_of_users = print_users();

    print_line();

    //tdelay after first loop
    sleep(tdelay);
    
    //loops through the rest of the samples, while including tdelay
    for (int i = 1; i < samples; i++){
        printf("\033[u");
        print_memory();
        printf("\033[s");
        printf("\033[%dB", samples-i+2);
        print_users_header();
        num_of_users = print_users();
        sleep(tdelay);
    }

    //positions cursor on the total cpu usage line
    printf("\033[%dF", num_of_users+3);
    print_cpu_usage();

    //positions cursor at the bottom line to print system info
    printf("\033[%dE", num_of_users+3);
    print_system_info();
    print_line();
}

void print_user_system_graphics(int samples, int tdelay){
/*
    Loops according to the number of samples and the tdelay, prints 
    the memory usage along with the graphics, user information, and 
    total cpu usage, along with the cpu usage graphics, and the 
    system information.
*/   

    double memory;
    int num_of_users;
    double cpu_usage;

    //printing borders and first sampling values, leaving newline spaces for remaining samples
    print_main_header(samples, tdelay);
    print_line();
    print_memory_header();
    memory = print_memory_graphics(-2);
    printf("\033[s");

    for(int j = 0; j < samples-1; j++){
        printf("\n");
    }
    print_line();

    //printing cpu usage with graphics, leaving newline spaces for remaining samples
    printf("total cpu use = \n");
    print_cpu_usage_graphics();
    for(int k = 0; k < samples-1; k++){
        printf("\n");
    }
    print_line();

    //printing list of users
    printf("### Sessions/users ###\n");
    num_of_users = print_users();
    print_line();

    //tdelay after first loop
    sleep(tdelay);

    //loop through the rest of the samples, while including tdelay
    for (int i = 1; i < samples; i++){

        //restores saved cursor position below first line of memory usage
        printf("\033[u");

        //pass in previous value of memory to print out graphical difference
        memory = print_memory_graphics(memory);
        printf("\033[s");

        //positions cursor to print cpu usage graphics
        printf("\033[%dB", samples+1);
        cpu_usage = print_cpu_usage_graphics();

        //prints newlines for remaining cpu usage samples
        for(int k = 0; k < samples-i-1; k++){
            printf("\n");
        }
        print_line();

        //print user info, which is refreshed along with the sampling
        printf("### Sessions/users ###\n");
        num_of_users = print_users();

        print_line();

        sleep(tdelay);
    }

    //positions cursor at the total cpu usage line
    printf("\033[%dA", 1+num_of_users + 2 + samples +1);
    printf("total cpu use = %0.2f%%", cpu_usage);

    //positions cursor at the bottom line to print system info
    printf("\033[%dE", 1+num_of_users + 2 + samples +1);
    print_system_info();
    print_line();
}

void print_system_graphics(int samples, int tdelay){
/*
    Loops according to the number of samples and the tdelay, prints 
    the memory usage along with the graphics, and total cpu usage, 
    along with the cpu usage graphics, and the system information.
*/   
    double memory;
    double cpu_usage;

    //printing borders and first sampling values, leaving newline spaces for remaining samples
    print_main_header(samples, tdelay);
    print_line();
    print_memory_header();
    memory = print_memory_graphics(-2);
    printf("\033[s");

    for(int j = 0; j < samples-1; j++){
        printf("\n");
    }
    print_line();

    //printing cpu usage with graphics, leaving newline spaces for remaining samples
    printf("total cpu use = \n");
    print_cpu_usage_graphics();
    for(int k = 0; k < samples-1; k++){
        printf("\n");
    }
    print_line();

    //tdelay after first loop
    sleep(tdelay);

    //looping through the rest of the samples, while including tdelay
    for (int i = 1; i < samples; i++){
        printf("\033[u");
        memory = print_memory_graphics(memory);
        printf("\033[s");
        printf("\033[%dB", samples+1);
        cpu_usage = print_cpu_usage_graphics();

        for(int k = 0; k < samples-i-1; k++){
            printf("\n");
        }
        print_line();

        sleep(tdelay);
    }

    //position cursor at the total cpu usage line
    printf("\033[%dA", samples +2);
    printf("total cpu use = %0.2f%%", cpu_usage);

    //position cursor at the bottom line to print system info
    printf("\033[%dE", samples +1);
    print_line();
    print_system_info();
    print_line();
}

void check_samples_tdelay(int *samples, int *tdelay, int argc, char *argv[]){
/*
    Arguments passed into functions are pointers to the int value of samples, tdelay,
    and the argc and argv arguments. Function changes values of samples and tdelay if
    properly indicated by user, uses functions check_samples_flag, check_tdelay_flag,
    and check_positional_arg
*/   
    //if samples flag is present, assign corresponding value given by user to the address
    //pointed to by int pointer samples
    if(check_samples_flag(argc, argv) != 0){
        *samples = check_samples_flag(argc, argv);
    }
    
    //if tdelay flag is present, assign corresponding value given by user to the address
    //pointed to by tdelay int pointer
    if(check_tdelay_flag(argc, argv) != 0){
        *tdelay = check_tdelay_flag(argc, argv);
    }

    //if last two arguments are numbers, assign corresponding values to samples and
    //tdelay respectively
    if(check_positional_arg(argc, argv) == 2){
        *samples = strtol(argv[argc-2], NULL, 10);
        *tdelay = strtol(argv[argc-1], NULL, 10);
    }

    //if only last argument is a number, assign corresponding value to samples
    else if(check_positional_arg(argc, argv) == 1){
        *samples = strtol(argv[argc-1], NULL, 10);
    }
}

int main(int argc, char *argv[]){
    //clear screen
    printf("\033c");

    int samples = 10;
    int tdelay = 1;
    int is_user = check_user_flag(argc, argv);
    int is_system = check_system_flag(argc, argv);
    int is_graphics = check_graphics_flag(argc, argv);

    //changing values of samples and tdelay accordingly if needed
    check_samples_tdelay(&samples, &tdelay, argc, argv);

    //prints info for user only
    if(is_user == 1 && is_system == 0){
        print_user_info(samples, tdelay);
        return 0;
    }

    //prints info for system only
    else if(is_system == 1 && is_user == 0 && is_graphics == 0){
        print_system(samples, tdelay);
        return 0;
    }

    //prints info for both user and system with no graphics
    else if((is_user == 1 && is_system == 1 && is_graphics == 0) || (is_user == 0 && is_system == 0 && is_graphics == 0)){
        print_user_system(samples, tdelay);
        return 0;
    }

    //prints info for both user and system with graphics
    else if((is_user == 1 && is_system == 1 && is_graphics == 1) || (is_user == 0 && is_system == 0 && is_graphics == 1)){
        print_user_system_graphics(samples, tdelay);
        return 0;
    }

    //prints info for system with graphics
    else if(is_system == 1 && is_graphics == 1 && is_user == 0){
        print_system_graphics(samples, tdelay);
        return 0;
    }

}