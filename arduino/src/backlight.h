/* Backlight controller. */

#ifndef _BACKLIGHT_H
#define _BACKLIGHT_H

class Backlight {
  public:
    Backlight(const int pin);

    void update(float elapsed);

    bool animationEnabled();
    void setAnimation(bool enabled);

    void setBrightness(float brightness);
    float getBrightness();
    float getRealBrightness();

    void sleep(float seconds);

    void flash(int times);

  private:
    int pin;

    float animate_speed;
    bool animate;

    float sleep_remaining;

    float brightness;
    float brightness_target;

    float flash_min_brightness;
    float flash_max_brightness;
    float flash_animate_speed;

    int flashes_remaining;
    float preflash_brightness;
};

#endif

