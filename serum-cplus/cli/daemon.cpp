#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>


#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <memory>
#include <sharedlib/include/Logger.h>
#include <serum/SERUM_Data_session.hpp>

using namespace std;
bool go_exit = false;

// Define the function to be called when ctrl-c (SIGINT) is sent to process
void signal_callback_handler(int signum) {
    std::cout << "Caught ctrl+c signal " << signum << std::endl;
    // Terminate program
    go_exit = true;
}

static void skeleton_daemon();

int main(int argc, char **argv) {
    skeleton_daemon();

    printf("Hello FixServer\n");

    std::string conf_file = "hf_server.xml";
    std::unique_ptr<FIX8::ServerSessionBase> ms(
            new FIX8::ServerSession<SERUM_Data_session>(FIX8::SERUM_Data::ctx(), conf_file, "SERUM_MD"));

    typedef std::shared_ptr<FIX8::SessionInstanceBase> ClientSession;
    std::vector<ClientSession> sessions;
   // sessions[0]->session_ptr()->shutdown();
    while(!go_exit) {
        if (!ms->poll())
        {
         /*   sessions.erase(
        std::remove_if(sessions.begin(),sessions.end(),[](ClientSession& sess)
                {
                    return sess->session_ptr()->is_shutdown();
                })
            )*/;
            continue;
        }
        std::shared_ptr<FIX8::SessionInstanceBase> inst(ms->create_server_instance());
        sessions.push_back(inst);
        //inst->session_ptr()->control() |= FIX8::Session::print;
        //FIX8::GlobalLogger::log("global_logger");
        const FIX8::ProcessModel pm(ms->get_process_model(ms->_ses));
        inst->start(true);
    }

    std::for_each(sessions.begin(),sessions.end(),[](ClientSession& sess)
    {
        if(!sess->session_ptr()->is_shutdown())
        {
            sess->session_ptr()->stop();
        }
    });

}
   
static void skeleton_daemon()
{
    pid_t pid;
    
    /* Fork off the parent process */
    pid = fork();
    
    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);
    
     /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);
    
    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);
    
    /* Catch, ignore and handle signals */
    /*TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    
    /* Fork off for the second time*/
    pid = fork();
    
    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);
    
    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);
    
    /* Set new file permissions */
    umask(0);
    
    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir("/");
    
    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close (x);
    }
    
    /* Open the log file */
    openlog ("firstdaemon", LOG_PID, LOG_DAEMON);
}