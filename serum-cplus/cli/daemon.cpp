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

static void daemon();

int main(int argc, char **argv) {
    daemon();

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

    closelog();
}
   
static void daemon()
{
    pid_t pid;
    
    /* Fork off the parent process */
    pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);
    if (setsid() < 0) {
        printf("Could not generate session ID for child process");
        exit(EXIT_FAILURE);
    }

    umask(0);    
    chdir("./");

    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close (x);
    }

    openlog ("FixServerLog", LOG_PID, LOG_DAEMON);
}