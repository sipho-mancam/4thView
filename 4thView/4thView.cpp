#include <iostream>
#include <conio.h>
#include "kevents.hpp"
#include <stdio.h>
#include "processes_manager.hpp"
#include "kafka_admin.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

int main()
{
   
   /**
   * @doc
   * The purpose of this module is being the systems main entry point, it prepares for all the modules
   * to be able to start correctly, ensuring that everything they expect to exist in a certain location, already exists.
   * And also handles the clean-up on system shutdown, killing all the modules
   * 
   * Steps:
   * 1. Initialize kafka topics.
   * 2. Check if the config file exists, if it doesn't dump a default one the the designated config location,
   * 3. Start all the system modules including the gui
   * 4. Monitor system modules for errors and crushes
   * 5. Kill all system modules and clean-up.
   * 6. Starts up apache kafka and kills it through docker
   */

    /*This function takes care of the dumping of the config file.*/
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("4th View Main");
    json globalConfig = KEvents::__load_config__();
    json systemSettings = globalConfig["systemSettings"];
    std::vector<std::string> registeredModules = systemSettings["registeredModules"];
    std::vector<std::string> moduleTopics;
    KafkaTopicCreator topicInit(systemSettings["kafka"]["broker"]);

    std::string localEventBusAddress = systemSettings["kafka"]["broker"];

    KEvents::kEventsLogger->info("Local event bus running at address: {}", localEventBusAddress);

    for (std::string module : registeredModules)
    {
        std::string topic = globalConfig[module]["serviceTopic"];
        logger->info("Registering Topic {}", topic);
        moduleTopics.push_back(topic);
    }
    topicInit.createTopics(moduleTopics);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    ProcessesManager procMan(globalConfig);
    procMan.init();
    logger->info("[info] Manager Shuting down ...");
    return 0;
}
