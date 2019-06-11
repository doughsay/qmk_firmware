#include <print.h>

RGB_MATRIX_EFFECT(doughsplash)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool doughsplash(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);

  HSV hsv = { 0, rgb_matrix_config.sat, 0 };
  uint8_t count = g_last_hit_tracker.count;
  uint8_t start = 0; // multi
  // uint8_t start = qsub8(g_last_hit_tracker.count, 1); // single

  for (uint8_t i = led_min; i < led_max; i++) {
    RGB_MATRIX_TEST_LED_FLAGS();

    hsv.h = rgb_matrix_config.hue;
    hsv.v = rgb_matrix_config.val;

    for (uint8_t j = start; j < count; j++) {
      uint16_t tick = scale16by8(g_last_hit_tracker.tick[j], rgb_matrix_config.speed);

      if (tick > 255)
        continue;

      int16_t dx = g_led_config.point[i].x - g_last_hit_tracker.x[j];
      int16_t dy = g_led_config.point[i].y - g_last_hit_tracker.y[j];
      uint8_t dist = sqrt16(dx * dx + dy * dy);


      // effect
      uint16_t effect = tick + dist * 5;

      if (effect > 255)
        effect = 255;
      effect = 255 - effect;

      hsv.v = qsub8(hsv.v, effect);
      // end effect

      // if (i == 79) {
      //     dprintf("dist: %d, tick: %d, effect: %d\n", dist, tick, effect);
      // }
    }

    hsv.v = scale8(hsv.v, rgb_matrix_config.val);

    RGB rgb1 = hsv_to_rgb(hsv);
    hsv.h += rgb_matrix_config.speed;
    RGB rgb2 = hsv_to_rgb(hsv);

    if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_MODIFIER)) {
      rgb_matrix_set_color(i, rgb2.r, rgb2.g, rgb2.b);
    } else {
      rgb_matrix_set_color(i, rgb1.r, rgb1.g, rgb1.b);
    }
  }

  return led_max < DRIVER_LED_TOTAL;
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
