#include <iostream>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main(void)
{
  int32_t target_pos = 1000;
  int32_t current_pos = 0;

  const uint16_t delay_max = 400; // 8191
  const uint16_t delay_min = 50;  // 127

  static uint16_t last_delay_microsec = delay_max;
  static uint32_t step_counter = 1; // counter should start from 1
  uint8_t stepping_state = 0;

  std::vector<uint16_t> delta_t_list;
  std::vector<float> V1_list;
  std::vector<float> V2_list;
  std::vector<float> V_max_list;
  std::vector<float> t_list;

  uint16_t speedup_cnt = 0;
  uint16_t speeddown_cnt = 0;

  float c = 0.6000;
  float factor = 2.8e3;
  uint16_t delta_t;
  while (target_pos != current_pos)
  {
    int32_t diff = target_pos - current_pos;
    delta_t = factor * c;
    //delay50:1 m/s, delay250:0.2 m/s

    delta_t_list.push_back(delta_t);
    V1_list.push_back(1.0 / delta_t);
    V_max_list.push_back(1.0 / delay_min);
    uint32_t sum_of_elems = 0;
    for (auto &n : delta_t_list)
      sum_of_elems += n;
    t_list.push_back(sum_of_elems);

    // update current position
    diff > 0
        ? current_pos++
        : current_pos--;

    // speed up
    if (delta_t > delay_min)
    {
      speedup_cnt++;
      c = c - 2.0 * c / (4.0 * step_counter + 1.0);
    }

    step_counter++;
  }
  std::cout << speedup_cnt << std::endl;
  float v_max = 1.0 / delay_min;

  plt::plot(t_list, V1_list, "o-b");
  plt::plot(t_list, V_max_list);
  plt::show();

  return EXIT_SUCCESS;
}