#include <thread>
#include <functional>
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

    std::for_each(sessions.begin(),sessions.end(),[](ClientSession& sess)
    {
        if(!sess->session_ptr()->is_shutdown())
        {
            sess->session_ptr()->stop();
        }
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
}

