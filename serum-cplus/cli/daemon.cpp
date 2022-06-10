#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <thread>


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

    while(!go_exit)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if(!sessions.empty()) {
            int count = sessions.size();
            sessions.erase(
                    std::remove_if(sessions.begin(), sessions.end(), [](ClientSession &sess) {
                        if (sess->session_ptr()->is_shutdown())
                            printf("Erase session: %s\n", sess->session_ptr()->get_sid().get_id().c_str());
                        return sess->session_ptr()->is_shutdown();
                    }),
                    sessions.end()
            );
            if(count != sessions.size()) {
                printf("Session removed, count= %d\n", (int) sessions.size());
            }
        }
        if (ms->poll())
        {
            std::shared_ptr<FIX8::SessionInstanceBase> inst(ms->create_server_instance());
            sessions.push_back(inst);
            printf("Session added, count= %d\n", (int)sessions.size());
            //inst->session_ptr()->control() |= FIX8::Session::print;
            //FIX8::GlobalLogger::log("global_logger");
            //const FIX8::ProcessModel pm(ms->get_process_model(ms->_ses));
            inst->start(false);
        }
    }

    //  deinitialize all services
    std::for_each(sessions.begin(),sessions.end(),[](ClientSession& sess)
    {
        if(!sess->session_ptr()->is_shutdown())
        {
            sess->session_ptr()->stop();
        }
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(250));

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