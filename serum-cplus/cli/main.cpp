
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <memory>
#include <sharedlib/include/Logger.h>
#include <serum/SERUM_Data_session.hpp>


bool go_exit = false;

// Define the function to be called when ctrl-c (SIGINT) is sent to process
void signal_callback_handler(int signum) {
    std::cout << "Caught ctrl+c signal " << signum << std::endl;
    // Terminate program
    go_exit = true;
}

int main(int argc, char **argv) {

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

