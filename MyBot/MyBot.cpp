#include "MyBot.h"
#include <dpp/dpp.h>
#include "NumberGenerator.h"
#include <string>

/* Be sure to place your token in the line below.
 * Follow steps here to get a token:
 * https://dpp.dev/creating-a-bot-application.html
 * When you invite the bot, be sure to invite it with the 
 * scopes 'bot' and 'applications.commands', e.g.
 * https://discord.com/oauth2/authorize?client_id=940762342495518720&scope=bot+applications.commands&permissions=139586816064
 */
const std::string    BOT_TOKEN    = "MTA3NzAxOTkwMDYxNjM5NjkyMA.GXJRsg.L1eOwFDAzYgqjhYgIHOPZbK-C2UiwmB80mgdJI";

//invite code = https://discord.com/api/oauth2/authorize?client_id=1077019900616396920&permissions=8&scope=bot%20applications.commands


int main()
{
    //instantiate and seed mt1997
    NumberGenerator* mt = new NumberGenerator;

    /* Create bot cluster */
    dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);

    /* Output simple log messages to stdout */
    bot.on_log(dpp::utility::cout_logger());


    /* Handle slash command */
    bot.on_slashcommand([&mt](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "r") {

            std::string input = std::get<std::string>(event.get_parameter("roll"));
            std::istringstream input_ss(input);
            std::string substr;
            getline(input_ss, substr, 'd');
            int num = 0;
            for (int i = 0; i < substr.size(); i++) {
                num *= 10;
                if (isdigit(substr[i])) num += substr[i] - '0';
            }
            getline(input_ss, substr);
            int sides = 0;
            for (int i = 0; i < substr.size(); i++) {
                sides *= 10;
                if (isdigit(substr[i])) sides += substr[i] - '0';
            }
            std::vector<int> rolls = mt->generateNumbers(num, sides);
            int result = 0;
            input += "  { ";
            for (int i = 0; i < rolls.size(); i++) {
                input += std::to_string(rolls[i]);
                if (i < rolls.size()-1) input += +", ";
                result += rolls[i];
            }
            input += " }  Score: " + std::to_string(result);
            event.reply(input);
        }

        if (event.command.get_command_name() == "pi") {
            std::string q = std::get<std::string>(event.get_parameter("query"));
            event.reply(q + ": coming soon!");
        }
    });

    /* Register slash command here in on_ready */
    bot.on_ready([&bot](const dpp::ready_t& event) {
        /* Wrap command registration in run_once to make sure it doesnt run on every full reconnection */
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("r", "Roll dice!", bot.me.id)
                .add_option(dpp::command_option(dpp::co_string, "roll", "/r XdY", true))
            );
            bot.global_command_create(dpp::slashcommand("pi", "Ask me anything", bot.me.id)
                .add_option(dpp::command_option(dpp::co_string, "query", "I know everything", true))
            );
            //bot.global_command_delete(1077155338593243196);
        }
    });


    bot.on_message_create([&bot](const dpp::message_create_t& event) {
        std::string message = event.msg.content;
        size_t n = message.size();

        if (message.find("<@1077019900616396920>", 0) != std::string::npos) {
            event.reply("I am unable to respond but I am listening :]");

        }
        if (message.substr(message.size()-2, 2) == "er") {
            std::istringstream message_ss(message);
            std::string substr;

            while (getline(message_ss, substr, ' ')) {}

            event.reply(substr + "? I barely know her!");
        }
    });

    /* Start the bot */
    bot.start(dpp::st_wait);

    delete mt;

    return 0;
}