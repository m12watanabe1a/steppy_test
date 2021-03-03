#include <iostream>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main(void)
{
  int32_t target_pos = 10000;
  int32_t current_pos = 0;

  const uint16_t delay_max = 8000; // 8191
  const uint16_t delay_min = 100;  // 127

  const uint16_t speedup_steps = 500;
  const uint16_t speeddown_steps = 500;

  static uint16_t last_delay_microsec = delay_max;
  static uint32_t step_counter = 1; // counter should start from 1
  uint8_t stepping_state = 0;
  uint8_t acc = 4;
  uint16_t delay_microsec = delay_max;

  std::vector<uint16_t> C_list;
  std::vector<float> C_inv;
  std::vector<int32_t> t_list;

  uint16_t speedup_cnt = 0;
  uint16_t speeddown_cnt = 0;

  while (target_pos != current_pos)
  {
    if (delay_microsec >= delay_max)
    {
      step_counter = 1; // counter should start from 1
    }

    int32_t diff = target_pos - current_pos;
    //delay50:1 m/s, delay250:0.2 m/s
    C_list.push_back(delay_microsec / acc);
    C_inv.push_back(1.0 / delay_microsec *acc);
    int32_t sum_of_elems = 0;
    for (auto &n : C_list)
      sum_of_elems += n;
    t_list.push_back(sum_of_elems);

    // update current position
    diff > 0
        ? current_pos++
        : current_pos--;

    // update delay time
    if (abs(diff) < speeddown_steps)
    {
      // speed down
      speeddown_cnt++;
      delay_microsec = delay_microsec - 2 * delay_microsec / (-4 * abs(diff) + 1);
    }
    else if (step_counter < speedup_steps)
    {
      // speed up
      speedup_cnt++;
      delay_microsec = delay_microsec - 2 * delay_microsec / (4 * step_counter + 1);
    }
    step_counter++;
  }
  std::cout << speedup_cnt << std::endl;
  std::cout << speeddown_cnt << std::endl;
  std::cout << step_counter << std::endl;
  float v_max = 1.0 / delay_min;

  plt::figure();
  plt::grid(true);
  // plt::xlim(0.0, 0.5e6);
  plt::plot(t_list, C_inv);
  std::vector<int32_t> t = {t_list.front(), t_list.back()};
  std::vector<float> v_max_vec = {v_max, v_max};
  plt::plot(t, v_max_vec);
  plt::show();

  return EXIT_SUCCESS;
}