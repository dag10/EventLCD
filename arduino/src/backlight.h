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

  private:
    int pin;
    float animate_speed;
    bool animate;
    float brightness;
    float brightness_target;
};

#endif

