// 1.cpp
#include <glog/logging.h>
#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#define LOGDIR "/home/vslam/learning/glog_test/log"
#define MKDIR "mkdir -p " LOGDIR

//将信息输出到单独的文件和 LOG(ERROR)
void waiter(const char *data, int size)
{
    std::string str = std::string(data, size);
    std::ofstream fs(LOGDIR "glog_dump.log", std::ios::app);
    fs << str;
    fs.close();
    LOG(ERROR) << str;
}

bool glogInit()
{
    system(MKDIR);
    google::InitGoogleLogging("");
#define DEBUG_MODE
#ifdef DEBUG_MODE
    google::SetStderrLogging(google::GLOG_INFO); //设置级别高于 GOOGLE_NAMESPACE::INFO 的日志同时输出到屏幕
#else
    GOOGLE_NAMESPACE::SetStderrLogging(GOOGLE_NAMESPACE::GLOG_FATAL); //设置级别高于 GOOGLE_NAMESPACE::FATAL 的日志同时输出到屏幕
#endif
    FLAGS_colorlogtostderr = true; //设置输出到屏幕的日志显示相应颜色
    //FLAGS_servitysinglelog = true;// 用来按照等级区分log文件</span>
    google::SetLogDestination(google::GLOG_FATAL, LOGDIR "/FATAL_");     // 设置 GOOGLE_NAMESPACE::FATAL 级别的日志存储路径和文件名前缀
    google::SetLogDestination(google::GLOG_ERROR, LOGDIR "/ERROR_");     //设置 GOOGLE_NAMESPACE::ERROR 级别的日志存储路径和文件名前缀
    google::SetLogDestination(google::GLOG_WARNING, LOGDIR "/WARNING_"); //设置 GOOGLE_NAMESPACE::WARNING 级别的日志存储路径和文件名前缀
    google::SetLogDestination(google::GLOG_INFO, LOGDIR "/INFO_");       //设置 GOOGLE_NAMESPACE::INFO 级别的日志存储路径和文件名前缀
    FLAGS_logbufsecs = 0;                                                //缓冲日志输出，默认为30秒，此处改为立即输出
    FLAGS_max_log_size = 5;                                              //最大日志大小为 5MB
    FLAGS_stop_logging_if_full_disk = true;                              //当磁盘被写满时，停止日志输出
    //GOOGLE_NAMESPACE::SetLogFilenameExtension("91_"); //设置文件名扩展，如平台？或其它需要区分的信息
    google::InstallFailureSignalHandler(); //捕捉 core dumped (linux)
    google::InstallFailureWriter(waiter);  //默认捕捉 SIGSEGV 信号信息输出会输出到 stderr

    return true;
}

int main(int argc, char *argv[])
{
    //google::InitGoogleLogging(argv[0]);
    glogInit();

    //google::SetLogDestination(google::INFO, "log/"); // 把日志同时记录文件，最低级别为INFO

    // 通过GFLAGS来设置参数，更多选项可以在logging.cc里面查询
    // 日志等级分为INFO, WARNING, ERROR, FATAL,如果是FATAL级别这直接运行报错
    FLAGS_stderrthreshold = google::WARNING;
    FLAGS_colorlogtostderr = true;

    FLAGS_log_dir = "./log";

    LOG(WARNING) << "Hello GLOG";
    // 条件输出
    LOG_IF(INFO, 20 > 10) << "20 > 10";
    // 频率输出，本质上是通过google::COUNTER这个计数机实现的
    for (int i = 0; i < 10; i++)
    {
        LOG_EVERY_N(ERROR, 3) << "每隔3次输出一次 " << i;
        LOG_FIRST_N(ERROR, 2) << "前两次输出 " << i;
        LOG_IF_EVERY_N(WARNING, 1 < 2, 2) << "条件输出+频率" << google::COUNTER;
    }
    google::ShutdownGoogleLogging();
}
