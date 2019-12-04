#include <stdio.h>
#include <tgbot/tgbot.h>
#include <string>
#include <vector>
#include <ctime>

int main() {
    TgBot::Bot bot("992358909:AAGvhLGMIDVVH0AACJ0Aw2C2a8i0hSA9xsI");
    bool configuration_calendar = false;


    std::vector<std::string> all_days;
    all_days.push_back("monday");
    all_days.push_back("tuesday");
    all_days.push_back("wednesday");
    all_days.push_back("thursday");
    all_days.push_back("friday");
    all_days.push_back("saturday");

    std::vector<std::string> all_days_italian;
    all_days_italian.push_back("Lunedi");
    all_days_italian.push_back("Martedi");
    all_days_italian.push_back("Mercoledi");
    all_days_italian.push_back("Giovedi");
    all_days_italian.push_back("Venerdi");
    all_days_italian.push_back("Sabato");

    std::vector<std::string> all_days_short;
    all_days_short.push_back("Mon");
    all_days_short.push_back("Tue");
    all_days_short.push_back("Wed");
    all_days_short.push_back("Thu");
    all_days_short.push_back("Fri");
    all_days_short.push_back("Sat");

    bot.getEvents().onCommand("start", [&bot,&all_days](TgBot::Message::Ptr message) {
        std::vector<std::string> all_users;
        // Apertura del file con il save degli username
        std::ifstream file("saves/accounts/chat_id/id.dat");
        
        // Inizializzazione della variabile
        std::string str; 
        // Aggiunzione di tutte le righe nel vector
        while (std::getline(file, str)) {
            all_users.push_back(str);
        }
        file.close();

        std::string number_to_string2 = std::to_string(message->chat->id);
        if (std::find(all_users.begin(), all_users.end(), number_to_string2) != all_users.end())
        {
            bot.getApi().sendMessage(message->chat->id, "Your account is already registered in the system.");
        }

        else{
            std::ofstream outfile;
            outfile.open("saves/accounts/chat_id/id.dat", std::ios::app);
            outfile << message->chat->id << "\n";
            outfile.close();
            outfile.open("saves/accounts/user_privileges/privileges.dat", std::ios::app);
            outfile << "user" << "\n";
            outfile.close();
            for (int i = 0; i < all_days.size(); i++){
                std::string number_to_string = std::to_string(message->chat->id);
                std::string filename_user = "saves/days/"+all_days[i]+"/"+number_to_string+".dat";
                std::ofstream file { filename_user };

                std::string filename_user2 = "saves/configuration/days/"+number_to_string+".dat";
                std::ofstream file2 { filename_user2 };

                outfile.open(filename_user2);
                outfile << "0";
                outfile.close();
            }
      		bot.getApi().sendMessage(message->chat->id, "Welcome to the School Management Bot.");
        	bot.getApi().sendMessage(message->chat->id, "This bot offers you a wide range of tools to manage your school life.");
            bot.getApi().sendMessage(message->chat->id, "To start configuring the calendar, type /settings.");
        }

    });
    
    bot.getEvents().onCommand("settings", [&bot,&configuration_calendar,&all_days,&all_days_italian](TgBot::Message::Ptr message) {
        std::string number_to_string = std::to_string(message->chat->id);
        std::vector<std::string> configuration_step;

        std::string filename_vector = "saves/configuration/days/"+number_to_string+".dat";
        std::ifstream file(filename_vector);

        std::string str; 

        while (std::getline(file, str)) {
            configuration_step.push_back(str);
        }
        file.close();

        int config_step_to_int = std::stoi( configuration_step[0] );

        std::string message_to_display = "Digita le materie per "+all_days_italian[config_step_to_int];
        bot.getApi().sendMessage(message->chat->id, message_to_display);
        bot.getApi().sendMessage(message->chat->id, "Quando hai finito digita /end e passerai al prossimo giorno.");
        
        configuration_calendar = true;
    });

    bot.getEvents().onCommand("today", [&bot,&all_days_short,&all_days](TgBot::Message::Ptr message) {
        time_t now = time(0);
        char* dt = ctime(&now);
        std::string the_day(dt);
        std::string day_of_the_week = the_day.substr(0, the_day.find(" "));

        std::vector<std::string>::iterator itr = std::find(all_days_short.begin(), all_days_short.end(), day_of_the_week);
        int day_position = std::distance(all_days_short.begin(), itr);

        std::string number_to_string = std::to_string(message->chat->id); 

        std::vector<std::string> all_calendar;
        std::string filename_user = "saves/days/"+all_days[day_position]+"/"+number_to_string+".dat";
        std::ifstream file(filename_user);

        //const char *text
        std::string str((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
        file.close();
        
        bot.getApi().sendMessage(message->chat->id, str);
    });

    // Admin commands
    bot.getEvents().onCommand("see_users_number", [&bot](TgBot::Message::Ptr message) {
        std::vector<std::string> all_users;
        // Apertura del file con il save degli username
        std::ifstream file("saves/accounts/chat_id/id.dat");
        
        // Inizializzazione della variabile
        std::string str; 
        // Aggiunzione di tutte le righe nel vector
        while (std::getline(file, str)) {
            all_users.push_back(str);
        }
        file.close();

        std::string number_to_string = std::to_string(message->chat->id); 

        std::vector<std::string>::iterator itr = std::find(all_users.begin(), all_users.end(), number_to_string);
        int user_position = std::distance(all_users.begin(), itr);


        std::ifstream file2("saves/accounts/user_privileges/privileges.dat");
        
        // Inizializzazione della variabile
        std::string str2; 
        // Aggiunzione di tutte le righe nel vector
        std::vector<std::string> all_users_privileges;
        while (std::getline(file2, str2)) {
            all_users_privileges.push_back(str2);
        }
        file2.close();

        if (all_users_privileges[user_position] == "root"){
        	std::string convert_vector = std::to_string(all_users.size());
        	convert_vector = "Users registered in the system: " + convert_vector;
        	bot.getApi().sendMessage(message->chat->id, convert_vector);
        }
        else if (all_users_privileges[user_position] == "admin"){
        	std::string convert_vector = std::to_string(all_users.size());
        	convert_vector = "Users registered in the system: " + convert_vector;
        	bot.getApi().sendMessage(message->chat->id, convert_vector);        	
        }
        else{
        	bot.getApi().sendMessage(message->chat->id, "Comando non riconosciuto");
        }	
    });

    bot.getEvents().onAnyMessage([&bot,&configuration_calendar,&all_days,&all_days_italian](TgBot::Message::Ptr message) {
        printf("User wrote %s\n", message->text.c_str());
        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }
        if (configuration_calendar == false){
            //bot.getApi().sendMessage(message->chat->id, "Il tuo messaggio: " + message->text);
        }
        if (StringTools::startsWith(message->text, "/end")){
            if (configuration_calendar == true){
                std::string number_to_string = std::to_string(message->chat->id);
                std::vector<std::string> configuration_step;

                std::string filename_vector = "saves/configuration/days/"+number_to_string+".dat";
                std::ifstream file(filename_vector);

                std::string str; 

                while (std::getline(file, str)) {
                    configuration_step.push_back(str);
                }
                file.close();

                int config_step_to_int = std::stoi( configuration_step[0] );

                std::ofstream outfile;

                outfile.open(filename_vector);
                int new_value = config_step_to_int + 1;
                outfile << new_value;
                outfile.close();
            }    
        } 
        if (StringTools::startsWith(message->text, "/end-config")){
            if (configuration_calendar == true){
                configuration_calendar = false;
                bot.getApi().sendMessage(message->chat->id, "Configurazione completata.");
            }    
        }          
        if (configuration_calendar == true){
            if (message->text=="/end"){
                std::string number_to_string = std::to_string(message->chat->id);
                std::vector<std::string> configuration_step;

                std::string filename_vector = "saves/configuration/days/"+number_to_string+".dat";
                std::ifstream file(filename_vector);

                std::string str; 

                while (std::getline(file, str)) {
                    configuration_step.push_back(str);
                }
                file.close();

                int config_step_to_int = std::stoi( configuration_step[0] );

                if (config_step_to_int>5){
                    std::string number_to_string = std::to_string(message->chat->id);
                    std::vector<std::string> configuration_step;

                    std::string filename_vector = "saves/configuration/days/"+number_to_string+".dat";
                    std::ifstream file(filename_vector);

                    std::ofstream outfile;
                    outfile.open(filename_vector);
                    outfile << 0;
                    outfile.close();

                    bot.getApi().sendMessage(message->chat->id, "Configurazione Completata.");
                    configuration_calendar = false;
                }
                else{
                    std::string message_to_display = "Digita le materie per "+all_days_italian[config_step_to_int];
                    bot.getApi().sendMessage(message->chat->id, message_to_display);
                    bot.getApi().sendMessage(message->chat->id, "Quando hai finito digita /end e passerai al prossimo giorno.");
                }
            }
            else{
                std::ofstream outfile;

                std::string number_to_string = std::to_string(message->chat->id);

                std::vector<std::string> configuration_step;

                std::string filename_vector = "saves/configuration/days/"+number_to_string+".dat";
                std::ifstream file(filename_vector);

                std::string str; 

                while (std::getline(file, str)) {
                    configuration_step.push_back(str);
                }
                file.close();

                int config_step_to_int = std::stoi( configuration_step[0] );

                std::string filename_to_save = "saves/days/"+all_days[config_step_to_int]+"/"+number_to_string+".dat";
                outfile.open(filename_to_save, std::ios::app);
                outfile << message->text << "\n";
                bot.getApi().sendMessage(message->chat->id, "Materia aggiunta con successo.");
            }

        }  
    });
    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}