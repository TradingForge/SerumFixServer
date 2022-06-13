#include <functional>
#include <ctime>
#include "SERUM_Order_sandbox_session.hpp"

#include <sharedlib/include/Logger.h>

class OrderTestLogger: public ILogger
{
private:

    typedef std::string string;

public:

    void Info(const char *content, ...) override;
    void Debug(const char *content, ...) override;
    void Error(const char *content, ...) override;
    void Critical(const char *content, ...) override;
    void Warn(const char *content, ...) override;
    void Trace(const char *content, ...) override;

    ~OrderTestLogger() = default;
};

void OrderTestLogger::Info(const char *content, ...) {
    time_t curr_time;
    curr_time = time(NULL);
    tm *tm_local = localtime(&curr_time);
    std::cout  << tm_local->tm_hour << ":" << tm_local->tm_min << ":" << tm_local->tm_sec << " | INFO | " << content << "\n";
}
void OrderTestLogger::Debug(const char *content, ...) {
    time_t curr_time;
    curr_time = time(NULL);
    tm *tm_local = localtime(&curr_time);
    std::cout  << tm_local->tm_hour << ":" << tm_local->tm_min << ":" << tm_local->tm_sec  << " | DEBUG | " << content << "\n";
}
void OrderTestLogger::Error(const char *content, ...) {
    time_t curr_time;
    curr_time = time(NULL);
    tm *tm_local = localtime(&curr_time);
    std::cout  << tm_local->tm_hour << ":" << tm_local->tm_min << ":" << tm_local->tm_sec  << " | ERROR | " << content << "\n";
}
void OrderTestLogger::Warn(const char *content, ...) {
    time_t curr_time;
    curr_time = time(NULL);
    tm *tm_local = localtime(&curr_time);
    std::cout  << tm_local->tm_hour << ":" << tm_local->tm_min << ":" << tm_local->tm_sec << " | WARN | " << content << "\n";
}
void OrderTestLogger::Critical(const char *content, ...) {
    time_t curr_time;
    curr_time = time(NULL);
    tm *tm_local = localtime(&curr_time);
    std::cout  << tm_local->tm_hour << ":" << tm_local->tm_min << ":" << tm_local->tm_sec << " | CRIT | " << content << "\n";
}
void OrderTestLogger::Trace(const char *content, ...) {
    time_t curr_time;
    curr_time = time(NULL);
    tm *tm_local = localtime(&curr_time);
    std::cout  << tm_local->tm_hour << ":" << tm_local->tm_min << ":" << tm_local->tm_sec << " | TRACE | " << content << "\n";
}

SERUM_Order_sandbox_session::SERUM_Order_sandbox_session(const FIX8::F8MetaCntx& ctx,
                    const FIX8::sender_comp_id& sci,
                    FIX8::Persister *persist,
                    FIX8::Logger *slogger,
                    FIX8::Logger *plogger):
        Session(ctx, sci, persist, slogger, plogger),
        FIX8::SERUM_Order::FIX8_SERUM_Order_Router(),
        _logger(new OrderTestLogger)
{
    _logger->Debug((boost::format("OSession | construct ")).str().c_str());
}

SERUM_Order_sandbox_session::~SERUM_Order_sandbox_session()
{
    _logger->Debug((boost::format("OSession | destruct ")).str().c_str());
}

const std::string& SERUM_Order_sandbox_session::sess_id()
{
    return this->get_sid().get_id();
}

// FIX8::Session implementation
bool SERUM_Order_sandbox_session::handle_application(const unsigned seqnum, const FIX8::Message *&msg)
{
    _logger->Debug((boost::format("OSession | handle_application ")).str().c_str());
    if(enforce(seqnum,msg)){
        _logger->Error("OSession | enforce checking problem ");
        return false;
    }
    try{return msg->process(*this) ;}
    catch(std::exception& ex)
    {
        _logger->Error((boost::format("OSession | handle_application exception (%1%) ") % ex.what()).str().c_str());
        //detach(msg);
        //msg = 0;
        throw;
    }
}

bool SERUM_Order_sandbox_session::handle_logon(const unsigned seqnum, const FIX8::Message *msg)
{
    _logger->Debug((boost::format("OSession | handle_logon ")).str().c_str());
    try {
        _logger->Info((boost::format("OSession | Serum DEX start ")).str().c_str());
        //_client->start();
    }
    catch(std::exception& ex)
    {
        _logger->Error((boost::format("OSession | Serum DEX start exception(%1%)")% ex.what()).str().c_str());
    }
    return FIX8::Session::handle_logon(seqnum, msg);
}

bool SERUM_Order_sandbox_session::handle_logout(const unsigned seqnum, const FIX8::Message *msg)
{
    _logger->Debug((boost::format("OSession | handle_logout ")).str().c_str());
    try {
        _logger->Info((boost::format("OSession | Serum DEX stop ")).str().c_str());
        //_client->stop();
        //while(_client->isConnected())
        //    std::this_thread::sleep_for(std::chrono::milliseconds(200));
        this->stop();
        // _control |= shutdown;
    }
    catch(std::exception& ex)
    {
        _logger->Error((boost::format("OSession | Serum DEX stop exception(%1%)")% ex.what()).str().c_str());
    }
    return FIX8::Session::handle_logout(seqnum, msg);
}

void SERUM_Order_sandbox_session::modify_outbound(FIX8::Message *msg)
{
    FIX8::Session::modify_outbound(msg);
}

bool SERUM_Order_sandbox_session::process(const FIX8::f8String& from)
{
    if(from.find("35=0") == -1) {
        // for all except heartbeat
        _logger->Info((boost::format("OSession | <-- %1%") % from).str().c_str());
    }
    return FIX8::Session::process(from);
}

FIX8::Message *SERUM_Order_sandbox_session::generate_logon(const unsigned heartbeat_interval, const FIX8::f8String davi)
{
    FIX8::Message* logon = FIX8::Session::generate_logon(heartbeat_interval, davi);
    return logon;
}

// FIX8::SERUM_Data::FIX8_SERUM_Data_Router implementation
bool SERUM_Order_sandbox_session::operator() (const class FIX8::SERUM_Order::NewOrderSingle *msg) const
{
    return false;
}


bool SERUM_Order_sandbox_session::operator() (const class FIX8::SERUM_Order::OrderCancelRequest *msg) const
{
    return false;
}

// IBrokerApplication implementation
void SERUM_Order_sandbox_session::onEvent (const std::string &exchangeName, IBrokerClient::BrokerEvent, const std::string &details)
{

}

void SERUM_Order_sandbox_session::onReport(const std::string& exchangeName, const std::string &symbol, const marketlib::execution_report_t&)
{

}

// FIX response implementation
void SERUM_Order_sandbox_session::sendExecutionReport(const marketlib::execution_report_t& report)
{

}

