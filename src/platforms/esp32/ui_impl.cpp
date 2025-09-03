#include "ui_impl.hpp"
#include <Adafruit_NeoPixel.h>
#include "Arduino.h"

#define RESET_BUTTON 4  // GPIO4
#define PLAY_BUTTON 5   // GPIO5
#define RGB_LED 48
#define NUM_LEDS 1

Adafruit_NeoPixel strip =
    Adafruit_NeoPixel(NUM_LEDS, RGB_LED, NEO_GRB + NEO_KHZ800);

const char* soundByteNames[100] = {
    "knockingonheavensdoor-godverdomme",
    "dp-and_so_begins_the_tale",
    "castlevania-intro",
    "ll-chicken",
    "bd-devine_intervention",
    "werner-jaja",
    "badtaste-suckmyspinng",
    "pf-size_of_that_gun",
    "sn-rede_ich_chinesisch",
    "megaman-start",
    "bb-i_am_the_one_who_knocks",
    "tm-giel_seiten_bank",
    "supermariokart-new_record",
    "bd-no_pudding",
    "misc-badum",
    "ww-garth_multi_schwing",
    "bb-great_cornholio_no_bunghole",
    "5th-asis_light",
    "sopranos-money_still_green",
    "spaceballs-you_went_over_my_helmet",
    "sw-lightsaber1",
    "jurassicpark-you_did_not_say_the_magic_word",
    "sw-father",
    "unrealturnament-killingspree",
    "bsth-sonst_qualmts",
    "ab-blood_reaching_brain",
    "metalgear-codec",
    "bb-keys",
    "sw-evacuate_in_our_moment_of_triumph",
    "bf-think_mc_fly",
    "overwatch-junkrat_brrrrring",
    "misc-slip",
    "misc-it_was_at_this_moment_that_he_knew",
    "hannibal-bowels_in_or_bowels_out_mikkelsen",
    "quake-flagcapture",
    "overwatch-s76_this_old_dog_learned_a_few_tricks",
    "sn-hallo_sie",
    "gv-arschlaecher",
    "apgm-trillions",
    "sopranos-could_have_bought_a_ferrari",
    "bd-bloody_sicko",
    "itcrowd-its_the_ateam",
    "5th-red_button",
    "supermariokart-tournament_win",
    "misc-risiken_und_nebenwirkungen",
    "misc-haaptsaach_eng_gaenz",
    "misc-say_whaaat",
    "freefire-pizza_delivery_service",
    "fright-i_do_not_like_you",
    "scarface-say_hello",
    "300-dine_in_hell",
    "bd-death_surrounds_him",
    "startrek-phaser_ship",
    "kk-mach_doch_deinen_scheiss",
    "aod-groovy",
    "5th-you_are_fired",
    "apsycho-no_wedding_cant_take_time_off_work",
    "ca-je_temerde",
    "ca-chier_toujours",
    "fmj-modern_art_masterpiece",
    "sn-gefickt_eingeschaedelt",
    "tg-in_a_land_before_time",
    "bl-moron",
    "aod-cryhavoc",
    "sfii-dhalsim_yoga_fire",
    "kg-volksfreund_abo",
    "supermariokart-goal",
    "ca-bon_film_ca",
    "quake-armor",
    "kg-wot",
    "sim-flanders",
    "misc-fut_oopsidaisy",
    "gameofthrones-break_the_wheel",
    "supermarioworld-course_clear",
    "bd-taking_a_piss",
    "supermariokart-choose_your_driver",
    "supermariokart-invincible",
    "bd-the_pipes",
    "kk-merde",
    "dp-dying_a_meaningful_death",
    "os-yahh",
    "badtaste-derricksrun",
    "startrek-intruder_alert",
    "os-mystapler",
    "pf-shutup_fatman",
    "ww-long_schwing",
    "sopranos-you_must_have_been_top_of_the_class",
    "gv-four_crematoire",
    "pf-kane_kung_fu",
    "wickedquake1-doc_12",
    "apsycho-head_on_a_stick",
    "sdw-kommst_du_mit_oder_nit",
    "supermarioworld-game_over",
    "darksouls-you_died",
    "aod-killkiss",
    "bf-libyans",
    "ll-intro",
    "wickedquake1-badblood_01",
    "icq-startup",
    "badtaste-goodone"};

/**
 * Blink the RGB LED several times in red without blocking
 */
void _blink_strip() {
    static unsigned long lastBlinkTime = 0;
    static int blinkState = 0;
    static int blinkCount = 0;

    unsigned long currentTime = millis();

    if (blinkCount < 3) {
        if (blinkState == 0 && currentTime - lastBlinkTime >= 200) {
            strip.setPixelColor(0, strip.Color(255, 0, 0));  // Red
            strip.show();
            lastBlinkTime = currentTime;
            blinkState = 1;
        } else if (blinkState == 1 && currentTime - lastBlinkTime >= 200) {
            strip.setPixelColor(0, strip.Color(0, 0, 0));  // Off
            strip.show();
            lastBlinkTime = currentTime;
            blinkState = 0;
            blinkCount++;
        }
    } else {
        blinkCount = 0;  // Reset for the next call
    }
}

void Esp32Ui::setup() {
    pinMode(RESET_BUTTON, INPUT_PULLUP);
    pinMode(PLAY_BUTTON, INPUT_PULLUP);
    pinMode(RGB_LED, OUTPUT);
    strip.begin();
    strip.show();
    strip.setBrightness(50);
    randomSeed(analogRead(0));  // Seed the random number generator
}

void Esp32Ui::tick() {
    char buffer[16];
    sprintf(buffer, "%d", digitalRead(RESET_BUTTON));
    if (digitalRead(RESET_BUTTON) == LOW) {
        config.clear();
        _blink_strip();
    }
    if (digitalRead(PLAY_BUTTON) == LOW) {
        logger.info("Play button pressed");
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        randomSeed(
            tv.tv_usec);  // Seed the random number generator with microseconds
        int randomIndex =
            random(0, 200);  // Adjusted range to match soundByteNames array
        audio.play((String("https://base-url/") + soundByteNames[randomIndex])
                       .c_str());
    }
}
