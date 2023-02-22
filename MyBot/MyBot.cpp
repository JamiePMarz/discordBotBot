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
// my id    = dpp::snowflake id = 137505549125287936;
const dpp::snowflake MY_ID = 137505549125287936;

int main()
{
    NumberGenerator* mt = new NumberGenerator;


    /* Create bot cluster */
    dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);

    /* Output simple log messages to stdout */
    bot.on_log(dpp::utility::cout_logger());


    /* Handle slash command */
    bot.on_slashcommand([&bot, &mt](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "r") {

            std::string input = std::get<std::string>(event.get_parameter("roll"));
			std::string rolls = mt->rollDice(input);
            std::string output = input + ": " + rolls;
            event.reply(output);
        }

        if (event.command.get_command_name() == "pi") {
            if (event.command.member.user_id == MY_ID) {
                std::string saythis = std::get<std::string>(event.get_parameter("query"));
                dpp::snowflake channel_id = event.command.get_channel().id;
                dpp::message msg(channel_id,saythis);
                bot.message_create(msg);
                dpp::message eph(channel_id, "Forward:");
                eph.set_flags(dpp::m_ephemeral);
                event.reply(eph);
                return;
            }
            std::string q = std::get<std::string>(event.get_parameter("query"));
            event.reply(q + ": coming soon!");
        }
    });

    /* Register slash command here in on_ready */
    bot.on_ready([&bot](const dpp::ready_t& event) {

		std::string bootMsg = 
            "Booting up PI systems...\n"
            "Loading data models...\n"
            "Systems ready to receive input.\n\n"

            "Hello there! How may I assist you today? :]\n";

        std::string ready =
            "...Loading Systems...\n"
            "...Functions Ready...\n"
            "Hello there! How may I be of assistance? :]";

        dpp::message msg(1077109197331759166, ready);
        bot.message_create(msg);
		
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
            std::string sub;

            while (getline(message_ss, sub, ' ')) {}
            
            if (sub != "her") {
                sub = sub.substr(0, sub.size()-2);
                event.reply(sub + "-ER? I barely know HER!");
            }

        }
    });


    /* Start the bot */
    bot.start(dpp::st_wait);

    delete mt;

    return 0;
}
