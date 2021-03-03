#include <iostream>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main(void)
{
  int32_t target_pos = 1000;
  int32_t current_pos = 0;

  const uint16_t delay_max = 0b0001111111111111; // 8191
  const uint16_t delay_min = 0b0000000001111111; // 127

  const uint16_t speedup_steps = 200;
  const uint16_t speeddown_steps = 200;

  static uint16_t last_delay_microsec = delay_max;
  static uint32_t step_counter = 1; // counter should start from 1
  uint8_t stepping_state = 0;
  uint8_t acc = 2;
  uint16_t delay_microsec = delay_max;

  std::vector<uint16_t> C_list;
  std::vector<float> C_inv;
  std::vector<int32_t> t_list;

  while (target_pos != current_pos)
  {
    if (delay_microsec == delay_max)
    {
      step_counter = 1; // counter should start from 1
    }

    int32_t diff = target_pos - current_pos;
    //delay50:1 m/s, delay250:0.2 m/s
    C_list.push_back(delay_microsec);
    C_inv.push_back(1.0 / delay_microsec);
    int32_t sum_of_elems = 0;
    for (auto &n : C_list)
      sum_of_elems += n;
    t_list.push_back(sum_of_elems);

    // update current position
    diff > 0
        ? current_pos++
        : current_pos--;

    // update delay time
    if (abs(diff) - 1 < speeddown_steps)
    {
      // speed down
      delay_microsec = delay_microsec - 2 * delay_microsec / (-4 * abs(diff) + 1);
    }
    else if (step_counter < speedup_steps)
    {
      // speed up
      delay_microsec = delay_microsec - 2 * delay_microsec / (4 * step_counter + 1);
    }
    step_counter++;
  }

  plt::figure();
  plt::plot(t_list, C_inv);
  plt::show();

  return EXIT_SUCCESS;
}